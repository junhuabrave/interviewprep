// Kth largest via quickselect — TypeScript
function findKthLargest(nums: number[], k: number): number {
    const a = nums.slice();
    const target = a.length - k;
    let l = 0, r = a.length - 1;
    while (l < r) {
        const p = partition(a, l, r);
        if (p === target) return a[p]!;
        if (p < target) l = p + 1;
        else r = p - 1;
    }
    return a[l]!;
}

function partition(a: number[], l: number, r: number): number {
    const pi = l + Math.floor(Math.random() * (r - l + 1));
    [a[pi], a[r]] = [a[r]!, a[pi]!];
    const pivot = a[r]!;
    let i = l;
    for (let j = l; j < r; ++j) {
        if (a[j]! < pivot) {
            [a[i], a[j]] = [a[j]!, a[i]!];
            i++;
        }
    }
    [a[i], a[r]] = [a[r]!, a[i]!];
    return i;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const cases: Array<[number[], number, number]> = [
    [[3, 2, 1, 5, 6, 4], 2, 5],
    [[3, 2, 3, 1, 2, 4, 5, 5, 6], 4, 4],
    [[1], 1, 1],
    [[7, 7, 7], 2, 7],
];
for (const [nums, k, want] of cases) {
    for (let t = 0; t < 5; ++t) {
        check(findKthLargest(nums, k) === want, `k=${k}`);
    }
}
console.log("PASS");
