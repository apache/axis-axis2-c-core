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
 * AXIS2C-1182: adb_XXXX_add_ functions failing to produce correct XML
 *
 * Analysis: When using adb_Predicate_add_nodes(), the generated XML produces
 * empty <nodes/> tags instead of separate <nodes>...</nodes> elements for each
 * item. This is because the struct was using a single pointer instead of an
 * axutil_array_list_t*, so add_ just overwrote the value each time.
 *
 * Fix: The native generator now:
 * - Uses axutil_array_list_t* for array elements (maxOccurs > 1)
 * - Generates add_X() to add items to the array list
 * - Generates get_X_at() to get item at index
 * - Generates sizeof_X() to get array size
 * - Serializes array by looping through list creating separate XML elements
 * - Deserializes array by collecting repeated elements into the list
 *
 * Test scenarios:
 * - Generated struct uses axutil_array_list_t* for array elements
 * - add_ function creates and populates array list
 * - get_at function retrieves items by index
 * - sizeof_ function returns array size
 * - Serialization creates multiple XML elements
 * - Deserialization collects multiple elements into array
 */

#include "adb_test_framework.h"

/* Test 1: Verify generated struct uses axutil_array_list_t* for arrays */
adb_test_result_t test_axis2c_1182_array_struct_type(void) {
    printf("Testing AXIS2C-1182: Generated struct uses axutil_array_list_t* for arrays...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/axis2c_1182_test.wsdl",
        .output_dir = "output/axis2c_1182_test",
        .databinding = "adb",
        .unwrap = 0,
        .server_side = 0
    };

    /* WSDL with array element (maxOccurs="unbounded") */
    const char *array_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://axis2c.apache.org/test/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://axis2c.apache.org/test/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://axis2c.apache.org/test/\">\n"
        "      <xsd:complexType name=\"Predicate\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"nodes\" type=\"xsd:string\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"PredicateRequest\" type=\"tns:Predicate\"/>\n"
        "      <xsd:element name=\"PredicateResponse\" type=\"xsd:string\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"PredicateMessage\">\n"
        "    <part name=\"body\" element=\"tns:PredicateRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"PredicateResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:PredicateResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"PredicatePortType\">\n"
        "    <operation name=\"testPredicate\">\n"
        "      <input message=\"tns:PredicateMessage\"/>\n"
        "      <output message=\"tns:PredicateResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"PredicateBinding\" type=\"tns:PredicatePortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"testPredicate\">\n"
        "      <soap:operation soapAction=\"urn:testPredicate\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"PredicateService\">\n"
        "    <port name=\"PredicatePort\" binding=\"tns:PredicateBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/PredicateService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Create directories */
    if (system("mkdir -p wsdl output/axis2c_1182_test") != 0) {
        printf("Warning: Failed to create directories\n");
    }

    /* Write WSDL */
    FILE *wsdl_file = fopen("wsdl/axis2c_1182_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create WSDL file");
    fputs(array_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Generate code */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Code generation failed");

    /* Verify generated struct uses axutil_array_list_t* */
    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char line[512];
    int found_array_list_type = 0;
    while (fgets(line, sizeof(line), source)) {
        /* Look for the struct definition with axutil_array_list_t* */
        if (strstr(line, "axutil_array_list_t*") && strstr(line, "nodes")) {
            found_array_list_type = 1;
            break;
        }
    }
    fclose(source);

    ADB_ASSERT_TRUE(found_array_list_type, "Generated struct should use axutil_array_list_t* for nodes");

    printf("  PASS: Generated struct uses axutil_array_list_t* for array elements\n");
    return ADB_TEST_SUCCESS;
}

/* Test 2: Verify add_ function is generated */
adb_test_result_t test_axis2c_1182_add_function_generated(void) {
    printf("Testing AXIS2C-1182: add_ function is generated...\n");

    /* Uses output from previous test */
    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char line[512];
    int found_add_function = 0;
    while (fgets(line, sizeof(line), source)) {
        if (strstr(line, "adb_Predicate_add_nodes")) {
            found_add_function = 1;
            break;
        }
    }
    fclose(source);

    ADB_ASSERT_TRUE(found_add_function, "Generated code should have add_nodes function");

    printf("  PASS: add_nodes() function is generated\n");
    return ADB_TEST_SUCCESS;
}

/* Test 3: Verify get_at function is generated */
adb_test_result_t test_axis2c_1182_get_at_function_generated(void) {
    printf("Testing AXIS2C-1182: get_at function is generated...\n");

    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char line[512];
    int found_get_at_function = 0;
    while (fgets(line, sizeof(line), source)) {
        if (strstr(line, "adb_Predicate_get_nodes_at")) {
            found_get_at_function = 1;
            break;
        }
    }
    fclose(source);

    ADB_ASSERT_TRUE(found_get_at_function, "Generated code should have get_nodes_at function");

    printf("  PASS: get_nodes_at() function is generated\n");
    return ADB_TEST_SUCCESS;
}

/* Test 4: Verify sizeof_ function is generated */
adb_test_result_t test_axis2c_1182_sizeof_function_generated(void) {
    printf("Testing AXIS2C-1182: sizeof_ function is generated...\n");

    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char line[512];
    int found_sizeof_function = 0;
    while (fgets(line, sizeof(line), source)) {
        if (strstr(line, "adb_Predicate_sizeof_nodes")) {
            found_sizeof_function = 1;
            break;
        }
    }
    fclose(source);

    ADB_ASSERT_TRUE(found_sizeof_function, "Generated code should have sizeof_nodes function");

    printf("  PASS: sizeof_nodes() function is generated\n");
    return ADB_TEST_SUCCESS;
}

/* Test 5: Verify serialization loops through array */
adb_test_result_t test_axis2c_1182_serialization_loops(void) {
    printf("Testing AXIS2C-1182: Serialization loops through array...\n");

    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char buffer[32768];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, source);
    buffer[bytes_read] = '\0';
    fclose(source);

    /* Check for serialization loop pattern */
    int found_ser_loop = (strstr(buffer, "_ser_size = axutil_array_list_size") != NULL) &&
                         (strstr(buffer, "for (_ser_i = 0; _ser_i < _ser_size") != NULL);

    ADB_ASSERT_TRUE(found_ser_loop, "Serialization should loop through array list");

    printf("  PASS: Serialization loops through array creating separate elements\n");
    return ADB_TEST_SUCCESS;
}

