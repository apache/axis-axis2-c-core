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
 * @file fuzz_xml_parser.c
 * @brief OSS-Fuzz target for Axis2/C XML/SOAP parsing via AXIOM
 *
 * Tests AXIOM XML parsing for memory safety issues including:
 * - Buffer overflows in XML element/attribute handling
 * - XXE (XML External Entity) injection attempts
 * - Billion laughs / XML bomb attacks
 * - Malformed XML handling
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axiom.h>
#include <axiom_xml_reader.h>
#include <axiom_stax_builder.h>

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
    axiom_xml_reader_t *xml_reader = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_document_t *document = NULL;
    axiom_node_t *root_node = NULL;

    if (!g_env || size == 0 || size > 1024 * 1024) {
        return 0;  /* Skip empty or oversized inputs */
    }

    /* Create XML reader from memory buffer */
    xml_reader = axiom_xml_reader_create_for_memory(
        g_env, (void *)data, (int)size, NULL, AXIS2_XML_PARSER_TYPE_BUFFER);

    if (!xml_reader) {
        return 0;
    }

    /* Create StAX builder */
    builder = axiom_stax_builder_create(g_env, xml_reader);
    if (!builder) {
        axiom_xml_reader_free(xml_reader, g_env);
        return 0;
    }

    /* Get document and try to parse */
    document = axiom_stax_builder_get_document(builder, g_env);
    if (document) {
        /* Try to get root element - this triggers full parsing */
        root_node = axiom_document_get_root_element(document, g_env);

        if (root_node) {
            /* Exercise the parsed tree */
            axiom_element_t *root_elem = NULL;
            root_elem = axiom_node_get_data_element(root_node, g_env);

            if (root_elem) {
                /* Get element name and namespace */
                axis2_char_t *name = axiom_element_get_localname(root_elem, g_env);
                axiom_namespace_t *ns = axiom_element_get_namespace(
                    root_elem, g_env, root_node);
                (void)name;
                (void)ns;

                /* Iterate children */
                axiom_node_t *child = axiom_node_get_first_child(root_node, g_env);
                int child_count = 0;
                while (child && child_count < 100) {
                    child = axiom_node_get_next_sibling(child, g_env);
                    child_count++;
                }

                /* Try serialization */
                axis2_char_t *xml_str = axiom_node_to_string(root_node, g_env);
                if (xml_str) {
                    AXIS2_FREE(g_env->allocator, xml_str);
                }
            }
        }
    }

    /* Cleanup - builder frees the reader */
    axiom_stax_builder_free(builder, g_env);

    return 0;
}
