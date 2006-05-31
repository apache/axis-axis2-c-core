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
 
 /** <wsa:RelatesTo RelationshipType="..."?>xs:anyURI</wsa:RelatesTo> */

#include <axis2_relates_to.h>
#include <axis2_string.h>

typedef struct axis2_relates_to_impl
{
    axis2_relates_to_t relates_to;    
    /** value */
    axis2_char_t *value;
    /** relationship type */
    axis2_char_t *relationship_type;
} axis2_relates_to_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(relates_to) ((axis2_relates_to_impl_t *)relates_to)


axis2_char_t* AXIS2_CALL 
axis2_relates_to_get_value(struct axis2_relates_to *relates_to, 
                           const axis2_env_t *env);
                           
axis2_status_t AXIS2_CALL 
axis2_relates_to_set_value(struct axis2_relates_to *relates_to, 
                           const axis2_env_t *env, 
                           axis2_char_t * value);
                           
axis2_char_t* AXIS2_CALL 
axis2_relates_to_get_relationship_type(struct axis2_relates_to *relates_to, 
                                       const axis2_env_t *env);
                                       
axis2_status_t AXIS2_CALL 
axis2_relates_to_set_relationship_type(struct axis2_relates_to *relates_to, 
                                       const axis2_env_t *env, 
                                       axis2_char_t *relationship_type);
                                       
axis2_status_t AXIS2_CALL 
axis2_relates_to_free (struct axis2_relates_to *relates_to, 
                       const axis2_env_t *env);

axis2_relates_to_t* AXIS2_CALL 
axis2_relates_to_create(const axis2_env_t *env, 
                        axis2_char_t *value, 
                        axis2_char_t *relationship_type) 
{
    axis2_relates_to_impl_t *relates_to_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    relates_to_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_relates_to_impl_t) );
    if (!relates_to_impl)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    relates_to_impl->relates_to.ops = NULL;
    relates_to_impl->value = NULL;
    relates_to_impl->relationship_type = NULL;
    
    if (value)
    {
        relates_to_impl->value = AXIS2_STRDUP(value, env);
        if (!(relates_to_impl->value))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_relates_to_free(&(relates_to_impl->relates_to), env);
            return NULL;        
        }
    }
    
    if (relationship_type)
    {
        relates_to_impl->relationship_type = AXIS2_STRDUP(relationship_type, env);
        if (!(relates_to_impl->relationship_type))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_relates_to_free(&(relates_to_impl->relates_to), env);
            return NULL;        
        }
    }

    /* initialize ops */
    relates_to_impl->relates_to.ops  = AXIS2_MALLOC( env->allocator, sizeof(axis2_relates_to_ops_t) );
    if (!relates_to_impl->relates_to.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_relates_to_free(&(relates_to_impl->relates_to), env);
        return NULL;        
    }

    relates_to_impl->relates_to.ops->get_value = 
        axis2_relates_to_get_value;
        
    relates_to_impl->relates_to.ops->set_value = 
        axis2_relates_to_set_value;
        
    relates_to_impl->relates_to.ops->get_relationship_type = 
        axis2_relates_to_get_relationship_type;
        
    relates_to_impl->relates_to.ops->set_relationship_type = 
        axis2_relates_to_set_relationship_type;
        
    relates_to_impl->relates_to.ops->free = 
        axis2_relates_to_free;

    return &(relates_to_impl->relates_to);
}

axis2_char_t* AXIS2_CALL 
axis2_relates_to_get_value(struct axis2_relates_to *relates_to, 
                           const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(relates_to)->value;
}

axis2_status_t AXIS2_CALL 
axis2_relates_to_set_value(struct axis2_relates_to *relates_to, 
                           const axis2_env_t *env, 
                           axis2_char_t * value) 
{
    axis2_relates_to_impl_t *relates_to_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    relates_to_impl = AXIS2_INTF_TO_IMPL(relates_to);
    
    if (relates_to_impl->value)
    {
        AXIS2_FREE(env->allocator, relates_to_impl->value);
        relates_to_impl->value = NULL;
    }

    if (value)
    {
        relates_to_impl->value = (axis2_char_t*)AXIS2_STRDUP(value, env);
        if (!(relates_to_impl->value))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL 
axis2_relates_to_get_relationship_type(struct axis2_relates_to *relates_to, 
                                       const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(relates_to)->relationship_type;
}

axis2_status_t AXIS2_CALL 
axis2_relates_to_set_relationship_type(struct axis2_relates_to *relates_to, 
                                       const axis2_env_t *env, 
                                       axis2_char_t *relationship_type) 
{
    axis2_relates_to_impl_t *relates_to_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    relates_to_impl = AXIS2_INTF_TO_IMPL(relates_to);
    
    if (relates_to_impl->relationship_type)
    {
        AXIS2_FREE(env->allocator, relates_to_impl->relationship_type);
        relates_to_impl->relationship_type = NULL;
    }

    if (relationship_type)
    {
        relates_to_impl->relationship_type = 
                (axis2_char_t*)AXIS2_STRDUP(relationship_type, env);
        if (!(relates_to_impl->relationship_type))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_relates_to_free (struct axis2_relates_to *relates_to, 
                       const axis2_env_t *env)
{
    axis2_relates_to_impl_t *relates_to_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    relates_to_impl = AXIS2_INTF_TO_IMPL(relates_to);
    
    if (relates_to_impl->relates_to.ops)
    {
        AXIS2_FREE(env->allocator, relates_to_impl->relates_to.ops);
        relates_to_impl->relates_to.ops = NULL;
    }
    
    if (relates_to_impl->value)
    {
        AXIS2_FREE(env->allocator, relates_to_impl->value);
        relates_to_impl->value = NULL;
    }    
    
    if (relates_to_impl->relationship_type)
    {
        AXIS2_FREE(env->allocator, relates_to_impl->relationship_type);
        relates_to_impl->relationship_type = NULL;
    }    

    AXIS2_FREE(env->allocator, relates_to_impl);
    relates_to_impl = NULL;
    
    return AXIS2_SUCCESS;
}
