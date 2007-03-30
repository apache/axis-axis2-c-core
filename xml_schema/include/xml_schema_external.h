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

#ifndef XML_SCHEMA_EXTERNAL_H
#define XML_SCHEMA_EXTERNAL_H

/**
 * @file xml_schema_external.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all external types.
 */

#include <xml_schema_annotated.h>
#include <axutil_hash.h>

/** @defgroup xml_schema_external Xml Schema Particle
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_external 
                    xml_schema_external_t;
typedef struct xml_schema_external_ops 
                    xml_schema_external_ops_t;
                    
struct xml_schema;                    

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_external_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *external,
            const axutil_env_t *env);
            
    xml_schema_annotated_t* (AXIS2_CALL *
    get_base_impl)(
            void *external,
            const axutil_env_t *env);            

    xml_schema_types_t (AXIS2_CALL *
    get_type)(
            void *external,
            const axutil_env_t *env);            


    axutil_hash_t* (AXIS2_CALL *
    super_objs)(
            void *external,
            const axutil_env_t *env);            

    struct xml_schema* (AXIS2_CALL *
    get_schema)(
            void *external,
            const axutil_env_t *env);
                
    axis2_status_t (AXIS2_CALL *
    set_schema)(
            void *external,
            const axutil_env_t *env,
            struct xml_schema* schema);
                
    axis2_char_t* (AXIS2_CALL *
    get_schema_location)(
            void *external,
            const axutil_env_t *env);
                               
    axis2_status_t (AXIS2_CALL *
    set_schema_location)(
            void *external,
            const axutil_env_t *env,
            axis2_char_t *location);

};

struct xml_schema_external
{
    xml_schema_annotated_t base;
    xml_schema_external_ops_t *ops;
};

/**
 * Creates new Xml Schema external
 */
AXIS2_EXTERN xml_schema_external_t * AXIS2_CALL
xml_schema_external_create(const axutil_env_t *env);

AXIS2_EXTERN xml_schema_external_t * AXIS2_CALL
xml_schema_include_create(const axutil_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_external_resolve_methods(
                                xml_schema_external_t *external,
                                const axutil_env_t *env,
                                xml_schema_external_t *external_impl,
                                XML_SCHEMA_SUPER_OBJS_FN super_objs,
                                XML_SCHEMA_GET_TYPE_FN get_type,
                                XML_SCHEMA_FREE_FN free_fn);


#define XML_SCHEMA_EXTERNAL_FREE(external, env) \
      (((xml_schema_external_t *) external)->ops->\
            free(external, env))

#define XML_SCHEMA_EXTERNAL_GET_BASE_IMPL(external, env) \
      (((xml_schema_external_t *) external)->ops->\
            get_base_impl(external, env))

#define XML_SCHEMA_EXTERNAL_SUPER_OBJS(external, env) \
      (((xml_schema_external_t *) external)->ops->\
            super_objs(external, env))

#define XML_SCHEMA_EXTERNAL_GET_TYPE(external, env) \
      (((xml_schema_external_t *) external)->ops->\
            get_type(external, env))

#define XML_SCHEMA_EXTERNAL_GET_SCHEMA(external, env) \
      (((xml_schema_external_t *) external)->ops->\
            get_schema(external, env))

#define XML_SCHEMA_EXTERNAL_SET_SCHEMA(external, env, sch) \
      (((xml_schema_external_t *) external)->ops->\
            set_schema(external, env, sch))
            
#define XML_SCHEMA_EXTERNAL_GET_SCHEMA_LOCATION(external, env) \
      (((xml_schema_external_t *) external)->ops->\
            get_schema_location(external, env))

#define XML_SCHEMA_EXTERNAL_SET_SCHEMA_LOCATION(external, env, location) \
      (((xml_schema_external_t *) external)->ops->\
            set_schema_location(external, env, location))            

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_EXTERNAL_H */
