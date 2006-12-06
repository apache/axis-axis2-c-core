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
  
#ifndef W2C_ENGINE_H
#define W2C_ENGINE_H

/**
 * @file w2c_engine.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <w2c_cmdline_option_parser.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_engine represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_engine w2c_engine_t;
typedef struct w2c_engine_ops w2c_engine_ops_t;

AXIS2_DECLARE_DATA  struct w2c_engine_ops
{
   /**
    * free w2c_engine.
    * @param  engine pointer to engine struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_engine_t *engine,
         const axis2_env_t *env);

   /**
    * signal to generate the code.
    * @param  engine pointer to engine struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    generate)(w2c_engine_t *engine,
          const axis2_env_t *env);
    

};

AXIS2_DECLARE_DATA  struct w2c_engine
{
     struct w2c_engine_ops *ops;
};

/**
 * create a engine obj.
 * @param env Environment. MUST NOT be NULL
 * @param parser user options parser
 * @return newly created engine obj
 */
AXIS2_EXTERN w2c_engine_t* AXIS2_CALL
w2c_engine_create_with_parser( const axis2_env_t *env,
                   w2c_cmdline_option_parser_t *parser );

/*************************** Function macros **********************************/

#define W2C_ENGINE_FREE(engine, env) \
      ((engine)->ops->free (engine, env)) 

#define W2C_ENGINE_GENERATE(engine, env) \
      ((engine)->ops->generate(engine, env ))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_ENGINE_H */
