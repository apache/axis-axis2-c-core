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
#include <stdarg.h>
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
/**
 * Copy up to out_len-1 bytes of an untrusted string for logging, replacing
 * every control character (including CR and LF) with '?', so a request body
 * cannot write line breaks or terminal escapes into the log.
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_log_excerpt(const char *src, char *out, size_t out_len)
{
    size_t i = 0;
    if (!out || out_len == 0) return;
    if (src) {
        for (; i + 1 < out_len && src[i]; i++) {
            unsigned char c = (unsigned char)src[i];
            out[i] = (c < 0x20 || c == 0x7f) ? '?' : (char)c;
        }
    }
    out[i] = '\0';
}

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

/* n_periods_per_year, read the same way by every operation. Absent: *out is
 * `def`. Present: it must be a JSON number, finite and > 0, and a whole
 * number when `whole_only` (portfolioVariance and monteCarlo hold it as an
 * int). Anything else returns 0 with *out = def, and the caller refuses the
 * request -- json_object_get_int would otherwise parse "12" out of a string
 * and truncate 252.25 to 252, both silently. */
static int
finbench_read_periods_per_year(json_object *json_obj, double def,
                               int whole_only, double *out)
{
    json_object *v = NULL;
    double p;

    *out = def;
    if (!json_object_object_get_ex(json_obj, "n_periods_per_year", &v))
        return 1;
    if (!json_object_is_type(v, json_type_int) &&
        !json_object_is_type(v, json_type_double))
        return 0;
    p = json_object_get_double(v);
    if (!isfinite(p) || p <= 0.0)
        return 0;
    if (whole_only && (p != floor(p) || p > 1e7))
        return 0;
    *out = p;
    return 1;
}

#define FINBENCH_PERIODS_INVALID_MSG \
    "n_periods_per_year must be a number > 0%s; omit it for the default."

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

    /* Extract covariance_matrix - supports both flat and 2D array formats.
     * The shape validation below rejects any matrix that would leave a cell
     * unwritten, so no separate zero-fill is needed. */
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
            /* Validate the exact shape, not just the element count: a ragged
             * matrix (e.g. [[1,2,3,4],[]]) can total n*n while leaving interior
             * cells unwritten. Require n_assets rows, each n_assets long. */
            int shape_ok = (outer_len == request->n_assets);
            for (i = 0; i < request->n_assets && i < outer_len; i++) {
                json_object *row = json_object_array_get_idx(array_obj, i);
                if (row && json_object_is_type(row, json_type_array)) {
                    int row_len = json_object_array_length(row);
                    if (row_len != request->n_assets) shape_ok = 0;
                    total_elements += row_len;
                    for (j = 0; j < request->n_assets && j < row_len; j++) {
                        json_object *cell = json_object_array_get_idx(row, j);
                        request->covariance_matrix[i * request->n_assets + j] =
                            json_object_get_double(cell);
                    }
                } else {
                    shape_ok = 0;   /* missing or non-array row */
                }
            }
            /* A wrong shape fails the n*n check below via a sentinel that can
             * never equal n*n; a correct n×n matrix reports its true count. */
            request->matrix_elements_provided = shape_ok ? total_elements : -1;
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

    /* n_periods_per_year — default 1: the matrix is taken as already
     * annualized, which is what composeCovariance, covarianceFromReturns and
     * every documented example produce. A caller holding a per-period matrix
     * (daily, weekly, monthly) passes 252, 52 or 12 explicitly. The default
     * was 252 until 2026-09; on an annualized matrix that reported an
     * annualized_volatility sqrt(252) ~ 15.9x too large, and the service has
     * no way to detect the mismatch, so the default now matches the basis its
     * own producers emit. */
    {
        double npy;
        if (!finbench_read_periods_per_year(json_obj, 1.0, 1, &npy))
            request->n_periods_per_year_invalid = AXIS2_TRUE;
        request->n_periods_per_year = (int)npy;
    }

    /* covarianceFromReturns echoes the factor it already applied as
     * n_periods_per_year beside covariance_basis "annualized". Taking that
     * echo as this matrix's basis would annualize it twice, so the pair is
     * flagged and refused in the calculation. */
    if (request->n_periods_per_year != 1 &&
        json_object_object_get_ex(json_obj, "covariance_basis", &value_obj) &&
        json_object_is_type(value_obj, json_type_string) &&
        strcmp(json_object_get_string(value_obj), "annualized") == 0) {
        request->basis_conflict = AXIS2_TRUE;
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
 * The KEY BENCHMARK: O(n²) matrix multiplication that exercises the
 * same math pattern used by production portfolio risk engines.
 *
 * Formula: σ²_p = Σ_i Σ_j w_i * w_j * σ_ij
 *
 * The output basis matches the input basis — if the caller passes a
 * daily covariance matrix, σ²_p is a daily variance. The service
 * additionally emits an annualized_volatility field computed as
 *   portfolio_volatility * sqrt(n_periods_per_year)
 * with n_periods_per_year defaulting to 1, i.e. the matrix is taken as
 * already annualized — the basis composeCovariance and
 * covarianceFromReturns emit. A caller with a PER-PERIOD matrix passes
 * 252 (daily), 52 (weekly) or 12 (monthly) explicitly. The response
 * echoes n_periods_per_year and a covariance_basis of "annualized" or
 * "per_period" so the basis applied is always visible to the caller.
 *
 * Weight normalization edge case:
 *   When normalize_weights=true, weights are rescaled in-place so they
 *   sum to 1.0. This silently discards the gross-exposure information
 *   (a {0.5, 0.5} long-short book becomes {0.5, 0.5} — same — but
 *   {-0.3, 0.3} becomes undefined because sum is 0). The service
 *   explicitly rejects a zero-sum portfolio under normalize_weights=true
 *   rather than producing NaN from division by zero.
 *
 * Numerical edge case:
 *   The O(n²) accumulator can produce variance < 0 from two sources:
 *     (1) floating-point cancellation on strongly-negative correlations
 *         with large weights — magnitude ~1e-16 to 1e-12, harmless
 *     (2) a non-PSD input covariance matrix — the math is wrong, not
 *         the code, and the magnitude can be large (e.g., -0.02)
 *   The implementation clamps both to 0.0 to avoid NaN from sqrt,
 *   which masks case (2). Callers who need hard non-PSD rejection
 *   should validate upstream (e.g., via Cholesky decomposition).
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

    if (request->n_periods_per_year_invalid) {
        char err_buf[160];
        snprintf(err_buf, sizeof(err_buf), FINBENCH_PERIODS_INVALID_MSG,
                 ", a whole number of periods");
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, err_buf);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench portfolioVariance: %s", err_buf);
        return response;
    }

    if (request->basis_conflict) {
        char err_buf[256];
        snprintf(err_buf, sizeof(err_buf),
            "covariance_basis is \"annualized\" but n_periods_per_year is %d. "
            "An annualized matrix takes n_periods_per_year 1 (or omit it); "
            "a per-period matrix must not be labelled annualized.",
            request->n_periods_per_year);
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, err_buf);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench portfolioVariance: %s", err_buf);
        return response;
    }

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

    /* Symmetry, same rule and tolerance as monteCarlo and composeCovariance.
     * w'Σw over an asymmetric matrix silently uses the mean of each
     * off-diagonal pair, so the caller gets a number that agrees with no
     * symmetrized version of what they sent. Refuse with the index.
     * j <= i so the diagonal is finiteness-checked too: a NaN or -Inf
     * there would otherwise slip past the PSD guard below (comparisons
     * against NaN, and -Inf < -Inf, are all false). */
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            double a = request->covariance_matrix[i * n + j];
            double b = request->covariance_matrix[j * n + i];
            if (!isfinite(a) || !isfinite(b)) {
                char err_buf[160];
                if (i == j)
                    snprintf(err_buf, sizeof(err_buf),
                        "covariance_matrix[%d][%d] is not finite.", i, i);
                else
                    snprintf(err_buf, sizeof(err_buf),
                        "covariance_matrix[%d][%d] or [%d][%d] is not finite.", i, j, j, i);
                response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
                response->error_message = axutil_strdup(env, err_buf);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench portfolioVariance: %s", err_buf);
                return response;
            }
            if (fabs(a - b) > FINBENCH_CORR_TOL * (1.0 + fabs(a))) {
                char err_buf[200];
                snprintf(err_buf, sizeof(err_buf),
                    "covariance_matrix is not symmetric at (%d,%d): %g vs %g.", i, j, a, b);
                response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
                response->error_message = axutil_strdup(env, err_buf);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench portfolioVariance: %s", err_buf);
                return response;
            }
        }
    }

    start_time = get_time_us();

    /*
     * Core O(n²) calculation:
     * σ²_p = Σ_i Σ_j w_i * w_j * σ_ij
     *
     * This is common for correlation calculations.
     * On a 500-asset portfolio, this is 250,000 operations.
     *
     * `magnitude` accumulates the absolute terms so a negative result can be
     * judged against the size of what was summed: cancellation noise is a
     * negative of order 1e-16 × magnitude; a matrix that is not positive
     * semi-definite for these weights is a negative of order magnitude.
     */
    double magnitude = 0.0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double term = request->weights[i] *
                          request->weights[j] *
                          request->covariance_matrix[i * n + j];
            variance += term;
            magnitude += fabs(term);
            ops++;
        }
    }

    end_time = get_time_us();

    /* The parser defaults this to 1 (annualized matrix); a direct C caller
     * that left the struct zeroed gets the same default, never the old 252. */
    int npy = (request->n_periods_per_year > 0) ? request->n_periods_per_year : 1;

    /* Entries are finite by now, but a NaN weight or entries large enough
     * to overflow the sum still give a non-finite result; refuse it rather
     * than let it past the sign test below. */
    if (!isfinite(variance)) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "w'Sigma*w is not finite: check weights and covariance_matrix "
            "for non-finite or overflowing values.");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench portfolioVariance: w'Sigma*w is not finite");
        return response;
    }

    /*
     * A negative w'Σw means the matrix is not positive semi-definite for
     * these weights — it is not a covariance matrix. Only a rounding-scale
     * negative (cancellation in the sum) is clamped to zero; anything larger
     * is refused, because a silently clamped zero volatility is a confidently
     * wrong answer nothing downstream can detect. composeCovariance and
     * covarianceFromReturns Cholesky-check what they produce; a matrix
     * assembled by hand gets no such check anywhere else.
     */
    if (variance < 0.0) {
        if (variance < -1e-9 * magnitude) {
            char err_buf[240];
            snprintf(err_buf, sizeof(err_buf),
                "w'Sigma*w = %g is negative: covariance_matrix is not positive "
                "semi-definite for these weights, so it is not a covariance matrix. "
                "Build it with composeCovariance or covarianceFromReturns, which check.",
                variance);
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env, err_buf);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench portfolioVariance: %s", err_buf);
            return response;
        }
        variance = 0.0;
    }
    double volatility = sqrt(variance);

    /* Populate response */
    response->status = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->portfolio_variance = variance;
    response->portfolio_volatility = volatility;
    response->annualized_volatility = volatility * sqrt((double)npy);
    response->n_periods_per_year = npy;
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

    /* Make the basis that was applied visible: a caller who sees
     * covariance_basis "per_period" on a matrix they know is annual has
     * their answer without reading the docs. Only meaningful on success. */
    if (response->n_periods_per_year > 0) {
        json_object_object_add(json_resp, "n_periods_per_year",
            json_object_new_int(response->n_periods_per_year));
        json_object_object_add(json_resp, "covariance_basis",
            json_object_new_string(response->n_periods_per_year == 1
                                   ? "annualized" : "per_period"));
    }

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
            "Optional: normalize_weights (bool), n_periods_per_year (int, default 1 = "
            "matrix already annualized; 252 for a daily matrix), request_id (string).\"}");
    }

    response = finbench_calculate_portfolio_variance(env, request);
    json_response = finbench_portfolio_variance_response_to_json(response, env);

    finbench_portfolio_variance_request_free(request, env);
    finbench_portfolio_variance_response_free(response, env);

    return json_response;
}

/* ============================================================================
 * Compose Covariance Implementation
 *
 * Σ = D·R·D, then Cholesky. See the header for the rationale.
 * ============================================================================
 */

/* Release and NULL every output array of a compose response. Used on every
 * failure path after allocation so a FAILED response never carries numbers,
 * and never carries an allocated-but-unfilled buffer. */
static void
compose_drop_outputs(
    const axutil_env_t *env,
    finbench_compose_covariance_response_t *response)
{
    if (response->covariance_matrix) {
        AXIS2_FREE(env->allocator, response->covariance_matrix);
        response->covariance_matrix = NULL;
    }
    if (response->correlation_matrix) {
        AXIS2_FREE(env->allocator, response->correlation_matrix);
        response->correlation_matrix = NULL;
    }
    if (response->volatilities) {
        AXIS2_FREE(env->allocator, response->volatilities);
        response->volatilities = NULL;
    }
}

