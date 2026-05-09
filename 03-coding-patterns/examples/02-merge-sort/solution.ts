// Merge sort — TypeScript
function mergeSort(arr: number[]): number[] {
    if (arr.length <= 1) return arr.slice();
    const mid = arr.length >> 1;
    return merge(mergeSort(arr.slice(0, mid)), mergeSort(arr.slice(mid)));
}

function merge(left: number[], right: number[]): number[] {
    const out: number[] = [];
    let i = 0, j = 0;
    while (i < left.length && j < right.length) {
        if (left[i]! <= right[j]!) out.push(left[i++]!);
        else out.push(right[j++]!);
    }
    while (i < left.length) out.push(left[i++]!);
    while (j < right.length) out.push(right[j++]!);
    return out;
}

function countInversions(arr: number[]): number {
    let total = 0;
    const buf = new Array<number>(arr.length);

    const sortCount = (a: number[], l: number, r: number): void => {
        if (r - l <= 1) return;
        const m = l + ((r - l) >> 1);
        sortCount(a, l, m); sortCount(a, m, r);
        let i = l, j = m, k = l;
        while (i < m && j < r) {
            if (a[i]! <= a[j]!) buf[k++] = a[i++]!;
            else { buf[k++] = a[j++]!; total += m - i; }
        }
        while (i < m) buf[k++] = a[i++]!;
        while (j < r) buf[k++] = a[j++]!;
        for (let t = l; t < r; ++t) a[t] = buf[t]!;
    };
    sortCount(arr.slice(), 0, arr.length);
    return total;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const arr = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5];
const want = [...arr].sort((a, b) => a - b);
check(JSON.stringify(mergeSort(arr)) === JSON.stringify(want), "sort-mixed");
check(JSON.stringify(mergeSort([])) === "[]", "sort-empty");
check(countInversions([2, 4, 1, 3, 5]) === 3, "inversions");

console.log("PASS");
