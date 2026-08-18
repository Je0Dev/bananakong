# bananakong gameplay overhaul — plan

Work is on branch `feat/gameplay-overhaul`, one commit per feature. No merge to
`main` unless the user explicitly says "merge". **Tests are written first**,
then the implementations that make them pass. Intermediate commits may not be
green; build + tests must be green before merge.

## Workflow rule (always)

- Implement **one feature at a time**: finish it (test + code + verify) before
  starting the next one.
- **Test first**: write the unit test, see it fail, then implement the feature
  until the test passes.
- Verify each feature: build clean, `ctest` green, headless run, `wc -l` <= 120.
- Commit each feature on its own, then move on.

## 8. Kenney sprite + sound integration (done)
- [x] Copy CC0 Kenney "New Platformer" sprites and sounds into `assets/`
- [x] `src/core/assets.c/.h` registry: load/unload textures + sounds, `assets_play`
- [x] Sprite rendering: `level_draw` (background/grass/ladder/flag), `player_draw`
      (idle/walk/jump/climb/hit + facing flip), `barrel_draw` (bomb sprite)
- [x] Sound wiring: jump, throw, coin, hurt, select, gem; audio init in `main.c`
- [x] README Assets section + Kenney credit, CHANGELOG entries, screenshots regenerated

## 1. Tests first (scaffolding + new suites)
- [x] `CMakeLists.txt`: add `difficulty.c`, `popup.c`, `highscore.c`, `level.c` to the
      test target (headers/sources listed as they are created)
- [ ] `tests/test_level.c`: rewrite for the new 60x40 stair map; assert every row is
      exactly 60 chars and add a connectivity test (BFS from both spawns to the goal
      over solid+ladder tiles)
- [x] `tests/test_difficulty.c`: spawn interval ramps 1.8s->0.6s, speed ramps 70->150,
      clamped at extremes, deterministic
- [x] `tests/test_popup.c`: spawn/update/lifetime/fade and max-active behavior
- [x] `tests/test_highscore.c`: save/load round-trip via a temp file, best-score logic
- [x] `tests/test_main.c`: register the new suites alongside existing physics/level tests

## 2. Bigger stage — more floors & stairs
- [ ] Widen screen 640x480 -> 960x640; grid 40x30 -> 60x40 (auto-derived in `constants.h`)
- [ ] Redesign level as 5-flight zigzag staircase from Kong's platform to the ground,
      connected by short junction ladders (map lives in `level.c`, validated by tests)

## 3. Player spawns bottom-left and bottom-right
- [x] Give `Level` two spawns (`spawn_left`, `spawn_right`); goal stays on Kong's platform
- [x] Game alternates spawn side each respawn (deterministic, testable)

## 4. Kong throws barrels (both directions)
- [x] Add `kong.c/.h` + `kong_draw.c`: boss drawn from the hero sprite (scaled,
      tinted) with a bobbing barrel, patrolling the top platform
- [x] Barrels spawn from his hand and alternate direction (`+/- BARREL_SPEED`) each throw
- [x] Barrels cascade down the stairs via gravity plus deterministic junction-ladder drops
- [x] Arc throws: one in three leaves his hand in a high parabola, clearing a
      floor or two before landing and rolling; barrels can bounce off screen edges

## 5. Gameplay features
- [x] Stomp: land on a barrel from above to break it (+50), bounce, no life lost
      (`physics_stomp_hit` pure helper, unit-tested)
- [x] Barrels visibly hop when reversing or dropping a step (`BARREL_HOP`)
- [x] `core/difficulty.c/.h`: interval + speed ramp driven by score, wired into game
- [x] `core/popup.c/.h`: floating +10/+50/+100 text that rises and fades, wired into game
- [x] `core/highscore.c/.h`: persisted to `bananakong_highscore.txt`, shown on title and
      game-over screens
- [x] Pause: new `GS_PAUSED` state, `P` toggles, "PAUSED" overlay

## 6. raylib docs & best practices -> markdown files
- [ ] README.md "Resources" section (raylib.com, cheatsheet HTML + PDF v6.0, examples,
      raylib.h reference, wiki + coding conventions/architecture/CMake/FAQ pages)
- [ ] CONSTRAINTS.md "raylib conventions & best practices" section (PascalCase API boundary,
      snake_case internal code, reuse physics/level helpers)
