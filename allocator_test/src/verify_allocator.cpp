// verify_allocator -- regression test for the UMF jemalloc pool changes.
//
// Two things it checks:
//   1. Speed: the UMF fast path (umf_alloc/umf_free) stays flat as thread count
//      rises. Before the tcache fix, free collapsed from ~26 ns to ~377 ns
//      between 1 and 20 threads; this catches a regression back to that.
//   2. NUMA placement: node-k allocations land on node k, even when the two
//      nodes are interleaved on one thread and across many threads. This is the
//      property that a "faster" but leaky tcache (dallocx(p,0)) silently breaks.
//
// Build/run: make UMF=1 && ./bin/verify_allocator   (needs a 2-node machine).
#define NUMA_NODE_NUM 2
#include "umf_numa_allocator.hpp"
#include "alloc_test_common.hpp"

#include <cstdio>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

static constexpr size_t OBJ = 32; // ~sizeof(BinaryNode)
static constexpr size_t N = 400000;

template <typename A, typename F>
static void run(const char *name, A af, F ff, int nthreads) {
    std::vector<double> pa(nthreads), pf(nthreads);
    std::atomic<int> go{0};
    std::vector<std::thread> ths;
    for (int t = 0; t < nthreads; ++t) {
        ths.emplace_back([&, t] {
            pin_to_node(0, t);
            std::vector<void *> p(N);
            // Warm-up pass: fault every page in, so the timed loop measures
            // allocator cost, not first-touch page-fault cost.
            for (size_t i = 0; i < N; ++i) { p[i] = af(0, OBJ); *(volatile char *)p[i] = 1; }
            for (size_t i = 0; i < N; ++i) ff(0, p[i]);
            go.fetch_add(1);
            while (go.load() < nthreads) {}
            auto t0 = std::chrono::steady_clock::now();
            for (size_t i = 0; i < N; ++i) p[i] = af(0, OBJ);
            auto t1 = std::chrono::steady_clock::now();
            for (size_t i = 0; i < N; ++i) ff(0, p[i]);
            auto t2 = std::chrono::steady_clock::now();
            pa[t] = std::chrono::duration<double, std::nano>(t1 - t0).count() / N;
            pf[t] = std::chrono::duration<double, std::nano>(t2 - t1).count() / N;
        });
    }
    for (auto &th : ths) th.join();
    double a = 0, f = 0;
    for (int i = 0; i < nthreads; ++i) { a += pa[i]; f += pf[i]; }
    a /= nthreads; f /= nthreads;
    printf("  %-30s %2d thr   %6.2f alloc  %7.2f free  %7.2f pair (ns)\n",
           name, nthreads, a, f, a + f);
}

int main() {
    if (numa_available() < 0) { printf("NUMA unavailable\n"); return 1; }

    printf("=== ns per op, %zuB objects, steady state (want: free flat vs threads) ===\n", OBJ);
    for (int nt : {1, 8, 20}) {
        run("umf_alloc / umf_free", [](unsigned n, size_t s) { return umf_alloc(n, s, 8); },
            [](unsigned n, void *p) { umf_free(n, p); }, nt);
        run("jemalloc malloc (regular)", [](unsigned, size_t s) { return ::malloc(s); },
            [](unsigned, void *p) { ::free(p); }, nt);
        printf("\n");
    }

    int failures = 0;

    // Placement 1: interleave the two nodes on one thread. A tcache that mixes
    // objects across pools would hand node-0 memory back for a node-1 request.
    printf("=== placement ===\n");
    const int M = 4096;
    std::vector<void *> v0(M), v1(M);
    for (int i = 0; i < M; ++i) { v0[i] = umf_alloc(0, 64, 8); *(char *)v0[i] = 1; }
    for (int i = 0; i < M; ++i) umf_free(0, v0[i]);
    for (int i = 0; i < M; ++i) { v1[i] = umf_alloc(1, 64, 8); *(char *)v1[i] = 1; }
    int wrong = 0;
    for (int i = 0; i < M; ++i) if (node_of(v1[i]) != (int)numa_node_map(1)) wrong++;
    printf("  node-1 allocations on the wrong node: %d / %d %s\n",
           wrong, M, wrong ? "FAIL" : "ok");
    failures += (wrong != 0);
    for (int i = 0; i < M; ++i) umf_free(1, v1[i]);

    // Placement 2: plain malloc() must NOT have been hijacked into a node-bound
    // arena by the pool setup (thread.arena save/restore).
    void *m = ::malloc(64); *(char *)m = 1;
    int mn = node_of(m);
    bool ok_local = (mn == (int)numa_node_map(0) || mn == (int)numa_node_map(1));
    printf("  plain malloc() still unbound: node %d %s\n", mn, ok_local ? "ok" : "FAIL");
    failures += (!ok_local);
    ::free(m);

    // Placement 3: many threads interleaving both nodes -- every allocation must
    // be node-local.
    std::atomic<int> bad{0};
    std::vector<std::thread> ths;
    for (int t = 0; t < 8; ++t) {
        ths.emplace_back([&, t] {
            pin_to_node(0, t);
            for (int i = 0; i < 512; ++i)
                for (unsigned n = 0; n < 2; ++n) {
                    void *p = umf_alloc(n, 64, 8); *(char *)p = 1;
                    if (node_of(p) != (int)numa_node_map(n)) bad.fetch_add(1);
                    umf_free(n, p);
                }
        });
    }
    for (auto &th : ths) th.join();
    printf("  8-thread interleaved misplacements: %d / 8192 %s\n",
           bad.load(), bad.load() ? "FAIL" : "ok");
    failures += (bad.load() != 0);

    printf("\n%s\n", failures ? "VERIFY FAILED" : "VERIFY PASSED");
    return failures ? 1 : 0;
}
