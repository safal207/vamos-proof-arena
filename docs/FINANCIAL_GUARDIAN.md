# VALTA-029 — Financial Guardian
Offline C++20 fixture simulator based on the provisional VALTA_FIRST.md mission specification. Not Valta implementation or verified API behavior. No real credentials, provider calls, financial transactions or deployment.

## Model
Integer simulated cents; one agent, one run, fixed day/month (no rollover). All three limits apply to settled + reserved exposure. Admission reserves the estimate; success up to estimate settles actual; fixture upstream 4xx releases; unknown holds reservation. An over-estimate result is marked uncertain and freezes new admissions, not treated as known zero or fully accounted debt. Explicit trusted resolution within the estimate can settle; administrative thaw is explicit. There is no production reconciliation, no authorization enforcement, no real rate limiter (rate denial is an injected fixture flag), no cancellation of in-flight requests, and no automatic retry. Duplicate valid request IDs are consumed within a ledger instance, even for denied requests; duplicate completion is invalid. All rules are local design choices pending sandbox confirmation.

First Deny: request 1000, run cap 300, dispatched=false, settled=reserved=0.
Stop the Loop: three 100-cent reservations, fourth denied.
Freeze: blocks new requests; already admitted requests can settle.
Error Detective: local rate denial vs upstream 4xx and uncertain outcome.
Find the Proof: compare receipts including original event identity, status, dispatch flag, balances, and order with replay of trusted trace and caps. The same implementation issues and verifies these receipts; no cryptographic signature, external witness, or independently authored verifier. Trusted inputs are a precondition.
Agent Challenge and UI wiring remain future work. Fifteen practice credits are separate from simulated cents and real provider tokens.

## QA evidence
GCC 14.2.0, C++20, -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g. Executed directly, exit 0, no sanitizer diagnostics. Git blob of tested source: 6007e3530d0394cf5756c33f43290bf20522786f (confirmed after upload).

Output: PASS 9 fixture checks; 1000 traces; 60000 events; 5000 negative proof checks

Seed 2901; 60 synthetic events per trace. The 5000 checks cover changed reserved amount, dispatch, settled amount, truncated receipt list, altered request identity. All 60000 original events checked for nonnegative accounting, pending-sum consistency and exposure within configured caps. These are synthetic sequential scenarios, not traffic load or concurrent tests. Replays additionally recheck invariants. CTest target registered; full CMake/CTest and GitHub Actions not run in this step.

## Reproduce
g++ -std=c++20 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g engine/financial_guardian.cpp -o guardian
./guardian

## Next
Expose fixture-driven missions in HTML, preserve baseline. Require sandbox permission, sanitized genuine Valta responses, contract confirmation (including uncertain/over-estimate settlement), trusted evidence admission and actual limits before connecting agents. No real billing until approval.
