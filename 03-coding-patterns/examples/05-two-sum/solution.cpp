// Two Sum — C++20/23
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

pair<int, int> two_sum(const vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (auto it = seen.find(target - nums[i]); it != seen.end()) {
            return {it->second, i};
        }
        seen[nums[i]] = i;
    }
    return {-1, -1};
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(two_sum({2,7,11,15}, 9) == make_pair(0, 1), "case-1");
    check(two_sum({3,2,4}, 6) == make_pair(1, 2), "case-2");
    check(two_sum({3,3}, 6) == make_pair(0, 1), "duplicates");
    check(two_sum({-1,-2,-3,-4,-5}, -8) == make_pair(2, 4), "negatives");

    cout << "PASS\n";
}
