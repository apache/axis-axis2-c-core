/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file login_service_handler.c
 * @brief Apache Axis2/C HTTP/2 Login Service Handler Implementation
 *
 * Axis2/C service skeleton handler for pure JSON authentication service.
 * Integrates login_service.c with the Axis2/C service framework.
 */

#include "login_service.h"
#include <axis2_svc_skeleton.h>
#include <axis2_msg_ctx.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axutil_string.h>
/* HTTP/2 Pure JSON Architecture - AXIOM headers needed for service skeleton compatibility */
/* TODO: These should be eliminated in a pure JSON implementation */
#include <axiom_text.h>
#include <axiom_node.h>
#include <axiom_element.h>

/* Service skeleton implementation structure */
typedef struct login_service_skeleton
{
    axis2_svc_skeleton_t svc_skeleton;
    axutil_array_list_t *func_array;
} login_service_skeleton_t;

#define AXIS2_INTF_TO_IMPL(svc_skeleton) ((login_service_skeleton_t *)(svc_skeleton))

/* Function prototypes */
axutil_array_list_t *AXIS2_CALL login_service_skeleton_get_invoke_function_names(
    axis2_svc_skeleton_t *svc_skeleton, const axutil_env_t *env);

axiom_node_t *AXIS2_CALL login_service_skeleton_invoke(
    axis2_svc_skeleton_t *svc_skeleton, const axutil_env_t *env,
    axiom_node_t *content_node, axis2_msg_ctx_t *msg_ctx);

int AXIS2_CALL login_service_skeleton_init(
    axis2_svc_skeleton_t *svc_skeleton, const axutil_env_t *env);

axiom_node_t *AXIS2_CALL login_service_skeleton_on_fault(
    axis2_svc_skeleton_t *svc_skeleton, const axutil_env_t *env, axiom_node_t *content_node);

void AXIS2_CALL login_service_skeleton_free(
    axis2_svc_skeleton_t *svc_skeleton, const axutil_env_t *env);

/* HTTP/2 JSON operation handler */
axiom_node_t *AXIS2_CALL login_service_do_login_operation(
    const axutil_env_t *env, axiom_node_t *content_node, axis2_msg_ctx_t *msg_ctx);

/* Utility functions */
static axis2_char_t* extract_json_from_axiom(const axutil_env_t *env, axiom_node_t *node);
static axiom_node_t* create_axiom_from_json(const axutil_env_t *env, const axis2_char_t *json_string);
static axiom_node_t* create_error_response(const axutil_env_t *env, const axis2_char_t *error_message);

/* JSON direct invocation entry point - required by axis2_json_rpc_msg_recv */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_invoke_json(
    axis2_svc_t *svc,
    const axutil_env_t *env,
    const axis2_char_t *json_request,
    axis2_msg_ctx_t *msg_ctx);

/**
 * Create Login Service Skeleton
 */
AXIS2_EXTERN axis2_svc_skeleton_t *AXIS2_CALL
axis2_login_service_create(const axutil_env_t *env)
{
    login_service_skeleton_t *svc_skeleton_impl = NULL;
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    axutil_array_list_t *func_array = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_skeleton_impl = (login_service_skeleton_t *)AXIS2_MALLOC(env->allocator,
                                                                 sizeof(login_service_skeleton_t));
    if (!svc_skeleton_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_skeleton = &(svc_skeleton_impl->svc_skeleton);
    svc_skeleton->ops = AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_skeleton_ops_t));
    if (!svc_skeleton->ops)
    {
        AXIS2_FREE(env->allocator, svc_skeleton_impl);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_skeleton->ops->free = login_service_skeleton_free;
    svc_skeleton->ops->init = login_service_skeleton_init;
    svc_skeleton->ops->invoke = login_service_skeleton_invoke;
    svc_skeleton->ops->on_fault = login_service_skeleton_on_fault;
    svc_skeleton->ops->get_invoke_function_names = login_service_skeleton_get_invoke_function_names;

    /* Create function array for supported operations */
    func_array = axutil_array_list_create(env, 0);
    if (func_array)
    {
        axutil_array_list_add(func_array, env, "doLogin");
        svc_skeleton_impl->func_array = func_array;
    }

    AXIS2_LOG_INFO(env->log, "Login Service skeleton created successfully");
    return svc_skeleton;
}

/**
 * Get invoke function names
 */
axutil_array_list_t *AXIS2_CALL
login_service_skeleton_get_invoke_function_names(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env)
{
    login_service_skeleton_t *svc_skeleton_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    svc_skeleton_impl = AXIS2_INTF_TO_IMPL(svc_skeleton);
    return svc_skeleton_impl->func_array;
}

/**
 * Main invoke function - handles HTTP/2 JSON requests
 */
