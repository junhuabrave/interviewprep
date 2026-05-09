// Number of Islands — C++20/23 (BFS)
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int num_islands(vector<vector<char>> grid) {        // by value: we mark in-place
    if (grid.empty() || grid[0].empty()) return 0;
    int R = static_cast<int>(grid.size());
    int C = static_cast<int>(grid[0].size());
    static const int DR[4] = {-1, 1, 0, 0};
    static const int DC[4] = {0, 0, -1, 1};

    int count = 0;
    queue<pair<int,int>> q;
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] != '1') continue;
            ++count;
            q.push({r, c});
            grid[r][c] = '0';
            while (!q.empty()) {
                auto [cr, cc] = q.front(); q.pop();
                for (int k = 0; k < 4; ++k) {
                    int nr = cr + DR[k], nc = cc + DC[k];
                    if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] == '1') {
                        grid[nr][nc] = '0';
                        q.push({nr, nc});
                    }
                }
            }
        }
    }
    return count;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    vector<vector<char>> g1 = {
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'},
    };
    vector<vector<char>> g2 = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'},
    };
    check(num_islands(g1) == 1, "case-1");
    check(num_islands(g2) == 3, "case-2");
    check(num_islands({}) == 0, "empty");
    check(num_islands({{'0','0'},{'0','0'}}) == 0, "all-water");

    cout << "PASS\n";
}
