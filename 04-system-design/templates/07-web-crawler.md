# Design a Web Crawler

## 1. Functional requirements

- Given a set of seed URLs, fetch the pages, extract links, fetch those, recursively.
- Index page contents (or hand off to indexer).
- Respect `robots.txt` and politeness (don't hammer servers).
- Detect duplicates (same URL, same content, near-duplicates).
- Refresh pages periodically (some sites change daily, others rarely).

**Out of scope**: ranking (PageRank), search query handling, JavaScript rendering at scale (would mention).

## 2. Non-functional requirements

- **Scale**: billions of pages; fetch tens of billions/year.
- **Politeness**: < 1 req/sec per domain (configurable).
- **Robustness**: tolerate slow / dead / malicious sites.
- **Freshness**: re-crawl popular pages within hours, long-tail within weeks.

## 3. Capacity estimates

- **10B pages crawled per month** = ~3,800/sec average. Plan for 10K/sec peak.
- Avg page = 500 KB compressed → 5 PB/month raw. With 3x replication, 15 PB.
- URL frontier: 10B URLs in queue × 100 B/URL → 1 TB just for the queue.
- DNS lookups: cache aggressively; otherwise ~10K DNS/sec hits a real bottleneck.

## 4. API (internal)

The crawler is a system, not a public API. Operator interfaces:
```
POST /seeds  body: [url1, url2, ...]
GET /status  -> { queue_size, crawl_rate, errors }
```

## 5. Data model

- **URL frontier**: priority queue of URLs to fetch.
- **Seen set**: bloom filter + persistent KV store of "URL already fetched".
- **Page store**: blob store keyed by URL hash, value = compressed HTML + headers.
- **Content fingerprints**: SimHash of each page for near-duplicate detection.
- **Domain stats**: per-host last-fetch-time, robots.txt cache, fetch interval.

## 6. High-level design

```
   +-----------+
   |  Seeds    |
   +-----------+
        |
        v
   +-----------+        +--------------+
   | URL       | <----> | Priority Q   |
   | scheduler |        | (Kafka/Redis)|
   +-----------+        +--------------+
        |
        v
   +---------------+    +-------------+
   | Politeness    | -> | DNS resolver|
   | & dedup gate  |    | (cached)    |
   +---------------+    +-------------+
        |
        v
   +-----------+
   | Fetcher   |  ← thousands of workers
   | (HTTP)    |
   +-----------+
        |
        v
   +-----------+    +--------------+    +-------------+
   | Parser /  | -> | Link extract | -> | URL filter  | --> back to scheduler
   | renderer  |    +--------------+    +-------------+
   +-----------+
        |
        v
   +-----------+    +--------------+
   | Content   | -> | Indexer      |
   | dedup     |    +--------------+
   +-----------+
        |
        v
   +-----------+
   | Page blob |
   | store (S3)|
   +-----------+
```

## 7. Deep dives

### URL frontier

A naive FIFO queue is wrong — you'd hammer one domain. Need a **two-level queue**:

1. **Front queues**: prioritize by importance (PageRank, recency, news-y signals).
2. **Back queues**: one per domain. The fetcher pulls from a back queue only after `last_fetch + interval` has passed.

A **heap of `(next_eligible_time, domain)`** controls which back queue is ready.

### Politeness

- Read `robots.txt`, cache for 24 h. Respect `Crawl-Delay` directive.
- Per-domain rate limit (default 1 req/sec; can override based on host capacity).
- Spread fetches across hostnames; never N parallel connections to same host.

### Dedup: URL-level vs content-level

- **URL dedup**: bloom filter of URL hashes, with persistent backing in a sharded KV store. Bloom filters reject most "already seen" URLs cheaply.
- **Content dedup**: SimHash or MinHash of each page. Pages with hamming distance ≤ 3 in 64-bit SimHash are considered near-duplicates and dropped from the index (still stored once).

### DNS

- Cache aggressively (TTL respected).
- Local DNS resolver per crawler region, with a shared in-memory cache.
- Avoid DNS as a bottleneck; it can become one for cross-domain crawls.

### Fetcher pool

- Async I/O (1 thread per worker can hold thousands of in-flight TCP connections).
- HTTP connection pool per host.
- Aggressive timeouts: connect 3s, read 10s, total 30s.
- HEAD-then-GET for content-type filtering.
- Bandwidth caps and rate limits to be a good citizen.

### Parser / renderer

- For static HTML, a streaming parser (Gumbo, libxml2, Nokogiri) is enough.
- For JS-heavy sites (SPAs), need a headless browser (Puppeteer/Playwright). 100-1000x more expensive.
- **Don't render every page** — cost-prohibitive. Render only when the static fetch yields little useful content (heuristic: ratio of `<script>` to text).

### Storage layout

- **Page blobs**: S3 / object store, keyed by hash of URL. Compressed (Zstandard).
- **Index data**: separate pipeline (out of scope). Often Bigtable/HBase for inverted index.
- **Crawl metadata**: HBase or Cassandra — `URL → (last_fetched, http_status, content_hash, etag)`.

### Refresh strategy

- Re-crawl popular pages (high in-link count, frequent changes) hourly.
- Long-tail pages monthly.
- ETag / Last-Modified to skip unchanged pages cheaply.

### Distributed coordination

- Sharded URL queue: hash by domain → assigned to a specific shard. Politeness is local to a shard.
- Coordinator (ZooKeeper / etcd) tracks worker liveness, assigns shards.
- Workers pull from local shard's back queues.

### Trap detection

- **Spider traps**: infinite URL spaces (calendars, search results). Cap crawl depth and per-domain page count.
- **Soft 404 / cloaking**: detect with content-similarity to known soft-404 patterns.

### Failure modes

- **Bad host**: timeout, mark domain unhealthy, exponentially back off.
- **Worker crash**: unfinished URLs stay in queue (visibility timeout); another worker picks up.
- **Disk full**: pause crawl, alert.

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Two-level frontier | Politeness + priority | Single FIFO (hammers single domains) |
| Bloom filter dedup | Tiny memory footprint | Set in DB (slow) |
| SimHash for content dedup | O(1) near-dup check | Full content compare (n²) |
| Async I/O fetchers | High concurrency per box | Thread-per-conn (memory hog) |
| Headless render only on demand | Cost control | Render everything (10-100x cost) |
| Sharded by domain | Local politeness | Sharded by URL hash (politeness becomes a coordination problem) |

### Scaling and cost levers

- Bandwidth dominates the bill. Crawl from regions cheap to egress.
- Storage is mostly cold; tier to Glacier after 90 days.
- Compress blobs with Zstandard (better than gzip).
