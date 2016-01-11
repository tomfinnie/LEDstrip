#ifndef TESTICKLE_H
#define	TESTICKLE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TEST_FIXTURE(number_of_tests) \
    do { \
        printf("TAP version 13\n"); \
        printf("1..%d\n", number_of_tests); \
    } while(0);

#define RUN_TEST(test_name, test_number) \
    do { \
        struct test_result result; \
        setup(); \
        result = test_name(); \
        if (result.test_passed) { \
            printf("ok %d - %s\n", test_number, #test_name); \
        } \
        else { \
            printf("not ok %d - %s\n", test_number, #test_name); \
            printf("  ---\n"); \
            printf("  message: %s\n", result.failure_message); \
            printf("  severity: fail\n"); \
            if (result.data_specified) { \
                printf("  data:\n"); \
                printf("    got: '%s'\n", result.got); \
                printf("    expect: '%s'\n", result.expect); \
            } \
            printf("  ...\n"); \
        } \
        teardown(); \
    } while(0);

#define TEST(test_name) \
    struct test_result test_name() \
    { \
        struct test_result result = {.test_passed = false, .data_specified = false};

#define END_TEST \
        result.test_passed = true; \
        return result; \
    }

#define ASSERT_TRUE(test, message) \
    if (!(test)) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        return result; \
    }

#define ASSERT_FALSE(test, message) \
    if (test) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        return result; \
    }

#define ASSERT_EQUAL(actual, expected, type, message) \
    if (actual != expected) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        result.data_specified = true; \
        snprintf(result.got, MESSAGE_LENGTH, type, actual); \
        snprintf(result.expect, MESSAGE_LENGTH, type, expected); \
        return result; \
    }

#define ASSERT_NOT_EQUAL(actual, expected, message) \
    if (actual == expected) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        return result; \
    }

#define ASSERT_STRING_EQUAL(actual, expected, message) \
    if (strncmp(actual, expected, MESSAGE_LENGTH) != 0) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        result.data_specified = true; \
        snprintf(result.got, MESSAGE_LENGTH, "%s", actual); \
        snprintf(result.expect, MESSAGE_LENGTH, "%s", expected); \
        return result; \
    }

#define ASSERT_STRING_EMPTY(actual, message) \
    if (strncmp(actual, "", MESSAGE_LENGTH) != 0) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        result.data_specified = true; \
        snprintf(result.got, MESSAGE_LENGTH, "%s", actual); \
        return result; \
    }

#define ASSERT_STRING_NOT_EMPTY(actual, message) \
    if (strncmp(actual, "", MESSAGE_LENGTH) == 0) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        return result; \
    }

#define ASSERT_NULL(expr, message) \
    if ((expr) != NULL) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        return result; \
    }

#define ASSERT_NOT_NULL(expr, message) \
    if ((expr) == NULL) { \
        snprintf(result.failure_message, MESSAGE_LENGTH, "%s", message); \
        return result; \
    }

#define MESSAGE_LENGTH 128

struct test_result {
    bool test_passed;
    char failure_message[MESSAGE_LENGTH];
    bool data_specified;
    char got[MESSAGE_LENGTH];
    char expect[MESSAGE_LENGTH];
};

void setup();
void teardown();

#endif	/* TESTICKLE_H */
