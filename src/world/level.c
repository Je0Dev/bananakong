#include "level.h"
#include "physics.h"
#include <stddef.h>

/* Level grid. Each row is GRID_COLS chars.
 * '#' platform (solid), 'L' ladder (climbable), 'G' goal, '.' empty.
 * Every ladder shaft runs between two solid rows so no stair floats. */
static const char TILES[GRID_ROWS][GRID_COLS + 1] = {
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "........................................",
    "###G######################..............",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "....L.................L.................",
    "##################################......",
    "................L.............L.........",
    "................L.............L.........",
    "................L.............L.........",
    "................L.............L.........",
    "................L.............L.........",
    "................L.............L.........",
    "................L.............L.........",
    "########################################",
    "########################################",
};

static const Vector2 SPAWN_LEFT = { 5.0f * TILE_SIZE, (GRID_ROWS - 2) * TILE_SIZE - PLAYER_HEIGHT };
static const Vector2 SPAWN_RIGHT = { 34.0f * TILE_SIZE, (GRID_ROWS - 2) * TILE_SIZE - PLAYER_HEIGHT };
static const Vector2 GOAL = { 3.0f * TILE_SIZE, 6.0f * TILE_SIZE };

char level_tile_at(int col, int row) {
    if (col < 0 || col >= GRID_COLS || row < 0 || row >= GRID_ROWS) return '.';
    return TILES[row][col];
}

const char *level_row(int row) {
    if (row < 0 || row >= GRID_ROWS) return NULL;
    return TILES[row];
}

void level_init(Level *level) {
    level->spawn_left = SPAWN_LEFT;
    level->spawn_right = SPAWN_RIGHT;
    level->goal = GOAL;
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