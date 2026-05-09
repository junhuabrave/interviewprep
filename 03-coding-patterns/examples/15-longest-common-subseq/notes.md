# Longest Common Subsequence (LCS)

## Problem

Given two strings `text1` and `text2`, return the length of their longest **subsequence** common to both. A subsequence preserves order but may skip characters. (LC 1143.)

Example: `"abcde"`, `"ace"` → `3` (`"ace"`).

## Approach: 2D DP

Let `dp[i][j]` = length of the LCS of `text1[:i]` and `text2[:j]`. Recurrence:

```
dp[i][j] = dp[i-1][j-1] + 1                if text1[i-1] == text2[j-1]
         = max(dp[i-1][j], dp[i][j-1])     otherwise
```

Base: `dp[0][*] = dp[*][0] = 0`.

## Complexity

| | Time | Space |
|---|---|---|
| 2D DP | O(n · m) | O(n · m) |
| Space-optimized | O(n · m) | O(min(n, m)) |

## Space optimization

Only the current and previous row are needed → keep two 1D arrays of length `m+1`. Halve space again with a single rolling array if you're careful with the order.

## Edge cases

- Either string empty → 0.
- Identical strings → length n.
- No characters in common → 0.

## Reconstructing the LCS string

After filling `dp`, walk from `(n, m)` backwards:
- If `text1[i-1] == text2[j-1]`: prepend that character; move diagonally `(i-1, j-1)`.
- Else: move toward the larger of `dp[i-1][j]` and `dp[i][j-1]`.

## Variants in the same DP family

- **Edit Distance** (LC 72): Levenshtein, with insert/delete/replace operations.
- **Longest Common Substring** (contiguous!): same table but reset to 0 on mismatch; track the global max.
- **Longest Palindromic Subsequence** (LC 516): LCS of the string with its reverse.
- **Distinct Subsequences** (LC 115): count subsequences of s that equal t.
- **Shortest Common Supersequence** (LC 1092): `n + m - LCS_length`.
