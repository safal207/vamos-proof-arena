# VALTA-037 — The Real World

## Implemented
After the first simulated Protection Proof, the primary CTA is now a real-product handoff; practice setup remains optional. Five steps: select agent, enable Cap and save run limit, save OpenAI key in official Valta, create virtual key for that same agent, follow official deny guide. No key fields, API traffic, automated spending, or credentials in VAMOS. English/Russian/simplified Chinese retained.

Official indexed references reviewed 2026-11-27: https://valta.co/docs/guides/proxy-setup, https://www.valta.co/docs/concepts/proxy, https://www.valta.co/docs/api-reference/proxy. Direct document fetch failed; indexed copies describe a no-provider-credit denial test and provider-funded allowed calls. Cap setup before provider key; scope per agent. Confirm deployed behavior with founder; no live contract test.

## Local measurement
Optional journey JSON: schema vamos.journey.v1, version, locale, relative first-observed times for allowlisted events start, first_proof, activation_bridge_open, practice_setup_complete, real_setup_open, real_guide_click. No user ID, alias, keys, prompts, IP, absolute timestamps or network submission. Repeated clicks do not change first-observed time. This is self-exported client data, not authenticated analytics. realProtectionVerified and realProxySuccessVerified are null; guide click is intent, not confirmed arrival. Reset clears journey.

## Proposed experiment for Debo (not deployed)
Compare A: corrected direct proxy onboarding, B: first game proof plus same proxy onboarding. Do not retain obsolete deposit flow as a necessary step. Optional baseline historical data only; current nine accounts are too few for causal claims. Randomize new eligible visitors at stable assignment, after appropriate notice/consent. Define population as unique eligible new users assigned; also report registered-user cohort separately. Use pseudonymous experiment ID with server-side linkage on approved Valta domains; no API secrets, prompts, request bodies or raw upstream responses in analytics. No cross-domain tracking until approved. Record canonical events once per user for milestones, with timestamp and experiment assignment; repeat calls separate operational metrics.

| Event | Authority |
|---|---|
| variant_assigned | experiment service |
| game_first_proof | client, diagnostic only |
| real_setup_open / real_guide_click | client, intent only |
| dashboard_arrived | Valta authenticated navigation |
| cap_enabled | saved policy state |
| provider_key_saved | successful management write, no key content |
| virtual_key_created | successful management write, no secret |
| first_real_cap_denial | actual policy denial, reason and no upstream dispatch verified by Valta |
| first_proxy_success | actual successful upstream result through Valta proxy, not approval alone |

Primary metric: unique assigned users with verified first_proxy_success within 7 days / unique eligible assigned users. Secondary: verified cap denial rate, median time to each milestone among achievers, abandonment at each transition, proxy failures by coarse reason. Treat missing as unobserved, not as confirmed failure before the window closes. Report cohort counts and uncertainty, not just percentages. Segment referral source only if reliably available; no inference from IP alone. Sample size and experiment duration to be agreed from baseline and minimum meaningful lift before inference. No real metrics collected yet.

## QA
Actual Chromium Playwright set_content at 320 RU, 390 ZH, 820 EN. Three full seasons, 18 proofs, direct handoff after first proof, link target checked, click twice preserves first timestamp, export JSON exact fields with real outcomes null, locale change preserves serialized state, optional five-step practice preserves 14 credits, return to season and victory, reset clears journey. Zero page errors and HTTP requests (external link navigation deliberately suppressed for click test). No horizontal overflow on tested handoff. No actual site navigation, API calls, full CI, native touch, other browsers or human usability study. Tested HTML blob abb0e54f5ba719ea5f8a870a067e3c699262b1c9 confirmed in GitHub. main and gh-pages unchanged. No publication or messages to Debo.

## Next gate
Founder confirms sandbox contract and approves telemetry and test design. Obtain sanitized actual decision/settlement examples and explicit spending permission before any live proxy test.
