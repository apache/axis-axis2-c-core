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

#ifndef AXIS2_XML_SCHEMA_REDEFINE_H
#define AXIS2_XML_SCHEMA_REDEFINE_H

/**
 * @file axis2_xml_schema_redefine.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all redefine types.
 */

#include <xml_schema/axis2_xml_schema_external.h>
#include <axis2_hash.h>
#include <xml_schema/axis2_xml_schema_obj_table.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>

/** @defgroup axis2_xml_schema_include Xml Schema Particle
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_redefine 
                    axis2_xml_schema_redefine_t;
typedef struct axis2_xml_schema_redefine_ops 
                    axis2_xml_schema_redefine_ops_t;
                    
struct axis2_xml_schema;                    

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_redefine_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *redefine,
            const axis2_env_t *env);
            
    axis2_xml_schema_external_t* (AXIS2_CALL *
    get_base_impl)(void *redefine,
                   const axis2_env_t *env);            

    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *redefine,
                const axis2_env_t *env);            

    axis2_xml_schema_types_t (AXIS2_CALL *
    get_type)(void *redefine,
          const axis2_env_t *env);            

    axis2_xml_schema_obj_table_t* (AXIS2_CALL *
    get_attribute_group)(void *redefine,
                const axis2_env_t *env);
                
    axis2_status_t (AXIS2_CALL *
    set_attribute_group)(void *redefine,
                const axis2_env_t *env,
                axis2_xml_schema_obj_table_t *group);
                   
    axis2_xml_schema_obj_collection_t* (AXIS2_CALL *
    get_items)(void *redefine,
               const axis2_env_t *env);
               
    axis2_xml_schema_obj_table_t* (AXIS2_CALL *
    get_schema_types)(void *redefine,
                      const axis2_env_t *env);
                      
    axis2_xml_schema_obj_table_t* (AXIS2_CALL *
    get_group)(void *redefine, 
               const axis2_env_t *env);                      
                      
};

struct axis2_xml_schema_redefine
{
    axis2_xml_schema_external_t base;
    axis2_xml_schema_redefine_ops_t *ops;
};

/**
 * Creates new Xml Schema redefine
 */
AXIS2_EXTERN axis2_xml_schema_redefine_t * AXIS2_CALL
axis2_xml_schema_redefine_create(const axis2_env_t *env);

#define AXIS2_XML_SCHEMA_REDEFINE_FREE(redefine, env) \
      (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            free(redefine, env))

#define AXIS2_XML_SCHEMA_REDEFINE_GET_BASE_IMPL(redefine, env) \
      (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            get_base_impl(redefine, env))

#define AXIS2_XML_SCHEMA_REDEFINE_GET_TYPE(redefine, env) \
      (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            get_type(redefine, env))

#define AXIS2_XML_SCHEMA_REDEFINE_SUPER_OBJS(redefine, env) \
      (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            super_objs(redefine, env))

#define AXIS2_XML_SCHEMA_REDEFINE_GET_ATTRIBUTE_GROUP(redefine, env) \
      (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            get_attribute_group(redefine, env))

#define AXIS2_XML_SCHEMA_REDEFINE_SET_ATTRIBUTE_GROUP(redefine, env, group) \
      (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            set_attribute_group(redefine, env, group))
            
#define AXIS2_XML_SCHEMA_REDEFINE_GET_ITEMS(redefine, env) \
        (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            get_items(redefine, env))
            
#define AXIS2_XML_SCHEMA_REDEFINE_GET_GROUP(redefine, env) \
        (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            get_group(redefine, env))                       

#define AXIS2_XML_SCHEMA_REDEFINE_GET_SCHEMA_TYPES(redefine, env) \
        (((axis2_xml_schema_redefine_t *) redefine)->ops->\
            get_schema_types(redefine, env))
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_REDEFINE_H */
