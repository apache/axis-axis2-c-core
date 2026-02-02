# Axis2/C Code Generation (WSDL2C)

The Axis2/C native code generator (WSDL2C) generates C code from WSDL definitions, creating type-safe structures and serialization functions for SOAP web services.

## Overview

The native WSDL2C generator (`tools/codegen/native/`) is a pure C implementation that:

- Parses WSDL 1.1/2.0 definitions
- Generates ADB (Axis Data Binding) C structures
- Creates serialization/deserialization functions
- Produces client stubs and service skeletons
- Requires no Java dependencies

## Quick Start

```bash
# Build the native code generator
cd tools/codegen/native
make

# Generate C code from a WSDL file
./wsdl2c -uri MyService.wsdl -o output_dir -d adb
```

## Databinding Modes

| Mode | Description |
|------|-------------|
| `adb` | Full ADB databinding with type-safe C structures (recommended) |
| `none` | Raw XML in/out mode without databinding |

## Related Documentation

The native code generator has extensive documentation covering architecture, testing, and design decisions:

### Architecture & Design

- **[Code Generation Approaches](https://github.com/apache/axis-axis2-c-core/blob/master/tools/codegen/native/CODEGEN_APPROACHES.md)** - Comparison of Java XSL templates vs native C generator, explaining the evolution and benefits of the current approach

- **[Build System Evaluation](https://github.com/apache/axis-axis2-c-core/blob/master/tools/codegen/native/BUILD_SYSTEM_EVALUATION.md)** - Analysis of CMake vs Autotools for the code generator build system

- **[Nested Arrays Analysis](https://github.com/apache/axis-axis2-c-core/blob/master/tools/codegen/native/NESTED_ARRAYS_ANALYSIS.md)** - Technical analysis and business decision on nested array support (intentionally not implemented due to complexity vs. real-world usage)

### Testing & Validation

- **[ADB Test Suite README](https://github.com/apache/axis-axis2-c-core/blob/master/tools/codegen/native/test/README.md)** - Overview of the comprehensive ADB test framework

- **[ADB Test Plan](https://github.com/apache/axis-axis2-c-core/blob/master/tools/codegen/native/test/adb_test_plan.md)** - Detailed test plan based on Axis2/Java's 90+ test categories

- **[ADB Feature Parity](https://github.com/apache/axis-axis2-c-core/blob/master/tools/codegen/native/test/ADB_FEATURE_PARITY.md)** - Feature comparison matrix between Axis2/Java ADB and Axis2/C native generator

## Historical Context: Java XSL Templates vs Native C Generator

Axis2/C has evolved through two distinct code generation approaches, each addressing different architectural needs and performance requirements. Many JIRA issues reference the original Java-based XSL template system, making it important to understand the relationship between these approaches.

### Java-Based XSL Template Approach (Legacy)

The original Axis2/C code generation relied on Java-based XSL transformations that processed WSDL schemas and generated C code through template expansion. This approach used complex XSL stylesheets (like `CADBBeanTemplateSource.xsl`) to transform XML schema definitions into C data binding code. Issues like AXIS2C-1614, AXIS2C-1617, and others contain patches targeting these XSL templates, which handled concerns such as required attribute validation (`<xsl:if test="not(@optional)">`) and memory management in generated serialization code. While powerful and feature-complete, this approach introduced Java dependencies and complex build requirements for C developers.

### Native C Generator Approach (Current)

The current native WSDL2C generator (`tools/codegen/native/`) represents a ground-up rewrite in pure C, eliminating Java dependencies while providing equivalent functionality. This approach generates ADB (Axis Data Binding) code directly from WSDL parsing, creating type-safe C structures and functions for serialization/deserialization. The native generator addresses the same fundamental issues as the XSL templates but through direct C code generation - for example, AXIS2C-1614's required attribute validation is implemented through `axiom_element_get_attribute_value_by_name()` calls with proper error handling, while AXIS2C-1616's type name conflicts are resolved through reserved word detection. This approach provides better performance, simpler deployment, and more maintainable code generation logic while preserving the robustness and validation capabilities of the original system.

### JIRA Issue Context

When reviewing historical JIRA issues, patches targeting XSL templates indicate problems that have been addressed in the native generator through equivalent C-based solutions. The comprehensive test suites in `tools/codegen/native/test/` validate that the native generator maintains compatibility with expected behaviors while providing the architectural benefits of a pure C implementation.
