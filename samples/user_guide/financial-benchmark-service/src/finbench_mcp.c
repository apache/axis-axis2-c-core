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
 * @file finbench_mcp.c
 * @brief MCP stdio transport — JSON-RPC 2.0 loop for the Financial Benchmark Service
 *
 * Three required MCP methods:
 *   initialize  — protocol version + server info + capabilities
 *   tools/list  — static tool catalog (portfolioVariance, monteCarlo, scenarioAnalysis)
 *   tools/call  — thin wrapper around finbench_*_json_only()
 *
 * JSON-RPC 2.0 rules honored:
 *   - Requests without "id" are notifications → silently consumed, no response
 *   - Parse errors produce id=null error responses
 *   - All responses include "jsonrpc": "2.0"
 *   - MCP tools/call result wraps the operation JSON in content[0].text
 */

#include "financial_benchmark_service.h"
#include "finbench_mcp.h"

#include <json-c/json.h>
#include <axutil_string.h>
#include <stdio.h>
#include <string.h>

/* ============================================================================
 * Protocol constants
 * ============================================================================
 */

#define FINBENCH_MCP_PROTOCOL_VERSION   "2024-11-05"
#define FINBENCH_MCP_SERVER_NAME        "axis2c-financial-benchmark"
#define FINBENCH_MCP_SERVER_VERSION     "2.0.1"

/* JSON-RPC 2.0 error codes */
#define MCP_ERR_PARSE_ERROR       -32700
#define MCP_ERR_INVALID_REQUEST   -32600
#define MCP_ERR_METHOD_NOT_FOUND  -32601
#define MCP_ERR_INVALID_PARAMS    -32602
#define MCP_ERR_INTERNAL_ERROR    -32603

/* ============================================================================
 * Tool catalog — input schemas (snake_case, matching C service field names)
 *
 * These are the same JSON field names accepted by the HTTP/2 JSON endpoints.
 * MCP clients use these schemas to populate requests.
 * ============================================================================
 */

static const char SCHEMA_PORTFOLIO_VARIANCE[] =
    "{"
    "\"type\":\"object\","
    "\"properties\":{"
        "\"n_assets\":{\"type\":\"integer\","
            "\"description\":\"Number of assets in the portfolio (max 2000)\"},"
        "\"weights\":{\"type\":\"array\",\"items\":{\"type\":\"number\"},"
            "\"description\":\"Portfolio weights. Must sum to 1.0 unless normalize_weights=true\"},"
        "\"covariance_matrix\":{\"type\":\"array\",\"items\":{\"type\":\"number\"},"
            "\"description\":\"Flattened n_assets x n_assets covariance matrix (row-major order)\"},"
        "\"normalize_weights\":{\"type\":\"boolean\","
            "\"description\":\"Rescale weights to sum to 1.0 before computing variance. Default: false\"},"
        "\"n_periods_per_year\":{\"type\":\"integer\","
            "\"description\":\"Trading periods per year for annualizing volatility. "
                "Default 252 (equity). Use 260 (some fixed-income), 365 (crypto), 12 (monthly)\"},"
        "\"request_id\":{\"type\":\"string\","
            "\"description\":\"Optional identifier echoed in the response for request tracing\"}"
    "},"
    "\"required\":[\"n_assets\",\"weights\",\"covariance_matrix\"]"
    "}";

static const char SCHEMA_MONTE_CARLO[] =
    "{"
    "\"type\":\"object\","
    "\"properties\":{"
        "\"n_simulations\":{\"type\":\"integer\","
            "\"description\":\"Number of simulation paths (max 1,000,000). Default: 10,000\"},"
        "\"n_periods\":{\"type\":\"integer\","
            "\"description\":\"Number of time steps per path (e.g. 252 = one trading year). Default: 252\"},"
        "\"initial_value\":{\"type\":\"number\","
            "\"description\":\"Initial portfolio value in currency units. Default: 1,000,000\"},"
        "\"expected_return\":{\"type\":\"number\","
            "\"description\":\"Expected annualized return (e.g. 0.08 for 8%). Default: 0.08\"},"
        "\"volatility\":{\"type\":\"number\","
            "\"description\":\"Annualized volatility (e.g. 0.20 for 20%). Default: 0.20\"},"
        "\"random_seed\":{\"type\":\"integer\","
            "\"description\":\"xorshift128+ seed for reproducibility. 0 = non-deterministic. Default: 0\"},"
        "\"n_periods_per_year\":{\"type\":\"integer\","
            "\"description\":\"Trading periods per year — controls GBM time step dt=1/n_periods_per_year. "
                "Default: 252. Must match the annualization basis of expected_return and volatility\"},"
        "\"percentiles\":{\"type\":\"array\",\"items\":{\"type\":\"number\"},"
            "\"description\":\"Percentile tail levels for VaR, e.g. [0.01, 0.05] for 99%% and 95%% VaR. "
                "Max 8 values. Default: [0.01, 0.05]\"},"
        "\"request_id\":{\"type\":\"string\"}"
    "},"
    "\"required\":[],"
    "\"description\":\"All fields have defaults — an empty {} request body is valid\""
    "}";

