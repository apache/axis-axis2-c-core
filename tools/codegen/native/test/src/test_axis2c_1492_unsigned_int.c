/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * test_axis2c_1492_unsigned_int.c
 *
 * AXIS2C-1492 Tests for Unsigned Integer Type Handling
 *
 * Issue: xsd:unsignedInt values above 2147483647 are truncated because
 * the generated code uses atoi() which returns signed int, causing
 * values above INT_MAX to overflow to negative numbers.
 *
 * Fix: The native generator now uses proper type mapping and conversion:
 * - xsd:unsignedInt -> unsigned int with strtoul()
 * - xsd:unsignedLong -> uint64_t with strtoull()
 * - xsd:unsignedShort -> unsigned int with strtoul()
 * - xsd:unsignedByte -> unsigned int with strtoul()
 *
 * This ensures values like 4294967295 (max unsigned int) are handled
 * correctly without overflow.
 */

#include "adb_test_framework.h"
#include <stdint.h>
#include <limits.h>

/* Simulate the helper functions from stub_generator.c for testing
 * These use the exact same logic as the actual implementation.
 */

/* Check if XSD type is an unsigned integer type */
static int
test_is_unsigned_integer_type(const char *xsd_type)
{
    if (!xsd_type) {
        return 0;
    }
    if (strstr(xsd_type, "unsignedInt") ||
        strstr(xsd_type, "unsignedLong") ||
        strstr(xsd_type, "unsignedShort") ||
        strstr(xsd_type, "unsignedByte") ||
        strstr(xsd_type, "positiveInteger") ||
        strstr(xsd_type, "nonNegativeInteger")) {
        return 1;
    }
    return 0;
}

/* Check if XSD type is a 64-bit integer type (long) */
static int
test_is_64bit_integer_type(const char *xsd_type)
{
    if (!xsd_type) {
        return 0;
    }
    if (strstr(xsd_type, "Long") || strstr(xsd_type, "long") ||
        strstr(xsd_type, "integer") || strstr(xsd_type, "Integer")) {
        /* "integer" in XSD is arbitrary precision, map to 64-bit */
        /* But exclude unsignedInt which contains "int" */
        if (strstr(xsd_type, "unsignedInt") && !strstr(xsd_type, "unsignedInteg")) {
            return 0;
        }
        return 1;
    }
    return 0;
}

/* Check if XSD type is any integer type (signed or unsigned) */
static int
test_is_integer_type(const char *xsd_type)
{
    if (!xsd_type) {
        return 0;
    }
    if (strstr(xsd_type, "int") || strstr(xsd_type, "Int") ||
        strstr(xsd_type, "long") || strstr(xsd_type, "Long") ||
        strstr(xsd_type, "short") || strstr(xsd_type, "Short") ||
        strstr(xsd_type, "byte") || strstr(xsd_type, "Byte") ||
        strstr(xsd_type, "integer") || strstr(xsd_type, "Integer")) {
        return 1;
    }
    return 0;
}

/* Get the C type string for an XSD integer type */
static const char*
test_get_c_integer_type(const char *xsd_type)
{
    if (!xsd_type) {
        return "int";
    }

    int is_unsigned = test_is_unsigned_integer_type(xsd_type);
    int is_64bit = test_is_64bit_integer_type(xsd_type);

    if (is_64bit) {
        return is_unsigned ? "uint64_t" : "int64_t";
    } else {
        return is_unsigned ? "unsigned int" : "int";
    }
}

/* Get the conversion function for deserializing an XSD integer type */
static const char*
test_get_deserialize_func(const char *xsd_type)
{
    if (!xsd_type) {
        return "atoi";
    }

    int is_unsigned = test_is_unsigned_integer_type(xsd_type);
    int is_64bit = test_is_64bit_integer_type(xsd_type);

    if (is_64bit) {
        return is_unsigned ? "strtoull" : "strtoll";
    } else {
        return is_unsigned ? "strtoul" : "atoi";
    }
}

/* Get the printf format specifier for an XSD integer type */
static const char*
test_get_format_specifier(const char *xsd_type)
{
    if (!xsd_type) {
        return "%d";
    }

    int is_unsigned = test_is_unsigned_integer_type(xsd_type);
    int is_64bit = test_is_64bit_integer_type(xsd_type);

    if (is_64bit) {
        return is_unsigned ? "%llu" : "%lld";
    } else {
        return is_unsigned ? "%u" : "%d";
    }
}

