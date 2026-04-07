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
 * @file financial_benchmark_service.c
 * @brief Apache Axis2/C HTTP/2 Financial Benchmark Service Implementation
 *
 * Demonstrates Axis2/C performance on resource-constrained hardware.
 * Target: Old Android phones (2GB RAM) where Java/WildFly cannot run.
 *
 * Key Algorithms:
 * 1. Portfolio Variance - O(n²) covariance matrix multiplication
 * 2. Monte Carlo VaR - Compute-intensive random walk simulation
 * 3. Scenario Analysis - Hash table lookups vs linear search
 */

#include "financial_benchmark_service.h"
#include <axutil_string.h>
#include <axutil_utils.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

/* ============================================================================
 * Internal Utility Functions
 * ============================================================================
 */

/**
 * Get current time in microseconds
 */
static long get_time_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000L + tv.tv_usec;
}

/**
 * Simple xorshift128+ PRNG for Monte Carlo (fast, good quality)
 */
typedef struct {
    uint64_t s[2];
} xorshift128plus_state;

static uint64_t xorshift128plus(xorshift128plus_state *state)
{
    uint64_t s1 = state->s[0];
    const uint64_t s0 = state->s[1];
    state->s[0] = s0;
    s1 ^= s1 << 23;
    state->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return state->s[1] + s0;
}

/**
 * Generate uniform random double [0, 1)
 */
static double rand_uniform(xorshift128plus_state *state)
{
    return (xorshift128plus(state) >> 11) * (1.0 / 9007199254740992.0);
}

/**
 * Generate standard normal random variable using Box-Muller transform
 */
static double rand_normal(xorshift128plus_state *state)
{
    double u1 = rand_uniform(state);
    double u2 = rand_uniform(state);
    /* Avoid log(0) */
    while (u1 <= 1e-10) u1 = rand_uniform(state);
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

/**
 * Comparison function for qsort (doubles)
 */
static int compare_doubles(const void *a, const void *b)
{
    double da = *(const double*)a;
    double db = *(const double*)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

/* ============================================================================
 * Memory Usage Tracking
 * ============================================================================
 */

AXIS2_EXTERN int AXIS2_CALL
finbench_get_memory_usage_kb(void)
{
#ifdef __linux__
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return (int)(usage.ru_maxrss); /* Already in KB on Linux */
    }
#endif
    return 0;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_get_device_info(const axutil_env_t *env)
{
    char buffer[512];

#ifdef __ANDROID__
    snprintf(buffer, sizeof(buffer),
        "Android Device (Axis2/C 2.0 HTTP/2 JSON)");
#elif defined(__linux__)
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        snprintf(buffer, sizeof(buffer),
            "Linux (RAM: %lu MB, Axis2/C 2.0 HTTP/2 JSON)",
            si.totalram / (1024 * 1024));
    } else {
        snprintf(buffer, sizeof(buffer), "Linux (Axis2/C 2.0 HTTP/2 JSON)");
    }
#else
    snprintf(buffer, sizeof(buffer), "Unknown Platform (Axis2/C 2.0 HTTP/2 JSON)");
#endif

    return axutil_strdup(env, buffer);
}

/* ============================================================================
 * Portfolio Variance Implementation
 *
 * Core calculation: σ²_p = Σ_i Σ_j w_i * w_j * σ_ij
 * This is O(n²) - common for correlation calculations.
 * ============================================================================
 */

AXIS2_EXTERN finbench_portfolio_variance_request_t* AXIS2_CALL
finbench_portfolio_variance_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    finbench_portfolio_variance_request_t *request = NULL;
    json_object *json_obj = NULL;
    json_object *value_obj = NULL;
    json_object *array_obj = NULL;
    int i, j;

    if (!env || !json_string) {
        return NULL;
    }

    json_obj = json_tokener_parse(json_string);
    if (!json_obj) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench: Failed to parse JSON request");
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(finbench_portfolio_variance_request_t));
    if (!request) {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(finbench_portfolio_variance_request_t));

    /* Extract n_assets - or infer from weights array length */
    if (json_object_object_get_ex(json_obj, "n_assets", &value_obj)) {
        request->n_assets = json_object_get_int(value_obj);
    } else if (json_object_object_get_ex(json_obj, "weights", &array_obj) &&
               json_object_is_type(array_obj, json_type_array)) {
        /* Infer n_assets from weights array length */
        request->n_assets = json_object_array_length(array_obj);
        AXIS2_LOG_INFO(env->log,
            "FinBench: Inferred n_assets=%d from weights array", request->n_assets);
    }

    /* Validate n_assets */
    if (request->n_assets <= 0 || request->n_assets > FINBENCH_MAX_ASSETS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench: Invalid n_assets: %d (max: %d)",
            request->n_assets, FINBENCH_MAX_ASSETS);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    /* Allocate weights array */
    request->weights = AXIS2_MALLOC(env->allocator,
        request->n_assets * sizeof(double));
    if (!request->weights) {
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    /* Extract weights — record actual length for dimension validation */
    if (json_object_object_get_ex(json_obj, "weights", &array_obj) &&
        json_object_is_type(array_obj, json_type_array)) {
        int len = json_object_array_length(array_obj);
        request->weights_provided = len;
        for (i = 0; i < request->n_assets && i < len; i++) {
            json_object *elem = json_object_array_get_idx(array_obj, i);
            request->weights[i] = json_object_get_double(elem);
        }
    }

    /* Allocate covariance matrix (n × n) */
    size_t matrix_size = (size_t)request->n_assets * request->n_assets;
    request->covariance_matrix = AXIS2_MALLOC(env->allocator,
        matrix_size * sizeof(double));
    if (!request->covariance_matrix) {
        AXIS2_FREE(env->allocator, request->weights);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    /* Extract covariance_matrix - supports both flat and 2D array formats */
    if (json_object_object_get_ex(json_obj, "covariance_matrix", &array_obj) &&
        json_object_is_type(array_obj, json_type_array)) {
        int outer_len = json_object_array_length(array_obj);

        /* Check if first element is an array (2D format) or number (flat format) */
        json_object *first_elem = json_object_array_get_idx(array_obj, 0);

        if (first_elem && json_object_is_type(first_elem, json_type_array)) {
            /* 2D array format: [[row0], [row1], ...] — record as flat element count */
            AXIS2_LOG_INFO(env->log,
                "FinBench: Parsing 2D covariance matrix format");
            int total_elements = 0;
            for (i = 0; i < request->n_assets && i < outer_len; i++) {
                json_object *row = json_object_array_get_idx(array_obj, i);
                if (row && json_object_is_type(row, json_type_array)) {
                    int row_len = json_object_array_length(row);
                    total_elements += row_len;
                    for (j = 0; j < request->n_assets && j < row_len; j++) {
                        json_object *cell = json_object_array_get_idx(row, j);
                        request->covariance_matrix[i * request->n_assets + j] =
                            json_object_get_double(cell);
                    }
                }
            }
            /* Account for any rows beyond n_assets (for mismatch detection) */
            for (i = request->n_assets; i < outer_len; i++) {
                json_object *row = json_object_array_get_idx(array_obj, i);
                if (row && json_object_is_type(row, json_type_array))
                    total_elements += json_object_array_length(row);
            }
            request->matrix_elements_provided = total_elements;
        } else {
            /* Flat array format: [row0_col0, row0_col1, ..., row1_col0, ...] */
            AXIS2_LOG_INFO(env->log,
                "FinBench: Parsing flat covariance matrix format");
            request->matrix_elements_provided = outer_len;
            for (i = 0; i < (int)matrix_size && i < outer_len; i++) {
                json_object *elem = json_object_array_get_idx(array_obj, i);
                request->covariance_matrix[i] = json_object_get_double(elem);
            }
        }
    }

    /* Extract request_id (optional) */
    if (json_object_object_get_ex(json_obj, "request_id", &value_obj)) {
        const char *rid = json_object_get_string(value_obj);
        if (rid) {
            request->request_id = axutil_strdup(env, rid);
        }
    }

    /* normalize_weights — default false; set true to rescale weights to sum 1.0 */
    if (json_object_object_get_ex(json_obj, "normalize_weights", &value_obj)) {
        request->normalize_weights = json_object_get_boolean(value_obj)
            ? AXIS2_TRUE : AXIS2_FALSE;
    } else {
        request->normalize_weights = AXIS2_FALSE;
    }

    /* n_periods_per_year — default 252 (equity trading days) */
    if (json_object_object_get_ex(json_obj, "n_periods_per_year", &value_obj)) {
        int npy = json_object_get_int(value_obj);
        request->n_periods_per_year = (npy > 0) ? npy : 252;
    } else {
        request->n_periods_per_year = 252;
    }

    json_object_put(json_obj);
    return request;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_portfolio_variance_request_free(
    finbench_portfolio_variance_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env) return;

    if (request->weights)
        AXIS2_FREE(env->allocator, request->weights);
    if (request->covariance_matrix)
        AXIS2_FREE(env->allocator, request->covariance_matrix);
    if (request->request_id)
        AXIS2_FREE(env->allocator, request->request_id);
    if (request->asset_ids) {
        int i;
        for (i = 0; i < request->n_assets; i++) {
            if (request->asset_ids[i])
                AXIS2_FREE(env->allocator, request->asset_ids[i]);
        }
        AXIS2_FREE(env->allocator, request->asset_ids);
    }

    AXIS2_FREE(env->allocator, request);
}

AXIS2_EXTERN finbench_portfolio_variance_response_t* AXIS2_CALL
finbench_portfolio_variance_response_create(const axutil_env_t *env)
{
    finbench_portfolio_variance_response_t *response;

    response = AXIS2_MALLOC(env->allocator,
        sizeof(finbench_portfolio_variance_response_t));
    if (response) {
        memset(response, 0, sizeof(finbench_portfolio_variance_response_t));
    }
    return response;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_portfolio_variance_response_free(
    finbench_portfolio_variance_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env) return;

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);
    if (response->error_message)
        AXIS2_FREE(env->allocator, response->error_message);
    if (response->request_id)
        AXIS2_FREE(env->allocator, response->request_id);
    if (response->device_info)
        AXIS2_FREE(env->allocator, response->device_info);

    AXIS2_FREE(env->allocator, response);
}

