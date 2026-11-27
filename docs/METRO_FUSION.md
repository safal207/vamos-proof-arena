# VALTA-027 — Metro Proof Fusion

Independent C++20 experiment inspired by Liminal Rail Metro's evidence bundle, replay verification and EWMA. It is not a port of Metro, does not implement its seven-artifact bundle or wire protocol, and does not call Metro. No changes to the game or its 15-credit practice rules.

## Reproduce

Requires C++20 and OpenSSL development libraries.
```sh
cmake -S . -B build -DVAMOS_METRO_FUSION=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The new source was actually compiled and executed directly with GCC 14.2.0, OpenSSL 3.5.7, -Wall -Wextra -Wpedantic -Werror and AddressSanitizer/UndefinedBehaviorSanitizer; exit 0. Tested Git blob: 54d93f1789573e3feb722fc24e06c9c54b2a6472. Follow-up local full CMake/CTest run with VAMOS_METRO_FUSION=ON and -Werror: GCC 14.2.0, OpenSSL 3.5.7, all three targets compiled and 3/3 CTest tests passed (verifier, transition_memory, metro_fusion). The four build/source Git blob hashes were matched to the branch before running. All reported experiment totals reproduced exactly. GitHub CI has not been executed for this revision.

## Experiment

100 training graphs (seed 2701), 1000 sequential test graphs (seed 2702). Four regimes of 250 draws: original, archive expensive, alternate route cheaper, original again. Fixed seven-node DAG with four valid complete paths. Strategies select before seeing current costs; after each scenario all adaptive planners get full cost feedback (including unused edges). The oracle sees current costs and is a lower bound, not an eligible competitor. The test budget is 24 synthetic units to admit all generated routes, not the 15-credit user economy.

| Strategy | Total synthetic cost |
|---|---:|
| Frozen average-cost matrix | 10720 |
| EWMA (alpha 0.2) | 8794 |
| EWMA + evaporating route trail | 8776 |
| Full-information oracle | 8368 |

Trail: decay 0.85, add inverse cost to the observed best route, subtract twice trail from estimated route cost. This is deterministic ant-inspired route reinforcement, not full ant-colony optimization. It contributes only 18 units of incremental savings (~0.2% vs EWMA). EWMA accounts for most of the improvement. No parameter tuning on additional seeds, partial feedback experiment, throughput benchmark or generalization claim.

## Receipts

Versioned SHA-256 digest binds trusted graph, attempt ID, budget, path and computed cost. Replay uses the externally supplied graph, ID and budget, recomputes the digest and route cost. The issuer and replay share the same cost function: this is a separate validation call, not independent implementation or independent witness. 4000 successful replay checks, and 8000 negative checks: altered cost, recomputed digest over false cost, invalid route, wrong identity, altered budget, modified graph, mismatched budget, duplicate ID in a local registry (1000 each). SHA-256 known-answer vector also checked. The duplicate registry is test-local, not durable admission control. A valid alternate path can be rehashed; unsigned digests do not authenticate origin or prove real-world execution.

## Next

Separate verifier library and independent implementation; versioned authoritative challenge manifest; authenticated attempt admission; durable atomic deduplication and accounting; proof bundle export with canonical bytes and provenance; then local API and online leaderboard. Partial feedback and independent seeds before claiming robust learning. No public deployment, agent API calls, real billing or outreach.
