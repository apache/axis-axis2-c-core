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

#ifndef FINANCIAL_BENCHMARK_SERVICE_H
#define FINANCIAL_BENCHMARK_SERVICE_H

/**
 * @file financial_benchmark_service.h
 * @brief Apache Axis2/C HTTP/2 Financial Benchmark Service Header
 *
 * Demonstrates Axis2/C performance on resource-constrained hardware
 * (old Android phones, embedded systems) where Java/WildFly cannot run.
 *
 * Features:
 * - Portfolio variance calculation (O(n²) matrix operations)
 * - Monte Carlo simulation (compute-intensive)
 * - Scenario analysis with hash table lookups
 * - Memory-efficient operation for 1-2GB devices
 * - HTTP/2 JSON transport using pure json-c
 *
 * Use Case: "Single Camera" equivalent - one old Android phone running
 * financial calculations that would require expensive server hardware
 * with Java/WildFly stack.
 */

#include <axutil_env.h>
#include <axutil_hash.h>
#include <json-c/json.h>
#include <time.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Maximum supported assets for benchmark */
#define FINBENCH_MAX_ASSETS         2000
#define FINBENCH_MAX_SCENARIOS      10
#define FINBENCH_MAX_SIMULATIONS    1000000

/* Memory constraint for Android devices */
#define FINBENCH_MEMORY_CONSTRAINT_MB   512

/* Status constants */
#define FINBENCH_STATUS_SUCCESS     "SUCCESS"
#define FINBENCH_STATUS_FAILED      "FAILED"

/* ============================================================================
 * Portfolio Variance Calculation
 *
 * Calculates portfolio variance using covariance matrix multiplication.
 * This is an O(n²) operation that mirrors common correlation calculations.
 *
 * Formula: σ²_p = Σ_i Σ_j w_i * w_j * σ_ij
 * where w = weights vector, σ = covariance matrix
 * ============================================================================
 */

/**
 * @brief Portfolio Variance Request
 */
typedef struct finbench_portfolio_variance_request
{
    /** Number of assets in portfolio */
    int n_assets;

    /** Portfolio weights (array of n_assets) */
    double *weights;

    /** Covariance matrix (flattened n_assets × n_assets) */
    double *covariance_matrix;

    /** Optional: Asset identifiers for reporting */
    char **asset_ids;

    /** Request identifier for tracing */
    char *request_id;

} finbench_portfolio_variance_request_t;

/**
 * @brief Portfolio Variance Response
 */
typedef struct finbench_portfolio_variance_response
{
    /** Processing status */
    char *status;

    /** Portfolio variance (σ²) */
    double portfolio_variance;

    /** Portfolio volatility (σ = sqrt(variance)) */
    double portfolio_volatility;

    /** Annualized volatility (σ × sqrt(252)) */
    double annualized_volatility;

    /** Processing time in microseconds */
    long calc_time_us;

    /** Peak memory used in KB */
    int memory_used_kb;

    /** Number of matrix operations performed */
    long matrix_operations;

    /** Throughput: operations per second */
    double ops_per_second;

    /** Error message (if status == FAILED) */
    char *error_message;

    /** Request ID echo */
    char *request_id;

    /** Device info for demo purposes */
    char *device_info;

} finbench_portfolio_variance_response_t;

/* ============================================================================
 * Monte Carlo Simulation
 *
 * Compute-intensive simulation for Value at Risk (VaR) calculations.
 * Demonstrates raw compute performance advantage of C over JVM.
 * ============================================================================
 */

/**
 * @brief Monte Carlo Simulation Request
 */
typedef struct finbench_monte_carlo_request
{
    /** Number of simulation paths */
    int n_simulations;

    /** Number of time periods (e.g., 252 trading days) */
    int n_periods;

    /** Initial portfolio value */
    double initial_value;

    /** Expected annual return (e.g., 0.08 for 8%) */
    double expected_return;

    /** Annual volatility (e.g., 0.20 for 20%) */
    double volatility;

    /** Random seed for reproducibility (0 = use time) */
    uint32_t random_seed;

    /** Request identifier */
    char *request_id;

} finbench_monte_carlo_request_t;

