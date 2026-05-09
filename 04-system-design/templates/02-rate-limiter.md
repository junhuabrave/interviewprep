# Design a Rate Limiter

## 1. Functional requirements

- Allow up to N requests per identity (user / IP / API key) per time window.
- Reject (or queue) requests beyond the limit, returning `429 Too Many Requests`.
- Configurable rules per route or tenant.
- Return informative headers: `X-RateLimit-Limit`, `X-RateLimit-Remaining`, `X-RateLimit-Reset`.

**Out of scope**: dynamic rule editing UI, complex hierarchical quotas.

## 2. Non-functional requirements

- **Latency overhead**: < 5 ms p99.
- **Throughput**: must handle 10x service throughput.
- **Accuracy**: tolerate small over-shoot (1-2%) for performance, or be strict if needed.
- **Availability**: rate limiter must not be a single point of failure.

## 3. Capacity estimates

Suppose the service handles 100K QPS, each request consults the limiter.
- 100K QPS × 1 read + 1 write to limiter store = **200K ops/sec** on the limiter.
- Counter state per identity: ~50 B. 10M active users → 500 MB; fits comfortably in Redis.

## 4. API design (internal)

The limiter is a **library** or **sidecar** consulted before request handling:

```
allow(key: string, route: string) -> (allowed: bool, remaining: int, reset_in: int)
```

Or as a service:

```
POST /ratelimit/check
  body: { key, route }
  -> { allowed, remaining, reset_in }
```

## 5. Data model (Redis)

For token bucket per `(key, route)`:
```
key: rl:{key}:{route}
value: { tokens: int, last_refill: ts }
```

For sliding window counter:
```
key: rl:{key}:{route}:{minute}
value: integer counter
TTL: 2 × window
```

## 6. High-level design

Two deployment modes:

### A) In-process (library)
Each app instance keeps a local counter — fast, but rate-limit "leaks" across instances. Useful when overshoot is acceptable.

### B) Centralized (Redis-backed)

```
   client ----> API gateway ----> [rate limiter check] ----> backend
                                        |
                                        v
                                  +-----------+
                                  | Redis     |
                                  | (cluster) |
                                  +-----------+
```

Redis-backed because:
- Sub-ms latency (well under our 5 ms budget).
- Atomic operations (`INCR`, `EXPIRE`) prevent race conditions.
- Cluster-shardable on the rate-limit key.

## 7. Deep dives

### Algorithm choice

| Algorithm | Memory per identity | Bursts? | Accuracy |
|---|---|---|---|
| Token bucket | O(1) | Yes (up to bucket size) | Exact |
| Leaky bucket | O(1) | No (smoothed) | Exact |
| Fixed window | O(1) | Yes (boundary spike) | Approximate |
| Sliding window log | O(N) | Yes | Exact |
| Sliding window counter | O(1) | Yes | Approximate (good) |

**Recommend token bucket** — most flexible, allows controlled bursts, O(1).

### Token bucket implementation in Redis (atomic Lua)

```lua
-- KEYS[1] = bucket key, ARGV[1]=now, ARGV[2]=rate, ARGV[3]=capacity
local data = redis.call("HMGET", KEYS[1], "tokens", "ts")
local tokens = tonumber(data[1]) or tonumber(ARGV[3])
local ts     = tonumber(data[2]) or tonumber(ARGV[1])
local now    = tonumber(ARGV[1])
tokens = math.min(tonumber(ARGV[3]),
                  tokens + (now - ts) * tonumber(ARGV[2]))
local allowed = 0
if tokens >= 1 then
    tokens = tokens - 1
    allowed = 1
end
redis.call("HMSET", KEYS[1], "tokens", tokens, "ts", now)
redis.call("EXPIRE", KEYS[1], 60)
return allowed
```

Single Lua script = atomic. ~0.5 ms round trip.

### Distributed counter problems

If you shard Redis by key, each `(user, route)` lives on one shard. Easy.

If you replicate Redis, eventual replication can let a user briefly exceed limit during failover. Acceptable for most APIs; for hard quotas, use a CP store (etcd, Consul) — slower.

### Failure modes

- **Redis down**:
  - **Fail open**: allow traffic to pass while limiter is degraded. Risk: DoS.
  - **Fail closed**: reject all. Risk: false positives, customer impact.
  - Common middle ground: fail open with **local fallback bucket** (in-process) at conservative rate.
- **Latency spikes**: short timeout (5 ms) on the limiter call; on timeout, fail open or use cached recent decision.

### Headers and UX

Always return:
```
HTTP/1.1 429 Too Many Requests
Retry-After: 30
X-RateLimit-Limit: 100
X-RateLimit-Remaining: 0
X-RateLimit-Reset: 1700000000
```

Client SDKs respect `Retry-After` for exponential backoff with jitter.

### Multi-tier limits

Often you need:
- Global per-IP (DDoS protection).
- Per-user (fair share).
- Per-route (e.g., POST /tweet limited tighter than GET /feed).
- Per-tenant burst limit.

Apply each as an independent token bucket and **AND** the results — slowest tier wins.

### Observability

- Metric: `ratelimit.allowed` and `ratelimit.rejected`, by route and identity tier.
- Log all rejects at low sample rate (PII concerns).
- Alert when reject rate > expected baseline (potential abuse or limiter misconfig).

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Token bucket | Bursts allowed, simple | Sliding window for stricter accuracy |
| Redis | Fast + atomic via Lua | etcd (CP, slower); local memcached |
| Fail-open | Availability > strictness | Fail-closed for billing-related limits |
| Centralized service | Strict global limits | Library mode for low-overhead |
