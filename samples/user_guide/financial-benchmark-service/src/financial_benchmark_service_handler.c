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
#include "mcp_catalog_handler.h"
#include "axis2_json_secure_fault.h"
#include <axis2_svc.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_endpoint_ref.h>
#include <axis2_http_transport.h>
#include <json-c/json.h>
#include <axutil_string.h>
#include <axutil_log.h>
#include <string.h>

/* ============================================================================
 * Field Filtering — ?fields=status,calc_time_us
 *
 * Parses the fields query parameter and filters a JSON response string
 * to include only the requested top-level fields. Re-parses the response
 * via json-c, deletes unwanted keys, re-serializes. The cost of parsing
 * a ~1 KB response object is negligible.
 *
 * Mirrors Axis2/Java FieldFilteringMessageFormatter.
 * @see https://github.com/apache/axis-axis2-java-core/blob/master/modules/json/src/org/apache/axis2/json/streaming/FieldFilteringMessageFormatter.java
 * ============================================================================
 */

/**
 * Extract the "fields" query parameter value from a request URI.
 * Returns NULL if not present; caller must free with AXIS2_FREE.
 */
static axis2_char_t*
finbench_extract_fields_param(
    const axutil_env_t *env,
    const axis2_char_t *uri)
{
    const char *qmark, *start, *amp;
    if (!uri) return NULL;
    qmark = strchr(uri, '?');
    if (!qmark) return NULL;

    start = qmark + 1;
    while (*start) {
        /* find end of this param */
        amp = strchr(start, '&');
        size_t plen = amp ? (size_t)(amp - start) : strlen(start);

        if (plen > 7 && strncmp(start, "fields=", 7) == 0) {
            size_t vlen = plen - 7;
            char *val = AXIS2_MALLOC(env->allocator, vlen + 1);
            if (!val) return NULL;
            memcpy(val, start + 7, vlen);
            val[vlen] = '\0';
            return val;
        }
        if (!amp) break;
        start = amp + 1;
    }
    return NULL;
}

/**
 * Filter a JSON response string to include only the specified fields.
 * If fields_csv is NULL or empty, returns json_str unchanged.
 * On filter, frees the original json_str and returns a new allocation.
 */
static axis2_char_t*
finbench_filter_json_response(
    const axutil_env_t *env,
    axis2_char_t *json_str,
    const axis2_char_t *fields_csv)
{
    json_object *root, *inner;
    const char *filtered_str;
    axis2_char_t *result;
    char fields_buf[512];
    const char *keep[64];
    int n_keep = 0;

    if (!fields_csv || !fields_csv[0] || !json_str) return json_str;

    /* Parse comma-separated field names into keep[] array */
    strncpy(fields_buf, fields_csv, sizeof(fields_buf) - 1);
    fields_buf[sizeof(fields_buf) - 1] = '\0';
    char *tok = strtok(fields_buf, ",");
    while (tok && n_keep < 64) {
        while (*tok == ' ') tok++;  /* trim leading space */
        if (*tok) keep[n_keep++] = tok;
        tok = strtok(NULL, ",");
    }
    if (n_keep == 0) return json_str;

    /* Parse the JSON response */
    root = json_tokener_parse(json_str);
    if (!root) return json_str;

    /* Navigate into {"response": {...}} wrapper if present */
    inner = NULL;
    json_object_object_get_ex(root, "response", &inner);
    if (!inner) inner = root;

    /* Collect keys to delete (cannot delete during iteration) */
    const char *del_keys[256];
    int n_del = 0;
    json_object_object_foreach(inner, key, val) {
        (void)val;
        int found = 0;
        for (int i = 0; i < n_keep; i++) {
            if (strcmp(key, keep[i]) == 0) { found = 1; break; }
        }
        if (!found && n_del < 256) del_keys[n_del++] = key;
    }

    /* Delete unwanted keys */
    for (int i = 0; i < n_del; i++) {
        json_object_object_del(inner, del_keys[i]);
    }

    /* Re-serialize and replace */
    filtered_str = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, filtered_str);
    json_object_put(root);
    AXIS2_FREE(env->allocator, json_str);
    return result;
}

