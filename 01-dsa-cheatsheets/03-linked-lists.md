# Linked Lists

## Why linked lists in interviews

- Tests pointer manipulation discipline.
- Often combined with hashmap (LRU, copy with random pointer).
- Iterative reverse / merge / detect-cycle are required muscle memory.

## Core templates

### Dummy head
Eliminates "is this the first node?" branches.
```python
dummy = ListNode(0, head)
prev = dummy
# ...mutate using prev.next...
return dummy.next
```

### Reverse a linked list (iterative)
```python
prev, cur = None, head
while cur:
    nxt = cur.next
    cur.next = prev
    prev = cur
    cur = nxt
return prev   # new head
```

### Floyd's cycle detection (slow/fast)
```python
slow = fast = head
while fast and fast.next:
    slow = slow.next
    fast = fast.next.next
    if slow is fast:
        return True
return False
```

To find the **start of the cycle**: after meeting, reset one pointer to head, advance both 1 step at a time; they meet at the cycle entrance.

### Merge two sorted lists
```python
dummy = ListNode()
tail = dummy
while a and b:
    if a.val <= b.val:
        tail.next, a = a, a.next
    else:
        tail.next, b = b, b.next
    tail = tail.next
tail.next = a or b
return dummy.next
```

### Find middle (slow/fast)
- Even length: slow ends at start of right half (with `while fast and fast.next`).
- Use this to split lists for merge sort.

## Doubly linked list (for LRU)
- Each node has `prev` and `next`.
- O(1) insert before/after a known node, O(1) remove given the node.
- Typical interview combo: hashmap of `key → node`, DLL of nodes by recency.

## Common gotchas

- **Lost pointer**: Always save `next` before reassigning.
- **Cycle in unexpected places**: After mutating, check that you didn't accidentally point a node back to an ancestor.
- **Returning the wrong head**: After reverse, return `prev`, not `head`.
- **Dummy node leak**: In C++ remember to free or use stack-allocated dummy.

## Top problems

- Reverse Linked List (iter + recursive)
- Reverse Nodes in k-Group
- Linked List Cycle / II
- Merge Two Sorted Lists
- Merge K Sorted Lists (heap or divide & conquer)
- Add Two Numbers
- Copy List with Random Pointer
- Remove Nth Node From End (two pointers)
- Reorder List
- LRU Cache (DLL + hashmap)
