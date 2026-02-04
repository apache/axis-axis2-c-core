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

/**
 * @file fuzz_json_reader.c
 * @brief OSS-Fuzz target for Axis2/C JSON Reader (HTTP/1.1 JSON processing)
 *
 * This fuzzer tests the HTTP/1.1 JSON processing path in Axis2/C:
 * - axis2_json_reader converts JSON to AXIOM XML tree representation
 * - Returns axiom_node_t* (NOT json_object*) from get_root_node()
 * - Used when JSON services are accessed via HTTP/1.1
 *
 * For HTTP/2 JSON processing (direct json-c, no AXIOM conversion),
 * see fuzz_json_parser.c which tests json_tokener_parse() directly.
 *
 * Architecture:
 * - HTTP/1.1: JSON -> axis2_json_reader -> axiom_node_t* (this fuzzer)
 * - HTTP/2:   JSON -> json_tokener_parse() -> json_object* (fuzz_json_parser)
 *
 * Attack vectors tested:
 * - Deeply nested JSON (stack exhaustion during AXIOM tree building)
 * - Large payloads (memory exhaustion, CVE-2020-12762)
 * - Malformed JSON structures
 * - Unicode and encoding edge cases
 * - NULL pointer handling in conversion functions
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>

/* Axis2/C JSON reader API (HTTP/1.1 path - converts JSON to AXIOM) */
#include <axis2_json_reader.h>

static axutil_env_t *g_env = NULL;

int LLVMFuzzerInitialize(int *argc, char ***argv)
{
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    if (!allocator) return -1;

    axutil_error_t *error = axutil_error_create(allocator);
    axutil_log_t *log = axutil_log_create(allocator, NULL, NULL);

    g_env = axutil_env_create_with_error_log(allocator, error, log);
    if (!g_env) return -1;

    /* Suppress log output during fuzzing */
    if (g_env->log) {
        g_env->log->level = AXIS2_LOG_LEVEL_CRITICAL;
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    axis2_json_reader_t *reader = NULL;

    if (!g_env || size == 0) {
        return 0;
    }

    /* Limit input size to prevent excessive memory usage
     * (Axis2/C default is 10MB, use smaller for fuzzing efficiency) */
    if (size > 1 * 1024 * 1024) {
        return 0;
    }

    /* Null-terminate for string functions */
    char *input = (char *)malloc(size + 1);
    if (!input) return 0;
    memcpy(input, data, size);
    input[size] = '\0';

    /* Test JSON reader creation from memory buffer
     * This is the HTTP/1.1 JSON processing entry point */
    reader = axis2_json_reader_create_for_memory(g_env, input, size);

    if (reader) {
        /* If reader creation succeeded, test reading/parsing
         * This converts JSON to AXIOM XML tree internally */
        axis2_status_t status = axis2_json_reader_read(reader, g_env);

        if (status == AXIS2_SUCCESS) {
            /* Get the parsed result - this is an axiom_node_t* (AXIOM XML tree)
             * NOT a json_object*. The JSON->AXIOM conversion was tested by
             * axis2_json_reader_read() succeeding.
             *
             * Note: The returned pointer is owned by the reader - do not free
             * it separately. axis2_json_reader_free() will free the tree.
             *
             * For HTTP/2 Pure JSON mode which returns json_object*,
             * see fuzz_json_parser.c which tests json-c directly. */
            void *root = axis2_json_reader_get_root_node(reader, g_env);
            (void)root; /* Verify we can get the root without crashing */
        }

        /* Free the reader and its AXIOM tree */
        axis2_json_reader_free(reader, g_env);
    }

    free(input);
    return 0;
}
