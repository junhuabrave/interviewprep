"""Network Delay Time (Dijkstra) — Python 3.12."""
import heapq
from collections import defaultdict


def network_delay_time(times: list[list[int]], n: int, k: int) -> int:
    graph: dict[int, list[tuple[int, int]]] = defaultdict(list)
    for u, v, w in times:
        graph[u].append((v, w))

    INF = float('inf')
    dist = {i: INF for i in range(1, n + 1)}
    dist[k] = 0
    pq: list[tuple[int, int]] = [(0, k)]

    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue                         # stale entry
        for v, w in graph[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))

    longest = max(dist.values())
    return -1 if longest == INF else longest


if __name__ == "__main__":
    cases = [
        ([[2,1,1],[2,3,1],[3,4,1]], 4, 2, 2),
        ([[1,2,1]], 2, 1, 1),
        ([[1,2,1]], 2, 2, -1),
        ([[1,2,1],[2,1,3]], 2, 1, 1),
    ]
    for times, n, k, want in cases:
        got = network_delay_time(times, n, k)
        assert got == want, f"got {got}, want {want}"
    print("PASS")
