#!/bin/bash
# test_autotools_integration.sh - Comprehensive autotools integration test
# Tests the native WSDL2C generator build system integration
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e  # Exit on any error

# Test configuration
TEST_DIR="/tmp/axis2c-autotools-test-$$"
INSTALL_DIR="/tmp/axis2c-test-install-$$"
ORIGINAL_DIR="$(pwd)"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[PASS]${NC} $1"
    TESTS_PASSED=$((TESTS_PASSED + 1))
}

log_error() {
    echo -e "${RED}[FAIL]${NC} $1"
    TESTS_FAILED=$((TESTS_FAILED + 1))
}

log_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

# Test tracking
run_test() {
    local test_name="$1"
    shift
    TESTS_RUN=$((TESTS_RUN + 1))
    log_info "Running test: $test_name"

    if "$@"; then
        log_success "$test_name"
        return 0
    else
        log_error "$test_name"
        return 1
    fi
}

# Cleanup function
cleanup() {
    log_info "Cleaning up test environment..."
    cd "$ORIGINAL_DIR"
    rm -rf "$TEST_DIR" "$INSTALL_DIR" 2>/dev/null || true
}

# Set up cleanup trap
trap cleanup EXIT INT TERM

# Test functions
test_prerequisites() {
    log_info "Checking prerequisites..."

    # Check for required tools
    local missing_tools=()
    for tool in autoconf automake libtool make gcc pkg-config; do
        if ! command -v "$tool" >/dev/null 2>&1; then
            missing_tools+=("$tool")
        fi
    done

    if [ ${#missing_tools[@]} -gt 0 ]; then
        log_error "Missing required tools: ${missing_tools[*]}"
        return 1
    fi

    # Check for libxml2
    if ! pkg-config --exists libxml-2.0; then
        log_error "libxml2 development package not found"
        return 1
    fi

    log_success "All prerequisites available"
    return 0
}

test_makefile_am_files_exist() {
    log_info "Checking Makefile.am files exist..."

    local required_files=(
        "tools/codegen/native/Makefile.am"
        "tools/codegen/native/test/Makefile.am"
    )

    for file in "${required_files[@]}"; do
        if [ ! -f "$file" ]; then
            log_error "Required file missing: $file"
            return 1
        fi
    done

    log_success "All required Makefile.am files exist"
    return 0
}

test_configure_ac_integration() {
    log_info "Checking configure.ac integration..."

    # Check for native-codegen option
    if ! grep -q "enable-native-codegen" configure.ac; then
        log_error "native-codegen option not found in configure.ac"
        return 1
    fi

    # Check for LIBXML2 check
    if ! grep -q "PKG_CHECK_MODULES.*LIBXML2" configure.ac; then
        log_error "LIBXML2 pkg-config check not found in configure.ac"
        return 1
    fi

    # Check for Makefile generation
    if ! grep -q "tools/codegen/native/Makefile" configure.ac; then
        log_error "Native generator Makefile not in AC_CONFIG_FILES"
        return 1
    fi

    log_success "configure.ac properly integrated"
    return 0
}

test_autogen_regeneration() {
    log_info "Testing autogen.sh regeneration..."

    # Create test directory and copy source
    mkdir -p "$TEST_DIR"
    cp -r . "$TEST_DIR/" 2>/dev/null || true
    cd "$TEST_DIR"

    # Clean any existing generated files
    make distclean 2>/dev/null || true
    rm -f configure Makefile.in tools/codegen/native/Makefile.in

    # Run autogen.sh
    if ! ./autogen.sh; then
        log_error "autogen.sh failed"
        return 1
    fi

    # Check that configure was generated
    if [ ! -f configure ]; then
        log_error "configure script not generated"
        return 1
    fi

    # Check that native generator Makefile.in was generated
    if [ ! -f tools/codegen/native/Makefile.in ]; then
        log_error "tools/codegen/native/Makefile.in not generated"
        return 1
    fi

    log_success "autogen.sh regeneration successful"
    return 0
}

test_configure_with_native_codegen() {
    log_info "Testing configure --enable-native-codegen..."

    cd "$TEST_DIR"

    # Configure with native codegen enabled
    if ! ./configure --prefix="$INSTALL_DIR" --enable-native-codegen --disable-tests --enable-json=no; then
        log_error "configure --enable-native-codegen failed"
        return 1
    fi

    # Check that native generator Makefile was created
    if [ ! -f tools/codegen/native/Makefile ]; then
        log_error "tools/codegen/native/Makefile not created"
        return 1
    fi

    # Check configure output mentioned native codegen
    if ! grep -q "native WSDL2C generator.*yes" config.log; then
        log_warning "configure output doesn't clearly show native codegen enabled"
    fi

    log_success "configure --enable-native-codegen successful"
    return 0
}

test_configure_disable_native_codegen() {
    log_info "Testing configure --disable-native-codegen..."

    cd "$TEST_DIR"

    # Clean previous configure
    make distclean 2>/dev/null || true

    # Configure with native codegen disabled
    if ! ./configure --prefix="$INSTALL_DIR" --disable-native-codegen --disable-tests --enable-json=no; then
        log_error "configure --disable-native-codegen failed"
        return 1
    fi

    # Native generator Makefile should still be created but won't build anything
    if [ ! -f tools/codegen/native/Makefile ]; then
        log_error "tools/codegen/native/Makefile not created (should exist but be conditional)"
        return 1
    fi

    log_success "configure --disable-native-codegen successful"
    return 0
}

test_make_native_generator() {
    log_info "Testing make of native generator..."

    cd "$TEST_DIR"

    # Reconfigure with native codegen enabled
    make distclean 2>/dev/null || true
    ./configure --prefix="$INSTALL_DIR" --enable-native-codegen --disable-tests --enable-json=no

    # Build just the native generator
    if ! make -C tools/codegen/native; then
        log_error "make -C tools/codegen/native failed"
        return 1
    fi

    # Check that binary was created
    if [ ! -f tools/codegen/native/wsdl2c-native ]; then
        log_error "wsdl2c-native binary not created"
        return 1
    fi

    # Check that binary is executable
    if [ ! -x tools/codegen/native/wsdl2c-native ]; then
        log_error "wsdl2c-native binary not executable"
        return 1
    fi

    log_success "Native generator build successful"
    return 0
}

test_native_generator_functionality() {
    log_info "Testing native generator basic functionality..."

    cd "$TEST_DIR"

    # Test --help option
    if ! ./tools/codegen/native/wsdl2c-native --help >/dev/null 2>&1; then
        log_error "wsdl2c-native --help failed"
        return 1
    fi

    # Test --version option
    if ! ./tools/codegen/native/wsdl2c-native --version >/dev/null 2>&1; then
        log_error "wsdl2c-native --version failed"
        return 1
    fi

    log_success "Native generator basic functionality works"
    return 0
}

test_make_install() {
    log_info "Testing make install..."

    cd "$TEST_DIR"

    # Install everything
    if ! make install; then
        log_error "make install failed"
        return 1
    fi

    # Check that native generator was installed
    if [ ! -f "$INSTALL_DIR/bin/wsdl2c-native" ]; then
        log_error "wsdl2c-native not installed to bin directory"
        return 1
    fi

    # Check that installed binary works
    if ! "$INSTALL_DIR/bin/wsdl2c-native" --help >/dev/null 2>&1; then
        log_error "Installed wsdl2c-native --help failed"
        return 1
    fi

    log_success "make install successful"
    return 0
}

test_wsdl_code_generation() {
    log_info "Testing WSDL code generation..."

    # Create a simple test WSDL
    cat > "$TEST_DIR/test.wsdl" << 'EOF'
<?xml version="1.0"?>
<definitions xmlns="http://schemas.xmlsoap.org/wsdl/"
             xmlns:tns="http://test/"
             xmlns:xsd="http://www.w3.org/2001/XMLSchema"
             xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/"
             targetNamespace="http://test/">
  <types>
    <xsd:schema targetNamespace="http://test/">
      <xsd:element name="TestRequest" type="xsd:string"/>
      <xsd:element name="TestResponse" type="xsd:string"/>
    </xsd:schema>
  </types>
  <message name="TestMessage">
    <part name="body" element="tns:TestRequest"/>
  </message>
  <message name="TestResponseMessage">
    <part name="body" element="tns:TestResponse"/>
  </message>
  <portType name="TestPortType">
    <operation name="TestOperation">
      <input message="tns:TestMessage"/>
      <output message="tns:TestResponseMessage"/>
    </operation>
  </portType>
  <binding name="TestBinding" type="tns:TestPortType">
    <soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
    <operation name="TestOperation">
      <soap:operation soapAction="urn:TestOperation"/>
      <input><soap:body use="literal"/></input>
      <output><soap:body use="literal"/></output>
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
    local codegen_dir="$TEST_DIR/codegen-output"
    mkdir -p "$codegen_dir"
    cd "$codegen_dir"

    if ! "$INSTALL_DIR/bin/wsdl2c-native" -uri "$TEST_DIR/test.wsdl" -o . -d adb; then
        log_error "WSDL code generation failed"
        return 1
    fi

    # Check that source files were generated
    if [ ! -d src ] || [ -z "$(ls src/ 2>/dev/null)" ]; then
        log_error "No source files generated"
        return 1
    fi

    # Check for ADB files
    local adb_files=$(find src/ -name "adb_*.c" 2>/dev/null | wc -l)
    if [ "$adb_files" -eq 0 ]; then
        log_error "No ADB source files generated"
        return 1
    fi

    log_success "WSDL code generation successful (generated $adb_files ADB files)"
    return 0
}

test_dependency_checking() {
    log_info "Testing dependency checking..."

    cd "$TEST_DIR"

    # Check that ldd shows libxml2 dependency
    local deps=$(ldd tools/codegen/native/wsdl2c-native 2>/dev/null | grep -c "libxml2" || true)
    if [ "$deps" -eq 0 ]; then
        log_warning "libxml2 dependency not visible in ldd output"
    else
        log_success "libxml2 dependency confirmed"
    fi

    return 0
}

# Main test execution
main() {
    echo "=================================="
    echo "Apache Axis2/C Autotools Integration Test Suite"
    echo "Testing native WSDL2C generator build system"
    echo "=================================="
    echo

    # Run all tests
    run_test "Prerequisites Check" test_prerequisites
    run_test "Makefile.am Files Exist" test_makefile_am_files_exist
    run_test "configure.ac Integration" test_configure_ac_integration
    run_test "autogen.sh Regeneration" test_autogen_regeneration
    run_test "Configure Enable Native Codegen" test_configure_with_native_codegen
    run_test "Configure Disable Native Codegen" test_configure_disable_native_codegen
    run_test "Make Native Generator" test_make_native_generator
    run_test "Native Generator Functionality" test_native_generator_functionality
    run_test "Make Install" test_make_install
    run_test "WSDL Code Generation" test_wsdl_code_generation
    run_test "Dependency Checking" test_dependency_checking

    # Results summary
    echo
    echo "=================================="
    echo "Test Results Summary"
    echo "=================================="
    echo "Tests Run:    $TESTS_RUN"
    echo -e "Tests Passed: ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Tests Failed: ${RED}$TESTS_FAILED${NC}"
    echo

    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}🎉 All tests passed! Autotools integration is working correctly.${NC}"
        exit 0
    else
        echo -e "${RED}❌ Some tests failed. Please review the output above.${NC}"
        exit 1
    fi
}

# Check if script is being sourced or executed
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi