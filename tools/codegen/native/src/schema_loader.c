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

/**
 * @file schema_loader.c
 * @brief External schema import support for WSDL2C generator
 *
 * Implements loading of external XSD files referenced via:
 * - <xsd:import namespace="..." schemaLocation="..."/>
 * - <xsd:include schemaLocation="..."/>
 * - <wsdl:import namespace="..." location="..."/>
 */

#include "schema_loader.h"
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

/* Namespace URIs */
#define XSD_NS_URI "http://www.w3.org/2001/XMLSchema"
#define WSDL_NS_URI "http://schemas.xmlsoap.org/wsdl/"

/* Forward declarations of static functions */
static axis2_status_t process_xsd_import(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr import_node,
    const axis2_char_t *parent_uri,
    const axutil_env_t *env);

static axis2_status_t process_xsd_include(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr include_node,
    const axis2_char_t *parent_uri,
    const axis2_char_t *parent_namespace,
    const axutil_env_t *env);

static void free_loaded_schema(
    wsdl2c_loaded_schema_t *schema,
    const axutil_env_t *env);

static axis2_char_t* make_schema_key(
    const axis2_char_t *namespace_uri,
    const axis2_char_t *source_uri,
    const axutil_env_t *env);

static axis2_status_t parse_complex_type_node(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr complex_type_node,
    const axis2_char_t *namespace_uri,
    const axutil_env_t *env);


/* Create a new schema registry */
AXIS2_EXTERN wsdl2c_schema_registry_t* AXIS2_CALL
wsdl2c_schema_registry_create(const axutil_env_t *env, const axis2_char_t *base_uri)
{
    wsdl2c_schema_registry_t *registry = NULL;

    AXIS2_PARAM_CHECK(env->error, env, NULL);

    registry = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_schema_registry_t));
    if (!registry) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to allocate schema registry");
        return NULL;
    }

    memset(registry, 0, sizeof(wsdl2c_schema_registry_t));

    /* Store base URI for relative path resolution */
    if (base_uri) {
        registry->base_uri = axutil_strdup(env, base_uri);
    }

    /* Initialize lists */
    registry->loaded_schemas = axutil_array_list_create(env, 16);
    registry->loaded_keys = axutil_array_list_create(env, 16);
    registry->all_types = axutil_array_list_create(env, 64);
    registry->total_types_count = 0;
    registry->total_schemas_count = 0;

    if (!registry->loaded_schemas || !registry->loaded_keys || !registry->all_types) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create registry lists");
        wsdl2c_schema_registry_free(registry, env);
        return NULL;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "Created schema registry with base URI: %s", base_uri ? base_uri : "(null)");

    return registry;
}

/* Free a schema registry */
AXIS2_EXTERN void AXIS2_CALL
wsdl2c_schema_registry_free(wsdl2c_schema_registry_t *registry, const axutil_env_t *env)
{
    int i;

    if (!registry) {
        return;
    }

    /* Free base URI */
    if (registry->base_uri) {
        AXIS2_FREE(env->allocator, registry->base_uri);
    }

    /* Free loaded schemas */
    if (registry->loaded_schemas) {
        int count = axutil_array_list_size(registry->loaded_schemas, env);
        for (i = 0; i < count; i++) {
            wsdl2c_loaded_schema_t *schema =
                (wsdl2c_loaded_schema_t*)axutil_array_list_get(registry->loaded_schemas, env, i);
            free_loaded_schema(schema, env);
        }
        axutil_array_list_free(registry->loaded_schemas, env);
    }

    /* Free loaded keys */
    if (registry->loaded_keys) {
        int count = axutil_array_list_size(registry->loaded_keys, env);
        for (i = 0; i < count; i++) {
            axis2_char_t *key = (axis2_char_t*)axutil_array_list_get(registry->loaded_keys, env, i);
            if (key) {
                AXIS2_FREE(env->allocator, key);
            }
        }
        axutil_array_list_free(registry->loaded_keys, env);
    }

    /* Free all_types list and the types themselves
     * Note: Types were added to all_types by parse_complex_type_node and
     * are NOT owned by loaded_schemas. merge_imported_types creates copies
     * for the main WSDL, so these originals can be freed safely. */
    if (registry->all_types) {
        int count = axutil_array_list_size(registry->all_types, env);
        for (i = 0; i < count; i++) {
            wsdl2c_complex_type_t *ct =
                (wsdl2c_complex_type_t*)axutil_array_list_get(registry->all_types, env, i);
            if (ct) {
                if (ct->name) AXIS2_FREE(env->allocator, ct->name);
                if (ct->c_name) AXIS2_FREE(env->allocator, ct->c_name);
                if (ct->base_type) AXIS2_FREE(env->allocator, ct->base_type);

                /* Free elements within the type */
                if (ct->elements) {
                    int j, elem_count = axutil_array_list_size(ct->elements, env);
                    for (j = 0; j < elem_count; j++) {
                        wsdl2c_schema_element_t *elem =
                            (wsdl2c_schema_element_t*)axutil_array_list_get(ct->elements, env, j);
                        if (elem) {
                            if (elem->name) AXIS2_FREE(env->allocator, elem->name);
                            if (elem->c_name) AXIS2_FREE(env->allocator, elem->c_name);
                            if (elem->type) AXIS2_FREE(env->allocator, elem->type);
                            if (elem->c_type) AXIS2_FREE(env->allocator, elem->c_type);
                            if (elem->namespace_uri) AXIS2_FREE(env->allocator, elem->namespace_uri);
                            AXIS2_FREE(env->allocator, elem);
                        }
                    }
                    axutil_array_list_free(ct->elements, env);
                }
                AXIS2_FREE(env->allocator, ct);
            }
        }
        axutil_array_list_free(registry->all_types, env);
    }

    AXIS2_FREE(env->allocator, registry);
}