/* Set a FAILED status with a formatted message; returns the response for
 * convenient "return fail(...)" use in the compose function. */
static finbench_compose_covariance_response_t *
compose_fail(
    const axutil_env_t *env,
    finbench_compose_covariance_response_t *response,
    const char *fmt, ...)
{
    char buf[320];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);
    response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
    response->error_message = axutil_strdup(env, buf);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench composeCovariance: %s", buf);
    return response;
}

AXIS2_EXTERN finbench_compose_covariance_request_t* AXIS2_CALL
finbench_compose_covariance_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    finbench_compose_covariance_request_t *request = NULL;
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
            "FinBench composeCovariance: Failed to parse JSON request");
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(finbench_compose_covariance_request_t));
    if (!request) {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(finbench_compose_covariance_request_t));
    request->check_positive_definite = AXIS2_TRUE;

    /* n_assets — explicit, or inferred from the volatilities array */
    if (json_object_object_get_ex(json_obj, "n_assets", &value_obj)) {
        request->n_assets = json_object_get_int(value_obj);
    } else if (json_object_object_get_ex(json_obj, "volatilities", &array_obj) &&
               json_object_is_type(array_obj, json_type_array)) {
        request->n_assets = json_object_array_length(array_obj);
        AXIS2_LOG_INFO(env->log,
            "FinBench composeCovariance: Inferred n_assets=%d from volatilities",
            request->n_assets);
    }

    if (request->n_assets <= 0 || request->n_assets > FINBENCH_MAX_COV_ASSETS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench composeCovariance: Invalid n_assets: %d (max: %d)",
            request->n_assets, FINBENCH_MAX_COV_ASSETS);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    /* volatilities */
    request->volatilities = AXIS2_MALLOC(env->allocator,
        (size_t)request->n_assets * sizeof(double));
    if (!request->volatilities) {
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }
    memset(request->volatilities, 0, (size_t)request->n_assets * sizeof(double));

    if (json_object_object_get_ex(json_obj, "volatilities", &array_obj) &&
        json_object_is_type(array_obj, json_type_array)) {
        int len = json_object_array_length(array_obj);
        request->volatilities_provided = len;
        for (i = 0; i < request->n_assets && i < len; i++) {
            json_object *elem = json_object_array_get_idx(array_obj, i);
            request->volatilities[i] = json_object_get_double(elem);
        }
    }

    /* correlation (uniform ρ) */
    if (json_object_object_get_ex(json_obj, "correlation", &value_obj) &&
        (json_object_is_type(value_obj, json_type_double) ||
         json_object_is_type(value_obj, json_type_int))) {
        request->correlation = json_object_get_double(value_obj);
        request->has_correlation = AXIS2_TRUE;
    }

    /* correlation_matrix — flat n² or 2D n×n, same shapes as portfolioVariance */
    if (json_object_object_get_ex(json_obj, "correlation_matrix", &array_obj) &&
        json_object_is_type(array_obj, json_type_array)) {
        size_t matrix_size = (size_t)request->n_assets * request->n_assets;
        int outer_len = json_object_array_length(array_obj);
        json_object *first_elem = json_object_array_get_idx(array_obj, 0);

        request->correlation_matrix = AXIS2_MALLOC(env->allocator,
            matrix_size * sizeof(double));
        if (!request->correlation_matrix) {
            AXIS2_FREE(env->allocator, request->volatilities);
            AXIS2_FREE(env->allocator, request);
            json_object_put(json_obj);
            return NULL;
        }
        memset(request->correlation_matrix, 0, matrix_size * sizeof(double));

        if (first_elem && json_object_is_type(first_elem, json_type_array)) {
            int total_elements = 0;
            int shape_ok = (outer_len == request->n_assets);
            for (i = 0; i < request->n_assets && i < outer_len; i++) {
                json_object *row = json_object_array_get_idx(array_obj, i);
                if (row && json_object_is_type(row, json_type_array)) {
                    int row_len = json_object_array_length(row);
                    if (row_len != request->n_assets) shape_ok = 0;
                    total_elements += row_len;
                    for (j = 0; j < request->n_assets && j < row_len; j++) {
                        json_object *cell = json_object_array_get_idx(row, j);
                        request->correlation_matrix[i * request->n_assets + j] =
                            json_object_get_double(cell);
                    }
                } else {
                    shape_ok = 0;
                }
            }
            request->matrix_elements_provided = shape_ok ? total_elements : -1;
        } else {
            request->matrix_elements_provided = outer_len;
            for (i = 0; i < (int)matrix_size && i < outer_len; i++) {
                json_object *elem = json_object_array_get_idx(array_obj, i);
                request->correlation_matrix[i] = json_object_get_double(elem);
            }
        }
    }

    /* check_positive_definite — default true */
    if (json_object_object_get_ex(json_obj, "check_positive_definite", &value_obj)) {
        request->check_positive_definite = json_object_get_boolean(value_obj)
            ? AXIS2_TRUE : AXIS2_FALSE;
    }

    /* asset_ids (optional; only kept when the length matches) */
    if (json_object_object_get_ex(json_obj, "asset_ids", &array_obj) &&
        json_object_is_type(array_obj, json_type_array) &&
        (int)json_object_array_length(array_obj) == request->n_assets) {
        request->asset_ids = AXIS2_MALLOC(env->allocator,
            (size_t)request->n_assets * sizeof(char *));
        if (request->asset_ids) {
            memset(request->asset_ids, 0, (size_t)request->n_assets * sizeof(char *));
            for (i = 0; i < request->n_assets; i++) {
                json_object *elem = json_object_array_get_idx(array_obj, i);
                const char *id = elem ? json_object_get_string(elem) : NULL;
                request->asset_ids[i] = id ? axutil_strdup(env, id) : NULL;
            }
        }
    }

    if (json_object_object_get_ex(json_obj, "request_id", &value_obj)) {
        const char *rid = json_object_get_string(value_obj);
        if (rid) {
            request->request_id = axutil_strdup(env, rid);
        }
    }

    json_object_put(json_obj);
    return request;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_compose_covariance_request_free(
    finbench_compose_covariance_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env) return;

    if (request->volatilities)
        AXIS2_FREE(env->allocator, request->volatilities);
    if (request->correlation_matrix)
        AXIS2_FREE(env->allocator, request->correlation_matrix);
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

AXIS2_EXTERN finbench_compose_covariance_response_t* AXIS2_CALL
finbench_compose_covariance_response_create(const axutil_env_t *env)
{
    finbench_compose_covariance_response_t *response;

    response = AXIS2_MALLOC(env->allocator,
        sizeof(finbench_compose_covariance_response_t));
    if (response) {
        memset(response, 0, sizeof(finbench_compose_covariance_response_t));
        response->cholesky_failed_at = -1;
    }
    return response;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_compose_covariance_response_free(
    finbench_compose_covariance_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env) return;

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);
    if (response->covariance_matrix)
        AXIS2_FREE(env->allocator, response->covariance_matrix);
    if (response->correlation_matrix)
        AXIS2_FREE(env->allocator, response->correlation_matrix);
    if (response->volatilities)
        AXIS2_FREE(env->allocator, response->volatilities);
    if (response->error_message)
        AXIS2_FREE(env->allocator, response->error_message);
    if (response->request_id)
        AXIS2_FREE(env->allocator, response->request_id);
    if (response->device_info)
        AXIS2_FREE(env->allocator, response->device_info);
    if (response->asset_ids) {
        int i;
        for (i = 0; i < response->n_assets; i++) {
            if (response->asset_ids[i])
                AXIS2_FREE(env->allocator, response->asset_ids[i]);
        }
        AXIS2_FREE(env->allocator, response->asset_ids);
    }

    AXIS2_FREE(env->allocator, response);
}

/**
 * Cholesky–Banachiewicz on a symmetric matrix held row-major in `a`
 * (n×n). Writes the lower factor L into `l` (row-major, upper part left 0).
 * Returns -1 on success, else the index of the first pivot that is not
 * strictly positive — which is exactly "not positive definite".
 * *min_pivot receives the smallest pivot (L_ii²) seen before any failure.
 */
static int
cholesky_lower(const double *a, double *l, int n, double *min_pivot)
{
    int i, j, k;
    *min_pivot = 0.0;

    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            double sum = a[i * n + j];
            for (k = 0; k < j; k++) {
                sum -= l[i * n + k] * l[j * n + k];
            }
            if (i == j) {
                /* A pivot that is zero, negative, or NaN means Σ is not
                 * positive definite (or the input carried a NaN). */
                if (!(sum > 0.0)) {
                    return i;
                }
                if (i == 0 || sum < *min_pivot) {
                    *min_pivot = sum;
                }
                l[i * n + i] = sqrt(sum);
            } else {
                l[i * n + j] = sum / l[j * n + j];
            }
        }
    }
    return -1;
}

