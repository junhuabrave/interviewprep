"""Coin Change — Python 3.12. Bottom-up DP."""


def coin_change(coins: list[int], amount: int) -> int:
    INF = amount + 1                      # any unreachable sentinel > amount
    dp = [INF] * (amount + 1)
    dp[0] = 0
    for a in range(1, amount + 1):
        for c in coins:
            if c <= a and dp[a - c] + 1 < dp[a]:
                dp[a] = dp[a - c] + 1
    return -1 if dp[amount] == INF else dp[amount]


def coin_change_combos(coins: list[int], amount: int) -> int:
    """Count combinations summing to amount."""
    ways = [0] * (amount + 1)
    ways[0] = 1
    for c in coins:                       # outer loop coins -> combos, not perms
        for a in range(c, amount + 1):
            ways[a] += ways[a - c]
    return ways[amount]


if __name__ == "__main__":
    cases_min = [
        ([1, 2, 5], 11, 3),
        ([2], 3, -1),
        ([1], 0, 0),
        ([1], 2, 2),
        ([1, 3, 4], 6, 2),                # greedy would give 3
        ([186, 419, 83, 408], 6249, 20),
    ]
    for coins, amt, want in cases_min:
        got = coin_change(coins, amt)
        assert got == want, f"min: got {got}, want {want} for {coins}, {amt}"

    cases_combo = [
        ([1, 2, 5], 5, 4),
        ([2], 3, 0),
        ([10], 10, 1),
    ]
    for coins, amt, want in cases_combo:
        got = coin_change_combos(coins, amt)
        assert got == want, f"combos: got {got}, want {want}"

    print("PASS")
