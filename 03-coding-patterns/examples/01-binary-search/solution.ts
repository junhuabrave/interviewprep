// Binary search variants — TypeScript
function search(nums: number[], target: number): number {
    let l = 0, r = nums.length - 1;
    while (l <= r) {
        const m = l + ((r - l) >> 1);
        const v = nums[m]!;
        if (v === target) return m;
        if (v < target) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

function lowerBound(nums: number[], target: number): number {
    let l = 0, r = nums.length;
    while (l < r) {
        const m = (l + r) >> 1;
        if (nums[m]! < target) l = m + 1;
        else r = m;
    }
    return l;
}

function searchRotated(nums: number[], target: number): number {
    let l = 0, r = nums.length - 1;
    while (l <= r) {
        const m = (l + r) >> 1;
        if (nums[m] === target) return m;
        if (nums[l]! <= nums[m]!) {
            if (nums[l]! <= target && target < nums[m]!) r = m - 1;
            else l = m + 1;
        } else {
            if (nums[m]! < target && target <= nums[r]!) l = m + 1;
            else r = m - 1;
        }
    }
    return -1;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(search([1, 3, 5, 7, 9], 5) === 2, "search-found");
check(search([1, 3, 5, 7, 9], 6) === -1, "search-missing");
check(search([], 1) === -1, "search-empty");
check(lowerBound([1, 2, 4, 4, 5], 4) === 2, "lb-mid");
check(lowerBound([1, 2, 4, 4, 5], 6) === 5, "lb-end");
check(searchRotated([4, 5, 6, 7, 0, 1, 2], 0) === 4, "rot-found");
check(searchRotated([4, 5, 6, 7, 0, 1, 2], 3) === -1, "rot-missing");

console.log("PASS");
