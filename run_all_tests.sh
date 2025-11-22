#!/bin/bash

echo "🎯 Testing Axis2/C Native WSDL2C Generator - Advanced Features"
echo "============================================================="

# Get the script directory and navigate to test directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_DIR="${SCRIPT_DIR}/tools/codegen/native/test"
LOG_FILE="${SCRIPT_DIR}/log.txt"

cd "${TEST_DIR}"

echo "🧹 Cleaning previous build..."
make clean

echo "🔨 Building and running comprehensive tests..."
make check > "${LOG_FILE}" 2>&1

echo ""
echo "📊 RESULTS SUMMARY:"
echo "=================="

if grep -q "Pass Rate:" "${LOG_FILE}"; then
    echo "✅ Tests completed successfully!"
    echo ""

    # Show overall results
    echo "📈 Overall Results:"
    grep -A 10 "Pass Rate:" "${LOG_FILE}" | tail -6
    echo ""

    # Show individual test suite results
    echo "📋 Test Suite Breakdown:"
    grep -B 1 -A 4 "Pass Rate:" "${LOG_FILE}" | head -25

    echo ""
    echo "🎉 Advanced Features Status:"
    if grep -q "test_complex_choice.*PASSED" "${LOG_FILE}"; then
        echo "  ✅ Complex Choice Elements: WORKING"
    else
        echo "  ❌ Complex Choice Elements: NEEDS WORK"
    fi

    if grep -q "test_nested_arrays.*PASSED" "${LOG_FILE}"; then
        echo "  ✅ Nested Arrays: WORKING"
    else
        echo "  ❌ Nested Arrays: NEEDS WORK"
    fi

    if grep -q "test_numeric_enumeration.*PASSED" "${LOG_FILE}"; then
        echo "  ✅ Numeric Enumerations: WORKING"
    else
        echo "  ❌ Numeric Enumerations: NEEDS WORK"
    fi

else
    echo "❌ Tests failed to complete. Check log.txt for errors."
fi

echo ""
echo "📄 Full details: ${LOG_FILE}"