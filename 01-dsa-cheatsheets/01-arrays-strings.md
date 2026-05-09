# Arrays & Strings

## Key techniques

### Two pointers
Use when: sorted array, palindrome check, removing duplicates, partitioning, "find pair with sum X".
- Opposite ends: `l=0, r=n-1`, move based on condition.
- Same direction: `slow, fast` — slow tracks write position, fast scans.

```
l, r = 0, n-1
while l < r:
    if condition: l += 1
    else: r -= 1
```

### Sliding window
Use when: contiguous subarray/substring with property (max sum, longest no-repeat, smallest covering, etc.).
- **Fixed size**: slide window of size k, update on enter/leave.
- **Variable size**: expand right, shrink left while condition violated.

```
l = 0
for r in range(n):
    add(arr[r])
    while window_invalid:
        remove(arr[l])
        l += 1
    update_answer(r - l + 1)
```

### Prefix sums
Use when: many range-sum queries, "subarray sum equals K", differences along runs.
- `pref[i+1] = pref[i] + arr[i]`. `sum(l..r) = pref[r+1] - pref[l]`.
- For "subarray sum = K": store `pref[i] → count`, lookup `pref[j] - K`.

### Difference array
Use when: many range-update queries. `diff[l] += v; diff[r+1] -= v`. Final array = prefix sum of diff.

## String specifics

- **In-place reverse**: two pointers swap.
- **Palindrome check**: two pointers from ends, skip non-alnum if needed.
- **Anagram check**: sort both, or count chars (size-26 array for lowercase).
- **Substring search**: KMP O(n+m), or `str.find()` typically Boyer-Moore.
- **Rolling hash (Rabin-Karp)**: O(n) substring matching with collision risk.

## Common gotchas

- Off-by-one in slice ends: `arr[l:r+1]` includes index `r`.
- String concatenation in a loop is O(n²) in Python/Java/Go (immutable strings) — use list-join or StringBuilder.
- Negative indexing: `arr[-1]` is last in Python; **not** valid in C++.
- Empty array: many algorithms break — special-case it.
- `int` overflow in C++ for `(l+r)/2` if `l+r > INT_MAX`. Use `l + (r-l)/2`.

## Top problems to know

- Two Sum (hashmap)
- 3Sum / 4Sum (sort + two pointers)
- Container With Most Water (two pointers)
- Trapping Rain Water (two pointers or stack)
- Longest Substring Without Repeating Characters (sliding window)
- Minimum Window Substring (sliding window)
- Subarray Sum Equals K (prefix sum + hashmap)
- Product of Array Except Self (prefix/suffix product)
- Rotate Array (reverse trick)
- Move Zeroes (two pointers)
