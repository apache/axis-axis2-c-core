/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
  
#ifndef W2C_SCHEMA_COMPILER_H
#define W2C_SCHEMA_COMPILER_H

/**
 * @file w2c_schema_compiler.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <w2c_schema_compiler_options.h>
#include <xml_schema.h>
#include <w2c_typemapper.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_schema_compiler represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */
typedef struct w2c_schema_compiler w2c_schema_compiler_t;
typedef struct w2c_schema_compiler_ops w2c_schema_compiler_ops_t;

AXIS2_DECLARE_DATA  struct w2c_schema_compiler_ops
{
   /**
    * free w2c_schema_compiler.
    * @param  schema_compiler pointer to schema_compiler struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_schema_compiler_t *schema_compiler,
         const axis2_env_t *env);

   /**
    * compiler single schema.
    * @param  schema_compiler pointer to schema_compiler struct
    * @param  env Environment. MUST NOT be NULL
    * @param  xml_schema schema to compile
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    compile)( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                xml_schema_t *xml_schema);

   /**
    * compiles list of schemas.
    * @param  schema_compiler pointer to schema_compiler struct
    * @param  env Environment. MUST NOT be NULL
    * @param  schema_list list of schemas
    * @return result typemapper
    */
    w2c_typemapper_t *(AXIS2_CALL *
    compile_schema_list)( w2c_schema_compiler_t *compiler,
                const axis2_env_t *env,
                axis2_array_list_t *schema_list);

};

AXIS2_DECLARE_DATA  struct w2c_schema_compiler
{
     struct w2c_schema_compiler_ops *ops;
};

/**
 * create a schema_compiler obj.
 * @param env Environment. MUST NOT be NULL
 * @param options compiler options
 * @return newly created schema_compiler obj
 */
AXIS2_EXTERN w2c_schema_compiler_t* AXIS2_CALL
w2c_schema_compiler_create( const axis2_env_t *env,
                w2c_schema_compiler_options_t *options);

/*************************** Function macros **********************************/

#define W2C_SCHEMA_COMPILER_FREE(schema_compiler, env) \
      ((schema_compiler)->ops->free (schema_compiler, env)) 

#define W2C_SCHEMA_COMPILER_COMPILE_SCHEMA_LIST(schema_compiler, env, list) \
      ((schema_compiler)->ops->compile_schema_list(schema_compiler, env, list)) 

#define W2C_SCHEMA_COMPILER_COMPILE(schema_compiler, env, schema) \
      ((schema_compiler)->ops->compile(schema_compiler, env, schema)) 

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_SCHEMA_COMPILER_H */
