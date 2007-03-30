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
  
#ifndef W2C_CMDLINE_OPTION_H
#define W2C_CMDLINE_OPTION_H

/**
 * @file w2c_cmdline_option.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axutil_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_cmdline_option represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_cmdline_option w2c_cmdline_option_t;
typedef struct w2c_cmdline_option_ops w2c_cmdline_option_ops_t;

AXIS2_DECLARE_DATA  struct w2c_cmdline_option_ops
{
   /**
    * free w2c_cmdline_option.
    * @param  cmdline_option pointer to cmdline_option struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */

    axis2_status_t (AXIS2_CALL *
    free)(w2c_cmdline_option_t *cmdline_option,
         const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_type)(w2c_cmdline_option_t *cmdline_option,
          const axis2_env_t *env);
    
    axis2_char_t *(AXIS2_CALL *
    get_value)(w2c_cmdline_option_t *cmdline_option,
          const axis2_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_values)(w2c_cmdline_option_t *cmdline_option,
          const axis2_env_t *env);
    
};

AXIS2_DECLARE_DATA  struct w2c_cmdline_option
{
     struct w2c_cmdline_option_ops *ops;
};

/**
 * create a cmdline_option obj
 * @param env Environment. MUST NOT be NULL
 * @param type type of the cmdline argument
 * @param values values of the cmdline argument
 * @return newly created cmdline_option obj
 */
AXIS2_EXTERN w2c_cmdline_option_t* AXIS2_CALL
w2c_cmdline_option_create_with_values( const axis2_env_t *env,
                   axis2_char_t *type,
                   axutil_array_list_t *values_arr );

/*************************** Function macros **********************************/

#define W2C_CMDLINE_OPTION_FREE(cmdline_option, env) \
      ((cmdline_option)->ops->free (cmdline_option, env)) 

#define W2C_CMDLINE_OPTION_GET_TYPE(cmdline_option, env) \
      ((cmdline_option)->ops->get_type (cmdline_option, env ))

#define W2C_CMDLINE_OPTION_GET_VALUE(cmdline_option, env) \
      ((cmdline_option)->ops->get_value (cmdline_option, env ))

#define W2C_CMDLINE_OPTION_GET_VALUES(cmdline_option, env) \
      ((cmdline_option)->ops->get_values (cmdline_option, env))


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_CMDLINE_OPTION_H */
