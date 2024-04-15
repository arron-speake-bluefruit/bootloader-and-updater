#include <stdio.h>
#include <stdlib.h>

void test_assert_fail(const char* file, const char* func, unsigned line, const char* message) {
    fprintf(stderr, "%s:%i in %s: %s\n", file, line, func, message);
    abort();
}
