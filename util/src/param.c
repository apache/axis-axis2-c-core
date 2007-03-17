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

#include <axis2_param.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include <axis2_generic_obj.h>

struct axis2_param
{
    /** Parameter name */
    axis2_char_t *name;
    /** Parameter value */
    void* value;
    /** Parameter locked? */
    axis2_bool_t locked;
    /** Parameter type */
    int type; /*default is AXIS2_TEXT_PARAM */
    axis2_hash_t *attrs;
    axis2_array_list_t *value_list;
    axis2_status_t (AXIS2_CALL *
    value_free) (void *param_value,
        const axis2_env_t *env);
};

AXIS2_EXTERN axis2_param_t* AXIS2_CALL
axis2_param_create(const axis2_env_t *env,
    axis2_char_t *name, 
    void *value)
{
    axis2_param_t *param = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    param = AXIS2_MALLOC(env->allocator, sizeof(axis2_param_t));
    if (!param)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);
        return NULL;
    }

    param->name = axis2_strdup(name, env);
    param->value = value; /* shallow copy.*/
    param->locked = AXIS2_FALSE;
    param->type = AXIS2_TEXT_PARAM;
    param->attrs = NULL;
    param->value_list = NULL;
    param->value_free = NULL;

    return param;
}

axis2_char_t* AXIS2_CALL
axis2_param_get_name(axis2_param_t *param,
    const axis2_env_t *env)
{
    return param->name;
}

void* AXIS2_CALL
axis2_param_get_value(axis2_param_t *param,
    const axis2_env_t *env)
{
    return param->value;
}

axis2_status_t AXIS2_CALL
axis2_param_set_name(axis2_param_t *param,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param->name = axis2_strdup(name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_param_set_value(axis2_param_t *param,
    const axis2_env_t *env,
    const void *value)
{
    void *param_value = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param_value = axis2_param_get_value(param, env);
    if (param_value)
    {
        if (param && param->value_free)
        {
            param->value_free(param_value, env);
        }
        else /* we assume that param value is axis2_char_t* */
        {
            AXIS2_FREE(env->allocator, param_value);
        }
    }
    param->value = (void *)value;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_param_is_locked(axis2_param_t *param,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return param->locked;
}

axis2_status_t AXIS2_CALL
axis2_param_set_locked(axis2_param_t *param,
    const axis2_env_t *env,
    axis2_bool_t value)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param->locked = value;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_param_get_param_type(axis2_param_t *param,
    const axis2_env_t *env)
{
    return param->type;
}

axis2_status_t AXIS2_CALL
axis2_param_set_param_type(axis2_param_t *param,
    const axis2_env_t *env,
    int type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param->type = type;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_param_set_attributes(axis2_param_t *param,
    const axis2_env_t *env,
    axis2_hash_t *attrs)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attrs, AXIS2_FAILURE);

    if (param->attrs)
    {
        axis2_hash_index_t *i = NULL;
        void *v = NULL;

        for (i = axis2_hash_first(param->attrs, env); i;
            i = axis2_hash_next(env, i))
        {
            axis2_hash_this(i, NULL, NULL, &v);
            axis2_generic_obj_free(v, env);
        }
        axis2_hash_free(param->attrs, env);
    }

    param->attrs = attrs;
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
axis2_param_get_attributes(axis2_param_t *param,
    const axis2_env_t *env)
{
    return param->attrs;
}

axis2_status_t AXIS2_CALL
axis2_param_set_value_list(axis2_param_t *param,
    const axis2_env_t *env,
    axis2_array_list_t *value_list)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value_list, AXIS2_FAILURE);

    if (param->value_list)
    {
        int i = 0, size = 0;

        size = axis2_array_list_size(param->value_list, env);
        for (i = 0; i < size; i++)
        {
            axis2_param_t *param = NULL;

            param = (axis2_param_t *) axis2_array_list_get(
                param->value_list, env, i);
            axis2_param_free(param, env);
        }
        axis2_array_list_free(param->value_list, env);
    }
    param->value_list = value_list;

    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
axis2_param_get_value_list(axis2_param_t *param,
    const axis2_env_t *env)
{
    return param->value_list;
}

axis2_status_t AXIS2_CALL
axis2_param_free(axis2_param_t *param,
    const axis2_env_t *env)
{
    void *param_value = NULL;
    axis2_char_t *param_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param_value = axis2_param_get_value(param, env);
    if (param_value)
    {
        if (param && param->value_free)
        {
            param->value_free(param_value, env);
        }
        else /* we assume that param value is axis2_char_t* */
        {
            AXIS2_FREE(env->allocator, param_value);
        }
    }

    if (param->attrs)
    {
        axis2_hash_index_t *i = NULL;
        void *v = NULL;

        for (i = axis2_hash_first(param->attrs, env); i;
            i = axis2_hash_next(env, i))
        {
            axis2_hash_this(i, NULL, NULL, &v);
            axis2_generic_obj_free(v, env);
        }
        axis2_hash_free(param->attrs, env);
    }

    if (param->value_list)
    {
        int i = 0, size = 0;

        size = axis2_array_list_size(param->value_list, env);
        for (i = 0; i < size; i++)
        {
            axis2_param_t *param_l = NULL;

            param_l = (axis2_param_t *) axis2_array_list_get(
                param->value_list, env, i);
            if (param_l)
            {
                axis2_param_free(param_l, env);
            }
        }
        axis2_array_list_free(param->value_list, env);
    }
    param_name = axis2_param_get_name(param, env);
    AXIS2_FREE(env->allocator, param_name);
    AXIS2_FREE(env->allocator, param);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_param_set_value_free(axis2_param_t *param,
    const axis2_env_t *env,
    void *free_fn)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param->value_free = free_fn;

    return AXIS2_SUCCESS;
}

