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
 *   where w = weights vector, σ = covariance matrix
 *
 * Intuition for readers new to portfolio math:
 *   A portfolio of N assets has N individual volatilities (how much each
 *   asset moves) AND N×N pairwise correlations (how they move together).
 *   The covariance matrix packages both facts in one 2D structure:
 *       cov[i][j] = vol_i * vol_j * corr_ij
 *   so the diagonal is each asset's variance (vol² — NOT vol) and the
 *   off-diagonal is the co-movement between pairs. A single portfolio
 *   number σ_p collapses all that into one risk figure by weighting each
 *   pairwise term by the caller's asset weights and summing. Lower
 *   (or negative) off-diagonals give diversification benefit: σ_p can be
 *   meaningfully smaller than the weighted-average of individual vols.
 *
 * Time basis (important — frequency consistency):
 *   σ²_p is computed in whatever time basis the input covariance matrix
 *   uses. If the caller provides a DAILY covariance matrix, σ²_p is a
 *   daily variance — in which case "annualized volatility" is computed
 *   by multiplying sqrt(σ²_p) by sqrt(n_periods_per_year). If the caller
 *   already provides an ANNUALIZED matrix (as quants often do), they
 *   should ignore the annualized_volatility field on the response; it
 *   will over-annualize by a factor of sqrt(n_periods_per_year).
 *
 * Numerical edge case — non-PSD input:
 *   A mathematically valid covariance matrix is positive-semi-definite
 *   (PSD). A matrix that is not PSD (from mixed-frequency estimation,
 *   shrinkage gone too far, or bad copy-paste) can yield negative
 *   σ²_p from the w'Σw accumulator. The implementation clamps such
 *   tiny negatives to zero before sqrt to avoid NaN, but callers
 *   who want hard rejection should validate PSD upstream.
 * ============================================================================
 */

/**
 * @brief Portfolio Variance Request
 */
