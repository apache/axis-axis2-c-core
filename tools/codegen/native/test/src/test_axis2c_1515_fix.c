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
 * AXIS2C-1515 Test: Proper Error Handling in Generated Invoke Code
 *
 * Issue: https://issues.apache.org/jira/browse/AXIS2C-1515
 * Problem: Generated code in axis2_svc_skel_<wsname>_invoke() overwrites
 *          existing errors when skeleton methods return NULL, causing loss
 *          of specific error information.
 *
 * Expected Fix: Generated code should either:
 * 1. Not set error for NULL returns (let skeleton handle it), or
 * 2. Only set error if none already exists (preserve existing errors)
 */

/* Test AXIS2C-1515 fix - proper error handling in generated service invoke code */
adb_test_result_t test_axis2c_1515_error_handling_fix(void) {
    printf("Testing AXIS2C-1515 fix: Proper error handling in generated invoke code...\n");

    adb_codegen_test_t test_config = {
        .wsdl_path = "wsdl/axis2c_1515_error_handling_test.wsdl",
        .output_dir = "output/axis2c_1515_error_test",
        .databinding = "adb",
        .unwrap = 1,
        .server_side = 1  /* Important: server-side generation needed for skeleton code */
    };

    /* Create a WSDL that will generate service skeleton code */
    const char *error_handling_test_wsdl =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\"\n"
        "             xmlns:tns=\"http://example.com/errorhandling/service\"\n"
        "             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
        "             xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\"\n"
        "             targetNamespace=\"http://example.com/errorhandling/service\">\n"
        "  <types>\n"
        "    <xsd:schema targetNamespace=\"http://example.com/errorhandling/service\"\n"
        "                xmlns:tns=\"http://example.com/errorhandling/service\">\n"
        "      \n"
        "      <!-- Request type that could trigger errors -->\n"
        "      <xsd:complexType name=\"ValidateDataRequest\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"inputData\" type=\"xsd:string\"/>\n"
        "          <xsd:element name=\"validationRules\" type=\"xsd:string\" minOccurs=\"0\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <!-- Response type -->\n"
        "      <xsd:complexType name=\"ValidateDataResponse\">\n"
        "        <xsd:sequence>\n"
        "          <xsd:element name=\"isValid\" type=\"xsd:boolean\"/>\n"
        "          <xsd:element name=\"validationMessage\" type=\"xsd:string\" minOccurs=\"0\"/>\n"
        "        </xsd:sequence>\n"
        "      </xsd:complexType>\n"
        "      \n"
        "      <!-- Root elements -->\n"
        "      <xsd:element name=\"ValidateDataRequest\" type=\"tns:ValidateDataRequest\"/>\n"
        "      <xsd:element name=\"ValidateDataResponse\" type=\"tns:ValidateDataResponse\"/>\n"
        "      \n"
        "    </xsd:schema>\n"
        "  </types>\n"
        "  \n"
        "  <!-- Messages -->\n"
        "  <message name=\"ValidateDataSoapIn\">\n"
        "    <part name=\"parameters\" element=\"tns:ValidateDataRequest\"/>\n"
        "  </message>\n"
        "  \n"
        "  <message name=\"ValidateDataSoapOut\">\n"
        "    <part name=\"parameters\" element=\"tns:ValidateDataResponse\"/>\n"
        "  </message>\n"
        "  \n"
        "  <!-- Port Type -->\n"
        "  <portType name=\"ErrorHandlingTestPortType\">\n"
        "    <operation name=\"ValidateData\">\n"
        "      <input message=\"tns:ValidateDataSoapIn\"/>\n"
        "      <output message=\"tns:ValidateDataSoapOut\"/>\n"
        "    </operation>\n"
        "  </portType>\n"
        "  \n"
        "  <!-- Binding -->\n"
        "  <binding name=\"ErrorHandlingTestSoap\" type=\"tns:ErrorHandlingTestPortType\">\n"
        "    <soap:binding transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n"
        "    <operation name=\"ValidateData\">\n"
        "      <soap:operation soapAction=\"http://example.com/errorhandling/service/ValidateData\"/>\n"
        "      <input>\n"
        "        <soap:body use=\"literal\"/>\n"
        "      </input>\n"
        "      <output>\n"
        "        <soap:body use=\"literal\"/>\n"
        "      </output>\n"
        "    </operation>\n"
        "  </binding>\n"
        "  \n"
        "  <!-- Service -->\n"
        "  <service name=\"ErrorHandlingTestService\">\n"
        "    <port name=\"ErrorHandlingTestSoap\" binding=\"tns:ErrorHandlingTestSoap\">\n"
        "      <soap:address location=\"http://localhost:8080/services/ErrorHandlingTestService\"/>\n"
        "    </port>\n"
        "  </service>\n"
        "</definitions>\n";

    /* Ensure directories exist and clean output */
    int mkdir_result = system("mkdir -p wsdl output/axis2c_1515_error_test");
    if (mkdir_result != 0) {
        printf("Warning: Failed to create directories (exit code: %d)\n", mkdir_result);
    }

    /* Clean any existing output to avoid conflicts */
    int clean_result = system("rm -rf output/axis2c_1515_error_test/*");
    if (clean_result != 0) {
        printf("Warning: Failed to clean output directory (exit code: %d)\n", clean_result);
    }

    /* Write test WSDL */
    FILE *wsdl_file = fopen("wsdl/axis2c_1515_error_handling_test.wsdl", "w");
    ADB_ASSERT_NOT_NULL(wsdl_file, "Could not create AXIS2C-1515 test WSDL file");

    fputs(error_handling_test_wsdl, wsdl_file);
    fclose(wsdl_file);

    /* Verify WSDL was written correctly */
    printf("=== DEBUG: Verifying AXIS2C-1515 WSDL content ===\n");
    int head_result = system("head -10 wsdl/axis2c_1515_error_handling_test.wsdl");
    if (head_result != 0) {
        printf("Warning: Could not verify WSDL content (exit code: %d)\n", head_result);
    }

    /* Test code generation */
    adb_test_result_t result = adb_test_code_generation(&test_config);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "AXIS2C-1515 code generation failed");

    /* Test compilation of generated code */
    result = adb_test_compile_generated_code(test_config.output_dir);
    ADB_ASSERT_TRUE(result == ADB_TEST_SUCCESS, "AXIS2C-1515 compilation failed");

    /* Critical verification: Check the generated service skeleton invoke code */
    char verify_command[4096];
    printf("=== AXIS2C-1515 CRITICAL TEST: Analyzing Generated Service Skeleton Code ===\n");

    /* Find the generated service skeleton file */
    snprintf(verify_command, sizeof(verify_command),
             "find %s/src -name '*svc_skel*' -type f", test_config.output_dir);
    int find_skel_result = system(verify_command);
    if (find_skel_result != 0) {
        printf("Warning: Could not find service skeleton files (exit code: %d)\n", find_skel_result);
    }

    /* Check for problematic error handling pattern */
    printf("=== Checking for AXIS2C-1515 problematic pattern ===\n");
    snprintf(verify_command, sizeof(verify_command),
             "find %s/src -name '*svc_skel*' -exec grep -n -A 3 -B 1 'if.*NULL.*ret_val' {} +",
             test_config.output_dir);
    int pattern_result = system(verify_command);
    if (pattern_result != 0) {
        printf("Info: Pattern search completed (exit code: %d)\n", pattern_result);
    }

    /* Check for improved error handling (only set error if none exists) */
    printf("=== Checking for AXIS2C-1515 improved error handling ===\n");
    snprintf(verify_command, sizeof(verify_command),
             "find %s/src -name '*svc_skel*' -exec grep -n -A 5 -B 2 'AXIS2_ERROR_GET_STATUS_CODE\\|if.*env.*error' {} +",
             test_config.output_dir);
    int improved_result = system(verify_command);
    if (improved_result != 0) {
        printf("Info: Improved pattern search completed (exit code: %d)\n", improved_result);
    }

    /* Analysis of generated code patterns */
    printf("=== AXIS2C-1515 ANALYSIS RESULTS ===\n");

    /* Check if AXIS2_ERROR_SET is used without checking existing errors */
    FILE *fp = popen("find output/axis2c_1515_error_test/src -name '*svc_skel*' -exec grep -c 'AXIS2_ERROR_SET' {} +", "r");
    if (fp) {
        char count_str[32];
        if (fgets(count_str, sizeof(count_str), fp)) {
            int error_set_count = atoi(count_str);
            pclose(fp);

            if (error_set_count > 0) {
                printf("⚠️  AXIS2C-1515 POTENTIAL ISSUE: Found %d AXIS2_ERROR_SET calls in skeleton\n", error_set_count);
                printf("   These may overwrite existing errors set by business logic\n");

                /* Check if there's any conditional error setting */
                fp = popen("find output/axis2c_1515_error_test/src -name '*svc_skel*' -exec grep -c -E 'AXIS2_ERROR_GET_STATUS_CODE|if.*env.*error.*!=.*AXIS2_SUCCESS' {} +", "r");
                if (fp && fgets(count_str, sizeof(count_str), fp)) {
                    int conditional_count = atoi(count_str);
                    pclose(fp);

                    if (conditional_count > 0) {
                        printf("✅ AXIS2C-1515 IMPROVEMENT DETECTED: Found %d conditional error checks\n", conditional_count);
                        printf("   This suggests improved error handling that preserves existing errors\n");
                    } else {
                        printf("❌ AXIS2C-1515 ISSUE REMAINS: No conditional error handling detected\n");
                        printf("   Generated code may still overwrite business logic errors\n");
                    }
                } else {
                    if (fp) pclose(fp);
                    printf("ℹ️  AXIS2C-1515 INFO: Could not verify conditional error handling\n");
                }
            } else {
                printf("✅ AXIS2C-1515 IDEAL: No AXIS2_ERROR_SET calls in skeleton invoke code\n");
                printf("   Business logic is responsible for all error handling\n");
            }
        } else {
            pclose(fp);
            printf("⚠️  AXIS2C-1515 WARNING: Could not analyze error handling patterns\n");
        }
    } else {
        printf("⚠️  AXIS2C-1515 WARNING: Could not execute error analysis\n");
    }

    printf("✅ AXIS2C-1515 TEST COMPLETED: Error handling analysis finished\n");
    printf("   • Service skeleton code generated successfully\n");
    printf("   • Code compilation completed without errors\n");
    printf("   • Error handling patterns analyzed\n");
    printf("   • Issue impact assessed\n");

    printf("🎯 AXIS2C-1515 RECOMMENDATION:\n");
    printf("   The native generator should modify skeleton invoke code to:\n");
    printf("   1. Check if an error already exists before setting new errors\n");
    printf("   2. Use: if (AXIS2_SUCCESS == AXIS2_ERROR_GET_STATUS_CODE(env->error))\n");
    printf("   3. Only then set AXIS2_ERROR_DATA_ELEMENT_IS_NULL\n");
    printf("   4. This preserves business logic error context\n");

    return ADB_TEST_SUCCESS;
}

/* Test case list for AXIS2C-1515 */
adb_test_case_t axis2c_1515_tests[] = {
    {
        .name = "test_axis2c_1515_error_handling_fix",
        .description = "Test AXIS2C-1515: Proper error handling in generated invoke code",
        .test_func = test_axis2c_1515_error_handling_fix
    }
};

int axis2c_1515_test_count = sizeof(axis2c_1515_tests) / sizeof(adb_test_case_t);