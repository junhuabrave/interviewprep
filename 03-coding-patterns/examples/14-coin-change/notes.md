# Coin Change

## Problem

Given `coins` (denominations, unlimited supply) and an `amount`, return the **minimum number of coins** needed to make that amount, or `-1` if impossible. (LC 322.)

Example: `coins = [1, 2, 5], amount = 11` → `3` (5+5+1).

## Approach: bottom-up DP

Let `dp[a]` = min coins to make amount `a`. Base: `dp[0] = 0`. Otherwise `dp[a] = 1 + min(dp[a - c] for c in coins if c ≤ a)`. Set `dp[a] = ∞` initially; if `dp[amount]` stays infinite, return `-1`.

```
dp[0] = 0
for a in 1..amount:
    dp[a] = min(dp[a-c] + 1 for c in coins if c <= a, default=INF)
```

## Complexity

Let `A = amount`, `C = number of coins`.

| | Time | Space |
|---|---|---|
| | O(A · C) | O(A) |

## Why not greedy?

Greedy ("always use the largest coin that fits") works for some denomination sets (US coins) but **fails in general**. E.g., `coins = [1, 3, 4], amount = 6` → greedy picks 4+1+1 = 3 coins; optimal is 3+3 = 2.

## Edge cases

- `amount = 0` → `0`.
- No coins → `-1` unless amount is 0.
- A coin ≤ 0 — assume valid input; positive denominations only.
- Amount unreachable (e.g., `coins = [2], amount = 3`) → `-1`.

## Variant: count combinations (Coin Change 2, LC 518)

Number of ways to make the amount. **Order coins outside, amounts inside** to avoid counting permutations:

```
ways[0] = 1
for c in coins:
    for a in c..amount:
        ways[a] += ways[a - c]
```

If you swap the loop order, you count permutations (and most likely overflow / overcount).

## Variant: maximum / unbounded knapsack

Same template; replace `min` with `max` and replace "+1" with "+ value[i]" if coins/items have weights and values.
