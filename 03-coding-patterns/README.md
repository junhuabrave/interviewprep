# Coding Patterns

Two things live here:
1. **A decision tree** (this file) — given a problem, which pattern fits?
2. **An algorithm bank** of 15 worked problems in [`examples/`](examples/), each with a `notes.md` and full solutions in **Python 3.12**, **C++ 20/23**, and **TypeScript**.

## Decision tree: pick the pattern

```
Is the input sorted (or can you sort it cheaply)?
├── YES → Two Pointers, Binary Search, Greedy on intervals
└── NO  → continue

Are you asked for a contiguous subarray/substring with a property?
├── YES → Sliding Window
└── NO  → continue

Do you need "have I seen X?" / counts / pairs summing to K?
├── YES → Hashmap / Hashset
└── NO  → continue

Is it a graph / grid / dependency problem?
├── YES → BFS (shortest, level-by-level), DFS (connectivity, paths),
│         Topological sort (DAG ordering), Union-Find (grouping),
│         Dijkstra (weighted shortest path)
└── NO  → continue

Are you asked for top/k-th element, streaming median, K-way merge?
├── YES → Heap (priority queue)
└── NO  → continue

Is it "min/max cost" / "count ways" / "can you reach"?
├── YES → Dynamic Programming
└── NO  → continue

Are you asked to enumerate all valid combinations / subsets / permutations?
├── YES → Backtracking
└── NO  → Re-read the problem; consider design (LRU, trie, segment tree)
```

## 15-pattern catalogue

| Pattern | When to use | Example in bank |
|---------|-------------|-----------------|
| Two pointers | Sorted array, palindrome, partitioning | `04-sort-colors`, `07-trapping-rain-water` |
| Sliding window | Contiguous subarray with property | `08-longest-substring-no-repeat` |
| Hashmap lookup | "Have I seen this?", complement, grouping | `05-two-sum`, `06-group-anagrams` |
| Binary search | Sorted array; or "binary search on the answer" | `01-binary-search` |
| Quickselect / partition | Kth element, top-k average O(n) | `03-quickselect` |
| Merge sort / divide & conquer | Stable sort, count inversions, linked list sort | `02-merge-sort` |
| Heap (top-K, K-way merge) | Streaming top-k, median, schedulers | (see [`heaps`](../01-dsa-cheatsheets/06-heaps.md)) |
| BFS | Shortest path in unweighted graph, level-by-level | `09-number-of-islands` |
| DFS | Connectivity, all paths, cycle detection | `09-number-of-islands` (alt) |
| Topological sort | Order with dependencies on a DAG | `10-course-schedule` |
| Dijkstra | Shortest path, non-negative weights | `11-network-delay-time` |
| Union-Find | Connected components, MST, "merge" queries | `12-connected-components` |
| 1D DP | "Count ways", "min cost to reach state" | `14-coin-change` |
| 2D DP | Two-sequence problems, grid DP | `15-longest-common-subseq` |
| Hashmap + DLL | LRU/LFU cache | `13-lru-cache` |

## Bank index

| # | Problem | Pattern | Difficulty |
|---|---------|---------|------------|
| 01 | [Binary Search](examples/01-binary-search/notes.md) | Binary search | Easy |
| 02 | [Merge Sort](examples/02-merge-sort/notes.md) | Divide & conquer | Medium |
| 03 | [Kth Largest (Quickselect)](examples/03-quickselect/notes.md) | Partition / heap | Medium |
| 04 | [Sort Colors](examples/04-sort-colors/notes.md) | Two pointers (Dutch flag) | Medium |
| 05 | [Two Sum](examples/05-two-sum/notes.md) | Hashmap | Easy |
| 06 | [Group Anagrams](examples/06-group-anagrams/notes.md) | Hashmap grouping | Medium |
| 07 | [Trapping Rain Water](examples/07-trapping-rain-water/notes.md) | Two pointers | Hard |
| 08 | [Longest Substring Without Repeating Characters](examples/08-longest-substring-no-repeat/notes.md) | Sliding window | Medium |
| 09 | [Number of Islands](examples/09-number-of-islands/notes.md) | BFS/DFS on grid | Medium |
| 10 | [Course Schedule](examples/10-course-schedule/notes.md) | Topological sort | Medium |
| 11 | [Network Delay Time](examples/11-network-delay-time/notes.md) | Dijkstra | Medium |
| 12 | [Number of Connected Components](examples/12-connected-components/notes.md) | Union-Find | Medium |
| 13 | [LRU Cache](examples/13-lru-cache/notes.md) | Hashmap + DLL | Medium |
| 14 | [Coin Change](examples/14-coin-change/notes.md) | 1D DP | Medium |
| 15 | [Longest Common Subsequence](examples/15-longest-common-subseq/notes.md) | 2D DP | Medium |

## Running the solutions

```bash
# Python 3.12
python3.12 examples/05-two-sum/solution.py

# C++23 (fall back to -std=c++20 if needed)
g++ -std=c++23 -O2 examples/05-two-sum/solution.cpp -o /tmp/sol && /tmp/sol

# TypeScript
npx tsx examples/05-two-sum/solution.ts
```

Each `solution.*` includes a runnable test harness that prints PASS/FAIL.