/* Free a loaded schema structure */
static void free_loaded_schema(wsdl2c_loaded_schema_t *schema, const axutil_env_t *env)
{
    int i;

    if (!schema) {
        return;
    }

    if (schema->source_uri) {
        AXIS2_FREE(env->allocator, schema->source_uri);
    }
    if (schema->namespace_uri) {
        AXIS2_FREE(env->allocator, schema->namespace_uri);
    }

    /* Free complex types from this schema */
    if (schema->complex_types) {
        int count = axutil_array_list_size(schema->complex_types, env);
        for (i = 0; i < count; i++) {
            wsdl2c_complex_type_t *ct =
                (wsdl2c_complex_type_t*)axutil_array_list_get(schema->complex_types, env, i);
            if (ct) {
                if (ct->name) AXIS2_FREE(env->allocator, ct->name);
                if (ct->c_name) AXIS2_FREE(env->allocator, ct->c_name);
                if (ct->base_type) AXIS2_FREE(env->allocator, ct->base_type);

                /* Free elements */
                if (ct->elements) {
                    int j, elem_count = axutil_array_list_size(ct->elements, env);
                    for (j = 0; j < elem_count; j++) {
                        wsdl2c_schema_element_t *elem =
                            (wsdl2c_schema_element_t*)axutil_array_list_get(ct->elements, env, j);
                        if (elem) {
                            if (elem->name) AXIS2_FREE(env->allocator, elem->name);
                            if (elem->c_name) AXIS2_FREE(env->allocator, elem->c_name);
                            if (elem->type) AXIS2_FREE(env->allocator, elem->type);
                            if (elem->c_type) AXIS2_FREE(env->allocator, elem->c_type);
                            if (elem->namespace_uri) AXIS2_FREE(env->allocator, elem->namespace_uri);
                            AXIS2_FREE(env->allocator, elem);
                        }
                    }
                    axutil_array_list_free(ct->elements, env);
                }
                AXIS2_FREE(env->allocator, ct);
            }
        }
        axutil_array_list_free(schema->complex_types, env);
    }

    /* Free the XML document */
    if (schema->doc) {
        xmlFreeDoc(schema->doc);
    }

    AXIS2_FREE(env->allocator, schema);
}

/* Create a key for tracking loaded schemas */
static axis2_char_t* make_schema_key(
    const axis2_char_t *namespace_uri,
    const axis2_char_t *source_uri,
    const axutil_env_t *env)
{
    axis2_char_t *key;
    size_t len;

    /* Key format: "namespace:source" or just "source" if no namespace */
    len = (namespace_uri ? strlen(namespace_uri) : 0) + 1 +
          (source_uri ? strlen(source_uri) : 0) + 1;

    key = AXIS2_MALLOC(env->allocator, len);
    if (!key) {
        return NULL;
    }

    snprintf(key, len, "%s:%s",
             namespace_uri ? namespace_uri : "",
             source_uri ? source_uri : "");

    return key;
}

/* Check if a schema has already been loaded */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
wsdl2c_is_schema_loaded(
    wsdl2c_schema_registry_t *registry,
    const axis2_char_t *namespace_uri,
    const axis2_char_t *source_uri,
    const axutil_env_t *env)
{
    axis2_char_t *key;
    int i, count;

    if (!registry || !source_uri) {
        return AXIS2_FALSE;
    }

    key = make_schema_key(namespace_uri, source_uri, env);
    if (!key) {
        return AXIS2_FALSE;
    }

    count = axutil_array_list_size(registry->loaded_keys, env);
    for (i = 0; i < count; i++) {
        axis2_char_t *existing_key =
            (axis2_char_t*)axutil_array_list_get(registry->loaded_keys, env, i);
        if (existing_key && strcmp(existing_key, key) == 0) {
            AXIS2_FREE(env->allocator, key);
            return AXIS2_TRUE;
        }
    }

    AXIS2_FREE(env->allocator, key);
    return AXIS2_FALSE;
}

