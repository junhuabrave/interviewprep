# Trapping Rain Water

## Problem

Given `n` non-negative integers representing an elevation map (each bar has width 1), compute how much water can be trapped after raining. (LC 42.)

Example: `[0,1,0,2,1,0,1,3,2,1,2,1]` → `6`.

## Key insight

For each index `i`, the water above it is `min(maxLeft[i], maxRight[i]) - height[i]` (clamped at 0).

## Approach: two pointers (O(n) time, O(1) space)

Maintain `l` and `r` from both ends, plus `leftMax` and `rightMax`. At each step, the side with the **smaller current bar** determines the answer at that index — its max-on-that-side is finalized.

```
while l < r:
    if height[l] < height[r]:
        leftMax = max(leftMax, height[l])
        ans += leftMax - height[l]
        l += 1
    else:
        rightMax = max(rightMax, height[r])
        ans += rightMax - height[r]
        r -= 1
```

## Alternative approaches

- **Prefix max + suffix max arrays**: O(n) time, O(n) space.
- **Monotonic stack**: O(n) time. Push indices; pop when current bar is taller, accumulating water layer-by-layer.
- **Brute force**: for each index, find max-left and max-right. O(n²).

## Complexity

| Approach | Time | Space |
|---|---|---|
| Two pointers | O(n) | O(1) |
| Prefix/suffix arrays | O(n) | O(n) |
| Monotonic stack | O(n) | O(n) |
| Brute force | O(n²) | O(1) |

## Edge cases

- Length < 3 → 0 (can't trap).
- Strictly increasing or decreasing → 0.
- Plateau on top — handled by `≤` vs `<` choice in the comparison.
