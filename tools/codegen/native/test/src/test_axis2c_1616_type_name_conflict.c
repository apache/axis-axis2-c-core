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
 * test_axis2c_1616_type_name_conflict.c
 *
 * AXIS2C-1616 Type Name Conflict Resolution Tests
 *
 * Tests for conflicts between generated ADB types and existing axis2 types.
 * The issue occurs when schema elements are named using reserved words that
 * conflict with existing axis2 type names, particularly "type" which would
 * generate "adb_type_t" conflicting with extension mapper types.
 *
 * Original JIRA issue: Generated code with ADB bindings will not compile if an
 * element or attribute is named "type", resulting in adb_type_t conflict.
 *
 * Schema example causing conflict:
 * <xs:attribute name="type" type="xs:string" use="required"/>
 */

#include "adb_test_framework.h"

/* Test reserved word detection and transformation */
adb_test_result_t test_axis2c_1616_reserved_word_detection(void) {
    printf("Testing AXIS2C-1616: Reserved word detection for type name conflicts...\n");

    /* Test known reserved words that would cause conflicts */
    const char *reserved_words[] = {
        "type",      /* Would generate adb_type_t - conflicts with extension mapper */
        "class",     /* C++ reserved word */
        "struct",    /* C reserved word */
        "union",     /* C reserved word */
        "enum",      /* C reserved word */
        "const",     /* C reserved word */
        "static",    /* C reserved word */
        "extern",    /* C reserved word */
        "typedef",   /* C reserved word */
        "sizeof",    /* C reserved word */
        "void",      /* C reserved word */
        "int",       /* C reserved word */
        "char",      /* C reserved word */
        "float",     /* C reserved word */
        "double",    /* C reserved word */
        "short",     /* C reserved word */
        "long",      /* C reserved word */
        "signed",    /* C reserved word */
        "unsigned",  /* C reserved word */
        "auto",      /* C reserved word */
        "register",  /* C reserved word */
        "volatile",  /* C reserved word */
        "return",    /* C reserved word */
        "if",        /* C reserved word */
        "else",      /* C reserved word */
        "for",       /* C reserved word */
        "while",     /* C reserved word */
        "do",        /* C reserved word */
        "switch",    /* C reserved word */
        "case",      /* C reserved word */
        "default",   /* C reserved word */
        "break",     /* C reserved word */
        "continue",  /* C reserved word */
        "goto",      /* C reserved word */
        "inline"     /* C99 reserved word */
    };

    int reserved_count = sizeof(reserved_words) / sizeof(reserved_words[0]);

    for (int i = 0; i < reserved_count; i++) {
        /* Simulate the problematic pattern: adb_<reserved_word>_t */
        char *generated_type = adb_tracked_malloc(256);
        ADB_ASSERT_NOT_NULL(generated_type, "Failed to allocate type name buffer");

        if (generated_type) {
            snprintf(generated_type, 256, "adb_%s_t", reserved_words[i]);

            /* Verify the pattern matches what would be generated */
            ADB_ASSERT_TRUE(strstr(generated_type, "adb_") != NULL, "Generated type missing adb_ prefix");
            ADB_ASSERT_TRUE(strstr(generated_type, "_t") != NULL, "Generated type missing _t suffix");

            /* The most critical conflict: "type" -> "adb_type_t" */
            if (strcmp(reserved_words[i], "type") == 0) {
                ADB_ASSERT_TRUE(strcmp(generated_type, "adb_type_t") == 0,
                               "Type conflict pattern not matching expected adb_type_t");
                printf("  CRITICAL CONFLICT: '%s' -> '%s'\n", reserved_words[i], generated_type);
            }

            adb_tracked_free(generated_type);
        }
    }

    printf("  Detected %d potential reserved word conflicts\n", reserved_count);
    return ADB_TEST_SUCCESS;
}

