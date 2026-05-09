# Design a Distributed Key-Value Store (DynamoDB-style)

## 1. Functional requirements

- `put(key, value)` — store/overwrite.
- `get(key)` — return value or "not found".
- `delete(key)`.
- Values up to 1 MB; keys up to 1 KB.

**Out of scope**: secondary indexes, transactions across keys, range queries.

## 2. Non-functional requirements

- **Availability**: 99.99% (always writable, even during partitions).
- **Durability**: tolerate single-node and single-AZ failure.
- **Latency**: p99 < 10 ms.
- **Scale**: 10s of TB to PBs, 100K+ QPS.
- **Consistency**: tunable. Default eventual; quorum reads/writes optional.

This is firmly an **AP** system in CAP — Dynamo-style.

## 3. Capacity estimates

- 1 TB initial, growing 1 PB.
- 100K QPS, mixed read/write (skewed read-heavy).
- Replication factor 3 → actual storage 3 PB.
- 100K QPS × 3 replicas = 300K I/O ops on the cluster.

## 4. API

```
PUT  /kv/{key}  body: bytes  -> 200 OK
GET  /kv/{key}  -> 200 + body, or 404
DELETE /kv/{key} -> 200 OK
```

Optional consistency hint:
```
GET /kv/{key}?consistency=strong   # quorum read
```

## 5. Data model

Internally each node stores `(key, value, vector_clock, ts)` triples.

- **Vector clocks** per replica to detect concurrent writes.
- **Tombstones** for deletes (kept until anti-entropy converges, then GC'd).

On disk: log-structured storage (LSM-tree) for high write throughput. SSTable + WAL.

## 6. High-level design

```
       client
         |
         v
   +----------+
   | Coordinator (any node) |
   +----------+
         |
         |  hash(key) -> ring -> 3 replica nodes
         v
   +----+----+----+
   | A  | B  | C  |    one row of consistent-hash ring
   +----+----+----+
   on N nodes total

Anti-entropy: periodic Merkle tree comparison + read repair.
```

### Write path
1. Client sends `PUT` to any node (the **coordinator**).
2. Coordinator hashes key, picks N replicas.
3. Forwards write to all N.
4. Returns success when **W replicas** ACK (typically W=2 of N=3).
5. Async replicate to remaining replicas.

### Read path
1. Coordinator queries N replicas.
2. Returns when **R replicas** respond (typically R=2 of N=3).
3. If responses disagree, return latest by vector clock; **read repair** propagates the right value to lagging replicas.

For strong-ish consistency: `R + W > N`. With N=3, W=2, R=2 → consistent reads.

## 7. Deep dives

### Consistent hashing

Map both nodes and keys onto a 0-2^160 ring (SHA-1 of key). Each key belongs to the next N nodes clockwise.

Use **virtual nodes** (each physical node = 100-200 ring positions) to spread load evenly. Adding a node only re-maps its slice.

### Replication strategy

- **N = 3** replicas. W=2, R=2 by default → good consistency, 1 node may be down.
- For latency-sensitive reads: R=1 (eventual). For strict: R=N (slow).
- Cross-AZ: place 3 replicas across 3 AZs for AZ-failure tolerance.

### Sloppy quorum + hinted handoff

If a replica is down, the coordinator writes to the next healthy node and tags the write with the intended owner. When the owner recovers, the temporary holder hands off the data. Keeps writes going during partitions (this is what makes it AP).

### Conflict resolution

Concurrent writes to different replicas → conflicting versions. Options:
- **Last-write-wins**: simplest, but loses concurrent updates.
- **Vector clocks**: detect conflicts; client resolves (or app-level merge function).
- **CRDTs**: data types where concurrent writes commute (counters, sets).

DynamoDB defaults to last-write-wins; Riak surfaces siblings; Cassandra supports CRDT counters.

### Anti-entropy

- **Read repair**: on mismatched read, push correct value to stale replica.
- **Merkle tree comparison**: nodes periodically exchange tree hashes. Mismatched subtrees indicate divergence; sync those keys.
- **Hinted handoff** for short-term outages.

### Storage engine — LSM-tree

Why not B-tree? B-trees do random writes — slow on SSD/HDD.

LSM-tree:
1. Writes go to **memtable** (in-memory sorted structure) + **WAL** (append).
2. Memtable flushes to immutable **SSTable** on disk (sorted run).
3. Background **compaction** merges SSTables, drops tombstones, reduces read amplification.

Reads check memtable + multiple SSTables (use **bloom filter** per SSTable to skip ones that can't contain the key).

Used by Cassandra, RocksDB, LevelDB, BigTable.

### Failure modes

- **Single node down**: replica reads still succeed; writes use sloppy quorum.
- **Whole AZ down**: 3 AZs × 1 replica each — survivable.
- **Network partition**: minority side becomes read-only or rejects; majority continues. Or in pure AP, both sides accept and merge later.
- **Hot key**: a single key getting 100K QPS — N=3 limits write throughput. Mitigation: caching layer in front, or split the key (sharded counters).

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Eventual consistency | Availability + low latency | Strong consistency (CP system: HBase, Spanner) |
| Consistent hashing | Easy scale-out | Range partitioning (good for range scans, bad for hot ranges) |
| LSM-tree | Write-optimized | B-tree (read-optimized; better for read-heavy + small data) |
| Vector clocks | Detect concurrent writes | LWW (simpler, lossy) |
| 3 replicas | Cost vs durability | 5 replicas for very critical data |

### What this design **isn't** good for

- Strong global consistency (use Spanner or CP store).
- Joins, transactions across keys.
- Range scans (Dynamo doesn't support; use Cassandra with clustering keys, or BigTable).
