# Heaps & Priority Queues

## Definition

A binary heap is a complete binary tree where each parent satisfies a heap property:
- **Min-heap**: parent ≤ children → root is min.
- **Max-heap**: parent ≥ children → root is max.

Stored as an array: parent of `i` is `(i-1)//2`, children are `2i+1`, `2i+2`.

## Operations

| Operation | Time |
|-----------|------|
| Peek (min/max) | O(1) |
| Push | O(log n) |
| Pop | O(log n) |
| Heapify (build from array) | O(n) |
| Heap sort | O(n log n) |

## When to use

- "Top K" problems (smallest, largest, most frequent).
- "K-way merge" (merge K sorted lists).
- Streaming median (two heaps).
- Dijkstra, Prim's MST.
- Schedulers, task queues with priorities.

## Top K pattern

To find **K smallest**, use a **max-heap of size K** — pop when size exceeds K. The max of the heap is "the worst of the kept set"; if a new element is smaller than that max, it deserves a spot.

(Symmetric for K largest with min-heap.)

```python
import heapq
def top_k_largest(arr, k):
    h = []
    for x in arr:
        heapq.heappush(h, x)        # min-heap
        if len(h) > k:
            heapq.heappop(h)
    return h     # k largest
```

## Streaming median (two heaps)

- Max-heap of lower half, min-heap of upper half.
- Maintain `len(lower) == len(upper)` or `len(lower) == len(upper) + 1`.
- Median = top of lower (odd total) or avg of tops (even total).

## K-way merge

Push the head of each of the K lists into a min-heap (with `(value, list_index, node_index)`). Pop smallest, advance that list, repeat. O(N log K).

## Language notes

| Language | Min-heap | Max-heap |
|---|---|---|
| Python | `heapq` (min) | Negate values, or `heapq._heapify_max` (private) |
| C++ | `std::priority_queue<T, vector<T>, greater<T>>` | `std::priority_queue<T>` (default = max) |
| TypeScript | None built-in — write your own or use a library |

Python's `heapq` is **min only**. To get max-heap of ints, push `-x`.

## Common gotchas

- **Tuple comparison**: heaps compare tuples lexicographically. Add an index as a tiebreaker if your objects aren't comparable: `(priority, count, item)`.
- **Modifying items inside the heap** breaks the invariant. Re-push or use a "lazy delete" set.
- **C++ `priority_queue::pop()`** doesn't return the element — call `top()` first.

## Top problems

- Kth Largest Element in an Array (heap or quickselect)
- Top K Frequent Elements
- K Closest Points to Origin
- Merge K Sorted Lists
- Find Median from Data Stream
- Task Scheduler
- Reorganize String
- Smallest Range Covering Elements from K Lists
- Sliding Window Median
