# VALTA-025 — C++20 verifier spike

This branch keeps the VALTA-024 HTML untouched. The C++ executable replays typed actions for the same three missions and derives simulated receipts, budget and ranking. No networking, real tokens, payments, model calls, database or server authentication. The executable currently runs self-tests, not a public submission API. Receipts are replay results, not signed real Valta audit records.

Build: cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure

Rules: 15 practice credits, graph edges copied from the baseline, wrong answers cost 2, correct answers cost 1. Invalid traces are excluded from ranked results. Rank complete valid traces by remaining credits descending; ID is a deterministic display tie-break, not a competitive advantage. Next: separate library and service, unique authenticated attempt IDs, server-issued challenge version, persisted trace and atomic budget accounting, WebSocket leaderboard. No production integration without review.
