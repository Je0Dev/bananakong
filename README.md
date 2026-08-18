# bananakong

A Donkey Kong-inspired platformer written in C with [raylib](https://www.raylib.com/).

## About the game

You play as **a heroic banana** on a single-screen climbing challenge. Barrels
roll down from the top of the stage and tumble across platforms, falling off
edges and even crashing down ladders. Your goal is simple: dodge the barrels,
scale the four wooden platforms, and reach the **golden flag** at the very top.

Each climb tests your timing as much as your reflexes - barrels never stop
coming, so every pause means another one is rolling your way. Get hit and you
lose a life; run out of three lives and it is game over. Score points by
surviving barrels and by winning the climb.

The whole game is rendered with raylib primitives (shapes, not sprites), so it
has no external assets and builds in seconds.

## Features

- Grid-based Donkey Kong-style level: platforms, ladders, and a goal flag.
- Banana hero with smooth movement, jumping, and ladder climbing.
- Rolling barrels that fall off edges and occasionally take ladders.
- Three lives, scoring, and a blinking invulnerability window after hits.
- Title, game-over, and win screens with one-key restart.
- Deterministic pure-logic modules (`physics`, `level`) with unit tests.
- Runs at 60 FPS with a fixed-size 640x480 window.

## Install raylib

### Linux (Fedora / RHEL)
```bash
sudo dnf install raylib-devel cmake gcc make
```

### Linux (Debian / Ubuntu)
```bash
sudo apt install libraylib-dev cmake gcc make
```

### macOS
```bash
brew install raylib cmake
```

### Windows
Install [CMake](https://cmake.org/download/) and a compiler such as
[MSYS2/MinGW-w64](https://www.msys2.org/), then install raylib via MSYS2:
```bash
pacman -S mingw-w64-x86_64-raylib
```
Or build raylib from [source](https://github.com/raysan5/raylib) and point CMake at it.

## Build & run

```bash
cmake -B build
cmake --build build
./build/bananakong
```

The CMake setup finds raylib automatically via its config file, pkg-config,
or a manual library lookup, so it works across all three platforms above.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

Unit tests cover the physics helpers and level tile queries (38 checks).

## Controls

| Action  | Keys                      |
|---------|---------------------------|
| Move    | Arrow keys / WASD         |
| Jump    | Space                     |
| Climb   | Up / Down on a ladder     |
| Start / restart | Enter           |

## Project layout

```
src/
├── main.c              entry point and game loop
├── config/constants.h  shared tuning constants
├── core/               game state, rendering, physics helpers
├── entities/           player and barrel logic + drawing
└── world/              level grid and collision queries
tests/                  unit tests (physics, level)
```

## Docs

- [CONSTRAINTS.md](CONSTRAINTS.md) - coding rules
- [CONTRIBUTING.md](CONTRIBUTING.md) - git workflow
- [CHANGELOG.md](CHANGELOG.md) - release history

## License

MIT. See [LICENSE](LICENSE).