/**
 * Core Portfolio Variance Calculation
 *
 * This is the KEY BENCHMARK: O(n²) matrix multiplication
 * Demonstrates C performance vs Java on constrained hardware.
 */
AXIS2_EXTERN finbench_portfolio_variance_response_t* AXIS2_CALL
finbench_calculate_portfolio_variance(
    const axutil_env_t *env,
    finbench_portfolio_variance_request_t *request)
{
    finbench_portfolio_variance_response_t *response = NULL;
    long start_time, end_time;
    int i, j, n;
    double variance = 0.0;
    long ops = 0;

    response = finbench_portfolio_variance_response_create(env);
    if (!response) return NULL;

    if (!request || !request->weights || !request->covariance_matrix) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, "Invalid request parameters");
        return response;
    }

    n = request->n_assets;

    /* -----------------------------------------------------------------------
     * Dimension validation — the kind of check a Python quant will probe
     * immediately. Mismatched or absent arrays silently produce wrong variance
     * if not caught here: n_assets=500 with 100 weights zero-fills the rest,
     * giving a mathematically meaningless result with no indication of bad
     * input. weights_provided == 0 means the "weights" key was absent — that
     * is also an error, not a license to compute variance with all-zero weights.
     * ----------------------------------------------------------------------- */
    if (request->weights_provided == 0) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "Missing required field: \"weights\" array (n_assets elements summing to 1.0).");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench portfolioVariance: weights array missing from request");
        return response;
    }

    if (request->weights_provided != n) {
        char err_buf[256];
        snprintf(err_buf, sizeof(err_buf),
            "weights array length %d != n_assets %d. "
            "Provide exactly n_assets weights (or set normalize_weights=true to rescale).",
            request->weights_provided, n);
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, err_buf);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench portfolioVariance: %s", err_buf);
        return response;
    }

    if (request->matrix_elements_provided == 0) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "Missing required field: \"covariance_matrix\" "
            "(n_assets*n_assets elements as flat array or n_assets x n_assets 2D array).");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench portfolioVariance: covariance_matrix missing from request");
        return response;
    }

    if (request->matrix_elements_provided != n * n) {
        char err_buf[256];
        snprintf(err_buf, sizeof(err_buf),
            "covariance_matrix element count %d != n_assets² (%d×%d=%d). "
            "Provide a flat array of n_assets² elements or an n_assets×n_assets 2D array.",
            request->matrix_elements_provided, n, n, n * n);
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, err_buf);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench portfolioVariance: %s", err_buf);
        return response;
    }

    /* -----------------------------------------------------------------------
     * Optional weight normalization.
     *
     * Compute weight sum first; if normalize_weights=true and sum != 1.0,
     * rescale all weights in-place. This lets callers pass unnormalized
     * exposures (e.g., notional values) without a preprocessing step.
     * ----------------------------------------------------------------------- */
    double weight_sum = 0.0;
    for (i = 0; i < n; i++) {
        weight_sum += request->weights[i];
    }
    response->weight_sum = weight_sum;
    response->weights_normalized = AXIS2_FALSE;

    if (request->normalize_weights) {
        if (weight_sum <= 0.0) {
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env,
                "normalize_weights=true but weights sum to zero or negative. "
                "Cannot normalize a zero-weight portfolio.");
            return response;
        }
        if (fabs(weight_sum - 1.0) > 1e-10) {
            for (i = 0; i < n; i++) {
                request->weights[i] /= weight_sum;
            }
            response->weights_normalized = AXIS2_TRUE;
            AXIS2_LOG_INFO(env->log,
                "FinBench portfolioVariance: normalized weights (sum was %.8f)", weight_sum);
        }
    } else {
        /* Reject clearly non-unit weights (tolerance 1e-6 — tighter than prob check) */
        if (fabs(weight_sum - 1.0) > 1e-4) {
            char err_buf[256];
            snprintf(err_buf, sizeof(err_buf),
                "weights sum to %.8f, expected 1.0 (tolerance 1e-4). "
                "Pass normalize_weights=true to rescale automatically.",
                weight_sum);
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env, err_buf);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "FinBench portfolioVariance: %s", err_buf);
            return response;
        }
    }

    start_time = get_time_us();

    /*
     * Core O(n²) calculation:
     * σ²_p = Σ_i Σ_j w_i * w_j * σ_ij
     *
     * This is common for correlation calculations.
     * On a 500-asset portfolio, this is 250,000 operations.
     */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            variance += request->weights[i] *
                        request->weights[j] *
                        request->covariance_matrix[i * n + j];
            ops++;
        }
    }

    end_time = get_time_us();

    int npy = (request->n_periods_per_year > 0) ? request->n_periods_per_year : 252;

    /*
     * Clamp variance to zero before sqrt. Floating-point cancellation in the
     * O(n²) sum can yield a tiny negative result (e.g., -1e-17) for a
     * numerically near-zero portfolio variance, producing NaN from sqrt().
     */
    if (variance < 0.0) variance = 0.0;
    double volatility = sqrt(variance);

    /* Populate response */
    response->status = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->portfolio_variance = variance;
    response->portfolio_volatility = volatility;
    response->annualized_volatility = volatility * sqrt((double)npy);
    response->calc_time_us = end_time - start_time;
    response->memory_used_kb = finbench_get_memory_usage_kb();
    response->matrix_operations = ops;

    if (response->calc_time_us > 0) {
        response->ops_per_second = (double)ops / (response->calc_time_us / 1000000.0);
    }

    if (request->request_id) {
        response->request_id = axutil_strdup(env, request->request_id);
    }

    response->device_info = finbench_get_device_info(env);

    AXIS2_LOG_INFO(env->log,
        "FinBench: Portfolio variance calculated for %d assets in %ld us "
        "(variance=%.6f, ops=%ld)",
        n, response->calc_time_us, variance, ops);

    return response;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_portfolio_variance_response_to_json(
    const finbench_portfolio_variance_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_resp;
    const char *json_str;
    axis2_char_t *result;

    if (!response || !env) return NULL;

    json_resp = json_object_new_object();

    json_object_object_add(json_resp, "status",
        json_object_new_string(response->status ? response->status : "UNKNOWN"));

    json_object_object_add(json_resp, "portfolio_variance",
        json_object_new_double(response->portfolio_variance));

    json_object_object_add(json_resp, "portfolio_volatility",
        json_object_new_double(response->portfolio_volatility));

    json_object_object_add(json_resp, "annualized_volatility",
        json_object_new_double(response->annualized_volatility));

    json_object_object_add(json_resp, "calc_time_us",
        json_object_new_int64(response->calc_time_us));

    json_object_object_add(json_resp, "memory_used_kb",
        json_object_new_int(response->memory_used_kb));

    json_object_object_add(json_resp, "matrix_operations",
        json_object_new_int64(response->matrix_operations));

    json_object_object_add(json_resp, "ops_per_second",
        json_object_new_double(response->ops_per_second));

    json_object_object_add(json_resp, "weight_sum",
        json_object_new_double(response->weight_sum));

    json_object_object_add(json_resp, "weights_normalized",
        json_object_new_boolean(response->weights_normalized));

    if (response->request_id) {
        json_object_object_add(json_resp, "request_id",
            json_object_new_string(response->request_id));
    }

    if (response->device_info) {
        json_object_object_add(json_resp, "device_info",
            json_object_new_string(response->device_info));
    }

    if (response->error_message) {
        json_object_object_add(json_resp, "error_message",
            json_object_new_string(response->error_message));
    }

    /* Add comparison note for demo */
    json_object_object_add(json_resp, "comparison_note",
        json_object_new_string(
            "Enterprise Java/WildFly deployments require 16-32GB RAM per server. "
            "This device runs Axis2/C in ~30MB."));

    json_str = json_object_to_json_string_ext(json_resp, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, json_str);
    json_object_put(json_resp);

    return result;
}

