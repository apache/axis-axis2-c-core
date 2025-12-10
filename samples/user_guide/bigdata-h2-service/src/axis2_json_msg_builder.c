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
 * @file axis2_json_msg_builder.c
 * @brief JSON Message Builder Implementation - Following Axis2/Java Pattern
 *
 * Axis2/Java JSONBuilder Architecture:
 * 1. Read JSON from HTTP stream
 * 2. Create dummy SOAP envelope (minimal AXIOM)
 * 3. Store JSON stream as MessageContext property
 * 4. Set IS_JSON_STREAM=true for message receivers
 * 5. Message receivers process JSON directly from properties
 *
 * Performance: No JSON→XML conversion, minimal AXIOM usage
 */

#include "axis2_json_msg_builder.h"
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>
#include <axiom_element.h>
#include <axiom_text.h>
#include <axiom_namespace.h>
#include <axutil_property.h>
#include <json-c/json.h>
#include <string.h>

/**
 * @brief JSON Message Builder structure
 * Following Axis2/C message builder pattern
 */
struct axis2_json_msg_builder
{
    axis2_msg_builder_t msg_builder;
};

/* Forward declarations */
static axiom_soap_envelope_t* AXIS2_CALL
json_msg_builder_process_document(
    axis2_msg_builder_t* msg_builder,
    const axutil_env_t* env,
    axutil_stream_t* stream,
    const axis2_char_t* content_type,
    axis2_msg_ctx_t* msg_ctx);

static void AXIS2_CALL
json_msg_builder_free(
    axis2_msg_builder_t* msg_builder,
    const axutil_env_t* env);

/**
 * @brief Create dummy SOAP envelope (minimal AXIOM usage)
 * Following Axis2/Java JSONBuilder pattern
 */
static axiom_soap_envelope_t*
create_dummy_soap_envelope(const axutil_env_t* env)
{
    axiom_soap_envelope_t* soap_envelope = NULL;
    axiom_soap_body_t* soap_body = NULL;
    axiom_element_t* body_element = NULL;
    axiom_node_t* body_node = NULL;
    axiom_namespace_t* soap_ns = NULL;

    /* Create minimal SOAP 1.2 envelope */
    soap_ns = axiom_namespace_create(env, AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI,
                                    AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_PREFIX);

    soap_envelope = axiom_soap_envelope_create(env, soap_ns);
    if (!soap_envelope) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create dummy SOAP envelope");
        return NULL;
    }

    /* Create empty SOAP body */
    soap_body = axiom_soap_body_create(env, soap_envelope);
    if (!soap_body) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create SOAP body");
        return NULL;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Created dummy SOAP envelope for JSON processing");
    return soap_envelope;
}

/**
 * @brief Create JSON Message Builder
 * Following Axis2/Java JSONBuilder creation pattern
 */
AXIS2_EXTERN axis2_msg_builder_t* AXIS2_CALL
axis2_json_msg_builder_create(const axutil_env_t* env)
{
    struct axis2_json_msg_builder* json_builder = NULL;
    axis2_msg_builder_t* msg_builder = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Creating JSON Message Builder");

    json_builder = (struct axis2_json_msg_builder*)AXIS2_MALLOC(env->allocator,
                                                               sizeof(struct axis2_json_msg_builder));
    if (!json_builder) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    memset(json_builder, 0, sizeof(struct axis2_json_msg_builder));

    msg_builder = &(json_builder->msg_builder);

    /* Set function pointers following Axis2/C pattern */
    axis2_msg_builder_set_process_document(msg_builder, env, json_msg_builder_process_document);
    axis2_msg_builder_set_free(msg_builder, env, json_msg_builder_free);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "JSON Message Builder created successfully");
    return msg_builder;
}

/**
 * @brief Process JSON document - Core Axis2/Java JSONBuilder logic
 * Creates dummy SOAP envelope and stores JSON in MessageContext properties
 */
static axiom_soap_envelope_t* AXIS2_CALL
json_msg_builder_process_document(
    axis2_msg_builder_t* msg_builder,
    const axutil_env_t* env,
    axutil_stream_t* stream,
    const axis2_char_t* content_type,
    axis2_msg_ctx_t* msg_ctx)
{
    axiom_soap_envelope_t* soap_envelope = NULL;
    axis2_char_t* json_string = NULL;
    int stream_len = 0;
    int bytes_read = 0;
    axutil_property_t* json_stream_prop = NULL;
    axutil_property_t* is_json_prop = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "JSON Message Builder: Processing JSON document (content-type: %s)",
                   content_type ? content_type : "unknown");

    if (!stream) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON stream is null");
        return NULL;
    }

    /* Read JSON from stream */
    stream_len = axutil_stream_get_len(stream, env);
    if (stream_len <= 0) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Empty JSON stream");
        return NULL;
    }

    json_string = (axis2_char_t*)AXIS2_MALLOC(env->allocator, stream_len + 1);
    if (!json_string) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    bytes_read = axutil_stream_read(stream, env, json_string, stream_len);
    json_string[bytes_read] = '\0';

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "JSON Message Builder: Read %d bytes of JSON data", bytes_read);

    /* Validate JSON format */
    json_object* json_obj = json_tokener_parse(json_string);
    if (!json_obj) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid JSON format");
        AXIS2_FREE(env->allocator, json_string);
        return NULL;
    }
    json_object_put(json_obj); /* Release validation object */

    /* Store JSON string in MessageContext properties (Axis2/Java pattern) */
    json_stream_prop = axutil_property_create(env);
    axutil_property_set_value(json_stream_prop, env, json_string);
    axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_JSON_STREAM_PROPERTY, json_stream_prop);

    /* Set IS_JSON_STREAM flag (Axis2/Java pattern) */
    is_json_prop = axutil_property_create(env);
    axutil_property_set_value(is_json_prop, env, "true");
    axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_IS_JSON_STREAM_PROPERTY, is_json_prop);

    /* Create dummy SOAP envelope to satisfy Axis2 framework */
    soap_envelope = create_dummy_soap_envelope(env);
    if (!soap_envelope) {
        AXIS2_FREE(env->allocator, json_string);
        return NULL;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                  "JSON Message Builder: Successfully processed JSON (%d bytes), created dummy envelope",
                  bytes_read);

    return soap_envelope;
}

/**
 * @brief Free JSON Message Builder
 */
static void AXIS2_CALL
json_msg_builder_free(
    axis2_msg_builder_t* msg_builder,
    const axutil_env_t* env)
{
    if (msg_builder) {
        AXIS2_FREE(env->allocator, msg_builder);
    }
}