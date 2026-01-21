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

#ifndef WSDL2C_SCHEMA_LOADER_H
#define WSDL2C_SCHEMA_LOADER_H

/**
 * @file schema_loader.h
 * @brief External schema import support for WSDL2C generator
 *
 * This module provides support for <xsd:import> and <xsd:include> directives
 * to enable processing WSDLs that reference external schema files.
 *
 * Features:
 * - Recursive loading of external XSD files
 * - URI resolution (relative and absolute paths)
 * - Circular import detection (tracks by namespace:sourceURI)
 * - Chameleon include support (namespace inheritance)
 *
 * Based on Axis2/Java approach:
 * - WSDL4JImportedWSDLHelper for WSDL imports
 * - SchemaCompiler for schema imports/includes
 * - XmlSchemaCollection for loaded schema management
 */

#include "wsdl2c_native.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Maximum path/key lengths for schema tracking */
#define WSDL2C_MAX_SCHEMA_KEY_LEN 2048
#define WSDL2C_MAX_URI_LEN 4096

/**
 * @brief Information about a loaded schema
 */
typedef struct wsdl2c_loaded_schema {
    axis2_char_t *source_uri;        /**< Source URI of the schema file */
    axis2_char_t *namespace_uri;     /**< Target namespace of the schema */
    xmlDocPtr doc;                   /**< Parsed XML document */
    xmlNodePtr schema_node;          /**< The <schema> root element */
    axutil_array_list_t *complex_types; /**< Complex types from this schema */
} wsdl2c_loaded_schema_t;

/**
 * @brief Schema registry for tracking loaded schemas and preventing circular imports
 *
 * Uses a simple array-based approach for loaded schema tracking.
 * Key format: "namespace_uri:source_uri" or just "source_uri" if no namespace.
 */
typedef struct wsdl2c_schema_registry {
    axis2_char_t *base_uri;              /**< Base URI for relative path resolution */
    axutil_array_list_t *loaded_schemas; /**< List of wsdl2c_loaded_schema_t* */
    axutil_array_list_t *loaded_keys;    /**< List of "namespace:uri" keys (axis2_char_t*) */
    axutil_array_list_t *all_types;      /**< All discovered complex types merged */
    int total_types_count;               /**< Total count of types discovered */
    int total_schemas_count;             /**< Total schemas loaded */
} wsdl2c_schema_registry_t;

/**
 * @brief Create a new schema registry
 *
 * @param env Environment
 * @param base_uri Base URI for resolving relative schema locations (typically WSDL path)
 * @return New schema registry or NULL on failure
 */
AXIS2_EXTERN wsdl2c_schema_registry_t* AXIS2_CALL
wsdl2c_schema_registry_create(const axutil_env_t *env, const axis2_char_t *base_uri);

/**
 * @brief Free a schema registry and all loaded schemas
 *
 * @param registry Registry to free
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
wsdl2c_schema_registry_free(wsdl2c_schema_registry_t *registry, const axutil_env_t *env);

/**
 * @brief Load an external schema file
 *
 * Parses the schema file and recursively follows any imports/includes.
 * Tracks loaded schemas to prevent circular imports.
 *
 * @param registry Schema registry
 * @param schema_location Schema location (relative or absolute)
 * @param parent_uri URI of the parent schema/WSDL (for relative resolution)
 * @param parent_namespace Namespace to inherit for includes (chameleon pattern)
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_load_external_schema(
    wsdl2c_schema_registry_t *registry,
    const axis2_char_t *schema_location,
    const axis2_char_t *parent_uri,
    const axis2_char_t *parent_namespace,
    const axutil_env_t *env);

/**
 * @brief Process all schema imports and includes within a schema node
 *
 * Finds all <xsd:import> and <xsd:include> elements and loads them.
 *
 * @param registry Schema registry
 * @param schema_node The <schema> node to process
 * @param parent_uri URI of this schema (for relative resolution)
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_process_schema_imports(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr schema_node,
    const axis2_char_t *parent_uri,
    const axutil_env_t *env);

/**
 * @brief Process WSDL-level imports
 *
 * Finds all <wsdl:import> elements and loads referenced WSDLs.
 *
 * @param registry Schema registry
 * @param doc WSDL document
 * @param wsdl_uri URI of this WSDL
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_process_wsdl_imports(
    wsdl2c_schema_registry_t *registry,
    xmlDocPtr doc,
    const axis2_char_t *wsdl_uri,
    const axutil_env_t *env);

/**
 * @brief Resolve a schema URI against a parent URI
 *
 * Resolution logic (matching Axis2/Java):
 * 1. If schema_location is absolute (file://, http://, /), use as-is
 * 2. If relative, resolve against parent URI's directory
 * 3. Handle ".." paths, URL encoding
 *
 * @param parent_uri Parent URI (directory base for resolution)
 * @param schema_location Schema location to resolve
 * @param env Environment
 * @return Newly allocated resolved URI (caller must free) or NULL on failure
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
wsdl2c_resolve_schema_uri(
    const axis2_char_t *parent_uri,
    const axis2_char_t *schema_location,
    const axutil_env_t *env);

/**
 * @brief Check if a schema has already been loaded
 *
 * Uses namespace:source_uri as the key to prevent circular imports.
 *
 * @param registry Schema registry
 * @param namespace_uri Namespace URI (can be NULL)
 * @param source_uri Source URI of the schema
 * @param env Environment
 * @return AXIS2_TRUE if already loaded, AXIS2_FALSE otherwise
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
wsdl2c_is_schema_loaded(
    wsdl2c_schema_registry_t *registry,
    const axis2_char_t *namespace_uri,
    const axis2_char_t *source_uri,
    const axutil_env_t *env);

/**
 * @brief Get all complex types from the registry
 *
 * Returns the merged list of all complex types from all loaded schemas.
 *
 * @param registry Schema registry
 * @param env Environment
 * @return Array list of wsdl2c_complex_type_t* (do not free - owned by registry)
 */
AXIS2_EXTERN axutil_array_list_t* AXIS2_CALL
wsdl2c_schema_registry_get_all_types(
    wsdl2c_schema_registry_t *registry,
    const axutil_env_t *env);

/**
 * @brief Parse complex types from a schema node
 *
 * Extracts all complexType definitions from the schema and adds them
 * to the registry's type collection.
 *
 * @param registry Schema registry
 * @param schema_node The <schema> node to parse
 * @param namespace_uri Target namespace of the schema
 * @param env Environment
 * @return AXIS2_SUCCESS or AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_schema_complex_types(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr schema_node,
    const axis2_char_t *namespace_uri,
    const axutil_env_t *env);

#ifdef __cplusplus
}
#endif

#endif /* WSDL2C_SCHEMA_LOADER_H */
