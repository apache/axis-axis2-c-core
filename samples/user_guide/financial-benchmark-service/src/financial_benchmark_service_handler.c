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
 * @file financial_benchmark_service_handler.c
 * @brief HTTP/2 Pure JSON Service Handler - No SOAP/XML dependencies
 *
 * This handler processes HTTP/2 JSON requests using pure json-c library.
 * Eliminates all SOAP patterns (no skeleton, stub, epr, axiom_node).
 *
 * Provides the Axis2/C entry points required for dynamic service loading:
 * - axis2_financial_benchmark_service_create()
 * - financial_benchmark_service_invoke_json()
 */

#include "financial_benchmark_service.h"
#include <axis2_svc.h>
#include <axis2_msg_ctx.h>
#include <axis2_http_transport.h>
#include <json-c/json.h>
#include <axutil_string.h>
#include <axutil_log.h>
#include <string.h>

/**
 * @brief Route JSON request to appropriate operation handler
 */
static axis2_char_t*
route_operation(
    const axutil_env_t *env,
    const axis2_char_t *operation_name,
    const axis2_char_t *json_request)
{
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "FinancialBenchmarkService: Routing to operation '%s'",
                   operation_name ? operation_name : "unknown");

    if (!operation_name)
    {
        return axutil_strdup(env, "{\"error\":\"No operation specified\"}");
    }

    /* portfolioVariance operation */
    if (axutil_strcmp(operation_name, "portfolioVariance") == 0)
    {
        return finbench_portfolio_variance_json_only(env, json_request);
    }

    /* monteCarlo operation */
    if (axutil_strcmp(operation_name, "monteCarlo") == 0)
    {
        return finbench_monte_carlo_json_only(env, json_request);
    }

    /* metadata operation */
    if (axutil_strcmp(operation_name, "metadata") == 0)
    {
        return finbench_get_metadata_json(env);
    }

    /* generateTestData operation */
    if (axutil_strcmp(operation_name, "generateTestData") == 0)
    {
        /* Extract n_assets from request if provided */
        int n_assets = 100;  /* Default */
        if (json_request)
        {
            json_object *json_obj = json_tokener_parse(json_request);
            if (json_obj)
            {
                json_object *value_obj;
                if (json_object_object_get_ex(json_obj, "n_assets", &value_obj))
                {
                    n_assets = json_object_get_int(value_obj);
                }
                json_object_put(json_obj);
            }
        }
        return finbench_generate_test_portfolio_json(env, n_assets);
    }

    /* scenarioAnalysis operation (stub for now) */
    if (axutil_strcmp(operation_name, "scenarioAnalysis") == 0)
    {
        return axutil_strdup(env,
            "{\"status\":\"NOT_IMPLEMENTED\",\"message\":\"Scenario analysis coming soon\"}");
    }

    /* Unknown operation */
    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                      "FinancialBenchmarkService: Unknown operation '%s'", operation_name);

    char error_msg[256];
    snprintf(error_msg, sizeof(error_msg),
             "{\"error\":\"Unknown operation: %s\"}", operation_name);
    return axutil_strdup(env, error_msg);
}

/**
 * @brief HTTP/2 JSON Service Handler - processes pure JSON requests
 *
 * This is the main entry point called by the Axis2/C engine for JSON requests.
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
financial_benchmark_service_invoke_json(
    axis2_svc_t *svc,
    const axutil_env_t *env,
    const axis2_char_t *json_request,
    axis2_msg_ctx_t *msg_ctx)
{
    const axis2_char_t *operation_name = NULL;
    axis2_op_t *op = NULL;
    axutil_qname_t *op_qname = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "FinancialBenchmarkService: Processing HTTP/2 JSON request");

    /* Try to get operation from message context */
    if (msg_ctx)
    {
        op = axis2_msg_ctx_get_op(msg_ctx, env);
        if (op)
        {
            op_qname = (axutil_qname_t *)axis2_op_get_qname(op, env);
            if (op_qname)
            {
                operation_name = axutil_qname_get_localpart(op_qname, env);
            }
        }
    }

    /* If we couldn't get operation from context, try parsing from JSON */
    if (!operation_name && json_request)
    {
        json_object *json_obj = json_tokener_parse(json_request);
        if (json_obj)
        {
            json_object *op_obj;
            if (json_object_object_get_ex(json_obj, "operation", &op_obj))
            {
                operation_name = json_object_get_string(op_obj);
            }
            /* Don't free json_obj yet - operation_name points into it */
            /* We'll handle this by copying the string */
            if (operation_name)
            {
                axis2_char_t *op_copy = axutil_strdup(env, operation_name);
                json_object_put(json_obj);
                axis2_char_t *result = route_operation(env, op_copy, json_request);
                AXIS2_FREE(env->allocator, op_copy);
                return result;
            }
            json_object_put(json_obj);
        }
    }

    /* Route to the appropriate operation */
    return route_operation(env, operation_name, json_request);
}

