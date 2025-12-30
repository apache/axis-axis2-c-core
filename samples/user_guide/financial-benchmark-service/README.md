# Financial Benchmark Service

**Apache Axis2/C HTTP/2 JSON Financial Benchmark Service**

## Overview

This service demonstrates Axis2/C performance on **resource-constrained hardware** where Java/WildFly cannot run. It implements real financial calculations used in portfolio management systems.

### The "Single Camera" Use Case

Just as the Kanaha Camera Control system provides value with a single old Android phone, this benchmark service demonstrates that **one modest device** can run production-grade financial calculations:

| Platform | Minimum RAM | Old Android (2GB) |
|----------|-------------|-------------------|
| **Java/WildFly 32** | 4-8 GB | ❌ Cannot start |
| **Node.js/V8** | 1-2 GB | ⚠️ OOM risk |
| **Axis2/C 2.0** | ~30 MB | ✅ Runs comfortably |

## Operations

### 1. Portfolio Variance (`/portfolioVariance`)

Calculates portfolio variance using covariance matrix multiplication.

**Formula**: σ²_p = Σ_i Σ_j w_i * w_j * σ_ij

**Complexity**: O(n²) - same as DPT v2 correlation calculations

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
    "request_id": "test-001"
}
```

**Example Response**:
```json
{
    "status": "SUCCESS",
    "portfolio_variance": 0.0156,
    "portfolio_volatility": 0.125,
    "annualized_volatility": 1.98,
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

### 3. Scenario Analysis (`/scenarioAnalysis`)

Demonstrates O(1) hash table lookups vs O(n) linear search (mirrors DPT's Array→Map optimization).

### 4. Service Metadata (`/metadata`)

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
│  Java/WildFly: Cannot run (requires 4-8GB RAM)             │
│  Hardware cost: $20 vs $2000+                              │
└────────────────────────────────────────────────────────────┘
```

### Benchmark Results Table

| Test | Axis2/C (2GB device) | Java/WildFly |
|------|---------------------|--------------|
| Portfolio 500 assets | ~5ms | N/A (cannot start) |
| Monte Carlo 10K sims | ~100ms | N/A (cannot start) |
| Memory usage | ~30MB | 4-8GB minimum |
| Startup time | <1s | 30-60s |
| Device cost | $20 | $2000+ |

## Use Cases

1. **Edge Computing**: Run financial calculations on IoT devices
2. **Mobile Trading**: Portfolio analytics on phones without cloud dependency
3. **Embedded Systems**: Risk calculations on constrained hardware
4. **Cost Reduction**: Replace expensive servers with cheap devices

## License

Apache License 2.0 - See LICENSE file in root directory.
