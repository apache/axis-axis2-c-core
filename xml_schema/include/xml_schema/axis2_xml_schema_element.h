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

#include <xml_schema/axis2_xml_schema_type_receiver.h>
#include <xml_schema/axis2_xml_schema_particle.h>
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
    axis2_status_t (AXIS2_CALL *
    free) (void *element,
            axis2_env_t **env);
    
    axis2_xml_schema_particle_t *(AXIS2_CALL *
    get_base_impl) (
            void *element,
            axis2_env_t **env);

    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(void *element,
          axis2_env_t **env);
          
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *element,
                axis2_env_t **env);          

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_constraints)(
            void *element,
            axis2_env_t **env); 

    axis2_char_t *(AXIS2_CALL * 
    get_default_value)(
            void *element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_default_value)(
            void *element,
            axis2_env_t **env,
            axis2_char_t *default_value);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_block)(
            void *element,
            axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL * 
    set_block)(
            void *element,
            axis2_env_t **env,
            struct axis2_xml_schema_derivation_method * block);
            

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_final) (void *element,
                axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_final)(void *element,
               axis2_env_t **env,
               struct axis2_xml_schema_derivation_method *final_derivation);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL * 
    get_block_resolved)(
            void *element,
            axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL * 
    get_fixed_value)(
            void *element,
            axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_fixed_value)(
            void *element,
            axis2_env_t **env,
            axis2_char_t *fixed_value); 
            

    void *(AXIS2_CALL *
    get_element_type)(
            void *element,
            axis2_env_t **env); 
            

    struct axis2_xml_schema_form *(AXIS2_CALL *
    get_form)(void *element,
              axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_form)(void *element,
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
    set_name)(
            void *element,
            axis2_env_t **env,
            axis2_char_t *name);

    axis2_qname_t *(AXIS2_CALL * 
    get_ref_qname)(
            void *element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_ref_qname)(
            void *element,
            axis2_env_t **env,
            axis2_qname_t *ref_qname);

    axis2_qname_t *(AXIS2_CALL * 
    get_qname)(
            void *element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_qname)(
            void *element,
            axis2_env_t **env,
            axis2_qname_t *qualified_name); 

    struct axis2_xml_schema_type *(AXIS2_CALL *
    get_schema_type)(
            void *element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_schema_type)(
            void *element,
            axis2_env_t **env,
            struct axis2_xml_schema_type *schema_type);

    axis2_qname_t *(AXIS2_CALL * 
    get_schema_type_qname)(
            void *element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_schema_type_qname)(
            void *element,
            axis2_env_t **env,
            axis2_qname_t *schema_type_qname);

    axis2_qname_t *(AXIS2_CALL * 
    get_substitution_group)(
            void *element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL * 
    set_substitution_group)(
            void *element,
            axis2_env_t **env,
            axis2_qname_t *substitution_group);

    axis2_char_t *(AXIS2_CALL * 
    to_string)(
            void *element,
            axis2_env_t **env,
            axis2_char_t *prefix, 
            int tab);

    axis2_status_t (AXIS2_CALL * 
    set_type)(
            void *element,
            axis2_env_t **env,
            struct axis2_xml_schema_type *type);

};

struct axis2_xml_schema_element
{
    axis2_xml_schema_particle_t base;
    axis2_xml_schema_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_element_t *)
axis2_xml_schema_element_create(axis2_env_t **env);

/************ macros **********************************************************/

#define AXIS2_XML_SCHEMA_ELEMENT_FREE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->free(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_BASE_IMPL(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_base_impl(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_TYPE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->type(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_SUPER_OBJS(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->super_objs(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_CONSTRAINTS(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_constraints(\
            element, env))
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_DEFAULT_VALUE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_default_value(\
            element, env))            
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_DEFAULT_VALUE(element, env, def_value) \
		(((axis2_xml_schema_element_t *) element)->ops->set_default_value(\
            element, env, def_value))            
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_BLOCK(element, env, block) \
		(((axis2_xml_schema_element_t *) element)->ops->set_block(\
            element, env, block))            
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_FINAL(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_final(\
            element, env))              

#define AXIS2_XML_SCHEMA_ELEMENT_SET_FINAL(element, env, final_derivation) \
		(((axis2_xml_schema_element_t *) element)->ops->set_final(\
            element, env, final_derivation))            

#define AXIS2_XML_SCHEMA_ELEMENT_GET_BLOCK_RESOLVED(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_block_resolved(\
            element, env)) 
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_FIXED_VALUE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_fixed_value(\
            element, env))             

#define AXIS2_XML_SCHEMA_ELEMENT_SET_FIXED_VALUE(element, env, fixed_value) \
		(((axis2_xml_schema_element_t *) element)->ops->set_fixed_value(\
            element, env, fixed_value))

#define AXIS2_XML_SCHEMA_ELEMENT_GET_ELEMENT_TYPE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_element_type(\
            element, env))             
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_FORM(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_form(\
            element, env))              

#define AXIS2_XML_SCHEMA_ELEMENT_SET_FORM(element, env, form) \
		(((axis2_xml_schema_element_t *) element)->ops->set_form(\
            element, env, form))             
                     

#define AXIS2_XML_SCHEMA_ELEMENT_IS_ABSTRACT(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->is_abstract(\
            element, env))

#define AXIS2_XML_SCHEMA_ELEMENT_SET_ABSTRACT(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->set_abstract(\
            element, env))   
            

#define AXIS2_XML_SCHEMA_ELEMENT_IS_NILLABLE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->is_nillable(\
            element, env))       
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_NILLABLE(element, env, is_nillable) \
		(((axis2_xml_schema_element_t *) element)->ops->set_nillable(\
            element, env, is_nillable))
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_NAME(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_name(\
            element, env))
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_NAME(element, env, name) \
		(((axis2_xml_schema_element_t *) element)->ops->set_name(\
            element, env, name)) 

            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_REF_NAME(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_ref_qname(\
            element, env)) 
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_REF_NAME(element, env, ref_name) \
		(((axis2_xml_schema_element_t *) element)->ops->get_ref_qname(\
            element, env, ref_name))
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_QNAME(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_qname(\
            element, env)) 
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_QNAME(element, env, qualified_name) \
		(((axis2_xml_schema_element_t *) element)->ops->set_qname(\
            element, env, qualified_name))
                                                                         
#define AXIS2_XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_schema_type(\
            element, env)) 
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(element, env, schema_type) \
		(((axis2_xml_schema_element_t *) element)->ops->set_schema_type(\
            element, env, schema_type))
            
#define AXIS2_XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE_QNAME(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_schema_type_qname(\
            element, env)) 
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE_QNAME(element, env, schema_type_name) \
		(((axis2_xml_schema_element_t *) element)->ops->set_schema_type_qname(\
            element, env, schema_type_name))            
        
#define AXIS2_XML_SCHEMA_ELEMENT_GET_SUBSTITUTION_GROUP(element, env) \
		(((axis2_xml_schema_element_t *) element)->ops->get_substitution_group(\
            element, env)) 
            
#define AXIS2_XML_SCHEMA_ELEMENT_SET_SUBSTITUTION_GROUP(element, env, sub_group) \
		(((axis2_xml_schema_element_t *) element)->ops->set_substitution_group(\
            element, env, sub_group))            
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ELEMENT_H */
