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

#ifndef ADB_TEST_FRAMEWORK_H
#define ADB_TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

/**
 * ADB Test Framework for Axis2/C Native Generator
 *
 * This framework provides testing infrastructure equivalent to
 * Axis2/Java AbstractTestCase for validating ADB functionality.
 */

typedef enum {
    ADB_TEST_SUCCESS = 0,
    ADB_TEST_FAILURE = 1,
    ADB_TEST_SKIP = 2
} adb_test_result_t;

typedef struct adb_test_case {
    const char *name;
    const char *description;
    adb_test_result_t (*test_func)(void);
} adb_test_case_t;

typedef struct adb_test_stats {
    int total;
    int passed;
    int failed;
    int skipped;
} adb_test_stats_t;

/* Test framework functions */
void adb_test_init(void);
void adb_test_cleanup(void);
adb_test_result_t adb_run_test(adb_test_case_t *test_case);
void adb_run_test_suite(adb_test_case_t *test_cases, int count);
void adb_print_test_stats(adb_test_stats_t *stats);

/* Test assertion macros */
#define ADB_ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "ASSERTION FAILED: %s - %s\n", #condition, message); \
            return ADB_TEST_FAILURE; \
        } \
    } while(0)

#define ADB_ASSERT_FALSE(condition, message) \
    ADB_ASSERT_TRUE(!(condition), message)

#define ADB_ASSERT_NULL(ptr, message) \
    ADB_ASSERT_TRUE((ptr) == NULL, message)

#define ADB_ASSERT_NOT_NULL(ptr, message) \
    ADB_ASSERT_TRUE((ptr) != NULL, message)

#define ADB_ASSERT_EQUALS_STR(expected, actual, message) \
    ADB_ASSERT_TRUE(strcmp((expected), (actual)) == 0, message)

#define ADB_ASSERT_EQUALS_INT(expected, actual, message) \
    ADB_ASSERT_TRUE((expected) == (actual), message)

/* XML utility functions */
xmlDocPtr adb_parse_xml_string(const char *xml_string);
xmlDocPtr adb_parse_xml_file(const char *filename);
int adb_compare_xml_docs(xmlDocPtr doc1, xmlDocPtr doc2);
char* adb_xml_doc_to_string(xmlDocPtr doc);
void adb_free_xml_doc(xmlDocPtr doc);

/* Memory leak detection */
void adb_memory_tracker_init(void);
void adb_memory_tracker_cleanup(void);
void* adb_tracked_malloc(size_t size);
void* adb_tracked_calloc(size_t nmemb, size_t size);
void adb_tracked_free(void *ptr);
int adb_check_memory_leaks(void);

/* Test data generation utilities */
char* adb_generate_test_string(const char *prefix, int index);
int adb_generate_test_int(int base_value);
double adb_generate_test_double(double base_value);

/* WSDL generation testing */
typedef struct adb_codegen_test {
    const char *wsdl_path;
    const char *output_dir;
    const char *databinding;
    int unwrap;
    int server_side;
} adb_codegen_test_t;

adb_test_result_t adb_test_code_generation(adb_codegen_test_t *test_config);
adb_test_result_t adb_test_compile_generated_code(const char *output_dir);
adb_test_result_t adb_test_serialization_roundtrip(const char *test_data_file);

/* Test case declarations */
extern adb_test_case_t adb_basic_type_tests[];
extern int adb_basic_type_test_count;

extern adb_test_case_t adb_complex_type_tests[];
extern int adb_complex_type_test_count;

extern adb_test_case_t adb_enumeration_tests[];
extern int adb_enumeration_test_count;

extern adb_test_case_t adb_choice_tests[];
extern int adb_choice_test_count;

extern adb_test_case_t adb_array_tests[];
extern int adb_array_test_count;

/* AXIS2C-1373 regression tests */
extern adb_test_case_t axis2c_1373_tests[];
extern int axis2c_1373_test_count;

