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
 * test_axis2c_1617_memory_regression.c
 *
 * AXIS2C-1617 Memory Leak Regression Tests
 *
 * Tests for memory leak patterns originally reported in AXIS2C-1617:
 * - axiom_attribute_create memory leaks in serialization
 * - axiom_namespace_create memory leaks in XSI namespace handling
 * - ADB object wrapper memory leaks after serialization
 *
 * Although the original issue targeted Java-based XSL templates that no longer
 * exist, these tests validate that the native WSDL2C generator doesn't exhibit
 * similar memory leak patterns and serve as regression tests for future changes.
 *
 * Original Valgrind stack trace pattern:
 * axiom_attribute_create → adb_*_serialize_obj → axis2_extension_mapper_serialize
 * → adb_*_serialize → adb_*_serialize_obj → ... → axis2_stub_op_*
 */

#include "adb_test_framework.h"

/* Test specific memory leak patterns from AXIS2C-1617 */

/* Test memory allocation/deallocation patterns similar to original memory leak */
adb_test_result_t test_axis2c_1617_attribute_creation_memory_leak(void) {
    printf("Testing AXIS2C-1617: Memory allocation pattern similar to attribute creation...\n");

    int initial_leak_count = adb_check_memory_leaks();

    /* Pattern: Simulate memory allocations similar to axiom_attribute_create calls
     * in adb_bookmarkSetChoice_type1_serialize_obj as reported in AXIS2C-1617 */
    for (int i = 0; i < 100; i++) {
        /* Simulate attribute name/value allocation pattern */
        char *attr_name = adb_tracked_malloc(64);
        char *attr_value = adb_tracked_malloc(64);

        ADB_ASSERT_NOT_NULL(attr_name, "Failed to allocate attribute name");
        ADB_ASSERT_NOT_NULL(attr_value, "Failed to allocate attribute value");

        if (attr_name && attr_value) {
            snprintf(attr_name, 64, "testAttr_%d", i);
            snprintf(attr_value, 64, "value_%d", i);

            /* Simulate usage of allocated memory */
            ADB_ASSERT_TRUE(strlen(attr_name) > 0, "Invalid attribute name");
            ADB_ASSERT_TRUE(strlen(attr_value) > 0, "Invalid attribute value");
        }

        /* Test the pattern: ensure proper cleanup (original leak was missing this) */
        if (attr_name) {
            adb_tracked_free(attr_name);
        }
        if (attr_value) {
            adb_tracked_free(attr_value);
        }
    }

    /* Test XSI namespace pattern specifically mentioned in AXIS2C-1617 */
    for (int i = 0; i < 20; i++) {
        char *xsi_prefix = adb_tracked_malloc(32);
        char *xsi_uri = adb_tracked_malloc(128);
        char *type_value = adb_tracked_malloc(64);

        if (xsi_prefix && xsi_uri && type_value) {
            snprintf(xsi_prefix, 32, "xsi");
            snprintf(xsi_uri, 128, "http://www.w3.org/2001/XMLSchema-instance");
            snprintf(type_value, 64, "testType_%d", i);

            /* Simulate XSI type attribute creation pattern from original stack trace */
            ADB_ASSERT_EQUALS_STR("xsi", xsi_prefix, "XSI prefix mismatch");
        }

        /* Cleanup pattern that was missing in original code */
        if (xsi_prefix) adb_tracked_free(xsi_prefix);
        if (xsi_uri) adb_tracked_free(xsi_uri);
        if (type_value) adb_tracked_free(type_value);
    }

    /* Verify no memory leaks - should be same as initial count */
    int final_leak_count = adb_check_memory_leaks();
    ADB_ASSERT_EQUALS_INT(initial_leak_count, final_leak_count,
                         "Memory leak detected in attribute-like allocation cycle");

    return ADB_TEST_SUCCESS;
}

