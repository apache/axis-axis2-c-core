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

#ifndef AXIS2_XML_SCHEMA_ELEMENT_H
#define AXIS2_XML_SCHEMA_ELEMENT_H

/**
 * @file axis2_xml_schema_element.h
 * @brief Axis2 Xml Schema Element Interface
 *          Class for elements. Represents the World Wide Web Consortium (W3C) 
 *          element element.
 */

#include <axis2_xml_schema_type_receiver.h>
#include <axis2_qname.h>

/** @defgroup axis2_xml_schema_element Xml Schema Element
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_element axis2_xml_schema_element_t;
typedef struct axis2_xml_schema_element_ops axis2_xml_schema_element_ops_t;
struct axis2_xml_schema;
struct axis2_xml_schema_type_receiver;
struct axis2_xml_schema_obj_collection;
struct axis2_xml_schema_derivation_method;
struct axis2_xml_schema_form;
struct axis2_xml_schema_type;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *element,
            axis2_env_t **env);
    
    struct axis2_xml_schema_type_receiver *(AXIS2_CALL *
    get_base_impl) (void *element,
                    axis2_env_t **env);

    /**
     * Returns a collection of constraints on the element.
     */
    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_constraints) (void *element,
                                axis2_env_t **env); 

    axis2_char_t *(AXIS2_CALL * 
    get_default_value) (void *element,
                                axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_default_value) (void *element,
                                axis2_env_t **env,
                                axis2_char_t *default_value);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_block) (void *element,
                        axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL * 
    set_block) (void *element,
                        axis2_env_t **env,
                        struct axis2_xml_schema_derivation_method * block);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_final) (void *element,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_final) (void *element,
                        axis2_env_t **env,
                        struct axis2_xml_schema_derivation_method *final_derivation);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL * 
    get_block_resolved) (void *element,
                                axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL * 
    get_fixed_value) (void *element,
                                axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_fixed_value) (void *element,
                                axis2_env_t **env,
                                axis2_char_t *fixed_value); 

    void *(AXIS2_CALL *
    get_element_type) (void *element,
                                axis2_env_t **env); 

    struct axis2_xml_schema_form *(AXIS2_CALL *
    get_form) (void *element,
                        axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_form) (void *element,
                        axis2_env_t **env,
                        struct axis2_xml_schema_form *form); 

    axis2_bool_t (AXIS2_CALL *
    is_abstract) (void *element,
                            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_abstract) (void *element,
                            axis2_env_t **env,
                            axis2_bool_t is_abstract);

    axis2_bool_t (AXIS2_CALL *
    is_nillable) (void *element,
                            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_nillable) (void *element,
                            axis2_env_t **env,
                            axis2_bool_t  is_nillable);

    axis2_char_t *(AXIS2_CALL * 
    get_name) (void *element,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_name) (void *element,
                        axis2_env_t **env,
                        axis2_char_t *name);

    axis2_qname_t *(AXIS2_CALL * 
    get_ref_qname) (void *element,
                            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_ref_qname) (void *element,
                            axis2_env_t **env,
                            axis2_qname_t *ref_qname);

    axis2_qname_t *(AXIS2_CALL * 
    get_qname) (void *element,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_qname) (void *element,
                        axis2_env_t **env,
                        axis2_qname_t *qualified_name); 

    struct axis2_xml_schema_type *(AXIS2_CALL *
    get_schema_type) (void *element,
                                axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_schema_type) (void *element,
                                axis2_env_t **env,
                                struct axis2_xml_schema_type *schema_type);

    axis2_qname_t *(AXIS2_CALL * 
    get_schema_type_qname) (void *element,
                                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_schema_type_qname) (void *element,
                                    axis2_env_t **env,
                                    axis2_qname_t *schema_type_qname);

    axis2_qname_t *(AXIS2_CALL * 
    get_substitution_group) (void *element,
                                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_substitution_group) (void *element,
                                    axis2_env_t **env,
                                    axis2_qname_t *substitution_group);

    axis2_char_t *(AXIS2_CALL * 
    to_string) (void *element,
                        axis2_env_t **env,
                        axis2_char_t *prefix, 
                        int tab);

    axis2_status_t (AXIS2_CALL * 
    set_type) (void *element,
                    axis2_env_t **env,
                    struct axis2_xml_schema_type *type);

};

struct axis2_xml_schema_element
{
    axis2_xml_schema_type_receiver_t base;
    axis2_xml_schema_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_element_t *)
axis2_xml_schema_element_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_element_resolve_methods(
                                axis2_xml_schema_element_t *element,
                                axis2_env_t **env,
                                axis2_xml_schema_element_t *element_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_ELEMENT_FREE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->free(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_BASE_IMPL(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_base_impl(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_ID(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_id(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_SET_ID(element, env, id) \
		(((axis2_xml_schema_element_t *) element)->ops->set_id(\
            element, env, id))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_ANNOTATION(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_annotation(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_SET_ANNOTATION(element, env, \
        annotation) \
		(((axis2_xml_schema_element_t *) element)->ops->set_annotation(\
            element, env, annotation))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_UNHANDLED_ATTRS(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_unhandled_attrs(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_SET_UNHANDLED_ATTRS(element, \
        env, unhandled_attrs) \
		(((axis2_xml_schema_element_t *) element)->ops->set_unhandled_attrs(\
            element, env, unhandled_attrs))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ELEMENT_H */
