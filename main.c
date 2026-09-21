#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashtable.c"
#include "splay.c"
#include "avl.c"

void generate_keys(int *keys, int n, int mode) {
    if (mode == 0) {  // random
        for (int i = 0; i < n; i++) keys[i] = i;
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = keys[i]; keys[i] = keys[j]; keys[j] = tmp;
        }
    } else if (mode == 1) {  // sorted
        for (int i = 0; i < n; i++) keys[i] = i;
    } else if (mode == 2) {  // reverse
        for (int i = 0; i < n; i++) keys[i] = n - i;
    }
}


void run_test(const char *scenario, int *keys, int n) {
    clock_t start, end;

    printf("\n=== %s (n=%d) ===\n", scenario, n);
    printf("+---------+----------+-------------+-------------+-------------+\n");
    printf("| Type    | Pocet    | Insert      | Search      | Delete      |\n");
    printf("+---------+----------+-------------+-------------+-------------+\n");

    // ── AVL ──
    AVL_NODE *avl_root = NULL;
    int avl_count = 0;
    start = clock();
    for (int i = 0; i < n; i++) avl_root = avl_insert(avl_root, keys[i], &avl_count);
    end = clock();
    double avl_ins = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) avl_find(avl_root, keys[i]);
    end = clock();
    double avl_sea = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) avl_root = avl_delete(avl_root, keys[i], &avl_count);
    end = clock();
    double avl_del = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("| AVL     | %8d | %8.2f ms | %8.2f ms | %8.2f ms |\n", n, avl_ins, avl_sea, avl_del);
    avl_free(avl_root);

    // ── Splay ──
    SPLAY_NODE *splay_root = NULL;
    int splay_count = 0;
    start = clock();
    for (int i = 0; i < n; i++) splay_root = splay_insert(splay_root, keys[i], &splay_count);
    end = clock();
    double splay_ins = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) splay_root = splay_find(splay_root, keys[i]);
    end = clock();
    double splay_sea = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) splay_root = splay_delete(splay_root, keys[i], &splay_count);
    end = clock();
    double splay_del = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("| Splay   | %8d | %8.2f ms | %8.2f ms | %8.2f ms |\n", n, splay_ins, splay_sea, splay_del);
    splay_free(splay_root);

    // ── Chaining ──
    HashTable ht_chain;
    init_hash_table(&ht_chain, next_prime(n * 2), CHAINING);
    start = clock();
    for (int i = 0; i < n; i++) insert_chaining(&ht_chain, keys[i]);
    end = clock();
    double chain_ins = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) search_chaining(&ht_chain, keys[i]);
    end = clock();
    double chain_sea = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) delete_chaining(&ht_chain, keys[i]);
    end = clock();
    double chain_del = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("| Chaining| %8d | %8.2f ms | %8.2f ms | %8.2f ms |\n", n, chain_ins, chain_sea, chain_del);
    free_hash(&ht_chain);

    // ── Double Hashing ──
    HashTable ht_double;
    init_hash_table(&ht_double, next_prime(n * 2), DOUBLE_HASHING);
    start = clock();
    for (int i = 0; i < n; i++) insert_double(&ht_double, keys[i]);
    end = clock();
    double double_ins = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) search_double(&ht_double, keys[i]);
    end = clock();
    double double_sea = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    start = clock();
    for (int i = 0; i < n; i++) delete_double(&ht_double, keys[i]);
    end = clock();
    double double_del = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("| Double  | %8d | %8.2f ms | %8.2f ms | %8.2f ms |\n", n, double_ins, double_sea, double_del);
    free_hash(&ht_double);

    printf("+---------+----------+-------------+-------------+-------------+\n");
}

int main() {
    srand(42);
    int sizes[] = {1000, 10000, 100000, 500000, 1000000};
    int count = sizeof(sizes) / sizeof(sizes[0]);
    const char *scenarios[] = {"Random", "Sorted", "Reverse"};

    for (int s = 0; s < 3; s++) {
        for (int i = 0; i < count; i++) {
            int n = sizes[i];
            int *keys = malloc(n * sizeof(int));
            generate_keys(keys, n, s);
            run_test(scenarios[s], keys, n);
            free(keys);
        }
    }
    return 0;
}