/* AXIS2C-1515 regression tests */
extern adb_test_case_t axis2c_1515_tests[];
extern int axis2c_1515_test_count;

/* AXIS2C-1699 regression tests - Sequence Type Generation Fix
 *
 * AXIS2C-1699: Wrong C code generation for sequence types with maxOccurs="unbounded"
 * Analysis (2024-11-21): C-specific bug not present in Java implementation.
 * Java correctly resolves element types using getSchemaType(), while C incorrectly
 * assigns parent container types to sequence elements, causing wrong xsi:type attributes.
 */
extern adb_test_case_t axis2c_1699_tests[];
extern int axis2c_1699_test_count;

/* AXIS2C-1685 regression tests - Serialization Runtime Error Fix
 *
 * AXIS2C-1685: Unable to serialize/deserialize object - runtime error in om_element.c
 * Analysis (2024-11-21): Runtime manifestation of AXIS2C-1699 type assignment bug.
 * When element types are incorrectly assigned, axiom_element_add_attribute fails with
 * "wrong element type or null node" error. Fix AXIS2C-1699 to resolve AXIS2C-1685.
 */
extern adb_test_case_t axis2c_1685_tests[];
extern int axis2c_1685_test_count;

/* AXIS2C-1679 feature tests - MTOM/Multipart SOAP Support
 *
 * AXIS2C-1679: Support Multipart soap in code generation tool (Feature Request)
 * Analysis (2024-11-21): Axis2/C has complete MTOM infrastructure but native WSDL2C
 * generator does not generate MTOM-aware code. Need to map xsd:base64Binary to
 * axiom_data_handler_t following Java's pattern (JavaTypeMap.java:114).
 */
extern adb_test_case_t axis2c_1679_tests[];
extern int axis2c_1679_test_count;

/* AXIS2C-1658 validation tests - WSDL Fault Definition Standards
 *
 * AXIS2C-1658: WSDL fault definition validation (type vs element)
 * Analysis (2024-11-21): Not a code generation bug but WSDL specification compliance issue.
 * User's WSDL incorrectly uses type= instead of element= in fault messages.
 * WSDL standard requires fault messages to use element= regardless of binding style.
 * Testing if native WSDL2C has same validation as Java WSDL2Java.
 */
extern adb_test_case_t axis2c_1658_tests[];
extern int axis2c_1658_test_count;

/* AXIS2C-1617 memory regression tests - Memory Leak Pattern Detection
 *
 * AXIS2C-1617: Fixes for memory leaks in generated code
 * Analysis (2024-11-21): Original patches targeted Java-based XSL templates no longer used.
 * These tests validate that native WSDL2C generator doesn't exhibit similar memory leak
 * patterns and serve as regression tests for future changes.
 *
 * Test patterns based on original Valgrind reports:
 * - axiom_attribute_create memory leaks in serialization chains
 * - axiom_namespace_create memory leaks in XSI namespace handling
 * - ADB wrapper object memory leaks after serialization
 * - Complex serialization chain memory management
 * - Cumulative memory leak detection over multiple iterations
 */
extern adb_test_case_t axis2c_1617_memory_tests[];
extern int axis2c_1617_memory_test_count;

/* AXIS2C-1616 type name conflict tests - Data Type Name Collision Resolution
 *
 * AXIS2C-1616: Conflict with data type names
 * Analysis (2024-11-21): Generated ADB code creates adb_type_t when schema elements
 * are named "type", conflicting with existing axis2 extension mapper types.
 * The issue occurs in stub_generator.c where typedef patterns directly use element names.
 *
 * Test scenarios:
 * - Reserved word detection (type, class, struct, etc.)
 * - Conflict resolution strategies (suffix/prefix approaches)
 * - Mock code generation showing conflicts and resolutions
 * - XML schema patterns from original JIRA issue
 * - Function naming conflicts beyond typedefs
 */
