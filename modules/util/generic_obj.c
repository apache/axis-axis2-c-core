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
 
#include <axis2_generic_obj.h>

typedef struct axis2_generic_obj_impl
{
    axis2_generic_obj_t generic_obj;
    AXIS2_FREE_VOID_ARG free_func;
    void *value;
    
}axis2_generic_obj_impl_t;

#define AXIS2_INTF_TO_IMPL(generic_obj) \
    ((axis2_generic_obj_impl_t *) generic_obj)

axis2_status_t AXIS2_CALL
axis2_generic_obj_free (axis2_generic_obj_t *generic_obj, 
                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_generic_obj_set_free_func(axis2_generic_obj_t *generic_obj,
                            axis2_env_t **env,
                            AXIS2_FREE_VOID_ARG free_func);

axis2_status_t AXIS2_CALL
axis2_generic_obj_set_value(axis2_generic_obj_t *generic_obj,
                            axis2_env_t **env,
                            void *value);

void *AXIS2_CALL
axis2_generic_obj_get_value(axis2_generic_obj_t *generic_obj,
                            axis2_env_t **env);

/************************** End of function prototypes ************************/

axis2_generic_obj_t *AXIS2_CALL 
axis2_generic_obj_create(axis2_env_t **env)
{
    axis2_generic_obj_impl_t *generic_obj_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	generic_obj_impl = (axis2_generic_obj_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_generic_obj_impl_t));
	
	if(NULL == generic_obj_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    generic_obj_impl->value= NULL;
    generic_obj_impl->free_func = 0;
    
    generic_obj_impl->generic_obj.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_generic_obj_ops_t));
	if(NULL == generic_obj_impl->generic_obj.ops)
    {
        axis2_generic_obj_free(&(generic_obj_impl->generic_obj), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    generic_obj_impl->generic_obj.ops->free =  axis2_generic_obj_free;
    generic_obj_impl->generic_obj.ops->set_free_func = axis2_generic_obj_set_free_func;
    generic_obj_impl->generic_obj.ops->set_value = axis2_generic_obj_set_value;
	generic_obj_impl->generic_obj.ops->get_value = axis2_generic_obj_get_value;
	return &(generic_obj_impl->generic_obj);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_generic_obj_free (axis2_generic_obj_t *generic_obj, 
                    axis2_env_t **env)
{
    axis2_generic_obj_impl_t *generic_obj_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    generic_obj_impl = AXIS2_INTF_TO_IMPL(generic_obj);
    
    if(generic_obj_impl->value)
    {
        if(generic_obj_impl->free_func)
        {
            generic_obj_impl->free_func(generic_obj_impl->value, env);
        }
        else
        {
            AXIS2_FREE((*env)->allocator, generic_obj_impl->value);
        }
        generic_obj_impl->value = NULL;
    }
    
    if(generic_obj_impl->generic_obj.ops)
    {
        AXIS2_FREE((*env)->allocator, generic_obj_impl->generic_obj.ops);
        generic_obj_impl->generic_obj.ops = NULL;
    }
    
    if(generic_obj_impl)
    {
        AXIS2_FREE((*env)->allocator, generic_obj_impl);
        generic_obj_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_generic_obj_set_free_func(axis2_generic_obj_t *generic_obj,
                            axis2_env_t **env,
                            AXIS2_FREE_VOID_ARG free_func)
{
    axis2_generic_obj_impl_t *generic_obj_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    generic_obj_impl = AXIS2_INTF_TO_IMPL(generic_obj);
   
    generic_obj_impl->free_func = free_func;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_generic_obj_set_value(axis2_generic_obj_t *generic_obj,
                            axis2_env_t **env,
                            void *value)
{
    axis2_generic_obj_impl_t *generic_obj_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    generic_obj_impl = AXIS2_INTF_TO_IMPL(generic_obj);
   
    generic_obj_impl->value = value;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_generic_obj_get_value(axis2_generic_obj_t *generic_obj,
                            axis2_env_t **env)
{
    axis2_generic_obj_impl_t *generic_obj_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    generic_obj_impl = AXIS2_INTF_TO_IMPL(generic_obj);

    return generic_obj_impl->value;
}
