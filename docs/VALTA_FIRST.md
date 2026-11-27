# VALTA-028 — Valta First
Research and mission specification, 2026-11-27. No authenticated requests or runtime integration performed.

## Product decision
VAMOS is a practice and acquisition layer for Valta, not a replacement financial service. Prioritize a first successful, explainable Valta denial and then a user-owned agent integration. C++ owns simulation and competition verification; Valta owns actual financial decisions. Metro-inspired receipts are separate local artifacts, never substitutes for Valta evidence.

## Public source map
- https://valta.co/docs — Demo Deny, Cap, proxy and quickstart index.
- https://www.valta.co/docs/quickstart — existing agents use a proxy or cooperative integration, not the built-in AI demo.
- https://www.valta.co/docs/api-reference/proxy — OpenAI proxy contract: estimated/actual cost, approval and denial headers, settlement, freeze and rate limits.
- https://www.valta.co/docs/concepts/proxy — deny reasons, upstream forwarding, no retries by Valta.
- https://www.valta.co/docs/concepts/cap — cooperative allow() gate, run/day/month cap; direct provider calls can bypass it.
- https://www.valta.co/docs/concepts/audit-trail — client-side chain verification and export.
- https://www.valta.co/docs/guides/proxy — separate financial-action proxy, caller-declared amounts, best-effort receipt recording.
- https://www.valta.co/docs/guides/key-safety — metadata and data-handling limitations.
- https://www.valta.co/docs/api-reference/intents — advanced intent proof, separate from LLM receipts.

Public search excerpts show versions of Cap documentation that describe hosted proxy as planned and others that describe it as available. Financial-action proxy and LLM proxy are distinct; do not unify paths or assume deployment parity. Full llms.txt and direct proxy/Cap page retrieval failed in this research session; the indexed excerpts support the provisional mapping only. Confirm deployed contract with founder and authorized sandbox.

## Season 01 — Protect your agent
All below are proposed local fixtures and acceptance requirements, not actual Valta observations.
| Mission | Documented concept | Practice evidence | Integration gate |
| --- | --- | --- | --- |
| 01 First Deny | Demo Deny: request $10 against $3 cap | denied, zero simulated debit | inspect actual demo event and audit scope |
| 02 Stop the Loop | per-run Cap and proxy | three synthetic allowed requests and a fourth denied at fixed cap | confirm shared run identity and exact reason |
| 03 Freeze | agent freeze/unfreeze | subsequent calls denied while frozen | verify no upstream dispatch; do not claim cancellation of in-flight work |
| 04 Error Detective | Valta rate-limit versus upstream error after approval | distinguish 429 using decision metadata; no blind retry | confirm actual headers and settlement |
| 05 Find the Proof | decision identifiers and audit verification | match attempt and evidence, reject edited fixture | obtain sanitized real sample and canonical verification contract |
| 06 Agent Challenge | OpenAI-compatible governed execution | score correctness then resource cost under same cap | sandbox credentials, spending approval, server-side evidence admission |

## Money and trust
15 starting VAMOS practice credits are virtual and cannot buy or imply OpenAI token entitlement. Proxy reference says provider usage is billed by OpenAI, not resold by Valta. Current plan limits and proposed pay-as-you-go VAMOS business model are distinct. No real purchases or account changes; validate economics and founder approval separately.
A Valta approval is not equivalent to a successful upstream request. Documented upstream 4xx releases estimated usage; timeout/5xx may retain estimate. Do not assume exact zero cost where evidence is absent. A hash or local replay is not an authenticated receipt or independent witness.
Do not log keys, prompts, private user context or full upstream payloads. Public leaderboard only contains authorized minimal metadata.

## P0 next
Implement offline C++ fixture verifier for decisions and settlement; test first deny, cumulative run budget, freeze, rate-limit distinction and unknown outcome. UI exposes a single next action and actual scope of each Proof. Then request founder-approved sandbox and sanitized sample responses; live integration only after explicit authorization. No direct provider calls, live financial effects, or public deployment in this stage.
