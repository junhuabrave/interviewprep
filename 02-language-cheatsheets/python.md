# Python 3.12 Cheat Sheet

Idioms, built-ins, and standard-library tools an interviewer expects to see.

## Type hints (3.12 style)

```python
def f(xs: list[int], m: dict[str, int], pair: tuple[int, str]) -> int | None:
    ...

from typing import Optional, Iterable, Callable, override
```

- `list[int]`, `dict[str, int]`, `tuple[int, ...]` — built-in generic syntax (no `from typing import List`).
- `int | None` is the modern union syntax (PEP 604).
- `@override` decorator (3.12) — declares method overrides explicitly.
- `type Alias = ...` (PEP 695): `type Vec = list[float]`.

## `match` statement (PEP 634+)

```python
def parse(token):
    match token:
        case ('+', a, b):           return a + b
        case ('-', a, b):           return a - b
        case [x, *rest]:            return x         # list pattern
        case {'op': op, 'args': args}:               # dict pattern
            return apply(op, args)
        case Point(x=0, y=y):       return y         # class pattern
        case _:                     raise ValueError
```

Bind with `as`: `case [x] as single: ...`. Guard with `if`: `case x if x > 0: ...`.

## Built-in collections you must know

```python
from collections import (
    Counter, defaultdict, deque, OrderedDict, namedtuple, ChainMap
)
```

- **`Counter`**: `Counter("aabb")` → `{'a':2,'b':2}`. `.most_common(k)`.
- **`defaultdict(list)`** / **`defaultdict(int)`** — auto-init missing keys.
- **`deque`** — O(1) appendleft/popleft. `maxlen` param truncates.
- **`heapq`** — min-heap functions: `heappush`, `heappop`, `heapify`, `nsmallest`, `nlargest`.
- **`bisect`** — `bisect_left`, `bisect_right`, `insort` for sorted-list maintenance.
- **`itertools`** — `combinations`, `permutations`, `product`, `accumulate`, `groupby`, `chain`, `pairwise`.
- **`functools`** — `lru_cache`, `cache` (3.9+), `reduce`, `partial`.

## Comprehensions

```python
squares = [x*x for x in range(10) if x % 2 == 0]
words   = {w.lower() for w in lines}
counts  = {k: v for k, v in pairs}
gen     = (x*x for x in range(10))           # generator, lazy
```

Nested: `[c for row in grid for c in row]`.

## Sorting

```python
arr.sort(key=lambda p: (p.priority, -p.ts))
sorted(arr, reverse=True)

from functools import cmp_to_key
arr.sort(key=cmp_to_key(lambda a, b: ...))
```

`sorted` returns a new list; `list.sort` mutates in place.

## Strings

```python
s.split(",")          # ["a","b"]
",".join(parts)
s.replace("a","b")
s.startswith("foo")
s.isalnum() / .isdigit() / .isalpha()
s.lower() / .upper()
s.strip()             # whitespace
ord('a') == 97; chr(97) == 'a'
s[::-1]               # reverse
```

f-strings: `f"{x = }"` (debugging), `f"{value:>10.2f}"` formatting.

## `dataclass` for problem objects

```python
from dataclasses import dataclass, field

@dataclass(frozen=True, slots=True)
class Point:
    x: int
    y: int

@dataclass
class Counter2:
    items: list[int] = field(default_factory=list)
```

`frozen=True` makes instances hashable. `slots=True` saves memory.

## Memoization

```python
from functools import cache

@cache
def fib(n: int) -> int:
    return n if n < 2 else fib(n-1) + fib(n-2)
```

`cache` is unbounded; `lru_cache(maxsize=N)` is bounded.

## Common pitfalls

- **Mutable default arguments**: `def f(x=[])` — the same list across calls. Use `None` and create inside.
- **Late-binding closures**: `[lambda: i for i in range(3)]` all return 2. Capture: `lambda i=i: i`.
- **`is` vs `==`**: `is` is identity. Use `==` for value equality. Exception: `is None`.
- **Slicing copies**: `arr[:]` makes a shallow copy. Mutating slices doesn't affect the original.
- **Recursion limit**: ~1000 default. `sys.setrecursionlimit(10**6)` for deep recursion.

## Reading stdin (competitive style)

```python
import sys
input = sys.stdin.readline       # faster than input()
data = sys.stdin.read().split()
```

## Print formatting

```python
print(f"{x:>4d}", end=" ")        # right-aligned, width 4
print(*arr, sep=",")
```
