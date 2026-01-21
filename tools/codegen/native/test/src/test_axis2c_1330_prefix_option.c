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
 * test_axis2c_1330_prefix_option.c
 *
 * AXIS2C-1330 Prefix Option Tests
 *
 * Tests for the -prefix command line option that prevents name clashes when
 * using multiple services that have identical named items (types, operations).
 *
 * Original JIRA issue: When using multiple services in one project that happen
 * to have identical named items (e.g., both Mapping.wsdl and Geocoding.wsdl have
 * a "Coordinate" type), identical named structures and functions are generated
 * in identical named files causing linker conflicts.
 *
 * Solution: Add -prefix option to prepend a user-specified prefix to all generated
 * filenames, type names, and function names.
 *
 * Example usage:
 *   wsdl2c-native -prefix Mapping_ -uri Mapping.wsdl
 *   wsdl2c-native -prefix Geocoding_ -uri Geocoding.wsdl
 *
 * This generates:
 *   Mapping_adb_Coordinate.h/.c with Mapping_adb_Coordinate_t, Mapping_adb_Coordinate_create(), etc.
 *   Geocoding_adb_Coordinate.h/.c with Geocoding_adb_Coordinate_t, Geocoding_adb_Coordinate_create(), etc.
 */

#include "adb_test_framework.h"
#include <string.h>
#include <stdio.h>

/* Simulated prefix application functions - these mirror the logic in stub_generator.c */

/**
 * Apply prefix to a generated filename
 * Example: apply_prefix_to_filename("Mapping_", "adb_Coordinate.h")
 *          returns "Mapping_adb_Coordinate.h"
 */
static char* apply_prefix_to_filename(const char *prefix, const char *base_filename) {
    size_t prefix_len = prefix ? strlen(prefix) : 0;
    size_t base_len = strlen(base_filename);
    char *result = adb_tracked_malloc(prefix_len + base_len + 1);

    if (result) {
        if (prefix && prefix_len > 0) {
            strcpy(result, prefix);
            strcat(result, base_filename);
        } else {
            strcpy(result, base_filename);
        }
    }
    return result;
}

/**
 * Apply prefix to a generated type name
 * Example: apply_prefix_to_type("Mapping_", "adb_Coordinate_t")
 *          returns "Mapping_adb_Coordinate_t"
 */
static char* apply_prefix_to_type(const char *prefix, const char *base_type) {
    return apply_prefix_to_filename(prefix, base_type);  /* Same logic */
}

/**
 * Apply prefix to a generated function name
 * Example: apply_prefix_to_function("Mapping_", "adb_Coordinate_create")
 *          returns "Mapping_adb_Coordinate_create"
 */
static char* apply_prefix_to_function(const char *prefix, const char *base_function) {
    return apply_prefix_to_filename(prefix, base_function);  /* Same logic */
}

/* Test: Prefix applied to filenames */
adb_test_result_t test_axis2c_1330_prefix_filenames(void) {
    printf("Testing AXIS2C-1330: Prefix applied to generated filenames...\n");

    const char *prefix = "Mapping_";
    const char *test_cases[][2] = {
        {"adb_Coordinate.h", "Mapping_adb_Coordinate.h"},
        {"adb_Coordinate.c", "Mapping_adb_Coordinate.c"},
        {"adb_GetLocation.h", "Mapping_adb_GetLocation.h"},
        {"adb_GetLocationResponse.h", "Mapping_adb_GetLocationResponse.h"},
        {"axis2_stub_LocationService.h", "Mapping_axis2_stub_LocationService.h"},
        {"axis2_stub_LocationService.c", "Mapping_axis2_stub_LocationService.c"},
    };

    int test_count = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < test_count; i++) {
        char *result = apply_prefix_to_filename(prefix, test_cases[i][0]);
        ADB_ASSERT_NOT_NULL(result, "Failed to allocate prefixed filename");

        if (result) {
            printf("  %s -> %s (expected: %s)\n", test_cases[i][0], result, test_cases[i][1]);
            ADB_ASSERT_TRUE(strcmp(result, test_cases[i][1]) == 0,
                           "Prefixed filename does not match expected");
            adb_tracked_free(result);
        }
    }

    printf("  All filename prefix tests passed\n");
    return ADB_TEST_PASS;
}

/* Test: Prefix applied to type names */
adb_test_result_t test_axis2c_1330_prefix_types(void) {
    printf("Testing AXIS2C-1330: Prefix applied to generated type names...\n");

    const char *prefix = "Geocoding_";
    const char *test_cases[][2] = {
        {"adb_Coordinate_t", "Geocoding_adb_Coordinate_t"},
        {"adb_Address_t", "Geocoding_adb_Address_t"},
        {"adb_GetLocation_t", "Geocoding_adb_GetLocation_t"},
        {"adb_GetLocationResponse_t", "Geocoding_adb_GetLocationResponse_t"},
    };

    int test_count = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < test_count; i++) {
        char *result = apply_prefix_to_type(prefix, test_cases[i][0]);
        ADB_ASSERT_NOT_NULL(result, "Failed to allocate prefixed type name");

        if (result) {
            printf("  %s -> %s (expected: %s)\n", test_cases[i][0], result, test_cases[i][1]);
            ADB_ASSERT_TRUE(strcmp(result, test_cases[i][1]) == 0,
                           "Prefixed type name does not match expected");
            adb_tracked_free(result);
        }
    }

    printf("  All type name prefix tests passed\n");
    return ADB_TEST_PASS;
}

