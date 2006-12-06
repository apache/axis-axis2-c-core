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
 
#ifndef XML_SCHEMA_INPUT_SOURCE_H 
#define XML_SCHEMA_INPUT_SOURCE_H
 
/**
 * @file xml_schema_input_source.h
 */ 

/** @defgroup xml_schema
  * @ingroup xml_schema
  * @{
  */ 

typedef struct xml_schema_input_source 
                xml_schema_input_source_t;
                
typedef struct xml_schema_input_source_ops
                xml_schema_input_source_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_input_source_ops
{
    axis2_char_t* (AXIS2_CALL *
    get_system_id)(
            xml_schema_input_source_t *source,
            const axis2_env_t *env);
                   
    axis2_status_t (AXIS2_CALL *                   
    set_system_id)(
            xml_schema_input_source_t *source,
            const axis2_env_t *env,
            const axis2_char_t *system_id);
            
    axis2_char_t* (AXIS2_CALL*
    get_public_id)(
            xml_schema_input_source_t *source,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL*
    set_public_id)(
            xml_schema_input_source_t *source,
            const axis2_env_t *env,
            const axis2_char_t *public_id);
            
    axis2_status_t (AXIS2_CALL*
    set_encoding)(
            xml_schema_input_source_t *source,
            const axis2_env_t *env,
            const axis2_char_t *encoding);
            
    axis2_char_t * (AXIS2_CALL*
    get_encoding)(
            xml_schema_input_source_t *source,
            const axis2_env_t *env);
                                        
    axis2_status_t (AXIS2_CALL *
    free)(xml_schema_input_source_t *source,
          const axis2_env_t *env);
};

struct xml_schema_input_source
{
    xml_schema_input_source_ops_t *ops;
};

AXIS2_EXTERN xml_schema_input_source_t* AXIS2_CALL
xml_schema_input_source_create(const axis2_env_t *env);

AXIS2_EXTERN xml_schema_input_source_t* AXIS2_CALL
xml_schema_input_source_create_with_system_id(
        const axis2_env_t *env,
        const axis2_char_t *id);
                                        
/************ Macros ************************************/

#define XML_SCHEMA_INPUT_SOURCE_FREE(source, env) \
        ((source)->ops->free(source, env))
        
#define XML_SCHEMA_INPUT_SOURCE_SET_PUBLIC_ID(source, env, id) \
        ((source)->ops->set_public_id(source, env, id))
        
#define XML_SCHEMA_INPUT_SOURCE_GET_PUBLIC_ID(source, env) \
        ((source)->ops->get_public_id(source, env)) 
        
#define XML_SCHEMA_INPUT_SOURCE_SET_SYSTEM_ID(source, env, id) \
        ((source)->ops->set_system_id(source, env, id))
        
#define XML_SCHEMA_INPUT_SOURCE_GET_SYSTEM_ID(source, env) \
        ((source)->ops->get_system_id(source, env))
        
#define XML_SCHEMA_INPUT_SOURCE_SET_ENCODING(source, env, encoding) \
        ((source)->ops->set_encoding(source, env, encoding))
        
#define XML_SCHEMA_INPUT_SOURCE_GET_ENCODING(source, env) \
        ((source)->ops->get_encoding(source, env))
        
#ifdef __cplusplus
}
#endif
/** @} */
#endif /* XML_SCHEMA_INPUT_SOURCE_H */

