#!/bin/bash
#
# HTTP/2 JSON Security Penetration Tests for Axis2/C 2.0.0
#
# Tests the BigDataH2Service and FinancialBenchmarkService against:
# - JSON parsing attacks (deep nesting, huge strings, malformed input)
# - Memory exhaustion attempts
# - Integer overflow conditions
# - Concurrent request handling
#
# Prerequisites:
# - Services deployed to Apache httpd with mod_h2 enabled
# - curl with HTTP/2 support (check: curl --version | grep HTTP2)
# - Optional: Build with --enable-asan for memory error detection
#
# Usage:
#   ./h2_penetration_test.sh [base_url]
#   ./h2_penetration_test.sh https://localhost:8443
#
# Output:
#   Results saved to /tmp/h2_pentest_<timestamp>/
#

set -e

# Configuration
BASE_URL="${1:-https://localhost:8443}"
BIGDATA_ENDPOINT="${BASE_URL}/axis2/services/BigDataH2Service"
FINANCIAL_ENDPOINT="${BASE_URL}/axis2/services/FinancialBenchmarkService"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
RESULTS_DIR="/tmp/h2_pentest_${TIMESTAMP}"
CURL_OPTS="--http2 -s -w '\nHTTP_CODE:%{http_code}\nTIME:%{time_total}s\n'"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Create results directory
mkdir -p "$RESULTS_DIR"
echo "Results will be saved to: $RESULTS_DIR"

# Logging
log_test() {
    local name="$1"
    local status="$2"
    local details="$3"

    if [ "$status" = "PASS" ]; then
        echo -e "${GREEN}[PASS]${NC} $name"
    elif [ "$status" = "FAIL" ]; then
        echo -e "${RED}[FAIL]${NC} $name - $details"
    elif [ "$status" = "BLOCKED" ]; then
        echo -e "${GREEN}[BLOCKED]${NC} $name - Attack correctly rejected"
    else
        echo -e "${YELLOW}[INFO]${NC} $name - $details"
    fi

    echo "[$(date '+%H:%M:%S')] $status: $name - $details" >> "$RESULTS_DIR/test.log"
}

# Check prerequisites
check_prerequisites() {
    echo "=== Checking Prerequisites ==="

    if ! curl --version | grep -q "HTTP2"; then
        echo "ERROR: curl does not support HTTP/2"
        exit 1
    fi
    log_test "curl HTTP/2 support" "PASS" ""

    # Check if server is reachable
    if ! curl -k -s --http2 -o /dev/null -w "%{http_code}" "${BASE_URL}" 2>/dev/null | grep -q "200\|404\|403"; then
        log_test "Server connectivity" "INFO" "Server may not be running at ${BASE_URL}"
        echo ""
        echo "NOTE: Tests will be generated but may fail if server is not running."
        echo "      Start Apache httpd with mod_h2 and deploy services first."
        echo ""
        return 1
    fi
    log_test "Server connectivity" "PASS" ""
    return 0
}

# =============================================================================
# JSON PARSING ATTACKS
# =============================================================================

test_deeply_nested_json() {
    echo ""
    echo "=== Test: Deeply Nested JSON (DoS via recursion) ==="

    # Generate JSON with 300 levels of nesting (should exceed AXIS2_JSON_MAX_NESTING_DEPTH)
    local depth=300
    local json="{"
    for i in $(seq 1 $depth); do
        json="${json}\"level${i}\":{"
    done
    json="${json}\"data\":\"test\""
    for i in $(seq 1 $depth); do
        json="${json}}"
    done
    json="${json}}"

    echo "$json" > "$RESULTS_DIR/nested_json_payload.json"

    local response=$(curl -k --http2 -s -X POST \
        -H "Content-Type: application/json" \
        -d "$json" \
        -w "\nHTTP_CODE:%{http_code}" \
        "${BIGDATA_ENDPOINT}/processData" 2>&1)

    echo "$response" > "$RESULTS_DIR/test_nested_json_response.txt"

    local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)

    if [ "$http_code" = "400" ] || [ "$http_code" = "413" ] || [ "$http_code" = "500" ]; then
        log_test "Deeply nested JSON (${depth} levels)" "BLOCKED" "HTTP $http_code - Server rejected excessive nesting"
    elif [ "$http_code" = "200" ]; then
        log_test "Deeply nested JSON (${depth} levels)" "FAIL" "HTTP $http_code - Server accepted dangerous input"
    else
        log_test "Deeply nested JSON (${depth} levels)" "INFO" "HTTP $http_code"
    fi
}

