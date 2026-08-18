#ifndef HIGHSCORE_H
#define HIGHSCORE_H

/* Read the best score from the given file; 0 when missing/unreadable. */
int highscore_load(const char *path);

/* Overwrite the given file with a best score. */
void highscore_save(const char *path, int score);

/* Keep the larger of the stored best and score, persisting if improved.
 * Returns the resulting best. Pure stdio, no raylib. */
int highscore_update(const char *path, int score);

#endif