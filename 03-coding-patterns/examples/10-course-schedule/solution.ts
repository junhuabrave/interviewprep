// Course Schedule II — TypeScript
function findOrder(numCourses: number, prereqs: number[][]): number[] {
    const graph: number[][] = Array.from({ length: numCourses }, () => []);
    const indeg = new Array<number>(numCourses).fill(0);
    for (const [a, b] of prereqs) {                  // b -> a
        graph[b!]!.push(a!);
        indeg[a!]++;
    }
    const q: number[] = [];
    for (let i = 0; i < numCourses; i++) {
        if (indeg[i] === 0) q.push(i);
    }
    const order: number[] = [];
    while (q.length > 0) {
        const u = q.shift()!;
        order.push(u);
        for (const v of graph[u]!) {
            if (--indeg[v]! === 0) q.push(v);
        }
    }
    return order.length === numCourses ? order : [];
}

function canFinish(n: number, p: number[][]): boolean {
    return findOrder(n, p).length === n;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(canFinish(2, [[1, 0]]) === true, "linear");
check(canFinish(2, [[1, 0], [0, 1]]) === false, "cycle");
check(canFinish(4, [[1, 0], [2, 0], [3, 1], [3, 2]]) === true, "diamond");
check(canFinish(1, []) === true, "single");
check(canFinish(3, [[0, 0]]) === false, "self-loop");

console.log("PASS");
