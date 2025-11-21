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

#include "wsdl2c_native.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    axutil_env_t *env = NULL;
    wsdl2c_context_t *context = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    int exit_code = 1;

    /* Initialize libxml2 */
    xmlInitParser();
    LIBXML_TEST_VERSION;

    /* Handle no arguments case */
    if (argc < 2) {
        wsdl2c_print_usage();
        exit_code = 1;
        goto cleanup;
    }

    /* Create environment */
    env = axutil_env_create_all("wsdl2c-native.log", AXIS2_LOG_LEVEL_INFO);
    if (!env) {
        fprintf(stderr, "Error: Failed to create environment\n");
        goto cleanup;
    }

    /* Create context */
    context = wsdl2c_context_create(env);
    if (!context) {
        fprintf(stderr, "Error: Failed to create WSDL2C context\n");
        goto cleanup;
    }

    /* Parse command line options */
    status = wsdl2c_parse_options(context, argc, argv, env);
    if (status != AXIS2_SUCCESS) {
        /* Options parser already printed error or help */
        exit_code = (status == AXIS2_SUCCESS) ? 0 : 1;
        goto cleanup;
    }

    /* Display configuration */
    printf("Native WSDL2C Code Generator (targeting Axis2/Java 2.0.1-SNAPSHOT compatibility)\n");
    printf("WSDL URI: %s\n", context->options->wsdl_uri);
    printf("Output Directory: %s\n", context->options->output_dir);
    printf("Data Binding: %s\n", context->options->databinding);
    printf("Unwrap: %s\n", context->options->unwrap ? "true" : "false");
    if (context->options->server_side) {
        printf("Server Side: true\n");
    }
    if (context->options->service_descriptor) {
        printf("Service Descriptor: true\n");
    }
    printf("\n");

    /* Parse WSDL */
    printf("Parsing WSDL...\n");
    status = wsdl2c_parse_wsdl(context, env);
    if (status != AXIS2_SUCCESS) {
        fprintf(stderr, "Error: Failed to parse WSDL\n");
        goto cleanup;
    }
    printf("WSDL parsing completed successfully\n");

    /* Generate code */
    printf("Generating C code...\n");
    status = wsdl2c_generate_code(context, env);
    if (status != AXIS2_SUCCESS) {
        fprintf(stderr, "Error: Failed to generate code\n");
        goto cleanup;
    }

    printf("Code generation completed successfully\n");
    exit_code = 0;

cleanup:
    if (context) {
        wsdl2c_context_free(context, env);
    }
    if (env) {
        axutil_env_free(env);
    }
    xmlCleanupParser();
    return exit_code;
}