/* Resolve a schema URI against a parent URI */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
wsdl2c_resolve_schema_uri(
    const axis2_char_t *parent_uri,
    const axis2_char_t *schema_location,
    const axutil_env_t *env)
{
    axis2_char_t *resolved = NULL;
    axis2_char_t *parent_copy = NULL;
    axis2_char_t *parent_dir = NULL;
    size_t len;

    if (!schema_location) {
        return NULL;
    }

    /* Check if schema_location is absolute */
    if (schema_location[0] == '/' ||
        strncmp(schema_location, "file://", 7) == 0 ||
        strncmp(schema_location, "http://", 7) == 0 ||
        strncmp(schema_location, "https://", 8) == 0) {
        /* Absolute path - use as-is */
        return axutil_strdup(env, schema_location);
    }

    /* Relative path - resolve against parent URI */
    if (!parent_uri) {
        /* No parent, use current directory */
        return axutil_strdup(env, schema_location);
    }

    /* Get directory part of parent URI */
    parent_copy = axutil_strdup(env, parent_uri);
    if (!parent_copy) {
        return NULL;
    }

    /* Handle file:// prefix */
    if (strncmp(parent_copy, "file://", 7) == 0) {
        memmove(parent_copy, parent_copy + 7, strlen(parent_copy) - 6);
    }

    /* Use dirname to get parent directory */
    parent_dir = dirname(parent_copy);

    /* Allocate resolved path */
    len = strlen(parent_dir) + 1 + strlen(schema_location) + 1;
    resolved = AXIS2_MALLOC(env->allocator, len);
    if (!resolved) {
        AXIS2_FREE(env->allocator, parent_copy);
        return NULL;
    }

    snprintf(resolved, len, "%s/%s", parent_dir, schema_location);

    AXIS2_FREE(env->allocator, parent_copy);

    /* Normalize the path (handle .. and .) */
    /* Simple normalization - could be enhanced for complex paths */
    {
        char *p;
        /* Replace /./ with / */
        while ((p = strstr(resolved, "/./")) != NULL) {
            memmove(p, p + 2, strlen(p + 2) + 1);
        }
        /* Handle /../ - simplified approach */
        while ((p = strstr(resolved, "/../")) != NULL) {
            char *prev_slash = p - 1;
            while (prev_slash > resolved && *prev_slash != '/') {
                prev_slash--;
            }
            if (prev_slash >= resolved) {
                memmove(prev_slash, p + 3, strlen(p + 3) + 1);
            } else {
                break; /* Can't go above root */
            }
        }
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "Resolved schema URI: %s -> %s (parent: %s)",
        schema_location, resolved, parent_uri);

    return resolved;
}

