// Number of Connected Components — Union-Find. C++20/23.
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

struct UnionFind {
    vector<int> parent, rank_;
    int components;
    explicit UnionFind(int n) : parent(n), rank_(n, 0), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];          // path compression
            x = parent[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) ++rank_[ra];
        --components;
        return true;
    }
};

int count_components(int n, const vector<vector<int>>& edges) {
    UnionFind uf(n);
    for (const auto& e : edges) uf.unite(e[0], e[1]);
    return uf.components;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(count_components(5, {{0,1},{1,2},{3,4}}) == 2, "two-comp");
    check(count_components(5, {{0,1},{1,2},{2,3},{3,4}}) == 1, "linear");
    check(count_components(4, {}) == 4, "no-edges");
    check(count_components(1, {}) == 1, "single");
    check(count_components(3, {{0,0},{1,2}}) == 2, "self-loop");
    check(count_components(3, {{0,1},{0,1}}) == 2, "dup-edge");

    cout << "PASS\n";
}
