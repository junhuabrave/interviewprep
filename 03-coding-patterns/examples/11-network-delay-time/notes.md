# Network Delay Time (Dijkstra)

## Problem

A network of `n` nodes labeled `1..n`. `times[i] = [u, v, w]` is a directed edge from `u` to `v` with travel time `w`. Send a signal from node `k`. Return the minimum time for **all** nodes to receive it; return `-1` if any node is unreachable. (LC 743.)

This is single-source shortest path with non-negative edge weights → **Dijkstra's algorithm**.

## Approach

1. Build adjacency list `u → [(v, w), ...]`.
2. Min-heap of `(dist_so_far, node)`. Start with `(0, k)`.
3. Pop the node with minimum `dist`. If we've finalized this node already (popped distance > stored distance), skip.
4. For each neighbor, relax: if `dist[u] + w < dist[v]`, update and push.
5. Answer = max of all finalized distances. If any node unreachable, return `-1`.

## Complexity

| | Time | Space |
|---|---|---|
| Binary-heap Dijkstra | O((V + E) log V) | O(V + E) |
| Fibonacci heap | O(E + V log V) | O(V + E) (rare in interviews) |

## Don't use when

- **Negative edge weights** → use Bellman-Ford O(VE).
- **Unweighted** graph → BFS suffices in O(V + E).

## Edge cases

- Source has no outgoing edges and `n > 1` → `-1`.
- Disconnected node → `-1`.
- Self-loops / parallel edges — keep the minimum on relax.
- Already-finalized check (`if popped > dist[u]: skip`) is important; otherwise stale heap entries cause O(E²).

## Variants

- **Cheapest Flights Within K Stops** (LC 787): Bellman-Ford-ish or modified BFS with stop count in the state.
- **Shortest path in a grid with weights**: Dijkstra on grid where each cell has a cost.
- **Path with minimum effort** (LC 1631): Dijkstra where edge weight is `max(...)` instead of `sum(...)`.
