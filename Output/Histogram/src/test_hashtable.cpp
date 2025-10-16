#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <limits>
#include "HashTable.hpp"

// ------------ Minimal test helpers ------------
static int g_failed = 0, g_passed = 0;

#define REQUIRE(cond) do { \
    if (!(cond)) { \
        std::fprintf(stderr, "REQUIRE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        std::fflush(stderr); \
        std::abort(); \
    } else { \
        g_passed++; \
    } \
} while(0)

static void check_eq_int(const char* what, int got, int expect) {
    if (got != expect) {
        std::fprintf(stderr, " %s: got %d, expected %d\n", what, got, expect);
        std::abort();
    } else {
        g_passed++;
    }
}

static void check_true(const char* what, bool cond) {
    if (!cond) {
        std::fprintf(stderr, " %s: condition is false\n", what);
        std::abort();
    } else {
        g_passed++;
    }
}

// ------------ Targeted unit tests ------------

// 1) Basic insert + count semantics
static void test_basic_insert_and_get() {
    HashTable ht(/*buckets=*/17);
    ht.insert("apple");
    ht.insert("banana");
    ht.insert("apple"); // increment existing key
    check_eq_int("count(apple)", ht.getCount("apple"), 2);
    check_eq_int("count(banana)", ht.getCount("banana"), 1);
    check_eq_int("count(missing)", ht.getCount("missing"), 0);
}

// 2) Remove semantics
static void test_remove() {
    HashTable ht(7);
    ht.insert("pear");
    ht.insert("pear");
    ht.insert("peach");
    check_eq_int("pre count(pear)", ht.getCount("pear"), 2);
    check_eq_int("pre count(peach)", ht.getCount("peach"), 1);

    ht.remove("pear"); // should remove the node entirely (spec assumes count node holds frequency)
    check_eq_int("post count(pear) after remove", ht.getCount("pear"), 0);
    check_eq_int("post count(peach) intact", ht.getCount("peach"), 1);

    // Removing a non-existent key should be a no-op
    ht.remove("dragonfruit");
    check_eq_int("post count(peach) still 1", ht.getCount("peach"), 1);
}

// 3) Case sensitivity expectations
static void test_case_sensitivity() {
    HashTable ht(5);
    ht.insert("Apple");
    ht.insert("apple");
    check_eq_int("count(Apple)", ht.getCount("Apple"), 1);
    check_eq_int("count(apple)", ht.getCount("apple"), 1);
    // Expect case-sensitive keys to be distinct.
}

// 4) Empty string handling
static void test_empty_string_key() {
    HashTable ht(3);
    ht.insert("");
    ht.insert("");
    check_eq_int("count(\"\")", ht.getCount(""), 2);
    ht.remove("");
    check_eq_int("count(\"\") after remove", ht.getCount(""), 0);
}

// 5) Very long key handling
static void test_long_key() {
    std::string longk(8192, 'x'); // 8KB key
    HashTable ht(19);
    ht.insert(longk.c_str());
    ht.insert(longk.c_str());
    check_eq_int("count(long key)", ht.getCount(longk.c_str()), 2);
    // Also verify long key does not affect other keys
    ht.insert("short");
    check_eq_int("count(short)", ht.getCount("short"), 1);
}

// 6) Collision + chaining behavior (probabilistic; use tiny bucket count)
static void test_collisions_and_chaining() {
    HashTable ht(2); // force many collisions regardless of hash
    const char* words[] = {"a","b","c","d","e","f","g","h","i","j"};
    for (auto w: words) ht.insert(w);
    for (auto w: words) check_eq_int("count(word)", ht.getCount(w), 1);

    // getAllKeys should return exactly the set of unique keys
    std::vector<char*> keys = ht.getAllKeys();
    // Convert to std::string for easier comparison
    std::vector<std::string> s;
    s.reserve(keys.size());
    for (char* k : keys) s.emplace_back(k);
    std::sort(s.begin(), s.end());
    std::vector<std::string> expected(words, words + sizeof(words)/sizeof(words[0]));
    std::sort(expected.begin(), expected.end());
    REQUIRE(s == expected);
}

// 7) Removing from head/middle/tail of collision chain
static void test_remove_positions_in_chain() {
    HashTable ht(1); // everything collides into a single bucket
    const char* w1 = "alpha";
    const char* w2 = "beta";
    const char* w3 = "gamma";
    ht.insert(w1);
    ht.insert(w2);
    ht.insert(w3);
    // Remove head
    ht.remove(w1);
    check_eq_int("count(alpha) removed", ht.getCount(w1), 0);
    check_eq_int("count(beta)", ht.getCount(w2), 1);
    check_eq_int("count(gamma)", ht.getCount(w3), 1);
    // Remove middle
    ht.remove(w2);
    check_eq_int("count(beta) removed", ht.getCount(w2), 0);
    check_eq_int("count(gamma) intact", ht.getCount(w3), 1);
    // Remove tail
    ht.remove(w3);
    check_eq_int("count(gamma) removed", ht.getCount(w3), 0);
}

// 8) Idempotency and stability: inserting many duplicates
static void test_many_duplicates() {
    HashTable ht(13);
    const char* key = "dup";
    for (int i = 0; i < 1000; ++i) ht.insert(key);
    check_eq_int("count(dup)=1000", ht.getCount(key), 1000);
    ht.remove(key);
    check_eq_int("count(dup)=0 after remove", ht.getCount(key), 0);
}

// 9) Fuzz test vs reference map
static void test_fuzz_against_unordered_map() {
    HashTable ht(97);
    std::unordered_map<std::string,int> ref;

    std::mt19937 rng(123456u);
    std::uniform_int_distribution<int> lenDist(0, 16);
    std::uniform_int_distribution<int> opDist(0, 2); // 0:insert, 1:remove, 2:query
    std::uniform_int_distribution<int> chDist(0, 25);

    auto randWord = [&]{
        int len = lenDist(rng);
        std::string s; s.reserve(len);
        for (int i = 0; i < len; ++i) s.push_back(char('a' + chDist(rng)));
        return s;
    };

    // Preload some words for better coverage
    std::vector<std::string> pool;
    for (int i = 0; i < 200; ++i) pool.push_back(randWord());

    for (int step = 0; step < 5000; ++step) {
        const std::string& w = pool[step % pool.size()];
        int op = opDist(rng);
        switch (op) {
            case 0: // insert
                ht.insert(w.c_str());
                ref[w]++;
                break;
            case 1: // remove
                // Our HashTable::remove removes the node entirely (resets count to 0).
                ht.remove(w.c_str());
                ref[w] = 0;
                break;
            case 2: // query
            default: {
                int got = ht.getCount(w.c_str());
                int expect = ref[w];
                if (expect < 0) expect = 0;
                check_eq_int("fuzz query", got, expect);
                break;
            }
        }
    }

    // Final cross-check: all keys in table must match reference non-zero entries
    auto keys = ht.getAllKeys();
    std::unordered_map<std::string,int> seen;
    for (char* k : keys) {
        int c = ht.getCount(k);
        if (c > 0) seen[std::string(k)] = c;
    }
    for (auto& kv : ref) {
        int expected = std::max(0, kv.second);
        int got = seen.count(kv.first) ? seen[kv.first] : 0;
        if (expected != got) {
            std::fprintf(stderr,
                "❌ mismatch for key '%s': got %d, expected %d\n",
                kv.first.c_str(), got, expected);
            std::abort();
        }
    }
}

// 10) getAllKeys contains exactly unique keys currently present
static void test_getAllKeys_uniqueness() {
    HashTable ht(11);
    ht.insert("x");
    ht.insert("x");
    ht.insert("y");
    ht.insert("z");
    auto keys = ht.getAllKeys();
    std::vector<std::string> ks;
    ks.reserve(keys.size());
    for (char* k : keys) ks.emplace_back(k);
    std::sort(ks.begin(), ks.end());
    std::vector<std::string> expected = {"x","y","z"};
    std::sort(expected.begin(), expected.end());
    REQUIRE(ks == expected);
}

// 11) Stress: deterministic large load
static void test_large_deterministic_load() {
    HashTable ht(257); // modestly sized
    const int N = 20000;
    for (int i = 0; i < N; ++i) {
        std::string k = "k" + std::to_string(i % 5000); // 5k unique keys
        ht.insert(k.c_str());
    }
    // Each of 5000 keys appears exactly N/5000 times
    for (int i = 0; i < 5000; ++i) {
        std::string k = "k" + std::to_string(i);
        int expected = N / 5000;
        check_eq_int("large load counts", ht.getCount(k.c_str()), expected);
    }
}

// Entry point for all tests
static void run_all_tests() {
    test_basic_insert_and_get();
    test_remove();
    test_case_sensitivity();
    test_empty_string_key();
    test_long_key();
    test_collisions_and_chaining();
    test_remove_positions_in_chain();
    test_many_duplicates();
    test_fuzz_against_unordered_map();
    test_getAllKeys_uniqueness();
    test_large_deterministic_load();

    std::printf("✅ All tests PASSED. Total checks: %d\n", g_passed);
}

int main() {
    run_all_tests();
    return 0;
}
