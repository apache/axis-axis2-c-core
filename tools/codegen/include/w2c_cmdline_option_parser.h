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
  
#ifndef W2C_CMDLINE_OPTION_PARSER_H
#define W2C_CMDLINE_OPTION_PARSER_H

/**
 * @file w2c_cmdline_option_parser.h
 * @brief parse command line options
 */

#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_cmdline_option_parser parse command line options
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_cmdline_option_parser w2c_cmdline_option_parser_t;
typedef struct w2c_cmdline_option_parser_ops w2c_cmdline_option_parser_ops_t;

  struct w2c_cmdline_option_parser_ops
{
   /**
    * free w2c_cmdline_option_parser.
    * @param  cmdline_option_parser pointer to cmdline_option_parser struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_cmdline_option_parser_t *cmdline_option_parser,
         const axis2_env_t *env);

   /**
    * retrieve all options hash.
    * @param  cmdline_option_parser pointer to cmdline_option_parser struct
    * @param  env Environment. MUST NOT be NULL
    * @return hash of options
    */
    axis2_hash_t* (AXIS2_CALL *
    get_options)(w2c_cmdline_option_parser_t *cmdline_option_parser,
          const axis2_env_t *env);
    
   /**
    * retrieve list of invalid optoins.
    * @param  cmdline_option_parser pointer to cmdline_option_parser struct
    * @param  env Environment. MUST NOT be NULL
    * @return list of invalid options
    */
    axis2_array_list_t* (AXIS2_CALL *
    get_invalid_options)(w2c_cmdline_option_parser_t *cmdline_option_parser,
          const axis2_env_t *env);

};

  struct w2c_cmdline_option_parser
{
    struct w2c_cmdline_option_parser_ops *ops;
};

/**
 * create a cmdline_option_parser obj.
 * @param env Environment. MUST NOT be NULL
 * @param argc length of the array
 * @param argv string arrays of args
 * @return newly created cmdline_option_parser obj
 */
AXIS2_EXTERN w2c_cmdline_option_parser_t* AXIS2_CALL
w2c_cmdline_option_parser_create(const axis2_env_t *env,
                   int argc,
                   axis2_char_t **argv);

/*************************** Function macros **********************************/

#define W2C_CMDLINE_OPTION_PARSER_FREE(cmdline_option_parser, env) \
      ((cmdline_option_parser)->ops->free (cmdline_option_parser, env))

#define W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(cmdline_option_parser, env) \
      ((cmdline_option_parser)->ops->get_options (cmdline_option_parser, env ))

#define W2C_CMDLINE_OPTION_PARSER_GET_INVALID_OPTIONS(cmdline_option_parser, env) \
      ((cmdline_option_parser)->ops->get_invalid_options(cmdline_option_parser, env ))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_CMDLINE_OPTION_PARSER_H */
