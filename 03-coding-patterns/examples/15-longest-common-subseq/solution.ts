// Longest Common Subsequence — TypeScript. 2D DP with rolling rows.
function longestCommonSubsequence(text1: string, text2: string): number {
    if (text1.length < text2.length) [text1, text2] = [text2, text1];
    const n = text1.length, m = text2.length;
    let prev = new Array<number>(m + 1).fill(0);
    let curr = new Array<number>(m + 1).fill(0);
    for (let i = 1; i <= n; i++) {
        for (let j = 1; j <= m; j++) {
            if (text1[i - 1] === text2[j - 1]) {
                curr[j] = prev[j - 1]! + 1;
            } else {
                curr[j] = Math.max(prev[j]!, curr[j - 1]!);
            }
        }
        [prev, curr] = [curr, prev];
    }
    return prev[m]!;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(longestCommonSubsequence("abcde", "ace") === 3, "case-1");
check(longestCommonSubsequence("abc", "abc") === 3, "identical");
check(longestCommonSubsequence("abc", "def") === 0, "disjoint");
check(longestCommonSubsequence("", "abc") === 0, "empty-1");
check(longestCommonSubsequence("abc", "") === 0, "empty-2");
check(longestCommonSubsequence("aggtab", "gxtxayb") === 4, "case-hard");

console.log("PASS");
