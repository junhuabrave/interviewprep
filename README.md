# Interview Prep — Mid-level SWE (L4/E4)

A 3–4 week curriculum covering **system design** and **coding in Python 3.12, C++20/23/26, and TypeScript (latest)**.

## How to use

Each section is a self-contained reference. Don't try to read everything top-to-bottom — instead:

1. Skim the cheat sheets in [`01-dsa-cheatsheets/`](01-dsa-cheatsheets/) and [`02-language-cheatsheets/`](02-language-cheatsheets/) to refresh.
2. Study the worked examples in [`03-coding-patterns/examples/`](03-coding-patterns/examples/) — read the notes, then trace each language solution.
3. Use [`03-coding-patterns/README.md`](03-coding-patterns/README.md) as a decision tree when you encounter new problems.
4. Walk through the system design templates in [`04-system-design/templates/`](04-system-design/templates/) one per day in week 3–4. Practice them out loud.
5. Rehearse behavioral answers using [`05-behavioral/star-templates.md`](05-behavioral/star-templates.md).

## 4-week schedule

### Week 1 — Foundations
- [ ] Read all 11 DSA cheat sheets ([`01-dsa-cheatsheets/`](01-dsa-cheatsheets/))
- [ ] Read all 3 language cheat sheets ([`02-language-cheatsheets/`](02-language-cheatsheets/))
- [ ] Read the pattern decision tree ([`03-coding-patterns/README.md`](03-coding-patterns/README.md))
- [ ] Walk through example 1: Two Sum in all 3 languages

### Week 2 — Coding patterns deep dive
- [ ] Walk through example 2: LRU Cache in all 3 languages
- [ ] Walk through example 3: Course Schedule in all 3 languages
- [ ] Daily: 1–2 LeetCode mediums, rotating language each day
- [ ] Re-derive any pattern's template from memory at end of week

### Week 3 — System design fundamentals
- [ ] Read [`04-system-design/01-fundamentals.md`](04-system-design/01-fundamentals.md)
- [ ] Read [`04-system-design/02-building-blocks.md`](04-system-design/02-building-blocks.md)
- [ ] Read [`04-system-design/03-interview-framework.md`](04-system-design/03-interview-framework.md)
- [ ] Read [`04-system-design/04-back-of-envelope.md`](04-system-design/04-back-of-envelope.md)
- [ ] Walk through 4 templates: URL Shortener, Rate Limiter, Key-Value Store, News Feed

### Week 4 — Mock practice + behavioral
- [ ] Walk through remaining 4 templates: Chat, Typeahead, Web Crawler, Video Streaming
- [ ] Practice each system design **out loud** to a timer (45 min each)
- [ ] Rehearse 5 STAR stories from [`05-behavioral/star-templates.md`](05-behavioral/star-templates.md)
- [ ] Daily: 1 timed coding problem (45 min) + 1 system design walkthrough (45 min)

## Index

| Section | Path |
|---|---|
| DSA cheat sheets | [`01-dsa-cheatsheets/`](01-dsa-cheatsheets/) |
| Language cheat sheets | [`02-language-cheatsheets/`](02-language-cheatsheets/) |
| Coding patterns + 3 examples | [`03-coding-patterns/`](03-coding-patterns/) |
| System design | [`04-system-design/`](04-system-design/) |
| Behavioral STAR | [`05-behavioral/star-templates.md`](05-behavioral/star-templates.md) |

## Running the code examples

```bash
# Python (3.10+ is fine; idioms target 3.12)
python3 03-coding-patterns/examples/01-binary-search/solution.py

# C++20 (clang++ on macOS works; g++ if you have GNU stdlib via Xcode/Homebrew)
clang++ -std=c++20 -O2 03-coding-patterns/examples/01-binary-search/solution.cpp -o /tmp/sol && /tmp/sol

# TypeScript via tsx (auto-installed by npx the first run)
npx tsx 03-coding-patterns/examples/01-binary-search/solution.ts
```

Each `solution.*` file includes a runnable test block that prints `PASS` (or fails fast). All 15 problems × 3 languages have been verified.

To run the full bank:
```bash
bash 03-coding-patterns/verify-all.sh
```
