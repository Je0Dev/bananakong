#include <stdio.h>
#include "highscore.h"

int highscore_load(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    int best = 0;
    if (fscanf(f, "%d", &best) != 1) best = 0;
    fclose(f);
    return best;
}

void highscore_save(const char *path, int score) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    fprintf(f, "%d\n", score);
    fclose(f);
}

int highscore_update(const char *path, int score) {
    int best = highscore_load(path);
    if (score > best) {
        best = score;
        highscore_save(path, best);
    }
    return best;
}