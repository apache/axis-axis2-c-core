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

#ifndef XML_SCHEMA_DATA_TYPE_H
#define XML_SCHEMA_DATA_TYPE_H

/**
 * @file xml_schema_data_type.h
 * @brief Axis2 Xml Schema Data Type  interface
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axutil_array_list.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_data_type xml_schema_data_type_t;
typedef struct xml_schema_data_type_ops xml_schema_data_type_ops_t;
struct xml_schema_tokenized_type;

/** @defgroup xml_schema_data_type Xml Schema Data Type
  * @ingroup xml_schema
  * @{
  */

struct xml_schema_data_type_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *data_type,
            const axutil_env_t *env);

    void * (AXIS2_CALL *
    parse_value) (void *data_type,
                    const axutil_env_t *env,
                    void *input);

    void *(AXIS2_CALL *
    value_type) (void *data_type,
                    const axutil_env_t *env);

    struct xml_schema_tokenized_type *(AXIS2_CALL *
    tokenized_type) (void *data_type,
                        const axutil_env_t *env);

};

struct xml_schema_data_type
{
    xml_schema_data_type_ops_t *ops;
};

AXIS2_EXTERN xml_schema_data_type_t * AXIS2_CALL
xml_schema_data_type_create(const axutil_env_t *env);

#define XML_SCHEMA_DATA_TYPE_FREE(data_type, env) \
      (((xml_schema_data_type_t *) data_type)->ops->free (data_type, env))

#define XML_SCHEMA_DATA_TYPE_PARSE_VALUE(data_type, env, input) \
      (((xml_schema_data_type_t *) data_type)->ops->parse_value (\
        data_type, env, input))

#define XML_SCHEMA_DATA_TYPE_VALUE_TYPE(data_type, env, line_num) \
      (((xml_schema_data_type_t *) data_type)->ops->value_type (\
        data_type, env, line_num))

#define XML_SCHEMA_DATA_TYPE_TOKENIZED_TYPE(data_type, env) \
      (((xml_schema_data_type_t *) data_type)->ops->tokenized_type (\
        data_type, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_DATA_TYPE_H */
