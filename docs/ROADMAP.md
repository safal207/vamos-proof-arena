# VAMOS development roadmap

## VALTA-024 — imported baseline
- Preserve the standalone prototype in index.html.
- Human quest, simulated agents, virtual credits and proof collection.

## VALTA-025 — multiplayer foundations
- Define a versioned challenge specification: input, allowed actions, resource caps, objective and verifier.
- Implement a deterministic server-side verifier, immutable attempt IDs and replayable decision traces.
- Rank valid solutions by correctness first, then declared resource cost and deterministic tie-breaking.
- Add authenticated sessions and a server-authoritative live leaderboard with reconnect handling.
- Keep local practice free; distinguish simulated from independently verified proof.

## VALTA-026 — bring your own agent
- Design provider-neutral submission and sandbox adapter.
- Separate agent permissions, secrets, actual token usage and virtual credits.
- Apply rate limits, idempotency, timeout, explicit spending ceilings and safe cancellation.
- Never expose private prompts, secrets or context in the public scoreboard.

## VALTA-027 — proof economy experiment
- Model cost per validated run and credit redemption under pessimistic usage.
- Starting allocation of 15 virtual credits is a hypothesis, not an API token entitlement.
- Adult opt-in purchases may be evaluated only after costing, legal and security review. No wagering or paid random prizes. No purchases in youth practice mode.
- A higher budget must not buy a higher competitive score.

## VALTA-028 — first season and design
- Original rounded mascots, tactile controls, mobile-first layouts.
- Strategy, context, discovery and safe refusal challenges.
- Original hidden clues, replayable puzzles and optional shareable evidence cards.
- Test comprehension, time to first proof and willingness to challenge an agent.

## VALTA-029 — Valta integration gate
- Founder agreement, API documentation, sandbox, anonymized logs and threat model.
- Validate real request limits, evidence integrity, retries and freeze/unfreeze enforcement.
- No live payments, deployment or customer outreach without explicit approval.

Each stage is a proposed backlog item, not a delivered feature.
