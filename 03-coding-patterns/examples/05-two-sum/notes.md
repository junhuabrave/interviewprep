# Two Sum

## Problem

Given an array `nums` and a `target`, return indices `(i, j)` such that `nums[i] + nums[j] == target`. Each input has exactly one solution; you may not use the same element twice. (LC 1.)

## Approach

Single pass with a **hashmap** of seen values → index. For each `x`, check if `target - x` is already in the map; if yes, we found the pair. Otherwise insert `x → i`.

This is the canonical "complement lookup" pattern.

## Complexity

| | Time | Space |
|---|---|---|
| | O(n) | O(n) |

## Variants to know

- **Sorted input → two pointers, O(1) extra space.** (LC 167.)
- **Return values, not indices** — easier; sort + two pointers.
- **All pairs / count pairs** — accumulate into a running answer.
- **3Sum / 4Sum** — sort + nested loop + two pointers; deduplicate carefully.

## Edge cases

- Duplicates: `nums = [3, 3], target = 6` → `(0, 1)`. The "is the complement in the map *before* I insert this index?" check handles this.
- No valid pair → problem says guaranteed; be ready to clarify.
- Negative numbers / zero — works the same.
