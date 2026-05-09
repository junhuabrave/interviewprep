// Course Schedule II — C++20/23
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

vector<int> find_order(int num_courses, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(num_courses);
    vector<int> indeg(num_courses, 0);
    for (const auto& e : prereqs) {                 // e = [a, b]: b -> a
        graph[e[1]].push_back(e[0]);
        ++indeg[e[0]];
    }
    queue<int> q;
    for (int i = 0; i < num_courses; ++i)
        if (indeg[i] == 0) q.push(i);
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : graph[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }
    if (static_cast<int>(order.size()) != num_courses) return {};
    return order;
}

bool can_finish(int n, const vector<vector<int>>& p) {
    return static_cast<int>(find_order(n, p).size()) == n;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(can_finish(2, {{1,0}}) == true, "linear");
    check(can_finish(2, {{1,0},{0,1}}) == false, "cycle");
    check(can_finish(4, {{1,0},{2,0},{3,1},{3,2}}) == true, "diamond");
    check(can_finish(1, {}) == true, "single");
    check(can_finish(3, {{0,0}}) == false, "self-loop");

    cout << "PASS\n";
}