/* Test 6: Verify deserialization collects into array */
adb_test_result_t test_axis2c_1182_deserialization_collects(void) {
    printf("Testing AXIS2C-1182: Deserialization collects into array...\n");

    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char buffer[32768];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, source);
    buffer[bytes_read] = '\0';
    fclose(source);

    /* Check for deserialization array collection pattern */
    int found_deser_collect = (strstr(buffer, "axutil_array_list_create(env, 4)") != NULL) &&
                              (strstr(buffer, "axutil_array_list_add(obj->nodes") != NULL);

    ADB_ASSERT_TRUE(found_deser_collect, "Deserialization should collect elements into array list");

    printf("  PASS: Deserialization collects multiple elements into array\n");
    return ADB_TEST_SUCCESS;
}

/* Test 7: Verify header declarations for array functions */
adb_test_result_t test_axis2c_1182_header_declarations(void) {
    printf("Testing AXIS2C-1182: Header has array function declarations...\n");

    FILE *header = fopen("output/axis2c_1182_test/src/adb_Predicate.h", "r");
    ADB_ASSERT_NOT_NULL(header, "Could not open generated header file");

    char buffer[16384];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, header);
    buffer[bytes_read] = '\0';
    fclose(header);

    /* Check for function declarations */
    int has_get = strstr(buffer, "adb_Predicate_get_nodes(") != NULL;
    int has_set = strstr(buffer, "adb_Predicate_set_nodes(") != NULL;
    int has_add = strstr(buffer, "adb_Predicate_add_nodes(") != NULL;
    int has_get_at = strstr(buffer, "adb_Predicate_get_nodes_at(") != NULL;
    int has_sizeof = strstr(buffer, "adb_Predicate_sizeof_nodes(") != NULL;

    ADB_ASSERT_TRUE(has_get, "Header should declare get_nodes()");
    ADB_ASSERT_TRUE(has_set, "Header should declare set_nodes()");
    ADB_ASSERT_TRUE(has_add, "Header should declare add_nodes()");
    ADB_ASSERT_TRUE(has_get_at, "Header should declare get_nodes_at()");
    ADB_ASSERT_TRUE(has_sizeof, "Header should declare sizeof_nodes()");

    printf("  PASS: Header declares all array accessor functions\n");
    return ADB_TEST_SUCCESS;
}

/* Test 8: Verify destructor frees array contents */
adb_test_result_t test_axis2c_1182_destructor_frees_array(void) {
    printf("Testing AXIS2C-1182: Destructor properly frees array contents...\n");

    FILE *source = fopen("output/axis2c_1182_test/src/adb_Predicate.c", "r");
    ADB_ASSERT_NOT_NULL(source, "Could not open generated source file");

    char buffer[32768];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, source);
    buffer[bytes_read] = '\0';
    fclose(source);

    /* Check for destructor array cleanup pattern */
    int found_arr_free = (strstr(buffer, "for (_arr_i = 0; _arr_i < _arr_size") != NULL) &&
                         (strstr(buffer, "axutil_array_list_free(") != NULL);

    ADB_ASSERT_TRUE(found_arr_free, "Destructor should free array contents");

    printf("  PASS: Destructor properly frees array list and contents\n");
    return ADB_TEST_SUCCESS;
}

/* Test case array */
adb_test_case_t axis2c_1182_tests[] = {
    {"test_axis2c_1182_array_struct_type", "AXIS2C-1182: Generated struct uses axutil_array_list_t*", test_axis2c_1182_array_struct_type},
    {"test_axis2c_1182_add_function_generated", "AXIS2C-1182: add_ function is generated", test_axis2c_1182_add_function_generated},
    {"test_axis2c_1182_get_at_function_generated", "AXIS2C-1182: get_at function is generated", test_axis2c_1182_get_at_function_generated},
    {"test_axis2c_1182_sizeof_function_generated", "AXIS2C-1182: sizeof_ function is generated", test_axis2c_1182_sizeof_function_generated},
    {"test_axis2c_1182_serialization_loops", "AXIS2C-1182: Serialization loops through array", test_axis2c_1182_serialization_loops},
    {"test_axis2c_1182_deserialization_collects", "AXIS2C-1182: Deserialization collects into array", test_axis2c_1182_deserialization_collects},
    {"test_axis2c_1182_header_declarations", "AXIS2C-1182: Header has array function declarations", test_axis2c_1182_header_declarations},
    {"test_axis2c_1182_destructor_frees_array", "AXIS2C-1182: Destructor properly frees array contents", test_axis2c_1182_destructor_frees_array}
};

int axis2c_1182_test_count = sizeof(axis2c_1182_tests) / sizeof(adb_test_case_t);
