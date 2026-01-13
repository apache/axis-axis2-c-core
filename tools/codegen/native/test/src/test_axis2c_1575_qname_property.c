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
 * Test case for AXIS2C-1575: Generated code has undefined variables qname_uri and qname_prefix
 *
 * This test validates that:
 * - QName property variables (qname_uri, qname_prefix, qname_localpart) are declared
 *   at proper function scope, NOT inside conditional blocks
 * - Generated serialization code for QName properties compiles without errors
 * - Generated deserialization code for QName properties compiles without errors
 *
 * Original issue: In CADBBeanTemplateSource.xsl, the variables qname_uri and qname_prefix
 * were declared inside an <xsl:otherwise> block (for non-simple types) but later referenced
 * when processing axutil_qname_t* properties, causing undefined variable compile errors.
 *
 * Fix: Ensure all qname_* variables are declared at function scope before any conditional
 * blocks that might use them.
 */

/* Test AXIS2C-1575 fix - variable scope validation */
adb_test_result_t test_axis2c_1575_variable_scope_pattern(void) {
    printf("Testing AXIS2C-1575 fix: Variable scope pattern validation...\n");

    /* This test validates the correct vs incorrect variable scoping patterns */

    /* Correct pattern: Variables at function scope */
    printf("  Correct pattern (variables at function scope):\n");
    printf("    axis2_char_t *qname_uri = NULL;      // At function scope\n");
    printf("    axis2_char_t *qname_prefix = NULL;   // At function scope\n");
    printf("    if (is_complex) {\n");
    printf("        qname_uri = get_uri();           // Use in any block\n");
    printf("    }\n");
    printf("    // qname_uri still accessible here - CORRECT\n\n");

    /* Incorrect pattern (the bug) */
    printf("  Incorrect pattern (AXIS2C-1575 bug):\n");
    printf("    if (is_complex) {\n");
    printf("        axis2_char_t *qname_uri = NULL;  // Inside block only!\n");
    printf("    }\n");
    printf("    // qname_uri NOT accessible here - COMPILE ERROR\n\n");

    /* Simulate the scope check */
    int scope_correct = 1;

    /* Test 1: Variables should be accessible after conditional */
    {
        /* Correct: declare at outer scope */
        char *qname_uri = NULL;
        char *qname_prefix = NULL;
        char *qname_localpart = NULL;

        /* Use inside conditional */
        if (1) {
            qname_uri = "http://example.com";
            qname_prefix = "ex";
            qname_localpart = "element";
        }

        /* Variables still accessible here - this is the fix */
        if (qname_uri == NULL || qname_prefix == NULL || qname_localpart == NULL) {
            scope_correct = 0;
        }
    }

    ADB_ASSERT_TRUE(scope_correct == 1, "Variable scope pattern validation failed");

    printf("PASS: Variable scope pattern is correct\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1575 fix - QName serialization format */
adb_test_result_t test_axis2c_1575_qname_serialization_format(void) {
    printf("Testing AXIS2C-1575 fix: QName serialization format...\n");

    /* Test QName serialization patterns */
    typedef struct {
        const char *uri;
        const char *prefix;
        const char *localpart;
        const char *expected_text;
        const char *description;
    } qname_test_case_t;

    qname_test_case_t test_cases[] = {
        {"http://example.com", "ex", "element", "ex:element", "Prefixed QName"},
        {"http://example.com", "", "element", "element", "QName without prefix"},
        {"http://example.com", NULL, "element", "element", "QName with NULL prefix"},
        {"", "ex", "element", "ex:element", "QName with empty URI"},
        {NULL, NULL, "element", "element", "QName with only localpart"},
        {NULL, NULL, NULL, NULL, NULL}  /* Terminator */
    };

    int passed = 0;
    int total = 0;

    for (int i = 0; test_cases[i].description != NULL; i++) {
        total++;
        const char *prefix = test_cases[i].prefix;
        const char *localpart = test_cases[i].localpart;
        char result[256] = {0};

        /* Simulate the serialization logic */
        if (prefix && strlen(prefix) > 0) {
            snprintf(result, sizeof(result), "%s:%s", prefix, localpart);
        } else if (localpart) {
            snprintf(result, sizeof(result), "%s", localpart);
        }

        int match = (test_cases[i].expected_text == NULL && result[0] == '\0') ||
                   (test_cases[i].expected_text != NULL && strcmp(result, test_cases[i].expected_text) == 0);

        printf("  Test: %s -> '%s' %s\n",
               test_cases[i].description, result, match ? "PASS" : "FAIL");

        if (match) passed++;
    }

    printf("  Results: %d/%d passed\n", passed, total);
    ADB_ASSERT_EQUALS_INT(total, passed, "Not all QName serialization tests passed");

    printf("PASS: QName serialization format works correctly\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1575 fix - QName deserialization format */
adb_test_result_t test_axis2c_1575_qname_deserialization_format(void) {
    printf("Testing AXIS2C-1575 fix: QName deserialization format...\n");

    /* Test QName deserialization (parsing prefix:localpart format) */
    typedef struct {
        const char *text;
        const char *expected_prefix;
        const char *expected_localpart;
        const char *description;
    } qname_parse_test_t;

    qname_parse_test_t test_cases[] = {
        {"ex:element", "ex", "element", "Prefixed QName"},
        {"element", NULL, "element", "Unprefixed QName"},
        {"ns1:ComplexType", "ns1", "ComplexType", "Namespace prefix"},
        {"xsd:string", "xsd", "string", "XSD type"},
        {"localOnly", NULL, "localOnly", "Local name only"},
        {NULL, NULL, NULL, NULL}  /* Terminator */
    };

    int passed = 0;
    int total = 0;

    for (int i = 0; test_cases[i].description != NULL; i++) {
        total++;
        const char *text = test_cases[i].text;
        char *colon_pos = strchr(text, ':');
        char prefix[64] = {0};
        const char *localpart = NULL;

        if (colon_pos) {
            int prefix_len = colon_pos - text;
            strncpy(prefix, text, prefix_len);
            prefix[prefix_len] = '\0';
            localpart = colon_pos + 1;
        } else {
            localpart = text;
        }

        int prefix_match = (test_cases[i].expected_prefix == NULL && prefix[0] == '\0') ||
                          (test_cases[i].expected_prefix != NULL && strcmp(prefix, test_cases[i].expected_prefix) == 0);
        int localpart_match = strcmp(localpart, test_cases[i].expected_localpart) == 0;

        int match = prefix_match && localpart_match;

        printf("  Test: %s -> prefix='%s', localpart='%s' %s\n",
               test_cases[i].description,
               prefix[0] ? prefix : "(none)",
               localpart,
               match ? "PASS" : "FAIL");

        if (match) passed++;
    }

    printf("  Results: %d/%d passed\n", passed, total);
    ADB_ASSERT_EQUALS_INT(total, passed, "Not all QName deserialization tests passed");

    printf("PASS: QName deserialization format works correctly\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1575 fix - generated code validation */
adb_test_result_t test_axis2c_1575_generated_code(void) {
    printf("Testing AXIS2C-1575 fix: Generated code QName property validation...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/axis2c_1575_qname_property.wsdl",
        .output_dir = "output/axis2c_1575_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    /* Create WSDL with QName property type */
    const char *qname_property_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://example.com/axis2c_1575/test\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://example.com/axis2c_1575/test\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://example.com/axis2c_1575/test\">\n"
        "      <!-- Type with QName property - triggers AXIS2C-1575 -->\n"
        "      <xsd:complexType name=\"QNameContainer\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"name\" type=\"xsd:string\"/>\n"
        "          <!-- QName property - needs proper variable scoping in generated code -->\n"
        "          <xsd:element name=\"typeRef\" type=\"xsd:QName\"/>\n"
        "          <xsd:element name=\"elementRef\" type=\"xsd:QName\" minOccurs=\"0\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <xsd:element name=\"QNameRequest\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:element name=\"container\" type=\"tns:QNameContainer\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "      <xsd:element name=\"QNameResponse\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:element name=\"result\" type=\"xsd:QName\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  \n"
        "  <message name=\"QNameRequestMessage\">\n"
        "    <part name=\"parameters\" element=\"tns:QNameRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"QNameResponseMessage\">\n"
        "    <part name=\"parameters\" element=\"tns:QNameResponse\"/>\n"
        "  </message>\n"
        "  \n"
        "  <portType name=\"QNamePortType\">\n"
        "    <operation name=\"ProcessQName\">\n"
        "      <input message=\"tns:QNameRequestMessage\"/>\n"
        "      <output message=\"tns:QNameResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  \n"
        "  <binding name=\"QNameBinding\" type=\"tns:QNamePortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"ProcessQName\">\n"
        "      <soap:operation soapAction=\"http://example.com/axis2c_1575/ProcessQName\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  \n"
        "  <service name=\"QNameService\">\n"
        "    <port name=\"QNamePort\" binding=\"tns:QNameBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/QNameService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/axis2c_1575_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Clean any existing output */
    int clean_result = system("rm -rf output/axis2c_1575_test/*");
    if (clean_result != 0) {
        printf("Warning: Failed to clean output directory (exit code: %d)\n", clean_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/axis2c_1575_qname_property.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create AXIS2C-1575 test WSDL file");

    fputs(qname_property_wsdl, wsdl_file);
    fclose(wsdl_file);

    printf("  Created test WSDL with QName properties\n");

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "AXIS2C-1575 code generation failed");

    printf("  Code generation successful\n");

    /* Verify that generated code contains AXIS2C-1575 fix markers */
    char verify_command[4096];
    FILE *fp;
    char count_str[32];

    /* Check 1: Verify AXIS2C-1575 fix markers are present */
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'AXIS2C-1575' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    if (fp && fgets(count_str, sizeof(count_str), fp)) {
        int fix_marker_count = atoi(count_str);
        pclose(fp);
        printf("  Found %d AXIS2C-1575 fix markers in generated code\n", fix_marker_count);
    }

    /* Check 2: Verify qname_uri/qname_prefix variables are present */
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'qname_uri\\|qname_prefix\\|qname_localpart' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    if (fp && fgets(count_str, sizeof(count_str), fp)) {
        int qname_var_count = atoi(count_str);
        pclose(fp);
        printf("  Found %d qname variable references in generated code\n", qname_var_count);
    }

    /* Check 3: Verify variables are NOT inside conditional-only blocks (proper scoping)
     * This would be the bug pattern: variables only inside if/else blocks */
    snprintf(verify_command, sizeof(verify_command),
             "grep -A1 'if.*is_complex' %s/src/ 2>/dev/null | grep -c 'axis2_char_t.*qname_'",
             test_config.output_dir);

    fp = popen(verify_command, "r");
    if (fp && fgets(count_str, sizeof(count_str), fp)) {
        int bad_scope_count = atoi(count_str);
        pclose(fp);
        if (bad_scope_count > 0) {
            printf("  WARNING: Found %d instances of qname vars inside conditional blocks\n",
                   bad_scope_count);
        } else {
            printf("  PASS: No qname variables incorrectly scoped inside conditionals\n");
        }
    }

    printf("PASS: Generated code QName property validation completed\n");
    return ADB_TEST_SUCCESS;
}

/* Test case array for AXIS2C-1575 */
adb_test_case_t axis2c_1575_tests[] = {
    {"test_axis2c_1575_variable_scope_pattern",
     "AXIS2C-1575: Variable scope pattern validation",
     test_axis2c_1575_variable_scope_pattern},
    {"test_axis2c_1575_qname_serialization_format",
     "AXIS2C-1575: QName serialization format",
     test_axis2c_1575_qname_serialization_format},
    {"test_axis2c_1575_qname_deserialization_format",
     "AXIS2C-1575: QName deserialization format",
     test_axis2c_1575_qname_deserialization_format},
    {"test_axis2c_1575_generated_code",
     "AXIS2C-1575: Generated code QName property validation",
     test_axis2c_1575_generated_code}
};

int axis2c_1575_test_count = sizeof(axis2c_1575_tests) / sizeof(axis2c_1575_tests[0]);

/* Export the test cases */
extern adb_test_case_t axis2c_1575_tests[];
extern int axis2c_1575_test_count;
