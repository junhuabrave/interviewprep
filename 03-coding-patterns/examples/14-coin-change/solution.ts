// Coin Change — TypeScript. Bottom-up DP.
function coinChange(coins: number[], amount: number): number {
    const INF = amount + 1;
    const dp = new Array<number>(amount + 1).fill(INF);
    dp[0] = 0;
    for (let a = 1; a <= amount; a++) {
        for (const c of coins) {
            if (c <= a && dp[a - c]! + 1 < dp[a]!) {
                dp[a] = dp[a - c]! + 1;
            }
        }
    }
    return dp[amount] === INF ? -1 : dp[amount]!;
}

function coinChangeCombos(coins: number[], amount: number): number {
    const ways = new Array<number>(amount + 1).fill(0);
    ways[0] = 1;
    for (const c of coins) {
        for (let a = c; a <= amount; a++) {
            ways[a]! += ways[a - c]!;
        }
    }
    return ways[amount]!;
}

function check(ok: boolean, label: string): void {
    if (!ok) { console.error(`FAIL: ${label}`); process.exit(1); }
}

check(coinChange([1, 2, 5], 11) === 3, "min-1");
check(coinChange([2], 3) === -1, "min-impossible");
check(coinChange([1], 0) === 0, "min-zero");
check(coinChange([1], 2) === 2, "min-trivial");
check(coinChange([1, 3, 4], 6) === 2, "min-non-greedy");
check(coinChange([186, 419, 83, 408], 6249) === 20, "min-hard");

check(coinChangeCombos([1, 2, 5], 5) === 4, "combos-1");
check(coinChangeCombos([2], 3) === 0, "combos-impossible");
check(coinChangeCombos([10], 10) === 1, "combos-trivial");

console.log("PASS");
