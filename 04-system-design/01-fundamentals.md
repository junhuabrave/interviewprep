# System Design Fundamentals

## CAP theorem

In a distributed system, in the presence of a network **Partition**, you must choose between **Consistency** and **Availability**.

- **CP**: Reject some requests during partitions to keep data consistent (e.g., HBase, MongoDB strict, etcd, ZooKeeper).
- **AP**: Always serve, accept temporary inconsistency (e.g., DynamoDB, Cassandra, Riak).

In practice, "no partition" is unrealistic at scale — every system has to handle them. CAP is best read as **CP vs AP** under partition.

PACELC extends it: when there is **no** partition, choose between **Latency** and **Consistency**.

## Consistency models (strongest → weakest)

| Model | Behavior |
|-------|----------|
| Linearizable (strict) | Every read sees the latest committed write, system-wide. |
| Sequential | All clients see the same total order, but it may lag wall-clock. |
| Causal | Reads respect happens-before. Concurrent writes can disagree. |
| Read-your-writes | A client always sees its own writes immediately. |
| Eventual | All replicas converge if writes stop. No timing guarantee. |

Distributed databases tend to offer **per-operation tunable consistency** (e.g., Cassandra `QUORUM` vs `ONE`).

## Latency vs throughput

- **Latency**: time for one request (p50, p95, p99 — always quote percentiles).
- **Throughput**: requests per second the system can sustain.
- They are *not* inversely related — adding more workers raises throughput at constant latency until something saturates (CPU, DB, network).
- **Amdahl's law**: serial fractions limit parallel speedup.
- **Little's law**: `concurrency = arrival_rate × latency`. If 1000 RPS and avg 100ms latency, you have ~100 in-flight requests.

## Availability targets (rough year-long downtime)

| SLA | Yearly downtime |
|-----|----------------|
| 99% ("two nines") | 3.65 days |
| 99.9% | 8.76 hours |
| 99.95% | 4.38 hours |
| 99.99% ("four nines") | 52.6 minutes |
| 99.999% ("five nines") | 5.26 minutes |

- **SLA**: contractual promise (with consequences).
- **SLO**: internal target.
- **SLI**: actual measured signal (latency, error rate).

Each "9" typically costs 5–10× more engineering.

## Reliability building blocks

- **Redundancy** at every layer — no single point of failure.
- **Health checks** + **automated failover** (load balancers remove unhealthy backends).
- **Circuit breakers** prevent retry storms.
- **Backpressure** signals overload upstream.
- **Idempotency keys** so retries are safe.
- **Graceful degradation**: serve cached or stale data if the primary path fails.

## Latency reference numbers

Memorize these — interviewers expect you to know orders of magnitude.

| Operation | Approx latency |
|---|---|
| L1 cache reference | 0.5 ns |
| Branch mispredict | 5 ns |
| L2 cache reference | 7 ns |
| Mutex lock/unlock | 25 ns |
| Main memory reference | 100 ns |
| Compress 1KB with Zippy | 3 µs |
| Send 1KB over 1 Gbps | 10 µs |
| Read 4KB randomly from SSD | 150 µs |
| Read 1MB sequentially from memory | 250 µs |
| Round trip within same datacenter | 500 µs |
| Read 1MB sequentially from SSD | 1 ms |
| Disk seek (HDD) | 10 ms |
| Read 1MB sequentially from disk (HDD) | 30 ms |
| Round trip CA → Netherlands → CA | 150 ms |

(From Jeff Dean's "Numbers Every Programmer Should Know".)

## Consistency vs availability — when to pick which

| Use case | Choose |
|---|---|
| Bank balance, inventory | Consistency (CP) |
| Social feed, comments | Availability (AP) |
| Distributed lock, leader election | Consistency (CP) |
| User session in cache | Availability (AP) |
| Analytics counters | Eventual — often "best effort" is fine |

## Key principles to repeat in interviews

1. **Identify the bottleneck first** — DB, network, CPU? Don't optimize blindly.
2. **Read-heavy vs write-heavy** changes everything. Caches help reads; sharding helps writes.
3. **Cache invalidation is hard** — be ready to discuss TTL, write-through, write-behind.
4. **Hot keys** kill simple sharding. Plan for skew (consistent hashing + virtual nodes).
5. **There is no perfect system** — make tradeoffs explicit, justify them with numbers.
