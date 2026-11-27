# VALTA-026 — BitOS-inspired transition memory spike

Independent C++20 experiment, not imported BitOS production code. The existing game and verifier are unchanged. Seven nodes, directed acyclic fixed topology, integer edge costs and a matrix of observation counts and sums. Training (100 synthetic graphs, seed 2601) has full edge-cost visibility; both greedy and dynamic-programming planners use the same frozen average-cost matrix. Evaluation uses actual withheld costs. The full-information oracle is checked against exhaustive search, not a fair competing agent.

| Evaluation | Greedy cost | Memory planner cost | Oracle cost | Wins/ties/losses vs greedy |
|---|---:|---:|---:|---|
| 100 held-out graphs, seed 2602 | 1102 | 786 | 780 | 89/6/5 |
| 100 shifted graphs, seed 2603 | 1120 | 1916 | 1057 | 0/0/100 |
| Same shifted test after 20 separate calibration graphs, seed 2604 | 1120 | 1120 | 1057 | 0/100/0 |

Costs are synthetic units, not actual API tokens. Shift increases the archive-to-proof edge base cost from 1 to 12. Calibration resets memory using independently generated shifted graphs; the same shifted test is reused for comparison. It is not automatic drift detection or online learning. Greedy and planning use identical training data; this tests planning on a learned matrix, not the isolated value of learning itself. No speed claim.

QA: compiled and ran the transition_memory.cpp file locally with GCC 14.2, C++20, -Wall -Wextra -Wpedantic -Werror, AddressSanitizer and UndefinedBehaviorSanitizer. Exit 0, no reported sanitizer errors. Matrix counts, unreachable graph, invalid paths and oracle checks passed across 200 unique held-out graph draws (shifted set evaluated twice). Git blob matches tested source: 6c26f52b684befbe4bd1468a3daafe3a6a2fae1e. Full CMake/CTest suite and GitHub CI not run in this step.

Next: controlled online drift detection, recency/evaporation and reinforcement, sparse topology and cycle-safe planner, ablation vs no memory, budgeted exploration, integration with verifier. No multiplayer, external agent, real payments or deployment.
