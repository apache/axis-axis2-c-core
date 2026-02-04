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
 * @file fuzz_http_header.c
 * @brief OSS-Fuzz target for Axis2/C HTTP header parser
 *
 * Tests HTTP/1.1 header parsing for buffer overflows, header injection,
 * and malformed input handling.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axis2_http_header.h>
#include <axis2_http_request_line.h>
#include <axis2_http_status_line.h>

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
    if (!g_env || size == 0 || size > 64 * 1024) {
        return 0;  /* Skip empty or oversized inputs */
    }

    /* Null-terminate the input for string functions */
    char *input = (char *)malloc(size + 1);
    if (!input) return 0;
    memcpy(input, data, size);
    input[size] = '\0';

    /* Test HTTP header parsing */
    axis2_http_header_t *header = axis2_http_header_create_by_str(g_env, input);
    if (header) {
        /* Exercise header accessors */
        const axis2_char_t *name = axis2_http_header_get_name(header, g_env);
        const axis2_char_t *value = axis2_http_header_get_value(header, g_env);
        (void)name;
        (void)value;

        /* Test serialization */
        axis2_char_t *str = axis2_http_header_to_external_form(header, g_env);
        if (str) {
            AXIS2_FREE(g_env->allocator, str);
        }

        axis2_http_header_free(header, g_env);
    }

    /* Test HTTP request line parsing */
    axis2_http_request_line_t *req_line = axis2_http_request_line_parse_line(
        g_env, input);
    if (req_line) {
        const axis2_char_t *method = axis2_http_request_line_get_method(
            req_line, g_env);
        const axis2_char_t *uri = axis2_http_request_line_get_uri(
            req_line, g_env);
        const axis2_char_t *version = axis2_http_request_line_get_http_version(
            req_line, g_env);
        (void)method;
        (void)uri;
        (void)version;

        axis2_http_request_line_free(req_line, g_env);
    }

    /* Test HTTP status line parsing */
    axis2_http_status_line_t *status_line = axis2_http_status_line_create(
        g_env, input);
    if (status_line) {
        int code = axis2_http_status_line_get_status_code(status_line, g_env);
        const axis2_char_t *reason = axis2_http_status_line_get_reason_phrase(
            status_line, g_env);
        (void)code;
        (void)reason;

        axis2_http_status_line_free(status_line, g_env);
    }

    free(input);
    return 0;
}
