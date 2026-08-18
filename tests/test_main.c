#include <stdlib.h>
#include "test_util.h"

void test_physics(void);
void test_level(void);
void test_difficulty(void);
void test_highscore(void);
void test_popup(void);

int g_checks = 0;
int g_failures = 0;

int main(void) {
    RUN_SUITE(test_physics);
    RUN_SUITE(test_level);
    RUN_SUITE(test_difficulty);
    RUN_SUITE(test_highscore);
    RUN_SUITE(test_popup);
    TEST_REPORT();
    return g_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}