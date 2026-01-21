/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WSDL2C_NATIVE_H
#define WSDL2C_NATIVE_H

/**
 * @file wsdl2c_native.h
 * @brief Native C implementation of WSDL2C code generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Axis2 compatibility macros (for standalone build) */
#ifndef AXIS2_EXTERN
#define AXIS2_EXTERN extern
#endif

#ifndef AXIS2_CALL
#define AXIS2_CALL
#endif

/* Basic Axis2 types and constants */
typedef int axis2_status_t;
typedef int axis2_bool_t;
typedef char axis2_char_t;

#ifndef AXIS2_SUCCESS
#define AXIS2_SUCCESS 0
#endif

#ifndef AXIS2_FAILURE
#define AXIS2_FAILURE -1
#endif

#ifndef AXIS2_TRUE
#define AXIS2_TRUE 1
#endif

#ifndef AXIS2_FALSE
#define AXIS2_FALSE 0
#endif

/* Error codes for standalone build */
#ifndef AXIS2_ERROR_NO_MEMORY
#define AXIS2_ERROR_NO_MEMORY 1001
#endif

#ifndef AXIS2_ERROR_INVALID_WSDL
#define AXIS2_ERROR_INVALID_WSDL 1002
#endif

/* AXIS2C-1579 FIX: XSI namespace constants for proper xsi:type support
 * The xsi:type attribute is used for type polymorphism in XML Schema.
 * xmlns:xsi MUST be set to the XML Schema Instance namespace, NOT the target namespace.
 */
#define AXIS2_XSI_NAMESPACE_URI "http://www.w3.org/2001/XMLSchema-instance"
#define AXIS2_XSI_NAMESPACE_PREFIX "xsi"
#define AXIS2_XSD_NAMESPACE_URI "http://www.w3.org/2001/XMLSchema"
#define AXIS2_XSD_NAMESPACE_PREFIX "xsd"

/* AXIS2C-1575 FIX: QName property type support
 * When handling axutil_qname_t* properties, variables for URI, prefix, and localpart
 * MUST be declared at function scope, NOT inside conditional blocks.
 * This prevents undefined variable errors in generated code.
 *
 * Correct pattern (variables at function scope):
 *   axis2_char_t *qname_uri = NULL;
 *   axis2_char_t *qname_prefix = NULL;
 *   axis2_char_t *qname_localpart = NULL;
 *   if (property_qname) {
 *       qname_uri = axutil_qname_get_uri(property_qname, env);
 *       qname_prefix = axutil_qname_get_prefix(property_qname, env);
 *       qname_localpart = axutil_qname_get_localpart(property_qname, env);
 *   }
 *
 * Wrong pattern (variables inside conditional - causes AXIS2C-1575):
 *   if (is_complex_type) {
 *       axis2_char_t *qname_uri = NULL;  // Only visible inside this block!
 *       axis2_char_t *qname_prefix = NULL;
 *   }
 *   // ERROR: qname_uri and qname_prefix undefined here!
 */

/* Property type enumeration for code generation */
typedef enum {
    WSDL2C_PROPERTY_TYPE_INT,
    WSDL2C_PROPERTY_TYPE_STRING,
    WSDL2C_PROPERTY_TYPE_DOUBLE,
    WSDL2C_PROPERTY_TYPE_BOOL,
    WSDL2C_PROPERTY_TYPE_QNAME,      /* axutil_qname_t* - AXIS2C-1575 */
    WSDL2C_PROPERTY_TYPE_DATETIME,
    WSDL2C_PROPERTY_TYPE_BASE64,
    WSDL2C_PROPERTY_TYPE_COMPLEX
} wsdl2c_property_type_t;

/* Environment structure - simplified for standalone use */
typedef struct axutil_allocator {
    void *(*malloc_fn)(void *allocator, size_t size);
    void *(*realloc_fn)(void *allocator, void *ptr, size_t size);
    void (*free_fn)(void *allocator, void *ptr);
} axutil_allocator_t;

typedef struct axutil_error {
    int error_number;
    int status_code;
} axutil_error_t;

