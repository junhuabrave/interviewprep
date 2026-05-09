# LRU Cache

## Problem

Design a data structure with O(1) `get(key)` and `put(key, value)`. When inserting beyond capacity, evict the **least-recently-used** key. Both `get` and `put` count as accesses. (LC 146.)

## Approach

**Hashmap + doubly linked list (DLL).** This is the canonical answer.

- **DLL** of `(key, value)` nodes ordered by recency (most recent at head, least recent at tail).
- **Hashmap** `key → DLL node` for O(1) lookup.

Operations:
- `get(key)`: look up node in map. If missing, return -1. Else move node to head, return value.
- `put(key, value)`: if key exists, update value and move to head. Else create node at head, insert in map. If size > capacity, remove tail node (and its key from map).

DLL operations (insert at head, unlink given node) are O(1) **only with prev/next pointers**. A singly linked list won't do.

## Complexity

| | Time | Space |
|---|---|---|
| `get`, `put` | O(1) | O(capacity) |

## Language-specific shortcuts

- **Python**: `collections.OrderedDict` — `move_to_end` and `popitem(last=False)` give the LRU semantics directly.
- **TypeScript / JS**: `Map` preserves insertion order; `delete` + `set` re-orders.
- **C++**: `std::list<pair<K,V>>` + `std::unordered_map<K, list_iterator>` and use `splice` to move nodes.

The interview answer: **show you can build the DLL yourself**, then mention the language shortcut as an alternative.

## Edge cases

- Capacity = 0 — every `put` immediately evicts; we'll assume capacity ≥ 1.
- Same key `put` twice — update value, move to head.
- `get` on missing key — return `-1` and **don't** insert.

## Variants

- **LFU Cache** (LC 460): evict least-*frequently*-used. Need frequency buckets, each a DLL.
- **TTL Cache** — items expire after time T. Use a min-heap of expiry times alongside.
