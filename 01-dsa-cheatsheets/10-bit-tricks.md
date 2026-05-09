# Bit Manipulation

## Operators

| Op | Meaning |
|----|---------|
| `&` | AND |
| `\|` | OR |
| `^` | XOR |
| `~` | NOT (bitwise complement) |
| `<<` | left shift (multiply by 2^k) |
| `>>` | right shift (divide by 2^k, **arithmetic** in C++/Python on signed; **logical** in JS `>>>`) |

## Essential tricks

### Test bit i
```
(x >> i) & 1
```

### Set / clear / toggle bit i
```
x | (1 << i)        # set
x & ~(1 << i)       # clear
x ^ (1 << i)        # toggle
```

### Lowest set bit
```
x & -x              # isolates the lowest 1-bit (two's complement)
x & (x - 1)         # clears the lowest 1-bit
```

### Count set bits (population count)
```python
bin(x).count('1')           # Python
__builtin_popcount(x)       # C++ GCC; std::popcount in C++20
x.toString(2).split('1').length - 1   # TS hack; or while loop
```

### Check power of 2
```
x > 0 and (x & (x - 1)) == 0
```

### Toggle case of a letter (a/A differ in bit 5)
```
ch ^ 0x20
```

## XOR identities

- `x ^ x = 0`
- `x ^ 0 = x`
- XOR is commutative and associative.

So XOR of all elements where every value appears twice except one → returns the unique one. (LeetCode "Single Number".)

## Subset enumeration (bitmask)

For a set of size n, iterate all 2^n subsets:
```python
for mask in range(1 << n):
    for i in range(n):
        if mask & (1 << i):
            # element i is in this subset
            ...
```

Iterate **subsets of a mask**:
```python
sub = mask
while sub > 0:
    # use sub
    sub = (sub - 1) & mask
# also include sub == 0 if needed
```

## Common gotchas

- **Operator precedence**: `&` and `|` are weaker than `==`. Always parenthesize: `(x & 1) == 0`.
- **Sign extension**: in C++/Java/Python, `>>` on negative ints is arithmetic (sign-extends). Use unsigned types if you need logical shift.
- **Python ints are arbitrary precision** — `~x` is `-(x+1)`, may surprise.
- **JS bitwise ops** convert to 32-bit signed ints — beware for numbers > 2^31.
- **Off-by-one in shifts**: `1 << 31` overflows `int32` in C++/JS — use `1LL << 31` (C++) or `BigInt` (JS).

## Top problems

- Single Number / II / III
- Number of 1 Bits
- Reverse Bits
- Counting Bits (DP: `dp[i] = dp[i>>1] + (i&1)`)
- Sum of Two Integers (without `+`)
- Missing Number (XOR trick)
- Subsets (bitmask enumeration)
- Power of Two / Power of Four
- Bitwise AND of Numbers Range