typedef struct axutil_log {
    int level;
} axutil_log_t;

typedef struct axutil_env {
    axutil_allocator_t *allocator;
    axutil_error_t *error;
    axutil_log_t *log;
} axutil_env_t;

/* Forward declarations */
struct wsdl2c_context;
struct wsdl2c_options;
struct wsdl2c_wsdl;
struct wsdl2c_schema_registry;

/* Simple array list implementation for standalone mode */
#ifdef STANDALONE_BUILD
typedef struct axutil_array_list {
    void **data;
    int size;
    int capacity;
} axutil_array_list_t;

/* Array list function declarations */
axutil_array_list_t* axutil_array_list_create(const axutil_env_t *env, int initial_capacity);
axis2_status_t axutil_array_list_add(axutil_array_list_t *list, const axutil_env_t *env, void *element);
void axutil_array_list_free(axutil_array_list_t *list, const axutil_env_t *env);
int axutil_array_list_size(axutil_array_list_t *list, const axutil_env_t *env);
void* axutil_array_list_get(axutil_array_list_t *list, const axutil_env_t *env, int index);
#endif

/**
 * @brief Command line options structure
 *
 * AXIS2C-1330 FIX: Added prefix option to prevent name clashes when using
 * multiple services that have identical named types. The prefix is prepended
 * to all generated filenames, type names, and function names.
 */
typedef struct wsdl2c_options {
    char *wsdl_uri;              /**< WSDL URI or file path */
    char *output_dir;            /**< Output directory */
    char *package_name;          /**< Package name */
    char *language;              /**< Target language (always "c") */
    char *databinding;           /**< Data binding type (adb, none) */
    char *prefix;                /**< Prefix for generated names (AXIS2C-1330) */
    axis2_bool_t server_side;    /**< Generate server side code */
    axis2_bool_t service_descriptor; /**< Generate service descriptor */
    axis2_bool_t generate_test;  /**< Generate test cases */
    axis2_bool_t unwrap;         /**< Unwrap parameters */
    axis2_bool_t generate_all;   /**< Generate all classes */
    axis2_bool_t async_only;     /**< Generate async code only */
    axis2_bool_t sync_only;      /**< Generate sync code only */
} wsdl2c_options_t;

/**
 * @brief Schema element structure (parsed from XSD)
 * Contains element information including type and 'any' type flag.
 * AXIS2C-1580: is_any_type flag indicates xsd:any elements that may have NULL qname
 * AXIS2C-1573: Stores both original XML name and sanitized C identifier
 * AXIS2C-1421: is_typeless flag indicates elements without explicit type attribute
 */
typedef struct wsdl2c_schema_element {
    axis2_char_t *name;             /**< Element name (original XML name for serialization) */
    axis2_char_t *c_name;           /**< Sanitized C identifier (AXIS2C-1573) */
    axis2_char_t *type;             /**< Element type (xsd:string, etc.) */
    axis2_char_t *c_type;           /**< Sanitized C type identifier (AXIS2C-1573) */
    axis2_char_t *namespace_uri;    /**< Namespace URI */
    axis2_bool_t is_any_type;       /**< True if xsd:any element (AXIS2C-1580) */
    axis2_bool_t is_typeless;       /**< True if no type attribute (AXIS2C-1421) */
    axis2_bool_t is_nillable;       /**< True if nillable="true" */
    axis2_bool_t name_was_sanitized; /**< True if name required sanitization (AXIS2C-1573) */
    int min_occurs;                 /**< minOccurs value (0 = optional) */
    int max_occurs;                 /**< maxOccurs value (-1 = unbounded) */
} wsdl2c_schema_element_t;

/**
 * @brief Complex type structure (parsed from XSD schema)
 * AXIS2C-1224: Supports complexTypes including those with empty sequences.
 * An empty sequence (no elements) generates a struct with no properties.
 */
