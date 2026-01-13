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
 */
typedef struct wsdl2c_options {
    char *wsdl_uri;              /**< WSDL URI or file path */
    char *output_dir;            /**< Output directory */
    char *package_name;          /**< Package name */
    char *language;              /**< Target language (always "c") */
    char *databinding;           /**< Data binding type (adb, none) */
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
 */
typedef struct wsdl2c_schema_element {
    axis2_char_t *name;             /**< Element name */
    axis2_char_t *type;             /**< Element type (xsd:string, etc.) */
    axis2_char_t *namespace_uri;    /**< Namespace URI */
    axis2_bool_t is_any_type;       /**< True if xsd:any element (AXIS2C-1580) */
    axis2_bool_t is_nillable;       /**< True if nillable="true" */
    int min_occurs;                 /**< minOccurs value (0 = optional) */
    int max_occurs;                 /**< maxOccurs value (-1 = unbounded) */
} wsdl2c_schema_element_t;

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
    axis2_bool_t has_any_type;          /**< True if any xsd:any elements found (AXIS2C-1580) */
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