/**
 * @brief Monte Carlo Simulation Response
 */
typedef struct finbench_monte_carlo_response
{
    /** Processing status */
    char *status;

    /** Mean final portfolio value */
    double mean_final_value;

    /** Median final portfolio value */
    double median_final_value;

    /** Standard deviation of final values */
    double std_dev_final_value;

    /** Value at Risk 95% (5th percentile loss) */
    double var_95;

    /** Value at Risk 99% (1st percentile loss) */
    double var_99;

    /** Conditional VaR 95% (Expected Shortfall) */
    double cvar_95;

    /** Maximum drawdown observed */
    double max_drawdown;

    /** Probability of profit */
    double prob_profit;

    /** Processing time in microseconds */
    long calc_time_us;

    /** Simulations per second achieved */
    double simulations_per_second;

    /** Memory used in KB */
    int memory_used_kb;

    /** Error message (if any) */
    char *error_message;

    /** Request ID echo */
    char *request_id;

} finbench_monte_carlo_response_t;

/* ============================================================================
 * Scenario Analysis (mirrors common financial patterns)
 *
 * Demonstrates O(1) hash table lookups vs O(n) array scans.
 * ============================================================================
 */

/**
 * @brief Asset Scenario Data
 */
typedef struct finbench_asset_scenario
{
    /** Asset identifier */
    int64_t asset_id;

    /** Current market price */
    double current_price;

    /** Target prices for each scenario */
    double scenario_prices[FINBENCH_MAX_SCENARIOS];

    /** Probability weights for each scenario */
    double probabilities[FINBENCH_MAX_SCENARIOS];

    /** Number of scenarios defined */
    int n_scenarios;

    /** Position size (shares) */
    double position_size;

} finbench_asset_scenario_t;

/**
 * @brief Scenario Analysis Request
 */
typedef struct finbench_scenario_request
{
    /** Number of assets */
    int n_assets;

    /** Array of asset scenarios */
    finbench_asset_scenario_t *assets;

    /** Enable hash table optimization (for comparison) */
    axis2_bool_t use_hash_lookup;

    /** Request identifier */
    char *request_id;

} finbench_scenario_request_t;

/**
 * @brief Scenario Analysis Response
 */
typedef struct finbench_scenario_response
{
    /** Processing status */
    char *status;

    /** Expected portfolio return */
    double expected_return;

    /** Probability-weighted portfolio value */
    double weighted_value;

    /** Upside potential (expected gain in positive scenarios) */
    double upside_potential;

    /** Downside risk (expected loss in negative scenarios) */
    double downside_risk;

    /** Upside/Downside ratio */
    double upside_downside_ratio;

    /** Processing time in microseconds */
    long calc_time_us;

    /** Number of lookups performed */
    long lookups_performed;

    /** Lookups per second */
    double lookups_per_second;

    /** Memory used in KB */
    int memory_used_kb;

    /** Hash vs Linear comparison (if applicable) */
    char *lookup_method;

    /** Error message */
    char *error_message;

    /** Request ID echo */
    char *request_id;

} finbench_scenario_response_t;

/* ============================================================================
 * Service Metadata
 * ============================================================================
 */

/**
 * @brief Service Metadata Response
 */
typedef struct finbench_metadata_response
{
    /** Service name */
    char *service_name;

    /** Service version */
    char *version;

    /** Build date */
    char *build_date;

    /** Supported operations */
    char **operations;
    int n_operations;

    /** Hardware info */
    char *hardware_info;

    /** Memory available MB */
    int memory_available_mb;

    /** Memory constraint MB */
    int memory_constraint_mb;

    /** Max assets supported */
    int max_assets;

    /** Max simulations supported */
    int max_simulations;

} finbench_metadata_response_t;

/* ============================================================================
 * Function Declarations - Portfolio Variance
 * ============================================================================
 */

/**
 * @brief Create portfolio variance request from JSON
 */
AXIS2_EXTERN finbench_portfolio_variance_request_t* AXIS2_CALL
finbench_portfolio_variance_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string);

/**
 * @brief Free portfolio variance request
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_portfolio_variance_request_free(
    finbench_portfolio_variance_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Create portfolio variance response
 */
