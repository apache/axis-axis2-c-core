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

/* Test basic choice element handling */
adb_test_result_t test_basic_choice(void) {
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/choice_test.wsdl",
        .output_dir = "output/choice_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *choice_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:complexType name=\"ChoiceType\">\n"
        "        <xsd:choice>\n"
        "          <xsd:element name=\"stringChoice\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"intChoice\" type=\"xsd:int\"/>\n"
        "          <xsd:element name=\"booleanChoice\" type=\"xsd:boolean\"/>\n"
        "        </xsd:choice>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"ChoiceRequest\" type=\"tns:ChoiceType\"/>\n"
        "      <xsd:element name=\"ChoiceResponse\" type=\"tns:ChoiceType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"ChoiceMessage\">\n"
        "    <part name=\"body\" element=\"tns:ChoiceRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"ChoiceResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:ChoiceResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"ChoicePortType\">\n"
        "    <operation name=\"ChoiceOperation\">\n"
        "      <input message=\"tns:ChoiceMessage\"/>\n"
        "      <output message=\"tns:ChoiceResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"ChoiceBinding\" type=\"tns:ChoicePortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"ChoiceOperation\">\n"
        "      <soap:operation soapAction=\"urn:ChoiceOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"ChoiceService\">\n"
        "    <port name=\"ChoicePort\" binding=\"tns:ChoiceBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/ChoiceService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/choice_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/choice_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create choice test WSDL file");

    fputs(choice_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Choice code generation failed");

    return ADB_TEST_SUCCESS;
}

/* Test complex choice with nested elements */
adb_test_result_t test_complex_choice(void) {
    printf("Testing complex choice with nested elements...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/complex_choice_test.wsdl",
        .output_dir = "output/complex_choice_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *complex_choice_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/complex\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/complex\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/complex\">\n"
        "      <!-- Complex choice with nested sequences and arrays -->\n"
        "      <xsd:complexType name=\"ComplexChoiceType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"id\" type=\"xsd:int\"/>\n"
        "          <xsd:choice>\n"
        "            <!-- Simple choice elements -->\n"
        "            <xsd:element name=\"simpleString\" type=\"xsd:string\"/>\n"
        "            <!-- Array choice elements -->\n"
        "            <xsd:element name=\"stringArray\" type=\"xsd:string\" maxOccurs=\"unbounded\"/>\n"
        "            <xsd:element name=\"intArray\" type=\"xsd:int\" maxOccurs=\"unbounded\"/>\n"
        "            <!-- Nested complex choice -->\n"
        "            <xsd:element name=\"nestedChoice\" type=\"tns:NestedChoiceType\"/>\n"
        "          </xsd:choice>\n"
        "          <xsd:element name=\"description\" type=\"xsd:string\" minOccurs=\"0\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <!-- Nested choice type -->\n"
        "      <xsd:complexType name=\"NestedChoiceType\">\n"
        "        <xsd:choice>\n"
        "          <xsd:element name=\"innerString\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"innerInt\" type=\"xsd:int\"/>\n"
        "          <xsd:element name=\"innerArray\" type=\"xsd:string\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:choice>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:element name=\"ComplexChoiceRequest\" type=\"tns:ComplexChoiceType\"/>\n"
        "      <xsd:element name=\"ComplexChoiceResponse\" type=\"tns:ComplexChoiceType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"ComplexChoiceMessage\">\n"
        "    <part name=\"body\" element=\"tns:ComplexChoiceRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"ComplexChoiceResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:ComplexChoiceResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"ComplexChoicePortType\">\n"
        "    <operation name=\"ComplexChoiceOperation\">\n"
        "      <input message=\"tns:ComplexChoiceMessage\"/>\n"
        "      <output message=\"tns:ComplexChoiceResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"ComplexChoiceBinding\" type=\"tns:ComplexChoicePortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"ComplexChoiceOperation\">\n"
        "      <soap:operation soapAction=\"urn:ComplexChoiceOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"ComplexChoiceService\">\n"
        "    <port name=\"ComplexChoicePort\" binding=\"tns:ComplexChoiceBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/ComplexChoiceService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    if (system("mkdir -p wsdl output/complex_choice_test") != 0) {
        printf("Warning: Failed to create directories\n");
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/complex_choice_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create complex choice test WSDL file");

    fputs(complex_choice_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Complex choice code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Complex choice compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Choice test cases array */
adb_test_case_t adb_choice_tests[] = {
    {"test_basic_choice", "Test basic choice element handling", test_basic_choice},
    {"test_complex_choice", "Test complex choice with nested elements and arrays", test_complex_choice}
};

int adb_choice_test_count = sizeof(adb_choice_tests) / sizeof(adb_test_case_t);