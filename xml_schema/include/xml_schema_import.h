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

#ifndef XML_SCHEMA_IMPORT_H
#define XML_SCHEMA_IMPORT_H

/**
 * @file xml_schema_import.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all import types.
 */

#include <xml_schema_external.h>
#include <axis2_hash.h>

/** @defgroup xml_schema_include Xml Schema Particle
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_import 
                    xml_schema_import_t;
typedef struct xml_schema_import_ops 
                    xml_schema_import_ops_t;
                    
struct xml_schema;                    

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_import_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *import,
            const axis2_env_t *env);

    xml_schema_external_t* (AXIS2_CALL *
    get_base_impl)(void *import,
                   const axis2_env_t *env);


    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *import,
                const axis2_env_t *env);
                
    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *import,
                const axis2_env_t *env);                
    
    axis2_char_t* (AXIS2_CALL *
    get_namespace)(void *import,
                const axis2_env_t *env);
                
                
                
    axis2_status_t (AXIS2_CALL *
    set_namespace)(void *import,
                const axis2_env_t *env,
                axis2_char_t *ns);

};

struct xml_schema_import
{
    xml_schema_external_t base;
    xml_schema_import_ops_t *ops;
};

/**
 * Creates new Xml Schema import
 */
AXIS2_EXTERN xml_schema_import_t * AXIS2_CALL
xml_schema_import_create(const axis2_env_t *env);


#define XML_SCHEMA_IMPORT_FREE(import, env) \
      (((xml_schema_import_t *) import)->ops->\
            free(import, env))

#define XML_SCHEMA_IMPORT_GET_BASE_IMPL(import, env) \
      (((xml_schema_import_t *) import)->ops->\
            get_base_impl(import, env))
            
#define XML_SCHEMA_IMPORT_GET_TYPE(import, env) \
      (((xml_schema_import_t *) import)->ops->\
            get_type(import, env))
            
#define XML_SCHEMA_IMPORT_SUPER_OBJS(import, env) \
      (((xml_schema_import_t *) import)->ops->\
            super_objs(import, env))                        

#define XML_SCHEMA_IMPORT_GET_NAMESPACE(import, env) \
      (((xml_schema_import_t *) import)->ops->\
            get_namespace(import, env))

#define XML_SCHEMA_IMPORT_SET_NAMESPACE(import, env, ns) \
      (((xml_schema_import_t *) import)->ops->\
            set_namespace(import, env, ns))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_IMPORT_H */