AXIS2_EXTERN finbench_compose_covariance_response_t* AXIS2_CALL
finbench_compose_covariance(
    const axutil_env_t *env,
    finbench_compose_covariance_request_t *request)
{
    finbench_compose_covariance_response_t *response = NULL;
    long start_time, end_time;
    int i, j, n;
    size_t matrix_size;

    response = finbench_compose_covariance_response_create(env);
    if (!response) return NULL;

    if (!request || !request->volatilities) {
        return compose_fail(env, response, "Invalid request parameters");
    }

    n = request->n_assets;
    response->n_assets = n;
    matrix_size = (size_t)n * n;

    /* --- Dimension and range validation. Every refusal names the field. --- */

    if (request->volatilities_provided == 0) {
        return compose_fail(env, response,
            "Missing required field: \"volatilities\" array (n_assets values, each > 0).");
    }
    if (request->volatilities_provided != n) {
        return compose_fail(env, response,
            "volatilities array length %d != n_assets %d.",
            request->volatilities_provided, n);
    }
    for (i = 0; i < n; i++) {
        double v = request->volatilities[i];
        if (!isfinite(v) || v <= 0.0) {
            return compose_fail(env, response,
                "volatilities[%d] = %g must be finite and > 0.", i, v);
        }
    }

    if (request->has_correlation && request->correlation_matrix) {
        return compose_fail(env, response,
            "Supply either \"correlation\" (uniform) or \"correlation_matrix\", not both.");
    }
    if (!request->has_correlation && !request->correlation_matrix) {
        return compose_fail(env, response,
            "Missing required field: one of \"correlation\" (uniform rho in [-1, 1]) "
            "or \"correlation_matrix\" (n_assets x n_assets).");
    }

    if (request->has_correlation) {
        double rho = request->correlation;
        if (!isfinite(rho) || rho < -1.0 || rho > 1.0) {
            return compose_fail(env, response,
                "correlation = %g must be within [-1, 1].", rho);
        }
    } else {
        if (request->matrix_elements_provided != (int)matrix_size) {
            return compose_fail(env, response,
                "correlation_matrix must have exactly n_assets*n_assets = %d elements "
                "(flat row-major) or be an n_assets x n_assets 2D array; got %d.",
                (int)matrix_size, request->matrix_elements_provided);
        }
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                double r = request->correlation_matrix[i * n + j];
                if (!isfinite(r) || r < -1.0 || r > 1.0) {
                    return compose_fail(env, response,
                        "correlation_matrix[%d][%d] = %g must be within [-1, 1].", i, j, r);
                }
                if (i == j && fabs(r - 1.0) > FINBENCH_CORR_TOL) {
                    return compose_fail(env, response,
                        "correlation_matrix[%d][%d] = %g must be 1 on the diagonal.", i, i, r);
                }
                if (j < i && fabs(r - request->correlation_matrix[j * n + i]) > FINBENCH_CORR_TOL) {
                    return compose_fail(env, response,
                        "correlation_matrix is not symmetric at (%d,%d): %g vs %g.",
                        i, j, r, request->correlation_matrix[j * n + i]);
                }
            }
        }
    }

    /* --- Allocate outputs --- */
    response->covariance_matrix = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
    response->correlation_matrix = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
    response->volatilities = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    if (!response->covariance_matrix || !response->correlation_matrix || !response->volatilities) {
        /* A partial success must not leave an unfilled matrix behind: the
         * serializer emits every non-NULL array, and an unfilled one would
         * put uninitialized heap into the response. */
        compose_drop_outputs(env, response);
        return compose_fail(env, response, "Memory allocation failed for %d assets.", n);
    }

    start_time = get_time_us();

    /* --- Σ = D·R·D --- */
    for (i = 0; i < n; i++) {
        response->volatilities[i] = request->volatilities[i];
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double r;
            if (request->has_correlation) {
                r = (i == j) ? 1.0 : request->correlation;
            } else {
                r = request->correlation_matrix[i * n + j];
            }
            response->correlation_matrix[i * n + j] = r;
            response->covariance_matrix[i * n + j] =
                request->volatilities[i] * request->volatilities[j] * r;
        }
    }

    /* --- Cholesky --- */
    if (request->check_positive_definite) {
        double *l = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
        int failed_at;
        double min_pivot = 0.0;

        if (!l) {
            compose_drop_outputs(env, response);
            return compose_fail(env, response,
                "Memory allocation failed for the Cholesky factor (%d assets).", n);
        }
        memset(l, 0, matrix_size * sizeof(double));
        failed_at = cholesky_lower(response->covariance_matrix, l, n, &min_pivot);
        AXIS2_FREE(env->allocator, l);

        response->positive_definite_checked = AXIS2_TRUE;
        response->min_pivot = min_pivot;
        response->cholesky_failed_at = failed_at;
        response->positive_definite = (failed_at < 0) ? AXIS2_TRUE : AXIS2_FALSE;

        if (failed_at >= 0) {
            /* Do not hand back a matrix that portfolioVariance would silently
             * accept. The caller gets the reason and the index, not numbers. */
            compose_drop_outputs(env, response);
            if (request->has_correlation) {
                return compose_fail(env, response,
                    "Not positive definite: Cholesky failed at index %d. "
                    "With %d assets a uniform correlation must satisfy %g < rho < 1; got %g.",
                    failed_at, n, (n > 1) ? -1.0 / (double)(n - 1) : -1.0, request->correlation);
            }
            return compose_fail(env, response,
                "Not positive definite: Cholesky failed at index %d "
                "(the leading %dx%d block of the correlation matrix is not positive definite).",
                failed_at, failed_at + 1, failed_at + 1);
        }
    }

    end_time = get_time_us();

    response->status = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->calc_time_us = end_time - start_time;
    response->memory_used_kb = finbench_get_memory_usage_kb();

    if (request->asset_ids) {
        response->asset_ids = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(char *));
        if (response->asset_ids) {
            for (i = 0; i < n; i++) {
                response->asset_ids[i] = request->asset_ids[i]
                    ? axutil_strdup(env, request->asset_ids[i]) : NULL;
            }
        }
    }
    if (request->request_id) {
        response->request_id = axutil_strdup(env, request->request_id);
    }
    response->device_info = finbench_get_device_info(env);

    AXIS2_LOG_INFO(env->log,
        "FinBench: composeCovariance for %d assets in %ld us (positive_definite=%s)",
        n, response->calc_time_us,
        response->positive_definite_checked
            ? (response->positive_definite ? "true" : "false") : "unchecked");

    return response;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_compose_covariance_response_to_json(
    const finbench_compose_covariance_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_resp;
    const char *json_str;
    axis2_char_t *result;
    int i;
    size_t matrix_size;

    if (!response || !env) return NULL;

    matrix_size = (size_t)response->n_assets * response->n_assets;
    json_resp = json_object_new_object();

    json_object_object_add(json_resp, "status",
        json_object_new_string(response->status ? response->status : "UNKNOWN"));

    json_object_object_add(json_resp, "n_assets",
        json_object_new_int(response->n_assets));

    /* Σ = D·R·D inherits the vols' basis; annualized vols are the documented
     * input, so the matrix is annualized. Stamped so a consumer can check the
     * basis instead of assuming it. portfolioVariance defaults to this basis. */
    json_object_object_add(json_resp, "covariance_basis",
        json_object_new_string("annualized"));

    if (response->covariance_matrix) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < (int)matrix_size; i++)
            json_object_array_add(arr, json_object_new_double(response->covariance_matrix[i]));
        json_object_object_add(json_resp, "covariance_matrix", arr);
    }
    if (response->correlation_matrix) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < (int)matrix_size; i++)
            json_object_array_add(arr, json_object_new_double(response->correlation_matrix[i]));
        json_object_object_add(json_resp, "correlation_matrix", arr);
    }
    if (response->volatilities) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < response->n_assets; i++)
            json_object_array_add(arr, json_object_new_double(response->volatilities[i]));
        json_object_object_add(json_resp, "volatilities", arr);
    }

    json_object_object_add(json_resp, "positive_definite",
        json_object_new_boolean(response->positive_definite));
    json_object_object_add(json_resp, "positive_definite_checked",
        json_object_new_boolean(response->positive_definite_checked));
    json_object_object_add(json_resp, "cholesky_failed_at",
        json_object_new_int(response->cholesky_failed_at));
    json_object_object_add(json_resp, "min_pivot",
        json_object_new_double(response->min_pivot));

    json_object_object_add(json_resp, "calc_time_us",
        json_object_new_int64(response->calc_time_us));
    json_object_object_add(json_resp, "memory_used_kb",
        json_object_new_int(response->memory_used_kb));

    if (response->asset_ids) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < response->n_assets; i++)
            json_object_array_add(arr, response->asset_ids[i]
                ? json_object_new_string(response->asset_ids[i]) : NULL);
        json_object_object_add(json_resp, "asset_ids", arr);
    }
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

    json_str = json_object_to_json_string_ext(json_resp, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, json_str);
    json_object_put(json_resp);

    return result;
}

/**
 * HTTP/2 JSON endpoint for compose covariance
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_compose_covariance_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    finbench_compose_covariance_request_t *request;
    finbench_compose_covariance_response_t *response;
    axis2_char_t *json_response;

    request = finbench_compose_covariance_request_create_from_json(env, json_request);
    if (!request) {
        return axutil_strdup(env,
            "{\"status\":\"FAILED\",\"error_message\":"
            "\"Failed to parse composeCovariance request. "
            "n_assets must be in [1, 500] and match the volatilities length. "
            "Required fields: volatilities (float[] > 0) and one of "
            "correlation (float in [-1,1]) or correlation_matrix (float[n²] flat or float[n][n] 2D). "
            "Optional: n_assets (int), check_positive_definite (bool, default true), "
            "asset_ids (string[]), request_id (string).\"}");
    }

    response = finbench_compose_covariance(env, request);
    json_response = finbench_compose_covariance_response_to_json(response, env);

    finbench_compose_covariance_request_free(request, env);
    finbench_compose_covariance_response_free(response, env);

    return json_response;
}

/* ============================================================================
 * Covariance From Returns Implementation
 * ============================================================================
 */

/* Drop every output array. Same reason as compose_drop_outputs: the
 * serializer emits whatever is non-NULL, so a half-filled matrix must never
 * survive a failure. */
static void
cfr_drop_outputs(
    const axutil_env_t *env,
    finbench_covariance_from_returns_response_t *response)
{
    if (response->covariance_matrix) {
        AXIS2_FREE(env->allocator, response->covariance_matrix);
        response->covariance_matrix = NULL;
    }
    if (response->correlation_matrix) {
        AXIS2_FREE(env->allocator, response->correlation_matrix);
        response->correlation_matrix = NULL;
    }
    if (response->volatilities) {
        AXIS2_FREE(env->allocator, response->volatilities);
        response->volatilities = NULL;
    }
    if (response->mean_returns) {
        AXIS2_FREE(env->allocator, response->mean_returns);
        response->mean_returns = NULL;
    }
    if (response->observations_per_pair) {
        AXIS2_FREE(env->allocator, response->observations_per_pair);
        response->observations_per_pair = NULL;
    }
}

/* Set a FAILED status with a formatted message; returns the response for
 * convenient "return cfr_fail(...)" use. */
static finbench_covariance_from_returns_response_t *
cfr_fail(
    const axutil_env_t *env,
    finbench_covariance_from_returns_response_t *response,
    const char *fmt, ...)
{
    char buf[320];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);
    response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
    response->error_message = axutil_strdup(env, buf);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench covarianceFromReturns: %s", buf);
    return response;
}

/* Store one returns element. A JSON null becomes NaN ("not observed"); a
 * number is taken as is; anything else is recorded as the first bad element
 * so the compute step can name it. Returns nothing: validation is deferred so
 * that one malformed cell produces a sentence, not a NULL request. */
static void
cfr_store_element(
    finbench_covariance_from_returns_request_t *request,
    json_object *elem,
    int row,
    int col)
{
    double *slot = &request->returns[(size_t)row * request->n_obs + col];

    if (!elem || json_object_is_type(elem, json_type_null)) {
        *slot = NAN;
        return;
    }
    if (json_object_is_type(elem, json_type_double) ||
        json_object_is_type(elem, json_type_int)) {
        *slot = json_object_get_double(elem);
        return;
    }
    *slot = NAN;
    if (request->bad_value_row < 0) {
        request->bad_value_row = row;
        request->bad_value_col = col;
    }
}

AXIS2_EXTERN finbench_covariance_from_returns_request_t* AXIS2_CALL
finbench_covariance_from_returns_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    finbench_covariance_from_returns_request_t *request = NULL;
    json_object *json_obj = NULL;
    json_object *value_obj = NULL;
    json_object *array_obj = NULL;
    json_object *first_elem = NULL;
    int outer_len = 0;
    int i, j;

    if (!env || !json_string) {
        return NULL;
    }

    json_obj = json_tokener_parse(json_string);
    if (!json_obj) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench covarianceFromReturns: Failed to parse JSON request");
        return NULL;
    }

    if (!json_object_object_get_ex(json_obj, "returns", &array_obj) ||
        !json_object_is_type(array_obj, json_type_array)) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench covarianceFromReturns: \"returns\" must be an array");
        json_object_put(json_obj);
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator,
        sizeof(finbench_covariance_from_returns_request_t));
    if (!request) {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(finbench_covariance_from_returns_request_t));
    request->check_positive_definite = AXIS2_TRUE;
    request->n_periods_per_year = FINBENCH_DEFAULT_PERIODS_PER_YEAR;
    request->ragged_at = -1;
    request->bad_value_row = -1;
    request->bad_value_col = -1;

    /* Shape: a 2D array is n_assets rows of n_obs; a flat array needs both
     * dimensions named, exactly as the covariance matrix inputs do. */
    outer_len = json_object_array_length(array_obj);
    first_elem = (outer_len > 0) ? json_object_array_get_idx(array_obj, 0) : NULL;
    request->rows_provided = outer_len;

    if (first_elem && json_object_is_type(first_elem, json_type_array)) {
        request->n_assets = outer_len;
        request->n_obs = json_object_array_length(first_elem);
        request->obs_provided = request->n_obs;
    } else {
        request->flat_shape = AXIS2_TRUE;
        if (json_object_object_get_ex(json_obj, "n_assets", &value_obj)) {
            request->n_assets = json_object_get_int(value_obj);
        }
        if (json_object_object_get_ex(json_obj, "n_obs", &value_obj)) {
            request->n_obs = json_object_get_int(value_obj);
        }
        request->obs_provided = outer_len;
    }

    if (request->n_assets <= 0 || request->n_assets > FINBENCH_MAX_RET_ASSETS ||
        request->n_obs <= 0 || request->n_obs > FINBENCH_MAX_OBS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "FinBench covarianceFromReturns: Invalid dimensions n_assets=%d (max %d), "
            "n_obs=%d (max %d)",
            request->n_assets, FINBENCH_MAX_RET_ASSETS,
            request->n_obs, FINBENCH_MAX_OBS);
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }

    request->returns = AXIS2_MALLOC(env->allocator,
        (size_t)request->n_assets * request->n_obs * sizeof(double));
    if (!request->returns) {
        AXIS2_FREE(env->allocator, request);
        json_object_put(json_obj);
        return NULL;
    }
    for (i = 0; i < request->n_assets * request->n_obs; i++) {
        request->returns[i] = NAN;
    }

    if (request->flat_shape) {
        for (i = 0; i < request->n_assets; i++) {
            for (j = 0; j < request->n_obs; j++) {
                int flat = i * request->n_obs + j;
                if (flat >= outer_len) break;
                cfr_store_element(request,
                    json_object_array_get_idx(array_obj, flat), i, j);
            }
        }
    } else {
        for (i = 0; i < request->n_assets; i++) {
            json_object *row = json_object_array_get_idx(array_obj, i);
            int row_len;

            if (!row || !json_object_is_type(row, json_type_array)) {
                if (request->ragged_at < 0) request->ragged_at = i;
                continue;
            }
            row_len = json_object_array_length(row);
            if (row_len != request->n_obs && request->ragged_at < 0) {
                request->ragged_at = i;
            }
            for (j = 0; j < request->n_obs && j < row_len; j++) {
                cfr_store_element(request, json_object_array_get_idx(row, j), i, j);
            }
        }
    }

    if (!finbench_read_periods_per_year(json_obj, FINBENCH_DEFAULT_PERIODS_PER_YEAR,
                                        0, &request->n_periods_per_year))
        request->n_periods_per_year_invalid = AXIS2_TRUE;

    if (json_object_object_get_ex(json_obj, "check_positive_definite", &value_obj)) {
        request->check_positive_definite = json_object_get_boolean(value_obj)
            ? AXIS2_TRUE : AXIS2_FALSE;
    }

    /* asset_ids (optional; only kept when the length matches) */
    if (json_object_object_get_ex(json_obj, "asset_ids", &array_obj) &&
        json_object_is_type(array_obj, json_type_array) &&
        (int)json_object_array_length(array_obj) == request->n_assets) {
        request->asset_ids = AXIS2_MALLOC(env->allocator,
            (size_t)request->n_assets * sizeof(char *));
        if (request->asset_ids) {
            memset(request->asset_ids, 0, (size_t)request->n_assets * sizeof(char *));
            for (i = 0; i < request->n_assets; i++) {
                json_object *elem = json_object_array_get_idx(array_obj, i);
                const char *id = elem ? json_object_get_string(elem) : NULL;
                request->asset_ids[i] = id ? axutil_strdup(env, id) : NULL;
            }
        }
    }

    if (json_object_object_get_ex(json_obj, "request_id", &value_obj)) {
        const char *rid = json_object_get_string(value_obj);
        if (rid) {
            request->request_id = axutil_strdup(env, rid);
        }
    }

    json_object_put(json_obj);
    return request;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_covariance_from_returns_request_free(
    finbench_covariance_from_returns_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env) return;

    if (request->returns)
        AXIS2_FREE(env->allocator, request->returns);
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

