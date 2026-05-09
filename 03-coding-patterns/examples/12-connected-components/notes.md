# Number of Connected Components in an Undirected Graph

## Problem

You have a graph of `n` nodes labeled `0..n-1`. Given a list of edges (undirected), return the number of connected components. (LC 323.)

Could also be solved with BFS/DFS, but **Union-Find** is shorter and the right tool when edges arrive incrementally or you must answer "are these two connected?" queries.

## Approach: Union-Find (Disjoint Set Union)

1. Initialize `parent[i] = i`, `rank[i] = 0`. Initially `n` components.
2. For each edge `(u, v)`: if `find(u) != find(v)`, union them and decrement the component count.
3. Return the final count.

**Path compression** (during `find`) and **union by rank** (during `union`) yield amortized ~O(α(n)) per op — effectively O(1).

## Complexity

| | Time | Space |
|---|---|---|
| Per operation | ~O(α(n)) ≈ O(1) | O(n) |
| Total | O((n + e) · α(n)) | O(n) |

## When to use Union-Find vs BFS/DFS

| Scenario | Tool |
|---|---|
| Edges given upfront, single-query "count components" | BFS/DFS or UF; UF is shorter |
| Incremental edges, repeated "are they connected?" queries | **Union-Find** |
| Need actual paths or distances | BFS / Dijkstra |
| Minimum Spanning Tree (Kruskal) | **Union-Find** |
| Cycle detection in undirected graph | **Union-Find** or DFS with parent |

## Edge cases

- `n = 0` → 0.
- No edges → `n` components.
- Self-loop `(u, u)` → no new component, no merge.
- Duplicate edges — second `union` is a no-op.

## Variants

- **Redundant Connection** (LC 684): the first edge that creates a cycle.
- **Accounts Merge** (LC 721): UF with email → account index, merge by shared emails.
- **Number of Provinces** (LC 547): same problem, given as adjacency matrix.
