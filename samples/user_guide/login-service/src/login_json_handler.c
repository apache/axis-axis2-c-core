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
 * @file login_json_handler.c
 * @brief Apache Axis2/C HTTP/2 Pure JSON Login Handler Implementation
 *
 * SOAP-free JSON handler for HTTP/2 authentication service.
 * Eliminates all service skeleton patterns for pure JSON processing.
 */

#include "login_service.h"
#include <axutil_env.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axutil_string.h>
#include <axutil_allocator.h>
#include <axutil_utils.h>
#include <axis2_defines.h>
#include <json-c/json.h>
#include <time.h>

/* Pure JSON handler structure - NO SOAP skeleton dependencies */
typedef struct login_json_handler
{
    axutil_allocator_t *allocator;
    axutil_log_t *log;
    axutil_error_t *error;
    axis2_bool_t initialized;
} login_json_handler_t;

/* Global handler instance */
static login_json_handler_t *g_login_handler = NULL;

/* Function prototypes */
static login_json_handler_t* login_json_handler_create(const axutil_env_t *env);
static void login_json_handler_free(login_json_handler_t *handler, const axutil_env_t *env);
static axis2_char_t* create_json_error_response(const axutil_env_t *env, const axis2_char_t *error_message);

/**
 * Initialize Login JSON Handler (replaces service skeleton create)
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_login_json_handler_init(const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (g_login_handler != NULL)
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "Login JSON handler already initialized");
        return AXIS2_SUCCESS;
    }

    g_login_handler = login_json_handler_create(env);
    if (!g_login_handler)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create Login JSON handler");
        return AXIS2_FAILURE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Login JSON handler initialized for HTTP/2 pure JSON processing");
    return AXIS2_SUCCESS;
}

/**
 * Cleanup Login JSON Handler (replaces service skeleton free)
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_login_json_handler_cleanup(const axutil_env_t *env)
{
    if (g_login_handler)
    {
        login_json_handler_free(g_login_handler, env);
        g_login_handler = NULL;
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Login JSON handler cleaned up");
    }
}

/**
 * Pure JSON doLogin operation handler - NO SOAP patterns
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_login_json_do_login(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    axis2_char_t *json_response = NULL;
    login_request_t *login_request = NULL;
    login_response_t *login_response = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!json_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "NULL JSON request received");
        return create_json_error_response(env, "Invalid JSON request");
    }

    if (!g_login_handler || !g_login_handler->initialized)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Login JSON handler not initialized");
        return create_json_error_response(env, "Service not initialized");
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Processing HTTP/2 pure JSON login request");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Login request JSON: %s", json_request);

    /* Parse JSON request */
    login_request = login_request_create_from_json(env, json_request);
    if (!login_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse login request JSON");
        return create_json_error_response(env, "Failed to parse login request");
    }

    /* Process login request */
    login_response = login_service_do_login(env, login_request);
    if (!login_response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Login service processing failed");
        login_request_free(login_request, env);
        return create_json_error_response(env, "Login processing failed");
    }

    /* Convert response to JSON */
    json_response = login_response_to_json(login_response, env);
    if (!json_response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to convert login response to JSON");
        login_response_free(login_response, env);
        login_request_free(login_request, env);
        return create_json_error_response(env, "Failed to generate response");
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Login response JSON: %s", json_response);

    /* Log authentication attempts (without sensitive data) */
    if (login_response->status && axutil_strcmp(login_response->status, LOGIN_STATUS_SUCCESS) == 0)
    {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                       "HTTP/2 pure JSON authentication successful for user: %s",
                       login_request->email ? login_request->email : "unknown");
    }
    else
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                          "HTTP/2 pure JSON authentication failed for user: %s",
                          login_request->email ? login_request->email : "unknown");
    }

    /* Clean up request/response objects */
    login_response_free(login_response, env);
    login_request_free(login_request, env);

    return json_response;
}

