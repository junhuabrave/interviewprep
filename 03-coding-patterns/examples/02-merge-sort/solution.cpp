// Merge sort — C++20/23
#include <iostream>
#include <vector>
using namespace std;

static void merge(vector<int>& a, int l, int m, int r, vector<int>& buf) {
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) buf[k++] = a[i++];
        else buf[k++] = a[j++];
    }
    while (i < m) buf[k++] = a[i++];
    while (j < r) buf[k++] = a[j++];
    for (int t = l; t < r; ++t) a[t] = buf[t];
}

static void merge_sort_rec(vector<int>& a, int l, int r, vector<int>& buf) {
    if (r - l <= 1) return;
    int m = l + (r - l) / 2;
    merge_sort_rec(a, l, m, buf);
    merge_sort_rec(a, m, r, buf);
    merge(a, l, m, r, buf);
}

void merge_sort(vector<int>& a) {
    vector<int> buf(a.size());
    merge_sort_rec(a, 0, static_cast<int>(a.size()), buf);
}

long long count_inversions(vector<int> a) {
    long long total = 0;
    vector<int> buf(a.size());

    auto sort_count = [&](auto& self, int l, int r) -> void {
        if (r - l <= 1) return;
        int m = l + (r - l) / 2;
        self(self, l, m); self(self, m, r);
        int i = l, j = m, k = l;
        while (i < m && j < r) {
            if (a[i] <= a[j]) buf[k++] = a[i++];
            else { buf[k++] = a[j++]; total += (m - i); }
        }
        while (i < m) buf[k++] = a[i++];
        while (j < r) buf[k++] = a[j++];
        for (int t = l; t < r; ++t) a[t] = buf[t];
    };
    sort_count(sort_count, 0, static_cast<int>(a.size()));
    return total;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    {
        vector<int> a{3,1,4,1,5,9,2,6,5,3,5};
        vector<int> want{1,1,2,3,3,4,5,5,5,6,9};
        merge_sort(a);
        check(a == want, "sort-mixed");
    }
    {
        vector<int> a{};
        merge_sort(a);
        check(a.empty(), "sort-empty");
    }
    check(count_inversions({2,4,1,3,5}) == 3, "inversions");

    cout << "PASS\n";
}
