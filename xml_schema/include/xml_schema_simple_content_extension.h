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

#ifndef XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_H
#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_H

/**
 * @file xml_schema_simple_content_extension.h
 * @brief Axis2 Xml Schema Group Interface
 *          Class that defines groups at the schema level that are referenced
 *          from the complex types. Groups a set of element declarations so that
 *          they can be incorporated as a sim_content_ext into complex type definitions.
 *          Represents the World Wide Web Consortium (W3C) sim_content_ext element.
 */

#include <xml_schema_annotated.h>
#include <xml_schema_obj_collection.h>
#include <xml_schema_any_attribute.h>
#include <axutil_hash.h>
#include <axis2_qname.h>
/** @defgroup xml_schema_simple_content_extension Xml Schema Group
  * @ingroup xml_schema
  * @{
  */



#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_simple_content_extension 
                xml_schema_simple_content_extension_t;
                
typedef struct xml_schema_simple_content_extension_ops 
                xml_schema_simple_content_extension_ops_t;

struct xml_schema_simple_content_extension_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *sim_content_ext,
            const axutil_env_t *env);

    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *sim_content_ext,
                    const axutil_env_t *env);
                    
    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *sim_content_ext,
          const axutil_env_t *env);
          
    axutil_hash_t* (AXIS2_CALL *
    super_objs)(void *sim_content_ext,
                const axutil_env_t *env);                              

    xml_schema_any_attribute_t* (AXIS2_CALL *
    get_any_attribute)(
            void* sim_content_ext,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_any_attribute)(
            void *sim_content_ext,
            const axutil_env_t *env,
            xml_schema_any_attribute_t *any_attr);
    
    xml_schema_obj_collection_t* (AXIS2_CALL *
    get_attributes)(
            void *sim_content_ext,
            const axutil_env_t *env);
            
    axis2_qname_t* (AXIS2_CALL*
    get_base_type_name)(
            void *sim_content_ext,
            const axutil_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_base_type_name)(
            void *sim_content_ext,
            const axutil_env_t *env,
            axis2_qname_t *qname);
            

    
};

struct xml_schema_simple_content_extension
{
    xml_schema_annotated_t base;
    xml_schema_simple_content_extension_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_EXTERN xml_schema_simple_content_extension_t * AXIS2_CALL
xml_schema_simple_content_extension_create(const axutil_env_t *env);

/************************** Macros ****************************************************/

#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_FREE(sim_content_ext, env) \
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
          free(sim_content_ext, env))

#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_BASE_IMPL(sim_content_ext, env) \
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
            get_base_impl(sim_content_ext, env))

#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_TYPE(sim_content_ext, env) \
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
            get_type(sim_content_ext, env))
            
#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_SUPER_OBJS(sim_content_ext, env) \
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
            super_objs(sim_content_ext, env))            
    
#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_ANY_ATTRIBUTE(sim_content_ext, env) \
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
          get_any_attribute(sim_content_ext, env))

#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_SET_ANY_ATTRIBUTE(sim_content_ext, env, any_attr)\
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
          set_any_attribute(sim_content_ext, env, any_attr ))

#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_ATTRIBUTES(sim_content_ext, env) \
      (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
          get_attributes(sim_content_ext, env))
            
#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_BASE_TYPE_NAME(sim_content_ext, env)\
        (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
            get_base_type_name(sim_content_ext, env))            

#define XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_SET_BASE_TYPE_NAME(sim_content_ext, \
          env, base_type_name) \
        (((xml_schema_simple_content_extension_t *)sim_content_ext)->ops->\
            set_base_type_name(sim_content_ext, env, base_type_name))
            
            
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_H */
