# C++ 20/23/26 Cheat Sheet

Modern C++ idioms expected in mid-level interviews. Compile with `g++ -std=c++23` (or `c++20` if 23 unavailable).

## Boilerplate

```cpp
#include <bits/stdc++.h>     // includes everything; great for interviews
using namespace std;          // tolerated in interview code

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // ...
}
```

In production, prefer named includes (`<vector>`, `<algorithm>`, ...) and avoid `using namespace std`.

## Containers (STL)

| Container | Underlying | Notes |
|-----------|-----------|-------|
| `vector<T>` | dynamic array | O(1) amortized push_back |
| `array<T, N>` | fixed array | C-array with iterators |
| `deque<T>` | segmented array | O(1) push/pop both ends |
| `list<T>` | doubly linked list | O(1) splice |
| `forward_list<T>` | singly linked | rare in interviews |
| `stack<T>` | adapter on deque | `top`, `push`, `pop` |
| `queue<T>` | adapter on deque | `front`, `back`, `push`, `pop` |
| `priority_queue<T>` | heap on vector | max-heap by default |
| `set<T>` / `map<K,V>` | red-black tree | O(log n), ordered |
| `unordered_set/map` | hash table | O(1) avg |
| `multiset / multimap` | duplicates allowed | |

```cpp
priority_queue<int> max_heap;
priority_queue<int, vector<int>, greater<int>> min_heap;     // min-heap

unordered_map<int, int> count;
for (int x : arr) ++count[x];          // value-init to 0

auto& [key, val] = *count.begin();     // structured bindings (C++17)
```

## Smart pointers

```cpp
auto p  = make_unique<Node>(1);     // unique ownership, no copy
auto sp = make_shared<Node>(1);     // ref-counted
weak_ptr<Node> w = sp;              // breaks shared_ptr cycles
```

Never use `new` / `delete` directly. `unique_ptr` is the default.

## References vs pointers

- **Reference** (`T&`): non-null, can't rebind. Use for function params, range-for.
- **Pointer** (`T*`): can be null, can re-assign. Use for "may be absent" or arrays.
- **Const ref** (`const T&`): pass-by-reference without copy or mutation. Default for non-trivial types.

```cpp
void f(const vector<int>& v);     // no copy
for (const auto& x : v) {...}     // no copy in loop
```

## Move semantics & rvalue refs

```cpp
vector<int> v = build_huge_vector();    // RVO, no copy
sink(std::move(v));                     // explicitly move; v is now valid but unspecified
```

Rule of 5: if you write any of `~`, copy ctor, copy assign, move ctor, move assign, you usually need to define (or `=delete` / `=default`) all of them.

## Lambdas

```cpp
auto add = [](int a, int b) { return a + b; };
auto bound_add = [k](int x) { return x + k; };           // capture by value
auto mut = [&](int x) mutable { sum += x; return sum; }; // capture by ref, mutable
```

Generic lambdas with `auto`:
```cpp
auto cmp = [](const auto& a, const auto& b) { return a.first < b.first; };
```

## Algorithms (`<algorithm>` + `<ranges>`)

C++20 ranges make algorithms cleaner — no iterator pairs needed:

```cpp
#include <ranges>
namespace rv = std::views;

vector<int> v = {3,1,4,1,5,9,2,6};
ranges::sort(v);
ranges::sort(v, greater<>());
auto it = ranges::find(v, 5);
auto k  = ranges::count(v, 1);
auto m  = ranges::max(v);

// views are lazy
for (int x : v | rv::filter([](int x){return x%2==0;}) | rv::transform([](int x){return x*x;}))
    cout << x << "\n";
```

C++23 / 26 additions (compiler-dependent):
- `std::print`, `std::println` (C++23) instead of `cout`.
- `std::expected<T, E>` (C++23) for fallible returns.
- `std::flat_map`, `std::flat_set` (C++23).
- Static `operator()` / `operator[]` (C++23).

```cpp
#include <print>
std::println("{} + {} = {}", a, b, a+b);   // c++23
```

If `<print>` is missing, fall back to `std::format`:
```cpp
#include <format>
cout << std::format("{} + {} = {}\n", a, b, a+b);
```

## Strings

```cpp
string s = "hello";
s.size(); s.empty(); s[0]; s.front(); s.back();
s.substr(2, 3);                  // "llo"
s.find("ell");                   // returns size_t, npos if not found
s += " world";
to_string(42); stoi("42"); stoll("123456789012");
```

`string_view` (C++17) is a non-owning reference to characters — pass by value, very cheap.

## Concepts (C++20)

```cpp
#include <concepts>
template <std::integral T>
T abs_val(T x) { return x < 0 ? -x : x; }
```

Define your own:
```cpp
template <class T>
concept Numeric = std::integral<T> || std::floating_point<T>;
```

## Common gotchas

- **Integer overflow**: `int` is 32-bit. For sums of arrays use `long long`.
- **`size()` returns `size_t`** (unsigned). `for (int i = 0; i < v.size() - 1; ...)` underflows on empty `v`. Cast or use `ssize(v)` (C++20).
- **`auto` type deduction**: `auto x = v[0]` copies; use `auto&` for reference.
- **Iterator invalidation**: `vector::push_back` may invalidate all iterators on reallocation.
- **`unordered_map` worst case** is O(n) on hash collisions. Don't rely on O(1) under adversarial input.
- **`endl` flushes** the buffer — slow in loops. Use `"\n"`.
- **Range-for over a temporary**: `for (auto x : f().items())` may dangle after C++20 fixes (still be careful).

## Quick I/O

```cpp
int n; cin >> n;
vector<int> a(n);
for (auto& x : a) cin >> x;

for (int x : a) cout << x << ' ';
cout << '\n';
```
