// Kth largest via quickselect — C++20/23
#include <iostream>
#include <random>
#include <vector>
using namespace std;

static mt19937 rng{random_device{}()};

static int partition_(vector<int>& a, int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    int pi = dist(rng);
    swap(a[pi], a[r]);
    int pivot = a[r], i = l;
    for (int j = l; j < r; ++j) {
        if (a[j] < pivot) swap(a[i++], a[j]);
    }
    swap(a[i], a[r]);
    return i;
}

int find_kth_largest(vector<int> nums, int k) {
    int target = static_cast<int>(nums.size()) - k;     // 0-indexed in ascending order
    int l = 0, r = static_cast<int>(nums.size()) - 1;
    while (l < r) {
        int p = partition_(nums, l, r);
        if (p == target) return nums[p];
        if (p < target) l = p + 1;
        else r = p - 1;
    }
    return nums[l];
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    vector<tuple<vector<int>, int, int>> cases = {
        {{3,2,1,5,6,4}, 2, 5},
        {{3,2,3,1,2,4,5,5,6}, 4, 4},
        {{1}, 1, 1},
        {{7,7,7}, 2, 7},
    };
    for (auto& [nums, k, want] : cases) {
        for (int t = 0; t < 5; ++t) {
            int got = find_kth_largest(nums, k);
            check(got == want, "kth-largest");
        }
    }
    cout << "PASS\n";
}
