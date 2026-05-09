# Stacks & Queues

## Stack patterns

### Matching brackets
```python
pairs = {')': '(', ']': '[', '}': '{'}
stack = []
for ch in s:
    if ch in '([{':
        stack.append(ch)
    elif not stack or stack.pop() != pairs[ch]:
        return False
return not stack
```

### Monotonic stack
Use when: "next greater/smaller element", "largest rectangle in histogram", "daily temperatures".

Idea: maintain a stack where elements are in increasing (or decreasing) order; when a new element violates the order, pop and process.

```python
# Next greater element
res = [-1] * n
stack = []   # holds indices, decreasing by value
for i, x in enumerate(arr):
    while stack and arr[stack[-1]] < x:
        res[stack.pop()] = x
    stack.append(i)
```

### Stack-based recursion → iterative
Any DFS can be converted to iterative using an explicit stack. Useful when recursion depth would overflow.

### Min stack
Keep a parallel stack of running mins; or store (value, current_min) pairs.

## Queue patterns

### BFS
The queue is the canonical structure for BFS — see [`07-graphs.md`](07-graphs.md).

### Sliding window maximum (monotonic deque)
```python
from collections import deque
dq = deque()   # holds indices, decreasing by value
out = []
for i, x in enumerate(arr):
    while dq and dq[0] <= i - k:
        dq.popleft()
    while dq and arr[dq[-1]] < x:
        dq.pop()
    dq.append(i)
    if i >= k - 1:
        out.append(arr[dq[0]])
```

### Two-stack queue
Push to stack A. Pop: if stack B empty, drain A→B. Amortized O(1).

## Implementation notes

| Language | Stack | Queue |
|---|---|---|
| Python | `list` (`append`/`pop`) | `collections.deque` (`append`/`popleft`) |
| C++ | `std::stack` or `std::vector` | `std::queue` or `std::deque` |
| TypeScript | `Array.push/pop` | `Array.push/shift` (shift is O(n)!) — use a custom deque or two-array trick |

## Common gotchas

- JS `Array.shift` is O(n) — don't use for big queues.
- C++ `std::queue` doesn't support iteration. Use `std::deque` if you need to peek inside.
- Forgetting to clear the stack at end (e.g., bracket matching).
- Recursive DFS depth: ~1000 in default Python, often fine in C++ if you bump stack size.

## Top problems

- Valid Parentheses
- Min Stack
- Daily Temperatures
- Next Greater Element I/II
- Largest Rectangle in Histogram
- Trapping Rain Water (stack variant)
- Evaluate Reverse Polish Notation
- Asteroid Collision
- Decode String
- Sliding Window Maximum
