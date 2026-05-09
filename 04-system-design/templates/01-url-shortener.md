# Design a URL Shortener (TinyURL / bit.ly)

## 1. Functional requirements

- Given a long URL, return a short URL (e.g., `tiny.url/aB3xZ2`).
- Given a short URL, redirect to the original long URL.
- Custom aliases (optional).
- Optional: link analytics (clicks per day, geo).

**Out of scope**: user accounts, expiry policies, password-protected links (could discuss).

## 2. Non-functional requirements

- **Read-heavy**: ~100 reads per write.
- **Latency**: p99 redirect < 100 ms.
- **Availability**: 99.99% — links must work.
- **Durability**: short URLs must never collide and never be lost.
- **Consistency**: short→long mapping must be strictly correct (no point in eventual consistency for the redirect).

## 3. Capacity estimates

- Assume **100M new URLs/year**, ~10B reads/year.
- Writes: 100M / 3.15×10^7 sec ≈ **3 writes/sec average**, peak ~30/sec.
- Reads: 10B / 3.15×10^7 ≈ **300 reads/sec average**, peak ~3,000/sec.
- Storage per record: ~500 B (long URL up to 2 KB max, plus metadata) → 100M × 500 B = **50 GB/year**, modest.
- Encoding: base-62 (`[a-zA-Z0-9]`). 6 chars → 62^6 ≈ 57B; 7 chars → 3.5T. **7 chars** comfortably handles 30+ years of growth.

## 4. API design

```
POST /shorten
  body: { long_url: string, custom_alias?: string }
  -> { short_url: string }

GET /{short_code}
  -> 301 redirect to long_url

GET /analytics/{short_code}
  -> { clicks_total, clicks_24h, top_countries }
```

301 (permanent) for SEO-friendly redirects, 302 (temporary) if you want to capture analytics on every hit.

## 5. Data model

```
url_map(
  short_code   VARCHAR(7) PRIMARY KEY,
  long_url     TEXT NOT NULL,
  user_id      BIGINT,
  created_at   TIMESTAMP,
  expiry_at    TIMESTAMP NULL
)

clicks(
  short_code   VARCHAR(7),
  ts           TIMESTAMP,
  ip, country, ua, ...
)
```

- `url_map` is small (50 GB) — fits comfortably in a single DB; sharding optional.
- Use **PostgreSQL** for the mapping (strict consistency, 50 GB is trivial).
- Use **Cassandra** or **ClickHouse** for `clicks` (high-write, append-only).

## 6. High-level design

```
                        +----------------+
   client --- DNS ----> | LB / API gw    |
                        +----------------+
                              |  |  |
                +-------------+  |  +--------------+
                v                v                 v
          +-----------+    +------------+    +-----------+
          | Shorten   |    | Redirect   |    | Analytics |
          | service   |    | service    |    | ingest    |
          +-----------+    +------------+    +-----------+
                |                |                  |
                v                v                  v
          +-----------+    +------------+    +-----------+
          | ID gen    |    | Redis cache|    | Kafka     |
          | (Snowflake|    | (hot URLs) |    +-----------+
          +-----------+    +------------+          |
                |                |                  v
                +-------+--------+            +-----------+
                        |                     | ClickHouse|
                        v                     +-----------+
                  +----------+
                  | Postgres |
                  +----------+
```

### Write path
1. Client calls `POST /shorten`.
2. Shorten service generates a 7-char ID (see below).
3. Insert `(short_code, long_url, ...)` into PostgreSQL.
4. Optionally write-through to Redis.
5. Return short URL.

### Read path
1. Client hits `tiny.url/aB3xZ2`.
2. LB routes to redirect service.
3. Look up in Redis (cache); on miss, query PostgreSQL.
4. Emit a click event to Kafka (fire-and-forget).
5. Return 301 to the long URL.

## 7. Deep dives

### Generating short codes

Three options, in order of preference:

**(a) Counter + base-62 encode** — single counter (Snowflake or distributed counter). Encode increasing integer in base-62. Pros: short codes, no collision, sorted. Cons: predictable URLs (info leak, scrape risk).

**(b) Random + collision check** — generate a random 7-char string, attempt insert with unique constraint, retry on collision. With 7 chars and 100M URLs, collision probability stays negligible. Simpler than counter but +1 DB roundtrip per write on collision.

**(c) Hash of long URL** (MD5/SHA-1, take 7 chars base-62). Same long URL → same short code (deduplicated). Need to handle hash collisions → fall back to (b).

**Recommend (a)** with Snowflake-style ID generator. 64-bit ID = `[timestamp][machine_id][seq]`. Gives sortable IDs, no central counter, no collision.

### Cache strategy

- 80% of redirects hit ~20% of URLs (popular links). Redis hit-rate target: **>95%**.
- LRU eviction. TTL ~24 hr to refresh data.
- Cache size: 20% of 100M × ~200 B (short_code + long_url) = ~4 GB → fits in a single Redis instance, but use cluster for HA.
- **Cache stampede** when a hot URL expires → use lazy refresh + jittered TTL.

### Database scaling

50 GB fits on a single Postgres node. If we 100x growth:
- Shard by `short_code` (consistent hash). 7-char base-62 distributes evenly.
- Use a primary + 2 read replicas per shard. Reads from replicas (eventual lag <100ms), writes to primary.

### Analytics pipeline

- Redirect service emits `(short_code, ts, ip, ua)` to Kafka. **Don't block the redirect.**
- Stream consumer aggregates into ClickHouse (per-day rollups).
- Dashboard reads from ClickHouse.

### Failure modes

- **Postgres down** → redirect serves from Redis (graceful degradation). Writes 503.
- **Redis down** → all reads fall through to Postgres. p99 will rise; primary still serves.
- **Kafka down** → emit to local disk buffer, flush when Kafka recovers. Don't lose clicks but don't block.

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Postgres for mapping | Strict consistency, modest size | DynamoDB, Cassandra (overkill) |
| 301 vs 302 | 301 cacheable by browser → fewer hits, cheaper. But loses per-click analytics. | 302 for analytics priority |
| Snowflake IDs | Sortable, decentralized | Auto-increment in DB (single point of contention) |
| Redis cache | Sub-ms reads | Memcached (less feature-rich) |
| Kafka for analytics | Decouples, replay-able | Direct write to ClickHouse (couples pipelines) |
