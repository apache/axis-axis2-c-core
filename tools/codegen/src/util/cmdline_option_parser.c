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
 
#include <w2c_cmdline_option_parser.h>
#include <w2c_cmdline_option_consts.h>
#include <w2c_cmdline_option.h>
#include <w2c_cmdline_option_validator.h>
#include <axutil_string.h>
#include <w2c_string.h>

/** 
 * @brief
 */
typedef struct w2c_cmdline_option_parser_impl
{
    w2c_cmdline_option_parser_t cmdline_option_parser;
    
    axutil_hash_t *hash;
       
} w2c_cmdline_option_parser_impl_t;

#define W2C_INTF_TO_IMPL(cmdline_option_parser) \
    ((w2c_cmdline_option_parser_impl_t *) cmdline_option_parser)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_cmdline_option_parser_free (
           w2c_cmdline_option_parser_t *cmdline_option_parser, 
           const axutil_env_t *env);


axutil_hash_t* AXIS2_CALL
w2c_cmdline_option_parser_get_options (
          w2c_cmdline_option_parser_t *cmdline_option_parser,
          const axutil_env_t *env);

axutil_array_list_t* AXIS2_CALL 
w2c_cmdline_option_parser_get_invalid_options (
          w2c_cmdline_option_parser_t *cmdline_option_parser,
          const axutil_env_t *env);

/** private method */
void
w2c_cmdline_option_parser_parse( w2c_cmdline_option_parser_impl_t *parser,
                          const axutil_env_t *env,
                          int argv, axis2_char_t **argc);
   

/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_cmdline_option_parser_t* AXIS2_CALL 
w2c_cmdline_option_parser_create (const axutil_env_t *env,
                                  int argc,
                                  axis2_char_t **argv)
{
    w2c_cmdline_option_parser_impl_t *cmdline_option_parser_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    cmdline_option_parser_impl = (w2c_cmdline_option_parser_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_cmdline_option_parser_impl_t));

    if(! cmdline_option_parser_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    cmdline_option_parser_impl->hash = NULL;
    
    cmdline_option_parser_impl->cmdline_option_parser.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_cmdline_option_parser_ops_t));
    if(! cmdline_option_parser_impl->cmdline_option_parser.ops)
    {
        w2c_cmdline_option_parser_free(
                     &(cmdline_option_parser_impl->cmdline_option_parser), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    w2c_cmdline_option_parser_parse
        ( cmdline_option_parser_impl, env, argc, argv );

    cmdline_option_parser_impl->cmdline_option_parser.ops->free = 
                                w2c_cmdline_option_parser_free;
    cmdline_option_parser_impl->cmdline_option_parser.ops->get_options =
                                w2c_cmdline_option_parser_get_options;
    cmdline_option_parser_impl->cmdline_option_parser.ops->get_invalid_options =
                                w2c_cmdline_option_parser_get_invalid_options;

    return &(cmdline_option_parser_impl->cmdline_option_parser);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_cmdline_option_parser_free (w2c_cmdline_option_parser_t *cmdline_option_parser, 
                            const axutil_env_t *env)
{
    w2c_cmdline_option_parser_impl_t *cmdline_option_parser_impl = NULL;
    axutil_hash_index_t *hi;
    w2c_cmdline_option_t *option = NULL;
  
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    cmdline_option_parser_impl = W2C_INTF_TO_IMPL(cmdline_option_parser);
    
    if(cmdline_option_parser->ops)
    {
        AXIS2_FREE(env->allocator, cmdline_option_parser->ops);
        cmdline_option_parser->ops = NULL;
    }
 
    if (cmdline_option_parser_impl-> hash)
    {
        for (hi = axutil_hash_first(cmdline_option_parser_impl->hash, env); hi;
                     hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, (void*)&option);
            W2C_CMDLINE_OPTION_FREE ( option, env );
        }
        axutil_hash_free ( cmdline_option_parser_impl-> hash, env );
    }

    if(cmdline_option_parser_impl)
    {
        AXIS2_FREE(env->allocator, cmdline_option_parser_impl);
        cmdline_option_parser_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axutil_hash_t* AXIS2_CALL
w2c_cmdline_option_parser_get_options (
          w2c_cmdline_option_parser_t *cmdline_option_parser,
          const axutil_env_t *env)
{
    w2c_cmdline_option_parser_impl_t *cmdline_option_parser_impl = NULL;
  
    AXIS2_ENV_CHECK(env, NULL);
    
    cmdline_option_parser_impl = W2C_INTF_TO_IMPL(cmdline_option_parser);
    return cmdline_option_parser_impl -> hash;   
}

axutil_array_list_t* AXIS2_CALL
w2c_cmdline_option_parser_get_invalid_options (
          w2c_cmdline_option_parser_t *cmdline_option_parser,
          const axutil_env_t *env)
{
    w2c_cmdline_option_parser_impl_t *cmdline_option_parser_impl = NULL;
    axutil_array_list_t *invalid_arr = NULL;
    axutil_hash_index_t *hi = NULL;
    w2c_cmdline_option_t *option = NULL;
  
    AXIS2_ENV_CHECK(env, NULL);
    
    cmdline_option_parser_impl = W2C_INTF_TO_IMPL(cmdline_option_parser);
    
    invalid_arr = axutil_array_list_create ( env, 1 );
    
    for (hi = axutil_hash_first(cmdline_option_parser_impl->hash, env); hi;
                        hi = axutil_hash_next( env, hi))
    {
         axutil_hash_this(hi, NULL, NULL, (void*)&option);
         if (w2c_cmdline_option_validator_isinvalid ( env,option ) )
         {
             axutil_array_list_add ( invalid_arr, env, option );
         }
     }
     return invalid_arr;
}

/** private method */
void
w2c_cmdline_option_parser_parse( w2c_cmdline_option_parser_impl_t *parser,
                          const axutil_env_t *env,
                          int argc, axis2_char_t **argv)
{
    int i = 0;
    w2c_cmdline_option_t *option = NULL;
    axutil_hash_t *hash;
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axutil_array_list_t *values_arr= NULL;
   
    hash = axutil_hash_make ( env );
    parser->hash = hash;
    while ( i < argc )
    {
        key = argv[i];
        values_arr = (axutil_array_list_t*)
                    axutil_array_list_create(env, 1 );
        i ++;
        while ( i < argc && !(w2c_string_indexof_cs ( argv[i], "-" ) == 0)
            && !(w2c_string_indexof_cs ( argv[i], "--" ) == 0) )
        {
            value = (axis2_char_t*)axis2_strdup(env, argv[i]);
            axutil_array_list_add ( values_arr, env, value );
            i ++;
        }
        option = (w2c_cmdline_option_t*)
           w2c_cmdline_option_create_with_values( env, key, values_arr );
        key = W2C_CMDLINE_OPTION_GET_TYPE ( option, env );
        axutil_hash_set ( hash, key, AXIS2_HASH_KEY_STRING, option );
    } 
    return;
}