/**
 * @brief Route JSON request to appropriate operation handler
 */
static axis2_char_t*
route_operation(
    const axutil_env_t *env,
    const axis2_char_t *operation_name,
    const axis2_char_t *json_request,
    const axis2_char_t *fields_csv)
{
    axis2_char_t *result = NULL;

    AXIS2_LOG_INFO(env->log,
                   "FinancialBenchmarkService: Routing to operation '%s'",
                   operation_name ? operation_name : "unknown");

    if (!operation_name)
    {
        char corr_id[AXIS2_JSON_CORR_ID_LEN];
        axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "[FinancialBenchmark][%s] route_operation: no operation name",
            corr_id);
        return axis2_json_secure_fault(env, corr_id, "no operation specified");
    }

    /* portfolioVariance operation */
    if (axutil_strcmp(operation_name, "portfolioVariance") == 0)
    {
        result = finbench_portfolio_variance_json_only(env, json_request);
    }

    /* monteCarlo operation */
    else if (axutil_strcmp(operation_name, "monteCarlo") == 0)
    {
        result = finbench_monte_carlo_json_only(env, json_request);
    }

    /* metadata operation */
    else if (axutil_strcmp(operation_name, "metadata") == 0)
    {
        result = finbench_get_metadata_json(env);
    }

    /* generateTestData operation */
    else if (axutil_strcmp(operation_name, "generateTestData") == 0)
    {
        int n_assets = 100;
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
        result = finbench_generate_test_portfolio_json(env, n_assets);
    }

    /* scenarioAnalysis operation */
    else if (axutil_strcmp(operation_name, "scenarioAnalysis") == 0)
    {
        result = finbench_scenario_json_only(env, json_request);
    }

    else
    {
        /* Unknown operation */
        char corr_id[AXIS2_JSON_CORR_ID_LEN];
        axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "[FinancialBenchmark][%s] unknown operation '%s'",
            corr_id, operation_name);
        return axis2_json_secure_fault(env, corr_id, "unknown operation");
    }

    /* Apply field filtering if ?fields= was present */
    if (result && fields_csv && fields_csv[0])
        result = finbench_filter_json_response(env, result, fields_csv);
    return result;
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

    AXIS2_LOG_INFO(env->log,
                   "FinancialBenchmarkService: Processing HTTP/2 JSON request");

    /* ── MCP catalog intercept ──────────────────────────────────────────────
     * GET /services/FinancialBenchmarkService/_mcp/openapi-mcp.json
     *
     * Must run before operation dispatch so it works even with no JSON body.
     * The catalog is generated by walking all deployed services via conf,
     * which mirrors the Java OpenApiSpecGenerator.generateMcpCatalogJson().
     * ────────────────────────────────────────────────────────────────────── */
    if (msg_ctx)
    {
        const axis2_char_t *request_uri = NULL;
        axis2_endpoint_ref_t *to = axis2_msg_ctx_get_to(msg_ctx, env);
        if (to)
            request_uri = axis2_endpoint_ref_get_address(to, env);

        if (request_uri)
        {
            axis2_conf_ctx_t *conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
            axis2_conf_t     *conf     = conf_ctx
                    ? axis2_conf_ctx_get_conf(conf_ctx, env)
                    : NULL;
            /* mcp_catalog_check_and_respond does suffix-based path matching
             * internally (no redundant strstr here).  Returns AXIS2_FALSE when
             * path does not match OR when catalog generation fails (NULL output),
             * so catalog_json is guaranteed non-NULL on AXIS2_TRUE. */
            axis2_char_t *catalog_json = NULL;
            if (mcp_catalog_check_and_respond(env, conf, request_uri, &catalog_json))
            {
                AXIS2_LOG_INFO(env->log,
                    "FinancialBenchmarkService: Returning MCP catalog (%zu bytes)",
                    catalog_json ? strlen(catalog_json) : 0u);
                return catalog_json;   /* caller (msg_recv) frees via AXIS2_FREE */
            }
        }
    }

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

    /* Extract ?fields= query parameter for response field filtering */
    axis2_char_t *fields_csv = NULL;
    if (msg_ctx)
    {
        const axis2_char_t *uri = NULL;
        axis2_endpoint_ref_t *to_ref = axis2_msg_ctx_get_to(msg_ctx, env);
        if (to_ref)
            uri = axis2_endpoint_ref_get_address(to_ref, env);
        if (uri)
            fields_csv = finbench_extract_fields_param(env, uri);
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
            if (operation_name)
            {
                axis2_char_t *op_copy = axutil_strdup(env, operation_name);
                json_object_put(json_obj);
                axis2_char_t *result = route_operation(env, op_copy, json_request, fields_csv);
                AXIS2_FREE(env->allocator, op_copy);
                if (fields_csv) AXIS2_FREE(env->allocator, fields_csv);
                return result;
            }
            json_object_put(json_obj);
        }
    }

    /* Route to the appropriate operation */
    axis2_char_t *result = route_operation(env, operation_name, json_request, fields_csv);
    if (fields_csv) AXIS2_FREE(env->allocator, fields_csv);
    return result;
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
    AXIS2_LOG_INFO(env->log,
                   "FinancialBenchmarkService: Processing pure JSON request (enableJSONOnly=true)");

    if (!json_request)
    {
        char corr_id[AXIS2_JSON_CORR_ID_LEN];
        axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[FinancialBenchmark][%s] process_json_only: no JSON request",
            corr_id);
        return axis2_json_secure_fault(env, corr_id, "no request body");
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
            axis2_char_t *result = route_operation(env, op_copy, json_request, NULL);
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
    AXIS2_LOG_INFO(env->log,
                  "FinancialBenchmarkService: Initializing HTTP/2 JSON service");
    AXIS2_LOG_INFO(env->log,
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

    AXIS2_LOG_INFO(env->log,
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

    AXIS2_LOG_INFO(env->log,
                  "FinancialBenchmarkService: Service created successfully");

    return svc;
}

/**
 * @brief Service factory function called by axutil_class_loader_create_dll().
 *
 * CRITICAL: The signature must match CREATE_FUNCT in axutil_dll_desc.h:
 *   typedef int (*CREATE_FUNCT)(void **inst, const axutil_env_t *env);
 *
 * The class loader calls: create_funct(&obj, env)
 * A wrong signature (e.g., returning axis2_svc_t* with only env param) causes
 * &obj to be misinterpreted as env, leading to a segfault in axis2_svc_create.
 *
 * Compare with bigdata-h2-service and login-service which avoid this by not
 * exporting axis2_get_instance (they use ServiceClass=<name> in services.xml
 * with a different loading path).
 */
AXIS2_EXTERN int AXIS2_CALL
axis2_get_instance(
    void **inst,
    const axutil_env_t *env)
{
    *inst = axis2_financial_benchmark_service_create(env);
    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/**
 * @brief Service cleanup function called by axutil_class_loader.
 *
 * CRITICAL: The signature must match DELETE_FUNCT in axutil_dll_desc.h:
 *   typedef int (*DELETE_FUNCT)(void *inst, const axutil_env_t *env);
 */
AXIS2_EXTERN int AXIS2_CALL
axis2_remove_instance(
    void *inst,
    const axutil_env_t *env)
{
    axis2_svc_t *svc = (axis2_svc_t *)inst;
    AXIS2_LOG_INFO(env->log,
                  "FinancialBenchmarkService: Removing service instance");

    if (svc)
    {
        axis2_svc_free(svc, env);
    }

    return AXIS2_SUCCESS;
}
