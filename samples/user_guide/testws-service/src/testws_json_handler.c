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
 * @file testws_json_handler.c
 * @brief Apache Axis2/C HTTP/2 Pure JSON TestWS Handler Implementation
 *
 * SOAP-free JSON handler for HTTP/2 XSS protection demonstration service.
 * Eliminates all service skeleton patterns for pure JSON processing.
 */

#include "testws_service.h"
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
typedef struct testws_json_handler
{
    axutil_allocator_t *allocator;
    axutil_log_t *log;
    axutil_error_t *error;
    axis2_bool_t initialized;
} testws_json_handler_t;

/* Global handler instance */
static testws_json_handler_t *g_testws_handler = NULL;

/* Function prototypes */
static testws_json_handler_t* testws_json_handler_create(const axutil_env_t *env);
static void testws_json_handler_free(testws_json_handler_t *handler, const axutil_env_t *env);
static axis2_char_t* create_json_error_response(const axutil_env_t *env, const axis2_char_t *error_message);

/**
 * Initialize TestWS JSON Handler (replaces service skeleton create)
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_testws_json_handler_init(const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (g_testws_handler != NULL)
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "TestWS JSON handler already initialized");
        return AXIS2_SUCCESS;
    }

    g_testws_handler = testws_json_handler_create(env);
    if (!g_testws_handler)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create TestWS JSON handler");
        return AXIS2_FAILURE;
    }

    AXIS2_LOG_INFO(env->log, "TestWS JSON handler initialized for HTTP/2 XSS protection demonstration");
    return AXIS2_SUCCESS;
}

/**
 * Cleanup TestWS JSON Handler (replaces service skeleton free)
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_testws_json_handler_cleanup(const axutil_env_t *env)
{
    if (g_testws_handler)
    {
        testws_json_handler_free(g_testws_handler, env);
        g_testws_handler = NULL;
        AXIS2_LOG_INFO(env->log, "TestWS JSON handler cleaned up");
    }
}

/**
 * Pure JSON doTestws operation handler - NO SOAP patterns
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_testws_json_do_testws(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    axis2_char_t *json_response = NULL;
    testws_request_t *testws_request = NULL;
    testws_response_t *testws_response = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!json_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "NULL JSON request received");
        return create_json_error_response(env, "Invalid JSON request");
    }

    if (!g_testws_handler || !g_testws_handler->initialized)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "TestWS JSON handler not initialized");
        return create_json_error_response(env, "Service not initialized");
    }

    AXIS2_LOG_INFO(env->log, "Processing HTTP/2 pure JSON TestWS XSS demonstration request");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "TestWS request JSON: %s", json_request);

    /* Parse JSON request */
    testws_request = testws_request_create_from_json(env, json_request);
    if (!testws_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse TestWS request JSON");
        return create_json_error_response(env, "Failed to parse TestWS request");
    }

    /* Process TestWS request - XSS protection demonstration */
    testws_response = testws_service_do_testws(env, testws_request);
    if (!testws_response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "TestWS service processing failed");
        testws_request_free(testws_request, env);
        return create_json_error_response(env, "TestWS processing failed");
    }

    /* Convert response to JSON */
    json_response = testws_response_to_json(testws_response, env);
    if (!json_response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to convert TestWS response to JSON");
        testws_response_free(testws_response, env);
        testws_request_free(testws_request, env);
        return create_json_error_response(env, "Failed to generate response");
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "TestWS response JSON: %s", json_response);

    /* Log security demonstration results */
    if (testws_response->status && axutil_strcmp(testws_response->status, TESTWS_STATUS_OK) == 0)
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                          "HTTP/2 pure JSON XSS demonstration completed. Input: %s, Output contains intentional XSS payload",
                          testws_request->message_in ? testws_request->message_in : "unknown");
    }
    else
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                          "HTTP/2 pure JSON XSS demonstration - input validation failed for: %s",
                          testws_request->message_in ? testws_request->message_in : "unknown");
    }

    /* Clean up request/response objects */
    testws_response_free(testws_response, env);
    testws_request_free(testws_request, env);

    return json_response;
}

