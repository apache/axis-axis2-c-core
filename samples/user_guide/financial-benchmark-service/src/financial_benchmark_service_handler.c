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
 * Field Filtering — ?fields=status,results.ticker,results.ops
 *
 * Supports both flat and nested (dot-notation) field selection:
 *
 *   ?fields=status,calc_time_us           — top-level only (original behavior)
 *   ?fields=status,results.ticker          — keep "status" at top level,
 *                                            keep only "ticker" inside "results"
 *   ?fields=results.ticker,results.ops     — filter inside nested object/array
 *
 * For nested paths like "results.ticker":
 *   - If "results" is a JSON object, keeps only "ticker" inside it
 *   - If "results" is a JSON array of objects, keeps only "ticker" in each element
 *
 * This enables filtering 126 of 127 fields inside a large nested payload
 * (e.g., a portfolio service returning 127 fields per asset).
 *
 * Re-parses via json-c, prunes unwanted keys, re-serializes.
 *
 * Mirrors and extends Axis2/Java FieldFilteringMessageFormatter.
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

    /* Walk the query string parameter by parameter (split on '&').
     * We're looking for "fields=value" — manual parsing avoids pulling
     * in a URL-parsing library for this single parameter. */
    start = qmark + 1;
    while (*start) {
        /* Find the end of this key=value pair */
        amp = strchr(start, '&');
        size_t plen = amp ? (size_t)(amp - start) : strlen(start);

        if (plen > 7 && strncmp(start, "fields=", 7) == 0) {
            /* Found it — extract just the value portion after "fields=" */
            size_t vlen = plen - 7;
            char *val = AXIS2_MALLOC(env->allocator, vlen + 1);
            if (!val) return NULL;
            memcpy(val, start + 7, vlen);
            val[vlen] = '\0';
            return val;  /* Caller must AXIS2_FREE this */
        }
        if (!amp) break;
        start = amp + 1;
    }
    return NULL;
}

/**
 * Trim leading and trailing whitespace from a string in place.
 * Returns pointer to the trimmed start (within the original buffer).
 */
static char*
finbench_trim(char *s)
{
    while (*s == ' ') s++;
    if (*s) {
        char *end = s + strlen(s) - 1;
        while (end > s && *end == ' ') *end-- = '\0';
    }
    return s;
}

/**
 * Filter keys of a JSON object, keeping only those in keep_keys[].
 * Handles the "cannot delete during iteration" constraint by collecting
 * keys to delete first, then deleting in a second pass.
 */
static void
finbench_filter_object_keys(
    const axutil_env_t *env,
    json_object *obj,
    const char **keep_keys,
    int n_keep)
{
    /*
     * Two-phase delete pattern:
     *
     * json-c's json_object_object_foreach uses internal hash iteration.
     * Deleting keys during iteration corrupts the iterator (pointer
     * aliasing). So we:
     *   Phase 1: iterate and collect keys NOT in keep_keys[] into del_keys[]
     *   Phase 2: delete collected keys in a separate loop
     *
     * The del_keys[] array holds const char* pointers into the json_object's
     * internal key storage — valid until json_object_object_del removes them.
     */
    int n_keys = json_object_object_length(obj);
    const char **del_keys = AXIS2_MALLOC(env->allocator,
        (size_t)n_keys * sizeof(const char *));
    if (!del_keys) return;  /* OOM: leave object unfiltered (graceful degradation) */
    int n_del = 0;

    /* Phase 1: identify keys to remove */
    json_object_object_foreach(obj, key, val) {
        (void)val;
        int found = 0;
        for (int i = 0; i < n_keep; i++) {
            if (strcmp(key, keep_keys[i]) == 0) { found = 1; break; }
        }
        if (!found) del_keys[n_del++] = key;
    }

    /* Phase 2: remove unwanted keys (safe — no longer iterating) */
    for (int i = 0; i < n_del; i++) {
        json_object_object_del(obj, del_keys[i]);
    }
    AXIS2_FREE(env->allocator, del_keys);
}

/**
 * Apply nested field filtering on a JSON object or array.
 *
 * For a path like "results" with sub-fields ["ticker", "ops"]:
 *   - If obj["results"] is a JSON object: filter its keys to ticker, ops
 *   - If obj["results"] is a JSON array: filter each element's keys
 */
