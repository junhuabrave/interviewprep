// LRU Cache — TypeScript. Uses Map's preserved-insertion-order property.
class LRUCache {
    private cap: number;
    private map = new Map<number, number>();

    constructor(capacity: number) {
        this.cap = capacity;
    }

    get(key: number): number {
        if (!this.map.has(key)) return -1;
        const v = this.map.get(key)!;
        this.map.delete(key);            // re-insert to mark as most recent
        this.map.set(key, v);
        return v;
    }

    put(key: number, value: number): void {
        if (this.map.has(key)) this.map.delete(key);
        this.map.set(key, value);
        if (this.map.size > this.cap) {
            // Map iteration is insertion order — first key is the oldest.
            const oldest = this.map.keys().next().value!;
            this.map.delete(oldest);
        }
    }
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const c = new LRUCache(2);
c.put(1, 1); c.put(2, 2);
check(c.get(1) === 1, "get-1");
c.put(3, 3);
check(c.get(2) === -1, "evicted-2");
c.put(4, 4);
check(c.get(1) === -1, "evicted-1");
check(c.get(3) === 3, "get-3");
check(c.get(4) === 4, "get-4");

const c1 = new LRUCache(1);
c1.put(1, 1); c1.put(2, 2);
check(c1.get(1) === -1 && c1.get(2) === 2, "cap-1");

console.log("PASS");