static const char SCHEMA_SCENARIO_ANALYSIS[] =
    "{"
    "\"type\":\"object\","
    "\"properties\":{"
        "\"assets\":{"
            "\"type\":\"array\","
            "\"description\":\"Portfolio assets with scenario prices and probabilities\","
            "\"items\":{"
                "\"type\":\"object\","
                "\"properties\":{"
                    "\"asset_id\":{\"type\":\"integer\","
                        "\"description\":\"Unique asset identifier\"},"
                    "\"current_price\":{\"type\":\"number\","
                        "\"description\":\"Current market price in currency units\"},"
                    "\"position_size\":{\"type\":\"number\","
                        "\"description\":\"Position size in shares/units. Scales upside/downside to dollar terms\"},"
                    "\"scenario_prices\":{\"type\":\"array\",\"items\":{\"type\":\"number\"},"
                        "\"description\":\"Target price for each scenario\"},"
                    "\"probabilities\":{\"type\":\"array\",\"items\":{\"type\":\"number\"},"
                        "\"description\":\"Probability weight for each scenario. Must sum to 1.0\"}"
                "},"
                "\"required\":[\"asset_id\",\"current_price\",\"scenario_prices\",\"probabilities\"]"
            "}"
        "},"
        "\"use_hash_lookup\":{\"type\":\"boolean\","
            "\"description\":\"Benchmark O(1) HashMap lookup against O(n) linear scan. Default: true\"},"
        "\"prob_tolerance\":{\"type\":\"number\","
            "\"description\":\"Tolerance for probability sum validation per asset. "
                "Default: 1e-4 (0.01%%). Clamped to [1e-10, 0.1]\"},"
        "\"request_id\":{\"type\":\"string\"}"
    "},"
    "\"required\":[\"assets\"]"
    "}";

/* ============================================================================
 * Tool catalog struct
 * ============================================================================
 */

typedef struct {
    const char *name;
    const char *description;
    const char *input_schema_json;
} finbench_mcp_tool_t;

static const finbench_mcp_tool_t finbench_mcp_tools[] = {
    {
        "portfolioVariance",
        "Calculate portfolio variance using O(n^2) covariance matrix multiplication: "
        "sigma_p^2 = sum_i sum_j w_i * w_j * sigma_ij. "
        "Returns variance, volatility, annualized volatility (sigma * sqrt(n_periods_per_year)), "
        "matrix operation count, and microsecond timing. "
        "Target: 500 assets in ~5ms on resource-constrained hardware.",
        SCHEMA_PORTFOLIO_VARIANCE
    },
    {
        "monteCarlo",
        "Monte Carlo Value at Risk (VaR) simulation using Geometric Brownian Motion: "
        "S(t+dt) = S(t) * exp((mu - sigma^2/2)*dt + sigma*sqrt(dt)*Z), where Z ~ N(0,1). "
        "Returns VaR at caller-specified percentiles, CVaR (Expected Shortfall at 95%%), "
        "max drawdown, probability of profit, and simulations-per-second throughput. "
        "Uses xorshift128+ PRNG + Box-Muller transform for high-throughput random number generation.",
        SCHEMA_MONTE_CARLO
    },
    {
        "scenarioAnalysis",
        "Probability-weighted scenario analysis for a multi-asset portfolio. "
        "Computes expected return E[r_i] = sum_j (p_j * (price_j/current_price - 1)), "
        "upside potential, downside risk, and upside/downside ratio. "
        "Also benchmarks O(1) axutil_hash_t lookup against O(n) array scan — "
        "the same HashMap optimization used in production systems for 500+ asset portfolios.",
        SCHEMA_SCENARIO_ANALYSIS
    },
    { NULL, NULL, NULL }  /* sentinel */
};

