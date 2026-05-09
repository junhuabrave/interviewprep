# TypeScript Cheat Sheet

Modern TypeScript idioms expected in mid-level interviews. Run with `npx tsx file.ts` or compile with `tsc --strict`.

## tsconfig essentials

```json
{
  "compilerOptions": {
    "target": "ES2022",
    "module": "ESNext",
    "strict": true,
    "noUncheckedIndexedAccess": true,
    "exactOptionalPropertyTypes": true,
    "moduleResolution": "Bundler"
  }
}
```

`strict: true` is non-negotiable for interview code — it enables `noImplicitAny`, `strictNullChecks`, etc.

## Core types

```ts
let n: number = 1;
let s: string = "";
let b: boolean = true;
let xs: number[] = [];
let tup: [string, number] = ["a", 1];
let m = new Map<string, number>();
let st = new Set<number>();

type Point = { x: number; y: number };
type Maybe<T> = T | null | undefined;

interface Counter {
    inc(): void;
    value: number;
}
```

`type` vs `interface`: largely interchangeable; `interface` allows declaration merging, `type` allows unions/conditionals/mapped types. Default to `type` unless you specifically want merging.

## Narrowing

```ts
function area(s: { kind: "circle"; r: number } | { kind: "square"; w: number }) {
    switch (s.kind) {
        case "circle": return Math.PI * s.r ** 2;
        case "square": return s.w ** 2;
    }
}
```

Other narrowing tools:
- `typeof x === "string"`
- `Array.isArray(x)`
- `x instanceof MyClass`
- `"prop" in obj`
- Custom guards: `function isPoint(x: unknown): x is Point { ... }`

## Generics

```ts
function first<T>(arr: T[]): T | undefined {
    return arr[0];
}

function pluck<T, K extends keyof T>(obj: T, key: K): T[K] {
    return obj[key];
}

class MinHeap<T> {
    private heap: T[] = [];
    constructor(private cmp: (a: T, b: T) => number) {}
    push(x: T): void { /* ... */ }
    pop(): T | undefined { /* ... */ }
}
```

`const` type parameters (TS 5.0+):
```ts
function asTuple<const T extends readonly unknown[]>(arr: T): T { return arr; }
const t = asTuple([1, "a"]);   // type is readonly [1, "a"]
```

## `satisfies` operator (TS 4.9+)

Validates a value matches a type without widening it:
```ts
const palette = {
    red:   [255, 0, 0],
    green: "#0f0",
} satisfies Record<string, [number, number, number] | string>;

palette.red.toFixed;     // OK — palette.red kept its tuple type
```

## Utility types

| Utility           | Effect |
|-------------------|--------|
| `Partial<T>`      | All props optional |
| `Required<T>`     | All props required |
| `Readonly<T>`     | All props readonly |
| `Pick<T, K>`      | Keep only keys in K |
| `Omit<T, K>`      | Drop keys in K |
| `Record<K, V>`    | `{ [k in K]: V }` |
| `ReturnType<F>`   | Return type of function F |
| `Parameters<F>`   | Tuple of F's params |
| `Awaited<P>`      | Unwrap Promise<T> to T |
| `NonNullable<T>`  | Remove `null \| undefined` |

## Collections

```ts
const m = new Map<string, number>();
m.set("a", 1); m.get("a"); m.has("a"); m.delete("a"); m.size;
for (const [k, v] of m) { /* ... */ }     // insertion order

const s = new Set<number>();
s.add(1); s.has(1); s.delete(1);
```

`Map<K,V>` and `Set<V>` preserve insertion order — useful for "LRU-ish" tricks.

JS arrays are dynamic and have all the obvious methods: `push/pop/shift/unshift/slice/splice/concat/sort/reverse/map/filter/reduce/some/every/find/findIndex/includes/indexOf/flat/flatMap`.

**`shift` is O(n)** — don't use as a queue. Implement a custom deque or use `[head++, tail++]` indices.

## Async / Promises

```ts
async function fetchAll(urls: string[]): Promise<string[]> {
    const results = await Promise.all(urls.map(u => fetch(u).then(r => r.text())));
    return results;
}

// Settle all even on errors
const settled = await Promise.allSettled(promises);
```

## Classes

```ts
class Node<T> {
    constructor(
        public val: T,
        public next: Node<T> | null = null,
    ) {}
}

abstract class Shape {
    abstract area(): number;
}
```

Parameter properties (`public`/`private`/`readonly` in constructor) are concise.

## Sorting

```ts
arr.sort((a, b) => a - b);        // numeric ascending
arr.sort((a, b) => b - a);        // numeric descending
arr.sort((a, b) => a.localeCompare(b));   // strings
```

**Default `arr.sort()` is lexicographic** — `[10, 2].sort()` → `[10, 2]`. Always pass a comparator for numbers.

## Common gotchas

- `arr.sort()` lexicographic by default (see above).
- `Array(5).fill([])` shares the same array reference — use `Array.from({length: 5}, () => [])`.
- `for (const k in obj)` iterates **inherited keys too**; use `for (const k of Object.keys(obj))`.
- `typeof null === "object"` (legacy bug).
- `0 == false`, `"" == false` — use `===` always.
- `JSON.stringify` strips `undefined`, functions, and `BigInt`.
- Bitwise ops coerce to **32-bit signed int** — `1 << 31` is negative.
- Floating-point: `0.1 + 0.2 !== 0.3`.
- `noUncheckedIndexedAccess`: `arr[i]` is typed `T | undefined` — narrow before use.

## Reading stdin (Node)

```ts
import { readFileSync } from "node:fs";
const lines = readFileSync(0, "utf8").trim().split("\n");
```

## Quick reference: language constructs

```ts
// Destructuring
const { x, y, ...rest } = point;
const [a, b, ...tail] = arr;

// Default parameters
function greet(name = "world") { ... }

// Spread / rest
const merged = [...a, ...b];
const obj = { ...base, override: true };

// Optional chaining + nullish coalescing
const v = data?.user?.email ?? "anonymous";

// Template literals
const msg = `Hello, ${name}!`;
```