/**
 * HTTP/2 JSON endpoint for portfolio variance
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_portfolio_variance_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    finbench_portfolio_variance_request_t *request;
    finbench_portfolio_variance_response_t *response;
    axis2_char_t *json_response;

    request = finbench_portfolio_variance_request_create_from_json(env, json_request);
    if (!request) {
        return axutil_strdup(env,
            "{\"status\":\"FAILED\",\"error_message\":"
            "\"Failed to parse portfolioVariance request. "
            "Required fields: n_assets (int), weights (float[]), "
            "covariance_matrix (float[n²] flat or float[n][n] 2D). "
            "Optional: normalize_weights (bool), n_periods_per_year (int, default 252), "
            "request_id (string).\"}");
    }

    response = finbench_calculate_portfolio_variance(env, request);
    json_response = finbench_portfolio_variance_response_to_json(response, env);

    finbench_portfolio_variance_request_free(request, env);
    finbench_portfolio_variance_response_free(response, env);

    return json_response;
}

/* ============================================================================
 * Monte Carlo Simulation Implementation
 * ============================================================================
 */

AXIS2_EXTERN finbench_monte_carlo_request_t* AXIS2_CALL
finbench_monte_carlo_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    finbench_monte_carlo_request_t *request = NULL;
    json_object *json_obj = NULL;
    json_object *value_obj = NULL;

    if (!env || !json_string) return NULL;

    json_obj = json_tokener_parse(json_string);
    if (!json_obj) return NULL;

    request = AXIS2_MALLOC(env->allocator, sizeof(finbench_monte_carlo_request_t));
    if (!request) {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(finbench_monte_carlo_request_t));

    /* Extract parameters with defaults */
    if (json_object_object_get_ex(json_obj, "n_simulations", &value_obj)) {
        request->n_simulations = json_object_get_int(value_obj);
    } else {
        request->n_simulations = 10000; /* Default */
    }

    if (json_object_object_get_ex(json_obj, "n_periods", &value_obj)) {
        request->n_periods = json_object_get_int(value_obj);
    } else {
        request->n_periods = 252; /* Trading days in a year */
    }

    if (json_object_object_get_ex(json_obj, "initial_value", &value_obj)) {
        request->initial_value = json_object_get_double(value_obj);
    } else {
        request->initial_value = 1000000.0; /* $1M default */
    }

    if (json_object_object_get_ex(json_obj, "expected_return", &value_obj)) {
        request->expected_return = json_object_get_double(value_obj);
    } else {
        request->expected_return = 0.08; /* 8% annual */
    }

    if (json_object_object_get_ex(json_obj, "volatility", &value_obj)) {
        request->volatility = json_object_get_double(value_obj);
    } else {
        request->volatility = 0.20; /* 20% annual */
    }

    if (json_object_object_get_ex(json_obj, "random_seed", &value_obj)) {
        request->random_seed = (uint32_t)json_object_get_int(value_obj);
    }

    /* n_periods_per_year — controls GBM time step dt = 1/n_periods_per_year */
    if (json_object_object_get_ex(json_obj, "n_periods_per_year", &value_obj)) {
        int npy = json_object_get_int(value_obj);
        request->n_periods_per_year = (npy > 0) ? npy : 252;
    } else {
        request->n_periods_per_year = 252;
    }

    /* percentiles — array of tail levels in (0,1); default {0.01, 0.05} */
    {
        json_object *pct_arr = NULL;
        if (json_object_object_get_ex(json_obj, "percentiles", &pct_arr) &&
            json_object_is_type(pct_arr, json_type_array)) {
            int n_pct = json_object_array_length(pct_arr);
            if (n_pct > FINBENCH_MAX_PERCENTILES) n_pct = FINBENCH_MAX_PERCENTILES;
            request->n_percentiles = 0;
            int pi;
            for (pi = 0; pi < n_pct; pi++) {
                json_object *pct_elem = json_object_array_get_idx(pct_arr, pi);
                double p = json_object_get_double(pct_elem);
                if (p > 0.0 && p < 1.0) {
                    request->percentiles[request->n_percentiles++] = p;
                }
            }
        } else {
            /* Default: 1% and 5% VaR levels */
            request->percentiles[0] = 0.01;
            request->percentiles[1] = 0.05;
            request->n_percentiles = 2;
        }
    }

    if (json_object_object_get_ex(json_obj, "request_id", &value_obj)) {
        const char *rid = json_object_get_string(value_obj);
        if (rid) request->request_id = axutil_strdup(env, rid);
    }

    /* Validate limits */
    if (request->n_simulations > FINBENCH_MAX_SIMULATIONS) {
        request->n_simulations = FINBENCH_MAX_SIMULATIONS;
    }

    json_object_put(json_obj);
    return request;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_monte_carlo_request_free(
    finbench_monte_carlo_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env) return;
    if (request->request_id) AXIS2_FREE(env->allocator, request->request_id);
    AXIS2_FREE(env->allocator, request);
}