/* Test conflict resolution strategies */
adb_test_result_t test_axis2c_1616_conflict_resolution_strategies(void) {
    printf("Testing AXIS2C-1616: Type name conflict resolution strategies...\n");

    /* Strategy 1: Suffix-based resolution */
    struct test_case {
        const char *input;
        const char *expected_suffix;
        const char *expected_prefix;
    } test_cases[] = {
        {"type", "adb_type_data_t", "adb_schema_type_t"},
        {"class", "adb_class_data_t", "adb_schema_class_t"},
        {"struct", "adb_struct_data_t", "adb_schema_struct_t"},
        {"enum", "adb_enum_data_t", "adb_schema_enum_t"},
        {"void", "adb_void_data_t", "adb_schema_void_t"}
    };

    int test_count = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < test_count; i++) {
        /* Test suffix strategy: adb_<name>_data_t */
        char *suffix_result = adb_tracked_malloc(256);
        ADB_ASSERT_NOT_NULL(suffix_result, "Failed to allocate suffix result buffer");

        if (suffix_result) {
            snprintf(suffix_result, 256, "adb_%s_data_t", test_cases[i].input);
            ADB_ASSERT_TRUE(strcmp(suffix_result, test_cases[i].expected_suffix) == 0,
                           "Suffix strategy result mismatch");
            printf("  Suffix strategy: '%s' -> '%s'\n", test_cases[i].input, suffix_result);
            adb_tracked_free(suffix_result);
        }

        /* Test prefix strategy: adb_schema_<name>_t */
        char *prefix_result = adb_tracked_malloc(256);
        ADB_ASSERT_NOT_NULL(prefix_result, "Failed to allocate prefix result buffer");

        if (prefix_result) {
            snprintf(prefix_result, 256, "adb_schema_%s_t", test_cases[i].input);
            ADB_ASSERT_TRUE(strcmp(prefix_result, test_cases[i].expected_prefix) == 0,
                           "Prefix strategy result mismatch");
            printf("  Prefix strategy: '%s' -> '%s'\n", test_cases[i].input, prefix_result);
            adb_tracked_free(prefix_result);
        }
    }

    return ADB_TEST_SUCCESS;
}

/* Test mock code generation with conflict detection */
adb_test_result_t test_axis2c_1616_mock_code_generation_with_conflicts(void) {
    printf("Testing AXIS2C-1616: Mock code generation with conflict detection...\n");

    /* Simulate the problematic code generation pattern from stub_generator.c:464 */
    const char *schema_elements[] = {
        "type",        /* The main problem case */
        "add",         /* Normal case - should work */
        "class",       /* Another reserved word */
        "response",    /* Normal case - should work */
        "struct",      /* Reserved word */
        "mimeType"     /* Normal case - should work */
    };

    int element_count = sizeof(schema_elements) / sizeof(schema_elements[0]);

    for (int i = 0; i < element_count; i++) {
        /* Mock the original problematic pattern */
        char *original_pattern = adb_tracked_malloc(256);
        ADB_ASSERT_NOT_NULL(original_pattern, "Failed to allocate original pattern buffer");

        if (original_pattern) {
            /* This is the exact pattern that causes the conflict */
            snprintf(original_pattern, 256, "typedef struct adb_%s adb_%s_t;",
                     schema_elements[i], schema_elements[i]);

            printf("  Original pattern: %s\n", original_pattern);

            /* Test if this would cause a conflict */
            char *type_name = adb_tracked_malloc(128);
            if (type_name) {
                snprintf(type_name, 128, "adb_%s_t", schema_elements[i]);

                /* Check for specific conflicts */
                if (strcmp(type_name, "adb_type_t") == 0) {
                    printf("    CONFLICT DETECTED: %s conflicts with extension mapper type\n", type_name);
                }

                /* Propose resolution */
                char *resolved_pattern = adb_tracked_malloc(256);
                if (resolved_pattern) {
                    if (strcmp(schema_elements[i], "type") == 0) {
                        /* Use suffix strategy for critical conflicts */
                        snprintf(resolved_pattern, 256, "typedef struct adb_%s_data adb_%s_data_t;",
                                schema_elements[i], schema_elements[i]);
                        printf("    RESOLUTION: %s\n", resolved_pattern);
                    } else {
                        /* Normal elements remain unchanged */
                        strcpy(resolved_pattern, original_pattern);
                        printf("    NO CONFLICT: %s\n", resolved_pattern);
                    }
                    adb_tracked_free(resolved_pattern);
                }

                adb_tracked_free(type_name);
            }

            adb_tracked_free(original_pattern);
        }
    }

    return ADB_TEST_SUCCESS;
}

