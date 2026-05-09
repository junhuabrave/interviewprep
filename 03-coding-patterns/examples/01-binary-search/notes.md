# Binary Search

## Problem

Given a sorted array of distinct integers `nums` and a target, return the index of `target` in `nums`, or `-1` if not present.

Variants we cover:
- `search` — exact match.
- `lower_bound` — first index `i` with `nums[i] >= target` (insertion point).
- `search_rotated` — search in a sorted array that has been rotated.

## Approach

Maintain `[l, r]` (inclusive) or `[l, r)` (half-open) as the candidate range. At each step, compare `nums[mid]` to the target and discard half.

Invariant: the answer, if it exists, lies in the current range.

## Complexity

| | Time | Space |
|---|---|---|
| All variants | O(log n) | O(1) |

## Edge cases

- Empty array → return `-1` (or `0` for `lower_bound`).
- Target smaller than all / larger than all.
- Duplicates: standard `search` returns *some* matching index; use `bisect_left` for the first.
- Overflow: use `l + (r-l)/2` instead of `(l+r)/2` in C++/Java for very large indices.

## Variations to be ready for

- "First/last occurrence" → lower_bound for target, upper_bound for target+1.
- "Search in rotated sorted array" → identify which half is sorted, then decide which half contains the target.
- "Binary search on the answer" → search over a value range, with a feasibility predicate.
