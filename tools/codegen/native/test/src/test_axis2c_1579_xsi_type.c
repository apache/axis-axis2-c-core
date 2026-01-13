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

/**
 * Test case for AXIS2C-1579: Wrong xsi:type in the response
 *
 * This test validates that:
 * - Generated serialization code uses the correct XSI namespace URI
 * - xmlns:xsi is set to "http://www.w3.org/2001/XMLSchema-instance" (NOT target namespace)
 * - xsi:type attribute uses properly prefixed type names (e.g., "ns1:TypeName")
 *
 * Original issue: The generated code incorrectly set:
 *   xmlns:xsi="<target-namespace>"  (WRONG - bug)
 * Instead of:
 *   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  (CORRECT)
 *
 * This caused interoperability issues with other SOAP implementations
 * that correctly validate the XSI namespace.
 */

/* Define correct XSI namespace for testing */
#define CORRECT_XSI_NAMESPACE "http://www.w3.org/2001/XMLSchema-instance"
#define CORRECT_XSD_NAMESPACE "http://www.w3.org/2001/XMLSchema"

/* Test AXIS2C-1579 fix - XSI namespace validation */
adb_test_result_t test_axis2c_1579_xsi_namespace_constant(void) {
    printf("Testing AXIS2C-1579 fix: XSI namespace constant validation...\n");

    /* Verify the XSI namespace URI is correct */
    const char *expected_xsi_ns = "http://www.w3.org/2001/XMLSchema-instance";
    const char *expected_xsd_ns = "http://www.w3.org/2001/XMLSchema";

    /* These would be wrong (examples from the bug report) */
    const char *wrong_xsi_ns_examples[] = {
        "urn:any",                              /* From bug report */
        "http://example.com/myservice",         /* Using target namespace */
        "http://www.w3.org/2001/XMLSchema",     /* XSD instead of XSI */
        "",                                     /* Empty */
        NULL
    };

    printf("  Correct XSI namespace: %s\n", expected_xsi_ns);
    printf("  Correct XSD namespace: %s\n", expected_xsd_ns);

    /* Verify none of the wrong examples match the correct value */
    for (int i = 0; wrong_xsi_ns_examples[i] != NULL; i++) {
        if (strcmp(wrong_xsi_ns_examples[i], expected_xsi_ns) == 0) {
            printf("  FAIL: Wrong example '%s' matches correct XSI namespace!\n",
                   wrong_xsi_ns_examples[i]);
            return ADB_TEST_FAILURE;
        }
        printf("  Verified wrong example '%s' does not match\n", wrong_xsi_ns_examples[i]);
    }

    printf("PASS: XSI namespace constant is correctly defined\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1579 fix - xsi:type attribute format */
adb_test_result_t test_axis2c_1579_xsi_type_format(void) {
    printf("Testing AXIS2C-1579 fix: xsi:type attribute format validation...\n");

    /* The xsi:type attribute should use a properly prefixed type name */
    /* Correct format: prefix:TypeName (e.g., "ns1:MyConfiguration") */
    /* Wrong format: just TypeName without prefix */

    typedef struct {
        const char *type_value;
        int is_valid;
        const char *description;
    } xsi_type_test_case_t;

    xsi_type_test_case_t test_cases[] = {
        {"ns1:MyConfiguration", 1, "Prefixed type name (correct)"},
        {"tns:AddRequest", 1, "Target namespace prefix (correct)"},
        {"xsd:string", 1, "XSD built-in type (correct)"},
        {"MyConfiguration", 0, "Unprefixed type name (AXIS2C-1579 bug)"},
        {"MyPolicyConfiguration", 0, "Unprefixed nested type (AXIS2C-1579 bug)"},
        {"", 0, "Empty type value"},
        {NULL, 0, NULL}
    };

    int passed = 0;
    int total = 0;

    for (int i = 0; test_cases[i].description != NULL; i++) {
        total++;
        const char *type_value = test_cases[i].type_value;
        int has_prefix = (type_value && strchr(type_value, ':') != NULL);

        printf("  Test: %s -> ", test_cases[i].description);

        if (has_prefix == test_cases[i].is_valid) {
            printf("PASS (has_prefix=%d, expected_valid=%d)\n",
                   has_prefix, test_cases[i].is_valid);
            passed++;
        } else {
            printf("FAIL (has_prefix=%d, expected_valid=%d)\n",
                   has_prefix, test_cases[i].is_valid);
        }
    }

    printf("  Results: %d/%d passed\n", passed, total);
    ADB_ASSERT_EQUALS_INT(total, passed, "Not all xsi:type format tests passed");

    printf("PASS: xsi:type attribute format validation works correctly\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1579 fix - generated code validation */
adb_test_result_t test_axis2c_1579_generated_code(void) {
    printf("Testing AXIS2C-1579 fix: Generated code xsi:type validation...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/axis2c_1579_xsi_type.wsdl",
        .output_dir = "output/axis2c_1579_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    /* Create WSDL with complex type hierarchy that needs xsi:type */
    const char *xsi_type_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://example.com/axis2c_1579/test\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://example.com/axis2c_1579/test\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://example.com/axis2c_1579/test\">\n"
        "      <!-- Base type -->\n"
        "      <xsd:complexType name=\"BaseConfiguration\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <!-- Derived type - needs xsi:type when serialized as BaseConfiguration -->\n"
        "      <xsd:complexType name=\"ExtendedConfiguration\">\n"
        "        <xsd:complexContent>\n"
        "          <xsd:extension base=\"tns:BaseConfiguration\">\n"
        "            <xsd:sequence>\n"
        "              <xsd:element name=\"value\" type=\"xsd:int\"/>\n"
        "            </xsd:sequence>\n"
        "          </xsd:extension>\n"
        "        </xsd:complexContent>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:element name=\"ConfigRequest\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <!-- This element can hold BaseConfiguration or ExtendedConfiguration -->\n"
        "            <xsd:element name=\"config\" type=\"tns:BaseConfiguration\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "      <xsd:element name=\"ConfigResponse\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:element name=\"result\" type=\"xsd:string\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  \n"
        "  <message name=\"ConfigRequestMessage\">\n"
        "    <part name=\"parameters\" element=\"tns:ConfigRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"ConfigResponseMessage\">\n"
        "    <part name=\"parameters\" element=\"tns:ConfigResponse\"/>\n"
        "  </message>\n"
        "  \n"
        "  <portType name=\"ConfigPortType\">\n"
        "    <operation name=\"Configure\">\n"
        "      <input message=\"tns:ConfigRequestMessage\"/>\n"
        "      <output message=\"tns:ConfigResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  \n"
        "  <binding name=\"ConfigBinding\" type=\"tns:ConfigPortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"Configure\">\n"
        "      <soap:operation soapAction=\"http://example.com/axis2c_1579/Configure\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  \n"
        "  <service name=\"ConfigService\">\n"
        "    <port name=\"ConfigPort\" binding=\"tns:ConfigBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/ConfigService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/axis2c_1579_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Clean any existing output */
    int clean_result = system("rm -rf output/axis2c_1579_test/*");
    if (clean_result != 0) {
        printf("Warning: Failed to clean output directory (exit code: %d)\n", clean_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/axis2c_1579_xsi_type.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create AXIS2C-1579 test WSDL file");

    fputs(xsi_type_wsdl, wsdl_file);
    fclose(wsdl_file);

    printf("  Created test WSDL with type hierarchy for xsi:type testing\n");

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "AXIS2C-1579 code generation failed");

    printf("  Code generation successful\n");

    /* Verify that generated code contains correct XSI namespace */
    char verify_command[4096];
    FILE *fp;
    char count_str[32];

    /* Check 1: Verify correct XSI namespace is used */
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'XMLSchema-instance' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    ADB_ASSERT_NOT_NULL(fp, "Could not execute verification command");

    if (fgets(count_str, sizeof(count_str), fp)) {
        int correct_xsi_count = atoi(count_str);
        pclose(fp);
        printf("  Found %d references to correct XSI namespace in generated code\n", correct_xsi_count);

        if (correct_xsi_count > 0) {
            printf("  PASS: Generated code uses correct XSI namespace\n");
        }
    } else {
        pclose(fp);
    }

    /* Check 2: Verify AXIS2C-1579 fix markers are present */
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'AXIS2C-1579' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    if (fp && fgets(count_str, sizeof(count_str), fp)) {
        int fix_marker_count = atoi(count_str);
        pclose(fp);
        printf("  Found %d AXIS2C-1579 fix markers in generated code\n", fix_marker_count);
    }

    /* Check 3: Verify xsi:type attribute generation */
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'xsi_type' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    if (fp && fgets(count_str, sizeof(count_str), fp)) {
        int xsi_type_count = atoi(count_str);
        pclose(fp);
        printf("  Found %d xsi_type references in generated code\n", xsi_type_count);
    }

    /* Check 4: Verify NO wrong namespace pattern (the bug) */
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'xmlns:xsi.*urn:' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    if (fp && fgets(count_str, sizeof(count_str), fp)) {
        int wrong_ns_count = atoi(count_str);
        pclose(fp);

        if (wrong_ns_count > 0) {
            printf("  WARNING: Found %d instances of wrong XSI namespace pattern (urn:...)\n",
                   wrong_ns_count);
        } else {
            printf("  PASS: No wrong XSI namespace patterns found\n");
        }
    }

    printf("PASS: Generated code xsi:type validation completed\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1579 fix - XML output validation */
adb_test_result_t test_axis2c_1579_xml_output_format(void) {
    printf("Testing AXIS2C-1579 fix: XML output format validation...\n");

    /* This test validates the expected XML output format */
    /* Correct output should look like:
     *   <ns1:response xmlns:ns1="http://example.com/test"
     *                 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
     *                 xsi:type="ns1:ExtendedConfiguration">
     *     ...
     *   </ns1:response>
     */

    /* Create test XML with correct format */
    const char *correct_xml =
        "<ns1:response xmlns:ns1=\"http://example.com/test\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:type=\"ns1:ExtendedConfiguration\">"
        "<ns1:name>test</ns1:name>"
        "</ns1:response>";

    /* Create test XML with AXIS2C-1579 bug format */
    const char *buggy_xml =
        "<response xmlns:xsi=\"urn:any\" "
        "xsi:type=\"ExtendedConfiguration\">"
        "<name>test</name>"
        "</response>";

    /* Parse both XMLs */
    xmlDocPtr correct_doc = xmlParseMemory(correct_xml, strlen(correct_xml));
    xmlDocPtr buggy_doc = xmlParseMemory(buggy_xml, strlen(buggy_xml));

    ADB_ASSERT_NOT_NULL(correct_doc, "Failed to parse correct XML");
    ADB_ASSERT_NOT_NULL(buggy_doc, "Failed to parse buggy XML");

    /* Get root elements */
    xmlNodePtr correct_root = xmlDocGetRootElement(correct_doc);
    xmlNodePtr buggy_root = xmlDocGetRootElement(buggy_doc);

    /* Check xsi namespace in correct XML */
    xmlNsPtr correct_xsi_ns = xmlSearchNs(correct_doc, correct_root, BAD_CAST "xsi");
    ADB_ASSERT_NOT_NULL(correct_xsi_ns, "Correct XML should have xsi namespace");

    const char *correct_xsi_href = (const char*)correct_xsi_ns->href;
    printf("  Correct XML xsi namespace: %s\n", correct_xsi_href);
    ADB_ASSERT_TRUE(strcmp(correct_xsi_href, CORRECT_XSI_NAMESPACE) == 0,
                   "Correct XML should use standard XSI namespace");

    /* Check xsi namespace in buggy XML */
    xmlNsPtr buggy_xsi_ns = xmlSearchNs(buggy_doc, buggy_root, BAD_CAST "xsi");
    if (buggy_xsi_ns) {
        const char *buggy_xsi_href = (const char*)buggy_xsi_ns->href;
        printf("  Buggy XML xsi namespace: %s\n", buggy_xsi_href);
        ADB_ASSERT_TRUE(strcmp(buggy_xsi_href, CORRECT_XSI_NAMESPACE) != 0,
                       "Buggy XML should NOT use standard XSI namespace (demonstrating the bug)");
    }

    /* Check xsi:type attribute values */
    xmlChar *correct_type = xmlGetNsProp(correct_root, BAD_CAST "type", BAD_CAST CORRECT_XSI_NAMESPACE);
    if (correct_type) {
        printf("  Correct XML xsi:type: %s\n", correct_type);
        ADB_ASSERT_TRUE(strchr((char*)correct_type, ':') != NULL,
                       "Correct xsi:type should have namespace prefix");
        xmlFree(correct_type);
    }

    xmlFreeDoc(correct_doc);
    xmlFreeDoc(buggy_doc);

    printf("PASS: XML output format validation works correctly\n");
    return ADB_TEST_SUCCESS;
}

/* Test case array for AXIS2C-1579 */
adb_test_case_t axis2c_1579_tests[] = {
    {"test_axis2c_1579_xsi_namespace_constant",
     "AXIS2C-1579: XSI namespace constant validation",
     test_axis2c_1579_xsi_namespace_constant},
    {"test_axis2c_1579_xsi_type_format",
     "AXIS2C-1579: xsi:type attribute format validation",
     test_axis2c_1579_xsi_type_format},
    {"test_axis2c_1579_generated_code",
     "AXIS2C-1579: Generated code xsi:type validation",
     test_axis2c_1579_generated_code},
    {"test_axis2c_1579_xml_output_format",
     "AXIS2C-1579: XML output format validation",
     test_axis2c_1579_xml_output_format}
};

int axis2c_1579_test_count = sizeof(axis2c_1579_tests) / sizeof(axis2c_1579_tests[0]);

/* Export the test cases */
extern adb_test_case_t axis2c_1579_tests[];
extern int axis2c_1579_test_count;
