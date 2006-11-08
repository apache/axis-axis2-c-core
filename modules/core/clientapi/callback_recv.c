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

#include "axis2_callback_recv.h"
#include <axis2_const.h>
#include <axis2_hash.h>

typedef struct axis2_callback_recv_impl
{
    /** context base struct */
    axis2_callback_recv_t callback_recv;
    /** base context struct */
    axis2_msg_recv_t *base;
    /** callback map */
    axis2_hash_t *callback_map;
    axis2_thread_mutex_t *mutex;
}
axis2_callback_recv_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(callback_recv) ((axis2_callback_recv_impl_t *)callback_recv)


axis2_msg_recv_t *AXIS2_CALL
axis2_callback_recv_get_base(
    const axis2_callback_recv_t *callback_recv,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_callback_recv_free(
    axis2_callback_recv_t *callback_recv,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_callback_recv_add_callback(
    axis2_callback_recv_t *callback_recv,
    const axis2_env_t *env,
    const axis2_char_t *msg_id,
    axis2_callback_t *callback);

axis2_status_t AXIS2_CALL
axis2_callback_recv_receive(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    void *callback_recv_param);

axis2_callback_recv_t *AXIS2_CALL
axis2_callback_recv_create(
    const axis2_env_t *env)
{
    axis2_callback_recv_impl_t *callback_recv_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    callback_recv_impl =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_recv_impl_t));

    if (!callback_recv_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    callback_recv_impl->callback_recv.ops = NULL;
    callback_recv_impl->base = NULL;
    callback_recv_impl->callback_map = NULL;
    callback_recv_impl->mutex = NULL;

    callback_recv_impl->base = axis2_msg_recv_create(env);
    if (!(callback_recv_impl->base))
    {
        axis2_callback_recv_free(&(callback_recv_impl->callback_recv), env);
        return NULL;
    }
    callback_recv_impl->base->derived = callback_recv_impl;
    callback_recv_impl->base->ops->receive = axis2_callback_recv_receive;

    callback_recv_impl->callback_map = axis2_hash_make(env);
    if (!(callback_recv_impl->callback_map))
    {
        axis2_callback_recv_free(&(callback_recv_impl->callback_recv), env);
        return NULL;
    }

    callback_recv_impl->mutex = axis2_thread_mutex_create(env->allocator,
                                 AXIS2_THREAD_MUTEX_DEFAULT);
    /* initialize ops */
    callback_recv_impl->callback_recv.ops  =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_recv_ops_t));

    if (!callback_recv_impl->callback_recv.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_callback_recv_free(&(callback_recv_impl->callback_recv), env);
        return NULL;
    }

    callback_recv_impl->callback_recv.ops->get_base =
        axis2_callback_recv_get_base;
    callback_recv_impl->callback_recv.ops->free =
        axis2_callback_recv_free;
    callback_recv_impl->callback_recv.ops->add_callback =
        axis2_callback_recv_add_callback;

    return &(callback_recv_impl->callback_recv);
}

axis2_msg_recv_t *AXIS2_CALL
axis2_callback_recv_get_base(
    const axis2_callback_recv_t *callback_recv,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(callback_recv)->base;
}

axis2_status_t AXIS2_CALL
axis2_callback_recv_free(
    axis2_callback_recv_t *callback_recv,
    const axis2_env_t *env)
{
    axis2_callback_recv_impl_t *callback_recv_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    callback_recv_impl = AXIS2_INTF_TO_IMPL(callback_recv);

    if (callback_recv_impl->callback_recv.ops)
    {
        AXIS2_FREE(env->allocator, callback_recv_impl->callback_recv.ops);
        callback_recv_impl->callback_recv.ops = NULL;
    }
    if(NULL != callback_recv_impl->mutex)
    {
        axis2_thread_mutex_destroy(callback_recv_impl->mutex);
        callback_recv_impl->mutex = NULL;
    }

    if (callback_recv_impl->callback_map)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(callback_recv_impl->callback_map, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            if (val)
                AXIS2_FREE(env->allocator, val);
            val = NULL;
        }

        axis2_hash_free(callback_recv_impl->callback_map, env);
        callback_recv_impl->callback_map = NULL;
    }

    if (callback_recv_impl->base)
    {
        AXIS2_MSG_RECV_FREE(callback_recv_impl->base, env);
    }

    if (callback_recv_impl)
    {
        AXIS2_FREE(env->allocator, callback_recv_impl);
        callback_recv_impl = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_callback_recv_add_callback(
    axis2_callback_recv_t *callback_recv,
    const axis2_env_t *env,
    const axis2_char_t *msg_id,
    axis2_callback_t *callback)
{
    axis2_callback_recv_impl_t *callback_recv_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    callback_recv_impl = AXIS2_INTF_TO_IMPL(callback_recv);

    if (msg_id)
    {
        axis2_char_t *mid = AXIS2_STRDUP(msg_id, env);
        axis2_hash_set(callback_recv_impl->callback_map,
                mid, AXIS2_HASH_KEY_STRING, callback);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_callback_recv_receive(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    void *callback_recv_param)
{
    axis2_callback_recv_impl_t *callback_recv_impl = NULL;
    axis2_relates_to_t *relates_to = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    callback_recv_impl = AXIS2_INTF_TO_IMPL(msg_recv->derived);

    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env);
    if (msg_info_headers)
    {
        relates_to = AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(msg_info_headers, env);
        if (relates_to)
        {
            const axis2_char_t *msg_id = AXIS2_RELATES_TO_GET_VALUE(relates_to, env);
            if (msg_id)
            {
                axis2_async_result_t *result = NULL;
                axis2_callback_t *callback = (axis2_callback_t*)
                        axis2_hash_get(callback_recv_impl->callback_map, msg_id, AXIS2_HASH_KEY_STRING);

                result = axis2_async_result_create(env, msg_ctx);
                if (callback && result)
                {
                    AXIS2_CALLBACK_INVOKE_ON_COMPLETE(callback, env, result);
                    AXIS2_CALLBACK_SET_COMPLETE(callback, env, AXIS2_TRUE);
                    AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, NULL);
                }

                AXIS2_ASYNC_RESULT_FREE(result, env);
                if (callback && result)
                    return AXIS2_SUCCESS;
            }
        }
    }

    return AXIS2_FAILURE;
}
