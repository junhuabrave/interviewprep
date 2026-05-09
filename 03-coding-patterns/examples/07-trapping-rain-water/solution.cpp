// Trapping Rain Water — C++20/23
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int trap(const vector<int>& height) {
    int n = static_cast<int>(height.size());
    if (n < 3) return 0;
    int l = 0, r = n - 1;
    int left_max = 0, right_max = 0, ans = 0;
    while (l < r) {
        if (height[l] < height[r]) {
            left_max = max(left_max, height[l]);
            ans += left_max - height[l];
            ++l;
        } else {
            right_max = max(right_max, height[r]);
            ans += right_max - height[r];
            --r;
        }
    }
    return ans;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(trap({0,1,0,2,1,0,1,3,2,1,2,1}) == 6, "case-1");
    check(trap({4,2,0,3,2,5}) == 9, "case-2");
    check(trap({}) == 0, "empty");
    check(trap({1}) == 0, "single");
    check(trap({1,2,3}) == 0, "monotone-up");
    check(trap({3,2,1}) == 0, "monotone-down");

    cout << "PASS\n";
}