/* Load an external schema file */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_load_external_schema(
    wsdl2c_schema_registry_t *registry,
    const axis2_char_t *schema_location,
    const axis2_char_t *parent_uri,
    const axis2_char_t *parent_namespace,
    const axutil_env_t *env)
{
    axis2_char_t *resolved_uri = NULL;
    axis2_char_t *target_namespace = NULL;
    axis2_char_t *key = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr root = NULL;
    xmlChar *tns = NULL;
    wsdl2c_loaded_schema_t *loaded_schema = NULL;
    struct stat file_stat;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_PARAM_CHECK(env->error, registry, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema_location, AXIS2_FAILURE);

    /* Resolve the URI */
    resolved_uri = wsdl2c_resolve_schema_uri(parent_uri, schema_location, env);
    if (!resolved_uri) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Failed to resolve schema URI: %s", schema_location);
        return AXIS2_FAILURE;
    }

    /* Check if file exists */
    if (stat(resolved_uri, &file_stat) != 0) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Schema file not found: %s", resolved_uri);
        AXIS2_FREE(env->allocator, resolved_uri);
        return AXIS2_FAILURE;
    }

    /* Check if already loaded (preliminary check without namespace) */
    if (wsdl2c_is_schema_loaded(registry, NULL, resolved_uri, env)) {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "Schema already loaded: %s", resolved_uri);
        AXIS2_FREE(env->allocator, resolved_uri);
        return AXIS2_SUCCESS;
    }

    /* Parse the schema file */
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "Loading external schema: %s", resolved_uri);

    doc = xmlParseFile(resolved_uri);
    if (!doc) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Failed to parse schema file: %s", resolved_uri);
        AXIS2_FREE(env->allocator, resolved_uri);
        return AXIS2_FAILURE;
    }

    /* Get root element - should be <schema> */
    root = xmlDocGetRootElement(doc);
    if (!root) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No root element in schema: %s", resolved_uri);
        xmlFreeDoc(doc);
        AXIS2_FREE(env->allocator, resolved_uri);
        return AXIS2_FAILURE;
    }

    /* Verify it's a schema element */
    if (xmlStrcmp(root->name, BAD_CAST "schema") != 0) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Root element is not <schema> in: %s (found <%s>)",
            resolved_uri, root->name);
        xmlFreeDoc(doc);
        AXIS2_FREE(env->allocator, resolved_uri);
        return AXIS2_FAILURE;
    }

    /* Get target namespace */
    tns = xmlGetProp(root, BAD_CAST "targetNamespace");
    if (tns) {
        target_namespace = axutil_strdup(env, (const char*)tns);
        xmlFree(tns);
    } else if (parent_namespace) {
        /* Chameleon include: inherit parent namespace */
        target_namespace = axutil_strdup(env, parent_namespace);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "Schema %s has no targetNamespace, inheriting: %s",
            resolved_uri, target_namespace);
    }

    /* Check with namespace now */
    if (wsdl2c_is_schema_loaded(registry, target_namespace, resolved_uri, env)) {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "Schema already loaded (with namespace check): %s [%s]",
            resolved_uri, target_namespace ? target_namespace : "(none)");
        xmlFreeDoc(doc);
        AXIS2_FREE(env->allocator, resolved_uri);
        if (target_namespace) AXIS2_FREE(env->allocator, target_namespace);
        return AXIS2_SUCCESS;
    }

    /* Create loaded schema structure */
    loaded_schema = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_loaded_schema_t));
    if (!loaded_schema) {
        xmlFreeDoc(doc);
        AXIS2_FREE(env->allocator, resolved_uri);
        if (target_namespace) AXIS2_FREE(env->allocator, target_namespace);
        return AXIS2_FAILURE;
    }

    memset(loaded_schema, 0, sizeof(wsdl2c_loaded_schema_t));
    loaded_schema->source_uri = resolved_uri;
    loaded_schema->namespace_uri = target_namespace;
    loaded_schema->doc = doc;
    loaded_schema->schema_node = root;
    loaded_schema->complex_types = axutil_array_list_create(env, 16);

    /* Add key to loaded keys BEFORE processing imports (prevents circular) */
    key = make_schema_key(target_namespace, resolved_uri, env);
    if (key) {
        axutil_array_list_add(registry->loaded_keys, env, key);
    }

    /* Add to loaded schemas */
    axutil_array_list_add(registry->loaded_schemas, env, loaded_schema);
    registry->total_schemas_count++;

    /* Parse complex types from this schema */
    status = wsdl2c_parse_schema_complex_types(registry, root, target_namespace, env);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Failed to parse complex types from: %s", resolved_uri);
    }

    /* Recursively process imports/includes in this schema */
    status = wsdl2c_process_schema_imports(registry, root, resolved_uri, env);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "Loaded schema: %s [namespace: %s] - %d types",
        resolved_uri,
        target_namespace ? target_namespace : "(none)",
        axutil_array_list_size(loaded_schema->complex_types, env));

    return status;
}

/* Process schema imports and includes */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_process_schema_imports(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr schema_node,
    const axis2_char_t *parent_uri,
    const axutil_env_t *env)
{
    xmlNodePtr child;
    axis2_char_t *target_namespace = NULL;
    xmlChar *tns;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_PARAM_CHECK(env->error, registry, AXIS2_FAILURE);

    if (!schema_node) {
        return AXIS2_SUCCESS;
    }

    /* Get target namespace for include (chameleon) handling */
    tns = xmlGetProp(schema_node, BAD_CAST "targetNamespace");
    if (tns) {
        target_namespace = axutil_strdup(env, (const char*)tns);
        xmlFree(tns);
    }

    /* Iterate through child elements looking for import/include */
    for (child = schema_node->children; child; child = child->next) {
        if (child->type != XML_ELEMENT_NODE) {
            continue;
        }

        /* Check for xsd:import */
        if (xmlStrcmp(child->name, BAD_CAST "import") == 0) {
            status = process_xsd_import(registry, child, parent_uri, env);
            if (status != AXIS2_SUCCESS) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to process xsd:import");
            }
        }
        /* Check for xsd:include */
        else if (xmlStrcmp(child->name, BAD_CAST "include") == 0) {
            status = process_xsd_include(registry, child, parent_uri, target_namespace, env);
            if (status != AXIS2_SUCCESS) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to process xsd:include");
            }
        }
    }

    if (target_namespace) {
        AXIS2_FREE(env->allocator, target_namespace);
    }

    return AXIS2_SUCCESS;
}

