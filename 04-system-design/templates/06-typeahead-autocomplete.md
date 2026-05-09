# Design Typeahead / Autocomplete (Search Suggestions)

## 1. Functional requirements

- As the user types, return the top K (e.g., 10) most likely completions for the current prefix.
- Suggestions are personalized + popularity-ranked.
- Update suggestions in near real time as new queries appear.

**Out of scope**: full-text search, spell correction (would mention).

## 2. Non-functional requirements

- **p99 latency < 100 ms** (must feel instant — keystroke-level).
- **High QPS**: every keystroke is a request → 5-10x search QPS.
- **High availability**: search box must always work; staleness is acceptable.
- **Eventual consistency**: a query trending today appearing in suggestions tomorrow is fine.

## 3. Capacity estimates

- Daily searches: ~10B (Google scale; even modest products see 100M).
- Average 4 keystrokes per query that hit the autocomplete → **40B autocomplete requests/day** ≈ 460K/sec, peak 1.5M/sec.
- Distinct prefixes (up to 20 chars): ~10s of millions in active use.
- Storage for prefix index: top-K per prefix × 10s of millions of prefixes × 100 B = **~10 GB** — fits in RAM on a few nodes.

## 4. API

```
GET /autocomplete?q=harr&limit=10
  -> { suggestions: ["harry potter", "harry styles", "harrison ford", ...] }
```

## 5. Data model

Two pieces:

### Aggregation store (offline)
```
queries(query TEXT, count BIGINT, last_seen TIMESTAMP)
```

### Prefix → top-K serving structure
A **trie** where each node holds the top-K queries for the prefix ending at that node. Or a hashmap `prefix → top-K` (simpler but uses more memory).

## 6. High-level design

```
   client ----> CDN/edge ----> Suggestion svc -----> Prefix index (in-memory)
                                                          ^
                                                          |
                                       +------------------+
                                       |  rebuild offline
                                       |
   query stream ---> Kafka ---> Aggregator (Spark/Flink) -----> queries DB (Postgres / Cassandra)
                                                          \
                                                           --> Trie builder ---> Index snapshot
```

### Read path
1. Client sends prefix to suggestion service (often via CDN with short TTL — 30s — for cacheable popular prefixes).
2. Suggestion service walks trie node for the prefix → returns its precomputed top-K.
3. Latency: in-memory trie lookup is ~µs; total p99 dominated by network.

### Write path (offline)
1. All search queries flow through Kafka.
2. Stream aggregator (Flink) maintains `query → count` over rolling time windows (last 7 days, decayed weighting).
3. Periodically (hourly), batch job rebuilds the trie:
   - For each query, walk all prefixes (up to length K).
   - At each prefix node, merge query into top-K heap.
4. New trie deployed atomically (blue/green) to suggestion servers.

## 7. Deep dives

### Trie design

Each node:
- `children: map<char, node>`
- `top_k: List<(query, score)>` — precomputed.

For prefix "harr", lookup is 4 hops down the trie. At node "harr", read its `top_k`.

Storage: ~50M nodes × ~200 B per node = 10 GB. Fits in RAM.

### Hot prefix cache

The top ~10K prefixes (e.g., "y", "ya", "you", "yout", "youtu", "youtub", ...) absorb 80% of traffic. Cache responses at the **CDN edge** with 30s TTL. This collapses ~80% of traffic before hitting our service.

### Sharding

Shard the trie by prefix's **first character** (or first 2 chars for finer granularity). 26-256 shards.

- Each shard fits in RAM on one server.
- Replicas for fault-tolerance and read scale.
- LB routes request by prefix's first char.

### Updates: batch vs incremental

- **Batch (recommended)**: rebuild the full trie offline every hour. Simpler, atomic deployment, cleaner.
- **Incremental**: update top-K in place as queries stream in. Faster to react, but harder to reason about (top-K maintenance, racy updates).

For most use cases, hourly batch is fine — users don't notice a 1-hour lag in suggestion freshness.

### Personalization

Add a per-user signal:
- User's recent queries → small per-user trie/list.
- Merge user's history with the global top-K at request time:
  ```
  final = merge(user_recent[prefix], global_top_k[prefix], weights)
  ```
- Per-user data fits in cache or KV store; ~few KB per user.

### Ranking signal

Top-K isn't pure count. Better signals:
- **Time-decayed popularity**: recent queries weighted higher (exponential decay over days).
- **Click-through rate**: did the suggestion get clicked?
- **Personalized**: user's history.
- **Localization**: country / language.
- **Trending detection**: spike in count over recent window → boost.

### Spell correction

Not in MVP, but you can mention:
- Edit-distance lookup at trie nodes.
- BK-trees for fuzzy queries.
- Or precompute "did you mean" mappings offline.

### Failure modes

- **Trie shard down**: serve cached responses from CDN; degrade to global top-K (least personalized).
- **Aggregator lagging**: serve last good snapshot. Suggestions become stale; users don't notice for hours.
- **Edge cache down**: traffic hits service directly; capacity should be sized for 5x to absorb.

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Trie with precomputed top-K | O(prefix) lookup, simple | Elasticsearch (heavier, slower); Aho-Corasick (substring matching) |
| In-memory | Sub-ms lookups | On-disk B-tree (slower) |
| Hourly batch rebuild | Simple, safe | Real-time updates (more moving parts) |
| Edge caching | 80% traffic absorbed | Origin-only (5x more capacity) |
| Shard by first char | Even-ish distribution | Hash sharding (loses prefix locality) |

### Scaling beyond a single trie

For very large prefix corpora (Wikipedia titles, product catalog):
- Compress the trie (radix tree / patricia trie — collapse single-child chains).
- Use a **finite state automaton** (FST) like Lucene's — RAM-efficient, sub-µs lookups.
