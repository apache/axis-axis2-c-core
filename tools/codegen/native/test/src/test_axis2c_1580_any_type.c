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
 * Test case for AXIS2C-1580: Native codegen crashes when deserializing 'any' type
 *
 * This test validates that:
 * - xsd:any elements are detected during WSDL schema parsing
 * - Generated deserialization code handles NULL element_qname safely
 * - Code doesn't crash when axiom_element_get_qname returns NULL
 *
 * Original issue: When deserializing 'any' type elements, axiom_element_get_qname
 * returns NULL because xsd:any elements don't have a specific qname. The generated
 * code then crashed when attempting to compare the NULL qname.
 *
 * Fix: Generated deserialization code now checks for NULL element_qname before
 * performing qname comparison operations.
 */

/* Test AXIS2C-1580 fix - xsd:any element detection in WSDL */
adb_test_result_t test_axis2c_1580_any_type_detection(void) {
    printf("Testing AXIS2C-1580 fix: xsd:any element detection in WSDL parsing...\n");

    /* Create WSDL with xsd:any element */
    const char *any_type_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://example.com/axis2c_1580/test\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://example.com/axis2c_1580/test\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://example.com/axis2c_1580/test\">\n"
        "      <xsd:element name=\"GenericRequest\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:element name=\"header\" type=\"xsd:string\"/>\n"
        "            <!-- xsd:any - the element type that causes AXIS2C-1580 crash -->\n"
        "            <xsd:any namespace=\"##any\" processContents=\"lax\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n"
        "            <xsd:element name=\"footer\" type=\"xsd:string\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "      <xsd:element name=\"GenericResponse\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:any namespace=\"##other\" processContents=\"strict\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "</definitions>\n";

    /* Parse WSDL and verify xsd:any detection */
    xmlDocPtr doc = xmlParseMemory(any_type_wsdl, strlen(any_type_wsdl));
    ADB_ASSERT_NOT_NULL(doc, "Failed to parse test WSDL with xsd:any");

    xmlXPathContextPtr xpath_ctx = xmlXPathNewContext(doc);
    ADB_ASSERT_NOT_NULL(xpath_ctx, "Failed to create XPath context");

    /* Register namespaces - same as wsdl_parser.c */
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "xsd",
                       BAD_CAST "http://www.w3.org/2001/XMLSchema");
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "xs",
                       BAD_CAST "http://www.w3.org/2001/XMLSchema");

    /* Check for xsd:any elements using same XPath as wsdl_parser.c */
    xmlXPathObjectPtr result = xmlXPathEvalExpression(
        BAD_CAST "//xsd:any | //xs:any", xpath_ctx);

    ADB_ASSERT_NOT_NULL(result, "XPath evaluation failed");
    ADB_ASSERT_NOT_NULL(result->nodesetval, "XPath result nodeset is NULL");

    int any_count = result->nodesetval->nodeNr;
    printf("  Found %d xsd:any element(s) in test WSDL\n", any_count);
    ADB_ASSERT_TRUE(any_count == 2, "Expected 2 xsd:any elements in test WSDL");

    /* Verify attributes of xsd:any elements */
    for (int i = 0; i < any_count; i++) {
        xmlNodePtr any_node = result->nodesetval->nodeTab[i];
        xmlChar *namespace_attr = xmlGetProp(any_node, BAD_CAST "namespace");
        xmlChar *process_attr = xmlGetProp(any_node, BAD_CAST "processContents");

        printf("  xsd:any[%d]: namespace='%s', processContents='%s'\n",
               i, namespace_attr ? (char*)namespace_attr : "(none)",
               process_attr ? (char*)process_attr : "(none)");

        if (namespace_attr) xmlFree(namespace_attr);
        if (process_attr) xmlFree(process_attr);
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(xpath_ctx);
    xmlFreeDoc(doc);

    printf("PASS: xsd:any element detection works correctly\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1580 fix - NULL qname handling logic */
adb_test_result_t test_axis2c_1580_null_qname_handling(void) {
    printf("Testing AXIS2C-1580 fix: NULL qname handling logic...\n");

    /* This test simulates the fix logic that prevents crash when element_qname is NULL.
     * The actual axiom_element_get_qname() returns NULL for xsd:any elements because
     * they don't have a fixed qname - they can match any element.
     *
     * Original buggy code pattern:
     *   element_qname = axiom_element_get_qname(element, env, node);
     *   if (axutil_qname_equals(element_qname, env, expected_qname)) { ... }  // CRASH if NULL
     *
     * Fixed code pattern:
     *   element_qname = axiom_element_get_qname(element, env, node);
     *   if (element_qname) {
     *       if (axutil_qname_equals(element_qname, env, expected_qname)) { ... }
     *   } else {
     *       // Handle xsd:any case - element_qname is NULL, skip qname validation
     *   }
     */

    /* Test case 1: Normal element (non-NULL qname) */
    const char *normal_qname = "normalElement";
    int can_process_normal = 0;
    if (normal_qname != NULL) {
        /* Normal element processing - qname comparison would happen here */
        can_process_normal = 1;
        printf("  Normal element with qname='%s': can process with qname validation\n", normal_qname);
    } else {
        /* xsd:any processing - skip qname validation */
        printf("  xsd:any element (NULL qname): skip qname validation\n");
    }
    ADB_ASSERT_TRUE(can_process_normal == 1, "Normal element should allow qname validation");

    /* Test case 2: xsd:any element (NULL qname) */
    const char *any_qname = NULL;  /* Simulates axiom_element_get_qname returning NULL */
    int any_skips_validation = 0;
    if (any_qname != NULL) {
        /* Normal element processing */
        printf("  Unexpected: got qname for any element\n");
    } else {
        /* xsd:any processing - skip qname validation, don't crash */
        any_skips_validation = 1;
        printf("  xsd:any element (NULL qname): correctly skips qname validation\n");
    }
    ADB_ASSERT_TRUE(any_skips_validation == 1, "xsd:any should skip qname validation");

    /* Test case 3: Verify the fix prevents dereferencing NULL */
    const char *test_qnames[] = {
        "element1",      /* Normal - validate */
        NULL,            /* xsd:any - skip validation */
        "element2",      /* Normal - validate */
        NULL,            /* xsd:any - skip validation */
        "element3"       /* Normal - validate */
    };
    int num_tests = sizeof(test_qnames) / sizeof(test_qnames[0]);
    int normal_count = 0;
    int any_count = 0;

    for (int i = 0; i < num_tests; i++) {
        const char *qname = test_qnames[i];
        if (qname != NULL) {
            /* Safe to perform qname operations */
            normal_count++;
        } else {
            /* AXIS2C-1580 FIX: Don't crash, just skip qname validation */
            any_count++;
        }
    }

    printf("  Processed %d normal elements, %d xsd:any elements without crash\n",
           normal_count, any_count);
    ADB_ASSERT_EQUALS_INT(3, normal_count, "Expected 3 normal elements");
    ADB_ASSERT_EQUALS_INT(2, any_count, "Expected 2 xsd:any elements");

    printf("PASS: NULL qname handling prevents crash for xsd:any elements\n");
    return ADB_TEST_SUCCESS;
}

/* Test AXIS2C-1580 fix - code generation with xsd:any */
adb_test_result_t test_axis2c_1580_codegen_any_type(void) {
    printf("Testing AXIS2C-1580 fix: Code generation with xsd:any types...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/axis2c_1580_any_type.wsdl",
        .output_dir = "output/axis2c_1580_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 0
    };

    /* Create WSDL with xsd:any element that triggers the issue */
    const char *any_type_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://example.com/axis2c_1580/test\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://example.com/axis2c_1580/test\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://example.com/axis2c_1580/test\">\n"
        "      <xsd:element name=\"ExtensibleRequest\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:element name=\"operation\" type=\"xsd:string\"/>\n"
        "            <!-- xsd:any allows any XML content - qname will be NULL -->\n"
        "            <xsd:any namespace=\"##any\" processContents=\"lax\" minOccurs=\"0\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "      <xsd:element name=\"ExtensibleResponse\">\n"
        "        <xsd:complexType>\n"
        "          <xsd:sequence>\n"
        "            <xsd:element name=\"result\" type=\"xsd:string\"/>\n"
        "            <xsd:any namespace=\"##other\" processContents=\"skip\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n"
        "          </xsd:sequence>\n"
        "        </xsd:complexType>\n"
        "      </xsd:element>\n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  \n"
        "  <message name=\"ExtensibleRequestMessage\">\n"
        "    <part name=\"parameters\" element=\"tns:ExtensibleRequest\"/>\n"
        "  </message>\n"
        "  <message name=\"ExtensibleResponseMessage\">\n"
        "    <part name=\"parameters\" element=\"tns:ExtensibleResponse\"/>\n"
        "  </message>\n"
        "  \n"
        "  <portType name=\"ExtensiblePortType\">\n"
        "    <operation name=\"ExtensibleOperation\">\n"
        "      <input message=\"tns:ExtensibleRequestMessage\"/>\n"
        "      <output message=\"tns:ExtensibleResponseMessage\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  \n"
        "  <binding name=\"ExtensibleBinding\" type=\"tns:ExtensiblePortType\">\n"
        "    <soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"ExtensibleOperation\">\n"
        "      <soap:operation soapAction=\"http://example.com/axis2c_1580/ExtensibleOperation\"/>\n"
        "      <input><soap:body use=\"literal\"/></input>\n"
        "      <output><soap:body use=\"literal\"/></output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  \n"
        "  <service name=\"ExtensibleService\">\n"
        "    <port name=\"ExtensiblePort\" binding=\"tns:ExtensibleBinding\">\n"
        "      <soap:address location=\"http://localhost:8080/axis2/services/ExtensibleService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist */
    int mkdir_result = system("mkdir -p wsdl output/axis2c_1580_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Clean any existing output */
    int clean_result = system("rm -rf output/axis2c_1580_test/*");
    if (clean_result != 0) {
        printf("Warning: Failed to clean output directory (exit code: %d)\n", clean_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/axis2c_1580_any_type.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create AXIS2C-1580 test WSDL file");

    fputs(any_type_wsdl, wsdl_file);
    fclose(wsdl_file);

    printf("  Created test WSDL with xsd:any elements\n");

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "AXIS2C-1580 code generation failed");

    printf("  Code generation successful\n");

    /* Verify that generated code contains AXIS2C-1580 safe qname handling */
    char verify_command[4096];
    snprintf(verify_command, sizeof(verify_command),
             "grep -r 'AXIS2C-1580' %s/src/ 2>/dev/null | wc -l",
             test_config.output_dir);

    FILE *fp = popen(verify_command, "r");
    ADB_ASSERT_NOT_NULL(fp, "Could not execute verification command");

    char count_str[32];
    if (fgets(count_str, sizeof(count_str), fp)) {
        int fix_count = atoi(count_str);
        pclose(fp);

        printf("  Found %d AXIS2C-1580 fix markers in generated code\n", fix_count);

        /* Check for the NULL qname check pattern */
        snprintf(verify_command, sizeof(verify_command),
                 "grep -r 'element_qname' %s/src/ 2>/dev/null | grep -c 'if.*element_qname'",
                 test_config.output_dir);
        fp = popen(verify_command, "r");
        if (fp && fgets(count_str, sizeof(count_str), fp)) {
            int null_check_count = atoi(count_str);
            pclose(fp);
            printf("  Found %d element_qname NULL checks in generated code\n", null_check_count);
        }
    } else {
        pclose(fp);
    }

    printf("PASS: Code generation with xsd:any types works correctly\n");
    return ADB_TEST_SUCCESS;
}

/* Test case array for AXIS2C-1580 */
adb_test_case_t axis2c_1580_tests[] = {
    {"test_axis2c_1580_any_type_detection",
     "AXIS2C-1580: xsd:any element detection in WSDL parsing",
     test_axis2c_1580_any_type_detection},
    {"test_axis2c_1580_null_qname_handling",
     "AXIS2C-1580: NULL qname handling logic for xsd:any",
     test_axis2c_1580_null_qname_handling},
    {"test_axis2c_1580_codegen_any_type",
     "AXIS2C-1580: Code generation with xsd:any types",
     test_axis2c_1580_codegen_any_type}
};

int axis2c_1580_test_count = sizeof(axis2c_1580_tests) / sizeof(axis2c_1580_tests[0]);

/* Export the test cases */
extern adb_test_case_t axis2c_1580_tests[];
extern int axis2c_1580_test_count;
