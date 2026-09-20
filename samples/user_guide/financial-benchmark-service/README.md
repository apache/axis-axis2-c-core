# Financial Benchmark Service

**Apache Axis2/C HTTP/2 JSON Financial Benchmark Service**

## Overview

This service demonstrates Axis2/C performance on **resource-constrained hardware** where Java/WildFly cannot run. It implements real financial calculations used in portfolio management systems.

### The "Single Camera" Use Case

Just as the Kanaha Camera Control system provides value with a single old Android phone, this benchmark service demonstrates that **one modest device** can run production-grade financial calculations.

The Java comparison here is not a straw man — it reflects real enterprise production deployments. Large financial services organizations routinely run **70+ WildFly servers**, all configured at **16 GB RAM minimum**, with the majority at **32 GB or higher**, running portfolio management, securities master, and data processing services under heavy load. These are the real numbers:

| Platform | Edge Device (2 GB) | Enterprise Production Reality |
|----------|--------------------|-----------------------|
| **Java/WildFly 32** | ❌ Cannot start | 16–32+ GB per server (70+ servers) |
| **Node.js/V8** | ⚠️ OOM risk | 4–8 GB |
| **Axis2/C 2.0** | ✅ Runs comfortably | ~30 MB (unchanged at any scale) |

## Operations

### 1. Portfolio Variance (`/portfolioVariance`)

Calculates portfolio variance using covariance matrix multiplication.

**Formula**: σ²_p = Σ_i Σ_j w_i * w_j * σ_ij

**Complexity**: O(n²) - mirrors typical portfolio correlation calculations

| Asset Count | Operations | Target Time |
|-------------|------------|-------------|
| 100 | 10,000 | <1 ms |
| 500 | 250,000 | <10 ms |
| 1,000 | 1,000,000 | <50 ms |
| 2,000 | 4,000,000 | <200 ms |

**Example Request**:
```json
{
    "n_assets": 500,
    "weights": [0.002, 0.002, ...],
    "covariance_matrix": [0.04, 0.006, ...],
    "n_periods_per_year": 1,
    "request_id": "test-001"
}
```

**Example Response**:
```json
{
    "status": "SUCCESS",
    "portfolio_variance": 0.0156,
    "portfolio_volatility": 0.125,
    "annualized_volatility": 0.125,
    "calc_time_us": 4523,
    "matrix_operations": 250000,
    "ops_per_second": 55273894,
    "memory_used_kb": 28456,
    "device_info": "Android Device (Axis2/C 2.0 HTTP/2 JSON)"
}
```

### 2. Monte Carlo Simulation (`/monteCarlo`)

Runs Value at Risk (VaR) simulation using Geometric Brownian Motion.

**Complexity**: n_simulations × n_periods iterations

| Simulations | Periods | Iterations | Target Time |
|-------------|---------|------------|-------------|
| 1,000 | 252 | 252,000 | <10 ms |
| 10,000 | 252 | 2,520,000 | <100 ms |
| 100,000 | 252 | 25,200,000 | <1 s |

**Example Request**:
```json
{
    "n_simulations": 10000,
    "n_periods": 252,
    "initial_value": 1000000,
    "expected_return": 0.08,
    "volatility": 0.20,
    "random_seed": 12345
}
```

**Example Response**:
```json
{
    "status": "SUCCESS",
    "mean_final_value": 1082345.67,
    "var_95": 145678.90,
    "var_99": 234567.89,
    "max_drawdown": 0.234,
    "prob_profit": 0.72,
    "calc_time_us": 234567,
    "simulations_per_second": 42650
}
```

### 3. Compose Covariance (`/composeCovariance`)

Builds the covariance matrix that `portfolioVariance` consumes from per-asset
volatilities and a correlation structure, and refuses to return one that is not
positive definite.

**Formula**: Σ_ij = vol_i × vol_j × R_ij  (Σ = D·R·D)

`R` is either a full n×n correlation matrix or one uniform correlation `rho`
applied to every pair. The result is checked with a Cholesky decomposition
(O(n³/3)); a matrix that fails returns `FAILED` with the failing index and no
numbers, so nothing non-PSD can reach `portfolioVariance` by this route.

Why it exists: a hypothetical regime ("every vol at 22%, correlation 0.5") has
no covariance matrix to hand, and vols and correlations are short, bounded and
checkable where a raw 25-number matrix is not. The result inherits the vols'
time basis: annualized vols in, annualized Σ out, to be passed to
`portfolioVariance` with `n_periods_per_year: 1`.

**Example Request** (uniform correlation, the stress case):
```json
{
    "volatilities": [0.243, 0.244, 0.314, 0.219, 0.167],
    "correlation": 0.8,
    "asset_ids": ["MSFT", "AAPL", "AMZN", "JPM", "JNJ"]
}
```

