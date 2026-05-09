# Hashmaps & Sets

## When to reach for a hashmap

- **Index/lookup table**: "have I seen X?", "what was paired with Y?"
- **Counting frequencies**: char counts, word counts, anagram groups.
- **Caching/memoization**: store recursion results.
- **Grouping**: bucket items by computed key (sorted-string for anagrams, parity, etc.).
- **Two-Sum-like**: "complement = target - x; have I seen complement?"

## Patterns

### Frequency map
```python
from collections import Counter
freq = Counter("aabbc")  # {'a':2, 'b':2, 'c':1}
```

### Group by key
```python
from collections import defaultdict
groups = defaultdict(list)
for s in strs:
    key = "".join(sorted(s))   # anagram key
    groups[key].append(s)
```

### "Seen-set" while iterating
```python
seen = set()
for x in arr:
    if target - x in seen:
        return ...
    seen.add(x)
```

### Prefix-sum + count map (Subarray Sum = K)
```python
count = {0: 1}     # empty prefix
total = ans = 0
for x in arr:
    total += x
    ans += count.get(total - k, 0)
    count[total] = count.get(total, 0) + 1
```

## Hashing tradeoffs

- **Average O(1)** is conditional on a good hash. With adversarial keys, worst-case O(n).
- Choosing keys: tuples are hashable (Python), `pair`/`tuple` need custom hash in C++ unless you use `std::map` (O(log n)).
- Use `frozenset` (Python) or `std::set` as keys when ordering doesn't matter.

## Common gotchas

- **Mutating a key after insert** breaks the map (Python: would error; C++: UB-ish).
- **Default values**: `dict.get(key, 0)`, `defaultdict(int)`, `Counter`. Mixing them up causes KeyError.
- **C++ `std::unordered_map`** is significantly slower than `absl::flat_hash_map` and has worst-case attacks. For interviews use it freely.
- **Iteration order**: Python 3.7+ preserves insertion order; C++ unordered does not; JS `Map` does.
- For "k most frequent" use `Counter.most_common(k)` (Python) — built-in heap.

## Set vs hashmap

Use **set** when you only care "is X present" (no associated value). Use **map** when you need to retrieve something by key.

## Top problems

- Two Sum
- Group Anagrams
- Longest Consecutive Sequence (set + only start runs from values with no `v-1`)
- Subarray Sum Equals K
- Top K Frequent Elements
- Valid Anagram
- Contains Duplicate / II / III
- Happy Number (cycle detection in seen set)
- Insert/Delete/GetRandom O(1) (map + dynamic array)
