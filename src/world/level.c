#include "level.h"
#include "physics.h"
#include "level_gen.h"
#include <stddef.h>

/* Current generated grid; rebuilt by level_init each time. */
static char grid[GRID_ROWS][GRID_COLS + 1];

char level_tile_at(int col, int row) {
    if (col < 0 || col >= GRID_COLS || row < 0 || row >= GRID_ROWS) return '.';
    return grid[row][col];
}

const char *level_row(int row) {
    if (row < 0 || row >= GRID_ROWS) return NULL;
    return grid[row];
}

static int level_goal_col(void) {
    for (int col = 0; col < GRID_COLS; col++) {
        if (grid[KONG_PLATFORM_ROW][col] == 'G') return col;
    }
    return 3;
}

void level_init(Level *level, int level_index, unsigned seed) {
    level_gen_fill(seed, level_index, grid);
    level->spawn_left = (Vector2){ 5.0f * TILE_SIZE, (GRID_ROWS - 2) * TILE_SIZE - PLAYER_HEIGHT };
    level->spawn_right = (Vector2){ 34.0f * TILE_SIZE, (GRID_ROWS - 2) * TILE_SIZE - PLAYER_HEIGHT };
    level->goal = (Vector2){ (float)(level_goal_col() * TILE_SIZE), (float)(KONG_PLATFORM_ROW * TILE_SIZE) };
}

bool level_is_solid(int col, int row) {
    char t = level_tile_at(col, row);
    return t == '#' || t == 'G';
}

bool level_is_ladder(int col, int row) {
    return level_tile_at(col, row) == 'L';
}

bool level_is_goal(int col, int row) {
    return level_tile_at(col, row) == 'G';
}

static bool rect_hits(int col0, int col1, int row0, int row1, bool (*test)(int, int)) {
    for (int row = row0; row <= row1; row++) {
        for (int col = col0; col <= col1; col++) {
            if (test(col, row)) return true;
        }
    }
    return false;
}

bool level_solid_at_rect(Rectangle r) {
    int c0 = physics_tile_col(r.x, TILE_SIZE);
    int c1 = physics_tile_col(r.x + r.width - 0.01f, TILE_SIZE);
    int r0 = physics_tile_row(r.y, TILE_SIZE);
    int r1 = physics_tile_row(r.y + r.height - 0.01f, TILE_SIZE);
    return rect_hits(c0, c1, r0, r1, level_is_solid);
}

bool level_ladder_at_rect(Rectangle r) {
    int c0 = physics_tile_col(r.x, TILE_SIZE);
    int c1 = physics_tile_col(r.x + r.width - 0.01f, TILE_SIZE);
    int r0 = physics_tile_row(r.y, TILE_SIZE);
    int r1 = physics_tile_row(r.y + r.height - 0.01f, TILE_SIZE);
    return rect_hits(c0, c1, r0, r1, level_is_ladder);
}