- [ ] AGENTS.md — add cheatsheet/wiki links to gotchas (committed on this branch)

## 7. Build, docs, verify
- [x] Regenerate README screenshots (still 640x480, matching the current window)
- [x] Update `CHANGELOG.md` (Unreleased)
- [x] Verify: 0 `-Wall -Wextra` warnings, `ctest` green, `timeout 3 ./build/bananakong`,
      `wc -l` <= 120 on all files

## 9. Polish pass — procedural levels, progression, animation, live HUD

Polish and core-mechanics pass on top of the finished gameplay. Same workflow:
one feature at a time, tests first, verify, commit. UI/UX uses the Kenney UI
pack panels added in the previous step. Files stay <= 120 lines (split
draw/helpers as needed).

### 9.1 Procedural level generation (foundation)
- [x] `src/world/level_gen.c/.h` (pure, testable): fills a caller grid buffer
      from a deterministic seed (xorshift32, no raylib RNG so tests are
      stable). Layout: full-width ground, Kong platform row 6 with the goal
      tile, and 1-4 intermediate zigzag platforms; ladders connect
      solid-to-solid (no floating stairs); platform widths vary by seed.
- [x] `level.c`: replace the static `TILES` const with a mutable grid buffer;
      `level_init(&level, level_index, seed)` fills it via `level_gen` and
      derives `spawn_left`/`spawn_right`/`goal` from the generated grid.
- [x] Rewrite `tests/test_level.c` generation-agnostic: for many seeds assert
      row lengths, bounds safety, no floating ladders, both spawns + goal are
      solid-anchored, BFS connectivity from both spawns to the goal, and that
      the same seed reproduces the same grid.
- [x] Each run seeds with `GetRandomValue` (done in `game.c`); showing the run
      seed on the title screen is folded into the 9.6 UI/UX pass.

### 9.2 Win → next level, harder each time
- [ ] `Game.level_index` (starts 1). On goal: record `level_clear_time` and
      `level_stomps` (level-scoped counters), +100 win score, `GS_WIN`
      ("LEVEL N CLEAR!").
- [ ] Enter on `GS_WIN` advances instead of restarting: `game_start_level(g)`
      keeps score/lives/best, bumps `level_index`, generates the next seed,
      respawns the player, clears barrels. Game over still resets the whole run.

### 9.3 Performance-based difficulty
- [ ] `difficulty.c`: new pure `difficulty_for_level(level_index, clear_time,
      stomps)` — base ramp from level index plus a performance factor (faster
      clear and more stomps make the next level slightly tighter: shorter
      spawn interval, faster barrels), all clamped.
- [ ] The in-level score ramp (`difficulty_for_score`) stays and scales on top
      of the level baseline during play.
- [ ] `tests/test_difficulty.c`: level index monotonic, performance factors
      deterministic + clamped, extremes stable.

### 9.4 Cleaner barrel animation
- [ ] `barrel_draw.c`: bomb sprite rolls via accumulated angle proportional to
      speed (DrawTexturePro rotation); brief squash/stretch on hop-landing;
      tilt along the flight path while arcing. Testable angle helpers go in
      `physics.c` if needed.

### 9.5 Kong walk / turn / throw animation
- [ ] `kong.c`: `facing` + a throw phase. Walking cycles the player walk
      frames (tinted, scaled); the sprite mirrors when `patrol_dir` flips.
- [ ] Throw anim: wind-up pose (forward tilt, barrel pulled back) during the
      tail of `throw_timer`, release at spawn, snap-back after. Hero sprite
      frames reused since the pack has no gorilla art.

### 9.6 Live HUD + UI/UX polish
- [ ] HUD (`game_draw.c` → `hud_draw.c`): live score that pulses on change,
      lives, level badge, and an altitude/progress meter (floors climbed vs
      total).
- [ ] Title screen: run seed line + controls hint; "NEW BEST!" banner when a
      run beats the saved high score.
- [ ] Level intro card ("LEVEL N — climb to the flag!", Kenney panel,
      auto-dismiss) on each new level.

### 9.7 Verify
- [ ] 0 `-Wall -Wextra` warnings, `ctest` green (level-gen + difficulty
      suites), headless run, `wc -l` <= 120 on all files.