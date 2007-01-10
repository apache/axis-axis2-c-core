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

#include <axis2_op_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_op.h>
#include <axis2_const.h>
#include <axis2_hash.h>

struct axis2_op_ctx
{
    /** base context struct */
    axis2_ctx_t *base;
    /** parent of operation context is a service context instance */
    struct axis2_svc_ctx *parent;
    /** message context map */
    axis2_hash_t *msg_ctx_map;
    /**
     * the operation of which this is a running instance. The MEP of this
     * operation must be one of the 8 predefined ones in WSDL 2.0.
     */
    axis2_op_t *op;
    /** operation Message Exchange Pattern (MEP) */
    int op_mep;
    /** is complete? */
    axis2_bool_t is_complete;
    /** the global message_id -> op_ctx map which is stored in
     * the axis2_conf_ctx. We are caching it here for faster access.
     */
    axis2_hash_t *op_ctx_map;
    /** op qname */
    axis2_qname_t *op_qname;
    /** service qname */
    axis2_qname_t *svc_qname;
    /* mutex to synchronize the read/write operations */
    axis2_thread_mutex_t *mutex;
};

AXIS2_EXTERN axis2_op_ctx_t *AXIS2_CALL
axis2_op_ctx_create(const axis2_env_t *env,
    axis2_op_t *op,
    struct axis2_svc_ctx *svc_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    op_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_op_ctx_t));
    if (!op_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    op_ctx->base = NULL;
    op_ctx->parent = NULL;
    op_ctx->msg_ctx_map = NULL;
    op_ctx->op = NULL;
    op_ctx->op_mep = 0;
    op_ctx->is_complete = AXIS2_FALSE;
    op_ctx->op_ctx_map = NULL;
    op_ctx->op_qname = NULL;
    op_ctx->svc_qname = NULL;
    op_ctx->mutex = axis2_thread_mutex_create(env->allocator,
            AXIS2_THREAD_MUTEX_DEFAULT);

    if (NULL == op_ctx->mutex)
    {
        axis2_op_ctx_free(op_ctx, env);
        return NULL;
    }

    op_ctx->base = axis2_ctx_create(env);
    if (!(op_ctx->base))
    {
        axis2_op_ctx_free(op_ctx, env);
        return NULL;
    }

    if (op)
    {
        op_ctx->op = op;
    }

    op_ctx->msg_ctx_map = axis2_hash_make(env);
    if (!(op_ctx->msg_ctx_map))
    {
        axis2_op_ctx_free(op_ctx, env);
        return NULL;
    }

    if (op_ctx->op)
    {
        op_ctx->op_qname = (axis2_qname_t *)AXIS2_OP_GET_QNAME(op_ctx->op, env);
        op_ctx->op_mep = AXIS2_OP_GET_AXIS_SPECIFIC_MEP_CONST(op_ctx->op, env);
    }

    axis2_op_ctx_set_parent(op_ctx, env, svc_ctx);

    return op_ctx;
}

