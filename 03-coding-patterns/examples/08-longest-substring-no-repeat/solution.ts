// Longest Substring Without Repeating Characters — TypeScript
function lengthOfLongestSubstring(s: string): number {
    const seen = new Map<string, number>();
    let l = 0, best = 0;
    for (let r = 0; r < s.length; r++) {
        const ch = s[r]!;
        const prev = seen.get(ch);
        if (prev !== undefined && prev >= l) {
            l = prev + 1;
        }
        seen.set(ch, r);
        best = Math.max(best, r - l + 1);
    }
    return best;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

const cases: Array<[string, number]> = [
    ["abcabcbb", 3],
    ["bbbbb", 1],
    ["pwwkew", 3],
    ["", 0],
    ["a", 1],
    ["dvdf", 3],
    ["abba", 2],
];
for (const [s, want] of cases) {
    check(lengthOfLongestSubstring(s) === want, `s="${s}"`);
}
console.log("PASS");
