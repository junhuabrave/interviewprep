# Group Anagrams

## Problem

Given an array of strings, group the anagrams together. (LC 49.)

Example: `["eat","tea","tan","ate","nat","bat"]` → `[["eat","tea","ate"], ["tan","nat"], ["bat"]]`.

## Approach

For each string, compute a **canonical key** that's identical for any permutation. Two natural choices:

1. **Sorted characters** as the key: `"".join(sorted(s))`. O(K log K) per string where K = string length.
2. **Frequency vector** of length 26 (lowercase) as a tuple/string: `(2, 0, 0, ..., 1, ...)`. O(K) per string.

Group strings by key into a hashmap, then return the values.

## Complexity

Let n = number of strings, K = max length.

| Approach | Time | Space |
|---|---|---|
| Sorted key | O(n · K log K) | O(n · K) |
| Frequency key | O(n · K) | O(n · K) |

## Edge cases

- Empty input → empty list.
- Empty strings — they're all anagrams of each other; one group.
- Mixed case / unicode — clarify character set; assume lowercase ASCII unless told otherwise.

## Why this matters

Demonstrates **hashmap grouping by computed key** — a frequent pattern. Same idea applies to "Group by frequency", "Group by remainder", etc.
