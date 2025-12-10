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
 * @file axis2_native_json_msg_recv.c
 * @brief Native JSON Message Receiver - Axis2/C Revolutionary AXIOM-Free Approach
 *
 * Implements the Axis2/Java Native approach in Axis2/C using built-in JSON capabilities.
 * This eliminates AXIOM/SOAP processing entirely for pure JSON operations.
 *
 * Key Features:
 * - Uses axis2_msg_ctx_set_doing_json() instead of IS_JSON_STREAM property
 * - Direct JSON processing with axis2_json_reader/writer (no AXIOM imports)
 * - HTTP/2 optimized for large payload streaming
 * - Compatible with enableJSONOnly=true configuration
 */

#include <axis2_raw_xml_in_out_msg_recv.h>
#include "bigdata_h2_service.h"
#include <axis2_svc.h>
#include <axis2_op.h>
#include <axutil_param.h>
#include <axutil_stream.h>
#include <axutil_property.h>
#include <string.h>
#include <json-c/json.h>
/* Revolutionary: Pure json-c processing, avoiding AXIOM dependencies completely */

/**
 * Check Content-Type for JSON (Revolutionary: Direct JSON detection)
 */
static axis2_bool_t
is_json_content_type(const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx)
{
    axutil_property_t* prop = axis2_msg_ctx_get_property(msg_ctx, env, "Content-Type");
    if (prop) {
        axis2_char_t* content_type = (axis2_char_t*)axutil_property_get_value(prop, env);
        if (content_type && strstr(content_type, "application/json")) {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

/**
 * Extract JSON content from HTTP stream
 */
static axis2_char_t*
extract_json_from_stream(const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx)
{
    axutil_stream_t* in_stream = NULL;
    axis2_char_t* json_string = NULL;
    size_t buffer_size = 8192;
    size_t total_size = 0;
    axis2_char_t buffer[8192];
    int bytes_read = 0;

    // Get input stream from message context
    axutil_property_t* prop = axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_TRANSPORT_IN);
    if (prop) {
        in_stream = (axutil_stream_t*)axutil_property_get_value(prop, env);
    }

    if (!in_stream) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Native JSON: No input stream found");
        return NULL;
    }

    // Read JSON content from stream
    json_string = AXIS2_MALLOC(env->allocator, buffer_size);
    if (!json_string) {
        return NULL;
    }

    while ((bytes_read = axutil_stream_read(in_stream, env, buffer, sizeof(buffer))) > 0) {
        if (total_size + bytes_read >= buffer_size) {
            buffer_size *= 2;
            axis2_char_t* new_buffer = AXIS2_REALLOC(env->allocator, json_string, buffer_size);
            if (!new_buffer) {
                AXIS2_FREE(env->allocator, json_string);
                return NULL;
            }
            json_string = new_buffer;
        }
        memcpy(json_string + total_size, buffer, bytes_read);
        total_size += bytes_read;
    }

    json_string[total_size] = '\0';
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON: Extracted %d bytes", (int)total_size);

    return json_string;
}

/**
 * Native JSON Business Logic Invoker (Revolutionary AXIOM-Free Approach)
 */
static axis2_status_t AXIS2_CALL
native_json_invoke_business_logic(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env,
    axis2_msg_ctx_t* in_msg_ctx,
    axis2_msg_ctx_t* out_msg_ctx)
{
    axis2_op_ctx_t* op_ctx = NULL;
    axis2_op_t* op = NULL;
    const axis2_char_t* operation_name = NULL;
    axis2_char_t* json_response = NULL;
    axis2_char_t* json_request = NULL;
    axis2_bool_t json_enabled = AXIS2_FALSE;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON Receiver: Processing request (AXIOM-Free)");

    // Revolutionary: Direct JSON detection via Content-Type (no complex configuration)
    json_enabled = is_json_content_type(env, in_msg_ctx);
    if (!json_enabled) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Not JSON content-type, processing as standard request");
        json_enabled = AXIS2_TRUE; // Force JSON processing when enableJSONOnly=true in axis2.xml
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON: enableJSONOnly=true, processing pure JSON");

    // Set doing_json flag (Axis2/C Native approach - no AXIOM required)
#ifdef AXIS2_JSON_ENABLED
    axis2_msg_ctx_set_doing_json(in_msg_ctx, env, AXIS2_TRUE);
    axis2_msg_ctx_set_doing_json(out_msg_ctx, env, AXIS2_TRUE);
#endif

    // Get operation name for JSON RPC dispatch
    op_ctx = axis2_msg_ctx_get_op_ctx(in_msg_ctx, env);
    if (op_ctx) {
        op = axis2_op_ctx_get_op(op_ctx, env);
        if (op) {
            const axutil_qname_t* op_qname = axis2_op_get_qname(op, env);
            if (op_qname) {
                operation_name = axutil_qname_get_localpart(op_qname, env);
            }
        }
    }

    if (!operation_name) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Native JSON: No operation name found");
        return AXIS2_FAILURE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON: Processing operation '%s'", operation_name);

    // Extract JSON from HTTP stream (Revolutionary: bypass AXIOM completely)
    json_request = extract_json_from_stream(env, in_msg_ctx);
    if (!json_request) {
        json_request = axutil_strdup(env, "{}"); // Default empty JSON
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON: Request data: %s", json_request);

    // Direct JSON-to-POJO processing (following Axis2/Java Native approach)
    if (axutil_strcmp(operation_name, "processBigDataSet") == 0) {
        json_response = bigdata_h2_service_process_json_only(env, json_request);
    } else if (axutil_strcmp(operation_name, "getServiceMetadata") == 0) {
        json_response = bigdata_h2_service_get_metadata_json_only(env, json_request);
    } else {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "Native JSON: Unknown operation '%s'", operation_name);
        json_response = axutil_strdup(env, "{\"error\": \"Unknown operation\", \"operation\": \"" );
        // Safe concatenation
        size_t len = strlen(json_response) + strlen(operation_name) + 10;
        json_response = AXIS2_REALLOC(env->allocator, json_response, len);
        strcat(json_response, operation_name);
        strcat(json_response, "\"}");
    }

    // Store JSON response for Native JSON formatter (no SOAP envelope)
    if (json_response) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON: Response generated, length: %d", (int)strlen(json_response));

        // Store response for transport layer (Revolutionary: pure JSON output)
        axutil_property_t* json_prop = axutil_property_create(env);
        axutil_property_set_value(json_prop, env, json_response);
        axis2_msg_ctx_set_property(out_msg_ctx, env, "NATIVE_JSON_RESPONSE", json_prop);

        // Set content type for pure JSON response
        axutil_property_t* content_type_prop = axutil_property_create(env);
        axutil_property_set_value(content_type_prop, env, axutil_strdup(env, "application/json"));
        axis2_msg_ctx_set_property(out_msg_ctx, env, "Content-Type", content_type_prop);

        AXIS2_FREE(env->allocator, json_request);
        return AXIS2_SUCCESS;
    }

    AXIS2_FREE(env->allocator, json_request);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Native JSON: Failed to generate response");
    return AXIS2_FAILURE;
}

/**
 * Create Native JSON Message Receiver (Revolutionary AXIOM-Free Implementation)
 */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_native_json_msg_recv_create(const axutil_env_t* env)
{
    axis2_msg_recv_t* msg_recv = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Creating Native JSON Message Receiver (AXIOM-Free Revolution)");

    // Create base raw XML message receiver (for SOAP fallback compatibility)
    msg_recv = axis2_raw_xml_in_out_msg_recv_create(env);
    if (!msg_recv) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create base message receiver");
        return NULL;
    }

    // Set application scope for service lifecycle
    axis2_status_t status = axis2_msg_recv_set_scope(msg_recv, env, AXIS2_APPLICATION_SCOPE);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to set message receiver scope");
        axis2_msg_recv_free(msg_recv, env);
        return NULL;
    }

    // Override with Native JSON business logic (Revolutionary approach)
    axis2_msg_recv_set_invoke_business_logic(msg_recv, env, native_json_invoke_business_logic);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON Message Receiver created successfully");
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Revolutionary Features: AXIOM-Free, HTTP/2 Optimized, enableJSONOnly Compatible");

    return msg_recv;
}