**Example Response**:
```json
{
    "status": "SUCCESS",
    "n_assets": 5,
    "covariance_matrix": [0.059049, 0.047434, 0.061042, 0.042574, 0.032465, ...],
    "correlation_matrix": [1.0, 0.8, 0.8, 0.8, 0.8, 0.8, 1.0, ...],
    "volatilities": [0.243, 0.244, 0.314, 0.219, 0.167],
    "positive_definite": true,
    "positive_definite_checked": true,
    "cholesky_failed_at": -1,
    "min_pivot": 0.00689,
    "calc_time_us": 3,
    "asset_ids": ["MSFT", "AAPL", "AMZN", "JPM", "JNJ"]
}
```

Feeding that `covariance_matrix` to `portfolioVariance` with weights
`[0.25, 0.25, 0.20, 0.15, 0.15]` and `n_periods_per_year: 1` gives a portfolio
volatility of 0.2228; the same five vols with the historical correlation matrix
give 0.1567.

**Refusals** name the field: a vol that is not > 0, a `rho` outside [−1, 1], an
asymmetric or non-unit-diagonal `correlation_matrix`, both forms supplied at
once, or a Cholesky failure (`"Not positive definite: Cholesky failed at index
2. With 5 assets a uniform correlation must satisfy -0.25 < rho < 1; got -0.5."`).
`check_positive_definite: false` skips the O(n³) check for very large n.

### 4. Scenario Analysis (`/scenarioAnalysis`)

Demonstrates O(1) hash table lookups vs O(n) linear search — a common optimization in enterprise portfolio systems migrating from array scans to hash-based asset lookups.

### 5. Service Metadata (`/metadata`)

Returns service capabilities, device info, and memory usage.

## Building

### Prerequisites

- Axis2/C 2.0 with HTTP/2 and JSON enabled
- json-c library (libjson-c-dev)
- For Android: NDK r21+ with arm64-v8a support

### Configure and Build

```bash
# From axis2-c root directory
./configure --enable-http2 --enable-json
make
make install
```

### Cross-Compile for Android

```bash
export NDK=/path/to/android-ndk
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64

./configure \
    --host=aarch64-linux-android \
    --enable-http2 \
    --enable-json \
    CC=$TOOLCHAIN/bin/aarch64-linux-android21-clang \
    CXX=$TOOLCHAIN/bin/aarch64-linux-android21-clang++

make
```

## Testing

### Generate Test Data

```bash
# Generate 500-asset portfolio
python3 test/generate_test_data.py --assets 500 --output portfolio_500.json

# Generate Monte Carlo request
python3 test/generate_test_data.py --monte-carlo --simulations 100000 --output mc.json
```

### Run Test Suite

```bash
# Test against local service
./test/test_benchmark.sh localhost 8443

# Test against Android device
./test/test_benchmark.sh 192.168.1.100 443
```

### Manual Testing with curl

```bash
# Portfolio variance
curl --http2 -k \
    -H "Content-Type: application/json" \
    -d @portfolio_500.json \
    https://192.168.1.100:443/services/FinancialBenchmarkService/portfolioVariance

# Monte Carlo
curl --http2 -k \
    -H "Content-Type: application/json" \
    -d '{"n_simulations":10000,"n_periods":252,"initial_value":1000000,"expected_return":0.08,"volatility":0.2}' \
    https://192.168.1.100:443/services/FinancialBenchmarkService/monteCarlo

# Metadata
curl --http2 -k \
    https://192.168.1.100:443/services/FinancialBenchmarkService/metadata
```

## Performance Comparison

### The Compelling Demo

On a **$20 used Android phone from 2014** (Galaxy S5, 2GB RAM):

```
┌────────────────────────────────────────────────────────────┐
│  Axis2/C Financial Benchmark                               │
│  ────────────────────────────────────────────────────────  │
│  Device: Samsung Galaxy S5 (2014, 2GB RAM)                 │
│  Test: 500-asset portfolio variance                        │
│  ────────────────────────────────────────────────────────  │
│  Result: 250,000 matrix ops in 4.5ms                       │
│  Memory: 28 MB peak                                        │
│  ────────────────────────────────────────────────────────  │
│  Java/WildFly: Cannot run on this device.                  │
│  Enterprise minimum: 16 GB RAM per WildFly server          │
│  Hardware cost: $20 device vs $5,000–$20,000+ server       │
└────────────────────────────────────────────────────────────┘
```

### Benchmark Results Table

| Test | Axis2/C (2GB device) | Java/WildFly 32 (Enterprise production) |
|------|---------------------|----------------------------------------|
| Portfolio 500 assets | ~5ms | N/A — cannot start on device |
| Monte Carlo 10K sims | ~100ms | N/A — cannot start on device |
| Memory footprint | ~30 MB | 16–32 GB (enterprise: 70+ servers configured this way) |
| Startup time | <1s | 30–60s |
| Hardware cost | $20 used phone | Enterprise server ($5,000–$20,000+) |

## Use Cases

1. **Edge Computing**: Run financial calculations on IoT devices
2. **Mobile Trading**: Portfolio analytics on phones without cloud dependency
3. **Embedded Systems**: Risk calculations on constrained hardware
4. **Cost Reduction**: Replace expensive servers with cheap devices

## License

Apache License 2.0 - See LICENSE file in root directory.
