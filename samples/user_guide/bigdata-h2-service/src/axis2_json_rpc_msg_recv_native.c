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
 * @file axis2_json_rpc_msg_recv_native.c
 * @brief Native JSON RPC Message Receiver - Following Axis2/Java JsonRpcMessageReceiver Pattern
 *
 * This is the Axis2/C equivalent of Axis2/Java's JsonRpcMessageReceiver.
 * Key architecture:
 * 1. Check IS_JSON_STREAM property (set by JSONBuilder)
 * 2. If true: Process pure JSON, bypass all SOAP/XML processing
 * 3. If false: Fall back to standard SOAP processing
 * 4. Use enableJSONOnly parameter for JSON-only services
 */

#include "axis2_json_rpc_msg_recv_native.h"
#include "bigdata_h2_service.h"
#include <axis2_raw_xml_in_out_msg_recv.h>
#include <axis2_msg_recv.h>
#include <axis2_svc.h>
#include <axis2_op.h>
#include <axutil_param.h>
#include <axutil_property.h>
#include <axutil_stream.h>
#include <json-c/json.h>
#include <string.h>
#include <stdio.h>

/* Property names matching Axis2/Java JsonConstant */
#define JSON_IS_JSON_STREAM_PROPERTY "IS_JSON_STREAM"
#define JSON_INPUT_STREAM_PROPERTY "JSON_INPUT_STREAM"
#define JSON_ENABLE_JSON_ONLY_PARAM "enableJSONOnly"

/**
 * @brief JSON RPC Message Receiver structure
 */
struct axis2_json_rpc_msg_recv_native
{
    axis2_msg_recv_t *msg_recv;
    axis2_msg_recv_t *base_msg_recv;  /* Fallback to standard SOAP processing */
};

/* Forward declarations */
static axis2_status_t AXIS2_CALL
json_rpc_native_invoke_business_logic(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env,
    axis2_msg_ctx_t* in_msg_ctx,
    axis2_msg_ctx_t* out_msg_ctx);

static void AXIS2_CALL
json_rpc_native_free(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env);

/**
 * @brief Check if request is JSON stream (following Axis2/Java pattern)
 */
static axis2_bool_t
is_json_stream_request(const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx)
{
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: is_json_stream_request - Checking for JSON_IS_JSON_STREAM property");
    axutil_property_t* is_json_prop = axis2_msg_ctx_get_property(msg_ctx, env, JSON_IS_JSON_STREAM_PROPERTY);
    if (is_json_prop) {
        axis2_char_t* is_json_value = (axis2_char_t*)axutil_property_get_value(is_json_prop, env);
        if (is_json_value && (axutil_strcasecmp(is_json_value, "true") == 0 || axutil_strcmp(is_json_value, "1") == 0)) {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

/**
 * @brief Check if enableJSONOnly is configured (following Axis2/Java pattern)
 */
static axis2_bool_t
is_enable_json_only(const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx)
{
    axis2_svc_t* svc = axis2_msg_ctx_get_svc(msg_ctx, env);
    if (!svc) return AXIS2_FALSE;

    axutil_param_t* param = axis2_svc_get_param(svc, env, JSON_ENABLE_JSON_ONLY_PARAM);
    if (param) {
        axis2_char_t* param_value = (axis2_char_t*)axutil_param_get_value(param, env);
        if (param_value && (axutil_strcasecmp(param_value, "true") == 0 || axutil_strcmp(param_value, "1") == 0)) {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

/* REMOVED: read_json_from_stream function - JSON data comes from message properties */

/**
 * @brief Process JSON request using pure JSON (no SOAP/XML)
 * Following Axis2/Java JsonRpcMessageReceiver.invokeService() pattern
 */
static axis2_char_t*
process_json_request(const axutil_env_t* env, axis2_msg_ctx_t* in_msg_ctx,
                     const axis2_char_t* operation_name, const axis2_char_t* json_request)
{
    axis2_char_t* json_response = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "JSON RPC Native: Processing operation '%s' with pure JSON", operation_name);

    /* Route to appropriate service operation (pure JSON processing) */
    if (axutil_strcmp(operation_name, "processBigDataSet") == 0) {
        json_response = bigdata_h2_service_process_json_only(env, json_request);
    } else if (axutil_strcmp(operation_name, "getServiceMetadata") == 0) {
        json_response = bigdata_h2_service_get_metadata_json_only(env, json_request);
    } else {
        /* Unknown operation - return error in JSON format */
        json_response = axutil_strdup(env, "{\"error\": \"Unknown operation\", \"operation\": \"");
        axis2_char_t* temp = axutil_stracat(env, json_response, operation_name);
        AXIS2_FREE(env->allocator, json_response);
        json_response = axutil_stracat(env, temp, "\", \"availableOperations\": [\"processBigDataSet\", \"getServiceMetadata\"]}");
        AXIS2_FREE(env->allocator, temp);
    }

    return json_response;
}

/**
 * @brief Write JSON response directly to HTTP stream (bypass SOAP)
 */
static axis2_status_t
write_json_response(const axutil_env_t* env, axis2_msg_ctx_t* out_msg_ctx, const axis2_char_t* json_response)
{
    axutil_stream_t* output_stream = NULL;
    int json_len = 0;
    int bytes_written = 0;

    /* Get output stream from message context */
    output_stream = axis2_msg_ctx_get_transport_out_stream(out_msg_ctx, env);
    if (!output_stream) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No output stream found in message context");
        return AXIS2_FAILURE;
    }

    /* Write JSON directly to HTTP response stream */
    json_len = strlen(json_response);
    bytes_written = axutil_stream_write(output_stream, env, json_response, json_len);

    if (bytes_written > 0) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                      "JSON RPC Native: Successfully wrote %d bytes of pure JSON response", bytes_written);
        return AXIS2_SUCCESS;
    } else {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to write JSON response to stream");
        return AXIS2_FAILURE;
    }
}

/**
 * @brief Main business logic processor - Following Axis2/Java JsonRpcMessageReceiver.invokeBusinessLogic()
 */
static axis2_status_t AXIS2_CALL
json_rpc_native_invoke_business_logic(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env,
    axis2_msg_ctx_t* in_msg_ctx,
    axis2_msg_ctx_t* out_msg_ctx)
{
    struct axis2_json_rpc_msg_recv_native* json_recv = NULL;
    axis2_op_ctx_t* op_ctx = NULL;
    axis2_op_t* op = NULL;
    const axis2_char_t* operation_name = NULL;
    axis2_char_t* json_request = NULL;
    axis2_char_t* json_response = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    json_recv = (struct axis2_json_rpc_msg_recv_native*)msg_recv;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "JSON RPC Native: Processing request");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Starting request processing");

    /* Check if this is a JSON stream request (following Axis2/Java pattern) */
    if (!is_json_stream_request(env, in_msg_ctx)) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                       "JSON RPC Native: Not a JSON stream request, falling back to SOAP processing");

        /* Fall back to standard SOAP processing */
        if (json_recv->base_msg_recv) {
            return axis2_msg_recv_invoke_business_logic(json_recv->base_msg_recv, env, in_msg_ctx, out_msg_ctx);
        }
        return AXIS2_FAILURE;
    }

    /* This is a JSON request - process with pure JSON (no SOAP/XML) */
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "JSON RPC Native: Processing pure JSON request (bypassing SOAP)");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Confirmed JSON stream request");

    /* Get operation name */
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Getting operation context");
    op_ctx = axis2_msg_ctx_get_op_ctx(in_msg_ctx, env);
    if (op_ctx) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Got operation context, getting operation");
        op = axis2_op_ctx_get_op(op_ctx, env);
        if (op) {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Got operation, getting qname");
            const axutil_qname_t* op_qname = axis2_op_get_qname(op, env);
            if (op_qname) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Got qname, getting localpart");
                operation_name = axutil_qname_get_localpart(op_qname, env);
            }
        }
    } else {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - ERROR: No operation context found");
    }

    if (!operation_name) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - ERROR: No operation name found");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON RPC Native: No operation name found");
        json_response = axutil_strdup(env, "{\"error\": \"No operation specified\"}");
        goto write_response;
    }

    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Operation name: %s", operation_name);

    /* Get JSON request from properties (set by JSON builder) */
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "DEBUG: JsonRpcMessageReceiver - Getting JSON from message properties");
    axutil_property_t* json_stream_prop = axis2_msg_ctx_get_property(in_msg_ctx, env, JSON_INPUT_STREAM_PROPERTY);
    if (json_stream_prop) {
        json_request = (axis2_char_t*)axutil_property_get_value(json_stream_prop, env);
    }
    if (!json_request) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON RPC Native: Failed to read JSON request");
        json_response = axutil_strdup(env, "{\"error\": \"Failed to read JSON request\"}");
        goto write_response;
    }

    /* Process JSON request with pure JSON (no AXIOM/SOAP) */
    json_response = process_json_request(env, in_msg_ctx, operation_name, json_request);
    if (!json_response) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON RPC Native: Service processing failed");
        json_response = axutil_strdup(env, "{\"error\": \"Service processing failed\"}");
    }