AXIS2_EXTERN finbench_covariance_from_returns_response_t* AXIS2_CALL
finbench_covariance_from_returns_response_create(const axutil_env_t *env)
{
    finbench_covariance_from_returns_response_t *response;

    response = AXIS2_MALLOC(env->allocator,
        sizeof(finbench_covariance_from_returns_response_t));
    if (response) {
        memset(response, 0, sizeof(finbench_covariance_from_returns_response_t));
        response->cholesky_failed_at = -1;
    }
    return response;
}

AXIS2_EXTERN void AXIS2_CALL
finbench_covariance_from_returns_response_free(
    finbench_covariance_from_returns_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env) return;

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);
    if (response->covariance_matrix)
        AXIS2_FREE(env->allocator, response->covariance_matrix);
    if (response->correlation_matrix)
        AXIS2_FREE(env->allocator, response->correlation_matrix);
    if (response->volatilities)
        AXIS2_FREE(env->allocator, response->volatilities);
    if (response->mean_returns)
        AXIS2_FREE(env->allocator, response->mean_returns);
    if (response->observations_per_pair)
        AXIS2_FREE(env->allocator, response->observations_per_pair);
    if (response->error_message)
        AXIS2_FREE(env->allocator, response->error_message);
    if (response->request_id)
        AXIS2_FREE(env->allocator, response->request_id);
    if (response->device_info)
        AXIS2_FREE(env->allocator, response->device_info);
    if (response->asset_ids) {
        int i;
        for (i = 0; i < response->n_assets; i++) {
            if (response->asset_ids[i])
                AXIS2_FREE(env->allocator, response->asset_ids[i]);
        }
        AXIS2_FREE(env->allocator, response->asset_ids);
    }

    AXIS2_FREE(env->allocator, response);
}

AXIS2_EXTERN finbench_covariance_from_returns_response_t* AXIS2_CALL
finbench_covariance_from_returns(
    const axutil_env_t *env,
    finbench_covariance_from_returns_request_t *request)
{
    finbench_covariance_from_returns_response_t *response = NULL;
    axis2_char_t *complete = NULL;
    long start_time, end_time;
    double factor;
    int i, j, t, n, obs, used = 0;
    size_t matrix_size;

    response = finbench_covariance_from_returns_response_create(env);
    if (!response) return NULL;

    if (!request || !request->returns) {
        return cfr_fail(env, response, "Invalid request parameters");
    }

    n = request->n_assets;
    obs = request->n_obs;
    factor = request->n_periods_per_year;
    matrix_size = (size_t)n * n;
    response->n_assets = n;
    response->n_obs_provided = obs;
    response->n_periods_per_year = factor;

    /* --- Shape and range validation. Every refusal names the field. --- */

    if (request->flat_shape && request->obs_provided != n * obs) {
        return cfr_fail(env, response,
            "returns must have exactly n_assets*n_obs = %d elements when flat; got %d.",
            n * obs, request->obs_provided);
    }
    if (!request->flat_shape && request->ragged_at >= 0) {
        return cfr_fail(env, response,
            "returns[%d] is not an array of %d observations; every asset needs the "
            "same observation count (complete-case handling uses nulls for gaps).",
            request->ragged_at, obs);
    }
    if (request->bad_value_row >= 0) {
        return cfr_fail(env, response,
            "returns[%d][%d] is neither a number nor null.",
            request->bad_value_row, request->bad_value_col);
    }
    if (request->n_periods_per_year_invalid) {
        return cfr_fail(env, response, FINBENCH_PERIODS_INVALID_MSG, "");
    }
    if (!isfinite(factor) || factor <= 0.0) {
        return cfr_fail(env, response,
            "n_periods_per_year = %g must be finite and > 0 (252 for daily, 1 to keep "
            "the input's own basis).", factor);
    }

    /* --- Complete-case mask: one sample for every pair --- */
    complete = AXIS2_MALLOC(env->allocator, (size_t)obs * sizeof(axis2_char_t));
    if (!complete) {
        return cfr_fail(env, response, "Memory allocation failed for %d observations.", obs);
    }
    for (t = 0; t < obs; t++) {
        complete[t] = 1;
        for (i = 0; i < n; i++) {
            double v = request->returns[(size_t)i * obs + t];
            if (isnan(v)) {
                complete[t] = 0;
                break;
            }
            if (!isfinite(v)) {
                AXIS2_FREE(env->allocator, complete);
                return cfr_fail(env, response,
                    "returns[%d][%d] = %g is not finite.", i, t, v);
            }
        }
        if (complete[t]) used++;
    }

    if (used < 2) {
        int u = used;
        AXIS2_FREE(env->allocator, complete);
        return cfr_fail(env, response,
            "Only %d of %d observations have every asset present; the sample "
            "covariance needs at least 2 (Bessel's correction divides by n-1).",
            u, obs);
    }
    response->n_obs_used = used;

    /* --- Allocate outputs --- */
    response->covariance_matrix = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
    response->correlation_matrix = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
    response->volatilities = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    response->mean_returns = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    response->observations_per_pair = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(int));
    if (!response->covariance_matrix || !response->correlation_matrix ||
        !response->volatilities || !response->mean_returns ||
        !response->observations_per_pair) {
        AXIS2_FREE(env->allocator, complete);
        cfr_drop_outputs(env, response);
        return cfr_fail(env, response,
            "Memory allocation failed for %d assets.", n);
    }

    start_time = get_time_us();

    /* --- Means over the complete cases --- */
    for (i = 0; i < n; i++) {
        double sum = 0.0;
        for (t = 0; t < obs; t++) {
            if (complete[t]) sum += request->returns[(size_t)i * obs + t];
        }
        response->mean_returns[i] = sum / used;
    }

    /* --- Sigma, Bessel-corrected and annualised --- */
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            double acc = 0.0;
            double cov;
            for (t = 0; t < obs; t++) {
                if (!complete[t]) continue;
                acc += (request->returns[(size_t)i * obs + t] - response->mean_returns[i]) *
                       (request->returns[(size_t)j * obs + t] - response->mean_returns[j]);
            }
            cov = (acc / (used - 1)) * factor;
            response->covariance_matrix[i * n + j] = cov;
            response->covariance_matrix[j * n + i] = cov;
        }
    }

    /* --- Observations per pair: what a pairwise estimator would have had --- */
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            int both = 0;
            for (t = 0; t < obs; t++) {
                if (!isnan(request->returns[(size_t)i * obs + t]) &&
                    !isnan(request->returns[(size_t)j * obs + t])) {
                    both++;
                }
            }
            response->observations_per_pair[i * n + j] = both;
            response->observations_per_pair[j * n + i] = both;
        }
    }

    AXIS2_FREE(env->allocator, complete);

    /* --- Volatilities and the implied correlation --- */
    for (i = 0; i < n; i++) {
        double var = response->covariance_matrix[i * n + i];
        if (!(var > 0.0)) {
            int idx = i;
            cfr_drop_outputs(env, response);
            return cfr_fail(env, response,
                "Asset %d has zero variance over the %d complete observations: it is "
                "constant, so no correlation with it is defined.", idx, used);
        }
        response->volatilities[i] = sqrt(var);
        response->mean_returns[i] *= factor;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double r;
            if (i == j) {
                /* An asset's correlation with itself is 1 by definition. The
                 * division would give 1 minus an ulp or so, which is noise in
                 * a field whose value is known. */
                response->correlation_matrix[i * n + j] = 1.0;
                continue;
            }
            r = response->covariance_matrix[i * n + j] /
                (response->volatilities[i] * response->volatilities[j]);
            /* Rounding can put a perfect correlation a few ulps outside the
             * range; the estimate itself is unchanged, only its presentation. */
            if (r > 1.0) r = 1.0;
            if (r < -1.0) r = -1.0;
            response->correlation_matrix[i * n + j] = r;
        }
    }

    /* --- Cholesky. A complete-case sample covariance is positive
     * semi-definite by construction, so a failure here means the sample is
     * rank-deficient: fewer complete observations than assets, or a duplicated
     * series. Report it the way composeCovariance does and return no numbers. */
    if (request->check_positive_definite) {
        double *l = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
        int failed_at;
        double min_pivot = 0.0;

        if (!l) {
            cfr_drop_outputs(env, response);
            return cfr_fail(env, response,
                "Memory allocation failed for the Cholesky factor (%d assets).", n);
        }
        memset(l, 0, matrix_size * sizeof(double));
        failed_at = cholesky_lower(response->covariance_matrix, l, n, &min_pivot);
        AXIS2_FREE(env->allocator, l);

        response->positive_definite_checked = AXIS2_TRUE;
        response->min_pivot = min_pivot;
        response->cholesky_failed_at = failed_at;
        response->positive_definite = (failed_at < 0) ? AXIS2_TRUE : AXIS2_FALSE;

        if (failed_at >= 0) {
            int idx = failed_at;
            cfr_drop_outputs(env, response);
            return cfr_fail(env, response,
                "Not positive definite: Cholesky failed at index %d. A complete-case "
                "sample covariance is positive semi-definite, so this is a rank-deficient "
                "sample: %d complete observations for %d assets (%d or more are needed), "
                "or two series that move together exactly.",
                idx, used, n, n + 1);
        }
    }

    end_time = get_time_us();

    response->status = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->calc_time_us = end_time - start_time;
    response->memory_used_kb = finbench_get_memory_usage_kb();

    if (request->asset_ids) {
        response->asset_ids = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(char *));
        if (response->asset_ids) {
            for (i = 0; i < n; i++) {
                response->asset_ids[i] = request->asset_ids[i]
                    ? axutil_strdup(env, request->asset_ids[i]) : NULL;
            }
        }
    }
    if (request->request_id) {
        response->request_id = axutil_strdup(env, request->request_id);
    }
    response->device_info = finbench_get_device_info(env);

    AXIS2_LOG_INFO(env->log,
        "FinBench: covarianceFromReturns for %d assets over %d of %d observations "
        "in %ld us (positive_definite=%s)",
        n, used, obs, response->calc_time_us,
        response->positive_definite_checked
            ? (response->positive_definite ? "true" : "false") : "unchecked");

    return response;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_covariance_from_returns_response_to_json(
    const finbench_covariance_from_returns_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_resp;
    const char *json_str;
    axis2_char_t *result;
    int i;
    size_t matrix_size;

    if (!response || !env) return NULL;

    matrix_size = (size_t)response->n_assets * response->n_assets;
    json_resp = json_object_new_object();

    json_object_object_add(json_resp, "status",
        json_object_new_string(response->status ? response->status : "UNKNOWN"));

    json_object_object_add(json_resp, "n_assets",
        json_object_new_int(response->n_assets));
    json_object_object_add(json_resp, "n_obs_provided",
        json_object_new_int(response->n_obs_provided));
    json_object_object_add(json_resp, "n_obs_used",
        json_object_new_int(response->n_obs_used));
    json_object_object_add(json_resp, "n_periods_per_year",
        json_object_new_double(response->n_periods_per_year));
    /* The sample covariance was multiplied by n_periods_per_year, so the
     * matrix is on an annual basis whatever the input frequency was. Stamped
     * for consumers; portfolioVariance defaults to this basis. */
    json_object_object_add(json_resp, "covariance_basis",
        json_object_new_string("annualized"));

    if (response->covariance_matrix) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < (int)matrix_size; i++)
            json_object_array_add(arr, json_object_new_double(response->covariance_matrix[i]));
        json_object_object_add(json_resp, "covariance_matrix", arr);
    }
    if (response->correlation_matrix) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < (int)matrix_size; i++)
            json_object_array_add(arr, json_object_new_double(response->correlation_matrix[i]));
        json_object_object_add(json_resp, "correlation_matrix", arr);
    }
    if (response->volatilities) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < response->n_assets; i++)
            json_object_array_add(arr, json_object_new_double(response->volatilities[i]));
        json_object_object_add(json_resp, "volatilities", arr);
    }
    if (response->mean_returns) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < response->n_assets; i++)
            json_object_array_add(arr, json_object_new_double(response->mean_returns[i]));
        json_object_object_add(json_resp, "mean_returns", arr);
    }
    if (response->observations_per_pair) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < (int)matrix_size; i++)
            json_object_array_add(arr, json_object_new_int(response->observations_per_pair[i]));
        json_object_object_add(json_resp, "observations_per_pair", arr);
    }

    json_object_object_add(json_resp, "positive_definite",
        json_object_new_boolean(response->positive_definite));
    json_object_object_add(json_resp, "positive_definite_checked",
        json_object_new_boolean(response->positive_definite_checked));
    json_object_object_add(json_resp, "cholesky_failed_at",
        json_object_new_int(response->cholesky_failed_at));
    json_object_object_add(json_resp, "min_pivot",
        json_object_new_double(response->min_pivot));

    json_object_object_add(json_resp, "calc_time_us",
        json_object_new_int64(response->calc_time_us));
    json_object_object_add(json_resp, "memory_used_kb",
        json_object_new_int(response->memory_used_kb));

    if (response->asset_ids) {
        json_object *arr = json_object_new_array();
        for (i = 0; i < response->n_assets; i++)
            json_object_array_add(arr, response->asset_ids[i]
                ? json_object_new_string(response->asset_ids[i]) : NULL);
        json_object_object_add(json_resp, "asset_ids", arr);
    }
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

    json_str = json_object_to_json_string_ext(json_resp, JSON_C_TO_STRING_PLAIN);
    result = axutil_strdup(env, json_str);
    json_object_put(json_resp);

    return result;
}