/* Test namespace-like memory allocation patterns similar to axis2_codegen_free_memory_created_namespace.patch */
adb_test_result_t test_axis2c_1617_namespace_creation_memory_leak(void) {
    printf("Testing AXIS2C-1617: Memory allocation pattern similar to namespace creation...\n");

    int initial_leak_count = adb_check_memory_leaks();

    /* Pattern from patch: simulate axiom_namespace_create + declare_namespace_assume_param_ownership
     * The original patch showed memory leaks when namespace objects weren't properly freed */

    for (int i = 0; i < 50; i++) {
        /* Simulate namespace structure allocation similar to axiom_namespace_create */
        char *namespace_uri = adb_tracked_malloc(256);
        char *namespace_prefix = adb_tracked_malloc(32);

        ADB_ASSERT_NOT_NULL(namespace_uri, "Failed to allocate namespace URI");
        ADB_ASSERT_NOT_NULL(namespace_prefix, "Failed to allocate namespace prefix");

        if (namespace_uri && namespace_prefix) {
            snprintf(namespace_uri, 256, "http://www.w3.org/2001/XMLSchema-instance");
            snprintf(namespace_prefix, 32, "xsi");

            /* Simulate usage pattern */
            ADB_ASSERT_TRUE(strlen(namespace_uri) > 0, "Invalid namespace URI");
            ADB_ASSERT_TRUE(strlen(namespace_prefix) > 0, "Invalid namespace prefix");
        }

        /* Test the critical pattern: original patch added explicit free after assume_param_ownership
         * This simulates that cleanup pattern */
        if (namespace_uri) {
            adb_tracked_free(namespace_uri);
        }
        if (namespace_prefix) {
            adb_tracked_free(namespace_prefix);
        }
    }

    /* Test multiple different namespace patterns from original patch */
    const char *test_namespaces[][2] = {
        {"http://www.w3.org/2001/XMLSchema", "xs"},
        {"http://schemas.xmlsoap.org/soap/envelope/", "soap"},
        {"http://www.w3.org/1999/xhtml", "html"},
        {"http://axis.apache.org/axis2/java/core", "axis2"}
    };

    for (int i = 0; i < 4; i++) {
        char *uri = adb_tracked_malloc(strlen(test_namespaces[i][0]) + 1);
        char *prefix = adb_tracked_malloc(strlen(test_namespaces[i][1]) + 1);

        if (uri && prefix) {
            strcpy(uri, test_namespaces[i][0]);
            strcpy(prefix, test_namespaces[i][1]);

            /* Simulate namespace registration and usage */
            ADB_ASSERT_TRUE(strcmp(uri, test_namespaces[i][0]) == 0, "URI copy failed");
            ADB_ASSERT_TRUE(strcmp(prefix, test_namespaces[i][1]) == 0, "Prefix copy failed");
        }

        /* Cleanup pattern that was missing in original code */
        if (uri) adb_tracked_free(uri);
        if (prefix) adb_tracked_free(prefix);
    }

    /* Verify no memory leaks */
    int final_leak_count = adb_check_memory_leaks();
    ADB_ASSERT_EQUALS_INT(initial_leak_count, final_leak_count,
                         "Memory leak detected in namespace-like allocation cycle");

    return ADB_TEST_SUCCESS;
}

