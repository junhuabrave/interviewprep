// Longest Substring Without Repeating Characters — C++20/23
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int length_of_longest_substring(const string& s) {
    unordered_map<char, int> seen;
    int l = 0, best = 0;
    for (int r = 0; r < static_cast<int>(s.size()); ++r) {
        if (auto it = seen.find(s[r]); it != seen.end() && it->second >= l) {
            l = it->second + 1;
        }
        seen[s[r]] = r;
        best = max(best, r - l + 1);
    }
    return best;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(length_of_longest_substring("abcabcbb") == 3, "case-1");
    check(length_of_longest_substring("bbbbb") == 1, "all-same");
    check(length_of_longest_substring("pwwkew") == 3, "case-3");
    check(length_of_longest_substring("") == 0, "empty");
    check(length_of_longest_substring("a") == 1, "single");
    check(length_of_longest_substring("dvdf") == 3, "tricky-1");
    check(length_of_longest_substring("abba") == 2, "tricky-2");

    cout << "PASS\n";
}
