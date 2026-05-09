"""Longest Common Subsequence — Python 3.12. 2D DP with O(min(n,m)) space."""


def longest_common_subsequence(text1: str, text2: str) -> int:
    if len(text1) < len(text2):
        text1, text2 = text2, text1                   # ensure text2 is shorter
    n, m = len(text1), len(text2)
    prev = [0] * (m + 1)
    curr = [0] * (m + 1)
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if text1[i - 1] == text2[j - 1]:
                curr[j] = prev[j - 1] + 1
            else:
                curr[j] = max(prev[j], curr[j - 1])
        prev, curr = curr, prev
    return prev[m]


def longest_common_subsequence_string(text1: str, text2: str) -> str:
    """Reconstruct the actual subsequence (one valid answer)."""
    n, m = len(text1), len(text2)
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
    out: list[str] = []
    i, j = n, m
    while i > 0 and j > 0:
        if text1[i - 1] == text2[j - 1]:
            out.append(text1[i - 1])
            i -= 1; j -= 1
        elif dp[i - 1][j] >= dp[i][j - 1]:
            i -= 1
        else:
            j -= 1
    return "".join(reversed(out))


if __name__ == "__main__":
    cases = [
        ("abcde", "ace", 3),
        ("abc", "abc", 3),
        ("abc", "def", 0),
        ("", "abc", 0),
        ("abc", "", 0),
        ("aggtab", "gxtxayb", 4),                     # "gtab"
    ]
    for a, b, want in cases:
        got = longest_common_subsequence(a, b)
        assert got == want, f"got {got}, want {want} for ({a!r}, {b!r})"

    s = longest_common_subsequence_string("aggtab", "gxtxayb")
    assert len(s) == 4 and all(ch in "aggtab" and ch in "gxtxayb" for ch in s), s
    print("PASS")
