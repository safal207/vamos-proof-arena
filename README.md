# VAMOS — Proof Arena

**What can your AI agent do? Don't tell us. Prove it.**

Private experimental project inspired by the Valta activation QA research. VAMOS is an educational challenge arena where people solve tasks themselves, then compare their solutions with AI agents under explicit constraints. Proof is a verifiable result, not a purchasable badge.

## Current baseline: VALTA-024 — Living Universe

Open `index.html` in a modern browser. No build, account, API key or backend is needed. The prototype includes three challenges (Hidden Route, Retry Trap, Missing Evidence), a Proof collection, 15 simulated starting credits, a simulated agent competition and local JSON export.

All competitors and credits are simulated. The prototype does not access Valta, use real API tokens, process payments or run an online multiplayer service. Browser data is local. Local results are not trusted server-side evidence.

## Product principles

- First play yourself; then challenge your agent.
- Clear rules, one primary action, optional deeper exploration.
- Reward correct, reproducible solutions and efficient resource use, never spending alone.
- Rounded expressive original characters; tactile buttons and warm food-inspired color palette.
- Hidden clues, optional discoveries and meaningful feedback; no coercive streaks, gambling or paid random rewards.
- Youth learning mode stays free and simulated, without payments.
- Real integrations only after approval, security review and budget controls.

See [roadmap](docs/ROADMAP.md) and [QA baseline](docs/QA.md).

## Local run

```sh
python3 -m http.server 8000
```

Open http://localhost:8000/ on your local machine. Do not expose the server externally.

This is a private experimental repository. No deployment or outreach is authorized by this README. Valta is an intended future integration, not a current affiliation or production service.
