#!/bin/bash
# test_autotools_build.sh - Simple autotools build integration test
# Tests that the native generator builds and works in autotools environment
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0

set -e

echo "=== Testing Native WSDL2C Generator Autotools Integration ==="

# Check that we're in the right directory
if [ ! -f "../../../../configure.ac" ]; then
    echo "Error: Must be run from tools/codegen/native/test/integration/"
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../../.." && pwd)"
TEST_DIR="/tmp/axis2c-quick-test-$$"

echo "Root directory: $ROOT_DIR"
echo "Test directory: $TEST_DIR"

# Cleanup function
cleanup() {
    rm -rf "$TEST_DIR" 2>/dev/null || true
}
trap cleanup EXIT

# Create test environment
mkdir -p "$TEST_DIR"
cd "$ROOT_DIR"

echo "1. Testing that native generator builds..."
if ! make -C tools/codegen/native wsdl2c-native; then
    echo "❌ FAIL: Native generator build failed"
    exit 1
fi
echo "✅ PASS: Native generator builds successfully"

echo "2. Testing basic functionality..."
if ! tools/codegen/native/wsdl2c-native --help >/dev/null 2>&1; then
    echo "❌ FAIL: wsdl2c-native --help failed"
    exit 1
fi
echo "✅ PASS: Native generator --help works"

if ! tools/codegen/native/wsdl2c-native --version >/dev/null 2>&1; then
    echo "❌ FAIL: wsdl2c-native --version failed"
    exit 1
fi
echo "✅ PASS: Native generator --version works"

echo "3. Testing WSDL code generation..."
# Create simple test WSDL
cat > "$TEST_DIR/simple.wsdl" << 'EOF'
<?xml version="1.0"?>
<definitions xmlns="http://schemas.xmlsoap.org/wsdl/"
             xmlns:tns="http://test/"
             xmlns:xsd="http://www.w3.org/2001/XMLSchema"
             xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/"
             targetNamespace="http://test/">
  <types>
    <xsd:schema targetNamespace="http://test/">
      <xsd:element name="TestRequest" type="xsd:string"/>
    </xsd:schema>
  </types>
  <message name="TestMessage">
    <part name="body" element="tns:TestRequest"/>
  </message>
  <portType name="TestPortType">
    <operation name="TestOperation">
      <input message="tns:TestMessage"/>
    </operation>
  </portType>
  <binding name="TestBinding" type="tns:TestPortType">
    <soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
    <operation name="TestOperation">
      <soap:operation soapAction="urn:TestOperation"/>
      <input><soap:body use="literal"/></input>
    </operation>
  </binding>
  <service name="TestService">
    <port name="TestPort" binding="tns:TestBinding">
      <soap:address location="http://localhost:8080/axis2/services/TestService"/>
    </port>
  </service>
</definitions>
EOF

# Test code generation
mkdir -p "$TEST_DIR/output"
cd "$TEST_DIR/output"

if ! "$ROOT_DIR/tools/codegen/native/wsdl2c-native" -uri ../simple.wsdl -o . -d adb; then
    echo "❌ FAIL: WSDL code generation failed"
    exit 1
fi

# Check that files were generated
if [ ! -d src ] || [ -z "$(ls src/ 2>/dev/null)" ]; then
    echo "❌ FAIL: No source files generated"
    exit 1
fi

GENERATED_FILES=$(ls src/ | wc -l)
echo "✅ PASS: WSDL code generation successful ($GENERATED_FILES files generated)"

echo ""
echo "🎉 All autotools integration tests passed!"
echo "   Native WSDL2C generator is properly integrated with the build system."