axiom_node_t *AXIS2_CALL
login_service_skeleton_invoke(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    axiom_node_t *content_node,
    axis2_msg_ctx_t *msg_ctx)
{
    axiom_node_t *result_node = NULL;
    axis2_op_t *operation = NULL;
    axutil_qname_t *op_qname = NULL;
    axis2_char_t *op_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    /* Get operation name */
    operation = axis2_msg_ctx_get_op(msg_ctx, env);
    if (operation)
    {
        op_qname = (axutil_qname_t *)axis2_op_get_qname(operation, env);
        if (op_qname)
        {
            op_name = axutil_qname_get_localpart(op_qname, env);
        }
    }

    AXIS2_LOG_INFO(env->log, "Login Service invoke: operation=%s",
                   op_name ? op_name : "unknown");

    /* Route to appropriate operation handler */
    if (op_name && axutil_strcmp(op_name, "doLogin") == 0)
    {
        result_node = login_service_do_login_operation(env, content_node, msg_ctx);
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unknown operation: %s", op_name ? op_name : "null");
        result_node = create_error_response(env, "Unknown operation");
    }

    return result_node;
}

/**
 * Initialize service skeleton
 */
int AXIS2_CALL
login_service_skeleton_init(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env)
{
    AXIS2_LOG_INFO(env->log, "Login Service initialized for HTTP/2 JSON processing");
    return AXIS2_SUCCESS;
}

/**
 * Handle service faults
 */
axiom_node_t *AXIS2_CALL
login_service_skeleton_on_fault(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    axiom_node_t *content_node)
{
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Login Service fault handler called");
    return create_error_response(env, "Service fault occurred");
}

/**
 * Free service skeleton
 */
void AXIS2_CALL
login_service_skeleton_free(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env)
{
    login_service_skeleton_t *svc_skeleton_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE_VOID);
    svc_skeleton_impl = AXIS2_INTF_TO_IMPL(svc_skeleton);

    if (svc_skeleton->ops)
    {
        AXIS2_FREE(env->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }

    if (svc_skeleton_impl->func_array)
    {
        axutil_array_list_free(svc_skeleton_impl->func_array, env);
        svc_skeleton_impl->func_array = NULL;
    }

    if (svc_skeleton_impl)
    {
        AXIS2_FREE(env->allocator, svc_skeleton_impl);
    }

    AXIS2_LOG_INFO(env->log, "Login Service skeleton freed");
}

/**
 * doLogin operation handler - HTTP/2 JSON authentication
 */
axiom_node_t *AXIS2_CALL
login_service_do_login_operation(
    const axutil_env_t *env,
    axiom_node_t *content_node,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *json_request = NULL;
    axis2_char_t *json_response = NULL;
    axiom_node_t *result_node = NULL;
    login_request_t *login_request = NULL;
    login_response_t *login_response = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    AXIS2_LOG_INFO(env->log, "Processing doLogin operation with HTTP/2 JSON");

    /* Extract JSON from axiom node */
    json_request = extract_json_from_axiom(env, content_node);
    if (!json_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to extract JSON from request");
        return create_error_response(env, "Invalid JSON request");
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Login request JSON: %s", json_request);

    /* Parse JSON request */
    login_request = login_request_create_from_json(env, json_request);
    if (!login_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse login request JSON");
        AXIS2_FREE(env->allocator, json_request);
        return create_error_response(env, "Failed to parse login request");
    }

    /* Process login request */
    login_response = login_service_do_login(env, login_request);
    if (!login_response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Login service processing failed");
        login_request_free(login_request, env);
        AXIS2_FREE(env->allocator, json_request);
        return create_error_response(env, "Login processing failed");
    }

    /* Convert response to JSON */
    json_response = login_response_to_json(login_response, env);
    if (!json_response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to convert login response to JSON");
        login_response_free(login_response, env);
        login_request_free(login_request, env);
        AXIS2_FREE(env->allocator, json_request);
        return create_error_response(env, "Failed to generate response");
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Login response JSON: %s", json_response);

    /* Convert JSON response to axiom node */
    result_node = create_axiom_from_json(env, json_response);

    /* Log successful authentication attempts (without sensitive data) */
    if (login_response->status && axutil_strcmp(login_response->status, LOGIN_STATUS_SUCCESS) == 0)
    {
        AXIS2_LOG_INFO(env->log,
                       "HTTP/2 JSON authentication successful for user: %s",
                       login_request->email ? login_request->email : "unknown");
    }
    else
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                          "HTTP/2 JSON authentication failed for user: %s",
                          login_request->email ? login_request->email : "unknown");
    }

    /* Clean up */
    login_response_free(login_response, env);
    login_request_free(login_request, env);
    AXIS2_FREE(env->allocator, json_request);
    AXIS2_FREE(env->allocator, json_response);

    return result_node;
}

