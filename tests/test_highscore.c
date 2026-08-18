#include <unistd.h>
#include "test_util.h"
#include "highscore.h"

static const char *PATH = "test_highscore.tmp";

static void test_empty_file_loads_zero(void) {
    unlink(PATH);
    CHECK(highscore_load(PATH) == 0);
}

static void test_save_then_load(void) {
    highscore_save(PATH, 100);
    CHECK(highscore_load(PATH) == 100);
}

static void test_update_keeps_best(void) {
    CHECK(highscore_update(PATH, 100) == 100);
    CHECK(highscore_update(PATH, 50) == 100);
    CHECK(highscore_update(PATH, 200) == 200);
    CHECK(highscore_load(PATH) == 200);
}

static void test_update_lower_does_not_write(void) {
    unlink(PATH);
    CHECK(highscore_update(PATH, 300) == 300);
    CHECK(highscore_update(PATH, 150) == 300);
    CHECK(highscore_load(PATH) == 300);
}

void test_highscore(void) {
    test_empty_file_loads_zero();
    test_save_then_load();
    test_update_keeps_best();
    test_update_lower_does_not_write();
    unlink(PATH);
}