/**
 * HTTP/2 JSON endpoint for covariance from returns
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_covariance_from_returns_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request)
{
    finbench_covariance_from_returns_request_t *request;
    finbench_covariance_from_returns_response_t *response;
    axis2_char_t *json_response;

    request = finbench_covariance_from_returns_request_create_from_json(env, json_request);
    if (!request) {
        return axutil_strdup(env,
            "{\"status\":\"FAILED\",\"error_message\":"
            "\"Failed to parse covarianceFromReturns request. "
            "Required field: returns, either n_assets arrays of n_obs numbers or one "
            "flat row-major array with n_assets and n_obs given. "
            "n_assets must be in [1, 50] and n_obs in [2, 5000]; a null element means "
            "the observation is missing. "
            "Optional: n_periods_per_year (float > 0, default 252), "
            "check_positive_definite (bool, default true), asset_ids (string[]), "
            "request_id (string).\"}");
    }

    response = finbench_covariance_from_returns(env, request);
    json_response = finbench_covariance_from_returns_response_to_json(response, env);

    finbench_covariance_from_returns_request_free(request, env);
    finbench_covariance_from_returns_response_free(response, env);

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
    {
        double npy;
        if (!finbench_read_periods_per_year(json_obj, 252.0, 1, &npy))
            request->n_periods_per_year_invalid = AXIS2_TRUE;
        request->n_periods_per_year = (int)npy;
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

    /* model — 0=GBM (default), 1=Merton jump-diffusion */
    if (json_object_object_get_ex(json_obj, "model", &value_obj)) {
        const char *model_str = json_object_get_string(value_obj);
        if (model_str && (strcmp(model_str, "merton") == 0 ||
                          strcmp(model_str, "MERTON") == 0)) {
            request->model = FINBENCH_MODEL_MERTON;
        } else {
            request->model = FINBENCH_MODEL_GBM;
        }
    } else {
        request->model = FINBENCH_MODEL_GBM;
    }

    /* Jump-diffusion parameters (Merton only; harmless defaults for GBM) */
    if (json_object_object_get_ex(json_obj, "jump_intensity", &value_obj)) {
        request->jump_intensity = json_object_get_double(value_obj);
    } else {
        request->jump_intensity = 1.0; /* 1 jump per year */
    }
    if (json_object_object_get_ex(json_obj, "jump_mean", &value_obj)) {
        request->jump_mean = json_object_get_double(value_obj);
    } else {
        request->jump_mean = -0.03; /* average 3% down */
    }
    if (json_object_object_get_ex(json_obj, "jump_vol", &value_obj)) {
        request->jump_vol = json_object_get_double(value_obj);
    } else {
        request->jump_vol = 0.05;
    }

    if (json_object_object_get_ex(json_obj, "request_id", &value_obj)) {
        const char *rid = json_object_get_string(value_obj);
        if (rid) request->request_id = axutil_strdup(env, rid);
    }

    /* ---- Correlated multi-asset book: only when a covariance matrix is given ---- */
    {
        json_object *cov_obj = NULL;
        if (json_object_object_get_ex(json_obj, "covariance_matrix", &cov_obj) &&
            json_object_is_type(cov_obj, json_type_array)) {
            json_object *array_obj = NULL;
            int i, j;

            if (json_object_object_get_ex(json_obj, "n_assets", &value_obj)) {
                request->n_assets = json_object_get_int(value_obj);
            } else if (json_object_object_get_ex(json_obj, "weights", &array_obj) &&
                       json_object_is_type(array_obj, json_type_array)) {
                request->n_assets = json_object_array_length(array_obj);
            }
            /* A covariance matrix with no usable n_assets still marks the request
             * as correlated (n_assets stays 0 or out of range) so that the
             * simulator reports it instead of silently running the scalar path. */
            if (request->n_assets == 0) request->n_assets = -1;

            if (request->n_assets >= 1 && request->n_assets <= FINBENCH_MAX_MC_ASSETS) {
                int n = request->n_assets;
                size_t matrix_size = (size_t)n * n;

                request->weights = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
                request->covariance_matrix = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
                request->expected_returns = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
                if (!request->weights || !request->covariance_matrix || !request->expected_returns) {
                    finbench_monte_carlo_request_free(request, env);
                    json_object_put(json_obj);
                    return NULL;
                }
                memset(request->weights, 0, (size_t)n * sizeof(double));
                memset(request->covariance_matrix, 0, matrix_size * sizeof(double));
                memset(request->expected_returns, 0, (size_t)n * sizeof(double));

                if (json_object_object_get_ex(json_obj, "weights", &array_obj) &&
                    json_object_is_type(array_obj, json_type_array)) {
                    int len = json_object_array_length(array_obj);
                    request->weights_provided = len;
                    for (i = 0; i < n && i < len; i++) {
                        request->weights[i] = json_object_get_double(
                            json_object_array_get_idx(array_obj, i));
                    }
                }

                {
                    int outer_len = json_object_array_length(cov_obj);
                    json_object *first_elem = json_object_array_get_idx(cov_obj, 0);
                    if (first_elem && json_object_is_type(first_elem, json_type_array)) {
                        int total_elements = 0;
                        int shape_ok = (outer_len == n);
                        for (i = 0; i < n && i < outer_len; i++) {
                            json_object *row = json_object_array_get_idx(cov_obj, i);
                            if (row && json_object_is_type(row, json_type_array)) {
                                int row_len = json_object_array_length(row);
                                if (row_len != n) shape_ok = 0;
                                total_elements += row_len;
                                for (j = 0; j < n && j < row_len; j++) {
                                    request->covariance_matrix[i * n + j] =
                                        json_object_get_double(json_object_array_get_idx(row, j));
                                }
                            } else {
                                shape_ok = 0;
                            }
                        }
                        request->matrix_elements_provided = shape_ok ? total_elements : -1;
                    } else {
                        request->matrix_elements_provided = outer_len;
                        for (i = 0; i < (int)matrix_size && i < outer_len; i++) {
                            request->covariance_matrix[i] =
                                json_object_get_double(json_object_array_get_idx(cov_obj, i));
                        }
                    }
                }

                if (json_object_object_get_ex(json_obj, "expected_returns", &array_obj) &&
                    json_object_is_type(array_obj, json_type_array)) {
                    int len = json_object_array_length(array_obj);
                    request->expected_returns_provided = len;
                    for (i = 0; i < n && i < len; i++) {
                        request->expected_returns[i] = json_object_get_double(
                            json_object_array_get_idx(array_obj, i));
                    }
                }
            }

            if (json_object_object_get_ex(json_obj, "normalize_weights", &value_obj)) {
                request->normalize_weights = json_object_get_boolean(value_obj)
                    ? AXIS2_TRUE : AXIS2_FALSE;
            }
        } else if (json_object_object_get_ex(json_obj, "covariance_matrix", &cov_obj)) {
            /* Present but not an array: the caller meant a correlated run and
             * got the shape wrong. Refuse in the simulator rather than quietly
             * running the scalar path with the default volatility. */
            request->covariance_matrix_malformed = AXIS2_TRUE;
        }
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
    if (request->weights) AXIS2_FREE(env->allocator, request->weights);
    if (request->covariance_matrix) AXIS2_FREE(env->allocator, request->covariance_matrix);
    if (request->expected_returns) AXIS2_FREE(env->allocator, request->expected_returns);
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
    if (response->model) AXIS2_FREE(env->allocator, response->model);
    if (response->error_message) AXIS2_FREE(env->allocator, response->error_message);
    if (response->request_id) AXIS2_FREE(env->allocator, response->request_id);
    if (response->simulation_mode) AXIS2_FREE(env->allocator, response->simulation_mode);
    if (response->weights) AXIS2_FREE(env->allocator, response->weights);
    AXIS2_FREE(env->allocator, response);
}

/**
 * Core Monte Carlo Simulation
 *
 * Runs n_simulations independent price paths, each of n_periods time
 * steps, to build an empirical distribution of terminal portfolio values.
 * That distribution is then summarized as mean, median, std dev, VaR at
 * user-specified percentiles, CVaR_95, max drawdown, and probability of
 * profit.
 *
 * Two models are supported (selected by request->model):
 *
 * GBM (model=0, default) update rule per step:
 *   S(t+dt) = S(t) * exp((μ − σ²/2) * dt  +  σ * sqrt(dt) * Z),  Z ~ N(0,1)
 *
 * Merton jump-diffusion (model=1) update rule per step:
 *   S(t+dt) = S(t) * exp((μ − σ²/2 − λk) * dt + σ√dt * Z) * J
 *   where J = exp(μ_J + σ_J * W) with probability λ*dt, else J = 1.
 *   Z,W ~ N(0,1) independent. k = exp(μ_J + σ_J²/2) − 1.
 *   The −λk drift correction ensures E[S(T)] = S(0)*exp(μ*T).
 *
 * Intuition:
 *   Each path is an independent future scenario for the portfolio;
 *   aggregating n_simulations of them produces a histogram of terminal
 *   values whose left tail is the loss distribution. "VaR at 95%"
 *   means "the 5th-percentile terminal loss" (so 95% of paths lose
 *   LESS than VaR_95, 5% lose MORE or equal). CVaR_95 (Expected
 *   Shortfall) is the AVERAGE loss over the worst 5% of paths,
 *   a strictly more conservative number than VaR_95.
 *
 * The Itô correction (−σ²/2) is NOT optional:
 *   Without the −σ²/2 drift adjustment, the expected terminal value
 *   would be S(0) * exp((μ + σ²/2) * T), drifting upward with vol.
 *   WITH the correction, E[S(T)] = S(0) * exp(μ * T), which is what
 *   any reader of Black-Scholes expects. Home-grown GBM
 *   implementations frequently omit this; the code here preserves it
 *   verbatim.
 *
 * Numerical edge cases (see also the header block for full discussion):
 *   - A step whose exponent exceeds 709 (log DBL_MAX), or whose product
 *     is no longer finite, ends its path as a terminal extreme outcome
 *     rather than being capped and continued. This diverges from
 *     Axis2/Java, which does NOT guard exp() — Java prefers NaN
 *     propagation as an alarm.
 *   - Variance uses a two-pass estimator (mean, then squared deviations),
 *     which cannot go negative, so no clamp precedes the sqrt.
 *
 * Reproducibility:
 *   xorshift128plus seeded from request->random_seed (or time(NULL)
 *   if seed == 0). Reproducibility is PER-PRNG only — callers
 *   reconciling against NumPy (PCG64), R (Mersenne Twister), or
 *   java.util.Random (LCG) will get different numbers for the SAME
 *   seed even with identical inputs.
 *
 * Performance note: this is the compute-heavy operation — n_simulations
 * × n_periods iterations of three multiplies, one exp, one PRNG step,
 * plus sort and reduce. Timings in response->simulations_per_second
 * are a useful hardware proxy for bare-metal scalar floating point.
 */