/**
 * JSON direct invocation entry point
 * This is called by axis2_json_rpc_msg_recv for HTTP/2 JSON requests.
 * Function naming convention: {service_name}_invoke_json
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_invoke_json(
    axis2_svc_t *svc,
    const axutil_env_t *env,
    const axis2_char_t *json_request,
    axis2_msg_ctx_t *msg_ctx)
{
    login_request_t *request = NULL;
    login_response_t *response = NULL;
    axis2_char_t *json_response = NULL;

    AXIS2_LOG_INFO(env->log,
                   "LoginService: Processing HTTP/2 JSON request via invoke_json");

    if (!json_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "LoginService: No JSON request provided");
        return axutil_strdup(env, "{\"status\":\"FAILED\",\"message\":\"No JSON request provided\"}");
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "LoginService request: %s", json_request);

    /* Parse JSON request */
    request = login_request_create_from_json(env, json_request);
    if (!request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "LoginService: Failed to parse JSON request");
        return axutil_strdup(env, "{\"status\":\"FAILED\",\"message\":\"Invalid JSON request format\"}");
    }

    /* Process login */
    response = login_service_do_login(env, request);
    if (!response)
    {
        login_request_free(request, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "LoginService: Login processing failed");
        return axutil_strdup(env, "{\"status\":\"FAILED\",\"message\":\"Login processing failed\"}");
    }

    /* Convert response to JSON */
    json_response = login_response_to_json(response, env);

    /* Clean up */
    login_response_free(response, env);
    login_request_free(request, env);

    if (!json_response)
    {
        return axutil_strdup(env, "{\"status\":\"FAILED\",\"message\":\"Failed to generate response\"}");
    }

    AXIS2_LOG_INFO(env->log,
                   "LoginService: Successfully processed JSON request");

    return json_response;
}

/* Utility Functions */

/**
 * Extract JSON string from axiom node
 */
static axis2_char_t*
extract_json_from_axiom(
    const axutil_env_t *env,
    axiom_node_t *node)
{
    axiom_element_t *element = NULL;
    axis2_char_t *text_value = NULL;
    axiom_node_t *text_node = NULL;
    axiom_text_t *text = NULL;

    if (!env || !node)
        return NULL;

    /* Get the element */
    element = (axiom_element_t *)axiom_node_get_data_element(node, env);
    if (!element)
        return NULL;

    /* Get first child (should be text node with JSON) */
    text_node = axiom_node_get_first_child(node, env);
    if (text_node && axiom_node_get_node_type(text_node, env) == AXIOM_TEXT)
    {
        text = (axiom_text_t *)axiom_node_get_data_element(text_node, env);
        if (text)
        {
            text_value = (axis2_char_t *)axiom_text_get_value(text, env);
            if (text_value)
            {
                return axutil_strdup(env, text_value);
            }
        }
    }

    /* If no text node, get element text directly */
    text_value = axiom_element_get_text(element, env, node);
    if (text_value)
    {
        return axutil_strdup(env, text_value);
    }

    return NULL;
}

/**
 * Create axiom node from JSON string
 */
static axiom_node_t*
create_axiom_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    axiom_node_t *result_node = NULL;
    axiom_element_t *result_element = NULL;
    axiom_text_t *json_text = NULL;
    axiom_node_t *text_node = NULL;
    axiom_namespace_t *ns = NULL;
    axutil_qname_t *qname = NULL;

    if (!env || !json_string)
        return NULL;

    /* Create response element */
    ns = axiom_namespace_create(env, "http://login.services.samples.axis2.apache.org", "ns1");
    qname = axutil_qname_create(env, "doLoginResponse", "http://login.services.samples.axis2.apache.org", "ns1");
    result_element = axiom_element_create(env, NULL, qname, ns, &result_node);

    if (result_element && result_node)
    {
        /* Create text node with JSON content */
        json_text = axiom_text_create(env, result_node, json_string, &text_node);
        if (!json_text)
        {
            /* Failed to create text node, free the result node */
            axiom_node_free_tree(result_node, env);
            result_node = NULL;
        }
    }

    /* Clean up qname and namespace */
    if (qname)
        axutil_qname_free(qname, env);
    if (ns)
        axiom_namespace_free(ns, env);

    return result_node;
}

/**
 * Create error response axiom node
 */
static axiom_node_t*
create_error_response(
    const axutil_env_t *env,
    const axis2_char_t *error_message)
{
    json_object *error_json = NULL;
    const char *error_json_str = NULL;
    axis2_char_t *json_string = NULL;
    axiom_node_t *error_node = NULL;

    if (!env || !error_message)
        return NULL;

    /* Create error JSON */
    error_json = json_object_new_object();
    if (error_json)
    {
        json_object_object_add(error_json, "status", json_object_new_string(LOGIN_STATUS_FAILED));
        json_object_object_add(error_json, "message", json_object_new_string(error_message));
        json_object_object_add(error_json, "responseTime", json_object_new_int64(time(NULL)));

        error_json_str = json_object_to_json_string(error_json);
        if (error_json_str)
        {
            json_string = axutil_strdup(env, error_json_str);
            error_node = create_axiom_from_json(env, json_string);
            AXIS2_FREE(env->allocator, json_string);
        }

        json_object_put(error_json);
    }

    return error_node;
}