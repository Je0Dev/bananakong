# bananakong gameplay overhaul — plan

Work is on branch `feat/gameplay-overhaul`, one commit per feature. No merge to
`main` unless the user explicitly says "merge". **Tests are written first**,
then the implementations that make them pass. Intermediate commits may not be
green; build + tests must be green before merge.

## 1. Tests first (scaffolding + new suites)
- [ ] `CMakeLists.txt`: add `difficulty.c`, `popup.c`, `highscore.c`, `level.c` to the
      test target (headers/sources listed as they are created)
- [ ] `tests/test_level.c`: rewrite for the new 60x40 stair map; assert every row is
      exactly 60 chars and add a connectivity test (BFS from both spawns to the goal
      over solid+ladder tiles)
- [ ] `tests/test_difficulty.c`: spawn interval ramps 1.8s->0.6s, speed ramps 70->150,
      clamped at extremes, deterministic
- [ ] `tests/test_popup.c`: spawn/update/lifetime/fade and max-active behavior
- [ ] `tests/test_highscore.c`: save/load round-trip via a temp file, best-score logic
- [ ] `tests/test_main.c`: register the new suites alongside existing physics/level tests

## 2. Bigger stage — more floors & stairs
- [ ] Widen screen 640x480 -> 960x640; grid 40x30 -> 60x40 (auto-derived in `constants.h`)
- [ ] Redesign level as 5-flight zigzag staircase from Kong's platform to the ground,
      connected by short junction ladders (map lives in `level.c`, validated by tests)

## 3. Player spawns bottom-left and bottom-right
- [ ] Give `Level` two spawns (`spawn_left`, `spawn_right`); goal stays on Kong's platform
- [ ] Game alternates spawn side each respawn (deterministic, testable)

## 4. Kong throws barrels (both directions)
- [ ] Add `core/kong.c/.h` + `kong_draw.c`: gorilla drawn with raylib shapes, arm-swing pose
- [ ] Barrels spawn from his hand and alternate direction (`+/- BARREL_SPEED`) each throw
- [ ] Barrels cascade down the stairs via gravity plus deterministic junction-ladder drops

## 5. Gameplay features
- [ ] Stomp: land on a barrel from above to break it (+50), bounce, no life lost
- [ ] Barrels visibly hop when reversing or dropping a step (`BARREL_HOP`)
- [ ] `core/difficulty.c/.h`: interval + speed ramp driven by score, wired into game
- [ ] `core/popup.c/.h`: floating +10/+50/+100 text that rises and fades, wired into game
- [ ] `core/highscore.c/.h`: persisted to `bananakong_highscore.txt`, shown on title and
      game-over screens
- [ ] Pause: new `GS_PAUSED` state, `P` toggles, "PAUSED" overlay

## 6. raylib docs & best practices -> markdown files
- [ ] README.md "Resources" section (raylib.com, cheatsheet HTML + PDF v6.0, examples,
      raylib.h reference, wiki + coding conventions/architecture/CMake/FAQ pages)
- [ ] CONSTRAINTS.md "raylib conventions & best practices" section (PascalCase API boundary,
      snake_case internal code, reuse physics/level helpers)
- [ ] AGENTS.md (uncommitted) — add cheatsheet/wiki links to gotchas; commit on this branch

## 7. Build, docs, verify
- [ ] Regenerate README screenshots (stale at 640x480) with a throwaway RenderTexture tool
- [ ] Update `CHANGELOG.md` (Unreleased)
- [ ] Verify: 0 `-Wall -Wextra` warnings, `ctest` green, `timeout 3 ./build/bananakong`,
      `wc -l` <= 120 on all files