AXIS2_EXTERN finbench_portfolio_variance_response_t* AXIS2_CALL
finbench_portfolio_variance_response_create(
    const axutil_env_t *env);

/**
 * @brief Convert portfolio variance response to JSON
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_portfolio_variance_response_to_json(
    const finbench_portfolio_variance_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Free portfolio variance response
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_portfolio_variance_response_free(
    finbench_portfolio_variance_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Calculate portfolio variance (main operation)
 *
 * O(n²) matrix calculation: σ²_p = Σ_i Σ_j w_i * w_j * σ_ij
 */
AXIS2_EXTERN finbench_portfolio_variance_response_t* AXIS2_CALL
finbench_calculate_portfolio_variance(
    const axutil_env_t *env,
    finbench_portfolio_variance_request_t *request);

/**
 * @brief Process portfolio variance with pure JSON (HTTP/2 endpoint)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_portfolio_variance_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

/* ============================================================================
 * Function Declarations - Monte Carlo
 * ============================================================================
 */

/**
 * @brief Create Monte Carlo request from JSON
 */
AXIS2_EXTERN finbench_monte_carlo_request_t* AXIS2_CALL
finbench_monte_carlo_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string);

/**
 * @brief Free Monte Carlo request
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_monte_carlo_request_free(
    finbench_monte_carlo_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Create Monte Carlo response
 */
AXIS2_EXTERN finbench_monte_carlo_response_t* AXIS2_CALL
finbench_monte_carlo_response_create(
    const axutil_env_t *env);

/**
 * @brief Convert Monte Carlo response to JSON
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_monte_carlo_response_to_json(
    const finbench_monte_carlo_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Free Monte Carlo response
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_monte_carlo_response_free(
    finbench_monte_carlo_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Run Monte Carlo simulation (main operation)
 */
AXIS2_EXTERN finbench_monte_carlo_response_t* AXIS2_CALL
finbench_run_monte_carlo(
    const axutil_env_t *env,
    finbench_monte_carlo_request_t *request);

/**
 * @brief Process Monte Carlo with pure JSON (HTTP/2 endpoint)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_monte_carlo_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

/* ============================================================================
 * Function Declarations - Scenario Analysis
 * ============================================================================
 */

/**
 * @brief Create scenario request from JSON
 */
AXIS2_EXTERN finbench_scenario_request_t* AXIS2_CALL
finbench_scenario_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string);

/**
 * @brief Free scenario request
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_scenario_request_free(
    finbench_scenario_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Create scenario response
 */
AXIS2_EXTERN finbench_scenario_response_t* AXIS2_CALL
finbench_scenario_response_create(
    const axutil_env_t *env);

/**
 * @brief Convert scenario response to JSON
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_scenario_response_to_json(
    const finbench_scenario_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Free scenario response
 */
AXIS2_EXTERN void AXIS2_CALL
finbench_scenario_response_free(
    finbench_scenario_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Calculate scenario analysis (main operation)
 */
AXIS2_EXTERN finbench_scenario_response_t* AXIS2_CALL
finbench_calculate_scenarios(
    const axutil_env_t *env,
    finbench_scenario_request_t *request);

/**
 * @brief Process scenario analysis with pure JSON (HTTP/2 endpoint)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_scenario_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

/* ============================================================================
 * Function Declarations - Service Metadata
 * ============================================================================
 */

/**
 * @brief Get service metadata as JSON
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_get_metadata_json(
    const axutil_env_t *env);

/* ============================================================================
 * Utility Functions
 * ============================================================================
 */

/**
 * @brief Get current memory usage in KB
 */
AXIS2_EXTERN int AXIS2_CALL
finbench_get_memory_usage_kb(void);

/**
 * @brief Get device/hardware info string
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_get_device_info(
    const axutil_env_t *env);

/**
 * @brief Generate test portfolio data (for benchmarking)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_generate_test_portfolio_json(
    const axutil_env_t *env,
    int n_assets);

#ifdef __cplusplus
}
#endif

#endif /* FINANCIAL_BENCHMARK_SERVICE_H */
