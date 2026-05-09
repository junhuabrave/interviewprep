"""Course Schedule (II) — return a valid order or [] if cycle. Python 3.12."""
from collections import deque, defaultdict


def find_order(num_courses: int, prerequisites: list[list[int]]) -> list[int]:
    graph: dict[int, list[int]] = defaultdict(list)
    indeg = [0] * num_courses
    for a, b in prerequisites:
        graph[b].append(a)
        indeg[a] += 1

    q = deque(i for i in range(num_courses) if indeg[i] == 0)
    order: list[int] = []
    while q:
        u = q.popleft()
        order.append(u)
        for v in graph[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)

    return order if len(order) == num_courses else []


def can_finish(num_courses: int, prerequisites: list[list[int]]) -> bool:
    return len(find_order(num_courses, prerequisites)) == num_courses


def _is_valid_order(num_courses: int, prereqs: list[list[int]], order: list[int]) -> bool:
    if len(order) != num_courses or set(order) != set(range(num_courses)):
        return False
    pos = {c: i for i, c in enumerate(order)}
    return all(pos[b] < pos[a] for a, b in prereqs)


if __name__ == "__main__":
    cases_finish = [
        (2, [[1, 0]], True),
        (2, [[1, 0], [0, 1]], False),
        (4, [[1, 0], [2, 0], [3, 1], [3, 2]], True),
        (1, [], True),
        (3, [[0, 0]], False),
    ]
    for n, p, want in cases_finish:
        assert can_finish(n, p) == want, f"can_finish({n}, {p})"

    for n, p, _ in cases_finish:
        order = find_order(n, p)
        if order:
            assert _is_valid_order(n, p, order), f"invalid order for ({n}, {p}): {order}"
    print("PASS")