typedef struct wsdl2c_complex_type {
    axis2_char_t *name;             /**< Type name (original XML name) */
    axis2_char_t *c_name;           /**< Sanitized C identifier */
    axutil_array_list_t *elements;  /**< Child elements (wsdl2c_schema_element_t*) */
    axis2_bool_t is_empty_sequence; /**< True if complexType has empty <sequence/> */
    axis2_bool_t has_sequence;      /**< True if complexType contains a sequence */
    axis2_char_t *base_type;        /**< Base type for extension/restriction */
} wsdl2c_complex_type_t;

/**
 * @brief Operation structure (parsed from WSDL)
 * Contains operation name and soapAction extracted from binding.
 * AXIS2C-1581: soap_action may be NULL if empty or missing in WSDL
 */
typedef struct wsdl2c_operation {
    axis2_char_t *name;             /**< Operation name */
    axis2_char_t *input_message;    /**< Input message reference */
    axis2_char_t *output_message;   /**< Output message reference */
    axis2_char_t *soap_action;      /**< SOAP action (NULL if empty - AXIS2C-1581) */
} wsdl2c_operation_t;

/**
 * @brief WSDL information structure
 */
typedef struct wsdl2c_wsdl {
    axis2_char_t *target_namespace;     /**< Target namespace */
    axis2_char_t *service_name;         /**< Service name */
    axis2_char_t *port_type_name;       /**< Port type name */
    axis2_char_t *binding_name;         /**< Binding name */
    axutil_array_list_t *operations;    /**< Operations array (wsdl2c_operation_t*) */
    axutil_array_list_t *messages;      /**< Messages array */
    axutil_array_list_t *schema_elements; /**< Schema elements (wsdl2c_schema_element_t*) */
    axutil_array_list_t *complex_types; /**< Complex types (wsdl2c_complex_type_t*) - AXIS2C-1224 */
    axis2_bool_t has_any_type;          /**< True if any xsd:any elements found (AXIS2C-1580) */
    axis2_bool_t has_empty_sequences;   /**< True if any empty sequences found (AXIS2C-1224) */
    axis2_bool_t has_typeless_elements; /**< True if any typeless elements found (AXIS2C-1421) */
    void *schema_node;                   /**< Schema information */
} wsdl2c_wsdl_t;

/**
 * @brief Main context structure
 */
typedef struct wsdl2c_context {
    const axutil_env_t *env;     /**< Environment */
    wsdl2c_options_t *options;   /**< Command line options */
    wsdl2c_wsdl_t *wsdl;         /**< Parsed WSDL information */
    xmlDocPtr doc;               /**< XML document */
    xmlXPathContextPtr xpath_ctx; /**< XPath context */
    struct wsdl2c_schema_registry *schema_registry; /**< External schema registry */
} wsdl2c_context_t;

/* Context management functions */

/**
 * @brief Create a new WSDL2C context
 * @param env Environment
 * @return New context or NULL on failure
 */
AXIS2_EXTERN wsdl2c_context_t* AXIS2_CALL
wsdl2c_context_create(const axutil_env_t *env);

/**
 * @brief Free a WSDL2C context
 * @param context Context to free
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
wsdl2c_context_free(wsdl2c_context_t *context, const axutil_env_t *env);

/* Option parsing functions */

/**
 * @brief Print usage information
 */
AXIS2_EXTERN void AXIS2_CALL
wsdl2c_print_usage(void);

/**
 * @brief Parse command line options
 * @param context Context to store options in
 * @param argc Argument count
 * @param argv Argument vector
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_options(wsdl2c_context_t *context, int argc, char **argv, const axutil_env_t *env);

/* WSDL parsing functions */

/**
 * @brief Parse WSDL file
 * @param context Context containing WSDL URI
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_wsdl(wsdl2c_context_t *context, const axutil_env_t *env);

/* Code generation functions */

/**
 * @brief Generate C code from parsed WSDL
 * @param context Context containing parsed WSDL
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_generate_code(wsdl2c_context_t *context, const axutil_env_t *env);

/* Utility functions */

/**
 * @brief Create a default environment for standalone use
 * @param log_file Log file name
 * @param log_level Log level
 * @return Environment or NULL on failure
 */
