# System Design Building Blocks

The vocabulary you'll combine into every design.

## Load balancer (LB)

Sits in front of a backend pool. Distributes requests, hides individual server failures.

- **L4 (TCP)** — fast, no awareness of HTTP. Examples: AWS NLB, HAProxy in TCP mode.
- **L7 (HTTP)** — can route by URL/header, terminate TLS, do health checks. Examples: nginx, Envoy, AWS ALB.

Algorithms:
- Round-robin / weighted round-robin.
- Least-connections / least-response-time.
- Consistent hashing (for cache stickiness).
- IP-hash (cheap session affinity).

Always have at least 2 LBs (active/passive or active/active) — the LB itself can fail.

## Cache

Stores frequently-read data closer to the consumer. Trade memory for latency.

- **Client-side cache** (browser, mobile) — fastest, but stale data risk.
- **CDN** — geographic edge cache for static assets.
- **In-process cache** (LRU within the app) — no network hop, but per-node only.
- **Distributed cache** (Redis, Memcached) — shared, near-millisecond reads, ~10K-1M ops/sec/node.

Patterns:
- **Cache-aside (lazy)**: app reads cache; on miss, reads DB and populates cache.
- **Read-through**: cache fronts the DB; cache itself fetches on miss.
- **Write-through**: write to cache and DB synchronously. Strongly consistent, slower writes.
- **Write-behind (write-back)**: write to cache, flush to DB async. Fast writes; data loss risk.

Eviction: **LRU**, **LFU**, **TTL**, FIFO. Watch for **cache stampede** — use jittered TTL or `singleflight` patterns.

## Database

### Relational (SQL)
- ACID transactions, joins, schema enforcement.
- Vertical scale + read replicas + sharding.
- Examples: PostgreSQL, MySQL.

### NoSQL document
- JSON-like, flexible schema, scale horizontally.
- Examples: MongoDB, DynamoDB, CouchDB.

### Wide-column
- Optimized for range scans, time-series, write-heavy.
- Examples: Cassandra, HBase, ScyllaDB, BigTable.

### Key-value
- Simple `get`/`put`. Used as caches and metadata stores.
- Examples: Redis, DynamoDB, etcd.

### Graph
- Relationship-heavy queries.
- Examples: Neo4j, Amazon Neptune.

### Time-series
- High-write, time-stamped.
- Examples: InfluxDB, TimescaleDB, Prometheus.

### Search
- Full-text, faceted search.
- Examples: Elasticsearch, OpenSearch.

## Replication

- **Leader-follower (primary-replica)**: writes go to leader, reads can fan out to followers. Simple, asynchronous follower lag.
- **Multi-leader**: writes to any leader, replicate among them. Conflict resolution required (last-write-wins, CRDTs, app-level merge).
- **Leaderless**: any node accepts writes; reads do quorum (e.g., Dynamo-style `R + W > N`).

## Sharding (horizontal partitioning)

Split data across shards by some key.
- **Range sharding** — `[A-G] [H-N] [O-Z]`. Hot ranges possible.
- **Hash sharding** — `hash(key) % N`. Even, but resharding is painful.
- **Consistent hashing** — minimal data movement on add/remove. Use **virtual nodes** to spread load.
- **Directory-based** — lookup table maps keys to shards. Flexible, but the directory becomes a bottleneck.

Cross-shard joins/transactions are expensive — design to keep related data co-located.

## Message queue / event log

Decouples producers from consumers, smooths bursts, enables async processing.

| System | Best for |
|---|---|
| Kafka | High-throughput append-only log, replay, fan-out |
| RabbitMQ | Routing/work queues with rich exchange types |
| AWS SQS / SNS | Managed, simple |
| Redis Streams | Lightweight, in-memory |
| NATS | Low-latency pub/sub |

Delivery semantics:
- **At-most-once**: may drop. Cheapest.
- **At-least-once**: may duplicate. Common; combine with idempotent consumers.
- **Exactly-once**: hardest; usually achieved as "at-least-once + idempotency".

## API gateway

A reverse proxy with routing, auth, rate-limiting, request transformation.

Sits between clients and many backend microservices. Examples: Kong, AWS API Gateway, Envoy, GraphQL gateways.

## CDN (Content Delivery Network)

Globally distributed caches for static assets (and some dynamic content).
- **Pull CDN** — fetches from origin on first request, then caches.
- **Push CDN** — you upload assets directly.

Examples: CloudFront, Cloudflare, Fastly, Akamai.

## Bloom filter

Space-efficient probabilistic set membership.
- "Is this key **definitely not** present?" → True is cheap, false has small false-positive rate.
- Used to skip disk lookups (LSM-trees), avoid duplicate URL crawls, ad targeting.

## Consistent hashing

Map both nodes and keys onto a ring. Each key belongs to the next node clockwise. Adding/removing a node only re-maps its slice.
- Use **virtual nodes** (each physical node = 100-200 ring positions) to avoid skew.
- Used by: DynamoDB, Cassandra, Memcached clients, distributed caches.

## Rate limiter algorithms

| Algorithm | Behavior |
|---|---|
| Token bucket | Allows bursts up to bucket size, refills at fixed rate |
| Leaky bucket | Smooths out — fixed output rate, queues excess |
| Fixed window | Counter resets every N seconds. Boundary spikes possible |
| Sliding window log | Store timestamps, count in last window. Memory-heavy |
| Sliding window counter | Combine current + previous fixed-window with weights — good compromise |

## Common SQL vs NoSQL decision

Use **SQL** when:
- ACID is required (payments, inventory).
- Complex queries / joins.
- Schema is stable.

Use **NoSQL** when:
- Massive horizontal scale is required.
- Schema is flexible / evolving.
- Workload is well-defined and access patterns are known up front.

You can mix — "polyglot persistence" is normal: PostgreSQL for orders, Redis for sessions, Elasticsearch for search, S3 for blobs.

## Storage tiers (rough $$ per GB-month)

| Tier | Latency | Cost |
|---|---|---|
| RAM | 100 ns | $$$$$ |
| SSD | 100 µs | $$$ |
| HDD | 10 ms | $$ |
| Object store (S3) | 50 ms | $ |
| Glacier / archive | minutes-hours | ¢ |

## Service-to-service communication

- **Synchronous**: REST/HTTP, gRPC. Simple, request-response. Failure cascades.
- **Asynchronous**: queues, event streams. Decoupled, resilient. Eventual consistency.
- **GraphQL**: client-specified queries, federated. Flexibility costs server-side complexity.

## Observability triad

- **Logs** — structured, queryable. ELK, Splunk, CloudWatch.
- **Metrics** — numeric, time-series. Prometheus, Datadog.
- **Traces** — request paths through services. Jaeger, Zipkin, Honeycomb.

Plus alerts on SLO violations.
