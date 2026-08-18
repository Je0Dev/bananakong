#include "test_util.h"
#include "level.h"
#include "physics.h"
#include "constants.h"
#include <string.h>

static const int GOAL_COL = 3;
static const int GOAL_ROW = 6;

static void test_row_lengths(void) {
    for (int row = 0; row < GRID_ROWS; row++) {
        const char *r = level_row(row);
        CHECK(r != NULL);
        CHECK((int)strlen(r) == GRID_COLS);
    }
}

static void test_bounds_safe(void) {
    CHECK(level_tile_at(-1, 0) == '.');
    CHECK(level_tile_at(0, -1) == '.');
    CHECK(level_tile_at(GRID_COLS, 0) == '.');
    CHECK(level_tile_at(0, GRID_ROWS) == '.');
    CHECK(level_row(-1) == NULL);
    CHECK(level_row(GRID_ROWS) == NULL);
    CHECK(!level_is_solid(-1, 0));
    CHECK(!level_is_ladder(GRID_COLS, 0));
}

static void test_platforms(void) {
    CHECK(level_is_solid(0, 6) && level_is_solid(25, 6));
    CHECK(!level_is_solid(26, 6));
    CHECK(level_is_solid(0, 20) && level_is_solid(33, 20));
    CHECK(!level_is_solid(34, 20));
    CHECK(level_is_solid(0, 29) && level_is_solid(39, 29));
    CHECK(level_is_solid(20, 29));
}

static void test_goal(void) {
    CHECK(level_is_goal(GOAL_COL, GOAL_ROW));
    CHECK(!level_is_goal(0, GOAL_ROW));
    CHECK(level_is_solid(GOAL_COL, GOAL_ROW));
}

static void test_ladder_shafts(void) {
    CHECK(level_is_ladder(4, 7) && level_is_ladder(4, 19));
    CHECK(level_is_ladder(22, 7) && level_is_ladder(22, 19));
    CHECK(level_is_ladder(16, 21) && level_is_ladder(16, 27));
    CHECK(level_is_ladder(30, 21) && level_is_ladder(30, 27));
    CHECK(!level_is_ladder(4, 6));
    CHECK(!level_is_ladder(5, 7));
    CHECK(!level_is_solid(4, 7));
}

static void test_no_floating_ladders(void) {
    /* The tile above each ladder top and below each ladder bottom is solid. */
    for (int col = 0; col < GRID_COLS; col++) {
        for (int row = 0; row < GRID_ROWS; row++) {
            if (!level_is_ladder(col, row)) continue;
            if (!level_is_ladder(col, row - 1)) CHECK(level_is_solid(col, row - 1));
            if (!level_is_ladder(col, row + 1)) CHECK(level_is_solid(col, row + 1));
        }
    }
}

static bool bfs_reaches_goal(int start_col, int start_row) {
    static bool seen[GRID_ROWS][GRID_COLS];
    static int queue[GRID_ROWS * GRID_COLS][2];
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++) seen[r][c] = false;

    int head = 0, tail = 0;
    seen[start_row][start_col] = true;
    queue[tail][0] = start_row;
    queue[tail][1] = start_col;
    tail++;

    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };
    while (head < tail) {
        int r = queue[head][0], c = queue[head][1];
        head++;
        if (r == GOAL_ROW && c == GOAL_COL) return true;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr < 0 || nr >= GRID_ROWS || nc < 0 || nc >= GRID_COLS) continue;
            if (seen[nr][nc] || level_tile_at(nc, nr) == '.') continue;
            seen[nr][nc] = true;
            queue[tail][0] = nr;
            queue[tail][1] = nc;
            tail++;
        }
    }
    return false;
}

static void test_spawn_on_ground(void) {
    Level level;
    level_init(&level);
    int col = physics_tile_col(level.spawn.x, TILE_SIZE);
    int row = physics_tile_row(level.spawn.y + PLAYER_HEIGHT, TILE_SIZE);
    CHECK(level_is_solid(col, row));
    CHECK(bfs_reaches_goal(col, row));
}

void test_level(void) {
    test_row_lengths();
    test_bounds_safe();
    test_platforms();
    test_goal();
    test_ladder_shafts();
    test_no_floating_ladders();
    test_spawn_on_ground();
}