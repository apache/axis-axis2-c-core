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

#include <axis2_msg_recv.h>
#include <axis2_param.h>
#include <axis2_description.h>
#include <axis2_class_loader.h>
#include <axis2_engine.h>
#include <axis2_core_utils.h>
#include <axis2_property.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>

/**
 * @brief Message Receiver struct impl
 * Axis2 Message Receiver impl
 */
typedef struct axis2_msg_recv_impl
{
    axis2_msg_recv_t msg_recv;
    axis2_char_t *scope;
}
axis2_msg_recv_impl_t;

#define AXIS2_INTF_TO_IMPL(msg_recv) ((axis2_msg_recv_impl_t *) msg_recv)

/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_msg_recv_free(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_recv_receive(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    struct axis2_msg_ctx *in_msg_ctx,
    void *callback_recv_param);

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_receive_sync(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    void *callback_recv_param);

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_receive_async(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    void *callback_recv_param);

axis2_svc_skeleton_t *AXIS2_CALL
axis2_msg_recv_make_new_svc_obj(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);

axis2_svc_skeleton_t *AXIS2_CALL
axis2_msg_recv_get_impl_obj(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);

axis2_status_t AXIS2_CALL
axis2_msg_recv_set_scope(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    const axis2_char_t *scope);

axis2_char_t *AXIS2_CALL
axis2_msg_recv_get_scope(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_recv_delete_svc_obj(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

/************************* End of function headers ****************************/

axis2_msg_recv_t *AXIS2_CALL
axis2_msg_recv_create(
    const axis2_env_t *env)
{
    axis2_msg_recv_impl_t *msg_recv_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    msg_recv_impl = (axis2_msg_recv_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_msg_recv_impl_t));

    if (NULL == msg_recv_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /*msg_recv_impl->scope = AXIS2_STRDUP(AXIS2_APPLICATION_SCOPE, env);*/
    msg_recv_impl->scope = AXIS2_STRDUP("app*", env);
    msg_recv_impl->msg_recv.ops = NULL;

    msg_recv_impl->msg_recv.ops = (axis2_msg_recv_ops_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axis2_msg_recv_ops_t));

    if (NULL == msg_recv_impl->msg_recv.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_msg_recv_free(&(msg_recv_impl->msg_recv), env);

        return NULL;
    }

    msg_recv_impl->msg_recv.ops->free_fn = axis2_msg_recv_free;
    msg_recv_impl->msg_recv.ops->make_new_svc_obj =
        axis2_msg_recv_make_new_svc_obj;
    msg_recv_impl->msg_recv.ops->get_impl_obj = axis2_msg_recv_get_impl_obj;
    msg_recv_impl->msg_recv.ops->set_scope = axis2_msg_recv_set_scope;
    msg_recv_impl->msg_recv.ops->get_scope = axis2_msg_recv_get_scope;
    msg_recv_impl->msg_recv.ops->delete_svc_obj = axis2_msg_recv_delete_svc_obj;
    msg_recv_impl->msg_recv.ops->receive_sync =
        axis2_raw_xml_in_out_msg_recv_receive_sync;
    msg_recv_impl->msg_recv.ops->receive_async =
        axis2_raw_xml_in_out_msg_recv_receive_async;

    return &(msg_recv_impl->msg_recv);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_msg_recv_free(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env)
{
    axis2_msg_recv_impl_t *recv_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    recv_impl = AXIS2_INTF_TO_IMPL(msg_recv);

    if (recv_impl->scope)
    {
        AXIS2_FREE(env->allocator, recv_impl->scope);
        recv_impl->scope = NULL;
    }

    if (msg_recv->ops)
    {
        AXIS2_FREE(env->allocator, msg_recv->ops);
        msg_recv->ops = NULL;
    }

    if (recv_impl)
    {
        AXIS2_FREE(env->allocator, recv_impl);
        recv_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_msg_recv_receive(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *in_msg_ctx,
    void *callback_recv_param)
{
    return AXIS2_SUCCESS;
}


axis2_svc_skeleton_t *AXIS2_CALL
axis2_msg_recv_make_new_svc_obj(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    struct axis2_svc *svc = NULL;
    struct axis2_op_ctx *op_ctx = NULL;
    struct axis2_svc_ctx *svc_ctx = NULL;
    axis2_param_t *impl_info_param = NULL;
    void *impl_class = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
    if (NULL == svc)
    {
        return NULL;
    }

    impl_class = axis2_svc_get_impl_class(svc, env);
    if (impl_class)
    {
        return impl_class;
    }

    impl_info_param = AXIS2_SVC_GET_PARAM(svc, env, AXIS2_SERVICE_CLASS);
    if (!impl_info_param)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_SVC,
                AXIS2_FAILURE);
        return NULL;
    }

    axis2_allocator_switch_to_global_pool(env->allocator);
    
    axis2_class_loader_init(env);

    impl_class = axis2_class_loader_create_dll(env, impl_info_param);
    axis2_svc_set_impl_class(svc, env, impl_class);

    axis2_allocator_switch_to_local_pool(env->allocator);

    return impl_class;
}


axis2_svc_skeleton_t *AXIS2_CALL
axis2_msg_recv_get_impl_obj(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    struct axis2_svc *svc = NULL;
    struct axis2_op_ctx *op_ctx = NULL;
    struct axis2_svc_ctx *svc_ctx = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
    if (NULL == svc)
    {
        return NULL;
    }
    
    return axis2_msg_recv_make_new_svc_obj(msg_recv, env, msg_ctx);
}

axis2_status_t AXIS2_CALL
axis2_msg_recv_set_scope(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    const axis2_char_t *scope)
{
    axis2_msg_recv_impl_t *msg_recv_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, scope, AXIS2_FAILURE);
    msg_recv_impl = AXIS2_INTF_TO_IMPL(msg_recv);

    if (msg_recv_impl->scope)
    {
        AXIS2_FREE(env->allocator, msg_recv_impl->scope);
        msg_recv_impl->scope = NULL;
    }
    msg_recv_impl->scope = AXIS2_STRDUP(scope, env);
    if (!msg_recv_impl->scope)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_msg_recv_get_scope(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(msg_recv)->scope;
}

axis2_status_t AXIS2_CALL
axis2_msg_recv_delete_svc_obj(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_svc_t *svc = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_param_t *scope_param = NULL;
    axis2_char_t *param_value = NULL;
    axis2_dll_desc_t *dll_desc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
    if (NULL == svc)
    {
        return AXIS2_FAILURE;
    }

    scope_param = AXIS2_SVC_GET_PARAM(svc, env, AXIS2_SCOPE);
    if (scope_param)
    {
        param_value = AXIS2_PARAM_GET_VALUE(scope_param, env);
    }
    if (param_value && (0 == AXIS2_STRCMP(AXIS2_APPLICATION_SCOPE,
            param_value)))
    {
        return AXIS2_SUCCESS;
    }

    impl_info_param = AXIS2_SVC_GET_PARAM(svc, env, AXIS2_SERVICE_CLASS);
    if (!impl_info_param)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_SVC,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    dll_desc = AXIS2_PARAM_GET_VALUE(impl_info_param, env);
    return axis2_class_loader_delete_dll(env, dll_desc);
}

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_receive_sync(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    void *callback_recv_param)
{
    axis2_msg_ctx_t *out_msg_ctx = NULL;
    axis2_engine_t *engine = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
        
    out_msg_ctx = axis2_core_utils_create_out_msg_ctx(env, msg_ctx);
    if (!out_msg_ctx)
    {
        return AXIS2_FAILURE;
    }
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(out_msg_ctx, env);
    if (!op_ctx)
    {
        axis2_core_utils_reset_out_msg_ctx(env, out_msg_ctx);
        AXIS2_MSG_CTX_FREE(out_msg_ctx, env);
        return AXIS2_FAILURE;
    }
    
    status = AXIS2_OP_CTX_ADD_MSG_CTX(op_ctx, env, out_msg_ctx);
    if (!status)
    {
        axis2_core_utils_reset_out_msg_ctx(env, out_msg_ctx);
        AXIS2_MSG_CTX_FREE(out_msg_ctx, env);
        return status;
    }
    status = AXIS2_OP_CTX_ADD_MSG_CTX(op_ctx, env, msg_ctx);
    if(!status)
        return status;

    status = AXIS2_MSG_RECV_INVOKE_IN_OUT_BUSINESS_LOGIC_SYNC(msg_recv, env,
            msg_ctx, out_msg_ctx);
    if (AXIS2_SUCCESS != status)
    {
        axis2_core_utils_reset_out_msg_ctx(env, out_msg_ctx);
        AXIS2_MSG_CTX_FREE(out_msg_ctx, env);
        return status;
    }
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env);
    engine = axis2_engine_create(env, conf_ctx);
    if (!engine)
    {
        AXIS2_MSG_CTX_FREE(out_msg_ctx, env);
        return AXIS2_FAILURE;
    }
    if (AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(out_msg_ctx, env))
    {
        axiom_soap_envelope_t *soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(
                    out_msg_ctx, env);
        if (soap_envelope)
        {
            axiom_soap_body_t *body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope,
                    env);
            if (body)
            {
                /* in case of a SOAP fault, we got to return failure so that
                   transport gets to know that it should send 500 */
                if (AXIOM_SOAP_BODY_HAS_FAULT(body, env))
                {
                    status = AXIS2_FAILURE;
                    AXIS2_MSG_CTX_SET_FAULT_SOAP_ENVELOPE(msg_ctx, env,
                            soap_envelope);
                    AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(out_msg_ctx, env, NULL);
                }
                else
                {
                    /* if it is two way and not a fault then send through engine.
                       if it is one way we do not need to do an engine send */
                    status = AXIS2_ENGINE_SEND(engine, env, out_msg_ctx);
                }
            }
        }
    }
    AXIS2_ENGINE_FREE(engine, env);
    if (!AXIS2_MSG_CTX_IS_PAUSED(out_msg_ctx, env) && 
            !AXIS2_MSG_CTX_IS_KEEP_ALIVE(out_msg_ctx, env))
    {
        axis2_core_utils_reset_out_msg_ctx(env, out_msg_ctx);
    }
    /* this is freed in http worker by resetting the operation context 
       holding this msg context
    AXIS2_MSG_CTX_FREE(out_msg_ctx, env); */
    out_msg_ctx = NULL;
    return status;
}

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_receive_async(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    void *callback_recv_param)
{
    axis2_svr_callback_t *callback = NULL;
    axis2_msg_ctx_t *new_msg_ctx = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    callback = axis2_svr_callback_create(env);
    if (!callback)
    {
        return AXIS2_FAILURE;
    }
    /* TODO run this code in a thread */

    new_msg_ctx = axis2_core_utils_create_out_msg_ctx(env, msg_ctx);
    if (!new_msg_ctx)
    {
        return AXIS2_FAILURE;
    }
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(new_msg_ctx, env);
    if (!op_ctx)
    {
        AXIS2_MSG_CTX_FREE(new_msg_ctx, env);
        return AXIS2_FAILURE;
    }
    AXIS2_OP_CTX_ADD_MSG_CTX(op_ctx, env, new_msg_ctx);
    status = AXIS2_MSG_RECV_INVOKE_IN_OUT_BUSINESS_LOGIC_ASYNC(msg_recv, env,
            msg_ctx, new_msg_ctx, callback);
    /* end of code that run in a thread */
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    /* get thread pool from conf_ctx and execute the thread task */
    /*messageCtx.getConfigurationContext().getThreadPool().execute(theadedTask);*/
    return status;
}
