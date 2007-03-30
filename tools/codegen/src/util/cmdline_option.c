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
 
#include <w2c_cmdline_option.h>
#include <w2c_cmdline_option_consts.h>
#include <axutil_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>

/** 
 * @brief
 */
typedef struct w2c_cmdline_option_impl
{
    w2c_cmdline_option_t cmdline_option;
    
    axis2_char_t *type;
    axutil_array_list_t *values_arr;
       
} w2c_cmdline_option_impl_t;

#define W2C_INTF_TO_IMPL(cmdline_option) \
    ((w2c_cmdline_option_impl_t *) cmdline_option)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_cmdline_option_free (w2c_cmdline_option_t *cmdline_option, 
           const axutil_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_cmdline_option_get_type(w2c_cmdline_option_t *cmdline_option,
          const axutil_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_cmdline_option_get_value(w2c_cmdline_option_t *cmdline_option,
          const axutil_env_t *env);

axutil_array_list_t* AXIS2_CALL
w2c_cmdline_option_get_values(w2c_cmdline_option_t *cmdline_option,
          const axutil_env_t *env);

/* internal functions */
axis2_char_t*
w2c_cmdline_option_set_type(
               w2c_cmdline_option_impl_t *cmdline_option_impl,
               const axutil_env_t *env,
               axis2_char_t *type );
/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_cmdline_option_t * AXIS2_CALL 
w2c_cmdline_option_create (const axutil_env_t *env)
{
    w2c_cmdline_option_impl_t *cmdline_option_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    cmdline_option_impl = (w2c_cmdline_option_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_cmdline_option_impl_t));

    if(! cmdline_option_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    cmdline_option_impl->type = NULL;
    cmdline_option_impl->values_arr = NULL;
    
    cmdline_option_impl->cmdline_option.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_cmdline_option_ops_t));
    if(! cmdline_option_impl->cmdline_option.ops)
    {
        w2c_cmdline_option_free(&(cmdline_option_impl->cmdline_option), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    cmdline_option_impl->cmdline_option.ops->free = w2c_cmdline_option_free;
    cmdline_option_impl->cmdline_option.ops->get_type =
                                                w2c_cmdline_option_get_type;
    cmdline_option_impl->cmdline_option.ops->get_value =
                                                w2c_cmdline_option_get_value;
    cmdline_option_impl->cmdline_option.ops->get_values = 
                                                w2c_cmdline_option_get_values;

    return &(cmdline_option_impl->cmdline_option);
}

AXIS2_EXTERN w2c_cmdline_option_t * AXIS2_CALL 
w2c_cmdline_option_create_with_values (const axutil_env_t *env,
                          axis2_char_t *type,
                          axutil_array_list_t *values_arr)
{
    w2c_cmdline_option_impl_t *cmdline_option_impl = NULL;
  
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, type, NULL);
    
    cmdline_option_impl = (w2c_cmdline_option_impl_t *) w2c_cmdline_option_create(env);
    if(!cmdline_option_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* populate data */
    w2c_cmdline_option_set_type(  cmdline_option_impl ,
                                env, (axis2_char_t*)axis2_strdup(env, type));
    cmdline_option_impl-> values_arr = values_arr;

    return &(cmdline_option_impl->cmdline_option);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_cmdline_option_free (w2c_cmdline_option_t *cmdline_option, 
                            const axutil_env_t *env)
{
    w2c_cmdline_option_impl_t *cmdline_option_impl = NULL;
    int i = 0;
    int size= 0;
    axis2_char_t *value = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    cmdline_option_impl = W2C_INTF_TO_IMPL(cmdline_option);
    
    if (cmdline_option_impl-> type)
    {
        AXIS2_FREE (env-> allocator, cmdline_option_impl-> type);
    }
    if( cmdline_option_impl-> values_arr )
    {
        size = axutil_array_list_size ( cmdline_option_impl-> values_arr, env );
        for (i =0; i < size; i++ )
        {
            value = axutil_array_list_get ( cmdline_option_impl-> values_arr, env, i );
            AXIS2_FREE ( env-> allocator, value );
        }
        axutil_array_list_free ( cmdline_option_impl-> values_arr, env );
    }
    if(cmdline_option->ops)
    {
        AXIS2_FREE(env->allocator, cmdline_option->ops);
        cmdline_option->ops = NULL;
    }
    if(cmdline_option_impl)
    {
        AXIS2_FREE(env->allocator, cmdline_option_impl);
        cmdline_option_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
w2c_cmdline_option_get_type(w2c_cmdline_option_t *cmdline_option,
          const axutil_env_t *env)
{
    w2c_cmdline_option_impl_t *cmdline_option_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    cmdline_option_impl = W2C_INTF_TO_IMPL(cmdline_option);
    
    return cmdline_option_impl-> type;
    
}

axis2_char_t* AXIS2_CALL
w2c_cmdline_option_get_value(w2c_cmdline_option_t *cmdline_option,
          const axutil_env_t *env)
{
    w2c_cmdline_option_impl_t *cmdline_option_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    cmdline_option_impl = W2C_INTF_TO_IMPL(cmdline_option);
    
   if ( cmdline_option_impl-> values_arr &&
           axutil_array_list_size ( cmdline_option_impl-> values_arr, env) > 0)
   {
       return axutil_array_list_get (cmdline_option_impl-> values_arr, env, 0);
   }
   return NULL;
}

axutil_array_list_t* AXIS2_CALL
w2c_cmdline_option_get_values(w2c_cmdline_option_t *cmdline_option,
          const axutil_env_t *env)
{
    w2c_cmdline_option_impl_t *cmdline_option_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    cmdline_option_impl = W2C_INTF_TO_IMPL(cmdline_option);
  
    return cmdline_option_impl-> values_arr;
 
}

/* internal functions */
axis2_char_t*
w2c_cmdline_option_set_type(
               w2c_cmdline_option_impl_t *cmdline_option_impl,
               const axutil_env_t *env,
               axis2_char_t *type )
{
    int index = 0;
    
    index = w2c_string_indexof_cs ( type, "--" );
    if ( index > -1 )
    {
        /* skip -- */
        type = (axis2_char_t*)
                 axis2_string_substring_starting_at ( type, index +2 );
    } 
    index = w2c_string_indexof_cs ( type, "-" );
    if ( index > -1 )
    {
        /* skip - */
        type = (axis2_char_t*)
                 axis2_string_substring_starting_at ( type, index +1 );
    }
    index = w2c_string_indexof_cs ( type, 
                          W2C_CMDLINE_OPTION_CONSTS_EXTRA_OPTIONTYPE_PREFIX );
     
    if (index < 0 )
    {
        type = (axis2_char_t*)axis2_string_tolower( type );
    }
    cmdline_option_impl-> type = type;
    return type;
}
