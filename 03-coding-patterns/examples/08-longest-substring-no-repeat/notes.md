# Longest Substring Without Repeating Characters

## Problem

Given a string `s`, find the length of the longest substring without repeating characters. (LC 3.)

Example: `"abcabcbb"` → `3` (`"abc"`).

## Approach: sliding window with hashmap

Maintain a window `[l, r]` containing only unique characters. As you extend `r`:
- If `s[r]` was seen at index `≥ l`, move `l` to `seen[s[r]] + 1`.
- Update `seen[s[r]] = r` and track the max window size.

The key trick is jumping `l` directly to past the previous occurrence — no inner loop.

## Complexity

| | Time | Space |
|---|---|---|
| | O(n) | O(min(n, alphabet)) |

## Edge cases

- Empty string → 0.
- All distinct → length n.
- All same character → 1.
- Unicode: `seen` is a hashmap, not a fixed array.

## Variants

- **Longest substring with at most K distinct chars**: classic sliding window with frequency map.
- **Longest substring with exactly K distinct chars**: difference of "at most K" and "at most K-1".
- **Minimum window substring** containing all chars of T (LC 76): expand-then-shrink window.
