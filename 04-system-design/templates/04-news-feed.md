# Design a News Feed (Twitter / Facebook home feed)

## 1. Functional requirements

- User can post a tweet (text, ≤ 280 chars).
- User can follow another user.
- User sees a home feed of tweets from people they follow, newest first.
- Pagination on the feed.

**Out of scope**: ranking by relevance (just chronological), DMs, search, hashtags, trending.

## 2. Non-functional requirements

- **Read-heavy**: ~100 reads per write.
- **p99 feed load < 200 ms**.
- **99.9% availability**.
- **Eventual consistency** acceptable (a tweet appearing on followers' feed seconds later is fine).

## 3. Capacity estimates

- 200M DAU.
- 2 tweets / user / day → 400M tweets/day → ~5K writes/sec average, 15K peak.
- 50 feed loads / user / day × 20 tweets per load = 200B tweet impressions/day → ~2.3M reads/sec on tweet objects (mostly absorbed by cache).
- Feed pulls / sec: 200M × 50 / 86,400 ≈ **115K feed loads/sec**, peak ~350K.
- Storage: 400M × 1 KB = **400 GB/day** of tweet data.

## 4. API design

```
POST /tweets
  body: { text }
  -> { tweet_id, created_at }

GET /users/{user_id}/feed?cursor=...&limit=20
  -> { tweets: [{tweet_id, author, text, created_at}, ...], next_cursor }

POST /users/{user_id}/follow
  body: { followee_id }
  -> 204
```

## 5. Data model

```
users(user_id PK, handle, display_name, ...)
tweets(tweet_id PK, author_id, text, created_at)        -- partition by author_id
follows(follower_id, followee_id, PRIMARY KEY (follower_id, followee_id))
follows_inverse(followee_id, follower_id, PRIMARY KEY (followee_id, follower_id))   -- reverse index for fanout
home_feed(user_id, tweet_id, created_at)                -- precomputed feed entries
```

## 6. High-level design

```
client ----> API gateway ----> Tweet svc ----> Tweet DB (Cassandra)
                                    |
                                    v
                           Fanout-on-write svc (Kafka consumer)
                                    |
                          (for each follower)
                                    v
                              Redis sorted set
                              "feed:{follower_id}"
                                    |
                                    |
                                    v
client <---- API gateway <---- Feed svc (read from Redis)
```

## 7. Deep dives

### Fanout strategy

Two extremes:

- **Fanout on write (push)**: when A posts, write the tweet ID into every follower's feed list. Read = O(1) lookup per page. Storage cost: 400M tweets × avg 200 followers = 80B feed entries/day.

- **Fanout on read (pull)**: when a user opens their feed, fetch latest tweets from each followee and merge. Read = O(F·k log k) for F followees. Storage cheap, write cheap — but slow on read for users with many follows.

### Hybrid (recommended)

- **Push for normal users** (most users have <1000 followers; cheap to fan out).
- **Pull for celebrities** (>10K followers — fanning out a single tweet to 50M timelines is wasteful and creates a thundering herd).
- At read time, merge precomputed feed (push) with on-demand fetch from celebrity authors (pull). Roughly k recent celeb tweets, merged into the timeline.

This is the actual Twitter approach.

### Why Cassandra for tweets?

- 400 GB/day → ~150 TB/year. Sharding required.
- Append-only, no updates.
- Partition key = author_id, clustering key = created_at DESC. Fetching "latest 20 tweets by author X" is one row scan.
- Linear write scale via consistent hashing.

### Why Redis for feeds?

- 200M users × 200-tweet feed × 16 B per entry = 640 GB. Fits in a Redis cluster (a dozen 64GB nodes).
- Sorted set keyed by `feed:{user_id}`, score = timestamp. `ZREVRANGEBYSCORE` returns paginated feed in O(log N + page).
- Eviction: trim to most recent 1000 entries per user (older fetched lazily).

### Pagination

Cursor-based, not offset-based.
- Cursor = `(timestamp, tweet_id)` of the last tweet.
- Stable across new tweets; no "page 2 missing items" problem.
- Encode and sign cursor opaquely (Base64 + HMAC) so clients can't forge.

### Cache strategy

- Tweet objects (`tweet_id → tweet content`) cached in Redis. ~95% hit rate.
- Feed itself in Redis sorted set (above).
- Hot author profiles cached.
- Cache stampede mitigation: `singleflight` per key on miss + jittered TTL.

### Failure modes

- **Fanout queue backed up**: tweet appears delayed in followers' feeds. Acceptable (eventual consistency).
- **Redis shard down**: serve from pull-fanout fallback (degraded latency, not failure).
- **Tweet DB down**: degraded — feeds work (Redis), but no new tweets accepted.

### Hot author / celebrity problem

A celebrity with 50M followers posting → fanning out to all 50M timelines synchronously kills the system. Solutions:
1. **Pull-on-read for celebs** (described above).
2. **Sharded fanout queue**: spread across many Kafka partitions, parallel consumers.
3. **Throttle**: rate-limit celeb posts (cosmetic — not actually used).

Threshold for "celebrity" is operational: ~10K followers seems to be the production cutoff.

### Read amplification math

A feed load fetches 20 tweet IDs (Redis ZREVRANGE) → 20 tweet object lookups (Redis MGET) → maybe 1-2 misses → DB. Total: 1 sorted-set read + 1 multi-get + occasional DB read. p99 < 50 ms on cache hit.

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Hybrid fanout | Best read latency without celeb explosion | Pure push (fails on celebs); pure pull (slow for high-follow users) |
| Cassandra tweets | Append-heavy, scale-out | Postgres (limits at this scale) |
| Redis feeds | Sub-ms reads, sorted-set semantics | Memcached (no sorted sets); precompute in DB (slow) |
| Kafka fanout | Decouples, replays, ordering per partition | Direct fan-out call (couples write latency to follower count) |
| Cursor pagination | Stable under inserts | Offset (cheap but fragile) |
| Eventual consistency | Feeds tolerate it; faster | Strong (overkill, expensive) |

### Extensions you can mention

- **Ranking**: replace chronological with ML-ranked feed (Twitter Home, Facebook News Feed). Adds a feature pipeline + model serving layer.
- **Timeline trimming**: only keep last N tweets per user; older fetched on demand.
- **Caching reactions/likes**: separate counter service.
- **Multi-region**: write to closest region, replicate async; user reads from closest.