test_huge_string_value() {
    echo ""
    echo "=== Test: Huge String Value (Memory exhaustion) ==="

    # Generate 10MB string
    local size_mb=10
    local huge_string=$(python3 -c "print('A' * (${size_mb} * 1024 * 1024))" 2>/dev/null || \
                        head -c $((size_mb * 1024 * 1024)) /dev/zero | tr '\0' 'A')

    local json="{\"datasetId\":\"${huge_string}\"}"

    echo "Payload size: $(echo -n "$json" | wc -c) bytes"

    local response=$(curl -k --http2 -s -X POST \
        -H "Content-Type: application/json" \
        -d "$json" \
        -w "\nHTTP_CODE:%{http_code}\nTIME:%{time_total}s" \
        --max-time 30 \
        "${BIGDATA_ENDPOINT}/processData" 2>&1)

    echo "$response" | tail -5 > "$RESULTS_DIR/test_huge_string_response.txt"

    local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)
    local time=$(echo "$response" | grep "TIME:" | cut -d: -f2)

    if [ "$http_code" = "413" ] || [ "$http_code" = "400" ]; then
        log_test "Huge string value (${size_mb}MB)" "BLOCKED" "HTTP $http_code - Server rejected large payload"
    elif [ "$http_code" = "200" ]; then
        log_test "Huge string value (${size_mb}MB)" "INFO" "HTTP $http_code in ${time} - Server processed (check ASAN output)"
    else
        log_test "Huge string value (${size_mb}MB)" "INFO" "HTTP $http_code in ${time}"
    fi
}

test_malformed_json() {
    echo ""
    echo "=== Test: Malformed JSON Variants ==="

    local -a payloads=(
        '{"unclosed": "string'
        '{"missing": value}'
        '{key: "unquoted"}'
        '{"trailing": "comma",}'
        '{"duplicate": 1, "duplicate": 2}'
        '["unclosed array"'
        '{"unicode": "\uD800"}' # Invalid surrogate
        '{"null_byte": "test\x00evil"}'
    )

    local -a names=(
        "Unclosed string"
        "Missing value quotes"
        "Unquoted key"
        "Trailing comma"
        "Duplicate keys"
        "Unclosed array"
        "Invalid unicode surrogate"
        "Embedded null byte"
    )

    for i in "${!payloads[@]}"; do
        local response=$(curl -k --http2 -s -X POST \
            -H "Content-Type: application/json" \
            -d "${payloads[$i]}" \
            -w "\nHTTP_CODE:%{http_code}" \
            "${BIGDATA_ENDPOINT}/processData" 2>&1)

        local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)

        if [ "$http_code" = "400" ]; then
            log_test "Malformed JSON: ${names[$i]}" "BLOCKED" "HTTP 400"
        else
            log_test "Malformed JSON: ${names[$i]}" "INFO" "HTTP $http_code"
        fi
    done
}

# =============================================================================
# INTEGER OVERFLOW ATTACKS
# =============================================================================