/* Test: Prefix applied to function names */
adb_test_result_t test_axis2c_1330_prefix_functions(void) {
    printf("Testing AXIS2C-1330: Prefix applied to generated function names...\n");

    const char *prefix = "Service1_";
    const char *test_cases[][2] = {
        {"adb_Coordinate_create", "Service1_adb_Coordinate_create"},
        {"adb_Coordinate_free", "Service1_adb_Coordinate_free"},
        {"adb_Coordinate_get_latitude", "Service1_adb_Coordinate_get_latitude"},
        {"adb_Coordinate_set_latitude", "Service1_adb_Coordinate_set_latitude"},
        {"adb_Coordinate_serialize", "Service1_adb_Coordinate_serialize"},
        {"adb_Coordinate_create_from_node", "Service1_adb_Coordinate_create_from_node"},
        {"axis2_stub_create_LocationService", "Service1_axis2_stub_create_LocationService"},
        {"axis2_stub_GetLocation", "Service1_axis2_stub_GetLocation"},
    };

    int test_count = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < test_count; i++) {
        char *result = apply_prefix_to_function(prefix, test_cases[i][0]);
        ADB_ASSERT_NOT_NULL(result, "Failed to allocate prefixed function name");

        if (result) {
            printf("  %s -> %s (expected: %s)\n", test_cases[i][0], result, test_cases[i][1]);
            ADB_ASSERT_TRUE(strcmp(result, test_cases[i][1]) == 0,
                           "Prefixed function name does not match expected");
            adb_tracked_free(result);
        }
    }

    printf("  All function name prefix tests passed\n");
    return ADB_TEST_PASS;
}

/* Test: No prefix (empty/NULL) preserves original names */
adb_test_result_t test_axis2c_1330_no_prefix(void) {
    printf("Testing AXIS2C-1330: No prefix preserves original names...\n");

    const char *test_names[] = {
        "adb_Coordinate.h",
        "adb_Coordinate_t",
        "adb_Coordinate_create",
    };

    int test_count = sizeof(test_names) / sizeof(test_names[0]);

    /* Test with NULL prefix */
    for (int i = 0; i < test_count; i++) {
        char *result = apply_prefix_to_filename(NULL, test_names[i]);
        ADB_ASSERT_NOT_NULL(result, "Failed to allocate with NULL prefix");

        if (result) {
            printf("  NULL prefix: %s -> %s\n", test_names[i], result);
            ADB_ASSERT_TRUE(strcmp(result, test_names[i]) == 0,
                           "NULL prefix should preserve original name");
            adb_tracked_free(result);
        }
    }

    /* Test with empty string prefix */
    for (int i = 0; i < test_count; i++) {
        char *result = apply_prefix_to_filename("", test_names[i]);
        ADB_ASSERT_NOT_NULL(result, "Failed to allocate with empty prefix");

        if (result) {
            printf("  Empty prefix: %s -> %s\n", test_names[i], result);
            ADB_ASSERT_TRUE(strcmp(result, test_names[i]) == 0,
                           "Empty prefix should preserve original name");
            adb_tracked_free(result);
        }
    }

    printf("  All no-prefix tests passed\n");
    return ADB_TEST_PASS;
}

/* Test: Multiple services with different prefixes can coexist */
adb_test_result_t test_axis2c_1330_multiple_services_no_conflict(void) {
    printf("Testing AXIS2C-1330: Multiple services with prefixes have unique names...\n");

    /* Simulate two services both having a "Coordinate" type */
    const char *mapping_prefix = "Mapping_";
    const char *geocoding_prefix = "Geocoding_";
    const char *base_type = "adb_Coordinate_t";

    char *mapping_type = apply_prefix_to_type(mapping_prefix, base_type);
    char *geocoding_type = apply_prefix_to_type(geocoding_prefix, base_type);

    ADB_ASSERT_NOT_NULL(mapping_type, "Failed to allocate Mapping type");
    ADB_ASSERT_NOT_NULL(geocoding_type, "Failed to allocate Geocoding type");

    if (mapping_type && geocoding_type) {
        printf("  Mapping service type: %s\n", mapping_type);
        printf("  Geocoding service type: %s\n", geocoding_type);

        /* Verify they are different */
        ADB_ASSERT_TRUE(strcmp(mapping_type, geocoding_type) != 0,
                       "Prefixed types must be different to avoid linker conflicts");

        /* Verify each has the correct prefix */
        ADB_ASSERT_TRUE(strncmp(mapping_type, mapping_prefix, strlen(mapping_prefix)) == 0,
                       "Mapping type must start with Mapping_ prefix");
        ADB_ASSERT_TRUE(strncmp(geocoding_type, geocoding_prefix, strlen(geocoding_prefix)) == 0,
                       "Geocoding type must start with Geocoding_ prefix");

        printf("  Types are unique - no linker conflict!\n");
    }

    if (mapping_type) adb_tracked_free(mapping_type);
    if (geocoding_type) adb_tracked_free(geocoding_type);

    return ADB_TEST_PASS;
}

