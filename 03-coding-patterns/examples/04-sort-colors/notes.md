# Sort Colors (Dutch National Flag)

## Problem

Given an array of integers each equal to 0, 1, or 2, sort them in-place so that 0s come first, then 1s, then 2s. Solve in **one pass** with **O(1) extra space**. (LC 75.)

## Approach: three-pointer (Dutch flag)

Maintain three regions:
- `[0, lo)` — known 0s.
- `[lo, mid)` — known 1s.
- `[mid, hi]` — unknown.
- `(hi, n)` — known 2s.

Walk `mid`:
- `nums[mid] == 0` → swap with `lo`, advance both.
- `nums[mid] == 1` → just advance `mid`.
- `nums[mid] == 2` → swap with `hi`, decrement `hi` (don't advance `mid`, the swapped-in value is unknown).

## Complexity

| | Time | Space |
|---|---|---|
| | O(n) | O(1) |

## Why this matters

Demonstrates a **three-pointer** invariant — easy to mis-step.
Generalizes to any 3-way partition (used in quicksort variants for many duplicates).

## Edge cases

- All zeros / all ones / all twos → still O(n).
- Length 0 or 1 → no work.
- Don't increment `mid` after a `2`-swap; the new value at `mid` could be any color.