write_response:
    /* Write pure JSON response directly to HTTP stream (bypass SOAP envelope) */
    if (json_response) {
        status = write_json_response(env, out_msg_ctx, json_response);
        AXIS2_FREE(env->allocator, json_response);
    }

    if (json_request) {
        AXIS2_FREE(env->allocator, json_request);
    }

    return status;
}

/**
 * @brief Create Native JSON RPC Message Receiver
 * Following Axis2/Java JsonRpcMessageReceiver pattern
 */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_json_rpc_msg_recv_native_create(const axutil_env_t* env)
{
    struct axis2_json_rpc_msg_recv_native* json_recv = NULL;
    axis2_msg_recv_t* msg_recv = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Creating Native JSON RPC Message Receiver");

    json_recv = (struct axis2_json_rpc_msg_recv_native*)AXIS2_MALLOC(env->allocator,
                                                                      sizeof(struct axis2_json_rpc_msg_recv_native));
    if (!json_recv) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    memset(json_recv, 0, sizeof(struct axis2_json_rpc_msg_recv_native));

    json_recv->msg_recv = axis2_msg_recv_create(env);
    msg_recv = json_recv->msg_recv;

    /* Create base message receiver for SOAP fallback */
    json_recv->base_msg_recv = axis2_raw_xml_in_out_msg_recv_create(env);

    /* Set function pointers */
    axis2_msg_recv_set_invoke_business_logic(msg_recv, env, json_rpc_native_invoke_business_logic);
    /* axis2_msg_recv_set_free(msg_recv, env, json_rpc_native_free); */

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Native JSON RPC Message Receiver created successfully");
    return msg_recv;
}

/**
 * @brief Free JSON RPC Message Receiver
 */
static void AXIS2_CALL
json_rpc_native_free(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env)
{
    struct axis2_json_rpc_msg_recv_native* json_recv = NULL;

    if (msg_recv) {
        json_recv = (struct axis2_json_rpc_msg_recv_native*)msg_recv;

        /* Free base message receiver */
        if (json_recv->base_msg_recv) {
            axis2_msg_recv_free(json_recv->base_msg_recv, env);
        }

        /* Free msg_recv */
        if (json_recv->msg_recv) {
            axis2_msg_recv_free(json_recv->msg_recv, env);
        }

        AXIS2_FREE(env->allocator, json_recv);
    }
}