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