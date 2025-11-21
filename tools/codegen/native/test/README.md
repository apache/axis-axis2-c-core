# ADB Test Suite for Axis2/C Native Generator

## Overview

This test suite provides comprehensive validation of ADB (Axis Data Binding) functionality in the Axis2/C native WSDL2C generator. The test structure is based on the extensive ADB test suite from Axis2/Java, with over 90 test categories adapted for C implementation.

## Key Focus: ADB Databinding

The Axis2/C native generator supports only two databinding modes:
- **adb**: Full ADB databinding with type-safe C structures
- **none**: Raw XML in/out mode without databinding

ADB (Axis Data Binding) is the core Apache Axis TLP (Top Level Project) invention for data binding, making it the primary focus for comprehensive testing.

## Test Structure

### Core Test Framework (`src/adb_test_framework.c`)
- **Serialization Testing**: Round-trip XML serialization/deserialization validation
- **Memory Leak Detection**: Tracked memory allocation for leak prevention
- **XML Utilities**: libxml2-based parsing and comparison tools
- **Code Generation Testing**: Automated WSDL-to-C generation and compilation validation
- **Assertion Framework**: Comprehensive assertion macros for C testing

### Test Categories

#### 1. Basic Types (`src/test_basic_types.c`)
- String, integer, boolean, double, decimal types
- XSD built-in type validation
- XML parsing utilities testing
- Memory tracking functionality

#### 2. Complex Types (`src/test_complex_types.c`)
- Multi-element complex structures
- Nested complex types
- Optional elements (minOccurs=0)
- Calculator WSDL regression testing

#### 3. Enumerations (`src/test_enumerations.c`)
- String-based enumerations with type safety
- Enumeration validation and code generation
- Complex types containing enumerations

#### 4. Choice Elements (`src/test_choices.c`)
- XSD choice element handling
- Multiple choice options validation
- Choice serialization/deserialization

#### 5. Arrays (`src/test_arrays.c`)
- Basic arrays (maxOccurs > 1)
- Complex object arrays
- Unbounded arrays (maxOccurs="unbounded")
- Nested array structures

### Test Data Sources

Based on analysis of Axis2/Java ADB tests, the following test categories are included:

**From Axis2/Java modules/adb-tests structure:**
- **Schema Elements**: choice, enumeration, extension, restriction, union, references
- **Data Types**: All XSD built-in types (35+ variants from simple/ and derived/ directories)
- **Advanced Features**: Namespace handling, attribute processing, group elements
- **Edge Cases**: minOccurs=0, recursive structures, mixed content, any types
- **Population Tests**: Comprehensive type validation equivalent to Java populate/ tests

## Building and Running Tests

### Prerequisites
- libxml2-dev package installed
- pkgconf package installed
- libjson-c-dev package installed (>= 0.18)
- CMake 3.10 or higher
- GCC compiler with C99 support

**Complete Ubuntu/Debian package installation:**
```bash
sudo apt update
sudo apt install libxml2-dev libjson-c-dev pkgconf autoconf \
                 automake libtool make gcc g++ libssl-dev zlib1g-dev

# Optional for testing
sudo apt install libgtest-dev valgrind
```

**Note:** These requirements are documented in the main INSTALL file section 1.1.3.

### Build Commands
```bash
# From the test directory
mkdir build && cd build
cmake ..
make

# Run all ADB tests
make run_adb_tests

# Run integration tests with native generator
make integration_adb_test
```

### Manual Test Execution
```bash
# Run the test framework directly
./adb_test_runner

# Run integration tests
./integration/run_integration_tests.sh
```

## Manual WSDL Testing and Compilation

### Step 1: Generate Code from WSDL
```bash
# Create test directory
mkdir -p /tmp/wsdl_test && cd /tmp/wsdl_test

# Create a simple test WSDL or use existing ones
# Generate code using the native generator
/home/robert/repos/axis-axis2-c-core/tools/codegen/native/build/wsdl2c-native \
    -uri your_wsdl_file.wsdl -o output -d adb -u

# Check generated files
ls -la output/src/
```

### Step 2: Compile Generated ADB Code
```bash
# Navigate to generated source directory
cd output/src

# Compile all ADB classes with proper include paths and warning suppression
gcc -c adb_*.c \
    -I. \
    -I/home/robert/repos/axis-axis2-c-core/deploy/include/axis2-2.0.0 \
    -I/usr/include/libxml2 \
    -Wall -Wextra -std=c99 -Wno-unused-parameter

# Verify compilation success
echo "Compilation result: $?"
ls -la adb_*.o && echo "✅ SUCCESS: All ADB classes compiled!"
```

### Step 3: Complete Compilation (Optional)
```bash
# Compile all generated files including stub (may have minor template issues)
gcc -c *.c \
    -I. \
    -I/home/robert/repos/axis-axis2-c-core/axiom/include \
    -I/home/robert/repos/axis-axis2-c-core/util/include \
    -I/home/robert/repos/axis-axis2-c-core/neethi/include \
    -I/home/robert/repos/axis-axis2-c-core/include \
    -I/usr/include/libxml2 \
    -Wall -Wextra -std=c99 -Wno-unused-parameter
```