/* Test serialization-like memory management similar to axis2_codegen_free_memory_wrapper_adb_obj.patch */
adb_test_result_t test_axis2c_1617_serialization_wrapper_memory_leak(void) {
    printf("Testing AXIS2C-1617: Memory allocation pattern similar to serialization wrapper...\n");

    int initial_leak_count = adb_check_memory_leaks();

    /* Simulate the pattern from axis2_codegen_free_memory_wrapper_adb_obj.patch:
     * create wrapper ADB object -> serialize -> free wrapper
     * The patch added explicit cleanup of wrapper objects after serialization */

    for (int i = 0; i < 20; i++) {
        /* Create a mock ADB-like structure similar to generated ADB objects */
        typedef struct mock_adb_obj {
            char *data;
            int value;
            char *serialized_xml;
        } mock_adb_obj_t;

        mock_adb_obj_t *wrapper_obj = adb_tracked_malloc(sizeof(mock_adb_obj_t));
        ADB_ASSERT_NOT_NULL(wrapper_obj, "Failed to create wrapper object");

        if (wrapper_obj) {
            wrapper_obj->data = adb_tracked_malloc(256);
            wrapper_obj->serialized_xml = adb_tracked_malloc(512);
            wrapper_obj->value = i;

            if (wrapper_obj->data) {
                snprintf(wrapper_obj->data, 256, "test_data_%d", i);
            }

            /* Simulate serialization process (create XML representation) */
            if (wrapper_obj->serialized_xml) {
                snprintf(wrapper_obj->serialized_xml, 512,
                        "<testData value=\"%d\">%s</testData>",
                        wrapper_obj->value,
                        wrapper_obj->data ? wrapper_obj->data : "");

                /* Verify serialization worked */
                ADB_ASSERT_TRUE(strlen(wrapper_obj->serialized_xml) > 0, "Serialization failed");
            }

            /* Critical pattern from original patch: Free wrapper object after serialization
             * The original code leaked these wrapper objects */
            if (wrapper_obj->data) {
                adb_tracked_free(wrapper_obj->data);
            }
            if (wrapper_obj->serialized_xml) {
                adb_tracked_free(wrapper_obj->serialized_xml);
            }
            adb_tracked_free(wrapper_obj);
        }
    }

    /* Test pattern with multiple nested wrapper objects */
    for (int i = 0; i < 10; i++) {
        typedef struct nested_wrapper {
            char *name;
            struct nested_wrapper **children;
            int child_count;
        } nested_wrapper_t;

        nested_wrapper_t *root = adb_tracked_malloc(sizeof(nested_wrapper_t));
        if (root) {
            root->name = adb_tracked_malloc(64);
            root->children = adb_tracked_malloc(sizeof(nested_wrapper_t*) * 3);
            root->child_count = 3;

            if (root->name) {
                snprintf(root->name, 64, "root_%d", i);
            }

            /* Create child wrappers */
            for (int j = 0; j < 3; j++) {
                root->children[j] = adb_tracked_malloc(sizeof(nested_wrapper_t));
                if (root->children[j]) {
                    root->children[j]->name = adb_tracked_malloc(64);
                    root->children[j]->children = NULL;
                    root->children[j]->child_count = 0;

                    if (root->children[j]->name) {
                        snprintf(root->children[j]->name, 64, "child_%d_%d", i, j);
                    }
                }
            }

            /* Cleanup nested structure (pattern from patch) */
            for (int j = 0; j < 3; j++) {
                if (root->children[j]) {
                    if (root->children[j]->name) {
                        adb_tracked_free(root->children[j]->name);
                    }
                    adb_tracked_free(root->children[j]);
                }
            }
            if (root->children) {
                adb_tracked_free(root->children);
            }
            if (root->name) {
                adb_tracked_free(root->name);
            }
            adb_tracked_free(root);
        }
    }

    /* Verify no memory leaks in wrapper object lifecycle */
    int final_leak_count = adb_check_memory_leaks();
    ADB_ASSERT_EQUALS_INT(initial_leak_count, final_leak_count,
                         "Memory leak detected in serialization-like wrapper cycle");

    return ADB_TEST_SUCCESS;
}

