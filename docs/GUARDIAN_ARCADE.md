# VALTA-030 — Guardian Arcade

Open guardian-arcade.html in a browser. No account, network, real money or provider calls. Six mission outcomes are precomputed by the C++20 VALTA-029 fixture ledger and embedded as JSON. The browser compares answers against those outcomes; it does not execute C++ or authenticate receipts. The browser score is untrusted, session-only and resets on reload. The player chooses predicted decisions rather than controlling a live agent. All three opposing strategies are scripted; the careful baseline reads the fixture answers, so is a teaching reference rather than a blind competitor. The hidden proof is an optional archive discovery, not a new financial event. The mascot is a simple original SVG, not final 3D art. Sound defaults off.

## Reproduce C++ fixtures
From the repository root:
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic -Werror engine/guardian_arcade_export.cpp -o /tmp/guardian-export
/tmp/guardian-export
```
Compare JSON output with the fixtures constant in guardian-arcade.html. Exporter is a temporary single-translation-unit bridge using the existing implementation with its main renamed; split into a library before production. The old main has an explicit return 0 now.

## Actual local QA
- Both exporter and financial regression compiled with C++20 and -Werror.
- Guardian regression: 9 fixture checks, 1000 traces, 60000 events, 5000 negative proof checks, exit 0.
- Chromium via Playwright at 320, 390, 820 px: three complete runs, 18 correct answers, one wrong-answer feedback check, archive discovery, local ranking, three exhausted balances, free reset, no page errors or external requests. No horizontal overflow on tested quest steps.
- Tested source Git blobs matched uploads: financial guardian 3c1d5881473670dec1681f6928d4c8910c196f63; exporter 9a4d4155f5c03dd2b7bbf68bc451d231fe9431dc; HTML 9a4e81d12b7ca021b8272e82a7051b23f0e2a1d3.
- Full CTest, GitHub CI, Firefox/WebKit, audio output and accessibility audit not run in this step.

Next: action-driven arcade interaction, reusable C++ library and local authoritative endpoint, persistence, blind agent submissions. Actual Valta API only after contract and sandbox approval. No deployment, no external outreach. Baseline index.html and main unchanged.