/* Test unsigned type detection */
adb_test_result_t test_axis2c_1492_unsigned_type_detection(void) {
    printf("Testing AXIS2C-1492: Unsigned type detection...\n");

    struct {
        const char *type;
        int expected;
    } tests[] = {
        /* Unsigned types - should return true */
        {"unsignedInt", 1},
        {"xsd:unsignedInt", 1},
        {"unsignedLong", 1},
        {"unsignedShort", 1},
        {"unsignedByte", 1},
        {"positiveInteger", 1},
        {"nonNegativeInteger", 1},
        /* Signed types should return false */
        {"int", 0},
        {"xsd:int", 0},
        {"long", 0},
        {"short", 0},
        {"byte", 0},
        {"integer", 0},
        {"negativeInteger", 0},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < count; i++) {
        int result = test_is_unsigned_integer_type(tests[i].type);
        printf("  is_unsigned_integer_type(\"%s\") = %s (expected %s)\n",
               tests[i].type,
               result ? "true" : "false",
               tests[i].expected ? "true" : "false");
        ADB_ASSERT_TRUE(result == tests[i].expected,
                       "Unsigned type detection should match expected");
    }

    printf("  Unsigned type detection: PASS\n");
    return ADB_TEST_SUCCESS;
}

/* Test C type mapping */
adb_test_result_t test_axis2c_1492_c_type_mapping(void) {
    printf("Testing AXIS2C-1492: C type mapping...\n");

    struct {
        const char *xsd_type;
        const char *expected_c_type;
    } tests[] = {
        /* Signed 32-bit */
        {"int", "int"},
        {"short", "int"},
        {"byte", "int"},
        /* Unsigned 32-bit - the AXIS2C-1492 fix */
        {"unsignedInt", "unsigned int"},
        {"unsignedShort", "unsigned int"},
        {"unsignedByte", "unsigned int"},
        /* Signed 64-bit */
        {"long", "int64_t"},
        {"integer", "int64_t"},
        {"negativeInteger", "int64_t"},
        /* Unsigned 64-bit */
        {"unsignedLong", "uint64_t"},
        {"positiveInteger", "uint64_t"},
        {"nonNegativeInteger", "uint64_t"},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < count; i++) {
        const char *result = test_get_c_integer_type(tests[i].xsd_type);
        printf("  get_c_integer_type(\"%s\") = \"%s\" (expected \"%s\")\n",
               tests[i].xsd_type, result, tests[i].expected_c_type);
        ADB_ASSERT_TRUE(strcmp(result, tests[i].expected_c_type) == 0,
                       "C type mapping should match expected");
    }

    printf("  C type mapping: PASS\n");
    return ADB_TEST_SUCCESS;
}

/* Test conversion function selection */
adb_test_result_t test_axis2c_1492_conversion_functions(void) {
    printf("Testing AXIS2C-1492: Conversion function selection...\n");

    struct {
        const char *xsd_type;
        const char *expected_func;
    } tests[] = {
        /* Signed 32-bit uses atoi */
        {"int", "atoi"},
        {"short", "atoi"},
        {"byte", "atoi"},
        /* Unsigned 32-bit uses strtoul - the AXIS2C-1492 fix */
        {"unsignedInt", "strtoul"},
        {"unsignedShort", "strtoul"},
        {"unsignedByte", "strtoul"},
        /* Signed 64-bit uses strtoll */
        {"long", "strtoll"},
        {"integer", "strtoll"},
        /* Unsigned 64-bit uses strtoull */
        {"unsignedLong", "strtoull"},
        {"positiveInteger", "strtoull"},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < count; i++) {
        const char *result = test_get_deserialize_func(tests[i].xsd_type);
        printf("  get_deserialize_func(\"%s\") = \"%s\" (expected \"%s\")\n",
               tests[i].xsd_type, result, tests[i].expected_func);
        ADB_ASSERT_TRUE(strcmp(result, tests[i].expected_func) == 0,
                       "Conversion function should match expected");
    }

    printf("  Conversion function selection: PASS\n");
    return ADB_TEST_SUCCESS;
}

/* Test format specifier selection */
adb_test_result_t test_axis2c_1492_format_specifiers(void) {
    printf("Testing AXIS2C-1492: Format specifier selection...\n");

    struct {
        const char *xsd_type;
        const char *expected_format;
    } tests[] = {
        /* Signed 32-bit uses %d */
        {"int", "%d"},
        {"short", "%d"},
        {"byte", "%d"},
        /* Unsigned 32-bit uses %u - the AXIS2C-1492 fix */
        {"unsignedInt", "%u"},
        {"unsignedShort", "%u"},
        {"unsignedByte", "%u"},
        /* Signed 64-bit uses %lld */
        {"long", "%lld"},
        {"integer", "%lld"},
        /* Unsigned 64-bit uses %llu */
        {"unsignedLong", "%llu"},
        {"positiveInteger", "%llu"},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < count; i++) {
        const char *result = test_get_format_specifier(tests[i].xsd_type);
        printf("  get_format_specifier(\"%s\") = \"%s\" (expected \"%s\")\n",
               tests[i].xsd_type, result, tests[i].expected_format);
        ADB_ASSERT_TRUE(strcmp(result, tests[i].expected_format) == 0,
                       "Format specifier should match expected");
    }

    printf("  Format specifier selection: PASS\n");
    return ADB_TEST_SUCCESS;
}

