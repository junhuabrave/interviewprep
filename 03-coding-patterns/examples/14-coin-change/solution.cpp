// Coin Change — C++20/23. Bottom-up DP.
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int coin_change(const vector<int>& coins, int amount) {
    const int INF = amount + 1;
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a) {
        for (int c : coins) {
            if (c <= a && dp[a - c] + 1 < dp[a]) {
                dp[a] = dp[a - c] + 1;
            }
        }
    }
    return dp[amount] == INF ? -1 : dp[amount];
}

long long coin_change_combos(const vector<int>& coins, int amount) {
    vector<long long> ways(amount + 1, 0);
    ways[0] = 1;
    for (int c : coins) {
        for (int a = c; a <= amount; ++a) ways[a] += ways[a - c];
    }
    return ways[amount];
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(coin_change({1,2,5}, 11) == 3, "min-1");
    check(coin_change({2}, 3) == -1, "min-impossible");
    check(coin_change({1}, 0) == 0, "min-zero");
    check(coin_change({1}, 2) == 2, "min-trivial");
    check(coin_change({1,3,4}, 6) == 2, "min-non-greedy");
    check(coin_change({186,419,83,408}, 6249) == 20, "min-hard");

    check(coin_change_combos({1,2,5}, 5) == 4, "combos-1");
    check(coin_change_combos({2}, 3) == 0, "combos-impossible");
    check(coin_change_combos({10}, 10) == 1, "combos-trivial");

    cout << "PASS\n";
}
