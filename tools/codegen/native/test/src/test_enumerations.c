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

/* Test basic enumeration handling */
adb_test_result_t test_basic_enumeration(void) {
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/enumeration_test.wsdl",
        .output_dir = "output/enumeration_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *enumeration_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:simpleType name=\"ColorType\">\n"
        "        <xsd:restriction base=\"xsd:string\">\n"
        "          <xsd:enumeration value=\"Red\"/>\n"
        "          <xsd:enumeration value=\"Green\"/>\n"
        "          <xsd:enumeration value=\"Blue\"/>\n"
        "        </xsd:restriction>\n"
        "      </xsd:simpleType>\n"
        "      <xsd:complexType name=\"ColoredObjectType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"color\" type=\"tns:ColorType\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"ColoredObjectRequest\" type=\"tns:ColoredObjectType\"/>\n"
        "      <xsd:element name=\"ColoredObjectResponse\" type=\"tns:ColoredObjectType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"ColoredObjectMessage\">\n"
        "    <part name=\"body\" element=\"tns:ColoredObjectRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"ColoredObjectResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:ColoredObjectResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"ColoredObjectPortType\">\n"
        "    <operation name=\"ColoredObjectOperation\">\n"
        "      <input message=\"tns:ColoredObjectMessage\"/>\n"
        "      <output message=\"tns:ColoredObjectResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"ColoredObjectBinding\" type=\"tns:ColoredObjectPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"ColoredObjectOperation\">\n"
        "      <soap:operation soapAction=\"urn:ColoredObjectOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"ColoredObjectService\">\n"
        "    <port name=\"ColoredObjectPort\" binding=\"tns:ColoredObjectBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/ColoredObjectService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/enumeration_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/enumeration_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create enumeration test WSDL file");

    fputs(enumeration_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Enumeration code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Enumeration compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Test enumeration with numeric values */
adb_test_result_t test_numeric_enumeration(void) {
    printf("Testing numeric enumeration handling...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/numeric_enumeration_test.wsdl",
        .output_dir = "output/numeric_enumeration_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *numeric_enum_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/numeric\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/numeric\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/numeric\">\n"
        "      <!-- Numeric enumeration based on integer type -->\n"
        "      <xsd:simpleType name=\"StatusCode\">\n"
        "        <xsd:restriction base=\"xsd:int\">\n"
        "          <xsd:enumeration value=\"100\"/>\n"
        "          <xsd:enumeration value=\"200\"/>\n"
        "          <xsd:enumeration value=\"404\"/>\n"
        "          <xsd:enumeration value=\"500\"/>\n"
        "        </xsd:restriction>\n"
        "      </xsd:simpleType>\n"
        "      \n"
        "      <!-- Priority levels using byte -->\n"
        "      <xsd:simpleType name=\"Priority\">\n"
        "        <xsd:restriction base=\"xsd:byte\">\n"
        "          <xsd:enumeration value=\"1\"/>\n"
        "          <xsd:enumeration value=\"2\"/>\n"
        "          <xsd:enumeration value=\"3\"/>\n"
        "          <xsd:enumeration value=\"5\"/>\n"
        "          <xsd:enumeration value=\"8\"/>\n"
        "        </xsd:restriction>\n"
        "      </xsd:simpleType>\n"
        "      \n"
        "      <xsd:complexType name=\"NumericEnumRequest\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"statusCode\" type=\"tns:StatusCode\"/>\n"
        "          <xsd:element name=\"priority\" type=\"tns:Priority\"/>\n"
        "          <xsd:element name=\"message\" type=\"xsd:string\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:element name=\"NumericEnumRequest\" type=\"tns:NumericEnumRequest\"/>\n"
        "      <xsd:element name=\"NumericEnumResponse\" type=\"tns:NumericEnumRequest\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"NumericEnumMessage\">\n"
        "    <part name=\"body\" element=\"tns:NumericEnumRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"NumericEnumResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:NumericEnumResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"NumericEnumPortType\">\n"
        "    <operation name=\"NumericEnumOperation\">\n"
        "      <input message=\"tns:NumericEnumMessage\"/>\n"
        "      <output message=\"tns:NumericEnumResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"NumericEnumBinding\" type=\"tns:NumericEnumPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"NumericEnumOperation\">\n"
        "      <soap:operation soapAction=\"urn:NumericEnumOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"NumericEnumService\">\n"
        "    <port name=\"NumericEnumPort\" binding=\"tns:NumericEnumBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/NumericEnumService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    if (system("mkdir -p wsdl output/numeric_enumeration_test") != 0) {
        printf("Warning: Failed to create directories\n");
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/numeric_enumeration_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create numeric enumeration test WSDL file");

    fputs(numeric_enum_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Numeric enumeration code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Numeric enumeration compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Enumeration test cases array */
adb_test_case_t adb_enumeration_tests[] = {
    {"test_basic_enumeration", "Test basic string enumeration", test_basic_enumeration},
    {"test_numeric_enumeration", "Test numeric enumeration handling", test_numeric_enumeration}
};

int adb_enumeration_test_count = sizeof(adb_enumeration_tests) / sizeof(adb_test_case_t);