/* Process an xsd:import element */
static axis2_status_t process_xsd_import(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr import_node,
    const axis2_char_t *parent_uri,
    const axutil_env_t *env)
{
    xmlChar *namespace_attr = NULL;
    xmlChar *schema_location = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    /* Get namespace attribute */
    namespace_attr = xmlGetProp(import_node, BAD_CAST "namespace");

    /* Get schemaLocation attribute */
    schema_location = xmlGetProp(import_node, BAD_CAST "schemaLocation");

    if (!schema_location) {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "xsd:import has no schemaLocation, namespace: %s",
            namespace_attr ? (const char*)namespace_attr : "(none)");
        if (namespace_attr) xmlFree(namespace_attr);
        return AXIS2_SUCCESS; /* Not an error - just no external file */
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "Processing xsd:import: namespace=%s, schemaLocation=%s",
        namespace_attr ? (const char*)namespace_attr : "(none)",
        (const char*)schema_location);

    /* Load the external schema */
    status = wsdl2c_load_external_schema(
        registry,
        (const axis2_char_t*)schema_location,
        parent_uri,
        (const axis2_char_t*)namespace_attr,
        env);

    if (namespace_attr) xmlFree(namespace_attr);
    if (schema_location) xmlFree(schema_location);

    return status;
}

/* Process an xsd:include element (chameleon pattern) */
static axis2_status_t process_xsd_include(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr include_node,
    const axis2_char_t *parent_uri,
    const axis2_char_t *parent_namespace,
    const axutil_env_t *env)
{
    xmlChar *schema_location = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    /* Get schemaLocation attribute */
    schema_location = xmlGetProp(include_node, BAD_CAST "schemaLocation");

    if (!schema_location) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "xsd:include has no schemaLocation");
        return AXIS2_FAILURE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "Processing xsd:include: schemaLocation=%s (parent namespace: %s)",
        (const char*)schema_location,
        parent_namespace ? parent_namespace : "(none)");

    /* Load the external schema - pass parent namespace for chameleon handling */
    status = wsdl2c_load_external_schema(
        registry,
        (const axis2_char_t*)schema_location,
        parent_uri,
        parent_namespace,
        env);

    if (schema_location) xmlFree(schema_location);

    return status;
}

/* Process WSDL-level imports */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_process_wsdl_imports(
    wsdl2c_schema_registry_t *registry,
    xmlDocPtr doc,
    const axis2_char_t *wsdl_uri,
    const axutil_env_t *env)
{
    xmlXPathContextPtr xpath_ctx = NULL;
    xmlXPathObjectPtr result = NULL;
    int i;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_PARAM_CHECK(env->error, registry, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, doc, AXIS2_FAILURE);

    /* Create XPath context */
    xpath_ctx = xmlXPathNewContext(doc);
    if (!xpath_ctx) {
        return AXIS2_FAILURE;
    }

    /* Register namespaces */
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "wsdl", BAD_CAST WSDL_NS_URI);
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "xsd", BAD_CAST XSD_NS_URI);
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "xs", BAD_CAST XSD_NS_URI);

    /* Find all wsdl:import elements */
    result = xmlXPathEvalExpression(BAD_CAST "//wsdl:import", xpath_ctx);
    if (result && result->nodesetval && result->nodesetval->nodeNr > 0) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "Found %d wsdl:import elements", result->nodesetval->nodeNr);

        for (i = 0; i < result->nodesetval->nodeNr; i++) {
            xmlNodePtr import_node = result->nodesetval->nodeTab[i];
            xmlChar *namespace_attr = xmlGetProp(import_node, BAD_CAST "namespace");
            xmlChar *location = xmlGetProp(import_node, BAD_CAST "location");

            if (location) {
                AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                    "Processing wsdl:import: namespace=%s, location=%s",
                    namespace_attr ? (const char*)namespace_attr : "(none)",
                    (const char*)location);

                /* Load the imported WSDL as a schema source */
                axis2_char_t *resolved_uri = wsdl2c_resolve_schema_uri(
                    wsdl_uri, (const axis2_char_t*)location, env);

                if (resolved_uri) {
                    /* Check if it's an XSD or another WSDL */
                    /* For now, try to load it as a schema */
                    xmlDocPtr imported_doc = xmlParseFile(resolved_uri);
                    if (imported_doc) {
                        xmlNodePtr root = xmlDocGetRootElement(imported_doc);
                        if (root) {
                            /* If it's a WSDL, process its types section */
                            if (xmlStrcmp(root->name, BAD_CAST "definitions") == 0) {
                                /* Find types section */
                                xmlNodePtr child;
                                for (child = root->children; child; child = child->next) {
                                    if (child->type == XML_ELEMENT_NODE &&
                                        xmlStrcmp(child->name, BAD_CAST "types") == 0) {
                                        /* Process schemas in types */
                                        xmlNodePtr schema_child;
                                        for (schema_child = child->children; schema_child;
                                             schema_child = schema_child->next) {
                                            if (schema_child->type == XML_ELEMENT_NODE &&
                                                xmlStrcmp(schema_child->name, BAD_CAST "schema") == 0) {
                                                wsdl2c_process_schema_imports(
                                                    registry, schema_child, resolved_uri, env);

                                                /* Parse types from embedded schema */
                                                xmlChar *tns = xmlGetProp(schema_child,
                                                    BAD_CAST "targetNamespace");
                                                wsdl2c_parse_schema_complex_types(
                                                    registry, schema_child,
                                                    tns ? (const char*)tns : NULL, env);
                                                if (tns) xmlFree(tns);
                                            }
                                        }
                                    }
                                }
                            }
                            /* If it's a schema, process directly */
                            else if (xmlStrcmp(root->name, BAD_CAST "schema") == 0) {
                                wsdl2c_process_schema_imports(registry, root, resolved_uri, env);
                                xmlChar *tns = xmlGetProp(root, BAD_CAST "targetNamespace");
                                wsdl2c_parse_schema_complex_types(
                                    registry, root, tns ? (const char*)tns : NULL, env);
                                if (tns) xmlFree(tns);
                            }
                        }
                        xmlFreeDoc(imported_doc);
                    }
                    AXIS2_FREE(env->allocator, resolved_uri);
                }

                xmlFree(location);
            }
            if (namespace_attr) xmlFree(namespace_attr);
        }
    }

    if (result) xmlXPathFreeObject(result);
    xmlXPathFreeContext(xpath_ctx);

    return status;
}

