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

#include <axis2_disp.h>
#include <axis2_handler_desc.h>
#include <axis2_string.h>
#include <axis2_relates_to.h>
#include <axis2_svc.h>
#include <axis2_const.h>
#include <axis2_msg_ctx.h>

const axis2_char_t *AXIS2_DISP_NAME = "abstract_dispatcher";

typedef struct axis2_disp_impl
{
    /** phase */
    axis2_disp_t disp;
    /** base class, inherits from handler */
    axis2_handler_t *base;
    /** phase name */
    axis2_string_t *name;
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

axis2_string_t *AXIS2_CALL
axis2_disp_get_name(
    const axis2_disp_t *disp,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_disp_set_name(
    struct axis2_disp *disp,
    const axis2_env_t *env,
    axis2_string_t *name);


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
    const axis2_string_t *name)
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

    disp_impl->name = NULL;
    disp_impl->base = NULL;

    if (name)
    {
        disp_impl->name = axis2_string_clone((axis2_string_t *)name, env);
        if (!(disp_impl->name))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_disp_free(&(disp_impl->disp), env);
            return NULL;
        }
    }
    else
    {
        /* create default name */
        disp_impl->name = axis2_string_create_const(env, (axis2_char_t**)&AXIS2_DISP_NAME);
        if (!(disp_impl->name))
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
    handler_desc = axis2_handler_desc_create(env, disp_impl->name);
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
    disp_impl->disp.ops->get_name = axis2_disp_get_name;
    disp_impl->disp.ops->set_name = axis2_disp_set_name;
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

axis2_string_t *AXIS2_CALL
axis2_disp_get_name(
    const axis2_disp_t *disp,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(disp)->name;
}

axis2_status_t AXIS2_CALL
axis2_disp_set_name(
    struct axis2_disp *disp,
    const axis2_env_t *env,
    axis2_string_t *name)
{
    axis2_disp_impl_t *disp_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    disp_impl = AXIS2_INTF_TO_IMPL(disp);

    if (disp_impl->name)
    {
        axis2_string_free(disp_impl->name, env);
        disp_impl->name = NULL;
    }

    if (name)
    {
        disp_impl->name = axis2_string_clone(name, env);
        if (!(disp_impl->name))
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

    axis_service =  axis2_msg_ctx_get_svc(msg_ctx, env);

    if (!axis_service)
    {
        axis_service =  axis2_msg_ctx_find_svc(msg_ctx, env);
        if (axis_service)
        {
             axis2_msg_ctx_set_svc(msg_ctx, env, axis_service);
            /*TODO Set the Service Group Context to the message Context*/
        }
    }
    op =  axis2_msg_ctx_get_op(msg_ctx, env);
    if (!op)
    {
        op =  axis2_msg_ctx_find_op(msg_ctx, env, axis_service);

        if (op)
        {
             axis2_msg_ctx_set_op(msg_ctx, env, op);
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

    if (disp_impl->name)
    {
        axis2_string_free(disp_impl->name, env);
        disp_impl->name = NULL;
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