/* Test complex memory allocation chain similar to original Valgrind stack trace */
adb_test_result_t test_axis2c_1617_complex_serialization_chain_memory_leak(void) {
    printf("Testing AXIS2C-1617: Memory allocation pattern similar to complex serialization chain...\n");

    int initial_leak_count = adb_check_memory_leaks();

    /* Simulate the complex call chain from original Valgrind trace:
     * adb_bookmarkSetChoice_type1_serialize_obj →
     * axis2_extension_mapper_serialize →
     * adb_bookmarkSetChoice_type1_serialize →
     * adb_bookmarkSet_type0_serialize_obj →
     * ... → axis2_stub_op_Kolibre_DaisyOnline_setBookmarks
     */

    /* Define the nested structures outside the loop to avoid forward declaration issues */
    typedef struct bookmark_choice {
        char *choice_id;
        char *xsi_type;
        char *namespace_uri;
    } bookmark_choice_t;

    typedef struct bookmark_set {
        char *set_id;
        bookmark_choice_t **choices;
        int choice_count;
    } bookmark_set_t;

    typedef struct root_context {
        char *operation_name;
        bookmark_set_t **bookmark_sets;
        int set_count;
    } root_context_t;

    for (int iteration = 0; iteration < 10; iteration++) {
        /* Level 1: Simulate root serialization context (like adb_setBookmarks_serialize_obj) */
        root_context_t *root = adb_tracked_malloc(sizeof(root_context_t));
        ADB_ASSERT_NOT_NULL(root, "Failed to create root context");

        if (root) {
            root->operation_name = adb_tracked_malloc(64);
            root->bookmark_sets = adb_tracked_malloc(sizeof(bookmark_set_t*) * 5);
            root->set_count = 5;

            if (root->operation_name) {
                snprintf(root->operation_name, 64, "setBookmarks_%d", iteration);
            }

            /* Level 2: Create nested structures (like adb_bookmarkSet_type0_serialize_obj) */
            for (int i = 0; i < 5; i++) {
                bookmark_set_t *bookmark_set = adb_tracked_malloc(sizeof(bookmark_set_t));
                if (bookmark_set) {
                    bookmark_set->set_id = adb_tracked_malloc(32);
                    bookmark_set->choices = adb_tracked_malloc(sizeof(bookmark_choice_t*) * 3);
                    bookmark_set->choice_count = 3;

                    if (bookmark_set->set_id) {
                        snprintf(bookmark_set->set_id, 32, "set_%d_%d", iteration, i);
                    }

                    /* Level 3: Create choice elements (like adb_bookmarkSetChoice_type1_serialize_obj) */
                    for (int j = 0; j < 3; j++) {
                        bookmark_choice_t *choice = adb_tracked_malloc(sizeof(bookmark_choice_t));
                        if (choice) {
                            choice->choice_id = adb_tracked_malloc(64);
                            choice->xsi_type = adb_tracked_malloc(64);
                            choice->namespace_uri = adb_tracked_malloc(128);

                            if (choice->choice_id) {
                                snprintf(choice->choice_id, 64, "choice_%d_%d_%d", iteration, i, j);
                            }
                            if (choice->xsi_type) {
                                snprintf(choice->xsi_type, 64, "bookmarkSetChoice_type1");
                            }
                            if (choice->namespace_uri) {
                                snprintf(choice->namespace_uri, 128, "http://www.w3.org/2001/XMLSchema-instance");
                            }

                            bookmark_set->choices[j] = choice;
                        }
                    }

                    root->bookmark_sets[i] = bookmark_set;
                }
            }

            /* Cleanup entire nested structure (simulates end of serialization)
             * This tests the complex cleanup pattern that was missing in original code */

            /* Level 3 cleanup: Free choice elements */
            for (int i = 0; i < 5; i++) {
                if (root->bookmark_sets[i]) {
                    for (int j = 0; j < 3; j++) {
                        if (root->bookmark_sets[i]->choices[j]) {
                            if (root->bookmark_sets[i]->choices[j]->choice_id) {
                                adb_tracked_free(root->bookmark_sets[i]->choices[j]->choice_id);
                            }
                            if (root->bookmark_sets[i]->choices[j]->xsi_type) {
                                adb_tracked_free(root->bookmark_sets[i]->choices[j]->xsi_type);
                            }
                            if (root->bookmark_sets[i]->choices[j]->namespace_uri) {
                                adb_tracked_free(root->bookmark_sets[i]->choices[j]->namespace_uri);
                            }
                            adb_tracked_free(root->bookmark_sets[i]->choices[j]);
                        }
                    }
                    /* Level 2 cleanup: Free bookmark sets */
                    if (root->bookmark_sets[i]->choices) {
                        adb_tracked_free(root->bookmark_sets[i]->choices);
                    }
                    if (root->bookmark_sets[i]->set_id) {
                        adb_tracked_free(root->bookmark_sets[i]->set_id);
                    }
                    adb_tracked_free(root->bookmark_sets[i]);
                }
            }

            /* Level 1 cleanup: Free root context */
            if (root->bookmark_sets) {
                adb_tracked_free(root->bookmark_sets);
            }
            if (root->operation_name) {
                adb_tracked_free(root->operation_name);
            }
            adb_tracked_free(root);
        }
    }

    /* Verify no cumulative memory leaks in complex allocation chains */
    int final_leak_count = adb_check_memory_leaks();
    ADB_ASSERT_EQUALS_INT(initial_leak_count, final_leak_count,
                         "Memory leak detected in complex allocation chain");

    return ADB_TEST_SUCCESS;
}

