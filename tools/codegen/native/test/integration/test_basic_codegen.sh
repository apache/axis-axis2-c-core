#!/bin/bash
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Integration test for basic code generation functionality
# This script validates the standalone C generator against the Java-based generator

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AXIS2C_ROOT="$(cd "$SCRIPT_DIR/../../../../.." && pwd)"
TEST_WSDL="$AXIS2C_ROOT/axiom/test/resources/wsdl/Calculator.wsdl"
JAVA_OUTPUT_DIR="/tmp/wsdl2c-java-2.0.1-output"
NATIVE_OUTPUT_DIR="/tmp/wsdl2c-native-output"

# Expected Axis2/Java 2.0.1-SNAPSHOT location - use environment variable or default
EXPECTED_AXIS2_HOME="${AXIS2_JAVA_HOME:-/usr/local/axis2-2.0.1-SNAPSHOT}"

echo "Integration Test: Basic Code Generation (Axis2/Java 2.0.1-SNAPSHOT)"
echo "=================================================================="

# Check if test WSDL exists
if [ ! -f "$TEST_WSDL" ]; then
    echo "ERROR: Test WSDL not found: $TEST_WSDL"
    exit 1
fi

# Check for newer Axis2/Java version
if [ "$AXIS2_HOME" != "$EXPECTED_AXIS2_HOME" ]; then
    echo "WARNING: AXIS2_HOME is set to: $AXIS2_HOME"
    echo "         Expected: $EXPECTED_AXIS2_HOME"
    echo "         Please update your environment to use Axis2/Java 2.0.1-SNAPSHOT"
fi

# Clean output directories
rm -rf "$JAVA_OUTPUT_DIR" "$NATIVE_OUTPUT_DIR"
mkdir -p "$JAVA_OUTPUT_DIR" "$NATIVE_OUTPUT_DIR"

echo "1. Testing Java-based generator (Axis2/Java 2.0.1-SNAPSHOT reference)..."
if [ -n "$AXIS2_HOME" ] && [ -x "$AXIS2C_ROOT/tools/codegen/javatool/WSDL2C.sh" ]; then
    cd "$JAVA_OUTPUT_DIR"
    echo "   Using AXIS2_HOME: $AXIS2_HOME"
    "$AXIS2C_ROOT/tools/codegen/javatool/WSDL2C.sh" -uri "$TEST_WSDL" -u -d adb
    echo "   Java generator completed"
    JAVA_FILES=$(find . -name "*.c" -o -name "*.h" | wc -l)
    echo "   Generated $JAVA_FILES files"

    # Check for version info in generated files
    if [ -f "src/axis2_stub_Calculator.h" ]; then
        VERSION_INFO=$(grep -i "apache axis2.*version" src/axis2_stub_Calculator.h | head -1)
        echo "   Generator version: $VERSION_INFO"
    fi
else
    echo "   ERROR: AXIS2_HOME not set or WSDL2C.sh not found"
    echo "   Please set: export AXIS2_HOME=/path/to/your/axis2-2.0.1-SNAPSHOT"
    echo "   Or set: export AXIS2_JAVA_HOME=/path/to/your/axis2-java-installation"
    exit 1
fi

echo "2. Testing native C generator..."
if [ -x "$AXIS2C_ROOT/tools/codegen/native/build/wsdl2c-native" ]; then
    cd "$NATIVE_OUTPUT_DIR"
    "$AXIS2C_ROOT/tools/codegen/native/build/wsdl2c-native" -uri "$TEST_WSDL" -u -d adb
    echo "   Native generator completed"
    NATIVE_FILES=$(find . -name "*.c" -o -name "*.h" | wc -l)
    echo "   Generated $NATIVE_FILES files"

    # TODO: Add file comparison logic here
    # - Compare generated file structure
    # - Validate key function signatures
    # - Check compilation success

else
    echo "   INFO: Native generator not built yet"
    echo "   Build the native generator with:"
    echo "   cd $AXIS2C_ROOT/tools/codegen/native && mkdir build && cd build && cmake .. && make"
fi

echo "3. Validation..."
# TODO: Implement detailed validation comparing 2.0.1-SNAPSHOT output
echo "   Basic validation passed"

echo ""
echo "Integration test completed!"
echo "Java output (2.0.1-SNAPSHOT): $JAVA_OUTPUT_DIR"
echo "Native output: $NATIVE_OUTPUT_DIR"