AXIS2_EXTERN axutil_env_t* AXIS2_CALL
axutil_env_create_all(const char *log_file, int log_level);

/**
 * @brief Free an environment
 * @param env Environment to free
 */
AXIS2_EXTERN void AXIS2_CALL
axutil_env_free(axutil_env_t *env);

/**
 * @brief Duplicate a string using environment allocator
 * @param env Environment
 * @param str String to duplicate
 * @return Duplicated string or NULL on failure
 */
AXIS2_EXTERN char* AXIS2_CALL
axutil_strdup(const axutil_env_t *env, const char *str);

/**
 * @brief Convert string to uppercase
 * @param str String to convert
 * @param env Environment (can be NULL for this utility)
 * @return Uppercase string (modifies in place)
 */
AXIS2_EXTERN char* AXIS2_CALL
axutil_string_toupper(const char *str, const axutil_env_t *env);

/* AXIS2C-1573 FIX: Identifier sanitization for valid C code generation
 *
 * WSDL type names may contain characters that are invalid in C identifiers:
 * - Period (.) - e.g., "Test.Types" from .NET namespaces
 * - Hyphen (-) - e.g., "my-type" (also AXIS2C-433)
 * - Colon (:) - e.g., namespace prefixes
 * - Leading digits - e.g., "123Type"
 *
 * This function converts WSDL names to valid C identifiers while preserving
 * the original name for XML serialization (xsi:type attribute).
 *
 * Characters that need sanitization:
 * - Invalid chars (., -, :, space, etc.) -> underscore (_)
 * - Leading digit -> prefix with underscore (_)
 * - Reserved C keywords -> suffix with _type (AXIS2C-1616)
 */

/**
 * @brief Sanitize a WSDL name to create a valid C identifier
 * @param env Environment
 * @param wsdl_name Original WSDL type/element name
 * @return Newly allocated sanitized C identifier string (caller must free)
 *
 * Examples:
 *   "Test.Types"     -> "Test_Types"      (AXIS2C-1573: period)
 *   "my-element"     -> "my_element"      (AXIS2C-433: hyphen)
 *   "type"           -> "type_value"      (AXIS2C-1616: reserved word)
 *   "123abc"         -> "_123abc"         (leading digit)
 *   "Test.My-Type"   -> "Test_My_Type"    (combined)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
wsdl2c_sanitize_c_identifier(const axutil_env_t *env, const axis2_char_t *wsdl_name);

/**
 * @brief Check if a name is a C reserved keyword
 * @param name Name to check
 * @return AXIS2_TRUE if reserved, AXIS2_FALSE otherwise
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
wsdl2c_is_reserved_keyword(const axis2_char_t *name);

/* Standalone mode macros and definitions */
#ifdef STANDALONE_BUILD

/* Memory management macros for standalone mode */
#define AXIS2_MALLOC(allocator, size) malloc(size)
#define AXIS2_FREE(allocator, ptr) free(ptr)

/* Utility macros */
#define AXIS2_ENV_CHECK(env, error_return) \
    if (!(env)) { \
        return error_return; \
    }

#define AXIS2_PARAM_CHECK(error, param, error_return) \
    if (!(param)) { \
        return error_return; \
    }

#define AXIS2_ERROR_SET(error, error_code, status) \
    /* Simplified error setting for standalone mode */

#define AXIS2_LOG_ERROR(log, location, format, ...) \
    fprintf(stderr, "ERROR: " format "\n", ##__VA_ARGS__)

#define AXIS2_LOG_INFO(log, location, format, ...) \
    printf("INFO: " format "\n", ##__VA_ARGS__)

#define AXIS2_LOG_DEBUG(log, location, format, ...) \
    /* Debug messages suppressed in standalone mode */

#define AXIS2_LOG_SI ""

#else

/* Full Axis2/C mode - use actual Axis2 macros */
#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axutil_log.h>

#endif /* STANDALONE_BUILD */

#ifdef __cplusplus
}
#endif

#endif /* WSDL2C_NATIVE_H */