AXIS2_EXTERN finbench_monte_carlo_response_t* AXIS2_CALL
finbench_monte_carlo_response_create(const axutil_env_t *env)
{
    finbench_monte_carlo_response_t *response;
    response = AXIS2_MALLOC(env->allocator, sizeof(finbench_monte_carlo_response_t));
    if (response) {
        memset(response, 0, sizeof(finbench_monte_carlo_response_t));
    }
    return response;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_monte_carlo_response_free(
    finbench_monte_carlo_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env) return;
    if (response->status) AXIS2_FREE(env->allocator, response->status);
    if (response->error_message) AXIS2_FREE(env->allocator, response->error_message);
    if (response->request_id) AXIS2_FREE(env->allocator, response->request_id);
    AXIS2_FREE(env->allocator, response);
}

/**
 * Core Monte Carlo Simulation
 *
 * Simulates portfolio value paths using Geometric Brownian Motion:
 * S(t+dt) = S(t) * exp((μ - σ²/2)*dt + σ*sqrt(dt)*Z)
 *
 * This is COMPUTE-INTENSIVE: n_simulations × n_periods iterations.
 */
AXIS2_EXTERN finbench_monte_carlo_response_t* AXIS2_CALL
finbench_run_monte_carlo(
    const axutil_env_t *env,
    finbench_monte_carlo_request_t *request)
{
    finbench_monte_carlo_response_t *response = NULL;
    xorshift128plus_state rng;
    double *final_values = NULL;
    long start_time, end_time;
    int sim, period;
    double dt, drift, vol_sqrt_dt;
    double sum_final = 0.0, sum_sq = 0.0;
    int profit_count = 0;
    double max_drawdown = 0.0;

    response = finbench_monte_carlo_response_create(env);
    if (!response) return NULL;

    if (!request) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, "Invalid request");
        return response;
    }

    /* Allocate array for final values (needed for percentiles) */
    final_values = AXIS2_MALLOC(env->allocator,
        request->n_simulations * sizeof(double));
    if (!final_values) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, "Memory allocation failed");
        return response;
    }

    /* Initialize PRNG */
    if (request->random_seed != 0) {
        rng.s[0] = request->random_seed;
        rng.s[1] = request->random_seed ^ 0x5DEECE66DL;
    } else {
        rng.s[0] = (uint64_t)time(NULL);
        rng.s[1] = rng.s[0] ^ 0xDEADBEEF;
    }

    /* Pre-calculate constants */
    {
        int npy = (request->n_periods_per_year > 0) ? request->n_periods_per_year : 252;
        dt = 1.0 / (double)npy;
    }
    drift = (request->expected_return - 0.5 * request->volatility * request->volatility) * dt;
    vol_sqrt_dt = request->volatility * sqrt(dt);

    start_time = get_time_us();

    /*
     * Main simulation loop - THIS IS THE COMPUTE BENCHMARK
     * n_simulations × n_periods iterations
     */
    for (sim = 0; sim < request->n_simulations; sim++) {
        double value = request->initial_value;
        double peak = value;
        double sim_max_drawdown = 0.0;

        for (period = 0; period < request->n_periods; period++) {
            double z = rand_normal(&rng);
            value *= exp(drift + vol_sqrt_dt * z);

            /* Track drawdown */
            if (value > peak) {
                peak = value;
            } else {
                double drawdown = (peak - value) / peak;
                if (drawdown > sim_max_drawdown) {
                    sim_max_drawdown = drawdown;
                }
            }
        }

        final_values[sim] = value;
        sum_final += value;
        sum_sq += value * value;

        if (value > request->initial_value) {
            profit_count++;
        }

        if (sim_max_drawdown > max_drawdown) {
            max_drawdown = sim_max_drawdown;
        }
    }

    end_time = get_time_us();

    /* Calculate statistics */
    double mean = sum_final / request->n_simulations;
    double variance = (sum_sq / request->n_simulations) - (mean * mean);

    /* Sort for percentiles */
    qsort(final_values, request->n_simulations, sizeof(double), compare_doubles);

    int n_sims = request->n_simulations;
    int idx_5  = (int)(0.05 * n_sims);
    int idx_1  = (int)(0.01 * n_sims);
    int idx_50 = n_sims / 2;

    /* Calculate CVaR (Expected Shortfall) - average of worst 5% */
    double cvar_sum = 0.0;
    {
        int ci;
        for (ci = 0; ci < idx_5; ci++) {
            cvar_sum += final_values[ci];
        }
    }
    double cvar_95 = (idx_5 > 0) ? (cvar_sum / idx_5) : final_values[0];

    /* Compute caller-requested percentile VaR values */
    response->n_percentiles = 0;
    {
        int pi;
        int n_pct = (request->n_percentiles > FINBENCH_MAX_PERCENTILES)
            ? FINBENCH_MAX_PERCENTILES : request->n_percentiles;
        for (pi = 0; pi < n_pct; pi++) {
            double p = request->percentiles[pi];
            if (p <= 0.0 || p >= 1.0) continue;
            int idx = (int)(p * n_sims);
            if (idx >= n_sims) idx = n_sims - 1;
            response->percentile_levels[response->n_percentiles] = p;
            response->var_at_percentile[response->n_percentiles] =
                request->initial_value - final_values[idx];
            response->n_percentiles++;
        }
    }

    /* Populate response */
    response->status = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->mean_final_value = mean;
    response->median_final_value = final_values[idx_50];
    response->std_dev_final_value = sqrt(variance);
    response->var_95 = request->initial_value - final_values[idx_5];
    response->var_99 = request->initial_value - final_values[idx_1];
    response->cvar_95 = request->initial_value - cvar_95;
    response->max_drawdown = max_drawdown;
    response->prob_profit = (double)profit_count / request->n_simulations;
    response->calc_time_us = end_time - start_time;
    response->memory_used_kb = finbench_get_memory_usage_kb();

    if (response->calc_time_us > 0) {
        response->simulations_per_second =
            (double)request->n_simulations / (response->calc_time_us / 1000000.0);
    }

    if (request->request_id) {
        response->request_id = axutil_strdup(env, request->request_id);
    }

    AXIS2_FREE(env->allocator, final_values);

    AXIS2_LOG_INFO(env->log,
        "FinBench: Monte Carlo completed %d sims × %d periods in %ld us "
        "(%.0f sims/sec, VaR95=%.2f)",
        request->n_simulations, request->n_periods,
        response->calc_time_us, response->simulations_per_second,
        response->var_95);

    return response;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_monte_carlo_response_to_json(
    const finbench_monte_carlo_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_resp;
    const char *json_str;
    axis2_char_t *result;

    if (!response || !env) return NULL;

    json_resp = json_object_new_object();

    json_object_object_add(json_resp, "status",
        json_object_new_string(response->status ? response->status : "UNKNOWN"));

    json_object_object_add(json_resp, "mean_final_value",
        json_object_new_double(response->mean_final_value));

    json_object_object_add(json_resp, "median_final_value",
        json_object_new_double(response->median_final_value));

    json_object_object_add(json_resp, "std_dev_final_value",
        json_object_new_double(response->std_dev_final_value));

    json_object_object_add(json_resp, "var_95",
        json_object_new_double(response->var_95));

    json_object_object_add(json_resp, "var_99",
        json_object_new_double(response->var_99));

    json_object_object_add(json_resp, "cvar_95",
        json_object_new_double(response->cvar_95));

    json_object_object_add(json_resp, "max_drawdown",
        json_object_new_double(response->max_drawdown));

    json_object_object_add(json_resp, "prob_profit",
        json_object_new_double(response->prob_profit));

    json_object_object_add(json_resp, "calc_time_us",
        json_object_new_int64(response->calc_time_us));

    json_object_object_add(json_resp, "simulations_per_second",
        json_object_new_double(response->simulations_per_second));

    json_object_object_add(json_resp, "memory_used_kb",
        json_object_new_int(response->memory_used_kb));

    /* Emit caller-specified percentile VaR values as a structured array */
    if (response->n_percentiles > 0) {
        json_object *pct_array = json_object_new_array();
        int pi;
        for (pi = 0; pi < response->n_percentiles; pi++) {
            json_object *entry = json_object_new_object();
            json_object_object_add(entry, "percentile",
                json_object_new_double(response->percentile_levels[pi]));
            json_object_object_add(entry, "var",
                json_object_new_double(response->var_at_percentile[pi]));
            json_object_array_add(pct_array, entry);
        }
        json_object_object_add(json_resp, "percentile_vars", pct_array);
    }

    if (response->request_id) {
        json_object_object_add(json_resp, "request_id",
            json_object_new_string(response->request_id));
    }

    if (response->error_message) {
        json_object_object_add(json_resp, "error_message",
            json_object_new_string(response->error_message));
    }

    json_str = json_object_to_json_string_ext(json_resp, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, json_str);
    json_object_put(json_resp);

    return result;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_monte_carlo_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    finbench_monte_carlo_request_t *request;
    finbench_monte_carlo_response_t *response;
    axis2_char_t *json_response;

    request = finbench_monte_carlo_request_create_from_json(env, json_request);
    if (!request) {
        return axutil_strdup(env,
            "{\"status\":\"FAILED\",\"error_message\":"
            "\"Failed to parse monteCarlo request. "
            "All fields optional with defaults: "
            "n_simulations (int, default 10000, max 1000000), "
            "n_periods (int, default 252), "
            "initial_value (float, default 1000000.0), "
            "expected_return (float, default 0.08), "
            "volatility (float, default 0.20), "
            "random_seed (int, default 0=random), "
            "n_periods_per_year (int, default 252), "
            "percentiles (float[], default [0.01, 0.05]).\"}");
    }

    response = finbench_run_monte_carlo(env, request);
    json_response = finbench_monte_carlo_response_to_json(response, env);

    finbench_monte_carlo_request_free(request, env);
    finbench_monte_carlo_response_free(response, env);

    return json_response;
}

