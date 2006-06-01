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

#ifndef AXIS2_WODEN_TYPES_ELEMENT_H
#define AXIS2_WODEN_TYPES_ELEMENT_H

/**
 * @file axis2_woden_types_element.h
 * @brief Axis2 Types Element Interface
 * This interface represents the &lt;wsdl:types&gt; element. 
 * It supports parsing, creating and manipulating a &lt;types&gt; element.
 * <p>
 * The data types used in WSDL elements are typically defined within a 
 * &lt;types&gt; element using a type system such as W3C XML Schema.
 * Schemas may be imported or inlined within &lt;types&gt;.
 * A &lt;types&gt; element may contain multiple schema import elements with
 * the same namespace attribute, so the schemaLocation attribute may be used 
 * to distinguish them.
 * Likewise, it is valid to have multiple inline schemas, so the id attribute 
 * may be used to distinguish them.
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <xml_schema/axis2_xml_schema.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_types_element axis2_woden_types_element_t;
typedef struct axis2_woden_types_element_ops axis2_woden_types_element_ops_t;
struct axis2_xml_schema;

/** @defgroup axis2_woden_types_element Types Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_types_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *types_el,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *types_el,
            const axis2_env_t *env);
    
    /**
     * Indicates the type system used within the &lt;types&gt; element. 
     * Typically the W3C XML Schema type system will be used, indicated by 
     * the namespace "http://www.w3.org/2001/XMLSchema". An alternative
     * schema-like type system is Relax NG (http://www.relaxng.org/).
     */
    axis2_status_t (AXIS2_CALL *
    set_type_system) (void *types_el,
            const axis2_env_t *env,
            axis2_char_t *type_system);

    /**
     * Get the string indicating the type system used within the &lt;types&gt;
     * element.
     */
    axis2_char_t *(AXIS2_CALL *
    get_type_system) (void *types_el,
            const axis2_env_t *env);

    /**
     * Add a Schema object for a schema inlined or imported within 
     * the &lt;types&gt; element.
     * 
     * @param schema the Schema object.
     */
    axis2_status_t (AXIS2_CALL *
    add_schema) (void *types_el,
            const axis2_env_t *env,
            struct axis2_xml_schema *schema);

    /**
     * Delete the specified Schema object.
     */
    axis2_status_t (AXIS2_CALL *
    remove_schema) (
            void *types_el,
            const axis2_env_t *env,
            struct axis2_xml_schema *schema);
    
    /**
     * Return the Schemas representing all inlined schemas or schema imports,
     * in the order in which they occur within the &lt;types&gt; element.
     * 
     * @return an array of Schema objects
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_schemas) (
            void *types_el,
            const axis2_env_t *env);
    
    /**
     * Return all Schemas where the specified namespace argument is either the
     * target namespace of an inlined schema or the imported namespace of a 
     * schema import. Schemas are returned in the order in which they occur 
     * within the &lt;types&gt; element.
     * <p>
     * A null namespace argument will return any inlined schemas missing their
     * target namespace attribute or any schema imports missing their namespace 
     * attribute.
     * 
     * @return the Schemas for the schema with the specified target namespace.
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_schemas_with_namespace) (
            void *types_el,
            const axis2_env_t *env,
            axis2_url_t *namespc);

    /**
     * Return all schemas inlined within the &lt;types&gt; element, in the order
     * in which they occur within &lt;types&gt;.
     * 
     * @return an array of Schema objects.
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_inlined_schemas) (
            void *types_el,
            const axis2_env_t *env);
    
    /**
     * Return all schema imports from within the &lt;types&gt; element, in the order
     * in which they occur within &lt;types&gt;.
     * 
     * @return an array of Schema objects.
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_imported_schemas) (
            void *types_el,
            const axis2_env_t *env);
};

struct axis2_woden_types_element
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_types_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
axis2_woden_types_element_resolve_methods(
        axis2_woden_types_element_t *types_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_TYPES_ELEMENT_FREE(types_el, env) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         free (types_el, env))

#define AXIS2_WODEN_TYPES_ELEMENT_TYPE(types_el, env) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         type (types_el, env))

#define AXIS2_WODEN_TYPES_ELEMENT_SET_TYPE_SYSTEM(types_el, env, type_system) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         set_type_system(types_el, env, type_system))

#define AXIS2_WODEN_TYPES_ELEMENT_GET_TYPE_SYSTEM(types_el, env) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         get_type_system(types_el, env))

#define AXIS2_WODEN_TYPES_ELEMENT_ADD_SCHEMA(types_el, env, schema) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         add_schema(types_el, env, schema))

#define AXIS2_WODEN_TYPES_ELEMENT_REMOVE_SCHEMA(types_el, env, schema) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         remove_schema(types_el, env, schema))

#define AXIS2_WODEN_TYPES_ELEMENT_GET_SCHEMAS(types_el, env) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         get_schemas(types_el, env))

#define AXIS2_WODEN_TYPES_ELEMENT_GET_SCHEMAS_WITH_NAMESPACE(types_el, env, \
        namespc) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         get_schemas_with_namespace(types_el, env, namespc))

#define AXIS2_WODEN_TYPES_ELEMENT_GET_INLINED_SCHEMAS(types_el, env) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         get_inlined_schemas(types_el, env))

#define AXIS2_WODEN_TYPES_ELEMENT_GET_IMPORTED_SCHEMAS(types_el, env) \
		(((axis2_woden_types_element_t *) types_el)->ops->\
         get_imported_schemas(types_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_TYPES_ELEMENT_H */
