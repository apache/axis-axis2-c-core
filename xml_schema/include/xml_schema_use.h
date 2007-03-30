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

#ifndef XML_SCHEMA_USE_H
#define XML_SCHEMA_USE_H

/**
 * @file xml_schema_use.h
 * @brief Axis2 Xml Schema Use Interface
 *          Indicator of how the attribute is used.
 *
 */

#include <xml_schema_defines.h>
#include <xml_schema_enum.h>

/** @defgroup xml_schema_use Xml Schema Use
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_use xml_schema_use_t;
typedef struct xml_schema_use_ops xml_schema_use_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_use_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *use,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (
            void *use,
            const axutil_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *use,
            const axutil_env_t *env);

    xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *use,
                    const axutil_env_t *env);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_values)(void *use,
                const axutil_env_t *env);
    
};

struct xml_schema_use
{
    xml_schema_enum_t base;
    xml_schema_use_ops_t *ops;
};

AXIS2_EXTERN xml_schema_use_t * AXIS2_CALL
xml_schema_use_create(const axutil_env_t *env,
                                    axis2_char_t* value);

/******************* macros ***************************************/

#define XML_SCHEMA_USE_FREE(use, env) \
      (((xml_schema_use_t *) use)->ops->free(use, env))

#define XML_SCHEMA_USE_SUPER_OBJS(use, env) \
      (((xml_schema_use_t *) use)->ops->super_objs(use, env))

#define XML_SCHEMA_USE_GET_TYPE(use, env) \
      (((xml_schema_use_t *) use)->ops->get_type(use, env))

#define XML_SCHEMA_USE_GET_BASE_IMPL(use, env) \
      (((xml_schema_use_t *) use)->ops->get_base_impl(use, env))

#define XML_SCHEMA_USE_GET_VALUES(use, env) \
      (((xml_schema_use_t *) use)->ops->values(use, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_USE_H */
