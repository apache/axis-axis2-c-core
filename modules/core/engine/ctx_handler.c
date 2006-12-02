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

#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_svc.h>
#include <axis2_conf_ctx.h>


/**
 * By the time the control comes to this handler, the dispatching must have
 * happened so that the message context contains the service group, service and
 * operation. This will then try to find the contexts for service group, service
 * and the operation.
 */

axis2_status_t AXIS2_CALL
axis2_ctx_handler_invoke(
    axis2_handler_t *handler,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);

axis2_handler_t *AXIS2_CALL
axis2_ctx_handler_create(
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_handler_t *handler = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    axis2_qname_t *handler_qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (qname)
    {
        handler_qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(handler_qname))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    else
    {
        /* create default qname */
        handler_qname = axis2_qname_create(env, "context_handler",
                "http://axis.ws.apache.org",
                NULL);
        if (!handler_qname)
        {
            return NULL;
        }
    }

    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }

    /* handler desc of base handler */
    handler_desc = axis2_handler_desc_create_with_qname(env, handler_qname);
    AXIS2_QNAME_FREE(handler_qname, env);
    if (!handler_desc)
    {
        AXIS2_HANDLER_FREE(handler, env);
        return NULL;
    }

    AXIS2_HANDLER_INIT(handler, env, handler_desc);

    /* set the base struct's invoke op */
    if (handler->ops)
        handler->ops->invoke = axis2_ctx_handler_invoke;

    return handler;
}


axis2_status_t AXIS2_CALL
axis2_ctx_handler_invoke(
    axis2_handler_t *handler,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    axis2_op_t *op = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_MSG_CTX_GET_SVC_CTX(msg_ctx, env);

    if (op_ctx && svc_ctx)
    {
        svc_grp_ctx = AXIS2_SVC_CTX_GET_PARENT(svc_ctx, env);
        if (svc_grp_ctx)
        {
            AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env,
                    AXIS2_SVC_GRP_CTX_GET_ID(svc_grp_ctx, env));
        }
        return AXIS2_SUCCESS;
    }

    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (op)
        op_ctx = AXIS2_OP_FIND_FOR_EXISTING_OP_CTX(op, env, msg_ctx);

    if (op_ctx)
    {
        AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, op_ctx);
        svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
        if (svc_ctx)
        {
            svc_grp_ctx = AXIS2_SVC_CTX_GET_PARENT(svc_ctx, env);
            AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx);
            AXIS2_MSG_CTX_SET_SVC_GRP_CTX(msg_ctx, env, svc_grp_ctx);

            AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env,
                    AXIS2_SVC_GRP_CTX_GET_ID(svc_grp_ctx, env));
        }
        return AXIS2_SUCCESS;
    }
    else if (op) /*  2. if no op_ctx, create new op_ctx */
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_allocator_switch_to_global_pool(env->allocator);
        op_ctx = axis2_op_ctx_create(env, op, NULL);
        if (!op_ctx)
        {
            return AXIS2_FAILURE;
        }

        AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_ctx);

        AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, op_ctx);

        conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        if (conf_ctx)
        {
            svc_grp_ctx = AXIS2_CONF_CTX_FILL_CTXS(conf_ctx, env, msg_ctx);
        }

        axis2_allocator_switch_to_local_pool(env->allocator);
    }

    if (!svc_grp_ctx)
        return AXIS2_FAILURE;
    return AXIS2_SUCCESS;
}
