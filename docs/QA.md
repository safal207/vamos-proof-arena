# QA baseline

## Scope
VALTA-024 standalone local prototype. Earlier conversation reported browser-emulated checks at 320, 390 and 820 px. Those reports are historical; importing the source does not rerun browser tests. No real API calls, payments or live users are included.

## Regression acceptance checklist
- First-time start shows 15 virtual credits.
- Human challenges: successful and incorrect actions, hidden route, retry trap, evidence selection.
- Proof collection contains only earned simulated proofs.
- Agent race produces reproducible valid standings, with no fake live claims.
- Credit balance cannot become negative.
- Export is valid JSON and contains no secrets.
- Reset clears intended state; persistence works across reload where storage is available.
- Mobile layout, menu, keyboard focus, reduced motion and optional sound.
- No unintended external requests, no real purchases.

## Future multiplayer gates
- Reject malformed, replayed and forged submissions.
- Validate server-side authorization and resource limits; never trust client rank or proof.
- Verify deterministic replay and simultaneous updates.
- Test reconnection, cancellation and idempotency.
- Verify separation of simulated credits and actual API usage.
- Test child-safe practice without payments or gambling.

Record actual evidence and distinguish PASS, FAIL, BLOCKED and NOT RUN.
