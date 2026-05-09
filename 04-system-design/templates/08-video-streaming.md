# Design a Video Streaming Service (YouTube / Netflix)

## 1. Functional requirements

- Users can upload videos.
- Videos are watchable on web and mobile, at multiple qualities (240p–4K).
- Adaptive bitrate streaming based on bandwidth.
- Search and browse (treat as out-of-scope detail; covered in news feed / typeahead).
- Counts: views, likes (approximate is OK).

**Out of scope**: recommendation engine, monetization, live streaming, content moderation pipelines (could mention).

## 2. Non-functional requirements

- **Read-heavy** at extreme scale: read:write = 1000:1.
- **Low playback start latency**: < 2 seconds time-to-first-frame.
- **Smooth playback**: minimal rebuffering. Adapt bitrate to network.
- **High availability**: 99.95%.
- **Global**: users on every continent.

## 3. Capacity estimates

- **2B users**, 200M DAU, 30 minutes watched per user = 100B viewing minutes/day.
- **500 hours of video uploaded/min** (real YouTube number) → ~30K hours/hour.
- 1 hour of HD video ≈ 1 GB → 30 TB/hour ingest, **720 TB/day uploads**.
- After transcoding to 6 resolutions, ~3-5x multiplier → 2-4 PB/day stored video.
- Bandwidth out: 200M × 30 min × ~5 Mbps avg = enormous. Real number: ~1 PB/hour egress at peak. Most of this comes from CDN edges.

## 4. API design

```
POST /videos/upload-url
  -> { upload_url (presigned S3), upload_id }

POST {upload_url}                         # client uploads to S3 directly
  -> 200

POST /videos
  body: { upload_id, title, desc, tags }
  -> { video_id, status: "processing" }

GET /videos/{video_id}/manifest.mpd       # MPEG-DASH manifest
  -> XML manifest with all bitrate variants

GET /videos/{video_id}/{bitrate}/{segment}.m4s
  -> binary video chunk

POST /videos/{video_id}/view
  -> 204                                  # tally view, fire-and-forget
```

## 5. Data model

```
videos(
    video_id PK,
    user_id,
    title, description,
    duration_sec,
    upload_status ENUM('uploading','processing','ready','failed'),
    created_at,
    canonical_blob_url      -- S3 URL of original
)

video_segments(
    video_id, bitrate, segment_idx,
    cdn_url, byte_offset, duration_ms
)

view_events(video_id, user_id, ts, ...)   -- ClickHouse, append-only
```

## 6. High-level design

```
                      Upload phase
client (browser) ──── S3 multipart upload ───→ S3 (raw bucket)
       |
       v                                          |
+-------------+                                   v
| Video meta  |                            +-------------+
| svc         |  ←──────────────────────── | Transcoder  | (jobs in queue)
+-------------+                            | pool (GPU)  |
       |                                    +-------------+
       v                                          |
+-------------+                                   v
| Postgres    |                            +--------------+
| videos meta |                            | S3 (segments)|--+
+-------------+                            +--------------+  |
                                                              |
                                                              v
                                                       +------------+
                                                       |  CDN edge  |
                                                       |  (global)  |
                                                       +------------+

                      Playback phase
client (player) ─→ DNS / GeoDNS ─→ CDN edge ─→ origin S3 (on miss)
       |
       v
+--------------+
| Manifest svc |
+--------------+
```

## 7. Deep dives

### Upload pipeline

1. Client requests pre-signed S3 URL (resumable upload).
2. Client uploads in chunks directly to S3 (bypasses our origin — saves bandwidth).
3. S3 event triggers the transcoding pipeline.
4. Transcoder enqueued (SQS / Kafka).

### Transcoding

- For each upload, generate **6+ variants**: 240p, 360p, 480p, 720p, 1080p, 4K.
- Each variant is segmented into **2-10 second chunks** (HLS/DASH).
- Each chunk is independently downloadable and cacheable.
- Use ffmpeg or AWS Elastic Transcoder; run on GPU instances for speed.
- Massively parallel: split the source by time into N parallel jobs, then concat.

### Storage

- **Original**: S3 standard for ~30 days, then move to Glacier (long-tail).
- **Transcoded segments**: S3 standard for hot videos; tier to Infrequent Access for cold tail.
- **Metadata**: PostgreSQL with read replicas. ~1 KB/video × 1B videos = 1 TB. Manageable.

### CDN

This is the most important component for cost and latency.

- **Push** popular content to edges proactively.
- **Pull** the long tail on first hit; cache for ~1 day.
- Edges close to users → < 50 ms first-byte latency.
- 90%+ of bytes served by CDN → origin only handles cold misses.
- Major providers: CloudFront, Akamai, Fastly. At YouTube scale, build your own edge POPs (Google has thousands).

### Adaptive bitrate (ABR)

- Manifest (MPEG-DASH `.mpd` or HLS `.m3u8`) lists all bitrate variants.
- Player measures actual download speed of recent chunks.
- Switches up/down between variants at chunk boundaries.
- Goal: maximize quality without rebuffering.

### Playback start optimization

To get first frame in <2s:
- Start at a **low bitrate** (360p) for fast initial start.
- Fetch first 1-2 segments, then upgrade.
- Pre-warm DNS, TLS to CDN edge.
- Use HTTP/2 or QUIC for fewer round trips.

### Counters: views, likes

Don't write per-event to a relational DB — at 100K views/sec it would die.

Pipeline:
1. Player POST `/views` → fire-and-forget to a logging endpoint.
2. Endpoint writes to **Kafka**.
3. Stream consumer (Flink) aggregates into ClickHouse and rolls up minute → hour → day.
4. Dashboard reads aggregates.
5. Approximate counter (HyperLogLog) for unique viewers.

### Video deduplication

If two users upload the same video:
- Hash the original (perceptual hash for video — pHash + audio fingerprint).
- Detect duplicates; share the same canonical asset to save 5x storage cost.
- Important for copyright detection too.

### Failure modes

- **CDN edge down**: client retries on different edge; GeoDNS routes around.
- **Transcoder backlog**: video stays in "processing" — non-fatal. Backoff producers.
- **Origin S3 down**: rare; CDN still serves cached content. Hot videos unaffected.

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| Direct S3 upload | Bypass origin, save BW | Through API server (slower, expensive) |
| HLS/DASH ABR | Industry standard, segmented | Progressive download (no adapt, big files) |
| 6 fixed bitrates | Predictable cache hits | Per-device-class adaptive (more variants, lower cache hit) |
| CDN-heavy | Cost + latency | Origin-only (cost-prohibitive at scale) |
| Aggregated counters | Cheap, scalable | Per-event in DB (won't scale) |
| Postgres metadata | ACID, queryable | DynamoDB (worse for joins, scale isn't the issue here) |

### Cost levers

- Most cost is **egress bandwidth**, not storage. CDN deals account for most of YouTube's infra cost.
- Tier old segments to cheap storage; recompute on demand if ever needed.
- Encode efficiency matters — VP9/AV1 cuts bandwidth 30-50% vs H.264 but costs CPU at encode time.

### Live streaming variant (briefly)

If asked: replace the upload pipeline with a low-latency ingestion (RTMP / SRT) + chunked transcoding + LL-HLS / WebRTC for viewing. Latency targets shift from 2s startup to ~3s end-to-end glass-to-glass.
