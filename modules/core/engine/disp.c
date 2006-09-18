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

#include <axis2_disp.h>
#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_relates_to.h>
#include <axis2_svc.h>
#include <axis2_const.h>
#include <axis2_msg_ctx.h>

typedef struct axis2_disp_impl
{
    /** phase */
    axis2_disp_t disp;
    /** base class, inherits from handler */
    axis2_handler_t *base;
    /** phase name */
    axis2_qname_t *qname;
    /** derived struct */
    void* derived; /* deep copy */
    int derived_type;
}
axis2_disp_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(disp) ((axis2_disp_impl_t *)disp)

axis2_handler_t *AXIS2_CALL
axis2_disp_get_base(
    const axis2_disp_t *disp,
    const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
axis2_disp_get_qname(
    const axis2_disp_t *disp,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_disp_set_qname(
    struct axis2_disp *disp,
    const axis2_env_t *env,
    axis2_qname_t *qname);


axis2_status_t AXIS2_CALL
axis2_disp_free(
    struct axis2_disp *disp,
    const axis2_env_t *env);

axis2_svc_t *AXIS2_CALL
axis2_disp_find_svc(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env);

axis2_op_t *AXIS2_CALL
axis2_disp_find_op(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env,
    const axis2_svc_t *svc);



axis2_disp_t *AXIS2_CALL
axis2_disp_create(
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_disp_impl_t *disp_impl = NULL;
    axis2_handler_desc_t *handler_desc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    disp_impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_disp_impl_t));
    if (!disp_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    disp_impl->qname = NULL;
    disp_impl->base = NULL;

    if (qname)
    {
        disp_impl->qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(disp_impl->qname))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_disp_free(&(disp_impl->disp), env);
            return NULL;
        }
    }
    else
    {
        /* create default qname */
        disp_impl->qname = axis2_qname_create(env, "abstract_dispatcher",
                "http://axis.ws.apache.org",
                NULL);
        if (!(disp_impl->qname))
        {
            axis2_disp_free(&(disp_impl->disp), env);
            return NULL;
        }
    }

    disp_impl->base = axis2_handler_create(env);
    if (!disp_impl->base)
    {
        axis2_disp_free(&(disp_impl->disp), env);
        return NULL;
    }

    /* handler desc of base handler */
    handler_desc = axis2_handler_desc_create_with_qname(env, disp_impl->qname);
    if (!handler_desc)
    {
        axis2_disp_free(&(disp_impl->disp), env);
        return NULL;
    }

    AXIS2_HANDLER_INIT(disp_impl->base, env, handler_desc);

    /* set the base struct's invoke op */
    if (disp_impl->base->ops)
        disp_impl->base->ops->invoke = axis2_disp_invoke;

    /* initialize ops */
    disp_impl->disp.ops = NULL;
    disp_impl->disp.ops  = AXIS2_MALLOC(env->allocator, sizeof(axis2_disp_ops_t));
    if (!disp_impl->disp.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_disp_free(&(disp_impl->disp), env);
        return NULL;
    }

    disp_impl->disp.ops->get_base = axis2_disp_get_base;
    disp_impl->disp.ops->get_qname = axis2_disp_get_qname;
    disp_impl->disp.ops->set_qname = axis2_disp_set_qname;
    disp_impl->disp.ops->free = axis2_disp_free;
    disp_impl->disp.ops->find_svc = axis2_disp_find_svc;
    disp_impl->disp.ops->find_op = axis2_disp_find_op;

    return &(disp_impl->disp);
}

axis2_handler_t *AXIS2_CALL
axis2_disp_get_base(
    const axis2_disp_t *disp,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(disp)->base;
}

axis2_qname_t *AXIS2_CALL
axis2_disp_get_qname(
    const axis2_disp_t *disp,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(disp)->qname;
}

axis2_status_t AXIS2_CALL
axis2_disp_set_qname(
    struct axis2_disp *disp,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    axis2_disp_impl_t *disp_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    disp_impl = AXIS2_INTF_TO_IMPL(disp);

    if (disp_impl->qname)
    {
        AXIS2_QNAME_FREE(disp_impl->qname, env);
        disp_impl->qname = NULL;
    }

    if (qname)
    {
        disp_impl->qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(disp_impl->qname))
            return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_disp_invoke(
    struct axis2_handler *handler,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    axis2_svc_t *axis_service = NULL;
    axis2_op_t *op = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    axis_service = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);

    if (!axis_service)
    {
        axis_service = AXIS2_MSG_CTX_FIND_SVC(msg_ctx, env);
        if (axis_service)
        {
            AXIS2_MSG_CTX_SET_SVC(msg_ctx, env, axis_service);
            /*TODO Set the Service Group Context to the message Context*/
        }
    }
    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (!op)
    {
        op = AXIS2_MSG_CTX_FIND_OP(msg_ctx, env, axis_service);

        if (op)
        {
            AXIS2_MSG_CTX_SET_OP(msg_ctx, env, op);
        }
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_disp_free(
    struct axis2_disp *disp,
    const axis2_env_t *env)
{
    axis2_disp_impl_t *disp_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    disp_impl = AXIS2_INTF_TO_IMPL(disp);

    if (disp_impl->qname)
    {
        AXIS2_QNAME_FREE(disp_impl->qname, env);
        disp_impl->qname = NULL;
    }

    disp_impl->base = NULL;

    if (disp_impl->disp.ops)
    {
        AXIS2_FREE(env->allocator, disp_impl->disp.ops);
        disp_impl->disp.ops = NULL;
    }

    AXIS2_FREE(env->allocator, disp_impl);
    disp_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_svc_t *AXIS2_CALL
axis2_disp_find_svc(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env)
{
    return NULL;
}

axis2_op_t *AXIS2_CALL
axis2_disp_find_op(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env,
    const axis2_svc_t *svc)
{
    return NULL;
}