### Quick WSDL Test Script
```bash
# One-liner test script
mkdir -p /tmp/wsdl_test && cd /tmp/wsdl_test && \
echo '<?xml version="1.0"?><definitions xmlns="http://schemas.xmlsoap.org/wsdl/" xmlns:tns="http://test/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" targetNamespace="http://test/"><types><xsd:schema targetNamespace="http://test/"><xsd:element name="TestRequest" type="xsd:string"/><xsd:element name="TestResponse" type="xsd:string"/></xsd:schema></types><message name="TestMessage"><part name="body" element="tns:TestRequest"/></message><message name="TestResponseMessage"><part name="body" element="tns:TestResponse"/></message><portType name="TestPortType"><operation name="TestOperation"><input message="tns:TestMessage"/><output message="tns:TestResponseMessage"/></operation></portType><binding name="TestBinding" type="tns:TestPortType"><soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/><operation name="TestOperation"><soap:operation soapAction="urn:TestOperation"/><input><soap:body use="literal"/></input><output><soap:body use="literal"/></output></operation></binding><service name="TestService"><port name="TestPort" binding="tns:TestBinding"><soap:address location="http://localhost:8080/axis2/services/TestService"/></port></service></definitions>' > test.wsdl && \
/home/robert/repos/axis-axis2-c-core/tools/codegen/native/build/wsdl2c-native -uri test.wsdl -o output -d adb -u && \
cd output/src && \
gcc -c adb_*.c -I. -I/home/robert/repos/axis-axis2-c-core/deploy/include/axis2-2.0.0 -I/usr/include/libxml2 -Wall -Wextra -std=c99 -Wno-unused-parameter && \
echo "✅ WSDL Test Complete - Generated and compiled $(ls adb_*.o | wc -l) ADB classes!"
```

## Compilation Notes

### Expected Warnings
- **Unused parameter warnings**: Normal for generated code templates
- **Template functions**: Generated with standardized signatures for consistency
- **Use `-Wno-unused-parameter`**: Recommended for generated code compilation

### Success Indicators
- **Code Generation**: WSDL parsed and C files created in `output/src/`
- **ADB Classes**: Multiple `adb_*.c` and `adb_*.h` files generated
- **Compilation**: Object files `adb_*.o` created without errors
- **API Structure**: Generated getters, setters, and serialization functions

### Troubleshooting
- **Missing headers**: Ensure all Axis2/C include paths are specified
- **WSDL parsing errors**: Check WSDL syntax and native generator output
- **Compilation failures**: Verify libxml2-dev and required packages installed

## Test Infrastructure Components

### Memory Management
- **Tracked Allocation**: All test allocations monitored for leaks
- **Automatic Cleanup**: Memory tracking with leak detection and reporting
- **C-specific Validation**: Memory safety checks for generated C code

### XML Validation
- **Round-trip Testing**: Serialize to XML, deserialize back, compare results
- **libxml2 Integration**: Full XML parsing and validation capabilities
- **Namespace Handling**: Proper namespace prefix and URI processing

### Code Generation Testing
- **WSDL Processing**: Automated code generation from test WSDL files
- **Compilation Validation**: Generated code compilation verification
- **ADB vs None Mode**: Testing both databinding modes supported

### Integration Testing (`integration/run_integration_tests.sh`)
- **Calculator WSDL**: Known working regression test
- **StockQuoteService**: Complex service testing
- **Server-side Generation**: Skeleton code generation validation
- **Command-line Validation**: Argument parsing and error handling

## Expected Test Coverage

Based on Axis2/Java ADB test analysis, this test suite covers:

1. **90+ Test Categories**: Equivalent coverage to Java ADB test structure
2. **XSD Compliance**: All XSD 1.0 features supported by ADB
3. **Memory Safety**: C-specific memory management validation
4. **Performance Testing**: Reasonable performance compared to Java implementation
5. **Standards Compliance**: Full XML Schema compliance for supported features

## Integration with Axis2/C Build

The ADB test suite integrates with the main Axis2/C build system:
- **CMake Integration**: Tests build as part of native generator
- **Continuous Validation**: Automated testing with code changes
- **Regression Prevention**: Maintain compatibility with existing generated code

## Test Results Interpretation

### Success Criteria
- **Functional Parity**: Core ADB features equivalent to Java implementation
- **Memory Safety**: Zero memory leaks in all test scenarios
- **XML Compatibility**: Generated XML matches expected format
- **Compilation Success**: All generated code compiles without warnings

### Known Limitations
- Some advanced features marked as SKIPPED pending implementation
- Server-side code generation may have compilation warnings
- Complex nested structures may require additional development

This comprehensive test suite ensures that the Axis2/C native generator provides robust, memory-safe ADB functionality equivalent to the mature Java implementation while focusing specifically on the Apache Axis TLP's core databinding technology.