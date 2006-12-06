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

#ifndef XML_SCHEMA_CONTENT_PROCESSING_H
#define XML_SCHEMA_CONTENT_PROCESSING_H

/**
 * @file xml_schema_content_processing.h
 * @brief Axis2 Xml Schema Content Processing Interface
 *          Indicator of how the attribute is content_processingd.
 *
 */

#include <xml_schema_defines.h>
#include <xml_schema_enum.h>

/** @defgroup xml_schema_content_processing Xml Schema Content Processing
  * @ingroup xml_schema
  * @{
  */


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct xml_schema_content_processing 
        xml_schema_content_processing_t;
        
typedef struct xml_schema_content_processing_ops 
        xml_schema_content_processing_ops_t;

struct xml_schema_content_processing_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *content_processing,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *content_processing,
            const axis2_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (void *content_processing,
            const axis2_env_t *env);

    xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *content_processing,
                    const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *content_processing,
                const axis2_env_t *env);
    
};

struct xml_schema_content_processing
{
    xml_schema_enum_t base;
    xml_schema_content_processing_ops_t *ops;
};

AXIS2_EXTERN xml_schema_content_processing_t * AXIS2_CALL
xml_schema_content_processing_create(const axis2_env_t *env,
                                     const axis2_char_t* value);

/*********************** macros ***********************************************/

#define XML_SCHEMA_CONTENT_PROCESSING_FREE(content_processing, env) \
      (((xml_schema_content_processing_t *) content_processing)->ops->\
         free(content_processing, env))

#define XML_SCHEMA_CONTENT_PROCESSING_SUPER_OBJS(content_processing, env) \
      (((xml_schema_content_processing_t *) content_processing)->ops->\
         super_objs(content_processing, env))

#define XML_SCHEMA_CONTENT_PROCESSING_GET_TYPE(content_processing, env) \
      (((xml_schema_content_processing_t *) content_processing)->ops->\
         get_type(content_processing, env))


#define XML_SCHEMA_CONTENT_PROCESSING_GET_BASE_IMPL(content_processing, env) \
      (((xml_schema_content_processing_t *) content_processing)->ops->\
         get_base_impl(content_processing, env))

#define XML_SCHEMA_CONTENT_PROCESSING_GET_VALUES(content_processing, env) \
      (((xml_schema_content_processing_t *) content_processing)->ops->\
         get_values(content_processing, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_CONTENT_PROCESSING_H */