/* ============================================================================
 * Scenario Analysis Implementation
 *
 * Demonstrates O(1) hash table vs O(n) linear search for asset lookups.
 * Mirrors DPT v2's Array→Map optimization for 500+ asset portfolios.
 *
 * Financial calculation per asset:
 *   expected_return = Σ( probability_i × (scenario_price_i / current_price - 1) )
 *   upside = Σ( probability_i × max(0, price_i - current) × position_size )  [gains only]
 *   downside = Σ( probability_i × max(0, current - price_i) × position_size ) [losses only]
 *
 * Benchmark: N_LOOKUPS = n_assets × 10 lookups measured for both methods.
 * ============================================================================
 */

AXIS2_EXTERN finbench_scenario_request_t* AXIS2_CALL
finbench_scenario_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    finbench_scenario_request_t *request = NULL;
    json_object *json_obj = NULL;
    json_object *assets_arr = NULL;
    json_object *value_obj = NULL;
    int i, j, n_assets;

    if (!env || !json_string) return NULL;

    json_obj = json_tokener_parse(json_string);
    if (!json_obj) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "ScenarioAnalysis: Failed to parse JSON request");
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(finbench_scenario_request_t));
    if (!request) {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(finbench_scenario_request_t));

    /* use_hash_lookup flag (default: run both, report both) */
    if (json_object_object_get_ex(json_obj, "use_hash_lookup", &value_obj)) {
        request->use_hash_lookup = json_object_get_boolean(value_obj)
            ? AXIS2_TRUE : AXIS2_FALSE;
    } else {
        request->use_hash_lookup = AXIS2_TRUE;
    }

    /*
     * prob_tolerance — probability sum tolerance per asset.
     * Default 1e-4; 0.0 → default; clamped to [1e-10, 0.1].
     */
    if (json_object_object_get_ex(json_obj, "prob_tolerance", &value_obj)) {
        double tol = json_object_get_double(value_obj);
        if (tol <= 0.0) tol = 1e-4;
        if (tol > 0.1)  tol = 0.1;
        request->prob_tolerance = tol;
    } else {
        request->prob_tolerance = 1e-4;
    }

    /* request_id */
    if (json_object_object_get_ex(json_obj, "request_id", &value_obj)) {
        const char *rid = json_object_get_string(value_obj);
        if (rid) request->request_id = axutil_strdup(env, rid);
    }

    /* assets array */
    if (!json_object_object_get_ex(json_obj, "assets", &assets_arr) ||
        !json_object_is_type(assets_arr, json_type_array)) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "ScenarioAnalysis: Missing or invalid 'assets' array");
        if (request->request_id) AXIS2_FREE(env->allocator, request->request_id);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    n_assets = json_object_array_length(assets_arr);
    if (n_assets <= 0 || n_assets > FINBENCH_MAX_ASSETS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "ScenarioAnalysis: n_assets=%d out of range (max %d)",
            n_assets, FINBENCH_MAX_ASSETS);
        if (request->request_id) AXIS2_FREE(env->allocator, request->request_id);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    request->n_assets = n_assets;
    request->assets = AXIS2_MALLOC(env->allocator,
        n_assets * sizeof(finbench_asset_scenario_t));
    if (!request->assets) {
        if (request->request_id) AXIS2_FREE(env->allocator, request->request_id);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }
    memset(request->assets, 0, n_assets * sizeof(finbench_asset_scenario_t));

    for (i = 0; i < n_assets; i++) {
        json_object *asset_obj = json_object_array_get_idx(assets_arr, i);
        json_object *field_obj = NULL;
        finbench_asset_scenario_t *a = &request->assets[i];

        if (!asset_obj) continue;

        if (json_object_object_get_ex(asset_obj, "asset_id", &field_obj))
            a->asset_id = (int64_t)json_object_get_int64(field_obj);
        else
            a->asset_id = (int64_t)(i + 1); /* auto-assign if missing */

        if (json_object_object_get_ex(asset_obj, "current_price", &field_obj))
            a->current_price = json_object_get_double(field_obj);

        if (json_object_object_get_ex(asset_obj, "position_size", &field_obj))
            a->position_size = json_object_get_double(field_obj);

        /* scenarios array: [{price, probability}, ...] */
        json_object *sc_arr = NULL;
        if (json_object_object_get_ex(asset_obj, "scenarios", &sc_arr) &&
            json_object_is_type(sc_arr, json_type_array)) {
            int n_sc = json_object_array_length(sc_arr);
            if (n_sc > FINBENCH_MAX_SCENARIOS) n_sc = FINBENCH_MAX_SCENARIOS;
            a->n_scenarios = n_sc;
            for (j = 0; j < n_sc; j++) {
                json_object *sc = json_object_array_get_idx(sc_arr, j);
                json_object *sc_field = NULL;
                if (!sc) continue;
                if (json_object_object_get_ex(sc, "price", &sc_field))
                    a->scenario_prices[j] = json_object_get_double(sc_field);
                if (json_object_object_get_ex(sc, "probability", &sc_field))
                    a->probabilities[j] = json_object_get_double(sc_field);
            }
        }
    }

    json_object_put(json_obj);
    AXIS2_LOG_INFO(env->log, "ScenarioAnalysis: Parsed %d assets", n_assets);
    return request;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_scenario_request_free(
    finbench_scenario_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env) return;
    if (request->assets) AXIS2_FREE(env->allocator, request->assets);
    if (request->request_id) AXIS2_FREE(env->allocator, request->request_id);
    AXIS2_FREE(env->allocator, request);
}

