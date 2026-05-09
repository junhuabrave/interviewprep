"""Two Sum — Python 3.12."""


def two_sum(nums: list[int], target: int) -> tuple[int, int]:
    seen: dict[int, int] = {}
    for i, x in enumerate(nums):
        if (j := seen.get(target - x)) is not None:
            return (j, i)
        seen[x] = i
    raise ValueError("no pair found")


if __name__ == "__main__":
    cases = [
        ([2, 7, 11, 15], 9, (0, 1)),
        ([3, 2, 4], 6, (1, 2)),
        ([3, 3], 6, (0, 1)),
        ([-1, -2, -3, -4, -5], -8, (2, 4)),
    ]
    for nums, target, want in cases:
        got = two_sum(nums, target)
        assert got == want, f"got {got}, want {want}"
    print("PASS")
