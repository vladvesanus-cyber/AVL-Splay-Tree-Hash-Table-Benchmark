# 🌳 AVL / Splay Tree & Hash Table Benchmark

A C benchmark comparing four classic data structures — **AVL tree**, **Splay tree**, and hash tables with **separate chaining** and **double hashing** — on insert, search, and delete operations across different data distributions and input sizes.

## What It Does

The program builds each data structure from scratch (no standard library containers) and measures raw performance under three access patterns:

- **Random** — shuffled keys
- **Sorted** — ascending keys (worst case for a naive/unbalanced BST)
- **Reverse** — descending keys

...at five input sizes: 1,000 / 10,000 / 100,000 / 500,000 / 1,000,000 elements, timing insert, search, and delete for each structure and printing the results as a table.

## Implementations

| File | Structure | Notes |
|---|---|---|
| `AVL.c` | AVL tree | Self-balancing via rotations; height-tracked at each node, guarantees O(log n) worst case regardless of insertion order |
| `Splay.c` | Splay tree | Self-adjusting via top-down splay operation; recently accessed keys move to the root |
| `hashtable.c` | Hash table | Supports both **chaining** and **double hashing** resolution strategies, with prime-sized tables and dynamic resizing |
| `main.c` | Benchmark driver | Generates key sets, runs all four structures through identical workloads, and reports timing |
| `test.c` | Tests | Correctness checks for the data structures |

## Why This Comparison Matters

- A plain unbalanced BST degrades to O(n) on sorted/reverse input — this benchmark makes that failure mode visible and shows how **AVL** (strict balancing) and **Splay** (adaptive/amortized balancing) each solve it differently, with different trade-offs.
- Hash tables trade ordering for near O(1) average-case access — chaining vs. double hashing is compared directly under the same key sets to show how collision resolution strategy affects real performance, not just theoretical complexity.

## Build & Run

```bash
gcc -O2 -o benchmark main.c -lm
./benchmark
```

Output is a table per scenario/size, e.g.:

```
=== Random (n=100000) ===
+---------+----------+-------------+-------------+-------------+
| Type    | Pocet    | Insert      | Search      | Delete      |
+---------+----------+-------------+-------------+-------------+
| AVL     |   100000 |       44 ms |       12 ms |       34 ms |
| Splay   |   100000 |       42 ms |       28 ms |       34 ms |
| Chaining|   100000 |        5 ms |        0 ms |        6 ms |
| Double  |   100000 |        1 ms |        1 ms |        1 ms |
+---------+----------+-------------+-------------+-------------+
```

### Run correctness tests

```bash
gcc -O2 -o test test.c -lm
./test
```

## Tech Stack

- **C** (no external dependencies beyond the standard library)
- Manual memory management (`malloc`/`free`) for all tree/table structures
- `time.h` (`clock()`) for benchmarking

---
*Author: Vladyslav Chytynskyi*