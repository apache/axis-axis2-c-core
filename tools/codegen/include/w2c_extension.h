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
  
#ifndef W2C_EXTENSION_H
#define W2C_EXTENSION_H

/**
 * @file w2c_extension.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <w2c_engine_configuration.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_extension represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_extension w2c_extension_t;
typedef struct w2c_extension_ops w2c_extension_ops_t;

AXIS2_DECLARE_DATA  struct w2c_extension_ops
{
   /**
    * free w2c_extension.
    * @param  extension pointer to extension struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_extension_t *extension,
         const axis2_env_t *env);

   /**
    * engage the extension.
    * @param  extension pointer to extension struct
    * @param  env Environment. MUST NOT be NULL
    * @param  conf engine configuration 
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    engage)(w2c_extension_t *extension,
         const axis2_env_t *env,
         w2c_engine_configuration_t *conf);


};

AXIS2_DECLARE_DATA  struct w2c_extension
{
     struct w2c_extension_ops *ops;
};

/** this is an interface for extension */

/*************************** Function macros **********************************/

#define W2C_EXTENSION_FREE(extension, env) \
      ((extension)->ops->free(extension, env)) 

#define W2C_EXTENSION_ENGAGE(extension, env, conf) \
      ((extension)->ops->engage(extension, env, conf)) 

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_EXTENSION_H */
