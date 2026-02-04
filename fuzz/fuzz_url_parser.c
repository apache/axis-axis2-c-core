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
 * @file fuzz_url_parser.c
 * @brief OSS-Fuzz target for Axis2/C URL parsing
 *
 * Tests URL parsing for:
 * - Buffer overflows in URL component extraction
 * - SSRF-style URL manipulation
 * - Malformed URL handling
 * - URL encoding/decoding edge cases
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_url.h>

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
    axutil_url_t *url = NULL;

    if (!g_env || size == 0 || size > 8192) {
        return 0;  /* Skip empty or oversized inputs */
    }

    /* Null-terminate for string functions */
    char *input = (char *)malloc(size + 1);
    if (!input) return 0;
    memcpy(input, data, size);
    input[size] = '\0';

    /* Test URL parsing */
    url = axutil_url_parse_string(g_env, input);
    if (url) {
        /* Exercise URL accessors */
        axis2_char_t *protocol = axutil_url_get_protocol(url, g_env);
        axis2_char_t *host = axutil_url_get_host(url, g_env);
        int port = axutil_url_get_port(url, g_env);
        axis2_char_t *path = axutil_url_get_path(url, g_env);

        (void)protocol;
        (void)host;
        (void)port;
        (void)path;

        /* Test URL serialization */
        axis2_char_t *url_str = axutil_url_to_external_form(url, g_env);
        if (url_str) {
            AXIS2_FREE(g_env->allocator, url_str);
        }

        axutil_url_free(url, g_env);
    }

    free(input);
    return 0;
}
