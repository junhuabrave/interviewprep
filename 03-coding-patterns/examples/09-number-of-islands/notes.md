# Number of Islands

## Problem

Given a 2D grid of `'1'` (land) and `'0'` (water), count the number of islands. Cells connect 4-directionally. (LC 200.)

## Approach: BFS or DFS flood fill

Walk every cell. When you find a `'1'`, start a BFS/DFS that **marks all connected land cells as visited** (e.g., overwrite to `'0'` or use a `visited` set). Each starting `'1'` corresponds to one island.

## Complexity

Let R × C = grid size.

| | Time | Space |
|---|---|---|
| | O(R·C) | O(R·C) (recursion / queue worst case) |

## DFS template (4-directional)

```
DIRS = [(-1,0),(1,0),(0,-1),(0,1)]
def dfs(r, c):
    if r<0 or r>=R or c<0 or c>=C: return
    if grid[r][c] != '1': return
    grid[r][c] = '0'                    # mark visited in-place
    for dr, dc in DIRS:
        dfs(r+dr, c+dc)
```

## DFS vs BFS

- DFS is shorter to write recursively. Risk: stack overflow on huge connected regions (e.g., 10^6 cells in C++/Python). Use BFS or iterative DFS for safety.
- BFS uses a queue; safer for very large inputs.

## Edge cases

- Empty grid → 0.
- All water → 0.
- All land → 1.
- Diagonals usually **don't** count (clarify if asked).

## Variants

- **Max area of island** — return size, not count.
- **Number of distinct islands** — canonicalize each shape (relative coords) and store in a set.
- **Surrounded regions** — flood fill from borders to mark "safe", flip the rest.
- **Pacific Atlantic Water Flow** — multi-source BFS from edges.
