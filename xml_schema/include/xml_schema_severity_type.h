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

#ifndef XML_SCHEMA_SEREVITY_TYPE_H
#define XML_SCHEMA_SEREVITY_TYPE_H

/**
 * @file axis2_xml_severity_type.h
 * @brief Axis2 Xml Schema Form Interface
 *          Indicates if attributes or elements need to be qualified or left 
 *          unqualified.
 *
 */

#include <xml_schema_enum.h>

/** @defgroup axis2_xml_severity_type Xml Schema Form
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_severity_type
                    xml_schema_severity_type_t;
typedef struct xml_schema_severity_type_ops 
                    xml_schema_severity_type_ops_t;

#define XML_SCHEMA_ERROR "error"
#define XML_SCHEMA_WARNING "warning"

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_severity_type_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *sevetiry,
            const axutil_env_t *env);

    xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *severity,
                    const axutil_env_t *env);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_values)(void *severity,
                const axutil_env_t *env);
                
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *severity,
                const axutil_env_t *env);
                
    xml_schema_types_t (AXIS2_CALL*
    get_type)(void *severity,
          const axutil_env_t *env);                              
    
};

struct xml_schema_severity_type
{
    xml_schema_enum_t base;
    xml_schema_severity_type_ops_t *ops;
};

AXIS2_EXTERN xml_schema_severity_type_t * AXIS2_CALL
axis2_xml_severity_type_create(const axutil_env_t *env,
                                    axis2_char_t* value);

/***************************** Macros ******************************************/

#define XML_SCHEMA_SEREVITY_TYPE_FREE(severity, env) \
      (((xml_schema_severity_type_t *) severity)->ops->free(severity, env))

#define XML_SCHEMA_SEREVITY_TYPE_GET_BASE_IMPL(severity, env) \
      (((xml_schema_severity_type_t *) severity)->ops->get_base_impl(severity, env))

#define XML_SCHEMA_SEREVITY_TYPE_GET_VALUES(severity, env) \
      (((xml_schema_severity_type_t *) severity)->ops->values(severity, env))

#define XML_SCHEMA_SEREVITY_TYPE_GET_TYPE(severity, env) \
      (((xml_schema_severity_type_t *) severity)->ops->get_type(severity, env))

#define XML_SCHEMA_SEREVITY_TYPE_SUPER_OBJS(severity, env) \
      (((xml_schema_severity_type_t *) severity)->ops->super_objs(severity, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_SEREVITY_TYPE_H */
