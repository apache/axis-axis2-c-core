# ADB Test Plan for Axis2/C Native Generator

## Overview

This test plan focuses specifically on ADB (Axis Data Binding) functionality in the Axis2/C native WSDL2C generator. ADB is the core Apache Axis TLP invention for data binding, and the native generator only supports "adb" and "none" databinding modes.

## Test Structure Based on Axis2/Java ADB Tests

The test structure mirrors the comprehensive ADB test suite from Axis2/Java with 90+ test categories, adapted for C implementation.

### Core Test Categories

#### 1. Schema Structure Tests
- **Choice Elements** (`choice/`)
  - Multiple choice selection validation
  - Choice element serialization/deserialization

- **Enumeration** (`enumeration/`)
  - Enum value handling and validation
  - Type-safe enumeration generation

- **Extensions** (`extension/`)
  - Simple type extensions
  - Complex type extensions with inheritance

- **Restrictions** (`restriction/`)
  - Simple type restrictions with facets
  - Complex type restrictions

- **Unions** (`union/`)
  - Union type handling
  - Multiple union member validation

#### 2. Data Type Coverage Tests
- **Simple Types** (`populate/simple/`)
  - boolean, float, double, decimal
  - date, dateTime, time, duration
  - gYear, gYearMonth, gMonth, gMonthDay, gDay
  - base64Binary, hexBinary
  - anyURI, QName
  - All XSD built-in types (35+ variants)

- **Derived Types** (`populate/derived/`)
  - integer, long, int, short, byte
  - unsignedLong, unsignedInt, unsignedShort, unsignedByte
  - positiveInteger, nonPositiveInteger, negativeInteger, nonNegativeInteger

- **Complex Structures** (`populate/other/`)
  - Arrays and nested arrays
  - Recursive structures
  - Anonymous complex types
  - minOccurs=0 handling
  - Mixed content support

#### 3. Advanced Features
- **Namespace Handling** (`nonamespace/`, `defaultnamespaces/`, `unqualified/`)
  - Default namespace processing
  - Unqualified element handling
  - Chameleon schema support

- **Attributes** (`attribute/`)
  - Attribute binding and validation
  - Default and fixed value attributes

- **Groups** (`group/`)
  - Schema group processing
  - Particle group handling

- **References** (`references/`)
  - Element and type references
  - Cross-schema references

#### 4. Edge Cases and Validation
- **Min/Max Occurs** (`particlemaxoccurs/`, minOccurs handling)
  - Cardinality validation
  - Optional element processing

- **Any Types** (`anytype/`)
  - xs:any element handling
  - Dynamic content processing

- **All Groups** (`all/`)
  - xs:all compositor support

- **Inner Particles** (`innerparticles/`, `innerchoice/`)
  - Nested particle structures
  - Inner choice element handling

### Test Implementation Strategy

#### Phase 1: Core ADB Infrastructure
1. **C Test Framework Creation**
   - Equivalent of AbstractTestCase for C
   - Serialization/deserialization validation framework
   - Memory management validation
   - XML comparison utilities

2. **Basic Type Support**
   - Simple XSD types (string, int, float, boolean)
   - Basic serialization round-trip tests
   - Memory leak detection

#### Phase 2: Schema Structure Support
1. **Choice and Enumeration**
   - Choice element generation and handling
   - Enumeration type safety and validation

2. **Complex Types**
   - Extension and restriction support
   - Nested structure handling

#### Phase 3: Advanced Features
1. **Array and Collection Support**
   - Dynamic array handling
   - Nested array structures

2. **Namespace and Attribute Processing**
   - Proper namespace handling in generated code
   - Attribute binding implementation

#### Phase 4: Edge Cases and Validation
1. **Cardinality and Optional Elements**
   - minOccurs=0 support
   - maxOccurs handling

2. **Advanced Schema Features**
   - Union types
   - Any types and dynamic content

### Test WSDLs and Schemas

Based on Axis2/Java structure, create test WSDL files for:

1. **Basic Types Test** - Simple service with all XSD built-in types
2. **Complex Structures Test** - Nested objects, arrays, choices
3. **Enumeration Test** - Type-safe enumerations
4. **Namespace Test** - Multiple namespaces and qualified elements
5. **Regression Tests** - Known issue reproductions and edge cases

### Success Criteria

1. **Functional Parity**: All core ADB features working equivalently to Java implementation
2. **Memory Safety**: No memory leaks in generated code and test framework
3. **XML Compatibility**: Generated XML matches Java ADB output format
4. **Performance**: Reasonable performance compared to Java implementation
5. **Standards Compliance**: Full XSD 1.0 compliance for supported features

### Test Data Sources

Leverage existing test resources:
- Use Calculator.wsdl as baseline test case
- Port key test schemas from Axis2/Java adb-tests module
- Create C-specific test cases for memory management validation

### Build Integration

1. **CMake Integration**: Tests as part of native generator build process
2. **Continuous Validation**: Automated testing with each code change
3. **Coverage Reporting**: Track ADB feature coverage and test completeness
4. **Regression Prevention**: Maintain compatibility with existing generated code

This test plan ensures comprehensive ADB validation while focusing specifically on the databinding technology that Apache Axis TLP invented and maintains.