static void
finbench_filter_nested(
    const axutil_env_t *env,
    json_object *parent,
    const char *container_key,
    const char **sub_fields,
    int n_sub)
{
    json_object *container = NULL;
    if (!json_object_object_get_ex(parent, container_key, &container))
        return;  /* Container key doesn't exist — nothing to filter */

    if (json_object_is_type(container, json_type_object)) {
        /*
         * Single nested object, e.g.:
         *   {"response": {"results": {"ticker":"AAPL", "ops":0.05, "pwr":0.12}}}
         * With ?fields=results.ticker,results.ops we keep ticker and ops,
         * delete pwr from the "results" object.
         */
        finbench_filter_object_keys(env, container, sub_fields, n_sub);
    }
    else if (json_object_is_type(container, json_type_array)) {
        /*
         * Array of objects — the key use case for portfolio services.
         * e.g.: {"response": {"assets": [{"ticker":"AAPL","ops":0.05,...},
         *                                {"ticker":"MSFT","ops":0.03,...}]}}
         * With ?fields=assets.ticker,assets.ops we filter EACH array
         * element independently, keeping only ticker and ops in every object.
         * Non-object array elements (strings, numbers) are left untouched.
         */
        int arr_len = json_object_array_length(container);
        for (int i = 0; i < arr_len; i++) {
            json_object *elem = json_object_array_get_idx(container, i);
            if (elem && json_object_is_type(elem, json_type_object)) {
                finbench_filter_object_keys(env, elem, sub_fields, n_sub);
            }
        }
    }
    /* If container is a scalar (string/number/bool), there are no sub-keys
     * to filter — leave it untouched. This handles edge cases like
     * ?fields=status.foo where "status" is a string, not an object. */
}

/**
 * Filter a JSON response string to include only the specified fields.
 * Supports dot-notation for nested filtering:
 *   "status"          — keep top-level "status"
 *   "results.ticker"  — keep "results" but filter it to only "ticker"
 *
 * If fields_csv is NULL or empty, returns json_str unchanged.
 * On filter, frees the original json_str and returns a new allocation.
 */