test_integer_overflow() {
    echo ""
    echo "=== Test: Integer Overflow Conditions ==="

    local -a values=(
        "9223372036854775807"   # INT64_MAX
        "-9223372036854775808"  # INT64_MIN
        "9223372036854775808"   # INT64_MAX + 1
        "18446744073709551615"  # UINT64_MAX
        "99999999999999999999999999999999"  # Way too big
        "-1"                    # Negative size
        "0"                     # Zero size
    )

    local -a names=(
        "INT64_MAX"
        "INT64_MIN"
        "INT64_MAX+1 overflow"
        "UINT64_MAX"
        "Enormous number"
        "Negative size"
        "Zero size"
    )

    for i in "${!values[@]}"; do
        local json="{\"datasetSize\": ${values[$i]}}"

        local response=$(curl -k --http2 -s -X POST \
            -H "Content-Type: application/json" \
            -d "$json" \
            -w "\nHTTP_CODE:%{http_code}" \
            --max-time 10 \
            "${BIGDATA_ENDPOINT}/processData" 2>&1)

        echo "$response" > "$RESULTS_DIR/test_integer_${i}_response.txt"

        local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)

        if [ "$http_code" = "400" ]; then
            log_test "Integer: ${names[$i]}" "BLOCKED" "HTTP 400 - Invalid value rejected"
        elif [ "$http_code" = "200" ]; then
            log_test "Integer: ${names[$i]}" "INFO" "HTTP 200 - Check ASAN for overflow"
        else
            log_test "Integer: ${names[$i]}" "INFO" "HTTP $http_code"
        fi
    done
}

# =============================================================================
# CONCURRENT REQUEST ATTACKS
# =============================================================================

test_concurrent_requests() {
    echo ""
    echo "=== Test: Concurrent Request Handling (Race Conditions) ==="

    local num_requests=50
    local json='{"datasetId":"concurrent_test","datasetSize":1024,"analyticsType":"standard"}'

    echo "Sending $num_requests concurrent requests..."

    local pids=()
    for i in $(seq 1 $num_requests); do
        curl -k --http2 -s -X POST \
            -H "Content-Type: application/json" \
            -d "$json" \
            -o "$RESULTS_DIR/concurrent_${i}.txt" \
            -w "%{http_code}" \
            "${BIGDATA_ENDPOINT}/processData" &
        pids+=($!)
    done

    # Wait for all requests
    local success=0
    local failed=0
    for pid in "${pids[@]}"; do
        if wait $pid; then
            ((success++))
        else
            ((failed++))
        fi
    done

    log_test "Concurrent requests ($num_requests)" "INFO" "Success: $success, Failed: $failed"

    # Check for any crashes or ASAN errors in server logs
    echo "Check server logs and ASAN output for race condition indicators"
}

# =============================================================================
# FINANCIAL SERVICE SPECIFIC TESTS
# =============================================================================

test_monte_carlo_dos() {
    echo ""
    echo "=== Test: Monte Carlo Computation DoS ==="

    # Request extremely large number of simulations
    local json='{
        "portfolioId": "test",
        "simulationCount": 999999999,
        "confidenceLevel": 0.99,
        "timeHorizon": 252
    }'

    local response=$(curl -k --http2 -s -X POST \
        -H "Content-Type: application/json" \
        -d "$json" \
        -w "\nHTTP_CODE:%{http_code}\nTIME:%{time_total}s" \
        --max-time 30 \
        "${FINANCIAL_ENDPOINT}/calculateVaR" 2>&1)

    local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)
    local time=$(echo "$response" | grep "TIME:" | cut -d: -f2)

    if [ "$http_code" = "400" ] || [ "$http_code" = "413" ]; then
        log_test "Monte Carlo DoS (huge simulation count)" "BLOCKED" "HTTP $http_code"
    else
        log_test "Monte Carlo DoS (huge simulation count)" "INFO" "HTTP $http_code in ${time}"
    fi
}

