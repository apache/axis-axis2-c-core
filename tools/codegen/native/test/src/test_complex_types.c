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

/* Test complex type with multiple elements */
adb_test_result_t test_complex_type_basic(void) {
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/complex_type_test.wsdl",
        .output_dir = "output/complex_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *complex_type_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:complexType name=\"PersonType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"age\" type=\"xsd:int\"/>\n"
        "          <xsd:element name=\"email\" type=\"xsd:string\" minOccurs=\"0\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"PersonRequest\" type=\"tns:PersonType\"/>\n"
        "      <xsd:element name=\"PersonResponse\" type=\"tns:PersonType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"PersonMessage\">\n"
        "    <part name=\"body\" element=\"tns:PersonRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"PersonResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:PersonResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"PersonPortType\">\n"
        "    <operation name=\"PersonOperation\">\n"
        "      <input message=\"tns:PersonMessage\"/>\n"
        "      <output message=\"tns:PersonResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"PersonBinding\" type=\"tns:PersonPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"PersonOperation\">\n"
        "      <soap:operation soapAction=\"urn:PersonOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"PersonService\">\n"
        "    <port name=\"PersonPort\" binding=\"tns:PersonBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/PersonService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/complex_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/complex_type_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create complex type test WSDL file");

    fputs(complex_type_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Complex type code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Complex type compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Test nested complex types */
adb_test_result_t test_nested_complex_types(void) {
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/nested_complex_test.wsdl",
        .output_dir = "output/nested_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *nested_complex_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:complexType name=\"AddressType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"street\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"city\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"zipCode\" type=\"xsd:string\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:complexType name=\"PersonWithAddressType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"age\" type=\"xsd:int\"/>\n"
        "          <xsd:element name=\"address\" type=\"tns:AddressType\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"PersonWithAddressRequest\" type=\"tns:PersonWithAddressType\"/>\n"
        "      <xsd:element name=\"PersonWithAddressResponse\" type=\"tns:PersonWithAddressType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"PersonWithAddressMessage\">\n"
        "    <part name=\"body\" element=\"tns:PersonWithAddressRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"PersonWithAddressResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:PersonWithAddressResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"PersonWithAddressPortType\">\n"
        "    <operation name=\"PersonWithAddressOperation\">\n"
        "      <input message=\"tns:PersonWithAddressMessage\"/>\n"
        "      <output message=\"tns:PersonWithAddressResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"PersonWithAddressBinding\" type=\"tns:PersonWithAddressPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"PersonWithAddressOperation\">\n"
        "      <soap:operation soapAction=\"urn:PersonWithAddressOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"PersonWithAddressService\">\n"
        "    <port name=\"PersonWithAddressPort\" binding=\"tns:PersonWithAddressBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/PersonWithAddressService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/nested_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/nested_complex_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create nested complex test WSDL file");

    fputs(nested_complex_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Nested complex type code generation failed");

    return ADB_TEST_SUCCESS;
}

/* Test Calculator WSDL (existing test case) */
adb_test_result_t test_calculator_wsdl(void) {
    /* Use the existing Calculator WSDL that we know works */
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/Calculator.wsdl",
        .output_dir = "output/calculator_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    /* Ensure output directory exists */
    if (system("mkdir -p output/calculator_test") != 0) {
        printf("Warning: Failed to create output directory\n");
    }

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Calculator WSDL code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Calculator generated code compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Test minOccurs=0 handling */
adb_test_result_t test_optional_elements(void) {
    printf("Testing optional elements (minOccurs=0)...\n");

    /* This test verifies that the generator handles optional elements correctly */
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/optional_elements_test.wsdl",
        .output_dir = "output/optional_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *optional_elements_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:complexType name=\"OptionalFieldsType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"requiredField\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"optionalField\" type=\"xsd:string\" minOccurs=\"0\"/>\n"
        "          <xsd:element name=\"anotherOptionalField\" type=\"xsd:int\" minOccurs=\"0\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"OptionalFieldsRequest\" type=\"tns:OptionalFieldsType\"/>\n"
        "      <xsd:element name=\"OptionalFieldsResponse\" type=\"tns:OptionalFieldsType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"OptionalFieldsMessage\">\n"
        "    <part name=\"body\" element=\"tns:OptionalFieldsRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"OptionalFieldsResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:OptionalFieldsResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"OptionalFieldsPortType\">\n"
        "    <operation name=\"OptionalFieldsOperation\">\n"
        "      <input message=\"tns:OptionalFieldsMessage\"/>\n"
        "      <output message=\"tns:OptionalFieldsResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"OptionalFieldsBinding\" type=\"tns:OptionalFieldsPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"OptionalFieldsOperation\">\n"
        "      <soap:operation soapAction=\"urn:OptionalFieldsOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"OptionalFieldsService\">\n"
        "    <port name=\"OptionalFieldsPort\" binding=\"tns:OptionalFieldsBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/OptionalFieldsService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/optional_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/optional_elements_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create optional elements test WSDL file");

    fputs(optional_elements_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Optional elements code generation failed");

    return ADB_TEST_SUCCESS;
}

/* Complex type test cases array */
adb_test_case_t adb_complex_type_tests[] = {
    {"test_complex_type_basic", "Test basic complex type code generation", test_complex_type_basic},
    {"test_nested_complex_types", "Test nested complex type structures", test_nested_complex_types},
    {"test_calculator_wsdl", "Test Calculator WSDL (existing test case)", test_calculator_wsdl},
    {"test_optional_elements", "Test optional elements (minOccurs=0)", test_optional_elements}
};

int adb_complex_type_test_count = sizeof(adb_complex_type_tests) / sizeof(adb_test_case_t);