/**
 * Get supported operations list - pure JSON format
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_testws_json_get_operations(const axutil_env_t *env)
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
        json_object_array_add(operations_array, json_object_new_string("doTestws"));
        json_object_object_add(operations_json, "service", json_object_new_string("TestwsService"));
        json_object_object_add(operations_json, "operations", operations_array);
        json_object_object_add(operations_json, "architecture", json_object_new_string("HTTP/2 Pure JSON"));
        json_object_object_add(operations_json, "purpose", json_object_new_string("XSS Protection Demonstration"));
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
 * Create TestWS JSON handler instance
 */
static testws_json_handler_t*
testws_json_handler_create(const axutil_env_t *env)
{
    testws_json_handler_t *handler = NULL;

    handler = (testws_json_handler_t *)AXIS2_MALLOC(env->allocator, sizeof(testws_json_handler_t));
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
 * Free TestWS JSON handler instance
 */
static void
testws_json_handler_free(testws_json_handler_t *handler, const axutil_env_t *env)
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
testws_service_invoke_json(
    const axutil_env_t *env,
    json_object *json_request)
{
    const char *json_request_str = NULL;
    json_object *json_response = NULL;
    testws_request_t *request = NULL;
    testws_response_t *response = NULL;

    AXIS2_LOG_INFO(env->log,
                   "TestwsService: Processing HTTP/2 JSON request via invoke_json");

    /* Convert json_object to string */
    if (json_request)
    {
        json_request_str = json_object_to_json_string(json_request);
    }

    if (!json_request_str)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "TestwsService: Failed to convert JSON request to string");
        json_response = json_object_new_object();
        json_object_object_add(json_response, "status", json_object_new_string("ERROR"));
        json_object_object_add(json_response, "messageout", json_object_new_string(""));
        json_object_object_add(json_response, "securityDetails", json_object_new_string("Invalid JSON request"));
        return json_response;
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "TestwsService request: %s", json_request_str);

    /* Parse JSON request directly */
    request = testws_request_create_from_json(env, json_request_str);
    if (!request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "TestwsService: Failed to parse JSON request");
        json_response = json_object_new_object();
        json_object_object_add(json_response, "status", json_object_new_string("ERROR"));
        json_object_object_add(json_response, "messageout", json_object_new_string(""));
        json_object_object_add(json_response, "securityDetails", json_object_new_string("Invalid request format"));
        return json_response;
    }

    /* Process the request */
    response = testws_service_do_testws(env, request);
    testws_request_free(request, env);

    if (!response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "TestwsService: Processing failed");
        json_response = json_object_new_object();
        json_object_object_add(json_response, "status", json_object_new_string("ERROR"));
        json_object_object_add(json_response, "messageout", json_object_new_string(""));
        json_object_object_add(json_response, "securityDetails", json_object_new_string("Processing failed"));
        return json_response;
    }

    /* Convert response to JSON object */
    json_response = json_object_new_object();
    json_object_object_add(json_response, "status", json_object_new_string(response->status ? response->status : "ERROR"));
    json_object_object_add(json_response, "messageout", json_object_new_string(response->message_out ? response->message_out : ""));
    json_object_object_add(json_response, "securityDetails", json_object_new_string(response->security_details ? response->security_details : ""));
    json_object_object_add(json_response, "responseTime", json_object_new_int64(response->response_time));

    testws_response_free(response, env);

    AXIS2_LOG_INFO(env->log, "TestwsService: Successfully processed request");

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
        json_object_object_add(error_json, "status", json_object_new_string(TESTWS_STATUS_ERROR));
        json_object_object_add(error_json, "messageout", json_object_new_string(""));
        json_object_object_add(error_json, "securityDetails", json_object_new_string(error_message));
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