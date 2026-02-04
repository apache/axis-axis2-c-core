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
 * @brief OSS-Fuzz target for json-c parsing (HTTP/2 Pure JSON Architecture)
 *
 * This fuzzer tests the HTTP/2 JSON processing path in Axis2/C:
 * - HTTP/2 mode uses json_tokener_parse() directly from json-c
 * - Returns json_object* (no AXIOM/XML conversion)
 * - Used by axis2_h2_transport_utils.c for HTTP/2 JSON services
 *
 * For HTTP/1.1 JSON processing (JSON->AXIOM conversion),
 * see fuzz_json_reader.c which tests axis2_json_reader.
 *
 * Architecture:
 * - HTTP/2:   JSON -> json_tokener_parse() -> json_object* (this fuzzer)
 * - HTTP/1.1: JSON -> axis2_json_reader -> axiom_node_t* (fuzz_json_reader)
 *
 * Attack vectors tested:
 * - CVE-2020-12762 (integer overflow in json-c)
 * - Stack exhaustion from deeply nested structures
 * - Malformed JSON handling
 * - Memory exhaustion from large payloads
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <json-c/json.h>

/**
 * Main fuzzer entry point - called millions of times with random data
 */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    struct json_tokener *tok = NULL;
    struct json_object *root = NULL;

    if (size == 0 || size > 10 * 1024 * 1024) {
        return 0;  /* Skip empty or oversized inputs */
    }

    /* Create JSON tokener with depth limit to prevent stack exhaustion */
    tok = json_tokener_new_ex(32);  /* Max depth of 32 */
    if (!tok) {
        return 0;
    }

    /* Attempt to parse - should handle any input without crashing */
    root = json_tokener_parse_ex(tok, (const char *)data, (int)size);

    /* If parsing succeeded, exercise the structure */
    if (root) {
        /* Exercise various json-c operations */
        enum json_type type = json_object_get_type(root);
        (void)type;

        if (json_object_is_type(root, json_type_object)) {
            struct json_object_iterator it = json_object_iter_begin(root);
            struct json_object_iterator end = json_object_iter_end(root);
            int count = 0;
            while (!json_object_iter_equal(&it, &end) && count < 100) {
                const char *key = json_object_iter_peek_name(&it);
                struct json_object *val = json_object_iter_peek_value(&it);
                (void)key;
                (void)val;
                json_object_iter_next(&it);
                count++;
            }
        } else if (json_object_is_type(root, json_type_array)) {
            size_t len = json_object_array_length(root);
            for (size_t i = 0; i < len && i < 100; i++) {
                struct json_object *item = json_object_array_get_idx(root, i);
                (void)item;
            }
        }

        /* Test serialization */
        const char *str = json_object_to_json_string(root);
        (void)str;

        json_object_put(root);  /* Free JSON object */
    }

    json_tokener_free(tok);
    return 0;
}