test_portfolio_size_overflow() {
    echo ""
    echo "=== Test: Portfolio Size Overflow ==="

    # Generate array with many assets
    local assets=""
    for i in $(seq 1 1000); do
        [ -n "$assets" ] && assets="${assets},"
        assets="${assets}{\"symbol\":\"ASSET${i}\",\"weight\":0.001,\"volatility\":0.2}"
    done

    local json="{\"portfolioId\":\"overflow_test\",\"assets\":[${assets}]}"

    echo "Payload size: $(echo -n "$json" | wc -c) bytes"

    local response=$(curl -k --http2 -s -X POST \
        -H "Content-Type: application/json" \
        -d "$json" \
        -w "\nHTTP_CODE:%{http_code}\nTIME:%{time_total}s" \
        --max-time 30 \
        "${FINANCIAL_ENDPOINT}/calculateVariance" 2>&1)

    echo "$response" | tail -5 > "$RESULTS_DIR/test_portfolio_overflow_response.txt"

    local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)
    local time=$(echo "$response" | grep "TIME:" | cut -d: -f2)

    log_test "Portfolio with 1000 assets" "INFO" "HTTP $http_code in ${time} - Check ASAN for O(n²) memory issues"
}

# =============================================================================
# HTTP/2 SPECIFIC ATTACKS
# =============================================================================

test_http2_header_overflow() {
    echo ""
    echo "=== Test: HTTP/2 Header Overflow ==="

    # Create many custom headers
    local headers=""
    for i in $(seq 1 100); do
        headers="${headers} -H 'X-Custom-Header-${i}: $(head -c 1000 /dev/urandom | base64 | head -c 500)'"
    done

    local response=$(eval "curl -k --http2 -s -X POST \
        -H 'Content-Type: application/json' \
        $headers \
        -d '{\"test\":true}' \
        -w '\nHTTP_CODE:%{http_code}' \
        --max-time 10 \
        '${BIGDATA_ENDPOINT}/processData'" 2>&1)

    local http_code=$(echo "$response" | grep "HTTP_CODE:" | cut -d: -f2)

    if [ "$http_code" = "431" ] || [ "$http_code" = "400" ]; then
        log_test "HTTP/2 header overflow (100 large headers)" "BLOCKED" "HTTP $http_code"
    else
        log_test "HTTP/2 header overflow (100 large headers)" "INFO" "HTTP $http_code"
    fi
}

# =============================================================================
# MAIN EXECUTION
# =============================================================================

main() {
    echo "=============================================="
    echo "HTTP/2 JSON Security Penetration Tests"
    echo "Axis2/C 2.0.0 Security Hardening Validation"
    echo "=============================================="
    echo ""
    echo "Target: $BASE_URL"
    echo "Time: $(date)"
    echo ""

    local server_available=true
    check_prerequisites || server_available=false

    if [ "$server_available" = "false" ]; then
        echo ""
        echo "Server not available. Generating test payloads only."
        echo "Payloads saved to: $RESULTS_DIR/"
        echo ""
        echo "To run tests:"
        echo "  1. Start Apache httpd with mod_h2"
        echo "  2. Deploy BigDataH2Service and FinancialBenchmarkService"
        echo "  3. Re-run: $0 $BASE_URL"
        echo ""

        # Still generate some payloads for manual testing
        test_deeply_nested_json 2>/dev/null || true
    else
        # Run all tests
        test_deeply_nested_json
        test_huge_string_value
        test_malformed_json
        test_integer_overflow
        test_concurrent_requests
        test_monte_carlo_dos
        test_portfolio_size_overflow
        test_http2_header_overflow
    fi

    echo ""
    echo "=============================================="
    echo "Test Summary"
    echo "=============================================="
    echo ""
    echo "Results saved to: $RESULTS_DIR/"
    echo "Test log: $RESULTS_DIR/test.log"
    echo ""

    if [ -f "$RESULTS_DIR/test.log" ]; then
        echo "Summary:"
        grep -c "PASS\|BLOCKED" "$RESULTS_DIR/test.log" 2>/dev/null && echo " tests passed/blocked"
        grep -c "FAIL" "$RESULTS_DIR/test.log" 2>/dev/null && echo " tests failed" || echo "0 tests failed"
    fi

    echo ""
    echo "IMPORTANT: Check ASAN output on the server for memory errors!"
    echo "  - Look for: heap-buffer-overflow, use-after-free, double-free"
    echo "  - Server logs: /var/log/apache2/error.log or similar"
    echo ""
}

main "$@"
