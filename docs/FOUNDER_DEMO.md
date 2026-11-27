# VALTA-031 — Founder demo: The Proof Hunt

Open vamos-demo.html in a modern browser. It is a standalone English-language, mobile-first concept for Valta activation. No setup, no account, no API key, no real money. The optional Valta docs link is the only external navigation; it is not opened automatically.

## Debo's walkthrough (about 2–3 minutes, target not measured)
1. Open VAMOS and press Protect the budget: $10 request, $3 cap, no dispatch, no simulated charge, first Protection Proof.
2. Finish the six missions: budget reservations, retry loop, freeze, uncertain outcome, local rate denial and evidence matching. Each completed mission adds a local Proof. Six successful missions consume six practice credits; wrong actions consume additional credits where available. A started mission can finish at zero; free restart is always available.
3. Claim victory. Explore the hidden route (S–B–E–P, five route units).
4. Copy the route challenge to a personally chosen AI, paste only its JSON result, verify locally and compare with the human and fixed reference routes. The submitted answer is not authenticated as an agent run. No model connection or secret is required.
5. Export the local evidence JSON; optionally follow the Valta documentation link.

## Activation hypothesis
The first simulated Proof takes two clicks from the introduction. This is an interaction count, not measured human activation time or conversion improvement. Local export captures elapsed time from Start, action outcomes, first proof, mission completions and pasted-route verification. It is session-only, no analytics collection or tracking requests; reload/restart clears state. Future opt-in funnel test should measure registration-to-first-meaningful-Valta-action, not just game completion.

## Implementation and limits
C++20 VALTA-030 precomputed financial fixtures are reused verbatim; this standalone HTML does not execute C++ at runtime. Correct choices advance fixed fixture traces; incorrect choices are teaching feedback rather than a real financial simulation of alternative actions. The route puzzle verifies graph edges and cost in JS; fixed reference routes are not live bots. There is no public multiplayer service, actual Valta integration, authentication, payment or production security claim. Mascot is original gradient SVG, not final 3D art. No publishing or messaging authorized.

## QA (actual local browser runs)
Chromium via Playwright: 320, 390, 820 px; each completed six missions (13 correct actions plus intentional wrong action), hidden route, rejected invalid agent JSON path, valid optimal path and JSON export, 15 wrong actions exhausting credits, free restart. No page errors, no external requests, no horizontal overflow on tested steps. Separate test confirmed last-credit continuation and nonnegative balance. Screenshots captured at 390 px. Local file Git blob matched uploaded demo: 866928c5a6797a092ca3ce9105ef225f4134e775. No new C++ build or full CI run, no Firefox/WebKit or human usability study. Baseline index.html and main unchanged.

## Founder feedback request
Could a new user understand Valta's value before setup? Which step would Debo change? Can he measure dashboard open, first key creation, first successful call and first real protection event? Review exact sandbox API and receipts before any integration.
