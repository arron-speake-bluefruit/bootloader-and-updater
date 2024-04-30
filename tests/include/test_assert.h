#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include <string.h>

void test_assert_fail(const char* file, const char* func, unsigned line, const char* message);

#define ASSERT_EQUAL(a, b) do { \
        if ((a) != (b)) { test_assert_fail(__FILE__, __func__, __LINE__, "assertion " #a " == " #b " failed\n"); } \
    } while (0)

#define ASSERT_LESS_THAN(a, b) do { \
        if ((a) >= (b)) { test_assert_fail(__FILE__, __func__, __LINE__, "assertion " #a " < " #b " failed\n"); } \
    } while (0)

#define ASSERT_STRINGS_EQUAL(a, b) do { \
        if (strcmp((a), (b)) != 0) { test_assert_fail(__FILE__, __func__, __LINE__, "assertion (strings) " #a " == " #b " failed\n"); } \
    } while (0)

#define ASSERT_FALSE(a) do { \
        if ((a)) { test_assert_fail(__FILE__, __func__, __LINE__, "assertion that " #a " was false failed\n"); } \
    } while (0)

#define ASSERT_TRUE(a) do { \
        if (!(a)) { test_assert_fail(__FILE__, __func__, __LINE__, "assertion that " #a " was true failed\n"); } \
    } while (0)

#define ASSERT_EQUAL_MEMORY(a, b, size) do { \
        if (memcmp((a), (b), (size)) != 0) { test_assert_fail(__FILE__, __func__, __LINE__, "assertion (memory) " #a " == " #b " failed\n"); } \
    } while (0)

#endif // TEST_ASSERT_H