/**
 * @brief Pure JSON processing function for enableJSONOnly=true
 * Following Axis2/Java JsonRpcMessageReceiver pattern
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
financial_benchmark_service_process_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "FinancialBenchmarkService: Processing pure JSON request (enableJSONOnly=true)");

    if (!json_request)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "FinancialBenchmarkService: No JSON request provided");
        return axutil_strdup(env, "{\"error\": \"No JSON request provided\"}");
    }

    /* Try to extract operation from JSON */
    const char *operation_name = NULL;
    json_object *json_obj = json_tokener_parse(json_request);
    if (json_obj)
    {
        json_object *op_obj;
        if (json_object_object_get_ex(json_obj, "operation", &op_obj))
        {
            operation_name = json_object_get_string(op_obj);
        }
        if (operation_name)
        {
            axis2_char_t *op_copy = axutil_strdup(env, operation_name);
            json_object_put(json_obj);
            axis2_char_t *result = route_operation(env, op_copy, json_request);
            AXIS2_FREE(env->allocator, (void*)op_copy);
            return result;
        }
        json_object_put(json_obj);
    }

    /* Default to metadata if no operation specified */
    return finbench_get_metadata_json(env);
}

/**
 * @brief Get HTTP/2 service operations (REST-style endpoints)
 */
AXIS2_EXTERN axutil_array_list_t* AXIS2_CALL
financial_benchmark_service_get_operations(
    axis2_svc_t *svc,
    const axutil_env_t *env)
{
    axutil_array_list_t *operations = NULL;

    operations = axutil_array_list_create(env, 10);
    if (operations)
    {
        axutil_array_list_add(operations, env, "portfolioVariance");
        axutil_array_list_add(operations, env, "monteCarlo");
        axutil_array_list_add(operations, env, "scenarioAnalysis");
        axutil_array_list_add(operations, env, "metadata");
        axutil_array_list_add(operations, env, "generateTestData");
    }

    return operations;
}

/**
 * @brief Initialize HTTP/2 JSON service
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
financial_benchmark_service_init(
    axis2_svc_t *svc,
    const axutil_env_t *env)
{
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                  "FinancialBenchmarkService: Initializing HTTP/2 JSON service");
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                  "FinancialBenchmarkService: Max assets=%d, Max simulations=%d",
                  FINBENCH_MAX_ASSETS, FINBENCH_MAX_SIMULATIONS);
    return AXIS2_SUCCESS;
}

/**
 * @brief Service factory function for HTTP/2 JSON service
 *
 * This is the primary entry point that Axis2/C calls to create the service instance.
 */
AXIS2_EXTERN axis2_svc_t* AXIS2_CALL
axis2_financial_benchmark_service_create(
    const axutil_env_t *env)
{
    axis2_svc_t *svc = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                  "FinancialBenchmarkService: Creating service instance");

    svc = axis2_svc_create(env);
    if (!svc)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                       "FinancialBenchmarkService: Failed to create service");
        return NULL;
    }

    /* Set service name */
    axis2_svc_set_name(svc, env, "FinancialBenchmarkService");

    /* Initialize the service */
    financial_benchmark_service_init(svc, env);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                  "FinancialBenchmarkService: Service created successfully");

    return svc;
}

/**
 * @brief Alternate service factory function (axis2_get_instance pattern)
 *
 * Some Axis2/C configurations look for this function name.
 */
AXIS2_EXTERN axis2_svc_t* AXIS2_CALL
axis2_get_instance(
    const axutil_env_t *env)
{
    return axis2_financial_benchmark_service_create(env);
}

/**
 * @brief Service cleanup function
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_remove_instance(
    axis2_svc_t *svc,
    const axutil_env_t *env)
{
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                  "FinancialBenchmarkService: Removing service instance");

    if (svc)
    {
        axis2_svc_free(svc, env);
    }

    return AXIS2_SUCCESS;
}
