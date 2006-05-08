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

#ifndef AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_H
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_H

/**
 * @file axis2_xml_schema_complex_content_restriction.h
 */

#include <axis2_xml_schema_annotated.h>
#include <axis2_xml_schema_obj_collection.h>
#include <axis2_xml_schema_any_attribute.h>
#include <axis2_xml_schema_simple_type.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

/** @defgroup axis2_xml_schema_complex_content_restriction Xml Schema Group
  * @ingroup axis2_xml_schema
  * @{
  */

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_complex_content_restriction 
                axis2_xml_schema_complex_content_restriction_t;
                
typedef struct axis2_xml_schema_complex_content_restriction_ops 
                axis2_xml_schema_complex_content_restriction_ops_t;

struct axis2_xml_schema_complex_content_restriction_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
           axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env);

    axis2_xml_schema_any_attribute_t* (AXIS2_CALL *
    get_any_attribute)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_any_attribute)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env,
            axis2_xml_schema_any_attribute_t *any_attr);
    
    axis2_xml_schema_obj_collection_t* (AXIS2_CALL *
    get_attributes)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env);
            
    axis2_qname_t* (AXIS2_CALL*
    get_base_type_name)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env);
            
    axis2_status_t (AXIS2_CALL *
    set_base_type_name)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env,
            axis2_qname_t *qname); 
            
    axis2_xml_schema_particle_t* (AXIS2_CALL *
    get_particle)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env);
                  
    axis2_status_t (AXIS2_CALL *
    set_particle)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env,
            axis2_xml_schema_particle_t *particle);
                  
    axis2_char_t* (AXIS2_CALL *
    to_string)(
            axis2_xml_schema_complex_content_restriction_t *cmp_content_res,
            axis2_env_t **env,
            axis2_char_t *prefix,
            int tab);
};

struct axis2_xml_schema_complex_content_restriction
{
    axis2_xml_schema_complex_content_restriction_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_DECLARE(axis2_xml_schema_complex_content_restriction_t *)
axis2_xml_schema_complex_content_restriction_create(axis2_env_t **env);



#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_FREE(cmp_content_res, env) \
		(( cmp_content_res)->ops->free(cmp_content_res, env))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_BASE_IMPL(cmp_content_res, env) \
		(( cmp_content_res)->ops->get_base_impl(cmp_content_res, env))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_ANY_ATTRIBUTE(cmp_content_res, env) \
		(( cmp_content_res)->ops->get_any_attribute(cmp_content_res, env))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_ANY_ATTRIBUTE(cmp_content_res,\
         env, any_attr)\
		(( cmp_content_res)->ops->set_content(cmp_content_res, env, any_attr ))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_ATTRIBUTES(cmp_content_res, env) \
		(( cmp_content_res)->ops->get_attributes(cmp_content_res, env))
            
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_BASE_TYPE_NAME(cmp_content_res, env)\
        (( cmp_content_res)->ops->get_base_type_name(cmp_content_res, env))            

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_BASE_TYPE_NAME(cmp_content_res,\
         env, base_type_name) \
        (( cmp_content_res)->ops->set_base_type_name(cmp_content_res, env, base_type_name))
            
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_PARTICLE(cmp_content_res, env) \
        (( cmp_content_res)->ops->get_particle(cmp_content_res, env))
            
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_PARTICLE(cmp_content_res, env) \
        (( cmp_content_res)->ops->set_particle(cmp_content_res, env))
                        
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_TO_STRING(cmp_content_res, env) \
        (( cmp_content_res)->ops->to_string(cmp_content_res, env))
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_H */