AXIS2_EXTERN finbench_scenario_response_t* AXIS2_CALL
finbench_scenario_response_create(const axutil_env_t *env)
{
    finbench_scenario_response_t *response;
    response = AXIS2_MALLOC(env->allocator, sizeof(finbench_scenario_response_t));
    if (response) memset(response, 0, sizeof(finbench_scenario_response_t));
    return response;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_scenario_response_free(
    finbench_scenario_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env) return;
    if (response->status)        AXIS2_FREE(env->allocator, response->status);
    if (response->error_message) AXIS2_FREE(env->allocator, response->error_message);
    if (response->lookup_method) AXIS2_FREE(env->allocator, response->lookup_method);
    if (response->request_id)    AXIS2_FREE(env->allocator, response->request_id);
    AXIS2_FREE(env->allocator, response);
}

/**
 * Core Scenario Analysis Calculation
 *
 * Step 1 — Financial computation: expected return, upside, downside per asset.
 * Step 2 — O(n) linear search benchmark: scan array for each lookup.
 * Step 3 — O(1) hash table benchmark: axutil_hash lookup for each query.
 *
 * Both timings always reported so the comparison is always visible.
 */
AXIS2_EXTERN finbench_scenario_response_t* AXIS2_CALL
finbench_calculate_scenarios(
    const axutil_env_t *env,
    finbench_scenario_request_t *request)
{
    finbench_scenario_response_t *response = NULL;
    axutil_hash_t *hash = NULL;
    long linear_start, linear_end, hash_build_start, hash_build_end;
    long hash_lookup_start, hash_lookup_end;
    int i, j, q;
    double total_upside = 0.0, total_downside = 0.0;
    double portfolio_weighted_value = 0.0;
    double portfolio_expected_return = 0.0;
    double total_position_value = 0.0;
    int n_lookups;
    long linear_found = 0, hash_found = 0;
    char key_buf[32];
    char **hash_keys = NULL; /* per-entry allocated keys (axutil_hash stores ptr, not copy) */

    response = finbench_scenario_response_create(env);
    if (!response) return NULL;

    if (!request || !request->assets || request->n_assets <= 0) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "Invalid request: no assets provided");
        return response;
    }

    /* -----------------------------------------------------------------------
     * Step 1: Validate probability distributions before any computation.
     *
     * Each asset's scenario probabilities must sum to 1.0 (within floating-
     * point tolerance). Without this check, malformed input silently produces
     * wrong expected-return figures — the kind of silent error that corrupts
     * downstream portfolio decisions without any indication that the input
     * data was bad.
     *
     * Tolerance: 1e-4 (0.01%). Looser than machine epsilon and typical
     * floating-point rounding (~1e-6) to accommodate legitimate representation
     * differences in JSON-encoded probabilities, but tight enough to catch
     * missing or miscounted scenarios (e.g., three scenarios summing to 0.85).
     * ----------------------------------------------------------------------- */
    double prob_tolerance = (request->prob_tolerance > 0.0)
        ? request->prob_tolerance : 1e-4;

    for (i = 0; i < request->n_assets; i++) {
        finbench_asset_scenario_t *a = &request->assets[i];
        double prob_sum = 0.0;
        int k;

        if (a->n_scenarios <= 0) continue;

        for (k = 0; k < a->n_scenarios; k++) {
            prob_sum += a->probabilities[k];
        }

        if (prob_sum < (1.0 - prob_tolerance) ||
            prob_sum > (1.0 + prob_tolerance)) {

            char err_buf[256];
            snprintf(err_buf, sizeof(err_buf),
                "Asset index %d (id=%" PRId64 "): scenario probabilities sum to "
                "%.8f, expected 1.0 (tolerance %.2g). "
                "All %d scenario probabilities must sum to exactly 1.0. "
                "Pass prob_tolerance to adjust validation strictness.",
                i, a->asset_id, prob_sum, prob_tolerance,
                a->n_scenarios);
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env, err_buf);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "ScenarioAnalysis: probability validation failed - %s", err_buf);
            return response;
        }
    }

    /* -----------------------------------------------------------------------
     * Step 2: Financial computation — expected return, upside, downside
     * ----------------------------------------------------------------------- */
    for (i = 0; i < request->n_assets; i++) {
        finbench_asset_scenario_t *a = &request->assets[i];
        double asset_expected_return = 0.0;
        double asset_upside = 0.0, asset_downside = 0.0;
        double asset_weighted_value = 0.0;

        if (a->current_price <= 0.0 || a->n_scenarios <= 0) continue;

        for (j = 0; j < a->n_scenarios; j++) {
            double prob = a->probabilities[j];
            double price = a->scenario_prices[j];
            double ret = (price / a->current_price) - 1.0;

            asset_expected_return += prob * ret;
            asset_weighted_value  += prob * price * a->position_size;

            if (price > a->current_price)
                asset_upside   += prob * (price - a->current_price) * a->position_size;
            else if (price < a->current_price)
                asset_downside += prob * (a->current_price - price) * a->position_size;
        }

        double position_value = a->current_price * a->position_size;
        portfolio_expected_return += asset_expected_return * position_value;
        total_position_value      += position_value;
        portfolio_weighted_value  += asset_weighted_value;
        total_upside              += asset_upside;
        total_downside            += asset_downside;
    }

    if (total_position_value > 0.0)
        portfolio_expected_return /= total_position_value;

    /* -----------------------------------------------------------------------
     * Step 3: O(n) linear search benchmark
     * Each query scans the array from index 0 until asset_id matches.
     * n_lookups = n_assets × 10 to amplify the timing difference.
     * ----------------------------------------------------------------------- */
    n_lookups = request->n_assets * 10;
    linear_start = get_time_us();

    for (q = 0; q < n_lookups; q++) {
        int64_t target_id = request->assets[q % request->n_assets].asset_id;
        for (i = 0; i < request->n_assets; i++) {
            if (request->assets[i].asset_id == target_id) {
                linear_found++;
                break;
            }
        }
    }

    linear_end = get_time_us();

    /* -----------------------------------------------------------------------
     * Step 4: O(1) hash table benchmark
     * Build axutil_hash keyed by asset_id string, then perform same lookups.
     * ----------------------------------------------------------------------- */
    /*
     * axutil_hash_set with AXIS2_HASH_KEY_STRING stores the key pointer — it does
     * NOT copy the string. Each entry needs its own allocated key so that all
     * n_assets entries in the table have distinct, stable key pointers.
     */
    hash_keys = (char **)AXIS2_MALLOC(env->allocator,
                                      request->n_assets * sizeof(char *));
    if (hash_keys) {
        for (i = 0; i < request->n_assets; i++) hash_keys[i] = NULL;
    } else {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench scenarioAnalysis: hash_keys alloc failed (%d assets) — "
            "hash benchmark will be skipped; only linear search timing reported",
            request->n_assets);
    }

    hash_build_start = get_time_us();
    hash = axutil_hash_make(env);
    if (hash && hash_keys) {
        for (i = 0; i < request->n_assets; i++) {
            snprintf(key_buf, sizeof(key_buf), "%" PRId64,
                     request->assets[i].asset_id);
            hash_keys[i] = axutil_strdup(env, key_buf); /* stable per-entry copy */
            if (hash_keys[i]) {
                axutil_hash_set(hash, hash_keys[i], AXIS2_HASH_KEY_STRING,
                                &request->assets[i]);
            }
        }
    }
    hash_build_end = get_time_us();

    hash_lookup_start = get_time_us();
    if (hash) {
        for (q = 0; q < n_lookups; q++) {
            snprintf(key_buf, sizeof(key_buf), "%" PRId64,
                     request->assets[q % request->n_assets].asset_id);
            finbench_asset_scenario_t *found =
                axutil_hash_get(hash, key_buf, AXIS2_HASH_KEY_STRING);
            if (found) hash_found++;
        }
    }
    hash_lookup_end = get_time_us();

    if (hash) axutil_hash_free(hash, env);
    if (hash_keys) {
        for (i = 0; i < request->n_assets; i++) {
            if (hash_keys[i]) AXIS2_FREE(env->allocator, hash_keys[i]);
        }
        AXIS2_FREE(env->allocator, hash_keys);
        hash_keys = NULL;
    }

    /* -----------------------------------------------------------------------
     * Populate response
     * ----------------------------------------------------------------------- */
    long linear_us = linear_end - linear_start;

    response->status              = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->expected_return     = portfolio_expected_return;
    response->weighted_value      = portfolio_weighted_value;
    response->upside_potential    = total_upside;
    response->downside_risk       = total_downside;
    response->upside_downside_ratio =
        (total_downside > 0.0) ? (total_upside / total_downside) : 0.0;
    response->calc_time_us        = linear_us; /* primary timing: linear */
    response->lookups_performed   = n_lookups;
    response->memory_used_kb      = finbench_get_memory_usage_kb();

    if (linear_us > 0)
        response->lookups_per_second =
            (double)n_lookups / (linear_us / 1000000.0);

    /*
     * Report lookup-only speedup (build cost excluded from comparison — amortized
     * in real workloads where the hash is built once and queried many times).
     */
    long hash_build_us  = hash_build_end  - hash_build_start;
    long hash_lookup_us = hash_lookup_end - hash_lookup_start;
    char method_buf[320];
    snprintf(method_buf, sizeof(method_buf),
        "linear_search_us=%ld hash_lookup_us=%ld lookup_speedup=%.1fx "
        "hash_build_us=%ld "
        "(linear=%ld found, hash=%ld found, n_assets=%d, n_lookups=%d)",
        linear_us, hash_lookup_us,
        (hash_lookup_us > 0) ? (double)linear_us / hash_lookup_us : 0.0,
        hash_build_us,
        linear_found, hash_found,
        request->n_assets, n_lookups);
    response->lookup_method = axutil_strdup(env, method_buf);

    if (request->request_id)
        response->request_id = axutil_strdup(env, request->request_id);

    AXIS2_LOG_INFO(env->log,
        "ScenarioAnalysis: %d assets, %d lookups — linear=%ldus "
        "hash_lookup=%ldus speedup=%.1fx build=%ldus E[r]=%.4f U/D=%.2f",
        request->n_assets, n_lookups, linear_us, hash_lookup_us,
        (hash_lookup_us > 0) ? (double)linear_us / hash_lookup_us : 0.0,
        hash_build_us, portfolio_expected_return,
        response->upside_downside_ratio);

    return response;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_scenario_response_to_json(
    const finbench_scenario_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_resp;
    const char *json_str;
    axis2_char_t *result;

    if (!response || !env) return NULL;

    json_resp = json_object_new_object();

    json_object_object_add(json_resp, "status",
        json_object_new_string(response->status ? response->status : "UNKNOWN"));

    json_object_object_add(json_resp, "expected_return",
        json_object_new_double(response->expected_return));

    json_object_object_add(json_resp, "weighted_value",
        json_object_new_double(response->weighted_value));

    json_object_object_add(json_resp, "upside_potential",
        json_object_new_double(response->upside_potential));

    json_object_object_add(json_resp, "downside_risk",
        json_object_new_double(response->downside_risk));

    json_object_object_add(json_resp, "upside_downside_ratio",
        json_object_new_double(response->upside_downside_ratio));

    json_object_object_add(json_resp, "calc_time_us",
        json_object_new_int64(response->calc_time_us));

    json_object_object_add(json_resp, "lookups_performed",
        json_object_new_int64(response->lookups_performed));

    json_object_object_add(json_resp, "lookups_per_second",
        json_object_new_double(response->lookups_per_second));

    json_object_object_add(json_resp, "memory_used_kb",
        json_object_new_int(response->memory_used_kb));

    if (response->lookup_method) {
        json_object_object_add(json_resp, "lookup_benchmark",
            json_object_new_string(response->lookup_method));
    }

    if (response->request_id) {
        json_object_object_add(json_resp, "request_id",
            json_object_new_string(response->request_id));
    }

    if (response->error_message) {
        json_object_object_add(json_resp, "error_message",
            json_object_new_string(response->error_message));
    }

    json_str = json_object_to_json_string_ext(json_resp, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, json_str);
    json_object_put(json_resp);

    return result;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_scenario_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    finbench_scenario_request_t *request;
    finbench_scenario_response_t *response;
    axis2_char_t *json_response;

    request = finbench_scenario_request_create_from_json(env, json_request);
    if (!request) {
        return axutil_strdup(env,
            "{\"status\":\"FAILED\",\"error_message\":"
            "\"Failed to parse scenarioAnalysis request. "
            "Required: assets (array of {asset_id, current_price, position_size, "
            "scenarios: [{price, probability}]}). "
            "Optional: use_hash_lookup (bool, default true), "
            "prob_tolerance (float, default 0.0001), "
            "request_id (string).\"}");
    }

    response = finbench_calculate_scenarios(env, request);
    json_response = finbench_scenario_response_to_json(response, env);

    finbench_scenario_request_free(request, env);
    finbench_scenario_response_free(response, env);

    return json_response;
}

