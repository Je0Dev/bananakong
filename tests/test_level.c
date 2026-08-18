#include "test_util.h"
#include "level.h"
#include "level_gen.h"
#include "physics.h"
#include "constants.h"
#include <string.h>

static Level g_level; /* level_init target; the grid itself lives in level.c */

static void test_grid_basics(void) {
    for (int row = 0; row < GRID_ROWS; row++) {
        CHECK(level_row(row) != NULL);
        CHECK((int)strlen(level_row(row)) == GRID_COLS);
    }
    CHECK(level_tile_at(-1, 0) == '.' && level_tile_at(GRID_COLS, 0) == '.');
    CHECK(level_row(-1) == NULL && level_row(GRID_ROWS) == NULL);
    CHECK(!level_is_solid(-1, 0) && !level_is_ladder(GRID_COLS, 0));
    int goals = 0;
    for (int col = 0; col < GRID_COLS; col++)
        if (level_is_goal(col, KONG_PLATFORM_ROW)) goals++;
    CHECK(goals == 1);
    CHECK(level_is_solid(0, GRID_ROWS - 1) && level_is_solid(GRID_COLS - 1, GRID_ROWS - 1));
}

static void test_no_floating_ladders(void) {
    for (int col = 0; col < GRID_COLS; col++) {
        for (int row = 0; row < GRID_ROWS; row++) {
            if (!level_is_ladder(col, row)) continue;
            if (!level_is_ladder(col, row - 1)) CHECK(level_is_solid(col, row - 1));
            if (!level_is_ladder(col, row + 1)) CHECK(level_is_solid(col, row + 1));
        }
    }
}

static bool bfs_reaches(int sc, int sr, int gc, int gr) {
    static bool seen[GRID_ROWS][GRID_COLS];
    static int q[GRID_ROWS * GRID_COLS][2];
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++) seen[r][c] = false;
    int head = 0, tail = 0;
    seen[sr][sc] = true;
    q[tail][0] = sr; q[tail][1] = sc; tail++;
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };
    while (head < tail) {
        int r = q[head][0], c = q[head][1];
        head++;
        if (r == gr && c == gc) return true;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr < 0 || nr >= GRID_ROWS || nc < 0 || nc >= GRID_COLS) continue;
            if (seen[nr][nc] || level_tile_at(nc, nr) == '.') continue;
            seen[nr][nc] = true;
            q[tail][0] = nr; q[tail][1] = nc; tail++;
        }
    }
    return false;
}

static int goal_col(void) {
    for (int c = 0; c < GRID_COLS; c++)
        if (level_is_goal(c, KONG_PLATFORM_ROW)) return c;
    return -1;
}

static void test_level_layout(unsigned seed, int level_index) {
    Level level;
    level_init(&level, level_index, seed);
    int gc = goal_col();
    CHECK(gc >= 0);
    CHECK(physics_tile_col(level.spawn_left.x, TILE_SIZE) !=
          physics_tile_col(level.spawn_right.x, TILE_SIZE));
    const Vector2 *spawns[2] = { &level.spawn_left, &level.spawn_right };
    for (int i = 0; i < 2; i++) {
        int c = physics_tile_col(spawns[i]->x, TILE_SIZE);
        int r = physics_tile_row(spawns[i]->y + PLAYER_HEIGHT, TILE_SIZE);
        CHECK(level_is_solid(c, r));
        CHECK(bfs_reaches(c, r, gc, KONG_PLATFORM_ROW));
    }
}

static void test_deterministic_and_progression(void) {
    level_init(&g_level, 3, 77);
    const char *before[GRID_ROWS];
    for (int r = 0; r < GRID_ROWS; r++) before[r] = level_row(r);
    level_init(&g_level, 3, 77);
    for (int r = 0; r < GRID_ROWS; r++) CHECK(strcmp(before[r], level_row(r)) == 0);
    CHECK(level_gen_floors(1) == 1 && level_gen_floors(3) == 3);
    CHECK(level_gen_floors(50) == 4 && level_gen_next_seed(42) != 42);
}

void test_level(void) {
    static const unsigned seeds[] = { 1, 7, 42, 1234, 999999, 0x9E3779B9 };
    level_init(&g_level, 1, 42);
    test_grid_basics();
    test_no_floating_ladders();
    for (size_t s = 0; s < sizeof(seeds) / sizeof(seeds[0]); s++) {
        for (int lvl = 1; lvl <= 5; lvl++) {
            level_init(&g_level, lvl, seeds[s]);
            test_grid_basics();
            test_no_floating_ladders();
            test_level_layout(seeds[s], lvl);
        }
    }
    test_deterministic_and_progression();
}