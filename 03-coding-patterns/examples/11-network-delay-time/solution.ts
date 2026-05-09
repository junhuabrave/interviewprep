// Network Delay Time (Dijkstra) — TypeScript
// Uses a hand-rolled binary min-heap of [dist, node].

class MinHeap {
    private h: Array<[number, number]> = [];
    size(): number { return this.h.length; }
    push(item: [number, number]): void {
        this.h.push(item);
        this.bubbleUp(this.h.length - 1);
    }
    pop(): [number, number] | undefined {
        if (this.h.length === 0) return undefined;
        const top = this.h[0]!;
        const last = this.h.pop()!;
        if (this.h.length > 0) {
            this.h[0] = last;
            this.bubbleDown(0);
        }
        return top;
    }
    private bubbleUp(i: number): void {
        while (i > 0) {
            const p = (i - 1) >> 1;
            if (this.h[p]![0] <= this.h[i]![0]) break;
            [this.h[p], this.h[i]] = [this.h[i]!, this.h[p]!];
            i = p;
        }
    }
    private bubbleDown(i: number): void {
        const n = this.h.length;
        while (true) {
            const l = 2 * i + 1, r = 2 * i + 2;
            let best = i;
            if (l < n && this.h[l]![0] < this.h[best]![0]) best = l;
            if (r < n && this.h[r]![0] < this.h[best]![0]) best = r;
            if (best === i) break;
            [this.h[best], this.h[i]] = [this.h[i]!, this.h[best]!];
            i = best;
        }
    }
}

function networkDelayTime(times: number[][], n: number, k: number): number {
    const graph: Array<Array<[number, number]>> = Array.from({ length: n + 1 }, () => []);
    for (const [u, v, w] of times) graph[u!]!.push([v!, w!]);

    const dist = new Array<number>(n + 1).fill(Infinity);
    dist[k] = 0;
    const pq = new MinHeap();
    pq.push([0, k]);

    while (pq.size() > 0) {
        const [d, u] = pq.pop()!;
        if (d > dist[u]!) continue;
        for (const [v, w] of graph[u]!) {
            const nd = d + w;
            if (nd < dist[v]!) {
                dist[v] = nd;
                pq.push([nd, v]);
            }
        }
    }

    let longest = 0;
    for (let i = 1; i <= n; i++) {
        if (dist[i] === Infinity) return -1;
        longest = Math.max(longest, dist[i]!);
    }
    return longest;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(networkDelayTime([[2, 1, 1], [2, 3, 1], [3, 4, 1]], 4, 2) === 2, "case-1");
check(networkDelayTime([[1, 2, 1]], 2, 1) === 1, "case-2");
check(networkDelayTime([[1, 2, 1]], 2, 2) === -1, "unreachable");
check(networkDelayTime([[1, 2, 1], [2, 1, 3]], 2, 1) === 1, "round-trip");

console.log("PASS");
