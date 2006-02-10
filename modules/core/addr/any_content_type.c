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
 
#include <axis2_any_content_type.h>
#include <axis2_string.h>

typedef struct axis2_any_content_type_impl
{
    axis2_any_content_type_t any_content_type;    
    /** map of values in the any contetnt type  */
    axis2_hash_t *value_map;
} axis2_any_content_type_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(any_content_type) \
        ((axis2_any_content_type_impl_t *)any_content_type)

axis2_status_t AXIS2_CALL 
axis2_any_content_type_add_value(axis2_any_content_type_t *any_content_type,
                                 axis2_env_t **env, 
                                 axis2_qname_t *qname, 
                                 axis2_char_t *value); 
                                 
                                 
axis2_char_t* AXIS2_CALL 
axis2_any_content_type_get_value(axis2_any_content_type_t *any_content_type,
                                 axis2_env_t **env, 
                                 axis2_qname_t *qname);
                                 
axis2_hash_t* AXIS2_CALL 
axis2_any_content_type_get_value_map(axis2_any_content_type_t *any_content_type,
                                     axis2_env_t **env);
                                     
axis2_status_t AXIS2_CALL 
axis2_any_content_type_free (axis2_any_content_type_t *any_content_type, 
                             axis2_env_t **env);


AXIS2_DECLARE(axis2_any_content_type_t*)
axis2_any_content_type_create(axis2_env_t **env) 
{
    axis2_any_content_type_impl_t *any_content_type_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    any_content_type_impl = AXIS2_MALLOC((*env)->allocator, 
                                        sizeof(axis2_any_content_type_impl_t) );
    if (!any_content_type_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    any_content_type_impl->any_content_type.ops = NULL;
    any_content_type_impl->value_map = NULL;
    
    any_content_type_impl->value_map  = axis2_hash_make(env);
    
    if (!(any_content_type_impl->value_map ))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_any_content_type_free(&(any_content_type_impl->any_content_type), env);
        return NULL;        
    }
    
    /* initialize ops */
    any_content_type_impl->any_content_type.ops  = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_any_content_type_ops_t) );
    if (!any_content_type_impl->any_content_type.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_any_content_type_free(&(any_content_type_impl->any_content_type), env);
        return NULL;        
    }

    any_content_type_impl->any_content_type.ops->add_value = 
        axis2_any_content_type_add_value;
    any_content_type_impl->any_content_type.ops->get_value = 
        axis2_any_content_type_get_value;
    any_content_type_impl->any_content_type.ops->get_value_map = 
        axis2_any_content_type_get_value_map;
    any_content_type_impl->any_content_type.ops->free = 
        axis2_any_content_type_free;
    
    return &(any_content_type_impl->any_content_type);
}

axis2_status_t AXIS2_CALL 
axis2_any_content_type_add_value(axis2_any_content_type_t *any_content_type, 
                                 axis2_env_t **env, 
                                 axis2_qname_t *qname, 
                                 axis2_char_t *value) 
{
    axis2_any_content_type_impl_t *any_content_type_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    any_content_type_impl = AXIS2_INTF_TO_IMPL(any_content_type);
    
    if (any_content_type_impl->value_map)
    {
        axis2_char_t *name = NULL;

        name = AXIS2_QNAME_TO_STRING(qname, env);
        axis2_hash_set(any_content_type_impl->value_map, name, 
            AXIS2_HASH_KEY_STRING, value);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL 
axis2_any_content_type_get_value(axis2_any_content_type_t *any_content_type, 
                                 axis2_env_t **env, 
                                 axis2_qname_t *qname) 
{
    axis2_any_content_type_impl_t *any_content_type_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    any_content_type_impl = AXIS2_INTF_TO_IMPL(any_content_type);
    
    if (any_content_type_impl->value_map)
    {
        axis2_char_t *name = NULL;

        name = AXIS2_QNAME_TO_STRING(qname, env);
        return axis2_hash_get(any_content_type_impl->value_map, name, 
            AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

axis2_hash_t* AXIS2_CALL 
axis2_any_content_type_get_value_map(axis2_any_content_type_t *any_content_type, 
                                     axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(any_content_type)->value_map;
}

axis2_status_t AXIS2_CALL 
axis2_any_content_type_free (axis2_any_content_type_t *any_content_type, 
                             axis2_env_t **env)
{
    axis2_any_content_type_impl_t *any_content_type_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    any_content_type_impl = AXIS2_INTF_TO_IMPL(any_content_type);
    
    if (any_content_type_impl->any_content_type.ops)
    {
        AXIS2_FREE((*env)->allocator, any_content_type_impl->any_content_type.ops);
        any_content_type_impl->any_content_type.ops = NULL;
    }
    
    if (any_content_type_impl->value_map)
    {
        axis2_hash_free(any_content_type_impl->value_map, env);
        any_content_type_impl->value_map = NULL;
    }    
    
    AXIS2_FREE((*env)->allocator, any_content_type_impl);
    any_content_type_impl = NULL;
    
    return AXIS2_SUCCESS;
}
