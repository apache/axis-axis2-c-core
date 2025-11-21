# Axis2/C Code Generation Approaches

## Historical Context: Java XSL Templates vs Native C Generator

Axis2/C has evolved through two distinct code generation approaches, each addressing different architectural needs and performance requirements. Many JIRA issues reference the original Java-based XSL template system, making it important to understand the relationship between these approaches.

## Java-Based XSL Template Approach (Legacy)

The original Axis2/C code generation relied on Java-based XSL transformations that processed WSDL schemas and generated C code through template expansion. This approach used complex XSL stylesheets (like `CADBBeanTemplateSource.xsl`) to transform XML schema definitions into C data binding code. Issues like AXIS2C-1614, AXIS2C-1617, and others contain patches targeting these XSL templates, which handled concerns such as required attribute validation (`<xsl:if test="not(@optional)">`) and memory management in generated serialization code. While powerful and feature-complete, this approach introduced Java dependencies and complex build requirements for C developers.

## Native C Generator Approach (Current)

The current native WSDL2C generator (`tools/codegen/native/`) represents a ground-up rewrite in pure C, eliminating Java dependencies while providing equivalent functionality. This approach generates ADB (Axis Data Binding) code directly from WSDL parsing, creating type-safe C structures and functions for serialization/deserialization. The native generator addresses the same fundamental issues as the XSL templates but through direct C code generation - for example, AXIS2C-1614's required attribute validation is implemented through `axiom_element_get_attribute_value_by_name()` calls with proper error handling, while AXIS2C-1616's type name conflicts are resolved through reserved word detection. This approach provides better performance, simpler deployment, and more maintainable code generation logic while preserving the robustness and validation capabilities of the original system.

## JIRA Issue Context

When reviewing historical JIRA issues, patches targeting XSL templates indicate problems that have been addressed in the native generator through equivalent C-based solutions. The comprehensive test suites in `tools/codegen/native/test/` validate that the native generator maintains compatibility with expected behaviors while providing the architectural benefits of a pure C implementation.