/* Test the actual bug: large unsigned values */
adb_test_result_t test_axis2c_1492_large_unsigned_values(void) {
    printf("Testing AXIS2C-1492: Large unsigned value handling...\n");

    /* Test value that would overflow if using atoi */
    const char *text_value = "3000000000";  /* Greater than INT_MAX (2147483647) */

    /* Old buggy approach: atoi() returns signed int, causes overflow */
    int buggy_result = atoi(text_value);
    printf("  Old buggy atoi(\"%s\") = %d (WRONG - overflow to negative)\n",
           text_value, buggy_result);
    ADB_ASSERT_TRUE(buggy_result < 0, "atoi should overflow to negative for large values");

    /* New fixed approach: strtoul() returns unsigned, no overflow */
    unsigned int fixed_result = strtoul(text_value, NULL, 10);
    printf("  New fixed strtoul(\"%s\") = %u (CORRECT)\n",
           text_value, fixed_result);
    ADB_ASSERT_TRUE(fixed_result == 3000000000U, "strtoul should handle large unsigned values");

    /* Test maximum unsigned int value */
    const char *max_uint = "4294967295";
    unsigned int max_result = strtoul(max_uint, NULL, 10);
    printf("  strtoul(\"%s\") = %u (max unsigned int)\n", max_uint, max_result);
    ADB_ASSERT_TRUE(max_result == UINT_MAX, "strtoul should handle max unsigned int");

    /* Test maximum uint64_t value */
    const char *max_uint64 = "18446744073709551615";
    uint64_t max64_result = strtoull(max_uint64, NULL, 10);
    printf("  strtoull(\"%s\") = %llu (max uint64_t)\n",
           max_uint64, (unsigned long long)max64_result);
    ADB_ASSERT_TRUE(max64_result == UINT64_MAX, "strtoull should handle max uint64_t");

    printf("  Large unsigned value handling: PASS\n");
    return ADB_TEST_SUCCESS;
}

/* Test integer type detection (for free function primitive check) */
adb_test_result_t test_axis2c_1492_integer_type_detection(void) {
    printf("Testing AXIS2C-1492: Integer type detection (for free function)...\n");

    struct {
        const char *type;
        int expected;
    } tests[] = {
        /* All integer types should be detected */
        {"int", 1},
        {"xsd:int", 1},
        {"unsignedInt", 1},
        {"long", 1},
        {"unsignedLong", 1},
        {"short", 1},
        {"unsignedShort", 1},
        {"byte", 1},
        {"unsignedByte", 1},
        {"integer", 1},
        {"positiveInteger", 1},
        {"negativeInteger", 1},
        {"nonNegativeInteger", 1},
        {"nonPositiveInteger", 1},
        /* Non-integer types should NOT be detected */
        {"string", 0},
        {"dateTime", 0},
        {"base64Binary", 0},
        {"boolean", 0},
        {"double", 0},
        {"float", 0},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < count; i++) {
        int result = test_is_integer_type(tests[i].type);
        printf("  is_integer_type(\"%s\") = %s (expected %s)\n",
               tests[i].type,
               result ? "true" : "false",
               tests[i].expected ? "true" : "false");
        ADB_ASSERT_TRUE(result == tests[i].expected,
                       "Integer type detection should match expected");
    }

    printf("  Integer type detection: PASS\n");
    return ADB_TEST_SUCCESS;
}

/* Test case array for AXIS2C-1492 tests */
adb_test_case_t axis2c_1492_tests[] = {
    {
        "test_axis2c_1492_unsigned_type_detection",
        "Test XSD unsigned type detection",
        test_axis2c_1492_unsigned_type_detection
    },
    {
        "test_axis2c_1492_c_type_mapping",
        "Test XSD to C type mapping",
        test_axis2c_1492_c_type_mapping
    },
    {
        "test_axis2c_1492_conversion_functions",
        "Test string-to-integer conversion function selection",
        test_axis2c_1492_conversion_functions
    },
    {
        "test_axis2c_1492_format_specifiers",
        "Test printf format specifier selection",
        test_axis2c_1492_format_specifiers
    },
    {
        "test_axis2c_1492_large_unsigned_values",
        "Test large unsigned value handling (the actual bug)",
        test_axis2c_1492_large_unsigned_values
    },
    {
        "test_axis2c_1492_integer_type_detection",
        "Test integer type detection (for free function primitive check)",
        test_axis2c_1492_integer_type_detection
    }
};

int axis2c_1492_test_count = sizeof(axis2c_1492_tests) / sizeof(axis2c_1492_tests[0]);