extern adb_test_case_t axis2c_1616_tests[];
extern int axis2c_1616_test_count;

/* AXIS2C-1614 required attribute validation tests - Deserialization Validation Enhancement
 *
 * AXIS2C-1614: Stop deserialization process when a required attribute is missing
 * Analysis (2024-11-21): Original Java XSL template patch added validation to fail
 * deserialization when required attributes are missing. Native WSDL2C generator needs
 * equivalent validation in generated deserialization code using AXIOM functions.
 *
 * Test scenarios:
 * - Detection of missing required attributes in XML input
 * - Validation logic simulation from original XSL patch
 * - Generated code patterns for attribute validation
 * - Error handling and logging for missing attributes
 * - Compatibility with dont_care_minoccurs flag
 */
extern adb_test_case_t axis2c_1614_tests[];
extern int axis2c_1614_test_count;

/* AXIS2C-1581 empty SOAP action tests - Content-Type Header Generation Fix
 *
 * AXIS2C-1581: Generated code creates bad action in multi-parts request
 * Analysis (2025-01-12): When soapAction is empty or missing in WSDL binding,
 * generated code should NOT include action="" in Content-Type header.
 *
 * Test scenarios:
 * - Empty soapAction="" should be omitted from generated code
 * - Missing soapAction attribute should result in NULL (not empty string)
 * - Valid soapAction values should be correctly extracted and used
 * - Binding operation soapAction extraction via XPath
 */
extern adb_test_case_t axis2c_1581_tests[];
extern int axis2c_1581_test_count;

/* AXIS2C-1580 any type handling tests - Safe Deserialization for xsd:any
 *
 * AXIS2C-1580: Native codegen crashes when deserializing 'any' type
 * Analysis (2025-01-12): When deserializing xsd:any elements, axiom_element_get_qname
 * returns NULL because xsd:any elements don't have a fixed qname. The generated code
 * must check for NULL before performing qname comparison operations.
 *
 * Test scenarios:
 * - xsd:any element detection in WSDL schema parsing
 * - NULL qname handling logic (prevents crash)
 * - Code generation with xsd:any types
 * - Safe deserialization patterns for extensible schemas
 */
extern adb_test_case_t axis2c_1580_tests[];
extern int axis2c_1580_test_count;

/* AXIS2C-1579 xsi:type support tests - Proper Type Polymorphism Serialization
 *
 * AXIS2C-1579: Wrong xsi:type in the response
 * Analysis (2025-01-12): Generated serialization code must use the correct XSI namespace
 * (http://www.w3.org/2001/XMLSchema-instance) for the xmlns:xsi declaration, NOT the
 * target namespace. The xsi:type attribute must also use properly prefixed type names.
 *
 * Test scenarios:
 * - XSI namespace constant validation
 * - xsi:type attribute format validation (prefixed type names)
 * - Generated code verification for correct namespace usage
 * - XML output format validation
 */
extern adb_test_case_t axis2c_1579_tests[];
extern int axis2c_1579_test_count;

/* AXIS2C-1575 QName property support tests - Variable Scope Fix
 *
 * AXIS2C-1575: Generated code has undefined variables qname_uri and qname_prefix
 * Analysis (2025-01-12): Variables qname_uri, qname_prefix, and qname_localpart MUST
 * be declared at function scope, NOT inside conditional blocks. The original XSL template
 * bug declared these inside <xsl:otherwise> but used them for all QName properties.
 *
 * Test scenarios:
 * - Variable scope pattern validation
 * - QName serialization format (prefix:localpart)
 * - QName deserialization format parsing
 * - Generated code verification for proper variable scoping
 */
extern adb_test_case_t axis2c_1575_tests[];
extern int axis2c_1575_test_count;

/* Global test statistics */
extern adb_test_stats_t g_test_stats;

#endif /* ADB_TEST_FRAMEWORK_H */