# Trees & Binary Search Trees

## Traversals

```
        1
       / \
      2   3
     / \
    4   5
```

| Order      | Visit sequence | Use case |
|------------|---------------|----------|
| Preorder   | 1 2 4 5 3     | Copy tree, serialize, expression-tree prefix |
| Inorder    | 4 2 5 1 3     | BST → sorted output |
| Postorder  | 4 5 2 3 1     | Delete tree, expression-tree postfix, bottom-up DP |
| Level-order (BFS) | 1 2 3 4 5 | Width / depth, shortest path |

### Recursive template
```python
def dfs(node):
    if not node: return
    # preorder action
    dfs(node.left)
    # inorder action
    dfs(node.right)
    # postorder action
```

### Iterative inorder (using stack)
```python
stack, cur = [], root
while cur or stack:
    while cur:
        stack.append(cur)
        cur = cur.left
    cur = stack.pop()
    visit(cur)
    cur = cur.right
```

### Level-order BFS
```python
from collections import deque
q = deque([root])
while q:
    for _ in range(len(q)):       # one level at a time
        node = q.popleft()
        if node.left: q.append(node.left)
        if node.right: q.append(node.right)
```

## Common subtree-info pattern

When a problem asks for tree-wide answers (height, diameter, max path, balanced check):

```python
def helper(node):
    if not node: return BASE_CASE
    L = helper(node.left)
    R = helper(node.right)
    # combine L, R, node into the answer for this subtree
    # update a global / nonlocal "best" if needed
    return SOMETHING_FOR_PARENT
```

Examples: diameter (return height, update `best = L+R`), max path sum (return best one-side sum, update best L+R+node).

## Binary Search Tree

- **Inorder traversal of BST = sorted ascending.**
- Insert/Search/Delete: O(h) — h is height. Balanced ⇒ O(log n). Skewed ⇒ O(n).
- Validate BST: pass down `(min, max)` range; each node must be in range.
- Lowest Common Ancestor in BST: walk down — if both targets < node, go left; if both > node, go right; else node is LCA.

## Self-balancing trees (know names, not implementation)

- AVL, Red-Black: O(log n) operations.
- B-tree / B+-tree: used in databases (high fanout, disk-friendly).
- Treap, Splay: less common in interviews.

## Common gotchas

- **Recursion depth**: degenerate tree of n=10^5 will stack overflow in default Python. Increase `sys.setrecursionlimit` or switch to iterative.
- **Forgetting `if not node: return`** → null pointer.
- **Returning vs updating global**: be clear about what helper returns vs what's tracked outside.
- **In BST validation**, comparing only against parent is wrong — must compare against ancestor range.

## Top problems

- Maximum Depth of Binary Tree
- Same Tree / Symmetric Tree
- Invert Binary Tree
- Binary Tree Level Order Traversal (BFS)
- Validate Binary Search Tree
- Lowest Common Ancestor of BT / BST
- Diameter of Binary Tree
- Binary Tree Maximum Path Sum
- Serialize/Deserialize Binary Tree
- Construct Binary Tree from Preorder + Inorder
- Kth Smallest in BST (iterative inorder)
- Path Sum III
