"""Sort Colors (Dutch national flag) — Python 3.12."""


def sort_colors(nums: list[int]) -> None:
    """In-place 3-way partition for values 0/1/2."""
    lo, mid, hi = 0, 0, len(nums) - 1
    while mid <= hi:
        match nums[mid]:
            case 0:
                nums[lo], nums[mid] = nums[mid], nums[lo]
                lo += 1; mid += 1
            case 1:
                mid += 1
            case 2:
                nums[mid], nums[hi] = nums[hi], nums[mid]
                hi -= 1
                # don't advance mid — swapped-in value is unknown


if __name__ == "__main__":
    cases = [
        ([2, 0, 2, 1, 1, 0], [0, 0, 1, 1, 2, 2]),
        ([2, 0, 1], [0, 1, 2]),
        ([], []),
        ([0], [0]),
        ([1, 1, 1], [1, 1, 1]),
    ]
    for arr, want in cases:
        sort_colors(arr)
        assert arr == want, f"got {arr}, want {want}"
    print("PASS")
