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

#include <axis2_desc.h>
#include <axis2_property.h>
#include <axis2_msg.h>
/**
 * @brief Message struct impl
 *   Axis2 Messages
 */
typedef struct axis2_desc_impl
{
    axis2_desc_t desc;

    /** parameter container */
    axis2_param_container_t *param_container;

    /** children of this description */
    axis2_hash_t *children;
}
axis2_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(desc) ((axis2_desc_impl_t *)desc)

axis2_status_t AXIS2_CALL
axis2_desc_free(
    axis2_desc_t *desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_desc_add_param(
    axis2_desc_t *desc,
    const axis2_env_t *env,
    axis2_param_t *param);

axis2_param_t *AXIS2_CALL
axis2_desc_get_param(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name);

axis2_array_list_t *AXIS2_CALL
axis2_desc_get_all_params(
    const axis2_desc_t *desc,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_desc_is_param_locked(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_desc_add_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key,
    const void* child);

axis2_hash_t *AXIS2_CALL
axis2_desc_get_all_children(
    const axis2_desc_t *desc,
    const axis2_env_t *env);

void *AXIS2_CALL
axis2_desc_get_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_desc_remove_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key);

AXIS2_EXTERN axis2_desc_t *AXIS2_CALL
axis2_desc_create(
    const axis2_env_t *env)
{
    axis2_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    desc_impl = (axis2_desc_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_desc_impl_t));

    if (NULL == desc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc_impl->param_container = NULL;
    desc_impl->children = NULL;
    desc_impl->desc.ops = NULL;

    desc_impl->param_container = (axis2_param_container_t *)
            axis2_param_container_create(env);
    if (NULL == desc_impl->param_container)
    {
        axis2_desc_free(&(desc_impl->desc), env);
        return NULL;
    }

    desc_impl->children = axis2_hash_make(env);
    if (NULL == desc_impl->children)
    {
        axis2_desc_free(&(desc_impl->desc), env);
        return NULL;
    }

    desc_impl->desc.ops = AXIS2_MALLOC(env->allocator, sizeof(axis2_desc_ops_t));
    if (NULL == desc_impl->desc.ops)
    {
        axis2_desc_free(&(desc_impl->desc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc_impl->desc.ops->free = axis2_desc_free;
    desc_impl->desc.ops->add_param = axis2_desc_add_param;
    desc_impl->desc.ops->get_param = axis2_desc_get_param;
    desc_impl->desc.ops->get_all_params = axis2_desc_get_all_params;
    desc_impl->desc.ops->is_param_locked = axis2_desc_is_param_locked;
    desc_impl->desc.ops->add_child = axis2_desc_add_child;
    desc_impl->desc.ops->get_all_children = axis2_desc_get_all_children;
    desc_impl->desc.ops->get_child = axis2_desc_get_child;
    desc_impl->desc.ops->remove_child = axis2_desc_remove_child;


    return &(desc_impl->desc);
}

axis2_status_t AXIS2_CALL
axis2_desc_free(
    axis2_desc_t *desc,
    const axis2_env_t *env)
{
    axis2_desc_impl_t *desc_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);

    if (desc_impl->children)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;

        for (hi = axis2_hash_first(desc_impl->children, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
            {
                AXIS2_MSG_FREE((axis2_msg_t *)val, env);
                val = NULL;
            }
        }

        axis2_hash_free(desc_impl->children, env);
        desc_impl->children = NULL;
    }

    if (desc_impl->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(desc_impl->param_container, env);
        desc_impl->param_container = NULL;
    }

    if (desc_impl->desc.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.ops);
        desc_impl->desc.ops = NULL;
    }

    if (desc_impl)
    {
        AXIS2_FREE(env->allocator, desc_impl);
        desc_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_desc_add_param(
    axis2_desc_t *desc,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    axis2_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FALSE);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);

    return AXIS2_PARAM_CONTAINER_ADD_PARAM(desc_impl->param_container, env, param);
}

axis2_param_t *AXIS2_CALL
axis2_desc_get_param(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_desc_impl_t *desc_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, NULL);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);
    return AXIS2_PARAM_CONTAINER_GET_PARAM(desc_impl->param_container, env, param_name);
}

axis2_array_list_t *AXIS2_CALL
axis2_desc_get_all_params(
    const axis2_desc_t *desc,
    const axis2_env_t *env)
{
    axis2_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);
    AXIS2_PARAM_CHECK(env->error, desc_impl->param_container, AXIS2_FALSE);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(desc_impl->param_container, env);
}

axis2_bool_t AXIS2_CALL
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

axis2_status_t AXIS2_CALL
axis2_desc_add_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key,
    const void *child)
{
    axis2_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);

    if (desc_impl->children)
    {
        axis2_hash_set(desc_impl->children, key,
                AXIS2_HASH_KEY_STRING, child);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_hash_t *AXIS2_CALL
axis2_desc_get_all_children(
    const axis2_desc_t *desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(desc)->children;
}

void *AXIS2_CALL
axis2_desc_get_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key)
{
    axis2_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);

    return axis2_hash_get(desc_impl->children, key, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_desc_remove_child(
    const axis2_desc_t *desc,
    const axis2_env_t *env,
    const axis2_char_t *key)
{
    axis2_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    desc_impl = AXIS2_INTF_TO_IMPL(desc);

    if (desc_impl->children)
    {
        axis2_hash_set(desc_impl->children, key,
                AXIS2_HASH_KEY_STRING, NULL);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
