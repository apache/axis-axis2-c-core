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

#ifndef XML_SCHEMA_GROUP_H
#define XML_SCHEMA_GROUP_H

/**
 * @file xml_schema_group.h
 * @brief Axis2 Xml Schema Group Interface
 *          Class that defines groups at the schema level that are referenced
 *          from the complex types. Groups a set of element declarations so that
 *          they can be incorporated as a group into complex type definitions.
 *          Represents the World Wide Web Consortium (W3C) group element.
 */

#include <xml_schema_defines.h>
#include <xml_schema_annotated.h>
#include <axutil_hash.h>

/** @defgroup xml_schema_group Xml Schema Group
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_group xml_schema_group_t;
typedef struct xml_schema_group_ops xml_schema_group_ops_t;
struct xml_schema_group_base;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_group_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *group,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *group,
            const axutil_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (void *group,
            const axutil_env_t *env);

    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *group,
                    const axutil_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_name)(void *group,
                const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_name)(void *group,
                    const axutil_env_t *env,
                    axis2_char_t *name);
    
    struct xml_schema_group_base *(AXIS2_CALL *
    get_particle)(void *group,
                const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_particle)(void *group,
                    const axutil_env_t *env,
                    struct xml_schema_group_base *particle);
};

struct xml_schema_group
{
    xml_schema_annotated_t base;
    xml_schema_group_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_EXTERN xml_schema_group_t * AXIS2_CALL
xml_schema_group_create(const axutil_env_t *env);

/**************************** Macros *************************************/
#define XML_SCHEMA_GROUP_FREE(group, env) \
      (((xml_schema_group_t *) group)->ops->\
            free(group, env))

#define XML_SCHEMA_GROUP_SUPER_OBJS(group, env) \
      (((xml_schema_group_t *) group)->ops->\
            super_objs(group, env))

#define XML_SCHEMA_GROUP_GET_TYPE(group, env) \
      (((xml_schema_group_t *) group)->ops->\
            get_type(group, env))

#define XML_SCHEMA_GROUP_GET_BASE_IMPL(group, env) \
      (((xml_schema_group_t *) group)->ops->\
            get_base_impl(group, env))

#define XML_SCHEMA_GROUP_GET_NAME(group, env) \
      (((xml_schema_group_t *) group)->ops->\
            get_name(group, env))

#define XML_SCHEMA_GROUP_SET_NAME(group, env, name) \
      (((xml_schema_group_t *) group)->ops->\
            set_name(group, env, name))

#define XML_SCHEMA_GROUP_GET_PARTICLE(group, env) \
      (((xml_schema_group_t *) group)->ops->\
            get_particle(group, env))

#define XML_SCHEMA_GROUP_SET_PARTICLE(group, env, particle) \
      (((xml_schema_group_t *) group)->ops->\
            set_particle(group, env, particle))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_GROUP_H */
