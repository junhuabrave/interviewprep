// Group Anagrams — C++20/23
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<string>> group_anagrams(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (const auto& s : strs) {
        array<int, 26> count{};
        for (char c : s) ++count[c - 'a'];
        string key;
        key.reserve(26 * 4);
        for (int i = 0; i < 26; ++i) {
            key += '#';
            key += to_string(count[i]);
        }
        groups[key].push_back(s);
    }
    vector<vector<string>> out;
    out.reserve(groups.size());
    for (auto& [_, v] : groups) out.push_back(std::move(v));
    return out;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    auto canon = [](vector<vector<string>> v) {
        for (auto& g : v) ranges::sort(g);
        ranges::sort(v);
        return v;
    };

    auto got = canon(group_anagrams({"eat","tea","tan","ate","nat","bat"}));
    auto want = canon(vector<vector<string>>{{"eat","tea","ate"}, {"tan","nat"}, {"bat"}});
    check(got == want, "main-case");

    check(group_anagrams({}).empty(), "empty");
    auto got2 = canon(group_anagrams({""}));
    check(got2 == vector<vector<string>>{{""}}, "empty-string");

    cout << "PASS\n";
}
