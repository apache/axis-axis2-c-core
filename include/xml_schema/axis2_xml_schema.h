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

#ifndef AXIS2_XML_SCHEMA_H
#define AXIS2_XML_SCHEMA_H

/**
 * @file axis2_xml_schema.h
 * @brief Axis2 Xml Schema Interface
 *          Contains the definition of a schema. All XML Schema definition 
 *          language (XSD) elements are children of the schema element. 
 *          Represents the World Wide Web Consortium (W3C) schema element
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema Xml Schema
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema axis2_xml_schema_t;
typedef struct axis2_xml_schema_ops axis2_xml_schema_ops_t;
struct axis2_xml_schema_validation_event_handler;
struct axis2_xml_schema_form;
struct axis2_xml_schema_obj_table;
struct axis2_xml_schema_derivation_method;
struct axis2_xml_schema_element;
struct axis2_xml_schema_type;
struct axis2_xml_schema_obj_collection;
struct axis2_xml_schema_collection;

#define AXIS2_XML_SCHEMA_NS "http://www.w3.org/2001/XMLSchema"

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *schema,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *schema,
                    axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_namespace) (void *schema,
                         axis2_env_t **env,
                         axis2_char_t *prefix);

    struct axis2_xml_schema_form *(AXIS2_CALL *
    get_attr_form_default) (void *schema,
                            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_attr_form_default) (void *schema,
                         axis2_env_t **env,
                         struct axis2_xml_schema_form *value);

    struct axis2_xml_schema_obj_table *(AXIS2_CALL *
    get_attr_groups) (void *schema,
                         axis2_env_t **env);

    struct axis2_xml_schema_obj_table *(AXIS2_CALL *
    get_attrs) (void *schema,
                         axis2_env_t **env);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_block_default) (void *schema,
                            axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_block_default) (void *schema,
                    axis2_env_t **env,
                    struct axis2_xml_schema_derivation_method *block_default); 

    struct axis2_xml_schema_form *(AXIS2_CALL *
    get_element_form_default) (void *schema,
                                    axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_element_form_default) (
                    struct axis2_xml_schema_form *element_form_default,
                    axis2_env_t **env); 

    struct axis2_xml_schema_obj_table *(AXIS2_CALL *
    get_elements) (void *schema,
                        axis2_env_t **env); 

    struct axis2_xml_schema_element *(AXIS2_CALL *
    get_element_by_qname) (void *schema,
                                axis2_env_t **env,
                                axis2_qname_t *qname); 

    struct axis2_xml_schema_type *(AXIS2_CALL *
    get_type_by_qname) (void *schema,
                            axis2_env_t **env,
                            axis2_qname_t *qname); 

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_final_default) (void *schema,
                            axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_final_default) (
                struct axis2_xml_schema_derivation_method *final_default,
                axis2_env_t **env); 

    struct axis2_xml_schema_obj_table *(AXIS2_CALL *
    get_groups) (void *schema,
                    axis2_env_t **env); 

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_includes) (void *schema,
                        axis2_env_t **env); 

    axis2_bool_t (AXIS2_CALL *
    is_compiled) (void *schema,
                     axis2_env_t **env); 

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_items) (void *schema,
                   axis2_env_t **env); 

    struct axis2_xml_schema_obj_table *(AXIS2_CALL *
    get_notations) (void *schema,
                        axis2_env_t **env); 

    struct axis2_xml_schema_obj_table *(AXIS2_CALL *
    get_schema_types) (void *schema,
                            axis2_env_t **env); 

    axis2_char_t *(AXIS2_CALL *
    get_target_namespace) (void *schema,
                                axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_target_namespace) (void *schema,
                                axis2_env_t **env,
                                axis2_char_t *target_namespc); 

    axis2_char_t *(AXIS2_CALL *
    get_version) (void *schema,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    compile) (void *schema,
                    axis2_env_t **env,
                    struct axis2_xml_schema_validation_event_handler *eh);

    axis2_status_t (AXIS2_CALL *
    write_a_out) (void *schema,
                        axis2_env_t **env,
                        void *outstream); 

    axis2_status_t (AXIS2_CALL *
    write_a_writer) (void *schema,
                        void *writer); 

    axis2_hash_t *(AXIS2_CALL *
    get_prefix_to_namespace_map) (void *schema,
                                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_prefix_to_namespace_map) (void *schema,
                                        axis2_env_t **env,
                                        axis2_hash_t *map);

    axis2_status_t (AXIS2_CALL *
    add_type) (void *schema,
                    axis2_env_t **env,
                    struct axis2_xml_schema_type *type);

};

struct axis2_xml_schema
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_ops_t *ops;
};

/**
 * Creates new Xml Schema
 */
AXIS2_DECLARE(axis2_xml_schema_t *)
axis2_xml_schema_create(axis2_env_t **env,
                                axis2_char_t *namespc,
                                struct axis2_xml_schema_collection *parent);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_resolve_methods(
                                axis2_xml_schema_t *schema,
                                axis2_env_t **env,
                                axis2_xml_schema_t *schema_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_FREE(schema, env) \
		(((axis2_xml_schema_t *) schema)->ops->\
            free(schema, env))

#define AXIS2_XML_SCHEMA_GET_BASE_IMPL(schema, env) \
		(((axis2_xml_schema_t *) schema)->ops->\
            get_base_impl(schema, env))

#define AXIS2_XML_SCHEMA_GET_XPATH(schema, env) \
		(((axis2_xml_schema_t *) schema)->ops->\
            get_schema(schema, env))

#define AXIS2_XML_SCHEMA_SET_XPATH(schema, env, x_path) \
		(((axis2_xml_schema_t *) schema)->ops->\
            set_schema(schema, env, x_path))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_H */
