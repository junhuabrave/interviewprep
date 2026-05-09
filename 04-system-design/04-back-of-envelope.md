# Back-of-the-Envelope Numbers

The figures and shortcuts you should be able to recall without thinking.

## Powers of 2

| Power | Approx | Common name |
|---|---|---|
| 2^10 | 1,024 | 1 K |
| 2^20 | ~1 M | 1 M |
| 2^30 | ~1 B | 1 G |
| 2^32 | ~4 B | int32 max |
| 2^40 | ~1 T | 1 T |
| 2^53 | 9 × 10^15 | safe JS integer |
| 2^63 | 9 × 10^18 | int64 max |

## Time conversions

- 1 day = **86,400 sec** ≈ 10^5 sec.
- 1 month = ~2.6 × 10^6 sec.
- 1 year = ~3.15 × 10^7 sec.

For DAU → QPS: divide by ~100K. (200M DAU averaging 1 op/day ≈ 2K QPS.)

## Latency ladder (from Jeff Dean)

| Op | Latency | Compared to 1 ns = 1 sec |
|---|---|---|
| L1 cache | 0.5 ns | 0.5 s |
| Branch mispredict | 5 ns | 5 s |
| L2 cache | 7 ns | 7 s |
| Mutex lock/unlock | 25 ns | 25 s |
| Main memory | 100 ns | 1.5 min |
| Compress 1KB | 3 µs | 50 min |
| 1 Gbps network 1 KB | 10 µs | 3 hr |
| SSD random 4 KB read | 150 µs | 1.5 days |
| Same-DC round-trip | 500 µs | 5 days |
| 1 MB seq from RAM | 250 µs | 3 days |
| 1 MB seq from SSD | 1 ms | 11 days |
| HDD seek | 10 ms | 4 months |
| 1 MB seq from HDD | 30 ms | 1 year |
| CA → NL → CA RTT | 150 ms | 5 years |

Rule of thumb: **memory ~100x faster than SSD ~100x faster than disk seek ~10x faster than cross-continent RTT.**

## Throughput ballparks (single commodity node)

| Workload | Ops/sec |
|---|---|
| Memory random reads | 10–50 M |
| SSD random reads | 10–100 K (4 KB) |
| HDD random reads | 100 |
| Single Postgres read (cache hit) | 50–100 K |
| Single Postgres write | 5–10 K |
| Redis ops | 100 K – 1 M |
| Kafka producer | 1–2 M msgs/sec/broker (small msgs) |
| Nginx static serve | 50–100 K req/sec |
| Average web app | 1 K – 10 K req/sec |

## Storage sizes

| Item | Size |
|---|---|
| ASCII char | 1 B |
| UTF-8 char | 1–4 B |
| UUID | 16 B (binary), 36 B (string) |
| Tweet body (280 chars) | ~280 B |
| Tweet w/ metadata | ~1 KB |
| Image (web JPEG) | 100–500 KB |
| Photo (mobile) | 1–5 MB |
| HD video, 1 min, H.264 | ~50 MB |
| 4K video, 1 min | ~300 MB |
| Audio MP3, 1 min | ~1 MB |

## Bandwidth

| Connection | Throughput |
|---|---|
| 4G mobile | 5–50 Mbps |
| Home broadband | 50–500 Mbps |
| Gigabit LAN | 1 Gbps = 125 MB/s |
| 10 Gbps DC link | 1.25 GB/s |
| 100 Gbps backbone | 12.5 GB/s |

## Sample calculations

### Twitter post-rate
200M DAU × 2 tweets/day = 400M/day → **4,600 / sec average**, peak ~15K/sec.

Storage: 400M × 1 KB = **400 GB/day** → **150 TB/year**. Replicate 3x → 450 TB/year of stored data.

### Photo upload service
50M users × 1 photo/day × 2 MB = 100M photos/day, **100 TB/day** = **36 PB/year**.

QPS: 100M / 86,400 ≈ **1,200 uploads/sec** average, peak ~3,500/sec.

### URL shortener
100M new short URLs/year. With base-62 encoding, **6 chars suffice for ~57 billion URLs**, 7 chars for ~3.5 trillion.

Reads:writes typically 100:1 → if writes = 3/sec, reads = 300/sec.

### Chat system
1M concurrent users, average 10 messages/min each → **170K msgs/sec**. Assuming 100 B per message, **17 MB/sec** ingest.

### News feed
200M DAU × 50 feed loads × 20 tweets per load = 200B tweet impressions/day = ~2.3M/sec. → Aggressive caching required (Redis cluster, fan-out cache per user).

## Useful identities

- **1 KB ≈ 10^3 B** (close enough to 2^10 for back-of-envelope).
- **1 MB ≈ 10^6 B**, **1 GB ≈ 10^9 B**, **1 TB ≈ 10^12 B**.
- **1 day ≈ 10^5 sec**.
- **86,400 ÷ N** seconds → about **N events/day** has some intuition: 1 RPS ≈ 86K/day.

## Heuristics

- **80/20 rule**: 80% of traffic on 20% of keys. Plan for hot keys.
- **Read amplification**: 1 user view = N DB calls? Multiply by users.
- **Storage costs are real but cheap**: $0.02/GB-month for cold S3, $0.10/GB-month for hot SSD.
- **Bandwidth is expensive at scale**: cross-region + egress costs dominate big-data architectures.
- **Replication 3x** is the default for durability. Sometimes 5x for very critical data.

## How to do estimates in interviews

1. State your **assumption** (e.g., "assume 200M DAU, each posting twice daily").
2. Convert to **per-second**.
3. Multiply by **payload size** for storage / bandwidth.
4. **Round generously** — 5x of correct order is fine.
5. Cross-check against the **latency budget** (e.g., "120K QPS at 200ms p99 = ~24K concurrent requests in flight").

If unsure of a number, say "I'll assume X — happy to revisit if it matters."