/*
 * Shared tail of both Monte Carlo paths — the scalar simulation and the
 * correlated book. Takes the per-path accumulators, sorts final_values in
 * place, and fills every response field that does not depend on which
 * simulator ran: moments, median, VaR/CVaR at the fixed and caller-requested
 * levels, drawdown, P(profit), timing and throughput. The caller sets
 * simulation_mode (and the book-only fields) and still owns final_values.
 * One copy on purpose: the two paths must never disagree on an estimator.
 */
static void
finbench_monte_carlo_finish(
    const axutil_env_t *env,
    const finbench_monte_carlo_request_t *request,
    finbench_monte_carlo_response_t *response,
    double *final_values,
    double sum_final,
    int profit_count,
    double max_drawdown,
    long calc_time_us)
{
    int sim;

    /* Calculate statistics — two-pass algorithm for variance.
     * The one-pass formula (sum_sq/N - mean^2) suffers from catastrophic
     * cancellation when std_dev << mean (common for low-vol strategies).
     * Two-pass: compute mean first, then sum squared deviations. This is
     * numerically stable and the extra pass over final_values[] is cheap
     * relative to the simulation itself. */
    double mean = sum_final / request->n_simulations;
    double variance;
    {
        double sum_sq_diff = 0.0;
        for (sim = 0; sim < request->n_simulations; sim++) {
            double d = final_values[sim] - mean;
            sum_sq_diff += d * d;
        }
        variance = sum_sq_diff / request->n_simulations;
    }

    /* Sort for percentiles */
    qsort(final_values, request->n_simulations, sizeof(double), compare_doubles);

    int n_sims = request->n_simulations;
    /* Percentile indexing: ceil(p * N) - 1 selects the k-th order
     * statistic such that exactly floor(p * N) observations are strictly
     * below the VaR level. This matches the standard quantile definition
     * and avoids the off-by-one that floor(p * N) introduces. */
    int idx_5  = (int)ceil(0.05 * n_sims) - 1;
    int idx_1  = (int)ceil(0.01 * n_sims) - 1;
    if (idx_5 < 0) idx_5 = 0;
    if (idx_1 < 0) idx_1 = 0;

    /* Sample median of a sorted array: for odd N take the middle element,
     * for even N average the two central elements. Using a single index
     * (n_sims/2) is only an approximation for even N and can produce small
     * reconciliation differences against NumPy/R, which both implement
     * the average-of-two rule. */
    double median = (n_sims % 2 == 0)
        ? (final_values[n_sims / 2 - 1] + final_values[n_sims / 2]) / 2.0
        : final_values[n_sims / 2];

    /* CVaR_95 (Expected Shortfall at 95%): the arithmetic mean of the
     * idx_5 worst final values after ascending sort. This is a common
     * discrete-sample estimator for E[L | L >= VaR_95] — the average
     * loss in the worst 5% of simulated outcomes.
     *
     * Estimator detail: this averages the floor(0.05 * n_sims) WORST
     * observations (positions 0 through idx_5 - 1 inclusive). For large
     * n_sims this matches the textbook definition to within one
     * observation. Systems reconciling against an alternate estimator
     * (e.g., one that averages L values that strictly exceed the VaR
     * threshold rather than the bottom k outcomes) may see minutely
     * different numbers, especially at small n_sims. */
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
            int idx = (int)ceil(p * n_sims) - 1;
            if (idx < 0) idx = 0;
            if (idx >= n_sims) idx = n_sims - 1;
            response->percentile_levels[response->n_percentiles] = p;
            response->var_at_percentile[response->n_percentiles] =
                request->initial_value - final_values[idx];
            response->n_percentiles++;
        }
    }

    /* Populate response */
    response->model = axutil_strdup(env,
        request->model == FINBENCH_MODEL_MERTON ? "merton" : "gbm");
    response->status = axutil_strdup(env, FINBENCH_STATUS_SUCCESS);
    response->mean_final_value = mean;
    response->median_final_value = median;
    response->std_dev_final_value = sqrt(variance);
    /* Sign convention: var_95, var_99, cvar_95 are returned as POSITIVE
     * LOSS MAGNITUDES in base-currency units. var_95 = 252000 means
     * "there is a 5% chance of losing $252,000 or more over the
     * simulated horizon." A profitable tail outcome would make these
     * figures NEGATIVE (a "loss" of -$1000 = a gain), which is normal
     * and not a bug. */
    response->var_95 = request->initial_value - final_values[idx_5];
    response->var_99 = request->initial_value - final_values[idx_1];
    response->cvar_95 = request->initial_value - cvar_95;
    response->max_drawdown = max_drawdown;
    response->prob_profit = (double)profit_count / request->n_simulations;
    response->calc_time_us = calc_time_us;
    response->memory_used_kb = finbench_get_memory_usage_kb();

    if (response->calc_time_us > 0) {
        response->simulations_per_second =
            (double)request->n_simulations / (response->calc_time_us / 1000000.0);
    }

    if (request->request_id) {
        response->request_id = axutil_strdup(env, request->request_id);
    }
}

/**
 * Correlated multi-asset Monte Carlo — see "Correlated multi-asset book" in
 * the header for the model. Reached from finbench_run_monte_carlo() after
 * the scalar validations; owns the correlated-specific validation, the
 * Cholesky factor and the simulation loop. The statistics section is the
 * scalar path's code, verbatim, over the same final_values[].
 */
static finbench_monte_carlo_response_t *
finbench_run_monte_carlo_correlated(
    const axutil_env_t *env,
    finbench_monte_carlo_request_t *request,
    finbench_monte_carlo_response_t *response)
{
    xorshift128plus_state rng;
    double *final_values = NULL;
    double *lsd = NULL;       /* L * sqrt(dt), row-major lower triangle */
    double *drift = NULL;     /* per-asset drift per step */
    double *s_vals = NULL;    /* per-asset values along one path */
    double *z = NULL;         /* per-step standard normals */
    long start_time, end_time;
    int sim, period, i, k, n;
    size_t matrix_size;
    double dt, sqrt_dt;
    double sum_final = 0.0;
    int profit_count = 0;
    double max_drawdown = 0.0;
    double weight_sum = 0.0;
    double book_variance = 0.0;
    double jump_lambda_dt_local = 0.0, jump_mean_local = 0.0, jump_vol_local = 0.0;
    double jump_compensation = 0.0;
    int is_merton = (request->model == FINBENCH_MODEL_MERTON);
    char err_buf[320];

#define MC_FAIL(...) do { \
        snprintf(err_buf, sizeof(err_buf), __VA_ARGS__); \
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED); \
        response->error_message = axutil_strdup(env, err_buf); \
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "FinBench monteCarlo (correlated): %s", err_buf); \
        if (final_values) AXIS2_FREE(env->allocator, final_values); \
        if (lsd) AXIS2_FREE(env->allocator, lsd); \
        if (drift) AXIS2_FREE(env->allocator, drift); \
        if (s_vals) AXIS2_FREE(env->allocator, s_vals); \
        if (z) AXIS2_FREE(env->allocator, z); \
        return response; \
    } while (0)

    n = request->n_assets;
    if (n < 1) {
        MC_FAIL("covariance_matrix was supplied but the asset count is unknown: "
                "supply n_assets or a weights array alongside it.");
    }
    if (n > FINBENCH_MAX_MC_ASSETS) {
        MC_FAIL("n_assets=%d exceeds the correlated-simulation maximum of %d.",
                n, FINBENCH_MAX_MC_ASSETS);
    }
    if (!request->weights || !request->covariance_matrix || !request->expected_returns) {
        MC_FAIL("Internal error: correlated request arrays missing.");
    }
    matrix_size = (size_t)n * n;

    if (request->weights_provided == 0) {
        MC_FAIL("Missing required field: \"weights\" (n_assets values >= 0) "
                "when covariance_matrix is supplied.");
    }
    if (request->weights_provided != n) {
        MC_FAIL("weights array length %d != n_assets %d.", request->weights_provided, n);
    }
    if (request->matrix_elements_provided != (int)matrix_size) {
        MC_FAIL("covariance_matrix must have exactly n_assets*n_assets = %d elements "
                "(flat row-major) or be an n_assets x n_assets 2D array; got %d.",
                (int)matrix_size, request->matrix_elements_provided);
    }
    if (request->expected_returns_provided != 0 && request->expected_returns_provided != n) {
        MC_FAIL("expected_returns array length %d != n_assets %d.",
                request->expected_returns_provided, n);
    }
    if ((int64_t)request->n_simulations * request->n_periods * (int64_t)n > FINBENCH_MAX_WORK) {
        MC_FAIL("n_simulations * n_periods * n_assets = %lld exceeds the computation budget (%ld). "
                "At n_simulations=%d and n_periods=%d the budget allows %d assets.",
                (long long)request->n_simulations * request->n_periods * n, (long)FINBENCH_MAX_WORK,
                request->n_simulations, request->n_periods,
                (int)(FINBENCH_MAX_WORK / ((int64_t)request->n_simulations * request->n_periods)));
    }
    for (i = 0; i < n; i++) {
        double w = request->weights[i];
        if (!isfinite(w) || w < 0.0) {
            MC_FAIL("weights[%d] = %g must be finite and >= 0 (long-only book; see the header).", i, w);
        }
        weight_sum += w;
        if (request->expected_returns_provided == 0) {
            request->expected_returns[i] = request->expected_return;
        } else if (!isfinite(request->expected_returns[i])) {
            MC_FAIL("expected_returns[%d] is not finite.", i);
        }
    }
    if (weight_sum <= 0.0) {
        MC_FAIL("weights must sum to a positive number; got %g.", weight_sum);
    }
    if (request->normalize_weights) {
        for (i = 0; i < n; i++) request->weights[i] /= weight_sum;
    } else if (fabs(weight_sum - 1.0) > 1e-4) {
        MC_FAIL("weights sum to %.6f, not 1.0 (set normalize_weights=true to rescale).", weight_sum);
    }
    for (i = 0; i < n; i++) {
        for (k = 0; k < n; k++) {
            double c = request->covariance_matrix[i * n + k];
            if (!isfinite(c)) {
                MC_FAIL("covariance_matrix[%d][%d] is not finite.", i, k);
            }
            if (k < i && fabs(c - request->covariance_matrix[k * n + i]) >
                    FINBENCH_CORR_TOL * (1.0 + fabs(c))) {
                MC_FAIL("covariance_matrix is not symmetric at (%d,%d): %g vs %g.",
                        i, k, c, request->covariance_matrix[k * n + i]);
            }
        }
    }

    /* Book volatility on the input basis: sqrt(w'Σw). Reported so the caller
     * can cross-check against portfolioVariance and against a scalar run. */
    for (i = 0; i < n; i++)
        for (k = 0; k < n; k++)
            book_variance += request->weights[i] * request->weights[k] *
                             request->covariance_matrix[i * n + k];
    if (book_variance < 0.0) book_variance = 0.0;

    /* Cholesky: Σ = L·Lᵀ. No factor means Σ is not positive definite and the
     * shocks cannot be generated; refuse with the index, as composeCovariance does. */
    lsd = AXIS2_MALLOC(env->allocator, matrix_size * sizeof(double));
    if (!lsd) MC_FAIL("Memory allocation failed for the Cholesky factor.");
    memset(lsd, 0, matrix_size * sizeof(double));
    {
        double min_pivot = 0.0;
        int failed_at = cholesky_lower(request->covariance_matrix, lsd, n, &min_pivot);
        if (failed_at >= 0) {
            MC_FAIL("covariance_matrix is not positive definite: Cholesky failed at index %d. "
                    "Build it with composeCovariance, which checks this.", failed_at);
        }
    }

    final_values = AXIS2_MALLOC(env->allocator, (size_t)request->n_simulations * sizeof(double));
    drift = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    s_vals = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    z = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    if (!final_values || !drift || !s_vals || !z) MC_FAIL("Memory allocation failed");

    if (request->random_seed != 0) {
        rng.s[0] = request->random_seed;
        rng.s[1] = request->random_seed ^ 0x5DEECE66DL;
    } else {
        rng.s[0] = (uint64_t)time(NULL);
        rng.s[1] = rng.s[0] ^ 0xDEADBEEF;
    }

    {
        int npy = (request->n_periods_per_year > 0) ? request->n_periods_per_year : 252;
        dt = 1.0 / (double)npy;
        sqrt_dt = sqrt(dt);
        if (is_merton) {
            double jv = (request->jump_vol >= 0.0) ? request->jump_vol : 0.05;
            double jm = request->jump_mean;
            double ji = (request->jump_intensity >= 0.0) ? request->jump_intensity : 1.0;
            double kk = exp(jm + 0.5 * jv * jv) - 1.0;
            jump_compensation = ji * kk;
            jump_lambda_dt_local = ji / (double)npy;
            jump_mean_local = jm;
            jump_vol_local = jv;
            if (jump_lambda_dt_local > 0.1) {
                MC_FAIL("jump_intensity too high for time step: lambda*dt > 0.1. "
                        "Reduce jump_intensity or increase n_periods_per_year.");
            }
        }
    }

    /* Per-asset drift per step, and sqrt(dt) folded into L so each shock is one
     * multiply-add per pair. For n = 1 the exponent is drift + (L11*sqrt(dt))*z,
     * the scalar path's expression, which is what keeps the two paths identical. */
    for (i = 0; i < n; i++) {
        double var_i = request->covariance_matrix[i * n + i];
        drift[i] = (request->expected_returns[i] - 0.5 * var_i - jump_compensation) * dt;
        for (k = 0; k <= i; k++) lsd[i * n + k] *= sqrt_dt;
    }

    start_time = get_time_us();

    for (sim = 0; sim < request->n_simulations; sim++) {
        double value = request->initial_value;
        double peak = value;
        double sim_max_drawdown = 0.0;
        int terminal = 0;

        for (i = 0; i < n; i++) s_vals[i] = request->weights[i] * request->initial_value;

        for (period = 0; period < request->n_periods; period++) {
            double jump = 0.0;

            for (k = 0; k < n; k++) z[k] = rand_normal(&rng);

            /* Systemic jump: one Poisson trial per step, one log-jump for the
             * whole book. Same draw order as the scalar path (Z, then U, then W). */
            if (is_merton) {
                double u = rand_uniform(&rng);
                if (u < jump_lambda_dt_local) {
                    double w = rand_normal(&rng);
                    jump = jump_mean_local + jump_vol_local * w;
                }
            }

            value = 0.0;
            for (i = 0; i < n; i++) {
                /* Accumulate onto the drift, first product first, so that with
                 * one asset this is exactly the scalar path's
                 * `drift + vol_sqrt_dt * z` — the same single expression, and
                 * therefore the same rounding whether or not the compiler
                 * contracts it into a fused multiply-add (clang does on
                 * aarch64, not on baseline x86-64). Summing the products into a
                 * separate accumulator and adding the drift afterwards gave a
                 * one-ulp difference on arm64 and broke the identity. */
                double exponent = drift[i];
                for (k = 0; k <= i; k++) exponent += lsd[i * n + k] * z[k];
                if (jump != 0.0) exponent += jump;
                /* Same overflow policy as the scalar path: an extreme step ends
                 * the path as a terminal extreme outcome rather than letting
                 * +Inf propagate into the statistics. */
                if (exponent > 709.0) { terminal = 1; break; }
                s_vals[i] *= exp(exponent);
                if (!isfinite(s_vals[i])) { terminal = 1; break; }
                value += s_vals[i];
            }
            if (terminal) {
                value = FINBENCH_EXTREME_VALUE;
                break;
            }

            if (value > peak) {
                peak = value;
            } else {
                double drawdown = (peak - value) / peak;
                if (drawdown > sim_max_drawdown) sim_max_drawdown = drawdown;
            }
        }

        final_values[sim] = value;
        sum_final += value;
        if (value > request->initial_value) profit_count++;
        if (sim_max_drawdown > max_drawdown) max_drawdown = sim_max_drawdown;
    }

    end_time = get_time_us();

    AXIS2_FREE(env->allocator, lsd);    lsd = NULL;
    AXIS2_FREE(env->allocator, drift);  drift = NULL;
    AXIS2_FREE(env->allocator, s_vals); s_vals = NULL;
    AXIS2_FREE(env->allocator, z);      z = NULL;

    response->simulation_mode = axutil_strdup(env, "correlated");
    response->n_assets = n;
    response->portfolio_volatility = sqrt(book_variance);
    response->weights = AXIS2_MALLOC(env->allocator, (size_t)n * sizeof(double));
    if (response->weights) {
        for (i = 0; i < n; i++) response->weights[i] = request->weights[i];
    }

    finbench_monte_carlo_finish(env, request, response, final_values,
                                sum_final, profit_count, max_drawdown,
                                end_time - start_time);

    AXIS2_FREE(env->allocator, final_values);

    AXIS2_LOG_INFO(env->log,
        "FinBench: correlated Monte Carlo completed %d sims x %d periods x %d assets in %ld us "
        "(%.0f sims/sec, VaR95=%.2f, book vol=%.4f)",
        request->n_simulations, request->n_periods, n,
        response->calc_time_us, response->simulations_per_second,
        response->var_95, response->portfolio_volatility);

    return response;
