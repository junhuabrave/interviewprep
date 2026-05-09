// Number of Islands — TypeScript (BFS)
function numIslands(grid: string[][]): number {
    if (grid.length === 0 || grid[0]!.length === 0) return 0;
    const R = grid.length, C = grid[0]!.length;
    const g = grid.map(row => row.slice());
    const DIRS = [[-1, 0], [1, 0], [0, -1], [0, 1]];

    let count = 0;
    for (let r = 0; r < R; r++) {
        for (let c = 0; c < C; c++) {
            if (g[r]![c] !== "1") continue;
            count++;
            const q: [number, number][] = [[r, c]];
            g[r]![c] = "0";
            while (q.length > 0) {
                const [cr, cc] = q.shift()!;     // small grids — shift is fine
                for (const [dr, dc] of DIRS) {
                    const nr = cr + dr!, nc = cc + dc!;
                    if (nr >= 0 && nr < R && nc >= 0 && nc < C && g[nr]![nc] === "1") {
                        g[nr]![nc] = "0";
                        q.push([nr, nc]);
                    }
                }
            }
        }
    }
    return count;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const g1 = [
    ["1", "1", "1", "1", "0"],
    ["1", "1", "0", "1", "0"],
    ["1", "1", "0", "0", "0"],
    ["0", "0", "0", "0", "0"],
];
const g2 = [
    ["1", "1", "0", "0", "0"],
    ["1", "1", "0", "0", "0"],
    ["0", "0", "1", "0", "0"],
    ["0", "0", "0", "1", "1"],
];
check(numIslands(g1) === 1, "case-1");
check(numIslands(g2) === 3, "case-2");
check(numIslands([]) === 0, "empty");
check(numIslands([["0", "0"], ["0", "0"]]) === 0, "all-water");

console.log("PASS");
