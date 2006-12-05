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
 
#include <w2c_properties.h>
#include <axis2_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <axis2_properties.h>

/** 
 * @brief
 */
typedef struct w2c_properties_impl
{
    w2c_properties_t properties;
    
    axis2_hash_t *prop_hash;
       
} w2c_properties_impl_t;

#define W2C_INTF_TO_IMPL(properties) \
    ((w2c_properties_impl_t *) properties)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_properties_free (w2c_properties_t *properties, 
           const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
w2c_properties_get_hash(w2c_properties_t *properties,
           const axis2_env_t *env);

axis2_char_t*
w2c_properties_trunk_and_dup( axis2_char_t *start, axis2_char_t *end,
                                   const axis2_env_t *env );
/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_properties_t * AXIS2_CALL 
w2c_properties_create (const axis2_env_t *env,
                        axis2_char_t *filename,
                        axis2_char_t seperator)
{
    w2c_properties_impl_t *properties_impl = NULL;
    axis2_properties_t *main_props = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_char_t *p = NULL;
    axis2_char_t *tag = NULL;
    axis2_hash_t *tmp_hash = NULL;
    axis2_array_list_t *arr_list = NULL;
    int i = 0;
   

    properties_impl = (w2c_properties_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_properties_impl_t));

    if(NULL == properties_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    main_props = axis2_properties_create(env);
    if(NULL == main_props)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	AXIS2_PROPERTIES_LOAD( main_props, env, filename );

    tmp_hash = AXIS2_PROPERTIES_GET_ALL( main_props, env);
    
    properties_impl-> prop_hash = axis2_hash_make (env);
    if(NULL == properties_impl-> prop_hash)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }

    if(tmp_hash)
    {
        for (hi = axis2_hash_first( tmp_hash, env);
                            hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
            if (key && value)
            {
                i = 0;
                arr_list = axis2_array_list_create(env, 10);
                for( p = value, tag = value; *p; p ++)
                {
                    if ( *p == seperator)
                    {
                        *p = '\0';
                        tag = w2c_properties_trunk_and_dup( tag, p, env);
                        AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, tag);
                        tag = p + 1;
                        i ++;
                    }
                }
                if ( p != tag )
                {
                    tag = w2c_properties_trunk_and_dup( tag, p, env);
                    AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, tag);
                }
                key = AXIS2_STRDUP(key, env);
                axis2_hash_set( properties_impl-> prop_hash, key, AXIS2_HASH_KEY_STRING, arr_list);
            }
        }
    }
    AXIS2_PROPERTIES_FREE( main_props, env);


    properties_impl->properties.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_properties_ops_t));
    if(NULL == properties_impl->properties.ops)
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
                            const axis2_env_t *env)
{
    w2c_properties_impl_t *properties_impl = NULL;
    axis2_array_list_t *values_arr = NULL;
    axis2_char_t *value = NULL;
    axis2_char_t *key = NULL;
    int i = 0;
    int size = 0;
    axis2_hash_index_t *hi = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    properties_impl = W2C_INTF_TO_IMPL(properties);
   
    if(properties_impl-> prop_hash)
    {
        for (hi = axis2_hash_first( properties_impl-> prop_hash, env);
                            hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, (void*)&key, NULL, (void*)&values_arr);
            if (key && values_arr)
            {
                size = AXIS2_ARRAY_LIST_SIZE( values_arr, env);
                for (i = 0; i < size; i ++)
                {
                    value = (axis2_char_t*)AXIS2_ARRAY_LIST_GET( values_arr, env, i);
                    if ( value != NULL)
                    {
                        AXIS2_FREE( env-> allocator, value);
                    }
                }
                AXIS2_ARRAY_LIST_FREE( values_arr, env);
                AXIS2_FREE(env-> allocator, key);
            }
        }
        axis2_hash_free( properties_impl-> prop_hash, env);
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

axis2_hash_t* AXIS2_CALL
w2c_properties_get_hash(w2c_properties_t *properties,
           const axis2_env_t *env)
{
    w2c_properties_impl_t *properties_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    properties_impl = W2C_INTF_TO_IMPL(properties);
    
    return properties_impl-> prop_hash;
}

axis2_char_t*
w2c_properties_trunk_and_dup( axis2_char_t* start, axis2_char_t* end,
                                   const axis2_env_t* env )
{
    for ( ; *start == ' '; start ++ ); /* remove front spaces */
    for ( end --; *end == ' '; end -- ); /* remove rear spaces */
    *(++end ) = '\0';
    start = (axis2_char_t*)AXIS2_STRDUP ( start, env );
    return start;
}

