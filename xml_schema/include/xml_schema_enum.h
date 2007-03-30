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

#ifndef XML_SCHEMA_ENUM_H
#define XML_SCHEMA_ENUM_H

/**
 * @file xml_schema_enum.h
 * @brief Axis2 Xml Schema Enum  interface
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils_defines.h>
#include <axutil_array_list.h>
#include <axutil_hash.h>
#include <xml_schema_defines.h>
#include <xml_schema_constants.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_enum xml_schema_enum_t;
typedef struct xml_schema_enum_ops xml_schema_enum_ops_t;

/** @defgroup xml_schema_enum Xml Schema Enum
  * @ingroup xml_schema
  * @{
  */

struct xml_schema_enum_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(
            void *schema_enum,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs)(
            void *schema_enum,
            const axutil_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *schema_enum,
            const axutil_env_t *env);

    axis2_char_t* (AXIS2_CALL *
    get_value)(
            void *schema_enum,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_value)(
            void *schema_enum,
            const axutil_env_t *env,
            const axis2_char_t *value);

    axis2_bool_t (AXIS2_CALL *
    equals)(void *schema_enum,
            const axutil_env_t *env,
            void *obj);

    axutil_array_list_t* (AXIS2_CALL *
    get_values)(
            void *schema_enum, 
            const axutil_env_t *env);
    
};

struct xml_schema_enum
{
    xml_schema_enum_ops_t *ops;
};

AXIS2_EXTERN xml_schema_enum_t * AXIS2_CALL
xml_schema_enum_create(const axutil_env_t *env,
                       const axis2_char_t *value);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_enum_resolve_methods(xml_schema_enum_t *schema_enum,
                                const axutil_env_t *env,
                                xml_schema_enum_t *schema_enum_impl,
                                XML_SCHEMA_SUPER_OBJS_FN super_objs,
                                XML_SCHEMA_GET_TYPE_FN get_type,
                                XML_SCHEMA_FREE_FN free_fn);

AXIS2_EXTERN int AXIS2_CALL
xml_schema_enum_index(axis2_char_t *value,
                            const axutil_env_t *env,
                            axutil_array_list_t *values);
                            
/***************** macros ****************************************************/                           

#define XML_SCHEMA_ENUM_FREE(schema_enum, env) \
      (((xml_schema_enum_t *) schema_enum)->ops->free (schema_enum, env))

#define XML_SCHEMA_ENUM_SUPER_OBJS(schema_enum, env) \
      (((xml_schema_enum_t *) schema_enum)->ops->super_objs (schema_enum, env))

#define XML_SCHEMA_ENUM_GET_TYPE(schema_enum, env) \
      (((xml_schema_enum_t *) schema_enum)->ops->get_type(schema_enum, env))

#define XML_SCHEMA_ENUM_GET_VALUE(schema_enum, env) \
      (((xml_schema_enum_t *) schema_enum)->ops->get_value \
         (schema_enum, env))

#define XML_SCHEMA_ENUM_SET_VALUE(schema_enum, env, value) \
      (((xml_schema_enum_t *) schema_enum)->ops->set_value \
         (schema_enum, env, value))

#define XML_SCHEMA_ENUM_EQUALS(schema_enum, env) \
      (((xml_schema_enum_t *) schema_enum)->ops->equals (schema_enum, env))

#define XML_SCHEMA_ENUM_GET_VALUES(schema_enum, env) \
      (((xml_schema_enum_t *) schema_enum)->ops->get_values \
         (schema_enum, env))

/**************** end macros *************************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_ENUM_H */
