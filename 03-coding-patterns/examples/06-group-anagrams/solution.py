"""Group Anagrams — Python 3.12."""
from collections import defaultdict


def group_anagrams(strs: list[str]) -> list[list[str]]:
    groups: dict[tuple[int, ...], list[str]] = defaultdict(list)
    for s in strs:
        key = [0] * 26
        for ch in s:
            key[ord(ch) - ord('a')] += 1
        groups[tuple(key)].append(s)
    return list(groups.values())


if __name__ == "__main__":
    strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
    got = sorted(sorted(g) for g in group_anagrams(strs))
    want = sorted(sorted(g) for g in [["eat", "tea", "ate"], ["tan", "nat"], ["bat"]])
    assert got == want, f"got {got}, want {want}"

    assert group_anagrams([]) == [], "empty"
    got2 = sorted(sorted(g) for g in group_anagrams([""]))
    assert got2 == [[""]], "empty-string"
    print("PASS")