/* Maximum request size — prevents DoS via unbounded memory allocation */
#define MAX_MCP_REQUEST_BYTES   (16 * 1024 * 1024)  /* 16 MB per line */
#define MCP_LINE_INITIAL_CAP    4096

/* ============================================================================
 * JSON-RPC 2.0 output helpers
 *
 * All output goes to stdout (the MCP transport channel).
 * Caller must ensure the Axis2/C logger does NOT write to stdout.
 * ============================================================================
 */

/**
 * Write a JSON-RPC 2.0 success response.
 * Takes ownership of result_obj (calls json_object_put on it).
 * id_obj must be non-NULL; borrowed from the request object.
 */
static void mcp_write_result(json_object *id_obj, json_object *result_obj)
{
    json_object *response = json_object_new_object();
    json_object_object_add(response, "jsonrpc",
        json_object_new_string("2.0"));

    /* Bump refcount: response takes a share; request still owns one */
    json_object_get(id_obj);
    json_object_object_add(response, "id", id_obj);

    json_object_object_add(response, "result", result_obj);  /* transfers ownership */

    printf("%s\n",
        json_object_to_json_string_ext(response, JSON_C_TO_STRING_PLAIN));
    fflush(stdout);

    json_object_put(response);  /* releases response + result; id refcount back to 1 */
}

/**
 * Write a JSON-RPC 2.0 error response.
 * id_obj may be NULL (id=null per spec, used for parse errors before id is known).
 */
static void mcp_write_error(json_object *id_obj, int code, const char *message)
{
    json_object *error_obj = json_object_new_object();
    json_object_object_add(error_obj, "code",    json_object_new_int(code));
    json_object_object_add(error_obj, "message", json_object_new_string(message));

    json_object *response = json_object_new_object();
    json_object_object_add(response, "jsonrpc",
        json_object_new_string("2.0"));

    if (id_obj) {
        json_object_get(id_obj);
        json_object_object_add(response, "id", id_obj);
    } else {
        json_object_object_add(response, "id", json_object_new_null());
    }

    json_object_object_add(response, "error", error_obj);

    printf("%s\n",
        json_object_to_json_string_ext(response, JSON_C_TO_STRING_PLAIN));
    fflush(stdout);

    json_object_put(response);
}

/* ============================================================================
 * MCP method handlers
 * ============================================================================
 */

static json_object *mcp_handle_initialize(void)
{
    json_object *result = json_object_new_object();
    json_object_object_add(result, "protocolVersion",
        json_object_new_string(FINBENCH_MCP_PROTOCOL_VERSION));

    json_object *capabilities = json_object_new_object();
    json_object_object_add(capabilities, "tools", json_object_new_object());
    json_object_object_add(result, "capabilities", capabilities);

    json_object *server_info = json_object_new_object();
    json_object_object_add(server_info, "name",
        json_object_new_string(FINBENCH_MCP_SERVER_NAME));
    json_object_object_add(server_info, "version",
        json_object_new_string(FINBENCH_MCP_SERVER_VERSION));
    json_object_object_add(result, "serverInfo", server_info);

    return result;
}

static json_object *mcp_handle_tools_list(void)
{
    json_object *tools_array = json_object_new_array();

    /* Iterate to NULL sentinel — no hardcoded count needed */
    for (const finbench_mcp_tool_t *tool = finbench_mcp_tools;
         tool->name != NULL; tool++) {
        json_object *tool_obj = json_object_new_object();

        json_object_object_add(tool_obj, "name",
            json_object_new_string(tool->name));
        json_object_object_add(tool_obj, "description",
            json_object_new_string(tool->description));

        /* Parse the inlined schema string into a json_object */
        json_object *schema = json_tokener_parse(tool->input_schema_json);
        if (!schema) {
            /* Fallback: minimal valid schema (should never happen) */
            schema = json_object_new_object();
            json_object_object_add(schema, "type", json_object_new_string("object"));
        }
        json_object_object_add(tool_obj, "inputSchema", schema);

        json_object_array_add(tools_array, tool_obj);
    }

    json_object *result = json_object_new_object();
    json_object_object_add(result, "tools", tools_array);
    return result;
}

/**
 * Dispatch tools/call to the appropriate financial operation.
 *
 * @param env        Axis2/C environment for the *_json_only() calls.
 * @param params     The JSON-RPC "params" object (may be NULL).
 * @param out_code   Set to a JSON-RPC error code on failure (0 on success).
 * @param out_msg    Set to a human-readable error message on failure.
 * @return           MCP result object on success, NULL on failure.
 */
