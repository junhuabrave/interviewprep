// LRU Cache — C++20/23. std::list (DLL) + std::unordered_map.
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
using namespace std;

class LRUCache {
    int cap;
    list<pair<int, int>> dll;          // front = most recent. (key, value).
    unordered_map<int, list<pair<int,int>>::iterator> map;

public:
    explicit LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        dll.splice(dll.begin(), dll, it->second);    // O(1) move-to-front
        return it->second->second;
    }

    void put(int key, int value) {
        if (auto it = map.find(key); it != map.end()) {
            it->second->second = value;
            dll.splice(dll.begin(), dll, it->second);
            return;
        }
        dll.emplace_front(key, value);
        map[key] = dll.begin();
        if (static_cast<int>(map.size()) > cap) {
            map.erase(dll.back().first);
            dll.pop_back();
        }
    }
};

int main() {
    auto check = [](bool ok, const char* label) {
        if (!ok) { cerr << "FAIL: " << label << "\n"; exit(1); }
    };

    {
        LRUCache c(2);
        c.put(1, 1); c.put(2, 2);
        check(c.get(1) == 1, "get-1");
        c.put(3, 3);
        check(c.get(2) == -1, "evicted-2");
        c.put(4, 4);
        check(c.get(1) == -1, "evicted-1");
        check(c.get(3) == 3, "get-3");
        check(c.get(4) == 4, "get-4");
    }
    {
        LRUCache c(1);
        c.put(1, 1); c.put(2, 2);
        check(c.get(1) == -1 && c.get(2) == 2, "cap-1");
    }

    cout << "PASS\n";
}
