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

#ifndef XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_H
#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_H

/**
 * @file xml_schema_complex_content_extension.h
 */

#include <xml_schema_annotated.h>
#include <xml_schema_obj_collection.h>
#include <xml_schema_any_attribute.h>
#include <xml_schema_particle.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
/** @defgroup xml_schema_complex_content_extension 
  * @ingroup xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_complex_content_extension 
                xml_schema_complex_content_extension_t;
                
typedef struct xml_schema_complex_content_extension_ops 
                xml_schema_complex_content_extension_ops_t;

struct xml_schema_complex_content_extension_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(void *cmp_content_ext,
          const axutil_env_t *env);

    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (
            void *cmp_content_ext,
            const axutil_env_t *env);
            
    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *cmp_content_ext,
            const axutil_env_t *env);  
            
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(
            void *cmp_content_ext,
            const axutil_env_t *env);                      

    xml_schema_any_attribute_t* (AXIS2_CALL *
    get_any_attribute)(
            void *cmp_content_ext,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_any_attribute)(
            void *cmp_content_ext,
            const axutil_env_t *env,
            xml_schema_any_attribute_t *any_attr);
    
    xml_schema_obj_collection_t* (AXIS2_CALL *
    get_attributes)(
            void *cmp_content_ext,
            const axutil_env_t *env);
            
    axis2_qname_t* (AXIS2_CALL*
    get_base_type_name)(
            void *cmp_content_ext,
            const axutil_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_base_type_name)(
            void *cmp_content_ext,
            const axutil_env_t *env,
            axis2_qname_t *qname);
            
    xml_schema_particle_t* (AXIS2_CALL *
    get_particle)(
            void *cmp_content_ext,
            const axutil_env_t *env);
                  
    axis2_status_t (AXIS2_CALL *
    set_particle)(
            void *cmp_content_ext,
            const axutil_env_t *env,
            xml_schema_particle_t *particle);
                  
    axis2_char_t* (AXIS2_CALL *
    to_string)(
            void *cmp_content_ext,
            const axutil_env_t *env,
            axis2_char_t *prefix,
            int tab);
    
};

struct xml_schema_complex_content_extension
{
    xml_schema_annotated_t base;
    xml_schema_complex_content_extension_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_EXTERN xml_schema_complex_content_extension_t * AXIS2_CALL
xml_schema_complex_content_extension_create(const axutil_env_t *env);

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_FREE(cmp_content_ext, env) \
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->free(cmp_content_ext, env))

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_BASE_IMPL(cmp_content_ext, env) \
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->\
          get_base_impl(cmp_content_ext, env))

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_TYPE(cmp_content_ext, env) \
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->\
          get_type(cmp_content_ext, env))

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SUPER_OBJS(cmp_content_ext, env) \
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->\
          super_objs(cmp_content_ext, env))


#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_ANY_ATTRIBUTE(cmp_content_ext, env) \
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->get_any_attribute(cmp_content_ext, env))

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_ANY_ATTRIBUTE(cmp_content_ext, \
         env, any_attr)\
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->\
      set_any_attribute(cmp_content_ext, env, any_attr ))

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_ATTRIBUTES(cmp_content_ext, env) \
      (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->get_attributes(cmp_content_ext, env))
            
#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_BASE_TYPE_NAME(cmp_content_ext, env)\
        (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->get_base_type_name(cmp_content_ext, env))            

#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_BASE_TYPE_NAME(cmp_content_ext,\
         env, base_type_name) \
        (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->set_base_type_name(cmp_content_ext, env, base_type_name))
            
#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_PARTICLE(cmp_content_ext, env) \
        (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->get_particle(cmp_content_ext, env))       
 
#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_PARTICLE(cmp_content_ext, env, particle) \
        (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->set_particle(cmp_content_ext, env, particle)) 
        
#define XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_TO_STRING(cmp_content_ext, env, prefix, tab) \
        (((xml_schema_complex_content_extension_t *)cmp_content_ext)->ops->to_string(cmp_content_ext, env, prefix, tab))                     
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_H */