static json_object *mcp_handle_tools_call(
    const axutil_env_t *env,
    json_object         *params,
    int                 *out_code,
    const char         **out_msg)
{
    *out_code = 0;
    *out_msg  = NULL;

    if (!params || !json_object_is_type(params, json_type_object)) {
        *out_code = MCP_ERR_INVALID_PARAMS;
        *out_msg  = "params must be an object with name and arguments fields";
        return NULL;
    }

    /* Extract tool name */
    json_object *name_obj = NULL;
    if (!json_object_object_get_ex(params, "name", &name_obj)
            || !json_object_is_type(name_obj, json_type_string)) {
        *out_code = MCP_ERR_INVALID_PARAMS;
        *out_msg  = "params.name is required and must be a string";
        return NULL;
    }
    const char *tool_name = json_object_get_string(name_obj);

    /* Extract arguments — optional; default to empty object.
     * axutil_strdup gives a stable copy independent of json-c's internal buffer
     * (json_object_to_json_string returns a pointer that is only valid until
     *  the next call to that function on *any* object). */
    json_object *args_obj = NULL;
    json_object_object_get_ex(params, "arguments", &args_obj);
    const char *args_json_tmp = args_obj
        ? json_object_to_json_string(args_obj)
        : "{}";
    axis2_char_t *args_json = axutil_strdup(env, args_json_tmp);
    if (!args_json) {
        *out_code = MCP_ERR_INTERNAL_ERROR;
        *out_msg  = "Failed to allocate memory for arguments";
        return NULL;
    }

    /* Dispatch to the matching finbench_*_json_only() function.
     * Each returns an axutil_strdup()'d string — free with AXIS2_FREE(). */
    axis2_char_t *result_json = NULL;

    if (strcmp(tool_name, "portfolioVariance") == 0) {
        result_json = finbench_portfolio_variance_json_only(env, args_json);
    } else if (strcmp(tool_name, "monteCarlo") == 0) {
        result_json = finbench_monte_carlo_json_only(env, args_json);
    } else if (strcmp(tool_name, "scenarioAnalysis") == 0) {
        result_json = finbench_scenario_json_only(env, args_json);
    } else {
        AXIS2_FREE(env->allocator, args_json);
        *out_code = MCP_ERR_METHOD_NOT_FOUND;
        *out_msg  = "Unknown tool name. Available: portfolioVariance, monteCarlo, scenarioAnalysis";
        return NULL;
    }

    AXIS2_FREE(env->allocator, args_json);

    if (!result_json) {
        *out_code = MCP_ERR_INTERNAL_ERROR;
        *out_msg  = "Operation returned a null response";
        return NULL;
    }

    /* Wrap in MCP content envelope: { "content": [{ "type": "text", "text": "..." }] } */
    json_object *content_item = json_object_new_object();
    json_object_object_add(content_item, "type", json_object_new_string("text"));
    json_object_object_add(content_item, "text", json_object_new_string(result_json));

    json_object *content_array = json_object_new_array();
    json_object_array_add(content_array, content_item);

    json_object *result = json_object_new_object();
    json_object_object_add(result, "content", content_array);

    AXIS2_FREE(env->allocator, result_json);

    return result;
}

/* ============================================================================
 * Public API: stdio loop
 * ============================================================================
 */

/*
 * Read one newline-terminated line from stdin into a buffer managed by the
 * Axis2/C allocator. Grows exponentially up to MAX_MCP_REQUEST_BYTES.
 *
 * Returns AXIS2_TRUE and sets *out_buf / *out_len on a complete line.
 * Returns AXIS2_FALSE on EOF or allocation failure.
 * On oversized input (> MAX_MCP_REQUEST_BYTES), drains the line, writes a
 * JSON-RPC error to stdout, and returns AXIS2_FALSE with *out_len == 0.
 */