#undef MC_FAIL
}

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
    double sum_final = 0.0;
    int profit_count = 0;
    double max_drawdown = 0.0;

    response = finbench_monte_carlo_response_create(env);
    if (!response) return NULL;

    if (!request) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, "Invalid request");
        return response;
    }

    if (request->n_periods_per_year_invalid) {
        char err_buf[160];
        snprintf(err_buf, sizeof(err_buf), FINBENCH_PERIODS_INVALID_MSG,
                 ", a whole number of steps per year");
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, err_buf);
        return response;
    }

    /* Fail-fast input validation — the simulation body has no guards of
     * its own for these, and silently-wrong results on garbage input
     * would be worse than an explicit rejection. */
    if (request->initial_value <= 0.0) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "initial_value must be > 0. Geometric Brownian Motion evolves "
            "multiplicatively through exp(...) and is undefined for "
            "non-positive starting values.");
        return response;
    }
    if (request->n_simulations <= 0 ||
        request->n_simulations > FINBENCH_MAX_SIMULATIONS) {
        char err_buf[192];
        snprintf(err_buf, sizeof(err_buf),
            "n_simulations=%d is out of range [1, %d].",
            request->n_simulations, FINBENCH_MAX_SIMULATIONS);
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, err_buf);
        return response;
    }
    if (request->n_periods <= 0) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "n_periods must be >= 1 (at least one time step is required).");
        return response;
    }
    if (request->n_periods > FINBENCH_MAX_PERIODS) {
        char pbuf[128];
        snprintf(pbuf, sizeof(pbuf),
            "n_periods exceeds the maximum (%d).", FINBENCH_MAX_PERIODS);
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env, pbuf);
        return response;
    }
    if ((int64_t)request->n_simulations * request->n_periods > FINBENCH_MAX_WORK) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "n_simulations * n_periods exceeds the computation budget.");
        return response;
    }
    if (request->volatility < 0.0) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "volatility must be >= 0 (negative volatility is not a "
            "meaningful input).");
        return response;
    }

    if (request->covariance_matrix_malformed) {
        response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
        response->error_message = axutil_strdup(env,
            "covariance_matrix must be a JSON array: n_assets*n_assets numbers "
            "in row-major order, or n_assets rows of n_assets numbers.");
        return response;
    }

    /* Correlated multi-asset book: a covariance matrix was supplied. The
     * correlated simulator owns its own validation and loop; the scalar path
     * below is untouched so existing results stay bit-identical. */
    if (request->n_assets != 0) {
        return finbench_run_monte_carlo_correlated(env, request, response);
    }

    /* Allocate array for final values (needed for percentiles).
     * Cast to size_t to prevent integer overflow on 32-bit platforms
     * when n_simulations is large (e.g., 1M × 8 bytes = 8 MB). */
    final_values = AXIS2_MALLOC(env->allocator,
        (size_t)request->n_simulations * sizeof(double));
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

    /*
     * Merton jump-diffusion drift correction:
     *   k = E[J-1] = exp(μ_J + σ_J²/2) − 1  (expected percentage jump)
     *   drift_adj = (μ − σ²/2 − λ*k) * dt
     *
     * This ensures E[S(T)] = S(0)*exp(μ*T) regardless of jump parameters.
     * For GBM (model=0), jump_lambda_dt=0, so the correction vanishes and
     * drift reduces to the standard (μ − σ²/2)*dt.
     */
    {
        double jump_compensation = 0.0;
        if (request->model == FINBENCH_MODEL_MERTON) {
            double jv = (request->jump_vol >= 0.0) ? request->jump_vol : 0.05;
            double jm = request->jump_mean;
            double ji = (request->jump_intensity >= 0.0) ? request->jump_intensity : 1.0;
            double k = exp(jm + 0.5 * jv * jv) - 1.0;
            jump_compensation = ji * k;
        }
        drift = (request->expected_return - 0.5 * request->volatility * request->volatility
                 - jump_compensation) * dt;
        vol_sqrt_dt = request->volatility * sqrt(dt);
    }

    /* Merton parameters for the inner loop (pre-extract to avoid
     * repeated struct dereferences in the hot path) */
    {
        double jump_lambda_dt_local = 0.0;
        double jump_mean_local = 0.0;
        double jump_vol_local = 0.0;
        int is_merton = (request->model == FINBENCH_MODEL_MERTON);

        if (is_merton) {
            int npy = (request->n_periods_per_year > 0) ? request->n_periods_per_year : 252;
            jump_lambda_dt_local = ((request->jump_intensity >= 0.0) ?
                                     request->jump_intensity : 1.0) / (double)npy;
            jump_mean_local = request->jump_mean;
            jump_vol_local = (request->jump_vol >= 0.0) ? request->jump_vol : 0.05;

            /* The Bernoulli approximation for a Poisson process is only valid
             * when λ*dt << 1. For λ*dt > 0.1 the probability of ≥2 jumps per
             * step becomes non-negligible; at λ*dt ≥ 1 the trial degenerates
             * to a deterministic jump every step. Fail fast rather than produce
             * silently incorrect tail risk estimates. */
            if (jump_lambda_dt_local > 0.1) {
                AXIS2_FREE(env->allocator, final_values);
                response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
                response->error_message = axutil_strdup(env,
                    "jump_intensity too high for time step: lambda*dt > 0.1. "
                    "Reduce jump_intensity or increase n_periods_per_year.");
                return response;
            }
        }

    start_time = get_time_us();

    /*
     * Main simulation loop - THIS IS THE COMPUTE BENCHMARK
     *
     * GBM (model=0): n_simulations × n_periods iterations of one exp() + PRNG.
     * Merton (model=1): same, plus a Poisson jump test per step. When
     * jump_lambda_dt is small (typical: ~0.004 for λ=1, npy=252), the
     * branch predictor will correctly predict "no jump" ~99.6% of the
     * time, so the Merton overhead is primarily the uniform random draw
     * for the Poisson test (~15-20% slower than pure GBM).
     */
    for (sim = 0; sim < request->n_simulations; sim++) {
        double value = request->initial_value;
        double peak = value;
        double sim_max_drawdown = 0.0;

        for (period = 0; period < request->n_periods; period++) {
            double z = rand_normal(&rng);
            double exponent = drift + vol_sqrt_dt * z;

            /* Merton jump component: compound Poisson process.
             * At each step, a jump occurs with probability λ*dt.
             * We use a uniform random draw rather than sampling a
             * full Poisson variate because dt is small enough that
             * P(≥2 jumps per step) ≈ (λ*dt)² ≈ 1.6e-5 is negligible.
             * The jump magnitude is log-normal: J = exp(μ_J + σ_J * W)
             * where W ~ N(0,1) is independent of the diffusion Z. */
            if (is_merton) {
                double u = rand_uniform(&rng);
                if (u < jump_lambda_dt_local) {
                    double w = rand_normal(&rng);
                    exponent += jump_mean_local + jump_vol_local * w;
                }
            }

            /* Guard against exp() overflow — exp(709.78) ≈ DBL_MAX.
             * Extreme GBM shocks with high volatility can produce
             * exponents that overflow to Inf, corrupting all downstream
             * statistics. Cap to a safe maximum. */
            /* Capping value alone is self-defeating: a later period would
             * multiply the cap and overflow to +Inf, which propagates NaN into
             * the whole response. Treat an extreme path as terminal instead. */
            if (exponent > 709.0) {
                value = FINBENCH_EXTREME_VALUE;   /* extreme outcome */
                break;
            }
            value *= exp(exponent);
            if (!isfinite(value)) {
                value = FINBENCH_EXTREME_VALUE;
                break;
            }

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

        if (value > request->initial_value) {
            profit_count++;
        }

        if (sim_max_drawdown > max_drawdown) {
            max_drawdown = sim_max_drawdown;
        }
    }

    end_time = get_time_us();

    } /* end of is_merton local scope */

    response->simulation_mode = axutil_strdup(env, "single");
    finbench_monte_carlo_finish(env, request, response, final_values,
                                sum_final, profit_count, max_drawdown,
                                end_time - start_time);

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

    if (response->model) {
        json_object_object_add(json_resp, "model",
            json_object_new_string(response->model));
    }
    if (response->simulation_mode) {
        json_object_object_add(json_resp, "simulation_mode",
            json_object_new_string(response->simulation_mode));
    }
    if (response->n_assets > 0) {
        json_object_object_add(json_resp, "n_assets",
            json_object_new_int(response->n_assets));
        json_object_object_add(json_resp, "portfolio_volatility",
            json_object_new_double(response->portfolio_volatility));
        if (response->weights) {
            json_object *warr = json_object_new_array();
            int wi;
            for (wi = 0; wi < response->n_assets; wi++)
                json_object_array_add(warr, json_object_new_double(response->weights[wi]));
            json_object_object_add(json_resp, "weights", warr);
        }
    }

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
            "percentiles (float[], default [0.01, 0.05]), "
            "model (string, 'gbm' or 'merton', default 'gbm'), "
            "correlated book: covariance_matrix (float[n*n] or float[n][n]) + weights (float[n] >= 0), "
            "optional expected_returns (float[n]), normalize_weights (bool); "
            "jump_intensity (float, default 1.0, jumps/year), "
            "jump_mean (float, default -0.03), "
            "jump_vol (float, default 0.05).\"}");
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
 * A common optimization pattern in portfolio analysis code that handles
 * 500+ assets.
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
            int n_sc_raw = json_object_array_length(sc_arr);
            int n_sc = (n_sc_raw > FINBENCH_MAX_SCENARIOS)
                ? FINBENCH_MAX_SCENARIOS : n_sc_raw;
            /* Store both counts so finbench_calculate_scenarios can fail
             * fast when the caller exceeded the cap rather than silently
             * truncating.  The fixed-size arrays below must never be
             * indexed past FINBENCH_MAX_SCENARIOS, so we still cap the
             * fill loop at n_sc. */
            a->n_scenarios = n_sc;
            a->n_scenarios_requested = n_sc_raw;
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
 * Three-step pipeline:
 *   Step 1 — Fail-fast input validation (probability sums, scenario
 *            count caps, non-positive current_price, empty scenarios).
 *            A bad input fails the whole request here rather than
 *            silently skipping individual assets, which previously
 *            omitted holdings from the result with no indication.
 *   Step 2 — Financial computation: per-asset expected return, upside,
 *            downside; roll up to portfolio level weighted by position
 *            value (asset_position_value = current_price × position_size).
 *   Step 3 — Benchmark pair: n_assets × 10 asset-id lookups performed
 *            once via linear scan (O(n)) and once via axutil_hash (O(1)),
 *            timings reported in the response for direct comparison.
 *
 * The financial formulas per asset are:
 *   expected_return_i = Σ_k  p_k * (scenario_price_k / current_price - 1)
 *   upside_i          = Σ_k  p_k * max(0, scenario_price_k - current_price) * position_size
 *   downside_i        = Σ_k  p_k * max(0, current_price - scenario_price_k) * position_size
 *   weighted_value_i  = Σ_k  p_k * scenario_price_k * position_size
 *
 * upside and downside are separate sums rather than a single signed
 * number, so callers can read distributional SHAPE (small mean return
 * with large dispersion vs small mean with small dispersion) off the
 * response rather than just the mean.
 *
 * Unlike Monte Carlo, there is no PRNG, no Itô correction, and no
 * discretization — this is a deterministic probability-weighted
 * average over a small number of user-supplied price scenarios.
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
        char err_buf[256];

        /* Fail-fast: current_price must be positive — scenario return is
         * computed as (scenario_price / current_price - 1), undefined for
         * non-positive current_price.  Previously this asset was silently
         * skipped, which quietly omitted a holding from the portfolio
         * result without any indication to the caller. */
        if (a->current_price <= 0.0) {
            snprintf(err_buf, sizeof(err_buf),
                "Asset index %d (id=%" PRId64 "): current_price=%.8f is not "
                "positive. Scenario analysis computes return as "
                "(price / current_price - 1), which is undefined for "
                "non-positive current_price.",
                i, a->asset_id, a->current_price);
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env, err_buf);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "ScenarioAnalysis: current_price validation failed - %s",
                err_buf);
            return response;
        }

        /* Fail-fast: must supply at least one scenario — without scenarios
         * the asset contributes nothing to the weighted portfolio, which
         * is almost certainly a caller mistake (empty JSON array or
         * malformed input) rather than a deliberate input. */
        if (a->n_scenarios <= 0) {
            snprintf(err_buf, sizeof(err_buf),
                "Asset index %d (id=%" PRId64 "): scenarios array is missing "
                "or empty. At least one scenario {price, probability} is "
                "required.",
                i, a->asset_id);
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env, err_buf);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "ScenarioAnalysis: scenarios validation failed - %s", err_buf);
            return response;
        }

        /* Fail-fast: reject requests that exceeded the per-asset scenario
         * cap.  The parser records n_scenarios_requested so the rejection
         * message can tell the caller the cap was exceeded rather than
         * silently truncating the input.  Suggest coalescing rare outcomes
         * to stay within the cap. */
        if (a->n_scenarios_requested > FINBENCH_MAX_SCENARIOS) {
            snprintf(err_buf, sizeof(err_buf),
                "Asset index %d (id=%" PRId64 "): scenarios count %d exceeds "
                "maximum %d. Coalesce low-probability outcomes to stay "
                "within the cap.",
                i, a->asset_id, a->n_scenarios_requested,
                FINBENCH_MAX_SCENARIOS);
            response->status = axutil_strdup(env, FINBENCH_STATUS_FAILED);
            response->error_message = axutil_strdup(env, err_buf);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "ScenarioAnalysis: MAX_SCENARIOS exceeded - %s", err_buf);
            return response;
        }

        for (k = 0; k < a->n_scenarios; k++) {
            prob_sum += a->probabilities[k];
        }

        if (prob_sum < (1.0 - prob_tolerance) ||
            prob_sum > (1.0 + prob_tolerance)) {

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

        /* Step 1 above has already rejected any asset with non-positive
         * current_price, missing scenarios, or an over-cap scenario count,
         * so no defensive skip is needed here. */

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
    /* Upside/downside ratio — three distinct regimes:
     *   downside > 0, finite      : standard ratio (the common path)
     *   downside ~ 0, upside > 0  : INFINITY (all-upside portfolio;
     *                               returning 0.0 here would falsely
     *                               imply "no upside")
     *   downside ~ 0, upside ~ 0  : NAN (both sides effectively zero —
     *                               the ratio is genuinely undefined)
     * The 1e-9 threshold avoids treating floating-point residue from the
     * probability-weighted sums as a real non-zero side.
     * Note on JSON: json-c serializes these as "Infinity" / "NaN", which
     * are valid JavaScript Number literals but NOT strict JSON per RFC
     * 8259 — strict parsers must handle the tokens or map them to null. */
    if (total_downside > 1e-9) {
        response->upside_downside_ratio = total_upside / total_downside;
    } else if (total_upside > 1e-9) {
        response->upside_downside_ratio = INFINITY;
    } else {
        response->upside_downside_ratio = NAN;
    }
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
    json_object_array_add(ops_array, json_object_new_string("composeCovariance"));
    json_object_array_add(ops_array, json_object_new_string("covarianceFromReturns"));
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
 * The server-side axis2_json_rpc_msg_recv entry point lives in
 * financial_benchmark_service_handler.c, where msg_ctx is available. This
 * one is for embedders that hold a json_object and no message context —
 * the statically linked Android adapters — and honours the "action" or
 * "operation" field before falling back to request shape.
 */
AXIS2_EXTERN json_object * AXIS2_CALL
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

    /* Log a bounded, control-character-free excerpt of the request. The body
     * is untrusted; a raw copy would let a caller write line breaks into the
     * log and forge entries. */
    json_str = json_object_to_json_string(json_request);
    {
        char excerpt[FINBENCH_LOG_EXCERPT];
        finbench_log_excerpt(json_str, excerpt, sizeof(excerpt));
        AXIS2_LOG_INFO(env->log,
            "FinancialBenchmarkService: Received JSON request: %s", excerpt);
    }

    /* The operation may be named by "action" (the camera service pattern) or
     * "operation" (what the engine adds on the Android path). A key that is
     * present but not a string is a malformed request, not an absent name:
     * refuse it rather than fall through to shape inference. */
    if (json_object_object_get_ex(json_request, "action", &action_obj) ||
        json_object_object_get_ex(json_request, "operation", &action_obj))
    {
        if (!json_object_is_type(action_obj, json_type_string))
        {
            response = json_object_new_object();
            json_object_object_add(response, "status",
                json_object_new_string(FINBENCH_STATUS_FAILED));
            json_object_object_add(response, "error_message",
                json_object_new_string("\"action\"/\"operation\" must be a string naming the operation"));
            return response;
        }
        action = json_object_get_string(action_obj);
    }

    {
        char excerpt[80];
        finbench_log_excerpt(action ? action : "(none - using request structure)",
                             excerpt, sizeof(excerpt));
        AXIS2_LOG_INFO(env->log,
            "FinancialBenchmarkService: Processing action '%s'", excerpt);
    }

    /* Route based on action or detect request type from structure */
    if (action)
    {
        if (strcmp(action, "portfolioVariance") == 0 ||
            strcmp(action, "portfolio_variance") == 0)
        {
            result_str = finbench_portfolio_variance_json_only(env, json_str);
        }
        else if (strcmp(action, "composeCovariance") == 0 ||
                 strcmp(action, "compose_covariance") == 0)
        {
            result_str = finbench_compose_covariance_json_only(env, json_str);
        }
        else if (strcmp(action, "covarianceFromReturns") == 0 ||
                 strcmp(action, "covariance_from_returns") == 0)
        {
            result_str = finbench_covariance_from_returns_json_only(env, json_str);
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
        /* No explicit action - detect operation from request structure.
         * Most specific markers first: a correlated monteCarlo request carries
         * "weights" and "covariance_matrix" exactly as a portfolioVariance
         * request does, so the simulation parameters must be tested before
         * the weights rule or the book would be routed to the variance op.
         *
         * Residual ambiguity, by design: a correlated monteCarlo request that
         * relies on every default -- nothing but "weights" and
         * "covariance_matrix" -- is indistinguishable from a portfolioVariance
         * request and is routed to portfolioVariance. Name the operation to
         * avoid it. Over HTTP the engine always does (see HTTP2_ANDROID.md);
         * MCP dispatches by tool name; only a direct caller can hit this. */
        json_object *temp_obj;

        /* "returns" is covarianceFromReturns' alone: it is the only operation
         * that takes a sample, and monteCarlo's nearest key is the distinct
         * "expected_returns". Tested first because it is unambiguous. */
        if (json_object_object_get_ex(json_request, "returns", &temp_obj))
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected covarianceFromReturns request");
            result_str = finbench_covariance_from_returns_json_only(env, json_str);
        }
        /* Any simulation parameter means Monte Carlo */
        else if (json_object_object_get_ex(json_request, "n_simulations", &temp_obj) ||
                 json_object_object_get_ex(json_request, "initial_value", &temp_obj) ||
                 json_object_object_get_ex(json_request, "expected_return", &temp_obj) ||
                 json_object_object_get_ex(json_request, "expected_returns", &temp_obj) ||
                 json_object_object_get_ex(json_request, "volatility", &temp_obj) ||
                 json_object_object_get_ex(json_request, "random_seed", &temp_obj) ||
                 json_object_object_get_ex(json_request, "model", &temp_obj) ||
                 json_object_object_get_ex(json_request, "n_periods", &temp_obj) ||
                 json_object_object_get_ex(json_request, "percentiles", &temp_obj))
        /* n_periods_per_year is deliberately not a marker: portfolioVariance
         * takes it too, and it must keep routing to the variance operation. */
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected monteCarlo request");
            result_str = finbench_monte_carlo_json_only(env, json_str);
        }
        /* 'weights' and 'covariance_matrix' without simulation parameters: portfolio variance */
        else if (json_object_object_get_ex(json_request, "weights", &temp_obj) &&
                 json_object_object_get_ex(json_request, "covariance_matrix", &temp_obj))
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected portfolioVariance request");
            result_str = finbench_portfolio_variance_json_only(env, json_str);
        }
        /* 'volatilities' is composeCovariance's alone */
        else if (json_object_object_get_ex(json_request, "volatilities", &temp_obj))
        {
            AXIS2_LOG_INFO(env->log,
                "FinancialBenchmarkService: Detected composeCovariance request");
            result_str = finbench_compose_covariance_json_only(env, json_str);
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
