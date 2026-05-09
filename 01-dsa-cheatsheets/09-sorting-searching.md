# Sorting & Searching

## When to sort

- "Find pair with property X" → often O(n log n) sort + O(n) two-pointer beats O(n²) brute force.
- "Find duplicates / kth element / median" → sometimes.
- "Schedule / interval merging" → almost always.

Sorting is rarely the answer when an O(n) hashmap or counting works.

## Custom comparators

```python
arr.sort(key=lambda x: (x.priority, -x.timestamp))
# Or with cmp_to_key for non-key-style comparison:
from functools import cmp_to_key
arr.sort(key=cmp_to_key(lambda a, b: ...))
```

```cpp
std::ranges::sort(v, [](auto& a, auto& b) { return a.priority < b.priority; });
```

```ts
arr.sort((a, b) => a.priority - b.priority);   // BEWARE for floats / overflow
```

## Binary search templates

### Standard (find exact match)
```python
def bsearch(arr, target):
    l, r = 0, len(arr) - 1
    while l <= r:
        m = l + (r - l) // 2
        if arr[m] == target: return m
        if arr[m] < target: l = m + 1
        else: r = m - 1
    return -1
```

### Lower bound (first index where arr[i] >= target)
```python
def lower_bound(arr, target):
    l, r = 0, len(arr)
    while l < r:
        m = (l + r) // 2
        if arr[m] < target: l = m + 1
        else: r = m
    return l   # could equal len(arr)
```

In Python: `bisect.bisect_left(arr, target)`.
In C++: `std::lower_bound(v.begin(), v.end(), target)`.

### Binary search on the answer
For "minimum X such that property P(X) holds" or "maximum X such that ...":

```python
l, r = lo, hi
while l < r:
    m = (l + r) // 2
    if feasible(m):
        r = m       # try smaller
    else:
        l = m + 1
return l
```

Examples: Koko Eating Bananas, Capacity to Ship Packages, Split Array Largest Sum.

## Quickselect (kth element in O(n) avg)

Partition like quicksort, but recurse only into the side containing the target index. Average O(n), worst O(n²) (mitigate with random pivot).

```python
def quickselect(arr, k):    # k is 0-indexed
    if len(arr) == 1: return arr[0]
    pivot = arr[len(arr)//2]
    lows  = [x for x in arr if x < pivot]
    highs = [x for x in arr if x > pivot]
    pivots = [x for x in arr if x == pivot]
    if k < len(lows):           return quickselect(lows, k)
    if k < len(lows)+len(pivots): return pivot
    return quickselect(highs, k - len(lows) - len(pivots))
```

## Common sorts (see also `00-complexity.md`)

- **Merge sort**: stable, O(n log n) worst, O(n) extra. Used for linked lists, external sort.
- **Quicksort**: in-place, O(n log n) avg, O(n²) worst. Pivot strategy matters.
- **Heap sort**: O(n log n) worst, O(1) extra, not stable.
- **Counting / radix sort**: O(n+k) when keys are small integers.
- Real languages use **Timsort** (Python `sort`, JS `Array.sort` in V8) or **introsort** (C++ `std::sort`).

## Stability

A sort is **stable** if equal keys keep their relative order. Matters when sorting by multiple keys with separate passes (sort by secondary key first, then by primary — only works if stable).

## Common gotchas

- `(l + r) // 2` overflows in C++ for very large ints — use `l + (r-l)/2`.
- JS default `Array.sort()` sorts as **strings** unless you give a comparator — `[10, 2].sort()` returns `[10, 2]`.
- Off-by-one in binary search: `<=` vs `<`, `m+1` vs `m`. Pick a template and stick with it.
- Quickselect on already-sorted input with poor pivot is O(n²).
- "Count" operations: `bisect_right - bisect_left` counts duplicates.

## Top problems

- Binary Search / Search Insert Position
- Search in Rotated Sorted Array (I + II)
- Find First and Last Position of Element
- Find Minimum in Rotated Sorted Array
- Median of Two Sorted Arrays
- Kth Largest Element in an Array (heap or quickselect)
- Koko Eating Bananas / Capacity to Ship / Split Array Largest Sum
- Sort Colors (Dutch national flag)
- Merge Intervals / Insert Interval
- Meeting Rooms II
