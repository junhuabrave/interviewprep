"""Trapping Rain Water (two pointers, O(n) time, O(1) space) — Python 3.12."""


def trap(height: list[int]) -> int:
    if len(height) < 3:
        return 0
    l, r = 0, len(height) - 1
    left_max = right_max = 0
    ans = 0
    while l < r:
        if height[l] < height[r]:
            left_max = max(left_max, height[l])
            ans += left_max - height[l]
            l += 1
        else:
            right_max = max(right_max, height[r])
            ans += right_max - height[r]
            r -= 1
    return ans


if __name__ == "__main__":
    cases = [
        ([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1], 6),
        ([4, 2, 0, 3, 2, 5], 9),
        ([], 0),
        ([1], 0),
        ([1, 2, 3], 0),
        ([3, 2, 1], 0),
    ]
    for h, want in cases:
        got = trap(h)
        assert got == want, f"got {got}, want {want} for {h}"
    print("PASS")
