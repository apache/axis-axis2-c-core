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

/* Test basic array handling (maxOccurs > 1) */
adb_test_result_t test_basic_array(void) {
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/array_test.wsdl",
        .output_dir = "output/array_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *array_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://test.apache.org/\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://test.apache.org/\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://test.apache.org/\">\n"
        "      <xsd:complexType name=\"StringArrayType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"values\" type=\"xsd:string\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:complexType name=\"IntArrayType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"numbers\" type=\"xsd:int\" maxOccurs=\"10\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"StringArrayRequest\" type=\"tns:StringArrayType\"/>\n"
        "      <xsd:element name=\"IntArrayRequest\" type=\"tns:IntArrayType\"/>\n"
        "      <xsd:element name=\"ArrayResponse\" type=\"xsd:string\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"StringArrayMessage\">\n"
        "    <part name=\"body\" element=\"tns:StringArrayRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"IntArrayMessage\">\n"
        "    <part name=\"body\" element=\"tns:IntArrayRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"ArrayResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:ArrayResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"ArrayPortType\">\n"
        "    <operation name=\"StringArrayOperation\">\n"
        "      <input message=\"tns:StringArrayMessage\"/>\n"
        "      <output message=\"tns:ArrayResponseMessage\"/>\n"
        "    </operation>\n"
        "    <operation name=\"IntArrayOperation\">\n"
        "      <input message=\"tns:IntArrayMessage\"/>\n"
        "      <output message=\"tns:ArrayResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"ArrayBinding\" type=\"tns:ArrayPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"StringArrayOperation\">\n"
        "      <soap:operation soapAction=\"urn:StringArrayOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "    <operation name=\"IntArrayOperation\">\n"
        "      <soap:operation soapAction=\"urn:IntArrayOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"ArrayService\">\n"
        "    <port name=\"ArrayPort\" binding=\"tns:ArrayBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/ArrayService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/array_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/array_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create array test WSDL file");

    fputs(array_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Array code generation failed");

    return ADB_TEST_SUCCESS;
}

/* Test complex object arrays */
adb_test_result_t test_complex_object_array(void) {
    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/complex_array_test.wsdl",
        .output_dir = "output/complex_array_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *complex_array_wsdl =
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
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:complexType name=\"PersonArrayType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"person\" type=\"tns:PersonType\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      <xsd:element name=\"PersonArrayRequest\" type=\"tns:PersonArrayType\"/>\n"
        "      <xsd:element name=\"PersonArrayResponse\" type=\"tns:PersonArrayType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"PersonArrayMessage\">\n"
        "    <part name=\"body\" element=\"tns:PersonArrayRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"PersonArrayResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:PersonArrayResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"PersonArrayPortType\">\n"
        "    <operation name=\"PersonArrayOperation\">\n"
        "      <input message=\"tns:PersonArrayMessage\"/>\n"
        "      <output message=\"tns:PersonArrayResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"PersonArrayBinding\" type=\"tns:PersonArrayPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"PersonArrayOperation\">\n"
        "      <soap:operation soapAction=\"urn:PersonArrayOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"PersonArrayService\">\n"
        "    <port name=\"PersonArrayPort\" binding=\"tns:PersonArrayBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/PersonArrayService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/complex_array_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/complex_array_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create complex array test WSDL file");

    fputs(complex_array_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Complex array code generation failed");

    return ADB_TEST_SUCCESS;
}

/* Test nested arrays */
adb_test_result_t test_nested_arrays(void) {
    printf("Testing nested array structures...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/nested_array_test.wsdl",
        .output_dir = "output/nested_array_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    const char *nested_array_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://soapinterop.org/xsd\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://soapinterop.org/xsd\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://soapinterop.org/xsd\">\n"
        "      <!-- Nested array structure similar to Java PopulateArrayInArrayTest -->\n"
        "      <xsd:complexType name=\"SoapStructure\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"varFloat\" type=\"xsd:float\"/>\n"
        "          <xsd:element name=\"varInt\" type=\"xsd:int\"/>\n"
        "          <!-- Array of strings within each structure -->\n"
        "          <xsd:element name=\"varString\" type=\"xsd:string\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:complexType name=\"NestedArrayType\">\n"
        "        <xsd:sequence>\n"
        "          <!-- Array of structures, each containing arrays -->\n"
        "          <xsd:element name=\"soapStructures\" type=\"tns:SoapStructure\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <!-- Multi-level nested arrays -->\n"
        "      <xsd:complexType name=\"MatrixType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"rows\" type=\"tns:RowType\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:complexType name=\"RowType\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"columns\" type=\"xsd:int\" maxOccurs=\"unbounded\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:element name=\"NestedArrayRequest\" type=\"tns:NestedArrayType\"/>\n"
        "      <xsd:element name=\"NestedArrayResponse\" type=\"tns:NestedArrayType\"/>\n"
        "      <xsd:element name=\"MatrixRequest\" type=\"tns:MatrixType\"/>\n"
        "      <xsd:element name=\"MatrixResponse\" type=\"tns:MatrixType\"/>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  <message name=\"NestedArrayMessage\">\n"
        "    <part name=\"body\" element=\"tns:NestedArrayRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"NestedArrayResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:NestedArrayResponse\"/>\n"
        "  </message>\n"
        "  <message name=\"MatrixMessage\">\n"
        "    <part name=\"body\" element=\"tns:MatrixRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"MatrixResponseMessage\">\n"
        "    <part name=\"body\" element=\"tns:MatrixResponse\"/>\n"
        "  </message>\n"
        "  <portType name=\"NestedArrayPortType\">\n"
        "    <operation name=\"NestedArrayOperation\">\n"
        "      <input message=\"tns:NestedArrayMessage\"/>\n"
        "      <output message=\"tns:NestedArrayResponseMessage\"/>\n"
        "    </operation>\n"
        "    <operation name=\"MatrixOperation\">\n"
        "      <input message=\"tns:MatrixMessage\"/>\n"
        "      <output message=\"tns:MatrixResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  <binding name=\"NestedArrayBinding\" type=\"tns:NestedArrayPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"NestedArrayOperation\">\n"
        "      <soap:operation soapAction=\"urn:NestedArrayOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "    <operation name=\"MatrixOperation\">\n"
        "      <soap:operation soapAction=\"urn:MatrixOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  <service name=\"NestedArrayService\">\n"
        "    <port name=\"NestedArrayPort\" binding=\"tns:NestedArrayBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/NestedArrayService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    if (system("mkdir -p wsdl output/nested_array_test") != 0) {
        printf("Warning: Failed to create directories\n");
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/nested_array_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create nested array test WSDL file");

    fputs(nested_array_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Nested array code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "Nested array compilation failed");

    return ADB_TEST_SUCCESS;
}

/* Array test cases array */
adb_test_case_t adb_array_tests[] = {
    {"test_basic_array", "Test basic array handling (maxOccurs > 1)", test_basic_array},
    {"test_complex_object_array", "Test complex object arrays", test_complex_object_array},
    {"test_nested_arrays", "Test nested array structures (array-in-array)", test_nested_arrays}
};

int adb_array_test_count = sizeof(adb_array_tests) / sizeof(adb_test_case_t);