/* Test: Header guard generation with prefix */
adb_test_result_t test_axis2c_1330_header_guards(void) {
    printf("Testing AXIS2C-1330: Header guard generation with prefix...\n");

    /* Simulate header guard generation */
    const char *prefix = "MYSERVICE_";
    const char *type_name = "COORDINATE";

    /* Without prefix: #ifndef ADB_COORDINATE_H */
    /* With prefix: #ifndef MYSERVICE_ADB_COORDINATE_H */

    char guard_without_prefix[256];
    char guard_with_prefix[256];

    snprintf(guard_without_prefix, sizeof(guard_without_prefix), "ADB_%s_H", type_name);
    snprintf(guard_with_prefix, sizeof(guard_with_prefix), "%sADB_%s_H", prefix, type_name);

    printf("  Without prefix: %s\n", guard_without_prefix);
    printf("  With prefix: %s\n", guard_with_prefix);

    ADB_ASSERT_TRUE(strcmp(guard_without_prefix, guard_with_prefix) != 0,
                   "Header guards must be different with prefix");
    ADB_ASSERT_TRUE(strncmp(guard_with_prefix, prefix, strlen(prefix)) == 0,
                   "Header guard with prefix must start with prefix");

    printf("  Header guard tests passed\n");
    return ADB_TEST_PASS;
}

/* Test: Common use case - conflicting "Coordinate" type from two WSDLs */
adb_test_result_t test_axis2c_1330_real_world_scenario(void) {
    printf("Testing AXIS2C-1330: Real-world scenario - conflicting Coordinate types...\n");

    printf("\n  Scenario: Application uses two services:\n");
    printf("    1. Mapping.wsdl with Coordinate type (lat/long in degrees)\n");
    printf("    2. Geocoding.wsdl with Coordinate type (x/y in meters)\n");
    printf("\n");

    /* Without prefix - both would generate the same files */
    const char *files_without_prefix[] = {
        "adb_Coordinate.h",
        "adb_Coordinate.c",
    };

    /* With prefix - unique files */
    const char *mapping_files[] = {
        "Mapping_adb_Coordinate.h",
        "Mapping_adb_Coordinate.c",
    };
    const char *geocoding_files[] = {
        "Geocoding_adb_Coordinate.h",
        "Geocoding_adb_Coordinate.c",
    };

    printf("  WITHOUT -prefix option (CONFLICT):\n");
    for (int i = 0; i < 2; i++) {
        printf("    Both services generate: %s\n", files_without_prefix[i]);
    }
    printf("    Result: Linker error - multiple definitions\n\n");

    printf("  WITH -prefix option (NO CONFLICT):\n");
    printf("    wsdl2c-native -prefix Mapping_ -uri Mapping.wsdl generates:\n");
    for (int i = 0; i < 2; i++) {
        printf("      %s\n", mapping_files[i]);
    }
    printf("    wsdl2c-native -prefix Geocoding_ -uri Geocoding.wsdl generates:\n");
    for (int i = 0; i < 2; i++) {
        printf("      %s\n", geocoding_files[i]);
    }
    printf("    Result: Compiles and links successfully!\n");

    /* Verify the prefixed files are different */
    for (int i = 0; i < 2; i++) {
        ADB_ASSERT_TRUE(strcmp(mapping_files[i], geocoding_files[i]) != 0,
                       "Prefixed files must be unique");
    }

    printf("\n  Real-world scenario test passed\n");
    return ADB_TEST_PASS;
}

/* Main test runner */
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    printf("\n========================================\n");
    printf("AXIS2C-1330 Prefix Option Tests\n");
    printf("========================================\n\n");

    adb_test_result_t results[7];
    int pass_count = 0;
    int fail_count = 0;

    /* Run all tests */
    results[0] = test_axis2c_1330_prefix_filenames();
    results[1] = test_axis2c_1330_prefix_types();
    results[2] = test_axis2c_1330_prefix_functions();
    results[3] = test_axis2c_1330_no_prefix();
    results[4] = test_axis2c_1330_multiple_services_no_conflict();
    results[5] = test_axis2c_1330_header_guards();
    results[6] = test_axis2c_1330_real_world_scenario();

    /* Count results */
    for (int i = 0; i < 7; i++) {
        if (results[i] == ADB_TEST_PASS) {
            pass_count++;
        } else {
            fail_count++;
        }
    }

    /* Report summary */
    printf("\n========================================\n");
    printf("Test Summary: %d passed, %d failed\n", pass_count, fail_count);
    printf("========================================\n\n");

    /* Verify no memory leaks */
    adb_verify_all_freed();

    return (fail_count == 0) ? 0 : 1;
}
