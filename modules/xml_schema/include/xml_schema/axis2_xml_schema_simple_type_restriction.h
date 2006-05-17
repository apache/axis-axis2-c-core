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

#ifndef AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_H
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_H

/**
 * @file axis2_xml_schema_simple_type_restriction.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all simple_type_restriction types.
 */


#include <xml_schema/axis2_xml_schema_simple_type.h>
#include <xml_schema/axis2_xml_schema_simple_type_content.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <axis2_hash.h>
/** @defgroup axis2_xml_schema_simple_type_restriction 
  * @ingroup axis2_xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_simple_type_restriction 
                    axis2_xml_schema_simple_type_restriction_t;
typedef struct axis2_xml_schema_simple_type_restriction_ops 
                    axis2_xml_schema_simple_type_restriction_ops_t;

struct axis2_xml_schema_simple_type_restriction_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *simple_type_restriction,
            axis2_env_t **env);

    axis2_xml_schema_simple_type_content_t *(AXIS2_CALL *
    get_base_impl) (void *simple_type_restriction,
                    axis2_env_t **env);
                    
    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(void *simple_type_restriction,
                axis2_env_t **env);
                
    axis2_hash_t *(AXIS2_CALL *
    super_objs)(void *simple_type_restriction,
                axis2_env_t **env);                                    

    axis2_xml_schema_simple_type_t *(AXIS2_CALL *
    get_base_type)(void *simple_type_restriction,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_base_type)(void *simple_type_restriction,
                    axis2_env_t **env,
                    axis2_xml_schema_simple_type_t *base_type);
    
    axis2_qname_t* (AXIS2_CALL *
    get_base_type_name)(void *simple_type_restriction,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_base_type_name)(void *simple_type_restriction,
                    axis2_env_t **env,
                    axis2_qname_t *qname);
                    
    axis2_xml_schema_obj_collection_t* (AXIS2_CALL *
    get_facets)(void *simple_type_restriction,
                axis2_env_t **env);
                
    axis2_char_t* (AXIS2_CALL *
    to_string)(void *simple_type_restriction,
               axis2_env_t **env,
               axis2_char_t *prefix,
               int tab);
               
                               
};

struct axis2_xml_schema_simple_type_restriction
{
    axis2_xml_schema_simple_type_content_t base;
    axis2_xml_schema_simple_type_restriction_ops_t *ops;
};

/**
 * Creates new Xml Schema Particle
 * Particle types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_DECLARE(axis2_xml_schema_simple_type_restriction_t *)
axis2_xml_schema_simple_type_restriction_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_simple_type_restriction_resolve_methods(
                    axis2_xml_schema_simple_type_restriction_t *simple_type_restriction,
                    axis2_env_t **env,
                    axis2_xml_schema_simple_type_restriction_t *simple_type_restriction_impl,
                    axis2_hash_t *methods);
*/


#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_FREE(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            free(simple_type_restriction, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_BASE_IMPL(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            get_base_impl(simple_type_restriction, env))
            
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_TYPE(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            type(simple_type_restriction, env))
            
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_SUPER_OBJS(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            super_objs(simple_type_restriction, env))                        

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_BASE_TYPE(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            get_max_occurs(simple_type_restriction, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_SET_BASE_TYPE(simple_type_restriction,\
         env, base_type) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            set_base_type(simple_type_restriction, env, base_type))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_BASE_TYPE_NAME(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            get_base_type_name(simple_type_restriction, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_SET_BASE_TYPE_NAME(simple_type_restriction, \
         env, base_type_name) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            set_base_type_name(simple_type_restriction, env, base_type_name))
            
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_FACETS(simple_type_restriction, env) \
		(((axis2_xml_schema_simple_type_restriction_t *) simple_type_restriction)->ops->\
            get_facets(simple_type_restriction, env))
            

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_H */
