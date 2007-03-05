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

#include <axis2_desc.h>
#include <axis2_property.h>
#include <axis2_msg.h>

struct axis2_desc
{
    /** parameter container */
    axis2_param_container_t *param_container;

    /** children of this description */
    axis2_hash_t *children;
};

AXIS2_EXTERN axis2_desc_t *AXIS2_CALL
axis2_desc_create(
    const axis2_env_t *env)
{
    axis2_desc_t *desc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    desc = (axis2_desc_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_desc_t));

    if (NULL == desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc->param_container = NULL;
    desc->children = NULL;

    desc->param_container = (axis2_param_container_t *)
            axis2_param_container_create(env);
    if (NULL == desc->param_container)
    {
        axis2_desc_free(desc, env);
        return NULL;
    }

    desc->children = axis2_hash_make(env);
    if (NULL == desc->children)
    {
        axis2_desc_free(desc, env);
        return NULL;
    }

    return desc;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_desc_free(
    axis2_desc_t *desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (desc->children)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;

        for (hi = axis2_hash_first(desc->children, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
            {
                AXIS2_MSG_FREE((axis2_msg_t *)val, env);
                val = NULL;
            }
        }

        axis2_hash_free(desc->children, env);
        desc->children = NULL;
    }

    if (desc->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(desc->param_container, env);
        desc->param_container = NULL;
    }

    if (desc)
    {
        AXIS2_FREE(env->allocator, desc);
        desc = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_desc_add_param(
    axis2_desc_t *desc,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FALSE);

    return AXIS2_PARAM_CONTAINER_ADD_PARAM(desc->param_container, env, param);
}

AXIS2_EXTERN axis2_param_t *AXIS2_CALL
axis2_desc_get_param(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, NULL);
    return AXIS2_PARAM_CONTAINER_GET_PARAM(desc->param_container, env, param_name);
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_desc_get_all_params(
    const axis2_desc_t *desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, desc->param_container, AXIS2_FALSE);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(desc->param_container, env);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_desc_is_param_locked(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_param_t *param_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FALSE);

    param_l = axis2_desc_get_param(desc, env, param_name);

    return (param_l  && AXIS2_PARAM_IS_LOCKED(param_l, env));
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_desc_add_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key,
    const void *child)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    if (desc->children)
    {
        axis2_hash_set(desc->children, key,
                AXIS2_HASH_KEY_STRING, child);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_desc_get_all_children(
    const axis2_desc_t *desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return desc->children;
}

AXIS2_EXTERN void *AXIS2_CALL
axis2_desc_get_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    return axis2_hash_get(desc->children, key, AXIS2_HASH_KEY_STRING);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_desc_remove_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    if (desc->children)
    {
        axis2_hash_set(desc->children, key,
                AXIS2_HASH_KEY_STRING, NULL);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
