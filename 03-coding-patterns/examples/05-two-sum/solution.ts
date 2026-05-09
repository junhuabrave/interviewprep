// Two Sum — TypeScript
function twoSum(nums: number[], target: number): [number, number] {
    const seen = new Map<number, number>();
    for (let i = 0; i < nums.length; i++) {
        const complement = target - nums[i]!;
        const j = seen.get(complement);
        if (j !== undefined) return [j, i];
        seen.set(nums[i]!, i);
    }
    throw new Error("no pair found");
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const cases: Array<[number[], number, [number, number]]> = [
    [[2, 7, 11, 15], 9, [0, 1]],
    [[3, 2, 4], 6, [1, 2]],
    [[3, 3], 6, [0, 1]],
    [[-1, -2, -3, -4, -5], -8, [2, 4]],
];
for (const [nums, target, want] of cases) {
    const got = twoSum(nums, target);
    check(got[0] === want[0] && got[1] === want[1], `target=${target}`);
}
console.log("PASS");