static axis2_char_t*
finbench_filter_json_response(
    const axutil_env_t *env,
    axis2_char_t *json_str,
    const axis2_char_t *fields_csv)
{
    /*
     * Memory ownership contract:
     *   - json_str is OWNED by this function (caller expects us to free or return it)
     *   - result starts as json_str; on success it becomes a new allocation and
     *     json_str is freed. On failure, result stays as json_str (unfiltered).
     *   - fields_buf is our working copy of fields_csv for tokenization
     *   - root is the parsed json-c tree — must json_object_put on all exit paths
     *   - All cleanup goes through the 'cleanup:' label at the bottom
     */
    json_object *root = NULL;
    axis2_char_t *result = json_str;
    char *fields_buf = NULL;

    /*
     * Parsing result: two parallel structures built from the ?fields= value.
     *
     * For "?fields=status,results.ticker,results.ops":
     *   top_keep[] = ["status", "results"]     (n_top = 2)
     *   nested[0]  = { container="results", subs=["ticker","ops"], n_subs=2 }
     *
     * Note: "results" appears in BOTH top_keep (so it survives the top-level
     * prune in Phase 1) AND in nested[] (so its contents get filtered in Phase 2).
     */
    const char *top_keep[64];
    int n_top = 0;

    typedef struct {
        const char *container;  /* e.g., "portfolio_results" */
        const char *subs[64];   /* e.g., ["ticker", "ops", ...] */
        int n_subs;
    } nested_spec_t;
    nested_spec_t nested[16];  /* Up to 16 distinct container names */
    int n_nested = 0;

    if (!fields_csv || !fields_csv[0] || !json_str) return json_str;

    /* ── Step 1: Copy fields_csv into a mutable working buffer ────────────
     *
     * We need a mutable copy because strtok_r writes NUL bytes into it.
     * Heap-allocated (not stack) because the query string can be arbitrarily
     * long. The 64KB cap prevents a malicious client from forcing a huge
     * allocation. */
    size_t fields_len = strlen(fields_csv);
    if (fields_len > 65535) {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "fields parameter length %zu exceeds 64KB limit, returning unfiltered",
            fields_len);
        return json_str;
    }
    fields_buf = AXIS2_MALLOC(env->allocator, fields_len + 1);
    if (!fields_buf) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "OOM allocating fields_buf, returning unfiltered response");
        return json_str;
    }
    memcpy(fields_buf, fields_csv, fields_len + 1);

    /* ── Step 2: Tokenize and classify each field spec ────────────────────
     *
     * Split on commas, then for each token:
     *   - "status"          → simple top-level field, goes into top_keep[]
     *   - "results.ticker"  → dot-notation, split into container="results"
     *                         and subfield="ticker". Container goes into
     *                         top_keep[] (to survive Phase 1 prune) AND
     *                         into nested[] (for Phase 2 inner filtering).
     *
     * All pointers (container, subfield, tok) point INTO fields_buf — they
     * remain valid as long as fields_buf is alive (freed in cleanup:).
     *
     * strtok_r (not strtok) is mandatory — Apache mod_h2 runs multiple
     * requests concurrently in worker threads. strtok uses a static buffer
     * that would be shared across threads → data corruption. */
    char *saveptr = NULL;
    char *tok = strtok_r(fields_buf, ",", &saveptr);
    while (tok) {
        tok = finbench_trim(tok);
        if (!*tok) { tok = strtok_r(NULL, ",", &saveptr); continue; }

        char *dot = strchr(tok, '.');
        if (dot) {
            /* Dot-notation: split "container.subfield" by writing NUL at the dot */
            *dot = '\0';
            const char *container = tok;
            const char *subfield = finbench_trim(dot + 1);

            /* Find existing nested_spec for this container, or create one.
             * Multiple fields from the same container accumulate:
             * "results.ticker,results.ops" → nested[i].subs = ["ticker","ops"] */
            int found_idx = -1;
            for (int i = 0; i < n_nested; i++) {
                if (strcmp(nested[i].container, container) == 0) {
                    found_idx = i;
                    break;
                }
            }
            if (found_idx < 0 && n_nested < 16) {
                found_idx = n_nested++;
                nested[found_idx].container = container;
                nested[found_idx].n_subs = 0;
            }
            if (found_idx >= 0 && nested[found_idx].n_subs < 64 && *subfield) {
                nested[found_idx].subs[nested[found_idx].n_subs++] = subfield;
            }

            /* The container key itself must survive the top-level prune
             * (Phase 1), otherwise Phase 2 has nothing to filter inside.
             * Dedup: don't add "results" to top_keep twice. */
            int already_top = 0;
            for (int i = 0; i < n_top; i++) {
                if (strcmp(top_keep[i], container) == 0) { already_top = 1; break; }
            }
            if (!already_top && n_top < 64) top_keep[n_top++] = container;
        }
        else {
            /* Simple top-level field — e.g., "status", "calc_time_us" */
            if (n_top < 64) top_keep[n_top++] = tok;
        }

        tok = strtok_r(NULL, ",", &saveptr);
    }

    /* Nothing to filter (e.g., "?fields=" with empty value, or all tokens
     * were whitespace/empty after trimming) */
    if (n_top == 0 && n_nested == 0) goto cleanup;

    /* ── Step 3: Parse the JSON response into a json-c tree ─────────────
     *
     * We parse the full response string into a mutable json-c object tree.
     * This lets us delete unwanted keys and re-serialize only what remains.
     * For typical service responses (~1KB), the parse cost is negligible. */
    root = json_tokener_parse(json_str);
    if (!root) {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "Failed to parse JSON for field filtering, returning unfiltered");
        goto cleanup;
    }

    /* ── Step 4: Navigate into the {"response": {...}} wrapper ────────────
     *
     * Axis2 JSON-RPC responses are wrapped: {"response": {actual fields...}}.
     * We filter inside the "response" object, not the root. If there's no
     * "response" wrapper (e.g., raw JSON), filter the root directly. */
    {
        json_object *inner = NULL;
        json_object_object_get_ex(root, "response", &inner);
        if (!inner) inner = root;

        /* ── Phase 1: Top-level prune ────────────────────────────────────
         *
         * Delete all keys from inner that are NOT in top_keep[].
         * After this, only requested top-level fields and container keys
         * survive. For "?fields=status,results.ticker":
         *   Before: {"status":"SUCCESS","results":{...},"calc_time_us":1,...}
         *   After:  {"status":"SUCCESS","results":{...}}
         */
        finbench_filter_object_keys(env, inner, top_keep, n_top);

        /* ── Phase 2: Nested prune ───────────────────────────────────────
         *
         * For each container that had dot-notation sub-fields, filter
         * inside the container. For "?fields=results.ticker,results.ops":
         *   Before: {"results":{"ticker":"AAPL","ops":0.05,"pwr":0.12,...}}
         *   After:  {"results":{"ticker":"AAPL","ops":0.05}}
         *
         * If the container is an array of objects (common for portfolio
         * data), each array element is filtered independently. */
        for (int i = 0; i < n_nested; i++) {
            finbench_filter_nested(env, inner, nested[i].container,
                                   nested[i].subs, nested[i].n_subs);
        }
    }

    /* ── Step 5: Re-serialize the pruned tree ────────────────────────────
     *
     * json_object_to_json_string_ext returns a pointer to json-c's internal
     * buffer — valid only until json_object_put(root). We must copy it via
     * axutil_strdup before releasing the tree.
     *
     * On success: free the original json_str, return the filtered copy.
     * On OOM:     keep original json_str as result (graceful degradation). */
    {
        const char *filtered_str = json_object_to_json_string_ext(root,
            JSON_C_TO_STRING_PLAIN);
        axis2_char_t *new_result = axutil_strdup(env, filtered_str);
        if (new_result) {
            AXIS2_FREE(env->allocator, json_str);
            result = new_result;
        }
        /* OOM on axutil_strdup: fall through to cleanup, return original json_str */
    }

    /* ── Cleanup: free all resources on every exit path ───────────────────
     *
     * goto-based cleanup is the standard C idiom for multi-resource functions.
     * Both root and fields_buf may be NULL (if we jumped here before allocating
     * them), so the NULL checks are required. */
cleanup:
    if (root) json_object_put(root);      /* Release the json-c parse tree */
    if (fields_buf) AXIS2_FREE(env->allocator, fields_buf);  /* Release our working copy */
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
    const axutil_qname_t *op_qname = NULL;

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
            op_qname = axis2_op_get_qname(op, env);
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
        if (uri) {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: endpoint ref address for fields extraction: '%s'",
                uri);
            fields_csv = finbench_extract_fields_param(env, uri);
            if (fields_csv) {
                AXIS2_LOG_INFO(env->log,
                    "FinancialBenchmarkService: extracted fields_csv: '%s'", fields_csv);
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
