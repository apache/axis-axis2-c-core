#!/bin/bash

echo "🎯 Testing Axis2/C Native WSDL2C Generator - Advanced Features"
echo "============================================================="

cd /home/robert/repos/axis-axis2-c-core/tools/codegen/native/test

echo "🧹 Cleaning previous build..."
make clean

echo "🔨 Building and running comprehensive tests..."
make check > /home/robert/repos/axis-axis2-c-core/wtf.txt 2>&1

echo ""
echo "📊 RESULTS SUMMARY:"
echo "=================="

if grep -q "Pass Rate:" /home/robert/repos/axis-axis2-c-core/wtf.txt; then
    echo "✅ Tests completed successfully!"
    echo ""

    # Show overall results
    echo "📈 Overall Results:"
    grep -A 10 "Pass Rate:" /home/robert/repos/axis-axis2-c-core/wtf.txt | tail -6
    echo ""

    # Show individual test suite results
    echo "📋 Test Suite Breakdown:"
    grep -B 1 -A 4 "Pass Rate:" /home/robert/repos/axis-axis2-c-core/wtf.txt | head -25

    echo ""
    echo "🎉 Advanced Features Status:"
    if grep -q "test_complex_choice.*PASSED" /home/robert/repos/axis-axis2-c-core/wtf.txt; then
        echo "  ✅ Complex Choice Elements: WORKING"
    else
        echo "  ❌ Complex Choice Elements: NEEDS WORK"
    fi

    if grep -q "test_nested_arrays.*PASSED" /home/robert/repos/axis-axis2-c-core/wtf.txt; then
        echo "  ✅ Nested Arrays: WORKING"
    else
        echo "  ❌ Nested Arrays: NEEDS WORK"
    fi

    if grep -q "test_numeric_enumeration.*PASSED" /home/robert/repos/axis-axis2-c-core/wtf.txt; then
        echo "  ✅ Numeric Enumerations: WORKING"
    else
        echo "  ❌ Numeric Enumerations: NEEDS WORK"
    fi

else
    echo "❌ Tests failed to complete. Check wtf.txt for errors."
fi

echo ""
echo "📄 Full details: /home/robert/repos/axis-axis2-c-core/wtf.txt"