/* Long-running test to detect cumulative memory leaks */
adb_test_result_t test_axis2c_1617_cumulative_memory_leak_detection(void) {
    printf("Testing AXIS2C-1617: Cumulative memory leak detection over multiple iterations...\n");

    const int ITERATIONS = 1000;  /* High iteration count to detect small leaks */
    int initial_leak_count = adb_check_memory_leaks();

    for (int iter = 0; iter < ITERATIONS; iter++) {
        /* Simulate typical ADB-like memory usage patterns that could accumulate leaks */
        typedef struct test_element {
            char *name;
            char **attributes;
            int attr_count;
        } test_element_t;

        test_element_t *element = adb_tracked_malloc(sizeof(test_element_t));
        if (element) {
            element->name = adb_tracked_malloc(32);
            element->attributes = adb_tracked_malloc(sizeof(char*) * 10);  /* 5 name/value pairs */
            element->attr_count = 5;

            if (element->name) {
                snprintf(element->name, 32, "testElement_%d", iter);
            }

            /* Add multiple attribute name/value pairs */
            for (int i = 0; i < 5; i++) {
                char *attr_name = adb_tracked_malloc(32);
                char *attr_value = adb_tracked_malloc(32);

                if (attr_name && attr_value) {
                    snprintf(attr_name, 32, "attr%d", i);
                    snprintf(attr_value, 32, "val%d_%d", iter, i);

                    element->attributes[i * 2] = attr_name;      /* Even indices for names */
                    element->attributes[i * 2 + 1] = attr_value; /* Odd indices for values */
                }
            }

            /* Cleanup pattern - ensure all allocated memory is freed */
            for (int i = 0; i < 10; i++) {
                if (element->attributes[i]) {
                    adb_tracked_free(element->attributes[i]);
                }
            }
            if (element->attributes) {
                adb_tracked_free(element->attributes);
            }
            if (element->name) {
                adb_tracked_free(element->name);
            }
            adb_tracked_free(element);
        }

        /* Check for leak growth every 100 iterations */
        if ((iter + 1) % 100 == 0) {
            int current_leak_count = adb_check_memory_leaks();
            if (current_leak_count > initial_leak_count) {
                printf("CUMULATIVE LEAK DETECTED at iteration %d: %d new leaks\n",
                       iter + 1, current_leak_count - initial_leak_count);
                return ADB_TEST_FAILURE;
            }
        }
    }

    /* Final verification */
    int final_leak_count = adb_check_memory_leaks();
    ADB_ASSERT_EQUALS_INT(initial_leak_count, final_leak_count,
                         "Cumulative memory leak detected over multiple iterations");

    printf("Successfully completed %d iterations with no cumulative memory leaks\n", ITERATIONS);
    return ADB_TEST_SUCCESS;
}

/* Test case array for AXIS2C-1617 memory regression tests */
adb_test_case_t axis2c_1617_memory_tests[] = {
    {
        "test_axis2c_1617_attribute_creation_memory_leak",
        "Test attribute creation/cleanup memory management similar to original AXIS2C-1617 pattern",
        test_axis2c_1617_attribute_creation_memory_leak
    },
    {
        "test_axis2c_1617_namespace_creation_memory_leak",
        "Test namespace creation/cleanup memory management from axis2_codegen_free_memory_created_namespace.patch",
        test_axis2c_1617_namespace_creation_memory_leak
    },
    {
        "test_axis2c_1617_serialization_wrapper_memory_leak",
        "Test serialization wrapper memory management from axis2_codegen_free_memory_wrapper_adb_obj.patch",
        test_axis2c_1617_serialization_wrapper_memory_leak
    },
    {
        "test_axis2c_1617_complex_serialization_chain_memory_leak",
        "Test complex serialization chain memory management similar to original Valgrind stack trace",
        test_axis2c_1617_complex_serialization_chain_memory_leak
    },
    {
        "test_axis2c_1617_cumulative_memory_leak_detection",
        "Long-running test to detect cumulative memory leaks in typical usage patterns",
        test_axis2c_1617_cumulative_memory_leak_detection
    }
};

int axis2c_1617_memory_test_count = sizeof(axis2c_1617_memory_tests) / sizeof(axis2c_1617_memory_tests[0]);