/* Parse complex types from a schema node */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_schema_complex_types(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr schema_node,
    const axis2_char_t *namespace_uri,
    const axutil_env_t *env)
{
    xmlNodePtr child;
    int type_count = 0;

    AXIS2_PARAM_CHECK(env->error, registry, AXIS2_FAILURE);

    if (!schema_node) {
        return AXIS2_SUCCESS;
    }

    /* Iterate through child elements looking for complexType */
    for (child = schema_node->children; child; child = child->next) {
        if (child->type != XML_ELEMENT_NODE) {
            continue;
        }

        if (xmlStrcmp(child->name, BAD_CAST "complexType") == 0) {
            xmlChar *name = xmlGetProp(child, BAD_CAST "name");
            if (name) {
                parse_complex_type_node(registry, child, namespace_uri, env);
                type_count++;
                xmlFree(name);
            }
        }
        /* Also check for elements with inline complexType definitions */
        else if (xmlStrcmp(child->name, BAD_CAST "element") == 0) {
            xmlNodePtr inner;
            for (inner = child->children; inner; inner = inner->next) {
                if (inner->type == XML_ELEMENT_NODE &&
                    xmlStrcmp(inner->name, BAD_CAST "complexType") == 0) {
                    /* Anonymous complexType - get name from parent element */
                    xmlChar *elem_name = xmlGetProp(child, BAD_CAST "name");
                    if (elem_name) {
                        /* Set temporary name attribute for parsing */
                        xmlSetProp(inner, BAD_CAST "name", elem_name);
                        parse_complex_type_node(registry, inner, namespace_uri, env);
                        type_count++;
                        xmlFree(elem_name);
                    }
                }
            }
        }
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "Parsed %d complex types from schema [namespace: %s]",
        type_count, namespace_uri ? namespace_uri : "(none)");

    return AXIS2_SUCCESS;
}

