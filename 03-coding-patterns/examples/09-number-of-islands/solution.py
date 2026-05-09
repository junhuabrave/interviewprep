"""Number of Islands — Python 3.12. BFS to avoid recursion-depth issues on big grids."""
from collections import deque


DIRS = [(-1, 0), (1, 0), (0, -1), (0, 1)]


def num_islands(grid: list[list[str]]) -> int:
    if not grid or not grid[0]:
        return 0
    R, C = len(grid), len(grid[0])
    grid = [row[:] for row in grid]              # don't mutate caller's grid
    count = 0
    for r in range(R):
        for c in range(C):
            if grid[r][c] != '1':
                continue
            count += 1
            q = deque([(r, c)])
            grid[r][c] = '0'
            while q:
                cr, cc = q.popleft()
                for dr, dc in DIRS:
                    nr, nc = cr + dr, cc + dc
                    if 0 <= nr < R and 0 <= nc < C and grid[nr][nc] == '1':
                        grid[nr][nc] = '0'
                        q.append((nr, nc))
    return count


if __name__ == "__main__":
    g1 = [
        ["1","1","1","1","0"],
        ["1","1","0","1","0"],
        ["1","1","0","0","0"],
        ["0","0","0","0","0"],
    ]
    g2 = [
        ["1","1","0","0","0"],
        ["1","1","0","0","0"],
        ["0","0","1","0","0"],
        ["0","0","0","1","1"],
    ]
    assert num_islands(g1) == 1, "case-1"
    assert num_islands(g2) == 3, "case-2"
    assert num_islands([]) == 0, "empty"
    assert num_islands([["0","0"],["0","0"]]) == 0, "all-water"
    print("PASS")
