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
 * @file axis2_json_msg_receiver.c
 * @brief Simple JSON Message Receiver - Works with Builder/Formatter Pattern
 *
 * Following Axis2/Java approach:
 * 1. Check IS_JSON_STREAM property (set by JSONBuilder)
 * 2. If true, process JSON from properties (no AXIOM)
 * 3. If false, fall back to standard processing
 * 4. Set JSON_RESPONSE property for JSONFormatter
 */

#include <axis2_raw_xml_in_out_msg_recv.h>
#include "bigdata_h2_service.h"
#include "axis2_json_msg_builder.h"
#include "axis2_json_msg_formatter.h"
#include <axis2_svc.h>
#include <axis2_op.h>
#include <axutil_param.h>
#include <axutil_property.h>

/**
 * @brief Check if request is JSON stream (set by JSON builder)
 * Following Axis2/Java JsonRpcMessageReceiver pattern
 */
static axis2_bool_t
is_json_stream_request(const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx)
{
    axutil_property_t* is_json_prop = axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_IS_JSON_STREAM_PROPERTY);
    if (is_json_prop) {
        axis2_char_t* is_json_value = (axis2_char_t*)axutil_property_get_value(is_json_prop, env);
        if (is_json_value && axutil_strcasecmp(is_json_value, "true") == 0) {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

/**
 * @brief Check if enableJSONOnly is configured for service
 * Following Axis2/Java JsonRpcMessageReceiver pattern
 */
static axis2_bool_t
is_json_only_enabled(const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx)
{
    axis2_svc_t* svc = axis2_msg_ctx_get_svc(msg_ctx, env);
    if (!svc) return AXIS2_FALSE;

    axutil_param_t* param = axis2_svc_get_param(svc, env, "enableJSONOnly");
    if (param) {
        axis2_char_t* param_value = (axis2_char_t*)axutil_param_get_value(param, env);
        if (param_value && axutil_strcasecmp(param_value, "true") == 0) {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

/**
 * @brief JSON-only business logic processor
 * Following Axis2/Java JsonRpcMessageReceiver.invokeBusinessLogic() pattern
 */
static axis2_status_t AXIS2_CALL
json_only_invoke_business_logic(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env,
    axis2_msg_ctx_t* in_msg_ctx,
    axis2_msg_ctx_t* out_msg_ctx)
{
    axis2_op_ctx_t* op_ctx = NULL;
    axis2_op_t* op = NULL;
    const axis2_char_t* operation_name = NULL;
    axutil_property_t* json_stream_prop = NULL;
    axis2_char_t* json_request = NULL;
    axis2_char_t* json_response = NULL;
    axutil_property_t* json_response_prop = NULL;

    AXIS2_LOG_INFO(env->log, "JSON Message Receiver: Processing pure JSON request");

    /* Verify this is a JSON stream request */
    if (!is_json_stream_request(env, in_msg_ctx)) {
        AXIS2_LOG_INFO(env->log, "Not a JSON stream request, skipping JSON processing");
        return AXIS2_FAILURE;
    }

    /* Verify enableJSONOnly is configured */
    if (!is_json_only_enabled(env, in_msg_ctx)) {
        AXIS2_LOG_INFO(env->log, "enableJSONOnly not configured, skipping JSON processing");
        return AXIS2_FAILURE;
    }

    /* Get operation name */
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
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON Message Receiver: No operation name found");
        json_response = axutil_strdup(env, "{\"error\": \"No operation specified\"}");
        goto set_response;
    }

    /* Get JSON request from properties (set by JSON builder) */
    json_stream_prop = axis2_msg_ctx_get_property(in_msg_ctx, env, AXIS2_JSON_STREAM_PROPERTY);
    if (json_stream_prop) {
        json_request = (axis2_char_t*)axutil_property_get_value(json_stream_prop, env);
    }

    if (!json_request) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON Message Receiver: No JSON request found in properties");
        json_response = axutil_strdup(env, "{\"error\": \"No JSON request data\"}");
        goto set_response;
    }

    AXIS2_LOG_INFO(env->log,
                   "JSON Message Receiver: Processing operation '%s' with JSON data", operation_name);

    /* Process JSON based on operation (pure JSON processing) */
    if (axutil_strcmp(operation_name, "processBigDataSet") == 0) {
        json_response = bigdata_h2_service_process_json_only(env, json_request);
    } else if (axutil_strcmp(operation_name, "getServiceMetadata") == 0) {
        json_response = bigdata_h2_service_get_metadata_json_only(env, json_request);
    } else {
        /* Unknown operation */
        json_response = axutil_strdup(env, "{\"error\": \"Unknown operation\", \"operation\": \"");
        axis2_char_t* temp = axutil_stracat(env, json_response, operation_name);
        AXIS2_FREE(env->allocator, json_response);
        json_response = axutil_stracat(env, temp, "\", \"availableOperations\": [\"processBigDataSet\", \"getServiceMetadata\"]}");
        AXIS2_FREE(env->allocator, temp);
    }

set_response:
    /* Set JSON response in output message context for JSON formatter */
    if (json_response) {
        json_response_prop = axutil_property_create(env);
        axutil_property_set_value(json_response_prop, env, json_response);
        axis2_msg_ctx_set_property(out_msg_ctx, env, AXIS2_JSON_RESPONSE_PROPERTY, json_response_prop);

        AXIS2_LOG_INFO(env->log,
                      "JSON Message Receiver: Successfully processed JSON request, response ready");
        return AXIS2_SUCCESS;
    } else {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JSON Message Receiver: Failed to generate JSON response");

        /* Set error response */
        json_response = axutil_strdup(env, "{\"error\": \"Service processing failed\"}");
        json_response_prop = axutil_property_create(env);
        axutil_property_set_value(json_response_prop, env, json_response);
        axis2_msg_ctx_set_property(out_msg_ctx, env, AXIS2_JSON_RESPONSE_PROPERTY, json_response_prop);

        return AXIS2_FAILURE;
    }
}

/**
 * @brief Create JSON-aware Message Receiver
 * Extends existing raw_xml_in_out_msg_recv with JSON processing
 */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_json_msg_receiver_create(const axutil_env_t* env)
{
    axis2_msg_recv_t* msg_recv = NULL;

    AXIS2_LOG_INFO(env->log, "Creating JSON-aware Message Receiver");

    /* Create base raw XML message receiver */
    msg_recv = axis2_raw_xml_in_out_msg_recv_create(env);
    if (!msg_recv) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create base message receiver");
        return NULL;
    }

    /* Override business logic to handle JSON processing */
    axis2_msg_recv_set_invoke_business_logic(msg_recv, env, json_only_invoke_business_logic);

    AXIS2_LOG_INFO(env->log, "JSON-aware Message Receiver created successfully");
    return msg_recv;
}