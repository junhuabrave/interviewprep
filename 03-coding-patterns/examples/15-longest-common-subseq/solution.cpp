// Longest Common Subsequence — C++20/23. 2D DP with rolling rows.
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

int longest_common_subsequence(string_view a, string_view b) {
    if (a.size() < b.size()) swap(a, b);
    int n = static_cast<int>(a.size()), m = static_cast<int>(b.size());
    vector<int> prev(m + 1, 0), curr(m + 1, 0);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) curr[j] = prev[j - 1] + 1;
            else curr[j] = max(prev[j], curr[j - 1]);
        }
        swap(prev, curr);
    }
    return prev[m];
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(longest_common_subsequence("abcde", "ace") == 3, "case-1");
    check(longest_common_subsequence("abc", "abc") == 3, "identical");
    check(longest_common_subsequence("abc", "def") == 0, "disjoint");
    check(longest_common_subsequence("", "abc") == 0, "empty-1");
    check(longest_common_subsequence("abc", "") == 0, "empty-2");
    check(longest_common_subsequence("aggtab", "gxtxayb") == 4, "case-hard");

    cout << "PASS\n";
}
