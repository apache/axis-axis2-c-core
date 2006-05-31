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

#ifndef AXIS2_XML_SCHEMA_COMPLEX_TYPE_H
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_H

/**
 * @file axis2_xml_schema_type.h
 * @brief Axis2 Xml Schema Type Interface
 *          The base class for all complex types and complex types.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <xml_schema/axis2_xml_schema_complex_type.h>
#include <xml_schema/axis2_xml_schema_any_attribute.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <xml_schema/axis2_xml_schema_obj_table.h>
#include <xml_schema/axis2_xml_schema_derivation_method.h>
#include <xml_schema/axis2_xml_schema_content_type.h>
/** @defgroup axis2_xml_schema_complex_type Xml Schema Type
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_complex_type 
                    axis2_xml_schema_complex_type_t;
typedef struct axis2_xml_schema_complex_type_ops 
                    axis2_xml_schema_complex_type_ops_t;
#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_complex_type_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *cmp_type,
           const axis2_env_t *env);

    axis2_xml_schema_type_t *(AXIS2_CALL *
    get_base_impl)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_hash_t *(AXIS2_CALL *
    super_objs)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(
            void *cmp_type,
            const axis2_env_t *env);                        

    axis2_xml_schema_any_attribute_t* (AXIS2_CALL *
    get_any_attribute)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t  (AXIS2_CALL *
    set_any_attribute)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_xml_schema_any_attribute_t *any_attr);
            
    axis2_xml_schema_obj_collection_t* (AXIS2_CALL *
    get_attributes)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_xml_schema_obj_table_t* (AXIS2_CALL *
    get_attribute_use)(
            void *cmp_type,
            const axis2_env_t *env);
    
    axis2_xml_schema_any_attribute_t* (AXIS2_CALL *
    get_attribute_wildcard)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_xml_schema_derivation_method_t* (AXIS2_CALL *
    get_block)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_block)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_xml_schema_derivation_method_t *block);
            
    axis2_xml_schema_derivation_method_t* (AXIS2_CALL *
    get_block_resolved)(
            void *cmp_type,
            const axis2_env_t *env);
            
    void* (AXIS2_CALL *
    get_content_model)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_content_model)(
            void *cmp_type,
            const axis2_env_t *env,
            void *content_model);
            
    axis2_xml_schema_content_type_t* (AXIS2_CALL *
    get_content_type)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_content_type)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_xml_schema_content_type_t *content_type);    
            
    axis2_xml_schema_particle_t * (AXIS2_CALL *
    get_content_type_particle)(
            void *cmp_type,
            const axis2_env_t *env);                    
                                                
    axis2_bool_t (AXIS2_CALL *
    is_abstract)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_abstract)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_bool_t b);
            
    axis2_bool_t (AXIS2_CALL*
    is_mixed)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_mixed)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_bool_t b);
            
    axis2_xml_schema_particle_t* (AXIS2_CALL *
    get_particle)(
            void *cmp_type,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_particle)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_xml_schema_particle_t *particle);
                        
            
            
    axis2_char_t* (AXIS2_CALL*
    to_string)(
            void *cmp_type,
            const axis2_env_t *env,
            axis2_char_t *prefix,
            int tab);
            
};

struct axis2_xml_schema_complex_type
{
    axis2_xml_schema_type_t base;
    axis2_xml_schema_complex_type_ops_t *ops;
};

/**
 * Creates new Xml Schema Type
 * Type types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_DECLARE(axis2_xml_schema_complex_type_t *)
axis2_xml_schema_complex_type_create(const axis2_env_t *env,
                        struct axis2_xml_schema *schema);

/************************** macros *******************************************/

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_FREE(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    free(complex_type, env))

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_BASE_IMPL(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_base_impl(complex_type, env))
		    
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_TYPE(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    type(complex_type, env))
		    
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    super_objs(complex_type, env))		    		    

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_ANY_ATTRIBUTE(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_any_attribute(complex_type, env))

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_ANY_ATTRIBUTE(complex_type, env, any_attr) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    set_any_attribute(complex_type, env, any_attr))

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_ATTRIBUTES(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_attributes(complex_type, env))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_ATTRIBUTE_USE(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_attribute_use(complex_type, env))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_ATTRIBUTE_WILDCARD(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_attribute_wildcard(complex_type, env))		

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_BLOCK(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_block(complex_type, env))

#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_BLOCK(complex_type, env, block) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    set_block(complex_type, env, block))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_BLOCK_RESOLVED(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_block_resolved(complex_type, env))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_CONTENT_MODEL(complex_type, env, cnt_mtype) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_content_model(complex_type, env, cnt_mtype))	
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_CONTENT_MODEL(complex_type, env, cnt_model) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    set_content_model(complex_type, env, cnt_model))
		    							
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_CONTENT_TYPE(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    get_content_type(complex_type, env))			
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_CONTENT_TYPE(complex_type, env, cnt_type) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    set_content_type(complex_type, env, cnt_type))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_CONTENT_TYPE_PARTICLE(complex_type, env) \
        (((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
            get_content_type_particle(complex_type, env))
        
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_IS_ABSTRACT(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    is_abstract(complex_type, env))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_ABSTRACT(complex_type, env, b) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    set_abstract(complex_type, env, b))	
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_IS_MIXED(complex_type, env) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    is_mixed(complex_type, env))
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_MIXED(complex_type, env, b) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    set_mixed(complex_type, env, b))				        				
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_PARTICLE(complex_type, env) \
        (((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
            get_particle(complex_type, env))
        
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_SET_PARTICLE(complex_type, env, particle) \
        (((axis2_xml_schema_complex_type_t *)complex_type)->ops->\
            set_particle(complex_type, env, particle))        		
		
#define AXIS2_XML_SCHEMA_COMPLEX_TYPE_TO_STRING(complex_type, env, prefix, tab) \
		(((axis2_xml_schema_complex_type_t *) complex_type)->ops->\
		    to_string(complex_type, env, prefix, tab))

/******************************** end macros **********************************/         
                                                  
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_COMPLEX_TYPE_H */
