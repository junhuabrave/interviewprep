# System Design Interview Framework

A repeatable 7-step approach for a 45-60 minute system design interview. Stick to the structure even when you feel rushed — interviewers grade on **completeness**, not just the final picture.

## Step 0: Listen and confirm scope (~3 min)

The interviewer gives a vague prompt: "Design Twitter." Your first job is to **narrow it**.

Ask, in roughly this order:
- "What are the most important features?" (post tweet, follow user, home feed.)
- "What scale are we targeting?" (DAU, QPS, payload size.)
- "What read:write ratio?"
- "What latency / availability matters most?"
- "Are there constraints I should respect — multi-region, mobile-first, cost-sensitive?"

**Write down** the agreed scope visibly. Treat anything outside as out of scope.

## Step 1: Functional requirements (~3 min)

List concrete user-visible operations. Use bullets, not prose.

- User can post a 280-char tweet.
- User can follow another user.
- User sees a home feed of tweets from people they follow, newest first.

Explicitly mark **non-goals**:

- Direct messages — out of scope.
- Search / hashtags — out of scope (could discuss at end).
- Trending — out of scope.

This prevents scope creep and signals product thinking.

## Step 2: Non-functional requirements (~3 min)

Numbers and qualities the system must meet.

- **Scale**: 200M DAU, average 50 reads + 2 writes/day → ~120K reads/sec, ~5K writes/sec.
- **Latency**: p99 home feed < 200 ms.
- **Availability**: 99.9% target.
- **Consistency**: eventual is OK for the feed; strict for follow graph.
- **Durability**: tweets must not be lost.

## Step 3: Capacity estimation (~5 min)

Back-of-envelope math. See [`04-back-of-envelope.md`](04-back-of-envelope.md) for reference numbers.

Example for Twitter:
- 200M DAU × 2 tweets/day = **400M tweets/day** ≈ 5K tweets/sec average, ~15K peak.
- Tweet size: 280 chars + metadata ≈ 1 KB.
- Storage: 400M × 1 KB = **400 GB/day**, ~150 TB/year.
- Reads: 200M × 50 = 10B reads/day ≈ **120K QPS** average, ~360K peak.

State your assumptions out loud. Round generously. The interviewer cares about **method**, not exact figures.

## Step 4: API design (~5 min)

Concrete endpoints. Method + URL + request body + response.

```
POST /tweets
  body: { text: string }
  -> { tweet_id, created_at }

GET /users/{id}/feed?cursor=...&limit=20
  -> { tweets: [...], next_cursor }

POST /users/{id}/follow
  -> 204 No Content
```

If using gRPC, sketch the service definition. If event-driven, list topics + payload schema.

## Step 5: Data model + storage choice (~5 min)

Schema sketch + justify the storage technology.

```
users(user_id PK, handle, display_name, created_at)
tweets(tweet_id PK, user_id, text, created_at, indexed by (user_id, created_at))
follows(follower_id, followee_id, PRIMARY KEY (follower_id, followee_id))
```

- `tweets` and `follows` could be PostgreSQL (sharded by user_id) or Cassandra (write-heavy, partition by user_id).
- Justify: "Cassandra because writes dominate, we want linear scale, and tweets are append-only."

## Step 6: High-level design (~10 min)

Draw an ASCII (or whiteboard) block diagram. Show data flow for the **two most important paths**: post and read.

```
   +--------+      +-----------+      +----------+
   | Client |---->| API Gateway |---->| Tweet svc |---> Tweet DB
   +--------+      +-----------+      +----------+        |
                                            |             |
                                            v             v
                                       Fanout svc -> Feed cache (Redis)
                                            ^             ^
                                            |             |
                                       +----------+      |
                                       | Follow DB|------+
                                       +----------+
```

State at each block: "this is stateless / horizontally scalable", "this fronts a Redis cluster", etc.

## Step 7: Deep dives + tradeoffs (~10 min)

The interviewer will pick 1-3 components and ask "tell me more". Be ready for:

- **Hot key / celebrity problem**: a celebrity with 100M followers can't be fanned out on write. Hybrid: pull-on-read for celebrities, push-on-write for normal users.
- **Cache strategy**: write-through vs lazy; how do we invalidate when a user blocks another?
- **Sharding strategy**: by user_id (locality of feed) vs by tweet_id (write distribution).
- **Failure modes**: what happens if Redis cluster loses a shard? What's the fallback?
- **Geographic distribution**: multi-region replication; which path needs strict consistency?

For every choice, name the **alternative** and the **reason you didn't pick it**. That's the signal interviewers grade on.

## Anti-patterns to avoid

- **Solutioning before scoping** — diving into Kafka before agreeing on what to build.
- **Buzzword bingo** — "we'll use Kubernetes, Kafka, Cassandra, Spark" without justification.
- **One huge monolith** — show service boundaries, don't dump everything into "the backend".
- **Ignoring failure** — if you don't bring up failure modes, the interviewer will, and that's a worse signal.
- **Skipping numbers** — designs without QPS/storage calculations look hand-wavy.
- **Premature optimization** — don't shard the database unless your numbers justify it.

## Time-budget summary

| Step | Minutes | Output |
|---|---|---|
| 0. Listen + scope | 3 | Agreed feature list |
| 1. Functional | 3 | Bullets of user ops |
| 2. Non-functional | 3 | Latency / scale / consistency targets |
| 3. Capacity estimates | 5 | QPS, storage, BW |
| 4. API design | 5 | Endpoint signatures |
| 5. Data model | 5 | Schema + storage choice |
| 6. High-level design | 10 | Block diagram |
| 7. Deep dives | 10-15 | Bottleneck + tradeoff analysis |

Total: ~45 min, leaving 5-10 min for follow-ups.
