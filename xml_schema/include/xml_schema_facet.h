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

#ifndef XML_SCHEMA_FACET_H
#define XML_SCHEMA_FACET_H

/**
 * @file xml_schema_facet.h
 * @brief Axis2 Xml Schema Facet Interface
 *          Abstract class for all facets that are used when simple types are
 *          derived by restriction.
 */

#include <xml_schema_annotated.h>
#include <xml_schema_defines.h>
#include <axiom_element.h>
#include <axiom_node.h>
#include <axis2_hash.h>

/** @defgroup xml_schema_facet Xml Schema Facet
  * @ingroup xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_facet 
                    xml_schema_facet_t;
typedef struct xml_schema_facet_ops 
                    xml_schema_facet_ops_t;

struct xml_schema_facet_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *facet,
            const axutil_env_t *env);

    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *facet,
                    const axutil_env_t *env);
    
    xml_schema_types_t (AXIS2_CALL*
    get_type)(void *facet,
          const axutil_env_t *env);
          
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *facet,
                const axutil_env_t *env);          

    axis2_bool_t (AXIS2_CALL *
    is_fixed)(void *facet,
                const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_fixed)(void *facet,
                    const axutil_env_t *env,
                    axis2_bool_t fixed);
    
    axis2_char_t *(AXIS2_CALL *
    get_value)(void *facet,
               const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_value)(void *facet,
               const axutil_env_t *env,
               axis2_char_t *value);
    
    axis2_status_t (AXIS2_CALL *
    set_facet_type)(void *facet,
                    const axutil_env_t *env,
                    int facet_type);                    
    
};

struct xml_schema_facet
{
    xml_schema_annotated_t base;
    xml_schema_facet_ops_t *ops;
};

/**
 * Creates new Xml Schema Facet
 */
AXIS2_EXTERN xml_schema_facet_t * AXIS2_CALL
xml_schema_facet_create(const axutil_env_t *env,
                              axis2_char_t *value,
                              axis2_bool_t fixed,
                              xml_schema_types_t type);

AXIS2_EXTERN xml_schema_facet_t * AXIS2_CALL
xml_schema_facet_construct(const axutil_env_t *env,
                                 axiom_node_t *node);
                                                                 
/************************* macros *********************************************/

#define XML_SCHEMA_FACET_FREE(facet, env) \
      (((xml_schema_facet_t *) facet)->ops->\
            free(facet, env))

#define XML_SCHEMA_FACET_GET_BASE_IMPL(facet, env) \
      (((xml_schema_facet_t *) facet)->ops->\
            get_base_impl(facet, env))

#define XML_SCHEMA_FACET_GET_TYPE(facet, env) \
      (((xml_schema_facet_t *) facet)->ops->\
            get_type(facet, env))

#define XML_SCHEMA_FACET_SUPER_OBJS(facet, env) \
        (((xml_schema_facet_t *) facet)->ops->\
            super_objs(facet, env))

#define XML_SCHEMA_FACET_IS_FIXED(facet, env) \
      (((xml_schema_facet_t *) facet)->ops->\
            is_fixed(facet, env))

#define XML_SCHEMA_FACET_SET_FIXED(facet, env, fixed) \
      (((xml_schema_facet_t *) facet)->ops->\
            set_fixed(facet, env, fixed))

#define XML_SCHEMA_FACET_GET_VALUE(facet, env) \
      (((xml_schema_facet_t *) facet)->ops->\
            get_value(facet, env))

#define XML_SCHEMA_FACET_SET_VALUE(facet, env, value) \
      (((xml_schema_facet_t *) facet)->ops->\
            set_value(facet, env, value))
            
#define XML_SCHEMA_FACET_SET_FACET_TYPE(facet, env, facet_type) \
        (((xml_schema_facet_t *) facet)->ops->\
            set_facet_type(facet, env, facet_type))
            
/*************************** macros ************************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_FACET_H */
