# Graphs

## Representations

| Repr | Space | Edge check | Iterate neighbors |
|------|-------|-----------|-------------------|
| Adjacency list | O(V+E) | O(deg) | O(deg) |
| Adjacency matrix | O(V²) | O(1) | O(V) |
| Edge list | O(E) | O(E) | O(E) |

For most interview problems use **adjacency list**: `dict[node] -> list[neighbors]` or `vector<vector<int>>`.

## BFS

Use when: shortest path in **unweighted** graph, level-by-level processing.

```python
from collections import deque
def bfs(start, graph):
    visited = {start}
    q = deque([start])
    dist = {start: 0}
    while q:
        u = q.popleft()
        for v in graph[u]:
            if v not in visited:
                visited.add(v)
                dist[v] = dist[u] + 1
                q.append(v)
    return dist
```

## DFS

Use when: connectivity, cycle detection, topological sort, path enumeration.

```python
def dfs(u, graph, visited):
    visited.add(u)
    for v in graph[u]:
        if v not in visited:
            dfs(v, graph, visited)
```

For deep graphs (n > 10^4), prefer iterative DFS with an explicit stack to avoid recursion overflow.

## Topological sort

Two methods on a DAG:

### Kahn's algorithm (BFS-based)
```python
indeg = {u: 0 for u in graph}
for u in graph:
    for v in graph[u]:
        indeg[v] += 1
q = deque([u for u, d in indeg.items() if d == 0])
order = []
while q:
    u = q.popleft()
    order.append(u)
    for v in graph[u]:
        indeg[v] -= 1
        if indeg[v] == 0:
            q.append(v)
return order if len(order) == len(graph) else []   # cycle if shorter
```

### DFS-based
Postorder DFS, push to a stack on exit. Reverse the stack at the end.

Detect cycles in a directed graph during DFS using **3 colors**: WHITE (unvisited), GRAY (in current path), BLACK (done). Edge to GRAY = back edge = cycle.

## Dijkstra (single-source shortest path, non-negative weights)

```python
import heapq
def dijkstra(src, graph):    # graph: dict[u] -> list[(v, w)]
    dist = {src: 0}
    pq = [(0, src)]
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]: continue        # stale entry
        for v, w in graph[u]:
            nd = d + w
            if nd < dist.get(v, float('inf')):
                dist[v] = nd
                heapq.heappush(pq, (nd, v))
    return dist
```

O((V+E) log V). Doesn't work with negative edges — use **Bellman-Ford** (O(VE)) for that.

## Union-Find (Disjoint Set Union)

```python
parent = list(range(n))
rank = [0] * n
def find(x):
    while parent[x] != x:
        parent[x] = parent[parent[x]]   # path compression (halving)
        x = parent[x]
    return x

def union(a, b):
    ra, rb = find(a), find(b)
    if ra == rb: return False
    if rank[ra] < rank[rb]: ra, rb = rb, ra
    parent[rb] = ra
    if rank[ra] == rank[rb]: rank[ra] += 1
    return True
```

Per-op is ~O(α(n)) ≈ O(1). Use for: connected components, Kruskal's MST, "merge accounts", "redundant connection".

## Grid as a graph

```python
DIRS = [(-1,0),(1,0),(0,-1),(0,1)]
for dr, dc in DIRS:
    nr, nc = r+dr, c+dc
    if 0 <= nr < R and 0 <= nc < C and grid[nr][nc] == '1':
        ...
```

Diagonals: add `(±1, ±1)` for 8-connected.

## Common gotchas

- **Disconnected graphs**: a single BFS/DFS won't visit everything. Loop over all nodes.
- **Cycle detection**: directed needs 3-color or topo sort; undirected can use union-find or DFS with parent.
- **Dijkstra with negative edges** — wrong answers, no warning.
- **Default dict mutation** during iteration → runtime error.
- **Large stack for recursive DFS** in C++ — bump with `ulimit -s` if needed; in Python switch to iterative.

## Top problems

- Number of Islands (BFS/DFS grid)
- Surrounded Regions
- Course Schedule / II (topological sort + cycle)
- Clone Graph
- Pacific Atlantic Water Flow
- Word Ladder (BFS shortest)
- Network Delay Time (Dijkstra)
- Number of Connected Components (Union-Find)
- Redundant Connection (Union-Find)
- Cheapest Flights Within K Stops (Bellman-Ford / BFS)
- Alien Dictionary (topological sort from order constraints)
