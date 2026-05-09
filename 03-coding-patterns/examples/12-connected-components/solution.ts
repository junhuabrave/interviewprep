// Number of Connected Components — Union-Find. TypeScript.
class UnionFind {
    private parent: number[];
    private rank: number[];
    components: number;

    constructor(n: number) {
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = new Array<number>(n).fill(0);
        this.components = n;
    }

    find(x: number): number {
        while (this.parent[x]! !== x) {
            this.parent[x] = this.parent[this.parent[x]!]!;
            x = this.parent[x]!;
        }
        return x;
    }

    union(a: number, b: number): boolean {
        let ra = this.find(a), rb = this.find(b);
        if (ra === rb) return false;
        if (this.rank[ra]! < this.rank[rb]!) [ra, rb] = [rb, ra];
        this.parent[rb] = ra;
        if (this.rank[ra] === this.rank[rb]) this.rank[ra]!++;
        this.components--;
        return true;
    }
}

function countComponents(n: number, edges: number[][]): number {
    const uf = new UnionFind(n);
    for (const [a, b] of edges) uf.union(a!, b!);
    return uf.components;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(countComponents(5, [[0, 1], [1, 2], [3, 4]]) === 2, "two-comp");
check(countComponents(5, [[0, 1], [1, 2], [2, 3], [3, 4]]) === 1, "linear");
check(countComponents(4, []) === 4, "no-edges");
check(countComponents(1, []) === 1, "single");
check(countComponents(3, [[0, 0], [1, 2]]) === 2, "self-loop");
check(countComponents(3, [[0, 1], [0, 1]]) === 2, "dup-edge");

console.log("PASS");
