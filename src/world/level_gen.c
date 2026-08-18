#include "level_gen.h"

/* xorshift32: tiny deterministic PRNG so generation reproduces exactly in
 * tests without touching raylib's global RNG. A zero seed never advances, so
 * it is replaced on entry. */
static unsigned xorshift32(unsigned *s) {
    unsigned x = *s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return *s = x;
}

int level_gen_floors(int level_index) {
    int n = level_index;
    if (n < 1) n = 1;
    if (n > 4) n = 4;
    return n;
}

unsigned level_gen_next_seed(unsigned seed) {
    return xorshift32(&seed);
}

void level_gen_fill(unsigned seed, int level_index, char grid[GRID_ROWS][GRID_COLS + 1]) {
    unsigned rng = (seed == 0) ? 0x9E3779B9u : seed;

    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) grid[row][col] = '.';
        grid[row][GRID_COLS] = '\0';
    }

    /* Ground spans the full width. */
    for (int col = 0; col < GRID_COLS; col++) {
        grid[GRID_ROWS - 2][col] = '#';
        grid[GRID_ROWS - 1][col] = '#';
    }

    /* Kong's platform at the top, with the goal tile somewhere on it. */
    const int kong_right = 25;
    for (int col = 0; col <= kong_right; col++) grid[6][col] = '#';
    int goal_col = 1 + (int)(xorshift32(&rng) % 24);
    grid[6][goal_col] = 'G';

    /* Intermediate platforms stay anchored at col 0 and widen as they descend,
     * so every platform overlaps the one above and a ladder can bridge them. */
    int cap = 33 + (int)(xorshift32(&rng) % 7); /* right-edge cap 33..39 */
    int step = (cap - kong_right - 1) / level_gen_floors(level_index);
    if (step < 2) step = 2;

    int prev_row = 6;
    int floors = level_gen_floors(level_index);
    for (int i = 1; i <= floors; i++) {
        int row = 6 + (22 * i) / (floors + 1);
        int right = kong_right + 1 + i * step;
        if (right > GRID_COLS - 1) right = GRID_COLS - 1;
        for (int col = 0; col <= right; col++) grid[row][col] = '#';

        int ladder_col = ((i % 2) == 1) ? 20 + (int)(xorshift32(&rng) % 4)
                                        : 2 + (int)(xorshift32(&rng) % 3);
        if (ladder_col == goal_col) ladder_col = (ladder_col > 12) ? ladder_col - 1 : ladder_col + 1;
        for (int r = prev_row + 1; r < row; r++) grid[r][ladder_col] = 'L';
        prev_row = row;
    }

    /* Final ladder from the last platform down to the ground. */
    int last_ladder = ((floors % 2) == 1) ? 20 + (int)(xorshift32(&rng) % 4)
                                          : 2 + (int)(xorshift32(&rng) % 3);
    if (last_ladder == goal_col) last_ladder = (last_ladder > 12) ? last_ladder - 1 : last_ladder + 1;
    for (int r = prev_row + 1; r < GRID_ROWS - 2; r++) grid[r][last_ladder] = 'L';
}