// Sort Colors (Dutch national flag) — C++20/23
#include <iostream>
#include <vector>
using namespace std;

void sort_colors(vector<int>& nums) {
    int lo = 0, mid = 0, hi = static_cast<int>(nums.size()) - 1;
    while (mid <= hi) {
        switch (nums[mid]) {
            case 0: swap(nums[lo++], nums[mid++]); break;
            case 1: ++mid; break;
            case 2: swap(nums[mid], nums[hi--]); break;     // don't advance mid
        }
    }
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    vector<pair<vector<int>, vector<int>>> cases = {
        {{2,0,2,1,1,0}, {0,0,1,1,2,2}},
        {{2,0,1}, {0,1,2}},
        {{}, {}},
        {{0}, {0}},
        {{1,1,1}, {1,1,1}},
    };
    for (auto& [a, want] : cases) {
        sort_colors(a);
        check(a == want, "sort-colors");
    }
    cout << "PASS\n";
}
