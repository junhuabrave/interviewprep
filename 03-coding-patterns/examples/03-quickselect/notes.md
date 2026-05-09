# Kth Largest Element (Quickselect)

## Problem

Given an unsorted array `nums` and integer `k`, return the **kth largest** element. (LC 215.)

Example: `nums = [3,2,1,5,6,4], k = 2` → `5`.

## Approaches

### 1. Sort: O(n log n)
Sort and return `nums[-k]`. Simple but suboptimal.

### 2. Min-heap of size k: O(n log k)
Push all, keep heap size ≤ k by popping the smallest when it grows. Final heap top = kth largest. Best when k ≪ n.

### 3. Quickselect: O(n) average
Same partition step as quicksort, but recurse only into the side containing the target index. Pick a random pivot to avoid O(n²) worst case on sorted input.

## Complexity

| Approach | Time avg | Time worst | Space |
|---|---|---|---|
| Sort | O(n log n) | O(n log n) | O(1) (in-place) |
| Heap | O(n log k) | O(n log k) | O(k) |
| Quickselect | O(n) | O(n²) | O(log n) recursion |

## Quickselect mechanics

1. Pick a pivot (random index).
2. Partition into `< pivot`, `== pivot`, `> pivot` (Lomuto/Hoare both work; 3-way handles duplicates well).
3. The kth largest sits in the partition where its index lies. Recurse only there.

For "kth largest", convert to "kth smallest from the end" or partition descending.

## Edge cases

- Duplicates → use 3-way partition or guarantee progress.
- `k = 1` (largest) or `k = n` (smallest) — quickselect still works.
- Empty array / `k > n` — clarify with interviewer; we assume valid input.
