# Course Schedule (Topological Sort)

## Problem

There are `numCourses` courses labeled `0..numCourses-1`. `prerequisites[i] = [a, b]` means you must take `b` before `a`. Return whether you can finish all courses (no cyclic dependencies). (LC 207 / 210.)

LC 210 is the same problem but asks for **a valid order** instead of just yes/no.

## Approach: Kahn's algorithm (BFS-based topological sort)

1. Build adjacency list `b → [a's]` and `indeg[a]` count of incoming edges.
2. Push all nodes with `indeg == 0` into a queue.
3. Repeatedly: pop a node, append to order, decrement `indeg` of its neighbors. When a neighbor reaches 0, push it.
4. If `len(order) == numCourses`, no cycle. Otherwise, there's a cycle.

## Complexity

| | Time | Space |
|---|---|---|
| | O(V + E) | O(V + E) |

## Alternative: DFS with 3-color cycle detection

Each node colored WHITE/GRAY/BLACK:
- WHITE: unvisited.
- GRAY: in the current DFS path.
- BLACK: fully explored.

A back edge to a GRAY node is a cycle. Push to a stack on exit; reverse for topo order.

## Edge cases

- No prerequisites → trivially valid; any order.
- Self-loop (`[a, a]`) → cycle, can't finish.
- Disconnected graph → BFS from all 0-indegree nodes.
- Duplicate edges — handle as multiset; indeg counts include duplicates.

## Variants

- **Alien Dictionary** (LC 269): derive ordering constraints from a list of words, then topo sort.
- **Build a parallel-execution schedule**: also reports the depth (max level) of the DAG.