/**
 * Get supported operations list - pure JSON format
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_login_json_get_operations(const axutil_env_t *env)
{
    json_object *operations_json = NULL;
    json_object *operations_array = NULL;
    const char *operations_json_str = NULL;
    axis2_char_t *result = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    /* Create operations JSON */
    operations_json = json_object_new_object();
    operations_array = json_object_new_array();

    if (operations_json && operations_array)
    {
        json_object_array_add(operations_array, json_object_new_string("doLogin"));
        json_object_object_add(operations_json, "service", json_object_new_string("LoginService"));
        json_object_object_add(operations_json, "operations", operations_array);
        json_object_object_add(operations_json, "architecture", json_object_new_string("HTTP/2 Pure JSON"));
        json_object_object_add(operations_json, "timestamp", json_object_new_int64(time(NULL)));

        operations_json_str = json_object_to_json_string(operations_json);
        if (operations_json_str)
        {
            result = axutil_strdup(env, operations_json_str);
        }

        json_object_put(operations_json);
    }

    return result;
}

/* Private helper functions */

/**
 * Create Login JSON handler instance
 */
static login_json_handler_t*
login_json_handler_create(const axutil_env_t *env)
{
    login_json_handler_t *handler = NULL;

    handler = (login_json_handler_t *)AXIS2_MALLOC(env->allocator, sizeof(login_json_handler_t));
    if (!handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    handler->allocator = env->allocator;
    handler->log = env->log;
    handler->error = env->error;
    handler->initialized = AXIS2_TRUE;

    return handler;
}

/**
 * Free Login JSON handler instance
 */
static void
login_json_handler_free(login_json_handler_t *handler, const axutil_env_t *env)
{
    if (handler)
    {
        handler->initialized = AXIS2_FALSE;
        AXIS2_FREE(env->allocator, handler);
    }
}

/**
 * JSON direct invocation entry point
 * This is called by axis2_json_rpc_msg_recv for HTTP/2 JSON requests.
 * Function naming convention: {service_name}_invoke_json
 *
 * Signature must match: json_object* (*)(const axutil_env_t *env, json_object *json_request)
 */
AXIS2_EXTERN json_object* AXIS2_CALL
login_service_invoke_json(
    const axutil_env_t *env,
    json_object *json_request)
{
    const char *json_request_str = NULL;
    axis2_char_t *json_response_str = NULL;
    json_object *json_response = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "LoginService: Processing HTTP/2 JSON request via invoke_json");

    /* Initialize handler if needed */
    if (!g_login_handler)
    {
        axis2_login_json_handler_init(env);
    }

    /* Convert json_object to string for existing handler */
    if (json_request)
    {
        json_request_str = json_object_to_json_string(json_request);
    }

    if (!json_request_str)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "LoginService: Failed to convert JSON request to string");
        json_response = json_object_new_object();
        json_object_object_add(json_response, "status", json_object_new_string("FAILED"));
        json_object_object_add(json_response, "message", json_object_new_string("Invalid JSON request"));
        return json_response;
    }

    /* Delegate to the do_login function */
    json_response_str = axis2_login_json_do_login(env, json_request_str);

    /* Convert string response back to json_object */
    if (json_response_str)
    {
        json_response = json_tokener_parse(json_response_str);
        AXIS2_FREE(env->allocator, json_response_str);
    }

    if (!json_response)
    {
        json_response = json_object_new_object();
        json_object_object_add(json_response, "status", json_object_new_string("FAILED"));
        json_object_object_add(json_response, "message", json_object_new_string("Failed to generate response"));
    }

    return json_response;
}

/**
 * Create JSON error response
 */
static axis2_char_t*
create_json_error_response(
    const axutil_env_t *env,
    const axis2_char_t *error_message)
{
    json_object *error_json = NULL;
    const char *error_json_str = NULL;
    axis2_char_t *result = NULL;

    if (!env || !error_message)
        return NULL;

    /* Create error JSON */
    error_json = json_object_new_object();
    if (error_json)
    {
        json_object_object_add(error_json, "status", json_object_new_string(LOGIN_STATUS_FAILED));
        json_object_object_add(error_json, "message", json_object_new_string(error_message));
        json_object_object_add(error_json, "responseTime", json_object_new_int64(time(NULL)));
        json_object_object_add(error_json, "architecture", json_object_new_string("HTTP/2 Pure JSON"));

        error_json_str = json_object_to_json_string(error_json);
        if (error_json_str)
        {
            result = axutil_strdup(env, error_json_str);
        }

        json_object_put(error_json);
    }

    return result;
}