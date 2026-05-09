# Merge Sort

## Problem

Sort an integer array using **merge sort**. Demonstrates divide-and-conquer.

## Approach

1. Divide the array in half.
2. Recursively sort each half.
3. Merge the two sorted halves into one.

The merge step uses two pointers walking the two halves, picking the smaller front element each time.

## Complexity

| | Time | Space |
|---|---|---|
| Best / Avg / Worst | O(n log n) | O(n) auxiliary |

## Why know merge sort

- **Stable** (quicksort is not).
- O(n log n) **worst case** (quicksort is O(n²) worst).
- Natural fit for **linked lists** (no random access needed).
- Foundation for **count-inversions** problem (O(n log n)).
- External sort (sorting data that doesn't fit in memory).

## Edge cases

- Empty / single element → return as-is.
- Already sorted → still O(n log n) (unlike Timsort which detects runs).
- Stability matters when the array contains pairs and you sort by one field.

## Variant: count inversions

During the merge, when you take from the right half while the left half still has elements, those remaining left-half elements form inversions with the chosen right element. Add `len(left) - i` to a counter.