static axis2_bool_t
mcp_read_line(const axutil_env_t *env,
              axis2_char_t      **buf_inout,
              size_t             *cap_inout,
              size_t             *out_len)
{
    axis2_char_t *buf = *buf_inout;
    size_t cap        = *cap_inout;
    size_t len        = 0;
    int    c;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            /* Strip trailing CR */
            if (len > 0 && buf[len - 1] == '\r') len--;
            buf[len]  = '\0';
            *out_len  = len;
            return AXIS2_TRUE;
        }

        /* Size guard — prevents DoS via unbounded allocation */
        if (len >= MAX_MCP_REQUEST_BYTES) {
            /* Drain remainder of the oversized line */
            while ((c = getchar()) != EOF && c != '\n') {}
            mcp_write_error(NULL, MCP_ERR_INVALID_REQUEST,
                "Request exceeds maximum size (16 MB)");
            *out_len = 0;
            return AXIS2_FALSE;
        }

        /* Grow buffer if needed (doubles each time) */
        if (len + 1 >= cap) {
            size_t new_cap = cap * 2;
            axis2_char_t *new_buf = AXIS2_MALLOC(env->allocator, new_cap);
            if (!new_buf) {
                mcp_write_error(NULL, MCP_ERR_INTERNAL_ERROR,
                    "Failed to grow line buffer");
                while ((c = getchar()) != EOF && c != '\n') {}
                *out_len = 0;
                return AXIS2_FALSE;
            }
            memcpy(new_buf, buf, len);
            AXIS2_FREE(env->allocator, buf);
            buf = new_buf;
            cap = new_cap;
            *buf_inout = buf;
            *cap_inout = cap;
        }

        buf[len++] = (axis2_char_t)c;
    }

    /* EOF without newline — treat as end of session */
    *out_len = 0;
    return AXIS2_FALSE;
}

void finbench_run_mcp_stdio(const axutil_env_t *env)
{
    /*
     * Line buffer managed by the Axis2/C allocator — avoids mixing stdlib
     * malloc/free with any custom allocator Axis2/C may be configured to use.
     * Initial capacity 4 KB; grows up to MAX_MCP_REQUEST_BYTES (16 MB) to
     * handle large portfolioVariance requests (500-asset matrix ~ 2 MB).
     */
    size_t         line_cap = MCP_LINE_INITIAL_CAP;
    axis2_char_t  *line_buf = AXIS2_MALLOC(env->allocator, line_cap);
    size_t         line_len = 0;

    if (!line_buf) {
        mcp_write_error(NULL, MCP_ERR_INTERNAL_ERROR,
            "Failed to allocate initial line buffer");
        return;
    }

    while (mcp_read_line(env, &line_buf, &line_cap, &line_len)) {
        if (line_len == 0) continue;

        /* ── Parse JSON-RPC 2.0 request ─────────────────────────────── */
        json_object *req = json_tokener_parse(line_buf);
        if (!req) {
            /* id is unknown on parse failure — use null per spec */
            mcp_write_error(NULL, MCP_ERR_PARSE_ERROR, "Parse error: invalid JSON");
            continue;
        }

        /* ── Check for "id" — absent means notification, no response ── */
        json_object *id_obj = NULL;
        if (!json_object_object_get_ex(req, "id", &id_obj)) {
            /* MCP notifications/initialized and similar — silently consume */
            json_object_put(req);
            continue;
        }

        /* ── Extract "method" ────────────────────────────────────────── */
        json_object *method_obj = NULL;
        if (!json_object_object_get_ex(req, "method", &method_obj)
                || !json_object_is_type(method_obj, json_type_string)) {
            mcp_write_error(id_obj, MCP_ERR_INVALID_REQUEST,
                "method is required and must be a string");
            json_object_put(req);
            continue;
        }
        const char *method = json_object_get_string(method_obj);

        /* ── Extract optional "params" ───────────────────────────────── */
        json_object *params_obj = NULL;
        json_object_object_get_ex(req, "params", &params_obj);

        /* ── Dispatch ────────────────────────────────────────────────── */
        if (strcmp(method, "initialize") == 0) {
            mcp_write_result(id_obj, mcp_handle_initialize());

        } else if (strcmp(method, "tools/list") == 0) {
            mcp_write_result(id_obj, mcp_handle_tools_list());

        } else if (strcmp(method, "tools/call") == 0) {
            int         err_code = 0;
            const char *err_msg  = NULL;
            json_object *result  = mcp_handle_tools_call(env, params_obj,
                                                          &err_code, &err_msg);
            if (result) {
                mcp_write_result(id_obj, result);
            } else {
                mcp_write_error(id_obj, err_code,
                    err_msg ? err_msg : "Internal error");
            }

        } else {
            mcp_write_error(id_obj, MCP_ERR_METHOD_NOT_FOUND, "Method not found");
        }

        json_object_put(req);  /* release request + all its members */
    }

    AXIS2_FREE(env->allocator, line_buf);
}
