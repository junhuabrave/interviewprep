"""Binary search variants — Python 3.12."""
from bisect import bisect_left


def search(nums: list[int], target: int) -> int:
    l, r = 0, len(nums) - 1
    while l <= r:
        m = l + (r - l) // 2
        if nums[m] == target:
            return m
        if nums[m] < target:
            l = m + 1
        else:
            r = m - 1
    return -1


def lower_bound(nums: list[int], target: int) -> int:
    """First index i with nums[i] >= target."""
    l, r = 0, len(nums)
    while l < r:
        m = (l + r) // 2
        if nums[m] < target:
            l = m + 1
        else:
            r = m
    return l


def search_rotated(nums: list[int], target: int) -> int:
    l, r = 0, len(nums) - 1
    while l <= r:
        m = (l + r) // 2
        if nums[m] == target:
            return m
        if nums[l] <= nums[m]:           # left half sorted
            if nums[l] <= target < nums[m]:
                r = m - 1
            else:
                l = m + 1
        else:                            # right half sorted
            if nums[m] < target <= nums[r]:
                l = m + 1
            else:
                r = m - 1
    return -1


if __name__ == "__main__":
    cases = [
        (search([1, 3, 5, 7, 9], 5), 2),
        (search([1, 3, 5, 7, 9], 6), -1),
        (search([], 1), -1),
        (lower_bound([1, 2, 4, 4, 5], 4), 2),
        (lower_bound([1, 2, 4, 4, 5], 6), 5),
        (search_rotated([4, 5, 6, 7, 0, 1, 2], 0), 4),
        (search_rotated([4, 5, 6, 7, 0, 1, 2], 3), -1),
    ]
    # cross-check lower_bound against bisect
    for arr, t in [([1, 2, 4, 4, 5], 4), ([], 1), ([5], 1)]:
        assert lower_bound(arr, t) == bisect_left(arr, t)

    for got, want in cases:
        assert got == want, f"got {got}, want {want}"
    print("PASS")
