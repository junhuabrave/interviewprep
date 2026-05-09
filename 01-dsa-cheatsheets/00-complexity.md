# Complexity Cheat Sheet

## Big-O of common operations

| Structure         | Access | Search | Insert | Delete | Notes |
|-------------------|--------|--------|--------|--------|-------|
| Array             | O(1)   | O(n)   | O(n)   | O(n)   | Cache-friendly |
| Dynamic array     | O(1)   | O(n)   | O(1) amortized | O(n) | Resizing doubles |
| Hashmap           | —      | O(1) avg | O(1) avg | O(1) avg | Worst O(n) on collisions |
| Hashset           | —      | O(1) avg | O(1) avg | O(1) avg | |
| Linked list       | O(n)   | O(n)   | O(1) at head | O(1) given node | |
| Stack / Queue     | —      | —      | O(1)   | O(1)   | Use deque/array |
| Binary heap       | O(1) min | O(n) | O(log n) | O(log n) | |
| BST (balanced)    | O(log n) | O(log n) | O(log n) | O(log n) | |
| BST (unbalanced)  | O(n)   | O(n)   | O(n)   | O(n)   | Worst case skewed |
| Trie              | —      | O(L)   | O(L)   | O(L)   | L = key length |

## Big-O of sorts

| Sort         | Best     | Avg      | Worst    | Space  | Stable? |
|--------------|----------|----------|----------|--------|---------|
| Quicksort    | O(n log n) | O(n log n) | O(n²)  | O(log n) | No |
| Mergesort    | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Heapsort     | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Timsort (Python sort, JS sort) | O(n) | O(n log n) | O(n log n) | O(n) | Yes |
| Counting     | O(n+k)   | O(n+k)   | O(n+k)   | O(k)   | Yes |
| Radix        | O(nk)    | O(nk)    | O(nk)    | O(n+k) | Yes |

## Big-O of graph algorithms

| Algorithm                      | Time             | Space  |
|--------------------------------|------------------|--------|
| BFS / DFS (adj list)           | O(V+E)           | O(V)   |
| Dijkstra (binary heap)         | O((V+E) log V)   | O(V)   |
| Bellman-Ford                   | O(VE)            | O(V)   |
| Floyd-Warshall                 | O(V³)            | O(V²)  |
| Topological sort               | O(V+E)           | O(V)   |
| Union-Find (path compression + union by rank) | ~O(α(n)) per op | O(n) |
| MST (Kruskal/Prim)             | O(E log V)       | O(V)   |

## Master theorem (recurrence T(n) = aT(n/b) + f(n))

Compare `f(n)` to `n^(log_b a)`:
- `f(n) = O(n^(log_b a − ε))` → T(n) = Θ(n^(log_b a))
- `f(n) = Θ(n^(log_b a))` → T(n) = Θ(n^(log_b a) · log n)
- `f(n) = Ω(n^(log_b a + ε))` → T(n) = Θ(f(n))

Quick references:
- T(n) = 2T(n/2) + O(n) → O(n log n) [merge sort]
- T(n) = T(n/2) + O(1) → O(log n) [binary search]
- T(n) = 2T(n/2) + O(1) → O(n) [tree traversal]

## Common pitfalls

- "I copy the slice each iteration" — turns O(n) into O(n²). Use indices.
- Recursive solutions with memoization have **call stack** O(depth). Watch for stack overflow at n ~ 10^4 in C++ default.
- Hashmap `O(1) avg` but **worst O(n)** — don't depend on it under adversarial input.
- Sort then linear scan is O(n log n), not O(n²). Often the simplest path.
- `n × log n` ≈ 10^7 for n=10^6. If problem allows 1s and you're planning O(n²) at n=10^5, you're going to TLE.

## Constraint → likely complexity (competitive programming guide)

| n             | Acceptable complexity      |
|---------------|----------------------------|
| n ≤ 10        | O(n!) or O(2^n)            |
| n ≤ 20        | O(2^n · n)                 |
| n ≤ 500       | O(n³)                      |
| n ≤ 5,000     | O(n²)                      |
| n ≤ 10^6      | O(n log n) or O(n)         |
| n ≤ 10^9      | O(log n) or O(√n)          |
