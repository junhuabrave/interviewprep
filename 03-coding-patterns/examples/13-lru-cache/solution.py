"""LRU Cache — Python 3.12. Hand-built DLL + dict (and an OrderedDict alt at the bottom)."""


class _Node:
    __slots__ = ("key", "value", "prev", "next")

    def __init__(self, key: int, value: int) -> None:
        self.key = key
        self.value = value
        self.prev: "_Node | None" = None
        self.next: "_Node | None" = None


class LRUCache:
    def __init__(self, capacity: int) -> None:
        self.cap = capacity
        self.map: dict[int, _Node] = {}
        # sentinels: head <-> ... <-> tail. Most recent next to head.
        self.head = _Node(0, 0)
        self.tail = _Node(0, 0)
        self.head.next = self.tail
        self.tail.prev = self.head

    def _unlink(self, node: _Node) -> None:
        node.prev.next = node.next      # type: ignore[union-attr]
        node.next.prev = node.prev      # type: ignore[union-attr]

    def _push_front(self, node: _Node) -> None:
        node.prev = self.head
        node.next = self.head.next
        self.head.next.prev = node      # type: ignore[union-attr]
        self.head.next = node

    def get(self, key: int) -> int:
        if (node := self.map.get(key)) is None:
            return -1
        self._unlink(node)
        self._push_front(node)
        return node.value

    def put(self, key: int, value: int) -> None:
        if (node := self.map.get(key)) is not None:
            node.value = value
            self._unlink(node)
            self._push_front(node)
            return
        node = _Node(key, value)
        self._push_front(node)
        self.map[key] = node
        if len(self.map) > self.cap:
            lru = self.tail.prev          # type: ignore[union-attr]
            self._unlink(lru)             # type: ignore[arg-type]
            del self.map[lru.key]         # type: ignore[union-attr]


# Alternative one-liner using OrderedDict:
from collections import OrderedDict


class LRUCacheOD:
    def __init__(self, capacity: int) -> None:
        self.cap = capacity
        self.od: OrderedDict[int, int] = OrderedDict()

    def get(self, key: int) -> int:
        if key not in self.od:
            return -1
        self.od.move_to_end(key)
        return self.od[key]

    def put(self, key: int, value: int) -> None:
        if key in self.od:
            self.od.move_to_end(key)
        self.od[key] = value
        if len(self.od) > self.cap:
            self.od.popitem(last=False)


if __name__ == "__main__":
    for Cache in (LRUCache, LRUCacheOD):
        c = Cache(2)
        c.put(1, 1); c.put(2, 2)
        assert c.get(1) == 1
        c.put(3, 3)                         # evicts 2
        assert c.get(2) == -1
        c.put(4, 4)                         # evicts 1
        assert c.get(1) == -1
        assert c.get(3) == 3
        assert c.get(4) == 4

        c = Cache(1)
        c.put(1, 1); c.put(2, 2)
        assert c.get(1) == -1 and c.get(2) == 2
    print("PASS")