/* Parse a single complexType node */
static axis2_status_t parse_complex_type_node(
    wsdl2c_schema_registry_t *registry,
    xmlNodePtr complex_type_node,
    const axis2_char_t *namespace_uri,
    const axutil_env_t *env)
{
    wsdl2c_complex_type_t *complex_type = NULL;
    xmlChar *type_name = NULL;
    xmlNodePtr child_node = NULL;
    xmlNodePtr sequence_node = NULL;
    int element_count = 0;

    /* Get complexType name */
    type_name = xmlGetProp(complex_type_node, BAD_CAST "name");
    if (!type_name) {
        return AXIS2_SUCCESS; /* Anonymous type, skip */
    }

    /* Check if we already have this type (avoid duplicates) */
    {
        int i, count = axutil_array_list_size(registry->all_types, env);
        for (i = 0; i < count; i++) {
            wsdl2c_complex_type_t *existing =
                (wsdl2c_complex_type_t*)axutil_array_list_get(registry->all_types, env, i);
            if (existing && existing->name && strcmp(existing->name, (const char*)type_name) == 0) {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Skipping duplicate complexType: %s", type_name);
                xmlFree(type_name);
                return AXIS2_SUCCESS;
            }
        }
    }

    /* Allocate and initialize complex type structure */
    complex_type = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_complex_type_t));
    if (!complex_type) {
        xmlFree(type_name);
        return AXIS2_FAILURE;
    }

    memset(complex_type, 0, sizeof(wsdl2c_complex_type_t));
    complex_type->name = axutil_strdup(env, (const axis2_char_t*)type_name);
    complex_type->c_name = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)type_name);
    complex_type->elements = axutil_array_list_create(env, 8);
    complex_type->is_empty_sequence = AXIS2_FALSE;
    complex_type->has_sequence = AXIS2_FALSE;

    /* Look for sequence element within the complexType */
    for (child_node = complex_type_node->children; child_node; child_node = child_node->next) {
        if (child_node->type != XML_ELEMENT_NODE) {
            continue;
        }

        /* Check for direct sequence child */
        if (xmlStrcmp(child_node->name, BAD_CAST "sequence") == 0) {
            sequence_node = child_node;
            complex_type->has_sequence = AXIS2_TRUE;
            break;
        }
        /* Check for all */
        if (xmlStrcmp(child_node->name, BAD_CAST "all") == 0) {
            sequence_node = child_node;
            complex_type->has_sequence = AXIS2_TRUE;
            break;
        }
        /* Check for choice */
        if (xmlStrcmp(child_node->name, BAD_CAST "choice") == 0) {
            sequence_node = child_node;
            complex_type->has_sequence = AXIS2_TRUE;
            break;
        }

        /* Check for sequence inside complexContent/restriction or complexContent/extension */
        if (xmlStrcmp(child_node->name, BAD_CAST "complexContent") == 0 ||
            xmlStrcmp(child_node->name, BAD_CAST "simpleContent") == 0) {
            xmlNodePtr content_child;
            for (content_child = child_node->children; content_child;
                 content_child = content_child->next) {
                if (content_child->type != XML_ELEMENT_NODE) {
                    continue;
                }
                if (xmlStrcmp(content_child->name, BAD_CAST "restriction") == 0 ||
                    xmlStrcmp(content_child->name, BAD_CAST "extension") == 0) {
                    /* Get base type */
                    xmlChar *base = xmlGetProp(content_child, BAD_CAST "base");
                    if (base) {
                        complex_type->base_type = axutil_strdup(env, (const axis2_char_t*)base);
                        xmlFree(base);
                    }
                    /* Look for sequence inside */
                    xmlNodePtr inner_child;
                    for (inner_child = content_child->children; inner_child;
                         inner_child = inner_child->next) {
                        if (inner_child->type == XML_ELEMENT_NODE &&
                            (xmlStrcmp(inner_child->name, BAD_CAST "sequence") == 0 ||
                             xmlStrcmp(inner_child->name, BAD_CAST "all") == 0 ||
                             xmlStrcmp(inner_child->name, BAD_CAST "choice") == 0)) {
                            sequence_node = inner_child;
                            complex_type->has_sequence = AXIS2_TRUE;
                            break;
                        }
                    }
                }
                if (sequence_node) break;
            }
        }
        if (sequence_node) break;
    }

    /* Parse elements within the sequence/all/choice */
    if (sequence_node) {
        xmlNodePtr elem_node;
        for (elem_node = sequence_node->children; elem_node; elem_node = elem_node->next) {
            if (elem_node->type == XML_ELEMENT_NODE &&
                xmlStrcmp(elem_node->name, BAD_CAST "element") == 0) {

                wsdl2c_schema_element_t *element = NULL;
                xmlChar *elem_name = NULL;
                xmlChar *elem_type = NULL;
                xmlChar *elem_ref = NULL;
                xmlChar *nillable = NULL;
                xmlChar *min_occurs = NULL;
                xmlChar *max_occurs = NULL;

                elem_name = xmlGetProp(elem_node, BAD_CAST "name");
                elem_ref = xmlGetProp(elem_node, BAD_CAST "ref");

                /* Handle ref attribute (element reference) */
                if (!elem_name && elem_ref) {
                    /* Extract local part of ref (after colon if prefixed) */
                    const char *ref_str = (const char*)elem_ref;
                    const char *colon = strchr(ref_str, ':');
                    elem_name = xmlStrdup(BAD_CAST (colon ? colon + 1 : ref_str));
                }

                if (!elem_name) {
                    if (elem_ref) xmlFree(elem_ref);
                    continue;
                }

                element = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_schema_element_t));
                if (!element) {
                    xmlFree(elem_name);
                    if (elem_ref) xmlFree(elem_ref);
                    continue;
                }
                memset(element, 0, sizeof(wsdl2c_schema_element_t));

                element->name = axutil_strdup(env, (const axis2_char_t*)elem_name);
                element->c_name = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)elem_name);
                element->is_any_type = AXIS2_FALSE;
                element->is_typeless = AXIS2_FALSE;

                /* Store namespace */
                if (namespace_uri) {
                    element->namespace_uri = axutil_strdup(env, namespace_uri);
                }

                elem_type = xmlGetProp(elem_node, BAD_CAST "type");
                if (elem_type) {
                    element->type = axutil_strdup(env, (const axis2_char_t*)elem_type);
                    element->c_type = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)elem_type);
                    xmlFree(elem_type);
                } else if (elem_ref) {
                    /* For ref, use the reference as type */
                    element->type = axutil_strdup(env, (const axis2_char_t*)elem_ref);
                    element->c_type = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)elem_ref);
                } else {
                    /* Element has no type - check for inline complexType */
                    xmlNodePtr inner;
                    axis2_bool_t has_inline_type = AXIS2_FALSE;
                    for (inner = elem_node->children; inner; inner = inner->next) {
                        if (inner->type == XML_ELEMENT_NODE &&
                            xmlStrcmp(inner->name, BAD_CAST "complexType") == 0) {
                            has_inline_type = AXIS2_TRUE;
                            /* Use element name as type name */
                            element->type = axutil_strdup(env, (const axis2_char_t*)elem_name);
                            element->c_type = wsdl2c_sanitize_c_identifier(env,
                                (const axis2_char_t*)elem_name);
                            break;
                        }
                    }
                    if (!has_inline_type) {
                        element->is_typeless = AXIS2_TRUE;
                        element->type = axutil_strdup(env, "anyType");
                        element->c_type = axutil_strdup(env, "axiom_node_t");
                    }
                }

                nillable = xmlGetProp(elem_node, BAD_CAST "nillable");
                element->is_nillable = (nillable && xmlStrcmp(nillable, BAD_CAST "true") == 0);
                if (nillable) xmlFree(nillable);

                min_occurs = xmlGetProp(elem_node, BAD_CAST "minOccurs");
                element->min_occurs = min_occurs ? atoi((const char*)min_occurs) : 1;
                if (min_occurs) xmlFree(min_occurs);

                max_occurs = xmlGetProp(elem_node, BAD_CAST "maxOccurs");
                if (max_occurs) {
                    if (xmlStrcmp(max_occurs, BAD_CAST "unbounded") == 0) {
                        element->max_occurs = -1;
                    } else {
                        element->max_occurs = atoi((const char*)max_occurs);
                    }
                    xmlFree(max_occurs);
                } else {
                    element->max_occurs = 1;
                }

                axutil_array_list_add(complex_type->elements, env, element);
                element_count++;

                xmlFree(elem_name);
                if (elem_ref) xmlFree(elem_ref);
            }
            /* Also handle xsd:any elements */
            else if (elem_node->type == XML_ELEMENT_NODE &&
                     xmlStrcmp(elem_node->name, BAD_CAST "any") == 0) {
                wsdl2c_schema_element_t *element = AXIS2_MALLOC(env->allocator,
                    sizeof(wsdl2c_schema_element_t));
                if (element) {
                    memset(element, 0, sizeof(wsdl2c_schema_element_t));
                    element->name = axutil_strdup(env, "any");
                    element->c_name = axutil_strdup(env, "any");
                    element->type = axutil_strdup(env, "anyType");
                    element->c_type = axutil_strdup(env, "axiom_node_t");
                    element->is_any_type = AXIS2_TRUE;

                    xmlChar *min_occurs = xmlGetProp(elem_node, BAD_CAST "minOccurs");
                    element->min_occurs = min_occurs ? atoi((const char*)min_occurs) : 1;
                    if (min_occurs) xmlFree(min_occurs);

                    xmlChar *max_occurs = xmlGetProp(elem_node, BAD_CAST "maxOccurs");
                    if (max_occurs) {
                        if (xmlStrcmp(max_occurs, BAD_CAST "unbounded") == 0) {
                            element->max_occurs = -1;
                        } else {
                            element->max_occurs = atoi((const char*)max_occurs);
                        }
                        xmlFree(max_occurs);
                    } else {
                        element->max_occurs = 1;
                    }

                    axutil_array_list_add(complex_type->elements, env, element);
                    element_count++;
                }
            }
        }

        /* Check for empty sequence */
        if (element_count == 0) {
            complex_type->is_empty_sequence = AXIS2_TRUE;
        }
    }

    /* Add to all_types list */
    axutil_array_list_add(registry->all_types, env, complex_type);
    registry->total_types_count++;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "Parsed complexType '%s': %d elements, empty_sequence=%d",
        complex_type->name, element_count, complex_type->is_empty_sequence);

    xmlFree(type_name);
    return AXIS2_SUCCESS;
}

/* Get all complex types from the registry */
AXIS2_EXTERN axutil_array_list_t* AXIS2_CALL
wsdl2c_schema_registry_get_all_types(
    wsdl2c_schema_registry_t *registry,
    const axutil_env_t *env)
{
    (void)env; /* Unused but kept for API consistency */

    if (!registry) {
        return NULL;
    }

    return registry->all_types;
}
