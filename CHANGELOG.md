# Changelog

All notable changes to this project are documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/) and this
project adheres to [Semantic Versioning](https://semver.org/).

## Unreleased

### Added

- Detailed project description in the README.
- Documentation comments across public headers and key game logic.
- Pause with `P` and a pause overlay.
- Score-based difficulty ramp: barrels spawn faster and roll faster.
- Floating score popups when barrels leave the screen or the flag is reached.
- Best score persisted to `bananakong_highscore.txt`, shown on title/game-over.
- `difficulty`, `highscore`, `popup`, and `scoring` modules with unit tests.
- Kenney "New Platformer" sprites for the player, tiles, background, and
  barrels (CC0), loaded via a new `assets` module.
- Sound effects for jumping, barrel throws, scoring, hits, menu, and winning,
  with audio device setup in `main.c`.
- Animated player (walk/climb frames, facing direction) and waving goal flag.
- A boss Kong on the top platform (hero sprite scaled up and tinted) that
  throws barrels from his hand, alternating left/right each throw; his throw
  cadence follows the score-based difficulty ramp, and he paces the platform.
- One throw in three leaves Kong's hand as a high arc that clears a floor or
  two, then rolls left or right once it lands; barrels also have a chance to
  bounce off the screen edge instead of rolling away.
- Two player spawn points (bottom-left and bottom-right); each respawn after a
  hit alternates to the opposite side.
- Stomping a barrel from above smashes it (+50), bounces the player up, and
  costs no life (a falling feet-on-top contact is detected by a pure physics
  helper with unit tests).
- Barrels hop visually when reversing off a wall or landing a full step down;
  the hop is a short sine arc applied to the sprite (draw moved to
  `barrel_draw.c` to respect the line limit).
- Kenney "UI Pack - Pixel Adventure" panels for the title-screen banner, the
  pause/game-over/win overlays, and a fading game backdrop behind the menus,
  drawn via a new `ui_draw` 9-slice helper (corner/border stay crisp at any
  size; `assets/ui/panel_light.png` and `panel_dark.png`).
- Procedural level generation: each run builds a fresh zigzag-staircase level
  from a random seed (`level_gen.c`, xorshift32, deterministic and testable);
  the grid lives in `level.c` and `level_init` now takes a level index and
  seed. Connectivity, no-floating-ladder, and determinism are covered by
  seed-agnostic unit tests.
- Clearing a level advances to the next one: the player keeps score, lives,
  and best, the level index bumps, and the next level is generated from a
  fresh seed (`game_start_level`); the win overlay reads "LEVEL N CLEAR!".
- Difficulty now has two layers: a level/performance baseline (each level is
  tighter, and clearing fast with plenty of stomps makes the next one
  slightly harder) and the existing score ramp on top (`difficulty_for_level`
  and `difficulty_for_state`, unit-tested).
- Barrels animate: they spin like a rolling wheel on the ground (accumulated
  roll angle), tumble along their velocity direction while arcing, and squash
  at the top of a hop (`barrel_draw`).

## [0.1.0] - 2026-08-18

### Added

- Core Donkey Kong-style gameplay: grid level with platforms and ladders.
- Banana hero with movement, jumping, and ladder climbing.
- Rolling barrels that fall off platform edges and take ladders.
- Win condition (reach the golden flag), lives, and score.
- Title, game over, and win screens with restart support.
- Project structure split into `config`, `core`, `entities`, and `world`.
- Pure `physics` helper module and `level` tile queries.
- Unit tests for physics and level logic (38 checks).
- CMake build with raylib discovery (CONFIG, pkg-config, manual fallback).
- Project documentation: README, CONSTRAINTS, CONTRIBUTING, this changelog.
- MIT license.