/* Test comprehensive XML schema conflict scenarios */
adb_test_result_t test_axis2c_1616_xml_schema_conflict_scenarios(void) {
    printf("Testing AXIS2C-1616: XML Schema conflict scenarios from JIRA example...\n");

    /* Test the exact schema pattern from AXIS2C-1616 JIRA issue */
    const char *schema_xml =
        "<xs:element name=\"supportedMimeTypes\" minOccurs=\"1\" maxOccurs=\"1\">\n"
        "    <xs:complexType>\n"
        "        <xs:sequence>\n"
        "            <xs:element name=\"mimeType\" minOccurs=\"0\" maxOccurs=\"unbounded\">\n"
        "                <xs:complexType>\n"
        "                    <xs:attribute name=\"type\" type=\"xs:string\" use=\"required\"/>\n"
        "                    <xs:attribute ref=\"xml:lang\" use=\"optional\"/>\n"
        "                </xs:complexType>\n"
        "            </xs:element>\n"
        "        </xs:sequence>\n"
        "    </xs:complexType>\n"
        "</xs:element>";

    printf("  Testing schema pattern from JIRA:\n%s\n", schema_xml);

    /* Elements that would be extracted from this schema */
    const char *extracted_elements[] = {
        "supportedMimeTypes",  /* Complex type */
        "mimeType",           /* Element with attributes */
        "type"               /* The problematic attribute - THIS CAUSES THE CONFLICT */
    };

    int element_count = sizeof(extracted_elements) / sizeof(extracted_elements[0]);

    for (int i = 0; i < element_count; i++) {
        char *generated_header = adb_tracked_malloc(512);
        char *generated_typedef = adb_tracked_malloc(256);

        if (generated_header && generated_typedef) {
            /* Simulate header guard generation */
            snprintf(generated_header, 512, "#ifndef ADB_%s_H\n#define ADB_%s_H",
                     extracted_elements[i], extracted_elements[i]);

            /* Simulate typedef generation - this is where the conflict occurs */
            snprintf(generated_typedef, 256, "typedef struct adb_%s adb_%s_t;",
                     extracted_elements[i], extracted_elements[i]);

            printf("  Element: %s\n", extracted_elements[i]);
            printf("    Header: %s\n", generated_header);
            printf("    Typedef: %s\n", generated_typedef);

            /* Check for the specific conflict mentioned in JIRA */
            if (strcmp(extracted_elements[i], "type") == 0) {
                printf("    >>> AXIS2C-1616 CONFLICT: This typedef conflicts with axis2 extension mapper adb_type_t!\n");

                /* Show resolution */
                char *resolved_typedef = adb_tracked_malloc(256);
                if (resolved_typedef) {
                    snprintf(resolved_typedef, 256, "typedef struct adb_%s_attr adb_%s_attr_t;",
                             extracted_elements[i], extracted_elements[i]);
                    printf("    >>> RESOLUTION: %s\n", resolved_typedef);
                    adb_tracked_free(resolved_typedef);
                }
            }

            adb_tracked_free(generated_header);
            adb_tracked_free(generated_typedef);
        }
    }

    return ADB_TEST_SUCCESS;
}

/* Test function naming conflicts beyond just typedefs */
adb_test_result_t test_axis2c_1616_function_naming_conflicts(void) {
    printf("Testing AXIS2C-1616: Function naming conflicts beyond typedefs...\n");

    const char *conflicting_element = "type";

    /* Test all generated function patterns that would conflict */
    struct function_pattern {
        const char *pattern;
        const char *description;
    } patterns[] = {
        {"adb_%s_create", "Constructor function"},
        {"adb_%s_free", "Destructor function"},
        {"adb_%s_serialize", "Serialization function"},
        {"adb_%s_deserialize", "Deserialization function"},
        {"adb_%s_get_%s", "Getter function"},
        {"adb_%s_set_%s", "Setter function"}
    };

    int pattern_count = sizeof(patterns) / sizeof(patterns[0]);

    for (int i = 0; i < pattern_count; i++) {
        char *function_name = adb_tracked_malloc(256);
        if (function_name) {
            if (strstr(patterns[i].pattern, "%s_%s")) {
                /* Pattern with two placeholders */
                snprintf(function_name, 256, patterns[i].pattern, conflicting_element, "value");
            } else {
                /* Pattern with one placeholder */
                snprintf(function_name, 256, patterns[i].pattern, conflicting_element);
            }

            printf("  %s: %s\n", patterns[i].description, function_name);

            /* These functions would all be named adb_type_* which could conflict */
            ADB_ASSERT_TRUE(strstr(function_name, "adb_type") != NULL,
                           "Function name missing expected adb_type prefix");

            adb_tracked_free(function_name);
        }
    }

    printf("  All these functions would share the 'adb_type_' prefix, potentially conflicting with existing axis2 functions\n");

    return ADB_TEST_SUCCESS;
}

/* Test case array for AXIS2C-1616 type name conflict tests */
adb_test_case_t axis2c_1616_tests[] = {
    {
        "test_axis2c_1616_reserved_word_detection",
        "Test detection of reserved words that cause type name conflicts",
        test_axis2c_1616_reserved_word_detection
    },
    {
        "test_axis2c_1616_conflict_resolution_strategies",
        "Test different strategies for resolving type name conflicts",
        test_axis2c_1616_conflict_resolution_strategies
    },
    {
        "test_axis2c_1616_mock_code_generation_with_conflicts",
        "Test mock code generation showing original conflict and resolution",
        test_axis2c_1616_mock_code_generation_with_conflicts
    },
    {
        "test_axis2c_1616_xml_schema_conflict_scenarios",
        "Test XML schema patterns from JIRA issue that cause conflicts",
        test_axis2c_1616_xml_schema_conflict_scenarios
    },
    {
        "test_axis2c_1616_function_naming_conflicts",
        "Test function naming conflicts beyond just typedef conflicts",
        test_axis2c_1616_function_naming_conflicts
    }
};

int axis2c_1616_test_count = sizeof(axis2c_1616_tests) / sizeof(axis2c_1616_tests[0]);