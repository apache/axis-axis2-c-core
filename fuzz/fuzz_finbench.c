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
 * @file fuzz_finbench.c
 * @brief OSS-Fuzz target for the Financial Benchmark Service
 *
 * This fuzzer exercises the complete request→compute→response path
 * of the financial benchmark service with random/malformed inputs:
 *
 * - portfolioVariance: O(n²) covariance matrix computation
 *   Tests: integer overflow in n_assets*n_assets, weight normalization
 *   div-by-zero, negative variance sqrt, array bounds
 *
 * - monteCarlo: GBM simulation with exp() overflow guard
 *   Tests: extreme volatility → exp() overflow, zero volatility,
 *   negative expected return, 1-simulation edge case, percentile
 *   out-of-bounds
 *
 * - scenarioAnalysis: hash table O(1) vs O(n) benchmark
 *   Tests: probability sum validation, zero prices, hash key
 *   collision, empty asset list
 *
 * The fuzzer feeds raw bytes as JSON to the service's request parsers,
 * which must handle any input without crashing. This catches the same
 * class of bugs that the pandemic stress test exercises but with far
 * more input diversity — millions of random inputs per day on OSS-Fuzz
 * infrastructure.
 *
 * Attack vectors:
 * - Malformed JSON (missing fields, wrong types, nested garbage)
 * - Extreme numeric values (INT_MAX, -0.0, NaN, Inf in JSON)
 * - Oversized arrays (n_assets near MAX, huge covariance matrices)
 * - Empty/null strings for request_id
 * - Memory exhaustion from large n_simulations
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>

/* Include the financial benchmark service header */
#include "../samples/user_guide/financial-benchmark-service/src/financial_benchmark_service.h"

static axutil_env_t *create_env(void)
{
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    axutil_error_t *error = axutil_error_create(allocator);
    axutil_log_t *log = axutil_log_create(allocator, NULL, NULL);
    axutil_thread_pool_t *pool = axutil_thread_pool_init(allocator);
    return axutil_env_create_with_error_log_thread_pool(allocator, error, log, pool);
}

/**
 * Main fuzzer entry point — called millions of times with random data.
 * The data is treated as a JSON string and fed to each service parser.
 */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    static axutil_env_t *env = NULL;
    if (!env) {
        env = create_env();
        if (!env) return 0;
    }

    /* Skip empty or oversized inputs (OSS-Fuzz best practice) */
    if (size == 0 || size > 1024 * 1024) {
        return 0;
    }

    /* Null-terminate the input for JSON parsing */
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) return 0;
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    /*
     * Exercise 1: Portfolio Variance request parsing + computation.
     * This hits the O(n²) covariance path, weight normalization,
     * and the variance→sqrt guard.
     */
    {
        finbench_portfolio_variance_request_t *req =
            finbench_portfolio_variance_request_create_from_json(env, json_str);
        if (req) {
            finbench_portfolio_variance_response_t *resp =
                finbench_calculate_portfolio_variance(env, req);
            if (resp) {
                /* Exercise response serialization */
                axis2_char_t *json_out =
                    finbench_portfolio_variance_response_to_json(resp, env);
                if (json_out) {
                    AXIS2_FREE(env->allocator, json_out);
                }
                finbench_portfolio_variance_response_free(resp, env);
            }
            finbench_portfolio_variance_request_free(req, env);
        }
    }

    /*
     * Exercise 2: Monte Carlo request parsing + GBM simulation.
     * This hits the exp() overflow guard, percentile clamping,
     * CVaR div-by-zero guard, and the xorshift128+ PRNG.
     */
    {
        finbench_monte_carlo_request_t *req =
            finbench_monte_carlo_request_create_from_json(env, json_str);
        if (req) {
            /* Cap simulations for fuzzing performance — don't burn
             * CPU on 1M sims for each of millions of fuzz inputs */
            if (req->n_simulations > 100) req->n_simulations = 100;
            if (req->n_periods > 50) req->n_periods = 50;

            finbench_monte_carlo_response_t *resp =
                finbench_calculate_monte_carlo(env, req);
            if (resp) {
                axis2_char_t *json_out =
                    finbench_monte_carlo_response_to_json(resp, env);
                if (json_out) {
                    AXIS2_FREE(env->allocator, json_out);
                }
                finbench_monte_carlo_response_free(resp, env);
            }
            finbench_monte_carlo_request_free(req, env);
        }
    }

    /*
     * Exercise 3: Scenario Analysis request parsing + hash benchmark.
     * This hits the hash table key management, probability validation,
     * and the O(n) vs O(1) benchmark paths.
     */
    {
        finbench_scenario_analysis_request_t *req =
            finbench_scenario_analysis_request_create_from_json(env, json_str);
        if (req) {
            finbench_scenario_analysis_response_t *resp =
                finbench_calculate_scenario_analysis(env, req);
            if (resp) {
                axis2_char_t *json_out =
                    finbench_scenario_analysis_response_to_json(resp, env);
                if (json_out) {
                    AXIS2_FREE(env->allocator, json_out);
                }
                finbench_scenario_analysis_response_free(resp, env);
            }
            finbench_scenario_analysis_request_free(req, env);
        }
    }

    free(json_str);
    return 0;
}
