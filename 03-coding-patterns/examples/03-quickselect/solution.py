"""Kth largest via quickselect — Python 3.12."""
import random


def find_kth_largest(nums: list[int], k: int) -> int:
    """Return the kth largest element. O(n) average."""
    nums = nums[:]                       # don't mutate caller's array
    target = len(nums) - k               # 0-indexed position when sorted ascending

    def select(l: int, r: int) -> int:
        while l < r:
            p = _partition(nums, l, r)
            if p == target: return nums[p]
            if p < target: l = p + 1
            else: r = p - 1
        return nums[l]

    return select(0, len(nums) - 1)


def _partition(nums: list[int], l: int, r: int) -> int:
    """Lomuto partition with random pivot. Returns final pivot index."""
    pi = random.randint(l, r)
    nums[pi], nums[r] = nums[r], nums[pi]
    pivot = nums[r]
    i = l
    for j in range(l, r):
        if nums[j] < pivot:
            nums[i], nums[j] = nums[j], nums[i]
            i += 1
    nums[i], nums[r] = nums[r], nums[i]
    return i


if __name__ == "__main__":
    cases = [
        ([3, 2, 1, 5, 6, 4], 2, 5),
        ([3, 2, 3, 1, 2, 4, 5, 5, 6], 4, 4),
        ([1], 1, 1),
        ([7, 7, 7], 2, 7),
    ]
    for nums, k, want in cases:
        # quickselect is randomized, so run a few times for confidence
        for _ in range(5):
            got = find_kth_largest(nums, k)
            assert got == want, f"got {got}, want {want} for nums={nums}, k={k}"
    print("PASS")
