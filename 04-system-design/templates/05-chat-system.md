# Design a Chat System (WhatsApp / Messenger)

## 1. Functional requirements

- 1-on-1 messaging.
- Group chats up to ~100 members.
- Send / receive text messages.
- Online presence (online / last-seen).
- Delivery + read receipts.
- Push notifications when offline.

**Out of scope**: end-to-end encryption details (would mention Signal protocol), voice/video calls, file attachments beyond a brief mention.

## 2. Non-functional requirements

- **p99 send → receive latency < 200 ms**.
- **99.99% availability** for message delivery.
- **Durability**: messages must not be lost.
- **Ordering**: messages within a conversation in order.
- **Scale**: 1B users, 100M concurrent connections, 100B messages/day.

## 3. Capacity estimates

- 100M concurrent users → need persistent connections (WebSocket/MQTT).
- 100B msgs/day = ~1.2M msgs/sec average, peak 5M/sec.
- Avg msg size 100 B + metadata 200 B = 300 B → 30 TB/day raw, with replication 100 TB/day.
- Connection servers: at 100K connections per server, need ~1000 servers minimum.

## 4. API design

WebSocket-based:
```
client → server: { type: "send", to_user_id, message_id (uuid), text, ts }
server → client: { type: "ack", message_id, status: "delivered" }
server → client: { type: "msg", from_user_id, message_id, text, ts }
server → client: { type: "presence", user_id, status }
```

REST for history:
```
GET /conversations/{conv_id}/messages?before=cursor&limit=50
GET /conversations  -> list of recent conversations
```

## 5. Data model

```
conversations(conv_id PK, type ENUM('1on1', 'group'), created_at)
conversation_members(conv_id, user_id, joined_at)
messages(conv_id, message_id (Snowflake, sortable by ts), sender_id, text, ts)
              -- partition by conv_id, clustered by ts DESC
                  -- in Cassandra/ScyllaDB

user_inbox(user_id, message_id, conv_id, ts, read_ts)
              -- per-user list of recent unread/recent
```

Also maintain an in-memory `connection_id → user_id → server_id` mapping (Redis) for routing.

## 6. High-level design

```
                                   +---------------------+
   +--------+     WebSocket        | Connection servers  |
   | Mobile |------------------->  |  (stateful, sticky) |
   +--------+                      +---------------------+
                                          ^      |
   +--------+                              \    /
   | Mobile |  <----- WebSocket -----  +-----------+
   +--------+                          | Pub/Sub   |  Kafka or Redis Pub/Sub
                                       +-----------+
                                          ^      ^
                                          |      |
                                  +-------+      +------+
                                  | Message svc         |
                                  | (validate, persist) |
                                  +---------------------+
                                            |
                                            v
                                       +----------+
                                       | Cassandra |
                                       | messages  |
                                       +----------+
                                            |
                                            v
                                  +-----------------+
                                  | Push gateway    | (APNs / FCM)
                                  +-----------------+
```

### Send flow
1. Client sends `send` over its WebSocket to its connection server.
2. Connection server forwards to **Message service** (gRPC).
3. Message service:
   - Generates Snowflake `message_id` (sortable by time).
   - Persists to Cassandra (`messages` partition = `conv_id`).
   - Publishes to Kafka topic keyed by `conv_id`.
4. Kafka consumers running on each connection server subscribe to topics for their conversations and push to recipient sockets.
5. ACK back to sender.

### Receive flow
- If recipient online (connected): pushed via WebSocket.
- If offline: send push notification via APNs/FCM. On open, client fetches missed messages.

## 7. Deep dives

### Connection servers

- **Stateful**, **sticky** (a user's connection lives on one server until disconnect).
- Maintain `local_map: user_id → socket`.
- Run a Kafka consumer per server, subscribed to "conversations whose members are connected here". Dynamic subscription.
- Heartbeats every 30 s; on missed heartbeat, mark offline.
- Load balancer routes by user_id hash → consistent server, helps reconnects find old session faster.

### Message ordering

- Within one conversation, messages must arrive in send-order.
- **Snowflake ID** gives global sortable timestamps with worker ID and sequence — no need for clock sync.
- Sender numbers messages with monotonic seq; server validates and gaps trigger retransmit.
- Recipient's client sorts by `(ts, message_id)`.

### Storage engine choice

Cassandra fits because:
- **Append-only**, no updates.
- **Partition by `conv_id`** → all messages in one chat colocated; range scan by ts is one disk seek.
- Linear write scale.
- Good cross-region replication.

For huge active conversations (group chats with thousands), partition further: `(conv_id, hour_bucket)`.

### Group chat fanout

For a 100-member group:
- Persist message once.
- Lookup `conversation_members` → list of 100 user_ids.
- Publish to Kafka → connection servers send to online members.
- For offline members, queue a push notification.
- This is fanout-on-read for storage, fanout-on-write for delivery.

For groups of 1000+ members, throttle pushes and consider a separate broadcast-friendly protocol (publish to a topic per group; clients subscribe directly).

### Presence

- **Online**: WebSocket connected.
- Each connection server emits `online`/`offline` events to a presence service backed by Redis (`user_id → state, ts`).
- Subscribers get presence updates via pub/sub. Sample (don't push every status change to every contact at scale).

### Push notifications

- For offline users, the message service emits to a push gateway.
- Push gateway batches per device and sends to APNs (iOS) / FCM (Android).
- Idempotency: include `message_id` so the client de-dupes on next sync.

### Read receipts

- When recipient renders a message, client emits `{type: "read", message_id, conv_id}` to server.
- Server updates `messages` (sparingly — could be a separate `message_states` table to avoid hot rewrites).
- Pushed to sender's online sessions.

### End-to-end encryption (mention briefly)

- Server stores ciphertext only.
- Signal Protocol: X3DH key agreement + Double Ratchet for forward secrecy.
- Implication: server can't search content, can't show rich link previews server-side.

### Failure modes

- **Connection server crashes**: clients reconnect via LB to a different server; in-flight messages either ACKed or retried by client.
- **Kafka partition lag**: messages delayed, not lost; clients see them via history fetch on reconnect.
- **Cassandra node down**: replicas serve.
- **Push gateway outage**: queue and retry with backoff.

### Tradeoffs and alternatives

| Choice | Why | Alternative |
|---|---|---|
| WebSockets | Bidirectional, low overhead, persistent | Long polling (high overhead); MQTT (good for low-bandwidth mobile) |
| Kafka pub/sub for fanout | Replay, decouple producers/consumers | Redis Pub/Sub (no persistence); direct gRPC (couples) |
| Cassandra | Append-heavy, partition by conv_id | DynamoDB (managed alternative); HBase |
| Snowflake IDs | Sortable, decentralized | UUIDv4 (random, not sortable); DB sequence (single point) |
| Sticky connection servers | Simpler routing, in-memory state | Stateless + every msg through pub/sub (more pub/sub load) |
| Push notifications via APNs/FCM | Required (only path to offline iOS/Android) | n/a |
