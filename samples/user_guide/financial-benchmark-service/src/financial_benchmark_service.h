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

/* Maximum supported assets for benchmark.
 * Enterprise portfolio management systems commonly have 2000+ assets
 * per department. Cap at 3000 to cover large portfolios with margin.
 * Note: 3000-asset covariance matrix payload is ~135 MB (O(n²)),
 * reachable via MCP stdio but exceeds HTTP 50MB LimitRequestBody. */
#define FINBENCH_MAX_ASSETS         3000
#define FINBENCH_MAX_SCENARIOS      10
#define FINBENCH_MAX_SIMULATIONS    1000000

/** Maximum number of percentile levels accepted in a monteCarlo request */
#define FINBENCH_MAX_PERCENTILES    8

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

    /** Actual array lengths as parsed (for dimension validation before computation) */
    int weights_provided;         /* length of weights[] in JSON */
    int matrix_elements_provided; /* length of covariance_matrix[] in JSON (flat count) */

    /**
     * Normalize weights to sum to 1.0 before computing variance.
     * When AXIS2_TRUE, weights are rescaled in-place if their sum != 1.0.
     * When AXIS2_FALSE (default), weights that do not sum to 1.0 return an error.
     * Quants often work with unnormalized exposures; this avoids a pre-processing step.
     */
    axis2_bool_t normalize_weights;

    /**
     * Trading periods per year used for annualizing volatility.
     * Default: 252 (equity trading days). Use 260 for some fixed-income conventions,
     * 365 for crypto, or 12 for monthly data.
     */
    int n_periods_per_year;

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

    /** Annualized volatility (σ × sqrt(n_periods_per_year)) */
    double annualized_volatility;

    /** Actual sum of weights as provided (before any normalization) */
    double weight_sum;

    /** Whether weights were renormalized to 1.0 (only when normalize_weights=true) */
    axis2_bool_t weights_normalized;

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

    /**
     * Trading periods per year — controls the GBM time step (dt = 1/n_periods_per_year).
     * Default: 252 (equity trading days). Must match the frequency of expected_return
     * and volatility inputs (both must be annualized).
     */
    int n_periods_per_year;

    /**
     * Percentile levels for VaR reporting (values in [0,1], e.g. 0.01 for 1%).
     * Defaults: {0.01, 0.05} — the 99% and 95% VaR levels.
     * Up to FINBENCH_MAX_PERCENTILES values; extras are ignored.
     * Each percentile p produces: VaR_p = initial_value - sorted_final_values[p * n_sims].
     */
    double percentiles[FINBENCH_MAX_PERCENTILES];

    /** Number of entries in percentiles[]. 0 → use defaults {0.01, 0.05}. */
    int n_percentiles;

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

    /** Value at Risk 95% (5th percentile loss) — always populated */
    double var_95;

    /** Value at Risk 99% (1st percentile loss) — always populated */
    double var_99;

    /** Conditional VaR 95% (Expected Shortfall) — always populated */
    double cvar_95;

    /**
     * Per-request percentile VaR values (parallel to request->percentiles[]).
     * var_at_percentile[i] = initial_value - sorted_final_values[percentiles[i] * n_sims]
     * Count is stored in n_percentiles; up to 8 entries.
     */
    double var_at_percentile[FINBENCH_MAX_PERCENTILES];
    double percentile_levels[FINBENCH_MAX_PERCENTILES];
    int n_percentiles;

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

    /** Number of scenarios defined (after capping at FINBENCH_MAX_SCENARIOS) */
    int n_scenarios;

    /**
     * Raw scenario count as supplied in the JSON request, BEFORE capping.
     * When the caller sends more than FINBENCH_MAX_SCENARIOS scenarios the
     * parser only fills the first FINBENCH_MAX_SCENARIOS entries but records
     * the original count here so that finbench_calculate_scenarios can
     * fail fast with a specific error rather than silently dropping data.
     */
    int n_scenarios_requested;

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

    /**
     * Tolerance for probability sum validation.
     * Each asset's scenario probabilities must sum to 1.0 within this tolerance.
     * Default: 1e-4 (0.01%) — accommodates JSON representation rounding while
     * catching genuinely miscounted scenarios (e.g., three scenarios summing to 0.85).
     * Pass 0.0 to use the default. Values > 0.1 are clamped to 0.1.
     */
    double prob_tolerance;

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
