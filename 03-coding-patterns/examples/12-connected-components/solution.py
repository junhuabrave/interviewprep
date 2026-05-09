"""Number of Connected Components — Union-Find. Python 3.12."""


class UnionFind:
    def __init__(self, n: int) -> None:
        self.parent = list(range(n))
        self.rank = [0] * n
        self.components = n

    def find(self, x: int) -> int:
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]   # path compression
            x = self.parent[x]
        return x

    def union(self, a: int, b: int) -> bool:
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        if self.rank[ra] < self.rank[rb]:
            ra, rb = rb, ra
        self.parent[rb] = ra
        if self.rank[ra] == self.rank[rb]:
            self.rank[ra] += 1
        self.components -= 1
        return True


def count_components(n: int, edges: list[list[int]]) -> int:
    uf = UnionFind(n)
    for a, b in edges:
        uf.union(a, b)
    return uf.components


if __name__ == "__main__":
    cases = [
        (5, [[0, 1], [1, 2], [3, 4]], 2),
        (5, [[0, 1], [1, 2], [2, 3], [3, 4]], 1),
        (4, [], 4),
        (1, [], 1),
        (3, [[0, 0], [1, 2]], 2),                       # self-loop
        (3, [[0, 1], [0, 1]], 2),                       # duplicate edge
    ]
    for n, edges, want in cases:
        got = count_components(n, edges)
        assert got == want, f"got {got}, want {want} for n={n}, edges={edges}"
    print("PASS")
