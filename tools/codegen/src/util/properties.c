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
 
#include <w2c_properties.h>
#include <axutil_array_list.h>
#include <axutil_string.h>
#include <w2c_string.h>
#include <axutil_properties.h>

/** 
 * @brief
 */
typedef struct w2c_properties_impl
{
    w2c_properties_t properties;
    
    axutil_hash_t *prop_hash;
       
} w2c_properties_impl_t;

#define W2C_INTF_TO_IMPL(properties) \
    ((w2c_properties_impl_t *) properties)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_properties_free (w2c_properties_t *properties, 
           const axutil_env_t *env);

axutil_hash_t* AXIS2_CALL
w2c_properties_get_hash(w2c_properties_t *properties,
           const axutil_env_t *env);

axis2_char_t*
w2c_properties_trunk_and_dup( axis2_char_t *start, axis2_char_t *end,
                                   const axutil_env_t *env );
/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_properties_t * AXIS2_CALL 
w2c_properties_create (const axutil_env_t *env,
                        axis2_char_t *filename,
                        axis2_char_t seperator)
{
    w2c_properties_impl_t *properties_impl = NULL;
    axutil_properties_t *main_props = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axutil_hash_index_t *hi = NULL;
    axis2_char_t *p = NULL;
    axis2_char_t *tag = NULL;
    axutil_hash_t *tmp_hash = NULL;
    axutil_array_list_t *arr_list = NULL;
    int i = 0;
   

    properties_impl = (w2c_properties_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_properties_impl_t));

    if(! properties_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    main_props = axutil_properties_create(env);
    if(! main_props)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	axutil_properties_load( main_props, env, filename );

    tmp_hash = axutil_properties_get_all( main_props, env);
    
    properties_impl-> prop_hash = axutil_hash_make (env);
    if(! properties_impl-> prop_hash)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }

    if(tmp_hash)
    {
        for (hi = axutil_hash_first( tmp_hash, env);
                            hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
            if (key && value)
            {
                i = 0;
                arr_list = axutil_array_list_create(env, 10);
                for( p = value, tag = value; *p; p ++)
                {
                    if ( *p == seperator)
                    {
                        *p = '\0';
                        tag = w2c_properties_trunk_and_dup( tag, p, env);
                        axutil_array_list_add_at( arr_list, env, i, tag);
                        tag = p + 1;
                        i ++;
                    }
                }
                if ( p != tag )
                {
                    tag = w2c_properties_trunk_and_dup( tag, p, env);
                    axutil_array_list_add_at( arr_list, env, i, tag);
                }
                key = axis2_strdup(env, key);
                axutil_hash_set( properties_impl-> prop_hash, key, AXIS2_HASH_KEY_STRING, arr_list);
            }
        }
    }
    axutil_properties_free( main_props, env);


    properties_impl->properties.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_properties_ops_t));
    if(! properties_impl->properties.ops)
    {
        w2c_properties_free(&(properties_impl->properties), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    properties_impl->properties.ops->free = w2c_properties_free;
    properties_impl->properties.ops->get_hash =
                                                w2c_properties_get_hash;

    return &(properties_impl->properties);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_properties_free (w2c_properties_t *properties, 
                            const axutil_env_t *env)
{
    w2c_properties_impl_t *properties_impl = NULL;
    axutil_array_list_t *values_arr = NULL;
    axis2_char_t *value = NULL;
    axis2_char_t *key = NULL;
    int i = 0;
    int size = 0;
    axutil_hash_index_t *hi = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    properties_impl = W2C_INTF_TO_IMPL(properties);
   
    if(properties_impl-> prop_hash)
    {
        for (hi = axutil_hash_first( properties_impl-> prop_hash, env);
                            hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, (void*)&key, NULL, (void*)&values_arr);
            if (key && values_arr)
            {
                size = axutil_array_list_size( values_arr, env);
                for (i = 0; i < size; i ++)
                {
                    value = (axis2_char_t*)axutil_array_list_get( values_arr, env, i);
                    if ( value != NULL)
                    {
                        AXIS2_FREE( env-> allocator, value);
                    }
                }
                axutil_array_list_free( values_arr, env);
                AXIS2_FREE(env-> allocator, key);
            }
        }
        axutil_hash_free( properties_impl-> prop_hash, env);
    }
    
    if(properties->ops)
    {
        AXIS2_FREE(env->allocator, properties->ops);
        properties->ops = NULL;
    }
    if(properties_impl)
    {
        AXIS2_FREE(env->allocator, properties_impl);
        properties_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axutil_hash_t* AXIS2_CALL
w2c_properties_get_hash(w2c_properties_t *properties,
           const axutil_env_t *env)
{
    w2c_properties_impl_t *properties_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    properties_impl = W2C_INTF_TO_IMPL(properties);
    
    return properties_impl-> prop_hash;
}

axis2_char_t*
w2c_properties_trunk_and_dup( axis2_char_t* start, axis2_char_t* end,
                                   const axutil_env_t* env )
{
    for ( ; *start == ' '; start ++ ); /* remove front spaces */
    for ( end --; *end == ' '; end -- ); /* remove rear spaces */
    *(++end ) = '\0';
    start = (axis2_char_t*)axis2_strdup (env,  start);
    return start;
}

