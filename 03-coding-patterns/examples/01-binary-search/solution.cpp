// Binary search variants — C++20/23
#include <algorithm>
#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>
using namespace std;

int search(const vector<int>& nums, int target) {
    int l = 0, r = static_cast<int>(nums.size()) - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (nums[m] == target) return m;
        if (nums[m] < target) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int lower_bound_idx(const vector<int>& nums, int target) {
    int l = 0, r = static_cast<int>(nums.size());
    while (l < r) {
        int m = l + (r - l) / 2;
        if (nums[m] < target) l = m + 1;
        else r = m;
    }
    return l;
}

int search_rotated(const vector<int>& nums, int target) {
    int l = 0, r = static_cast<int>(nums.size()) - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (nums[m] == target) return m;
        if (nums[l] <= nums[m]) {        // left half sorted
            if (nums[l] <= target && target < nums[m]) r = m - 1;
            else l = m + 1;
        } else {                          // right half sorted
            if (nums[m] < target && target <= nums[r]) l = m + 1;
            else r = m - 1;
        }
    }
    return -1;
}

int main() {
    auto check = [](bool ok, string_view label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(search({1,3,5,7,9}, 5) == 2, "search-found");
    check(search({1,3,5,7,9}, 6) == -1, "search-missing");
    check(search({}, 1) == -1, "search-empty");
    check(lower_bound_idx({1,2,4,4,5}, 4) == 2, "lb-mid");
    check(lower_bound_idx({1,2,4,4,5}, 6) == 5, "lb-end");
    // sanity vs std::lower_bound
    vector<int> v{1,2,4,4,5};
    check(lower_bound_idx(v, 4) == ranges::lower_bound(v, 4) - v.begin(), "lb-vs-std");
    check(search_rotated({4,5,6,7,0,1,2}, 0) == 4, "rot-found");
    check(search_rotated({4,5,6,7,0,1,2}, 3) == -1, "rot-missing");

    cout << "PASS\n";
}
