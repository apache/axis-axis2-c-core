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

#ifndef XML_SCHEMA_GROUP_REF_H
#define XML_SCHEMA_GROUP_REF_H

/**
 * @file xml_schema_group_ref.h
 * @brief Axis2 Xml Schema Group Ref Interface
 *          Class used within complex types that defines the reference to
 *          groups defined at the schema level. Represents the World Wide
 *          Web Consortium (W3C) group element with ref attribute.
 */

#include <xml_schema_defines.h>
#include <xml_schema_particle.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

/** @defgroup_ref xml_schema_group_ref Xml Schema Group Ref
  * @ingroup_ref xml_schema
  * @{
  */

typedef struct xml_schema_group_ref xml_schema_group_ref_t;
typedef struct xml_schema_group_ref_ops xml_schema_group_ref_ops_t;
struct xml_schema_group_base;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_group_ref_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *group_ref,
           const axutil_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *group_ref,
            const axutil_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *group_ref,
            const axutil_env_t *env);

    xml_schema_particle_t *(AXIS2_CALL *
    get_base_impl) (void *group_ref,
                    const axutil_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_ref_qname)(void *group_ref,
                    const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_ref_qname)(void *group_ref,
                    const axutil_env_t *env,
                    axis2_qname_t *ref_qname);
    
    void *(AXIS2_CALL *
    get_particle)(void *group_ref,
                  const axutil_env_t *env);
                    
    axis2_status_t (AXIS2_CALL *
    set_particle)(void *group_ref,
                  const axutil_env_t *env,
                  void *particle);
                        
    
};

struct xml_schema_group_ref
{
    xml_schema_particle_t base;
    xml_schema_group_ref_ops_t *ops;
};

/**
 * Creates new Xml Schema Group Ref
 */
AXIS2_EXTERN xml_schema_group_ref_t * AXIS2_CALL
xml_schema_group_ref_create(const axutil_env_t *env);

/******************* Macros ******************************************************/ 
#define XML_SCHEMA_GROUP_REF_FREE(group_ref, env) \
      (((xml_schema_group_ref_t *) group_ref)->ops->\
            free(group_ref, env))

#define XML_SCHEMA_GROUP_REF_SUPER_OBJS(group_ref, env) \
      (((xml_schema_group_ref_t *) group_ref)->ops->super_objs(group_ref, env))

#define XML_SCHEMA_GROUP_REF_GET_TYPE(group_ref, env) \
      (((xml_schema_group_ref_t *) group_ref)->ops->get_type(group_ref, env))

#define XML_SCHEMA_GROUP_REF_GET_BASE_IMPL(group_ref, env) \
      (((xml_schema_group_ref_t *) group_ref)->ops->\
            get_base_impl(group_ref, env))

#define XML_SCHEMA_GROUP_REF_GET_REF_QNAME(group_ref, env) \
      (((xml_schema_group_ref_t *) group_ref)->ops->\
            get_ref_qname(group_ref, env))

#define XML_SCHEMA_GROUP_REF_SET_REF_QNAME(group_ref, env, ref_qname) \
      (((xml_schema_group_ref_t *) group_ref)->ops->\
            set_ref_qname(group_ref, env, ref_qname))

#define XML_SCHEMA_GROUP_REF_GET_PARTICLE(group_ref, env) \
      (((xml_schema_group_ref_t *) group_ref)->ops->\
            get_particle(group_ref, env))

#define XML_SCHEMA_GROUP_REF_SET_PARTICLE(group_ref, env, particle) \
      (((xml_schema_group_ref_t *) group_ref)->ops->\
            set_particle(group_ref, env, particle))

/*************************** end macros ****************************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_GROUP_REF_H */
