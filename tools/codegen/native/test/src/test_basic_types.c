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

#include "adb_test_framework.h"

/* Test basic string type handling */
adb_test_result_t test_string_type(void) {
    /* Test code generation for basic string types */
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/basic_string_test.wsdl",
        .output_dir = "output/string_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    /* Create basic string test WSDL if it doesn't exist */
    const char *basic_string_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:element name=\"StringRequest\" type=\"xsd:string\"/>\n"
        "      <xsd:element name=\"StringResponse\" type=\"xsd:string\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"StringMessage\">\n"
        "    <part name=\"body\" element=\"tns:StringRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"StringResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:StringResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"StringPortType\">\n"
        "    <operation name=\"StringOperation\">\n"
        "      <input message=\"tns:StringMessage\"/>\n"
        "      <output message=\"tns:StringResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"StringBinding\" type=\"tns:StringPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"StringOperation\">\n"
        "      <soap:operation soapAction=\"urn:StringOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"StringService\">\n"
        "    <port name=\"StringPort\" binding=\"tns:StringBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/StringService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    if (system("mkdir -p wsdl output/string_test") != 0) {
        printf("Warning: Failed to create directories\n");
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/basic_string_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create string test WSDL file");

    fputs(basic_string_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "String type code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "String type compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Test basic integer type handling */
adb_test_result_t test_integer_type(void) {
    /* Test code generation for basic integer types */
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/basic_integer_test.wsdl",
        .output_dir = "output/integer_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *basic_integer_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:element name=\"IntegerRequest\" type=\"xsd:int\"/>\n"
        "      <xsd:element name=\"IntegerResponse\" type=\"xsd:int\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"IntegerMessage\">\n"
        "    <part name=\"body\" element=\"tns:IntegerRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"IntegerResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:IntegerResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"IntegerPortType\">\n"
        "    <operation name=\"IntegerOperation\">\n"
        "      <input message=\"tns:IntegerMessage\"/>\n"
        "      <output message=\"tns:IntegerResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"IntegerBinding\" type=\"tns:IntegerPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"IntegerOperation\">\n"
        "      <soap:operation soapAction=\"urn:IntegerOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"IntegerService\">\n"
        "    <port name=\"IntegerPort\" binding=\"tns:IntegerBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/IntegerService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    if (system("mkdir -p wsdl output/integer_test") != 0) {
        printf("Warning: Failed to create directories\n");
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/basic_integer_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create integer test WSDL file");

    fputs(basic_integer_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Integer type code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Integer type compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Test basic boolean type handling */
adb_test_result_t test_boolean_type(void) {
    printf("Testing boolean type code generation...\n");

    /* For now, just verify that the native generator accepts boolean types */
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/basic_boolean_test.wsdl",
        .output_dir = "output/boolean_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *basic_boolean_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:element name=\"BooleanRequest\" type=\"xsd:boolean\"/>\n"
        "      <xsd:element name=\"BooleanResponse\" type=\"xsd:boolean\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"BooleanMessage\">\n"
        "    <part name=\"body\" element=\"tns:BooleanRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"BooleanResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:BooleanResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"BooleanPortType\">\n"
        "    <operation name=\"BooleanOperation\">\n"
        "      <input message=\"tns:BooleanMessage\"/>\n"
        "      <output message=\"tns:BooleanResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"BooleanBinding\" type=\"tns:BooleanPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"BooleanOperation\">\n"
        "      <soap:operation soapAction=\"urn:BooleanOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"BooleanService\">\n"
        "    <port name=\"BooleanPort\" binding=\"tns:BooleanBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/BooleanService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/boolean_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/basic_boolean_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create boolean test WSDL file");

    fputs(basic_boolean_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Boolean type code generation failed");

    return ADB_TEST_SUCCESS;
}

/* Test XML parsing and validation */
adb_test_result_t test_xml_parsing(void) {
    const char *test_xml =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<root>\n"
        "  <element1>value1</element1>\n"
        "  <element2>value2</element2>\n"
        "</root>\n";

    xmlDocPtr doc = adb_parse_xml_string(test_xml);
    ADB_ASSERT_NOT_NULL(doc, "Failed to parse test XML string");

    char *xml_str = adb_xml_doc_to_string(doc);
    ADB_ASSERT_NOT_NULL(xml_str, "Failed to convert XML doc to string");

    printf("Parsed XML: %s\n", xml_str);

    free(xml_str);
    adb_free_xml_doc(doc);

    return ADB_TEST_SUCCESS;
}

/* Test memory tracking functionality */
adb_test_result_t test_memory_tracking(void) {
    /* Test tracked memory allocation */
    void *ptr1 = adb_tracked_malloc(100);
    ADB_ASSERT_NOT_NULL(ptr1, "Failed to allocate tracked memory");

    void *ptr2 = adb_tracked_calloc(10, 10);
    ADB_ASSERT_NOT_NULL(ptr2, "Failed to allocate tracked calloc memory");

    /* Free one pointer */
    adb_tracked_free(ptr1);

    /* Check that we still have one allocation (ptr2) */
    /* Note: We can't easily test leak detection here without more infrastructure */

    adb_tracked_free(ptr2);

    return ADB_TEST_SUCCESS;
}

/* Basic type test cases array */
adb_test_case_t adb_basic_type_tests[] = {
    {"test_string_type", "Test basic string type code generation", test_string_type},
    {"test_integer_type", "Test basic integer type code generation", test_integer_type},
    {"test_boolean_type", "Test basic boolean type code generation", test_boolean_type},
    {"test_xml_parsing", "Test XML parsing utilities", test_xml_parsing},
    {"test_memory_tracking", "Test memory tracking functionality", test_memory_tracking}
};

int adb_basic_type_test_count = sizeof(adb_basic_type_tests) / sizeof(adb_test_case_t);