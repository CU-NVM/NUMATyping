// throughput_compare -- how much faster is the UMF node-bound allocator than
// numa_alloc_onnode(), the libnuma call in the #else branch of the generated
// numa<T,k>::operator new (i.e. the naive "regular NUMA" allocation path).
//
// Measures the realistic cycle: allocate a small object, first-touch it (like a
// node constructor), then free it -- batched to bound RSS, pinned to node 0,
// swept over thread counts. numa_alloc_onnode mmaps+mbinds+munmaps every object,
// so it is orders of magnitude slower and does not scale; it gets a much smaller
// op budget here so the sweep finishes quickly.
//
// Build/run: make UMF=1 && ./bin/throughput_compare [umf-only]
#define NUMA_NODE_NUM 2
#include "umf_numa_allocator.hpp"
#include "alloc_test_common.hpp"

#include <cstdio>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

static constexpr size_t OBJ = 32; // ~sizeof(BinaryNode)

// af() -> void*, ff(void*). Each thread does reps*batch alloc+touch+free ops.
template <typename A, typename F>
static double run(const char *name, A af, F ff, int nthreads, int batch, int reps) {
    std::vector<double> pa(nthreads), pf(nthreads);
    std::atomic<int> go{0};
    std::vector<std::thread> ths;
    for (int t = 0; t < nthreads; ++t) {
        ths.emplace_back([&, t] {
            pin_to_node(0, t);
            std::vector<void *> p(batch);
            go.fetch_add(1);
            while (go.load() < nthreads) {}
            double atot = 0, ftot = 0;
            for (int r = 0; r < reps; ++r) {
                auto t0 = std::chrono::steady_clock::now();
                for (int i = 0; i < batch; ++i) { p[i] = af(); *(volatile char *)p[i] = 1; }
                auto t1 = std::chrono::steady_clock::now();
                for (int i = 0; i < batch; ++i) ff(p[i]);
                auto t2 = std::chrono::steady_clock::now();
                atot += std::chrono::duration<double, std::nano>(t1 - t0).count();
                ftot += std::chrono::duration<double, std::nano>(t2 - t1).count();
            }
            pa[t] = atot / ((double)reps * batch);
            pf[t] = ftot / ((double)reps * batch);
        });
    }
    for (auto &th : ths) th.join();
    double a = 0, f = 0;
    for (int i = 0; i < nthreads; ++i) { a += pa[i]; f += pf[i]; }
    a /= nthreads; f /= nthreads;
    printf("  %-22s %2d thr   %9.1f alloc+touch   %8.1f free   %9.1f pair (ns)\n",
           name, nthreads, a, f, a + f);
    return a + f;
}

int main(int argc, char **argv) {
    if (numa_available() < 0) { printf("NUMA unavailable\n"); return 1; }
    bool umf_only = (argc > 1 && strcmp(argv[1], "umf-only") == 0);

    printf("=== alloc(%zuB)+first-touch then free, node 0, ns/op ===\n", OBJ);
    for (int nt : {1, 8, 20}) {
        double numa = 0;
        if (!umf_only)
            // numa_alloc_onnode is ~100-8000x slower, so give it a tiny budget.
            numa = run("numa_alloc_onnode",
                       [] { return numa_alloc_onnode(OBJ, numa_node_map(0)); },
                       [](void *p) { numa_free(p, OBJ); }, nt, 512, 2);
        double umf = run("UMF (umf_alloc)",
                         [] { return umf_alloc(0, OBJ, 8); },
                         [](void *p) { umf_free(0, p); }, nt, 2048, 200);
        if (!umf_only && umf > 0)
            printf("  -> UMF is %.0fx faster than numa_alloc_onnode at %d threads\n",
                   numa / umf, nt);
        printf("\n");
    }
    return 0;
}
