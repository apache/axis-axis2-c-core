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

#ifndef XML_SCHEMA_GROUP_BASE_H
#define XML_SCHEMA_GROUP_BASE_H

/**
 * @file xml_schema_group_base.h
 * @brief Axis2 Xml Schema Group Base Interface
 *          An abstract class for xml_schema_choice, xml_schema_all, or 
 *          xml_schema_sequence.
 *
 */

#include <xml_schema_defines.h>
#include <xml_schema_particle.h>
#include <xml_schema_obj_collection.h>

/** @defgroup xml_schema_group_base Xml Schema Group Base
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_group_base 
                    xml_schema_group_base_t;
typedef struct xml_schema_group_base_ops 
                    xml_schema_group_base_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_group_base_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *group_base,
            const axutil_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *group_base,
            const axutil_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (void *group_base,
            const axutil_env_t *env);

    xml_schema_particle_t *(AXIS2_CALL *
    get_base_impl) (void *group_base,
                    const axutil_env_t *env);
    
    xml_schema_obj_collection_t *(AXIS2_CALL *
    get_items)(void *group_base,
                    const axutil_env_t *env);
    /**
     * only valid if the type is XML_SCHEMA_SEQUENCE
     */
    axis2_status_t (AXIS2_CALL *
    to_string)(void *group_base,
               const axutil_env_t *env,
               axis2_char_t *prefix,
               int tab);
               

};

struct xml_schema_group_base
{
    xml_schema_particle_t base;
    xml_schema_group_base_ops_t *ops;
};


AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_group_base_create(const axutil_env_t *env);

AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_all_create(const axutil_env_t *env);

AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_sequence_create(const axutil_env_t *env);

AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_choice_create(const axutil_env_t *env);

/********************** macros ***********************************************/
#define XML_SCHEMA_GROUP_BASE_FREE(group_base, env) \
      (((xml_schema_group_base_t *) group_base)->ops->free(group_base, env))

#define XML_SCHEMA_GROUP_BASE_SUPER_OBJS(group_base, env) \
      (((xml_schema_group_base_t *) group_base)->ops->super_objs(group_base, env))

#define XML_SCHEMA_GROUP_BASE_GET_TYPE(group_base, env) \
      (((xml_schema_group_base_t *) group_base)->ops->get_type(group_base, env))

#define XML_SCHEMA_GROUP_BASE_GET_BASE_IMPL(group_base, env) \
      (((xml_schema_group_base_t *) group_base)->ops->get_base_impl(group_base, env))

#define XML_SCHEMA_GROUP_BASE_GET_ITEMS(group_base, env) \
      (((xml_schema_group_base_t *) group_base)->ops->get_items(group_base, env))

#define XML_SCHEMA_GROUP_BASE_TO_STRING(group_base, env, prefix, tab) \
        (((xml_schema_group_base_t *) group_base)->ops->to_string(group_base, \
            env, prefix, tab))

/*********************** end macros ********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_GROUP_BASE_H */
