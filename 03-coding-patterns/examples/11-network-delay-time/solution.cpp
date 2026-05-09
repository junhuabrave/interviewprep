// Network Delay Time (Dijkstra) — C++20/23
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int network_delay_time(const vector<vector<int>>& times, int n, int k) {
    vector<vector<pair<int,int>>> graph(n + 1);
    for (const auto& e : times) graph[e[0]].push_back({e[1], e[2]});

    vector<int> dist(n + 1, INT_MAX);
    dist[k] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, k});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& [v, w] : graph[u]) {
            int nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    int longest = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] == INT_MAX) return -1;
        longest = max(longest, dist[i]);
    }
    return longest;
}

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    check(network_delay_time({{2,1,1},{2,3,1},{3,4,1}}, 4, 2) == 2, "case-1");
    check(network_delay_time({{1,2,1}}, 2, 1) == 1, "case-2");
    check(network_delay_time({{1,2,1}}, 2, 2) == -1, "unreachable");
    check(network_delay_time({{1,2,1},{2,1,3}}, 2, 1) == 1, "round-trip");

    cout << "PASS\n";
}