/* ============================================================================
 * Service Metadata
 * ============================================================================
 */

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_get_metadata_json(const axutil_env_t *env)
{
    json_object *json_resp;
    json_object *ops_array;
    const char *json_str;
    axis2_char_t *result;
    char *device_info;

    json_resp = json_object_new_object();

    json_object_object_add(json_resp, "service_name",
        json_object_new_string("FinancialBenchmarkService"));

    json_object_object_add(json_resp, "version",
        json_object_new_string("1.0.0"));

    json_object_object_add(json_resp, "description",
        json_object_new_string(
            "Axis2/C HTTP/2 Financial Benchmark Service - "
            "Demonstrates performance on resource-constrained hardware "
            "where Java/WildFly cannot run."));

    /* Operations */
    ops_array = json_object_new_array();
    json_object_array_add(ops_array, json_object_new_string("portfolioVariance"));
    json_object_array_add(ops_array, json_object_new_string("monteCarlo"));
    json_object_array_add(ops_array, json_object_new_string("scenarioAnalysis"));
    json_object_array_add(ops_array, json_object_new_string("metadata"));
    json_object_object_add(json_resp, "operations", ops_array);

    /* Limits */
    json_object_object_add(json_resp, "max_assets",
        json_object_new_int(FINBENCH_MAX_ASSETS));

    json_object_object_add(json_resp, "max_simulations",
        json_object_new_int(FINBENCH_MAX_SIMULATIONS));

    json_object_object_add(json_resp, "memory_constraint_mb",
        json_object_new_int(FINBENCH_MEMORY_CONSTRAINT_MB));

    /* Device info */
    device_info = finbench_get_device_info(env);
    if (device_info) {
        json_object_object_add(json_resp, "device_info",
            json_object_new_string(device_info));
        AXIS2_FREE(env->allocator, device_info);
    }

    /* Current memory usage */
    json_object_object_add(json_resp, "current_memory_kb",
        json_object_new_int(finbench_get_memory_usage_kb()));

    /* Comparison note */
    json_object_object_add(json_resp, "comparison_note",
        json_object_new_string(
            "This service runs on hardware with 1-2GB RAM where "
            "enterprise Java/WildFly (16-32GB per server minimum) cannot operate. "
            "Demonstrates Axis2/C efficiency for edge computing."));

    json_str = json_object_to_json_string_ext(json_resp, JSON_C_TO_STRING_PRETTY);
    result = axutil_strdup(env, json_str);
    json_object_put(json_resp);

    return result;
}

