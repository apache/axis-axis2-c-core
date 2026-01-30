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
 * @file fuzz_json_parser.c
 * @brief OSS-Fuzz target for Axis2/C JSON parser
 *
 * Tests the json-c integration in Axis2/C HTTP/2 JSON mode.
 * Targets: CVE-2020-12762 (integer overflow), stack exhaustion,
 * malformed JSON handling.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axis2_json_reader.h>

/* Persistent environment for efficiency (reused across fuzzer iterations) */
static axutil_env_t *g_env = NULL;

/**
 * One-time initialization (called by libFuzzer)
 */
int LLVMFuzzerInitialize(int *argc, char ***argv)
{
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    if (!allocator) return -1;

    axutil_error_t *error = axutil_error_create(allocator);
    axutil_log_t *log = axutil_log_create(allocator, NULL, NULL);

    g_env = axutil_env_create_with_error_log(allocator, error, log);
    if (!g_env) return -1;

    /* Suppress log output during fuzzing */
    axutil_log_set_level(g_env, AXIS2_LOG_LEVEL_CRITICAL);

    return 0;
}

/**
 * Main fuzzer entry point - called millions of times with random data
 */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (!g_env || size == 0 || size > 10 * 1024 * 1024) {
        return 0;  /* Skip empty or oversized inputs */
    }

    /* Create JSON reader from arbitrary input bytes */
    axis2_json_reader_t *reader = axis2_json_reader_create_for_memory(
        g_env, (void *)data, (int)size);

    if (reader) {
        /* Attempt to parse - should handle any input without crashing */
        json_object *root = axis2_json_reader_get_root(reader, g_env);

        /* If parsing succeeded, try to access the structure */
        if (root) {
            /* Exercise various json-c operations */
            json_type type = json_object_get_type(root);
            (void)type;  /* Prevent unused warning */

            if (json_object_is_type(root, json_type_object)) {
                json_object_object_foreach(root, key, val) {
                    (void)key;
                    (void)val;
                }
            } else if (json_object_is_type(root, json_type_array)) {
                size_t len = json_object_array_length(root);
                for (size_t i = 0; i < len && i < 100; i++) {
                    json_object *item = json_object_array_get_idx(root, i);
                    (void)item;
                }
            }
        }

        axis2_json_reader_free(reader, g_env);
    }

    return 0;
}
