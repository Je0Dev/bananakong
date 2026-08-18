# AGENTS.md

Donkey Kong-style platformer in C99 + raylib. Dev machine is Fedora with
`raylib-devel` 5.5 installed system-wide (shared lib only). The game runs
here via `DISPLAY=:0`. `gh` is authenticated to `Je0Dev/bananakong`.

## Commands

- Configure/build: `cmake -B build && cmake --build build` (Release by default;
  sources are listed explicitly in `CMakeLists.txt`, no globbing)
- Tests: `ctest --test-dir build --output-on-failure`
- Run game: `./build/bananakong` (verifiable headlessly with `timeout 3 ./build/bananakong`)
- `build/`, `build-*/`, `dist/` are gitignored staging areas

## Must-follow rules (full text: CONSTRAINTS.md, CONTRIBUTING.md)

- Never commit to `main`. Every change goes on its own branch (`feat/`,
  `test/`, `fix/`, `docs/`, `chore/`), merged with `--no-ff`.
- **Do not merge to or push `main` unless the user explicitly says "merge".**
  Otherwise push the branch and leave it for the user to approve.
- **One feature at a time**: test first (write test, see it fail, implement
  until green), then verify build + tests, then commit and move on.
- Conventional Commits; update `CHANGELOG.md` (Unreleased) with each push.
- Files ≤ 100–120 lines; no monolithic functions; no `TODO`; comment *why*;
  zero `-Wall -Wextra` warnings; build + tests green before merge.
- Tests are for **really important features** (physics, scoring, persistence),
  not for everything implemented. Small glue helpers need no tests.

## Architecture

- `src/config/constants.h` — single home for all tuning constants
  (window, tile size, physics, lives). Tweak the game here.
- `src/core/` — `game.c` (state machine + update), `game_draw.c` (rendering),
  `physics.c/.h` (pure, testable math/AABB helpers).
- `src/entities/` — `player.c` + `player_draw.c`, `barrel.c/.h`. Update
  functions call raylib input directly.
- `src/world/` — `level.c/.h`: char-grid map (`#` platform, `L` ladder, `G`
  goal) plus tile/rect collision queries.
- `src/main.c` — entry point: InitWindow, fixed loop (update → draw), close.

## Gotchas

- raylib `Rectangle` fields are `width`/`height`, **not** `w`/`h`; `Vector2`
  is `x`/`y`.
- raylib is shared-only here (no `.a`); a release binary needs `libraylib.so`
  on the target machine.
- Only pure logic is unit-tested. `game`/`player`/`barrel` update code reads
  keyboard input, so the test target compiles only pure modules (`physics.c`,
  `level.c`, `difficulty.c`, `popup.c`, `highscore.c`). Add any new testable
  module to that target in `CMakeLists.txt`.
- Constant macros must not end in `_H` — they collide with header include
  guards (silently empty-include bug, fixed once already). Use e.g.
  `PLAYER_HEIGHT`, not `PLAYER_H`.
- Headers are included flat (`"level.h"`), not by relative path — all src
  subdirs are on the include path via CMake.

## Repo state

- Released: `v0.1.0` on GitHub (banner/screenshots pending on unmerged branch
  `docs/readme-assets`, awaiting user review). Start new work from `main`.