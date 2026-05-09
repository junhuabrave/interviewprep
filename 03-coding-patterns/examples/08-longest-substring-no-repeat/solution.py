"""Longest Substring Without Repeating Characters — Python 3.12."""


def length_of_longest_substring(s: str) -> int:
    seen: dict[str, int] = {}
    l = 0
    best = 0
    for r, ch in enumerate(s):
        if (prev := seen.get(ch, -1)) >= l:
            l = prev + 1
        seen[ch] = r
        best = max(best, r - l + 1)
    return best


if __name__ == "__main__":
    cases = [
        ("abcabcbb", 3),
        ("bbbbb", 1),
        ("pwwkew", 3),
        ("", 0),
        ("a", 1),
        ("dvdf", 3),
        ("abba", 2),
    ]
    for s, want in cases:
        got = length_of_longest_substring(s)
        assert got == want, f"got {got}, want {want} for {s!r}"
    print("PASS")
