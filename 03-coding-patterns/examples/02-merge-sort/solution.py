"""Merge sort — Python 3.12."""


def merge_sort(arr: list[int]) -> list[int]:
    if len(arr) <= 1:
        return arr[:]
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    return _merge(left, right)


def _merge(left: list[int], right: list[int]) -> list[int]:
    out: list[int] = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:          # <= preserves stability
            out.append(left[i]); i += 1
        else:
            out.append(right[j]); j += 1
    out.extend(left[i:])
    out.extend(right[j:])
    return out


def count_inversions(arr: list[int]) -> int:
    """Count pairs (i, j) with i<j and arr[i] > arr[j]."""

    def sort_count(a: list[int]) -> tuple[list[int], int]:
        if len(a) <= 1: return a[:], 0
        m = len(a) // 2
        left, lc = sort_count(a[:m])
        right, rc = sort_count(a[m:])
        merged, mc = [], 0
        i = j = 0
        while i < len(left) and j < len(right):
            if left[i] <= right[j]:
                merged.append(left[i]); i += 1
            else:
                merged.append(right[j]); j += 1
                mc += len(left) - i
        merged.extend(left[i:])
        merged.extend(right[j:])
        return merged, lc + rc + mc

    return sort_count(arr)[1]


if __name__ == "__main__":
    cases = [
        ([3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5], sorted([3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5])),
        ([], []),
        ([1], [1]),
        ([2, 1], [1, 2]),
    ]
    for arr, want in cases:
        got = merge_sort(arr)
        assert got == want, f"got {got}, want {want}"
    assert count_inversions([2, 4, 1, 3, 5]) == 3, "inversions"
    print("PASS")
