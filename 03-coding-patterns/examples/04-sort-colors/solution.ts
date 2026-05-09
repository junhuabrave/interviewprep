// Sort Colors (Dutch national flag) — TypeScript
function sortColors(nums: number[]): void {
    let lo = 0, mid = 0, hi = nums.length - 1;
    while (mid <= hi) {
        switch (nums[mid]) {
            case 0:
                [nums[lo], nums[mid]] = [nums[mid]!, nums[lo]!];
                lo++; mid++;
                break;
            case 1:
                mid++;
                break;
            case 2:
                [nums[mid], nums[hi]] = [nums[hi]!, nums[mid]!];
                hi--;
                break;
        }
    }
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const cases: Array<[number[], number[]]> = [
    [[2, 0, 2, 1, 1, 0], [0, 0, 1, 1, 2, 2]],
    [[2, 0, 1], [0, 1, 2]],
    [[], []],
    [[0], [0]],
    [[1, 1, 1], [1, 1, 1]],
];
for (const [a, want] of cases) {
    sortColors(a);
    check(JSON.stringify(a) === JSON.stringify(want), "sort-colors");
}
console.log("PASS");
