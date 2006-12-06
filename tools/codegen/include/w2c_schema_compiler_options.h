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
  
#ifndef W2C_SCHEMA_COMPILER_OPTIONS_H
#define W2C_SCHEMA_COMPILER_OPTIONS_H

/**
 * @file w2c_schema_compiler_options.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axis2_hash.h>
#include <w2c_engine_configuration.h>
#include <w2c_qname2name_maker.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_schema_compiler_options represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_schema_compiler_options w2c_schema_compiler_options_t;
typedef struct w2c_schema_compiler_options_ops w2c_schema_compiler_options_ops_t;

AXIS2_DECLARE_DATA  struct w2c_schema_compiler_options_ops
{
   /**
    * free w2c_schema_compiler_options.
    * @param  schema_compiler_options pointer to schema_compiler_options struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */

    axis2_status_t (AXIS2_CALL *
    free)(w2c_schema_compiler_options_t *schema_compiler_options,
         const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_output_location)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_char_t *output_location);

    axis2_char_t* (AXIS2_CALL *
    get_output_location)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_mapper_class_package)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_char_t *mapper_class_package);

    axis2_char_t* (AXIS2_CALL *
    get_mapper_class_package)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_package_name)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_char_t *package_name);

    axis2_char_t* (AXIS2_CALL *
    get_package_name)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_helper_mode)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_bool_t helper_mode);

    axis2_bool_t (AXIS2_CALL *
    get_helper_mode)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_write_out)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_bool_t write_out);

    axis2_bool_t (AXIS2_CALL *
    get_write_out)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_wrap_classes)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_bool_t wrap_classes);

    axis2_bool_t (AXIS2_CALL *
    get_wrap_classes)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_ns2package_map)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_hash_t *ns2package_map);

    axis2_hash_t* (AXIS2_CALL *
    get_ns2package_map)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_language)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        axis2_char_t *language);

    axis2_char_t* (AXIS2_CALL *
    get_language)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_qname2name_maker)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        w2c_qname2name_maker_t *maker);

    w2c_qname2name_maker_t* (AXIS2_CALL *
    get_qname2name_maker)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_name_maker_func)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env,
        W2C_ENGINE_CONFIGURATION_NAMEMAKER maker);

    W2C_ENGINE_CONFIGURATION_NAMEMAKER (AXIS2_CALL *
    get_name_maker_func)( w2c_schema_compiler_options_t *schema_compiler_options,
        const axis2_env_t *env);



};

AXIS2_DECLARE_DATA  struct w2c_schema_compiler_options
{
     struct w2c_schema_compiler_options_ops *ops;
};

/**
 * create a schema_compiler_options obj
 * @param env Environment. MUST NOT be NULL
 * @return newly created schema_compiler_options obj
 */
AXIS2_EXTERN w2c_schema_compiler_options_t* AXIS2_CALL
w2c_schema_compiler_options_create( const axis2_env_t *env);

/*************************** Function macros **********************************/

#define W2C_SCHEMA_COMPILER_OPTIONS_FREE(schema_compiler_options, env) \
      ((schema_compiler_options)->ops->free (schema_compiler_options, env)) 

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_OUTPUT_LOCATION( schema_compiler_options, env) \
    ((schema_compiler_options)->ops->get_output_location ( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_OUTPUT_LOCATION( schema_compiler_options, env, val) \
    ((schema_compiler_options)->ops->set_output_location ( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_PACKAGE_NAME( schema_compiler_options, env) \
     ((schema_compiler_options)->ops->get_package_name( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_PACKAGE_NAME( schema_compiler_options, env, val) \
     ((schema_compiler_options)->ops->set_package_name( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_HELPER_MODE( schema_compiler_options, env) \
      ((schema_compiler_options)->ops->get_helper_mode( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_HELPER_MODE( schema_compiler_options, env, val) \
      ((schema_compiler_options)->ops->set_helper_mode( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_WRITE_OUT( schema_compiler_options, env) \
       ((schema_compiler_options)->ops->get_write_out( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_WRITE_OUT( schema_compiler_options, env, val) \
       ((schema_compiler_options)->ops->set_write_out( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_WRAP_CLASSES( schema_compiler_options, env) \
        ((schema_compiler_options)->ops->get_wrap_classes( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_WRAP_CLASSES( schema_compiler_options, env, val) \
        ((schema_compiler_options)->ops->set_wrap_classes( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_NS2PACKAGE_MAP( schema_compiler_options, env) \
         ((schema_compiler_options)->ops->get_ns2package_map( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_NS2PACKAGE_MAP( schema_compiler_options, env, val) \
         ((schema_compiler_options)->ops->set_ns2package_map( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_MAPPER_CLASS_PACKAGE( schema_compiler_options, env) \
         ((schema_compiler_options)->ops->get_mapper_class_package( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_MAPPER_CLASS_PACKAGE( schema_compiler_options, env, val) \
         ((schema_compiler_options)->ops->set_mapper_class_package( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_LANGUAGE( schema_compiler_options, env) \
         ((schema_compiler_options)->ops->get_language( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_LANGUAGE( schema_compiler_options, env, val) \
         ((schema_compiler_options)->ops->set_language( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_QNAME2NAME_MAKER( schema_compiler_options, env) \
         ((schema_compiler_options)->ops->get_qname2name_maker( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_QNAME2NAME_MAKER( schema_compiler_options, env, val) \
         ((schema_compiler_options)->ops->set_qname2name_maker( schema_compiler_options, env, val))

#define W2C_SCHEMA_COMPILER_OPTIONS_GET_NAME_MAKER_FUNC( schema_compiler_options, env) \
         ((schema_compiler_options)->ops->get_name_maker_func( schema_compiler_options, env))

#define W2C_SCHEMA_COMPILER_OPTIONS_SET_NAME_MAKER_FUNC( schema_compiler_options, env, val) \
         ((schema_compiler_options)->ops->set_name_maker_func( schema_compiler_options, env, val))


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_SCHEMA_COMPILER_OPTIONS_H */
