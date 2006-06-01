/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_WODEN_TYPES_H
#define AXIS2_WODEN_TYPES_H

/**
 * @file axis2_woden_types.h
 * @brief Axis2 Types Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_types_element.h>
#include <woden/wsdl20/axis2_woden_documentable.h>
#include <woden/wsdl20/axis2_woden_nested_component.h>
#include <woden/axis2_woden_wsdl_obj.h>
#include <xml_schema/axis2_xml_schema.h>
#include <xml_schema/axis2_xml_schema_element.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <axis2_array_list.h>


/** @defgroup axis2_woden_types Types
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_types_base axis2_woden_types_base_t;
typedef struct axis2_woden_types axis2_woden_types_t;
typedef struct axis2_woden_types_ops axis2_woden_types_ops_t;
struct axis2_woden_documentation_element;
struct axis2_woden_documentation;
struct axis2_xml_schema;
struct axis2_xml_schema_element;
struct axis2_xml_schema_type;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_types_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *types,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *types,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *types,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *types,
            axis2_env_t **env);
    
    /* ************************************************************
     *  Non-API implementation methods
     * ************************************************************/

    /* 
     * 
     * Returns the schema element declaration identified by the QName,
     * providing the element declaration is referenceable to the 
     * WSDL description (i.e. visible). This means it must exist in a
     * Schema that has been inlined or resolved from a schema import
     * within the &lt;types&gt; element according to the schema 
     * referenceability rules in the WSDL 2.0 spec.
     * If the element declaration is not referenceable, null is returned.
     * If validation is disabled, the referenceability rules do not apply
     * so all schemas are considered referenceable by the WSDL. 
     * 
     */
    void *(AXIS2_CALL * 
    get_element_declaration) (
            void *types,
            axis2_env_t **env,
            axis2_qname_t *qname);

    /*
     * 
     * Returns the schema type definition identified by the QName,
     * providing the type definition is referenceable by the 
     * WSDL description (i.e. visible). This means it must exist in a
     * Schema that has been inlined or resolved from a schema import
     * within the &lt;types&gt; element according to the schema
     * referenceability rules in the WSDL 2.0 spec.
     * If the type definition is not referenceable, null is returned.
     * If validation is disabled, the referenceability rules do not apply
     * so all schemas are considered referenceable by the WSDL.
     *  
     */
    void *(AXIS2_CALL * 
    get_type_definition) (
            void *types,
            axis2_env_t **env,
            axis2_qname_t *qname);
/*
 * Returns a List of Xml Schema objects for all schemas that are referenceable 
 * by the containing WSDL. Examples of schemas that are not referenceable include
 * schemas without a target namespace or schemas resolved from a schema import
 * whose target namespace does not match the imported namespace. Referenceability
 * is determined by validation.
 */
    axis2_array_list_t *(AXIS2_CALL *
    get_referenceable_schema_defs) (
            void *types,
            axis2_env_t **env);

    /*
     * Indicates if the namespace represented by the specified URI
     * is in-scope or referenceable by the WSDL description. 
     * This means it must be the target namespace of a schema correctly
     * imported or inlined directly within the &lt;types&gt; element.
     * 
     * @return true if the namespace is in-scope, or false if it is not.
     * 
     */
    axis2_bool_t (AXIS2_CALL * 
    is_namespace_in_scope_with_namespace_uri) (
            void *types,
            axis2_env_t **env,
            axis2_url_t *namespc_uri);

    /*
     * Indicates if the namespace represented by the specified QName
     * is in-scope or referenceable by the WSDL description. 
     * This means it must be the target namespace of a schema correctly
     * imported or inlined directly within the &lt;types&gt; element.
     * 
     */
    axis2_bool_t (AXIS2_CALL * 
    is_namespace_in_scope_with_qname) (
            void *types,
            axis2_env_t **env,
            axis2_qname_t *qname);

    /*
     * 
     * Indicates if the namespace represented by the specified String
     * is in-scope or referenceable by the WSDL description. 
     * This means it must be the target namespace of a schema correctly
     * imported or inlined directly within the &lt;types&gt; element.
     * 
     */
    axis2_bool_t (AXIS2_CALL * 
    is_namespace_in_scope_with_namespace) (
            void *types,
            axis2_env_t **env,
            axis2_char_t *namespc);


};

union axis2_woden_types_base
{
    axis2_woden_documentable_t documentable;
    axis2_woden_types_element_t types_element;
    axis2_woden_nested_component_t nested_component;
};

struct axis2_woden_types
{
    axis2_woden_types_base_t base;
    axis2_woden_types_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_types_element(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_nested_element(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_documentable_element(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_documentable(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_wsdl_obj(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_nested_component(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_wsdl_component(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_element_extensible(
        void *types,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_attr_extensible(
        void *types,
        axis2_env_t **env);


AXIS2_DECLARE(axis2_status_t)
axis2_woden_types_resolve_methods(
        axis2_woden_types_t *types,
        axis2_env_t **env,
        axis2_woden_types_t *types_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_TYPES_FREE(types, env) \
		(((axis2_woden_types_t *) types)->ops->free(types, env))

#define AXIS2_WODEN_TYPES_SUPER_OBJS(types, env) \
		(((axis2_woden_types_t *) types)->ops->super_objs(types, env))

#define AXIS2_WODEN_TYPES_TYPE(types, env) \
		(((axis2_woden_types_t *) types)->ops->type(types, env))

#define AXIS2_WODEN_TYPES_GET_BASE_IMPL(types, env) \
		(((axis2_woden_types_t *) types)->ops->get_base_impl(types, env))

#define AXIS2_WODEN_TYPES_GET_ELEMENT_DECLARATION(types, env, qname) \
		(((axis2_woden_types_t *) types)->ops->\
         get_element_declaration(types, env, qname))

#define AXIS2_WODEN_TYPES_GET_TYPE_DEFINITION(types, env, qname) \
		(((axis2_woden_types_t *) types)->ops->\
         get_type_definition(types, env, qname))

#define AXIS2_WODEN_TYPES_GET_REFERENCEABLE_SCHEMA_DEFS(types, env) \
		(((axis2_woden_types_t *) types)->ops->\
         get_referenceable_schema_defs(types, env))

#define AXIS2_WODEN_TYPES_GET_REFERENCEABLE_SCHEMA_DEFS_WITH_NAMESPACE(types, env, namespc) \
		(((axis2_woden_types_t *) types)->ops->\
         get_referenceable_schema_defs_with_namespace(types, env, namespc))

#define AXIS2_WODEN_TYPES_IS_NAMESPACE_IN_SCOPE_WITH_NAMESPACE_URI(types, env, namespc_uri) \
		(((axis2_woden_types_t *) types)->ops->\
         is_namespace_in_scope_with_namespace_uri(types, env, namespc_uri))

#define AXIS2_WODEN_TYPES_IS_NAMESPACE_IN_SCOPE_WITH_QNAME(types, env, qname) \
		(((axis2_woden_types_t *) types)->ops->\
         is_namespace_in_scope_with_qname(types, env, qname))

#define AXIS2_WODEN_TYPES_IS_NAMESPACE_IN_SCOPE_WITH_NAMESPACE(types, env, namespc) \
		(((axis2_woden_types_t *) types)->ops->\
         is_namespace_in_scope_with_namespace(types, env, namespc))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_TYPES_H */