typedef struct finbench_portfolio_variance_request
{
    /** Number of assets in portfolio (must be in [1, FINBENCH_MAX_ASSETS]). */
    int n_assets;

    /**
     * Portfolio weights (array of exactly n_assets). Must sum to 1.0 within
     * 1e-4 unless normalize_weights=true, in which case the service
     * rescales them in place. Fractional / short positions (negative
     * weights) are allowed; the math is identical for long-short books.
     */
    double *weights;

    /**
     * Covariance matrix, flattened row-major: element (i,j) is at index
     * i * n_assets + j. The matrix MUST be on a per-period basis — e.g.,
     * daily returns covariance when n_periods_per_year = 252, or annual
     * returns covariance when n_periods_per_year = 1.
     * A real covariance matrix is symmetric and positive-semi-definite;
     * the implementation does not enforce symmetry, so cov[i][j] != cov[j][i]
     * silently produces a different w'Σw than a symmetrized version.
     */
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
 *
 * Model: Geometric Brownian Motion (GBM), the standard textbook process
 * for equity prices under the risk-neutral measure. Each period:
 *   S(t+dt) = S(t) * exp((μ − σ²/2) * dt  +  σ * sqrt(dt) * Z)
 *   where Z ~ N(0,1), μ = expected_return, σ = volatility, dt = 1/n_periods_per_year.
 *
 * Intuition for readers new to GBM:
 *   GBM produces log-normal terminal prices: S(T)/S(0) is log-normal, so
 *   S(T) is always > 0 (prices cannot go negative). The exp(...) is the
 *   multiplicative return over one time step, and compounding one step
 *   to the next gives the full path. The {@code −σ²/2} correction
 *   (Itô's lemma) ensures E[S(T)] = S(0) * exp(μ * T); WITHOUT it, the
 *   expected terminal value would drift upward with higher volatility.
 *   This correction is the single most common bug in home-grown GBM
 *   code and is implemented here verbatim.
 *
 * VaR sign convention: var_95, var_99, and cvar_95 are returned as
 *   POSITIVE LOSS MAGNITUDES in base-currency units. So var_95 = 252000
 *   means "there is a 5% chance of losing $252k or more over the
 *   simulated horizon". A negative VaR is meaningful and indicates a
 *   profit at that percentile (the tail outcome is still a gain); a
 *   zero VaR means break-even at that percentile.
 *
 * Estimator detail (floor(p * N) percentile rule):
 *   After sorting ascending, VaR at level p is computed as
 *       var_p = initial_value - sorted_final_values[floor(p * n_sims)]
 *   and CVaR at 95% is the average of the floor(0.05 * n_sims) worst
 *   outcomes (positions 0..idx_5-1 after sort). This matches the textbook
 *   E[L | L ≥ VaR_95] to within one observation for large n_sims; systems
 *   that reconcile against NumPy / R should be aware that alternate
 *   estimators (e.g., ceil((1-p)*N)-1, or interpolation between ranks)
 *   can give minutely different numbers.
 *
 * Reproducibility (PRNG scope):
 *   When random_seed != 0, the xorshift128plus PRNG is seeded
 *   deterministically, so repeated runs with the same seed yield
 *   bit-identical output AGAINST THIS IMPLEMENTATION. Reproducibility
 *   is per-PRNG, NOT cross-PRNG: the same seed under java.util.Random
 *   (LCG), NumPy's Generator (PCG64), or Mersenne Twister produces
 *   DIFFERENT numbers. Cross-system comparison must use the same PRNG
 *   or compare distributions rather than bit-exact outputs.
 *
 * Numerical edge cases:
 *   - exp() overflow: extreme volatility × long horizon can push the
 *     GBM exponent past ~709 (log DBL_MAX). The implementation caps the
 *     exponent and marks such paths as an extreme outcome rather than
 *     letting +Inf propagate through sort/sum into NaN. NOTE: this
 *     divergent from Axis2/Java, which deliberately does NOT cap —
 *     Java prefers NaN propagation as an alarm. Both choices are
 *     defensible for their respective ecosystems.
 *   - Variance cancellation: the streaming estimator (sumSq/N − mean²)
 *     can go slightly negative from floating-point cancellation on
 *     near-constant samples. The implementation clamps to 0.0 before
 *     the final sqrt so std_dev_final_value is never NaN.
 * ============================================================================
 */

/**
 * @brief Monte Carlo Simulation Request
 */
typedef struct finbench_monte_carlo_request
{
    /**
     * Number of simulation paths. More paths → smoother VaR / CVaR
     * estimates but linear additional work. Must be in [1, FINBENCH_MAX_SIMULATIONS].
     * Default when absent from JSON: 10000.
     */
    int n_simulations;

    /**
     * Number of time steps per simulated path. The simulated horizon in
     * years is n_periods / n_periods_per_year. A 1-day VaR uses n_periods=1
     * with n_periods_per_year=252; a 10-day regulatory VaR uses n_periods=10.
     * Default: 252 (one trading year at daily granularity).
     */
    int n_periods;

    /**
     * Starting portfolio value. Must be > 0 — GBM evolves multiplicatively
     * through exp(...) and is undefined for non-positive starting values.
     * The service rejects initial_value <= 0 fail-fast.
     * Default: 1,000,000.0.
     */
    double initial_value;

    /**
     * ANNUALIZED expected return as a decimal. 0.08 means 8% per year
     * (NOT 8.0). This is the μ parameter of the GBM log-return process
     * on an annual basis; the service de-annualizes to per-step drift
     * via dt = 1 / n_periods_per_year. Default: 0.08.
     */
    double expected_return;

    /**
     * ANNUALIZED volatility as a decimal. 0.20 means 20% annualized std
     * dev (NOT 20.0). Must be >= 0 (the service rejects negative values).
     * For extreme values (σ > 1.0 i.e. > 100%) combined with long horizons,
     * the exp(...) term in the GBM inner loop can overflow to +Inf; the
     * implementation caps the exponent defensively so one blown path
     * does not corrupt the entire response. Default: 0.20.
     */
    double volatility;

    /**
     * PRNG seed. 0 (default) → seeded from time(NULL), producing
     * non-deterministic runs. A non-zero value seeds the xorshift128plus
     * state deterministically so repeated runs with the same seed
     * produce bit-identical output under THIS implementation. Cross-PRNG
     * reproducibility does not hold — see the Monte Carlo section header
     * for details.
     */
    uint32_t random_seed;

    /**
     * Number of time steps per year used to de-annualize μ and σ for the
     * simulation step dt = 1 / n_periods_per_year. This picks the
     * discretization GRANULARITY — it does not change the input frequency
     * (expected_return and volatility are ALWAYS annualized, see above).
     * For equity daily simulation: 252 (US trading days) or 260
     * (calendar-day-ish). For weekly: 52. For monthly: 12. Default: 252.
     * Mismatching the granularity with n_periods (e.g., n_periods_per_year=1
     * with n_periods=252 to simulate "a year") silently produces a
     * mathematically different VaR — the horizon becomes 252 years of
     * annual steps rather than one year of daily steps.
     */
    int n_periods_per_year;

    /**
     * Percentile levels for VaR reporting, values strictly in (0, 1). The
     * service interprets p as a LOSS PROBABILITY, so 0.05 means "5% chance
     * of losing at least VaR_p" (i.e., a 95% VaR). Defaults: {0.01, 0.05}.
     * Up to FINBENCH_MAX_PERCENTILES values; any beyond that are ignored.
     *
     * Estimator: floor(p * n_sims). After ascending sort,
     *   VaR_p = initial_value - sorted_final_values[floor(p * n_sims)]
     * so p = 0.01 gives the 1st-percentile WORST outcome. Callers
     * reconciling against other risk systems should note that
     * interpolation-based estimators and ceil((1-p)*N)-1 can differ by
     * one observation; the difference shrinks as n_sims grows.
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
 *
 * Financial computation per asset:
 *   expected_return_i = Σ_k  p_k * (scenario_price_k / current_price - 1)
 *   upside_i          = Σ_k  p_k * max(0, scenario_price_k - current_price) * position_size
 *   downside_i        = Σ_k  p_k * max(0, current_price - scenario_price_k) * position_size
 *   weighted_value_i  = Σ_k  p_k * scenario_price_k * position_size
 *
 * Intuition for readers new to scenario analysis:
 *   Unlike Monte Carlo (which SAMPLES from a continuous distribution),
 *   scenario analysis sums over a FEW discrete user-defined outcomes
 *   (e.g., "bull", "base", "bear") each weighted by a subjective
 *   probability. There is no Itô correction and no PRNG — the output
 *   is a deterministic weighted average. This is the right tool when
 *   the caller has a view on a small number of specific futures and
 *   wants the portfolio-level expected figures.
 *
 * Upside/downside asymmetry:
 *   The upside and downside sums are NOT symmetric around
 *   expected_return. A portfolio can have small expected return but
 *   large upside AND large downside (high dispersion), or small
 *   expected return with both sides modest (low dispersion). Reporting
 *   them separately lets callers see the distributional shape, not
 *   just the mean.
 *
 * upside_downside_ratio semantics:
 *   - finite positive value : common case; > 1 means more expected
 *                              upside than downside
 *   - Double.INFINITY        : downside ≈ 0, upside > 0 — the portfolio
 *                              has no modeled downside in these
 *                              scenarios
 *   - Double.NaN             : both sides ≈ 0 — the ratio is genuinely
 *                              undefined (not "no upside")
 *
 *   Note on JSON: json-c serializes Infinity and NaN as "Infinity" /
 *   "NaN" string tokens, which are valid JavaScript Number literals but
 *   NOT strict JSON per RFC 8259. Strict JSON parsers must configure
 *   their parser or map these to null before parsing.
 *
 * Benchmark shape: asset_id lookups are performed n_assets × 10 times,
 * once via linear scan and once via axutil_hash, so the linear-vs-hash
 * timing comparison is always visible in the response.
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
     * Each asset's scenario probabilities must sum to 1.0 within this
     * tolerance; the service rejects the whole request otherwise.
     * Default: 1e-4 (0.01%). Passing 0.0 or any negative value falls back
     * to this default. Values > 0.1 are capped at 0.1 to prevent an
     * overly-loose tolerance from hiding real probability-sum bugs
     * (e.g., three scenarios at 0.3 each summing to 0.9).
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
