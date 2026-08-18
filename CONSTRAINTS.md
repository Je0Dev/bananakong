# Constraints

These rules apply to every commit in this repository. Reviews must enforce them.

## Git workflow

- `main` is protected: **never commit directly to it**.
- **Every feature, test, or fix gets its own branch** created from an up-to-date
  `main`, then merged with a merge commit. Example:
  - `feat/<name>` for gameplay features,
  - `test/<name>` for new or updated tests,
  - `fix/<name>`, `docs/<name>`, `chore/<name>` for the rest.
- Commit messages use Conventional Commits (`feat:`, `test:`, `fix:`, `docs:`,
  `chore:`). See [CONTRIBUTING.md](CONTRIBUTING.md) for the full workflow.
- Tests must pass on a branch before it is merged into `main`.

## Code size

- Every source file must stay within **100-120 lines maximum**.
- If a file grows past the limit, split it (e.g. separate `*_draw.c` for rendering).
- Header files should stay small (declarations only, no logic).

## Functions

- Keep functions small, focused, and single-purpose. **No monolithic functions**
  (a function must not do everything: read input, move, collide, and render at once).
- Prefer several small helpers over one large function.
- A function that becomes hard to read should be split into named helpers.

## Comments

- Comment *why*, not what - the code should already show what it does.
- Every public function gets a short doc comment in the header.
- Keep comments brief and to the point.

## Simplicity

- Keep things simple and direct. Avoid clever one-liners and unnecessary abstraction.
- Use existing helpers (e.g. `physics_*`) instead of duplicating math.
- No `/* TODO: ... */` or unfinished code may be committed.

## Tests

- Every non-trivial function needs unit tests.
- Put tests in `tests/`, registered in `CMakeLists.txt` and runnable via `ctest`.
- Pure logic lives in testable modules (e.g. `core/physics.c`, `world/level.c`),
  separate from raylib input/render code.
- All tests must pass before merging (`ctest --test-dir build`).

## Maintainability & scalability

- Code must be maintainable, scalable, and optimal where it matters:
  - no `O(n^2)` tile scans in hot loops without justification,
  - fixed arrays sized from `constants.h`,
  - deterministic and side-effect-free helper functions where possible.
- Build cleanly with `-Wall -Wextra` and no warnings.

## Style

- C99, standard library only, raylib for IO.
- Braces on the same line as the control statement (K&R style).
- `snake_case` for functions and variables, `UPPER_SNAKE` for constants.
- Order helpers top-down: static helpers above the public functions that use them.