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

#include <axis2_param.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include <axis2_generic_obj.h>

typedef struct axis2_param_impl_s
{
    /** Parameter struct */
    axis2_param_t param;
    /** Parameter name */
    axis2_char_t *name;
    /** Parameter value */
    void* value;
    /** Parameter locked? */
    axis2_bool_t locked;
    /** Parameter type */
    int type; /*default is AXIS2_TEXT_PARAM */
    axis2_hash_t *attrs;
} axis2_param_impl_t;

#define AXIS2_INTF_TO_IMPL(param) ((axis2_param_impl_t *)param)

axis2_char_t* AXIS2_CALL 
axis2_param_get_name(axis2_param_t *param, 
							const axis2_env_t *env);
	
void* AXIS2_CALL 
axis2_param_get_value(axis2_param_t *param, 
							const axis2_env_t *env);
	
axis2_status_t AXIS2_CALL 
axis2_param_set_name(axis2_param_t *param, 
							const axis2_env_t *env, 
							axis2_char_t *name);

axis2_status_t AXIS2_CALL 
axis2_param_set_value(axis2_param_t *param, 
							const axis2_env_t *env, 
							void *value);

axis2_bool_t AXIS2_CALL 
axis2_param_is_locked(axis2_param_t *param, 
							const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_param_set_locked(axis2_param_t *param, 
							const axis2_env_t *env, 
							axis2_bool_t value);

int AXIS2_CALL 
axis2_param_get_param_type(axis2_param_t *param, 
									const axis2_env_t *env);
axis2_status_t AXIS2_CALL 
axis2_param_set_param_type(axis2_param_t *param, 
									const axis2_env_t *env, 
									int type);

axis2_status_t AXIS2_CALL 
axis2_param_set_attributes(
        axis2_param_t *param,
        const axis2_env_t *env,
        axis2_hash_t *attrs);

axis2_hash_t* AXIS2_CALL 
axis2_param_get_attributes(
        axis2_param_t *param,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_param_free(axis2_param_t *param, 
						const axis2_env_t *env);

/******************************************************************************/
AXIS2_EXTERN axis2_param_t* AXIS2_CALL
axis2_param_create(const axis2_env_t *env, 
						axis2_char_t *name, void *value)
{
    axis2_param_impl_t *param_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    param_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_param_impl_t) );
    if (!param_impl)
    { 
        AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);
        return NULL;        
    }
    
    param_impl->name = AXIS2_STRDUP(name, env);
    param_impl->value = value; /* shallow copy.*/
    param_impl->locked = AXIS2_FALSE;
    param_impl->type = AXIS2_TEXT_PARAM;
    param_impl->attrs = NULL;
    
    param_impl->param.ops = 
		AXIS2_MALLOC (env->allocator, sizeof(axis2_param_ops_t));
	if(NULL == param_impl->param.ops)
    {
        axis2_param_free(&(param_impl->param), env);
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    param_impl->param.ops->value_free = NULL;
    /* initialize ops */
    param_impl->param.ops->get_name = axis2_param_get_name;
    param_impl->param.ops->get_value = axis2_param_get_value;
    param_impl->param.ops->set_name = axis2_param_set_name; 
    param_impl->param.ops->set_value = axis2_param_set_value; 
    param_impl->param.ops->is_locked = axis2_param_is_locked;
    param_impl->param.ops->set_locked = axis2_param_set_locked;
    param_impl->param.ops->get_param_type = 
		axis2_param_get_param_type;
    param_impl->param.ops->set_param_type = 
		axis2_param_set_param_type;
    param_impl->param.ops->set_attributes = 
        axis2_param_set_attributes;
    param_impl->param.ops->get_attributes = 
        axis2_param_get_attributes;
    param_impl->param.ops->free = axis2_param_free;
    
    return &(param_impl->param);
}

/******************************************************************************/

axis2_char_t* AXIS2_CALL 
axis2_param_get_name(axis2_param_t *param, 
							const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(param)->name;
}

void* AXIS2_CALL 
axis2_param_get_value(axis2_param_t *param, 
							const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(param)->value;
}

axis2_status_t AXIS2_CALL 
axis2_param_set_name(axis2_param_t *param, 
							const axis2_env_t *env, 
							axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(param)->name = AXIS2_STRDUP(name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_param_set_value(axis2_param_t *param, 
							const axis2_env_t *env, 
							void *value)
{
    void *param_value = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    param_value = axis2_param_get_value(param, env);
    if(param_value)
    {
        if(param->ops && param->ops->value_free)
        { 
            param->ops->value_free(param_value, env);
        }
        else /* we assume that param value is axis2_char_t* */
        {
            AXIS2_FREE(env->allocator, param_value);
        }
    }
    AXIS2_INTF_TO_IMPL(param)->value = value;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
axis2_param_is_locked(axis2_param_t *param, 
							const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(param)->locked;
}

axis2_status_t AXIS2_CALL 
axis2_param_set_locked(axis2_param_t *param, 
							const axis2_env_t *env, 
							axis2_bool_t value)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(param)->locked = value;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL 
axis2_param_get_param_type(axis2_param_t *param, 
									const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return AXIS2_INTF_TO_IMPL(param)->type;
}

axis2_status_t AXIS2_CALL 
axis2_param_set_param_type(axis2_param_t *param, 
									const axis2_env_t *env, 
									int type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(param)->type = type;
    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL 
axis2_param_set_attributes(
        axis2_param_t *param,
        const axis2_env_t *env,
        axis2_hash_t *attrs)
{
    axis2_param_impl_t *param_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attrs, AXIS2_FAILURE);
    param_impl = AXIS2_INTF_TO_IMPL(param);

    if(param_impl->attrs)
    {
        axis2_hash_index_t *i = NULL;
        void *v = NULL;

        for (i = axis2_hash_first (param_impl->attrs, env); i; 
                i = axis2_hash_next (env, i))
        {
            axis2_hash_this (i, NULL, NULL, &v);
            AXIS2_GENERIC_OBJ_FREE(v, env);
        }
        axis2_hash_free(param_impl->attrs, env);
    }
   
    param_impl->attrs = attrs;
    return AXIS2_SUCCESS;


}

axis2_hash_t* AXIS2_CALL 
axis2_param_get_attributes(
        axis2_param_t *param,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(param)->attrs;
}


axis2_status_t AXIS2_CALL 
axis2_param_free(axis2_param_t *param, 
						const axis2_env_t *env)
{
    axis2_param_impl_t *param_impl = NULL;
    void *param_value = NULL;
    axis2_char_t *param_name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    param_impl = AXIS2_INTF_TO_IMPL(param);
    
    param_value = axis2_param_get_value(param, env);
    if(param_value)
    {
        if(param->ops && param->ops->value_free)
        { 
            param->ops->value_free(param_value, env);
        }
        else /* we assume that param value is axis2_char_t* */
        {
            AXIS2_FREE(env->allocator, param_value);
        }
    }

    if(param_impl->attrs)
    {
        axis2_hash_index_t *i = NULL;
        void *v = NULL;

        for (i = axis2_hash_first (param_impl->attrs, env); i; 
                i = axis2_hash_next (env, i))
        {
            axis2_hash_this (i, NULL, NULL, &v);
            AXIS2_GENERIC_OBJ_FREE(v, env);
        }
        axis2_hash_free(param_impl->attrs, env);
    }
    param_name = axis2_param_get_name(param, env);
    AXIS2_FREE(env->allocator, param_name);
    AXIS2_FREE(env->allocator, param->ops);
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(param));    
    return AXIS2_SUCCESS;
}