/* ============================================================================
 * Test Data Generator
 * ============================================================================
 */

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_generate_test_portfolio_json(
    const axutil_env_t *env,
    int n_assets)
{
    json_object *json_req;
    json_object *weights_array;
    json_object *cov_array;
    const char *json_str;
    axis2_char_t *result;
    int i, j;

    if (n_assets <= 0 || n_assets > FINBENCH_MAX_ASSETS) {
        n_assets = 100; /* Default */
    }

    json_req = json_object_new_object();
    json_object_object_add(json_req, "n_assets", json_object_new_int(n_assets));

    /* Generate equal weights (normalized) */
    weights_array = json_object_new_array();
    double equal_weight = 1.0 / n_assets;
    for (i = 0; i < n_assets; i++) {
        json_object_array_add(weights_array, json_object_new_double(equal_weight));
    }
    json_object_object_add(json_req, "weights", weights_array);

    /* Generate synthetic covariance matrix (positive semi-definite) */
    cov_array = json_object_new_array();
    for (i = 0; i < n_assets; i++) {
        for (j = 0; j < n_assets; j++) {
            double cov;
            if (i == j) {
                /* Diagonal: variance (0.04 = 20% vol squared) */
                cov = 0.04;
            } else {
                /* Off-diagonal: correlation × vol × vol */
                /* Use decreasing correlation with distance */
                double corr = 0.3 * exp(-0.1 * abs(i - j));
                cov = corr * 0.2 * 0.2; /* corr × vol_i × vol_j */
            }
            json_object_array_add(cov_array, json_object_new_double(cov));
        }
    }
    json_object_object_add(json_req, "covariance_matrix", cov_array);

    json_object_object_add(json_req, "request_id",
        json_object_new_string("test-portfolio"));

    json_str = json_object_to_json_string_ext(json_req, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, json_str);
    json_object_put(json_req);

    return result;
}

/* ============================================================================
 * JSON Object Dispatcher - json_object-based calling convention
 *
 * This is used by axis2_json_rpc_msg_recv when it passes a parsed json_object
 * directly.  The exported entry point for string-based callers and for
 * the Axis2/C dynamic loader is in financial_benchmark_service_handler.c
 * ============================================================================
 */

/**
 * @brief JSON object dispatcher — routes a pre-parsed request to the right op.
 *
 * Internal function; the public axis2_json_rpc_msg_recv entry point lives in
 * financial_benchmark_service_handler.c where msg_ctx is available.
 */
static json_object *
finbench_dispatch_json_obj(
    const axutil_env_t *env,
    json_object *json_request)
{
    json_object *response = NULL;
    json_object *action_obj = NULL;
    const char *action = NULL;
    const char *json_str = NULL;
    axis2_char_t *result_str = NULL;

    if (!env || !json_request)
    {
        response = json_object_new_object();
        json_object_object_add(response, "status",
            json_object_new_string(FINBENCH_STATUS_FAILED));
        json_object_object_add(response, "error_message",
            json_object_new_string("Invalid request parameters"));
        return response;
    }

    /* DEBUG: Log received request */
    json_str = json_object_to_json_string(json_request);
    AXIS2_LOG_INFO(env->log,
        "FinancialBenchmarkService: Received JSON request: %s",
        json_str ? json_str : "NULL");

    /* Try to extract operation from 'action' field (Camera service pattern) */
    if (json_object_object_get_ex(json_request, "action", &action_obj))
    {
        action = json_object_get_string(action_obj);
    }
    /* Also try 'operation' field (alternative pattern) */
    else if (json_object_object_get_ex(json_request, "operation", &action_obj))
    {
        action = json_object_get_string(action_obj);
    }

    AXIS2_LOG_INFO(env->log,
        "FinancialBenchmarkService: Processing action '%s'",
        action ? action : "(none - using request structure)");

    /* Route based on action or detect request type from structure */
    if (action)
    {
        if (strcmp(action, "portfolioVariance") == 0 ||
            strcmp(action, "portfolio_variance") == 0)
        {
            result_str = finbench_portfolio_variance_json_only(env, json_str);
        }
        else if (strcmp(action, "monteCarlo") == 0 ||
                 strcmp(action, "monte_carlo") == 0)
        {
            result_str = finbench_monte_carlo_json_only(env, json_str);
        }
        else if (strcmp(action, "metadata") == 0 ||
                 strcmp(action, "get_metadata") == 0)
        {
            result_str = finbench_get_metadata_json(env);
        }
        else if (strcmp(action, "generateTestData") == 0 ||
                 strcmp(action, "generate_test_data") == 0)
        {
            json_object *n_obj;
            int n_assets = 100;
            if (json_object_object_get_ex(json_request, "n_assets", &n_obj))
            {
                n_assets = json_object_get_int(n_obj);
            }
            result_str = finbench_generate_test_portfolio_json(env, n_assets);
        }
        else
        {
            /* Unknown action */
            response = json_object_new_object();
            json_object_object_add(response, "status",
                json_object_new_string(FINBENCH_STATUS_FAILED));

            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Unknown action: %s", action);
            json_object_object_add(response, "error_message",
                json_object_new_string(error_msg));
            return response;
        }
    }
    else
    {
        /* No explicit action - detect operation from request structure */
        json_object *temp_obj;

        /* If request has 'weights' and 'covariance_matrix', it's portfolio variance */
        if (json_object_object_get_ex(json_request, "weights", &temp_obj) &&
            json_object_object_get_ex(json_request, "covariance_matrix", &temp_obj))
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected portfolioVariance request");
            result_str = finbench_portfolio_variance_json_only(env, json_str);
        }
        /* If request has 'n_simulations', it's Monte Carlo */
        else if (json_object_object_get_ex(json_request, "n_simulations", &temp_obj))
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected monteCarlo request");
            result_str = finbench_monte_carlo_json_only(env, json_str);
        }
        /* If request only has 'n_assets' without weights, generate test data */
        else if (json_object_object_get_ex(json_request, "n_assets", &temp_obj) &&
                 !json_object_object_get_ex(json_request, "weights", NULL))
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected generateTestData request");
            /* Re-extract n_assets since temp_obj may have been modified by the weights check */
            json_object *n_assets_obj = NULL;
            json_object_object_get_ex(json_request, "n_assets", &n_assets_obj);
            int n_assets = n_assets_obj ? json_object_get_int(n_assets_obj) : 100;
            result_str = finbench_generate_test_portfolio_json(env, n_assets);
        }
        /* Default to metadata */
        else
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Defaulting to metadata");
            result_str = finbench_get_metadata_json(env);
        }
    }

    /* Convert result string to json_object */
    if (result_str)
    {
        response = json_tokener_parse(result_str);
        AXIS2_FREE(env->allocator, result_str);

        if (!response)
        {
            /* Parse failed - return error */
            response = json_object_new_object();
            json_object_object_add(response, "status",
                json_object_new_string(FINBENCH_STATUS_FAILED));
            json_object_object_add(response, "error_message",
                json_object_new_string("Failed to parse result JSON"));
        }
    }
    else
    {
        response = json_object_new_object();
        json_object_object_add(response, "status",
            json_object_new_string(FINBENCH_STATUS_FAILED));
        json_object_object_add(response, "error_message",
            json_object_new_string("Operation returned null result"));
    }

    return response;
}