AXIS2_EXTERN axis2_ctx_t *AXIS2_CALL
axis2_op_ctx_get_base(
    const axis2_op_ctx_t *op_ctx,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return op_ctx->base;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_ctx_free(
    struct axis2_op_ctx *op_ctx,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (op_ctx->base)
    {
        AXIS2_CTX_FREE(op_ctx->base, env);
        op_ctx->base = NULL;
    }

    if (op_ctx->msg_ctx_map)
    {
        axis2_hash_index_t *hi = NULL;
        void *ctx = NULL;
        for (hi = axis2_hash_first(op_ctx->msg_ctx_map, env);
            hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &ctx);
            if (ctx)
            {
                axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
                AXIS2_MSG_CTX_FREE(msg_ctx, env);
            }
        }

        axis2_hash_free(op_ctx->msg_ctx_map, env);
        op_ctx->msg_ctx_map = NULL;
    }

    if (op_ctx->mutex)
    {
        axis2_thread_mutex_destroy(op_ctx->mutex);
        op_ctx->mutex = NULL;
    }

    AXIS2_FREE(env->allocator, op_ctx);
    op_ctx = NULL;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_ctx_init(
    struct axis2_op_ctx *op_ctx,
    const axis2_env_t *env,
    struct axis2_conf *conf)
{
    axis2_hash_index_t *hi = NULL;
    void *ctx = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (op_ctx->op_qname && op_ctx->svc_qname)
    {
        axis2_svc_t *svc = NULL;
        axis2_char_t *svc_name = NULL;

        svc_name = AXIS2_QNAME_GET_LOCALPART(op_ctx->svc_qname, env);

        if (svc_name)
        {
            svc = AXIS2_CONF_GET_SVC(conf, env, svc_name);

            if (svc)
            {
                op_ctx->op =
                    AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_ctx->op_qname);
            }
        }
    }

    if (!(op_ctx->msg_ctx_map))
        return AXIS2_SUCCESS;

    for (hi = axis2_hash_first(op_ctx->msg_ctx_map, env);
            hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
            AXIS2_MSG_CTX_INIT(msg_ctx, env, conf);
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_op_ctx_get_op(
    const axis2_op_ctx_t *op_ctx,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return op_ctx->op;
}

AXIS2_EXTERN struct axis2_svc_ctx *AXIS2_CALL
            axis2_op_ctx_get_parent(
                const axis2_op_ctx_t *op_ctx,
                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return op_ctx->parent;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_ctx_add_msg_ctx(
    struct axis2_op_ctx *op_ctx,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    axis2_thread_mutex_lock(op_ctx->mutex);
    if (op_ctx->msg_ctx_map)
    {
        axis2_msg_ctx_t *out_msg_ctx = NULL;
        axis2_msg_ctx_t *in_msg_ctx = NULL;

        /*const axis2_char_t *message_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);
        if (message_id)
        {
            axis2_hash_set(op_ctx->msg_ctx_map, 
                message_id, AXIS2_HASH_KEY_STRING, msg_ctx); 
        }*/

        out_msg_ctx = axis2_hash_get(op_ctx->msg_ctx_map,
                AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING);
        in_msg_ctx = axis2_hash_get(op_ctx->msg_ctx_map,
                AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING);

        if (out_msg_ctx && in_msg_ctx)
        {
            /*TODO:error - completed*/
            return AXIS2_FAILURE;
        }

        if (!out_msg_ctx)
        {
            axis2_hash_set(op_ctx->msg_ctx_map,
                AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING,
                    msg_ctx);
        }
        else
        {
            axis2_hash_set(op_ctx->msg_ctx_map,
                AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING,
                    msg_ctx);
        }
    }
    axis2_thread_mutex_unlock(op_ctx->mutex);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_msg_ctx_t *AXIS2_CALL
axis2_op_ctx_get_msg_ctx(
    const axis2_op_ctx_t *op_ctx,
    const axis2_env_t *env,
    const axis2_char_t *message_id)
{
    AXIS2_ENV_CHECK(env, NULL);

    axis2_thread_mutex_lock(op_ctx->mutex);
    if (op_ctx->msg_ctx_map)
    {
        axis2_msg_ctx_t *rv = NULL;
        rv = axis2_hash_get(op_ctx->msg_ctx_map, message_id,
                AXIS2_HASH_KEY_STRING);
        axis2_thread_mutex_unlock(op_ctx->mutex);
        return rv;
    }
    axis2_thread_mutex_unlock(op_ctx->mutex);
    return NULL;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_op_ctx_get_is_complete(
    const axis2_op_ctx_t *op_ctx,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return op_ctx->is_complete;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_ctx_set_complete(
    struct axis2_op_ctx *op_ctx,
    const axis2_env_t *env,
    axis2_bool_t is_complete)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    op_ctx->is_complete = is_complete;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_ctx_cleanup(
    struct axis2_op_ctx *op_ctx,
    const axis2_env_t *env)
{
    axis2_hash_index_t *hi = NULL;
    void *ctx = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!(op_ctx->msg_ctx_map))
        return AXIS2_SUCCESS;

    for (hi = axis2_hash_first(op_ctx->msg_ctx_map, env);
            hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
            const axis2_char_t *message_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);
            if (message_id)
            {
                axis2_hash_set(op_ctx->msg_ctx_map,
                        message_id, AXIS2_HASH_KEY_STRING, NULL);
                return AXIS2_MSG_CTX_FREE(msg_ctx, env);
            }
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_ctx_set_parent(
    struct axis2_op_ctx *op_ctx,
    const axis2_env_t *env,
    struct axis2_svc_ctx *svc_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (svc_ctx)
    {
        op_ctx->parent = svc_ctx;
    }

    if (op_ctx->parent) /* that is if there is a service context associated */
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(op_ctx->parent, env);
        if (conf_ctx)
        {
            op_ctx->op_ctx_map =
                AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, env);
        }
        op_ctx->svc_qname =
            (axis2_qname_t *)AXIS2_SVC_GET_QNAME(AXIS2_SVC_CTX_GET_SVC(op_ctx->parent, env), env);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_op_ctx_get_msg_ctx_map(
    const axis2_op_ctx_t *op_ctx,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return op_ctx->msg_ctx_map;
}
