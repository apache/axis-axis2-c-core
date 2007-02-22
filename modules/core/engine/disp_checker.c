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

#include "axis2_disp_checker.h"
#include <axis2_handler_desc.h>
#include <axis2_string.h>
#include <axis2_relates_to.h>
#include <axis2_svc.h>
#include <axis2_const.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_svc_ctx.h>
#include <axis2_endpoint_ref.h>
#include <axiom_soap.h>
#include <axiom.h>

const axis2_char_t *AXIS2_DISP_CHECKER_NAME = "dispatch_post_conditions_evaluator";

typedef struct axis2_disp_checker_impl
{
    /** phase */
    axis2_disp_checker_t disp_checker;
    /** base class, inherits from handler */
    axis2_handler_t *base;
    /** phase name */
    axis2_string_t *name;
}
axis2_disp_checker_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(disp_checker) ((axis2_disp_checker_impl_t *)disp_checker)

axis2_status_t AXIS2_CALL
axis2_disp_checker_invoke(
    struct axis2_handler *handler,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);

axis2_handler_t *AXIS2_CALL
axis2_disp_checker_get_base(
    const axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env);

axis2_string_t *AXIS2_CALL
axis2_disp_checker_get_name(
    const axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_disp_checker_set_name(
    axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env,
    const axis2_string_t *name);

axis2_status_t AXIS2_CALL
axis2_disp_checker_free(
    axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env);

axis2_disp_checker_t *AXIS2_CALL
axis2_disp_checker_create(
    const axis2_env_t *env)
{
    axis2_disp_checker_impl_t *disp_checker_impl = NULL;
    axis2_handler_desc_t *handler_desc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    disp_checker_impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_disp_checker_impl_t));
    if (!disp_checker_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    disp_checker_impl->disp_checker.ops = NULL;
    disp_checker_impl->name = NULL;
    disp_checker_impl->base = NULL;

    /* create default name */
    disp_checker_impl->name = axis2_string_create_const(env, (axis2_char_t**)&AXIS2_DISP_CHECKER_NAME);
    if (!(disp_checker_impl->name))
    {
        axis2_disp_checker_free(&(disp_checker_impl->disp_checker), env);
        return NULL;
    }

    disp_checker_impl->base = axis2_handler_create(env);
    if (!disp_checker_impl->base)
    {
        axis2_disp_checker_free(&(disp_checker_impl->disp_checker), env);
        return NULL;
    }

    /* handler desc of base handler */
    handler_desc = axis2_handler_desc_create(env, disp_checker_impl->name);
    if (!handler_desc)
    {
        axis2_disp_checker_free(&(disp_checker_impl->disp_checker), env);
        return NULL;
    }

    AXIS2_HANDLER_INIT(disp_checker_impl->base, env, handler_desc);

    /* set the base struct's invoke op */
    if (disp_checker_impl->base->ops)
        disp_checker_impl->base->ops->invoke = axis2_disp_checker_invoke;

    /* initialize ops */
    disp_checker_impl->disp_checker.ops  = AXIS2_MALLOC(env->allocator, sizeof(axis2_disp_checker_ops_t));
    if (!disp_checker_impl->disp_checker.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_disp_checker_free(&(disp_checker_impl->disp_checker), env);
        return NULL;
    }

    disp_checker_impl->disp_checker.ops->get_base = axis2_disp_checker_get_base;
    disp_checker_impl->disp_checker.ops->get_name = axis2_disp_checker_get_name;
    disp_checker_impl->disp_checker.ops->set_name = axis2_disp_checker_set_name;
    disp_checker_impl->disp_checker.ops->free = axis2_disp_checker_free;

    return &(disp_checker_impl->disp_checker);
}

axis2_handler_t *AXIS2_CALL
axis2_disp_checker_get_base(
    const axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(disp_checker)->base;
}

axis2_string_t *AXIS2_CALL
axis2_disp_checker_get_name(
    const axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(disp_checker)->name;
}

axis2_status_t AXIS2_CALL
axis2_disp_checker_set_name(
    axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env,
    const axis2_string_t *name)
{
    axis2_disp_checker_impl_t *disp_checker_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    disp_checker_impl = AXIS2_INTF_TO_IMPL(disp_checker);

    if (disp_checker_impl->name)
    {
        axis2_string_free(disp_checker_impl->name, env);
        disp_checker_impl->name = NULL;
    }

    if (name)
    {
        disp_checker_impl->name = axis2_string_clone((axis2_string_t *)name, env);
        if (!(disp_checker_impl->name))
            return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_disp_checker_free(
    axis2_disp_checker_t *disp_checker,
    const axis2_env_t *env)
{
    axis2_disp_checker_impl_t *disp_checker_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    disp_checker_impl = AXIS2_INTF_TO_IMPL(disp_checker);

    if (disp_checker_impl->name)
    {
        axis2_string_free(disp_checker_impl->name, env);
        disp_checker_impl->name = NULL;
    }

    if (disp_checker_impl->disp_checker.ops)
    {
        AXIS2_FREE(env->allocator, disp_checker_impl->disp_checker.ops);
        disp_checker_impl->disp_checker.ops = NULL;
    }

    AXIS2_FREE(env->allocator, disp_checker_impl);
    disp_checker_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_disp_checker_invoke(
    axis2_handler_t *handler,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_t *op = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_t *svc = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    const axis2_char_t *address = NULL;
	axiom_soap_fault_t *soap_fault;
	axiom_soap_envelope_t *soap_envelope;
	axiom_soap_body_t *soap_body;
	int soap_version = AXIOM_SOAP12;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    if (!(AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env))) /*if is client side, no point in proceeding*/
        return AXIS2_SUCCESS;

    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);

    if (!op)
    {
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if (op_ctx)
        {
            axis2_op_t *op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
            if (op)
                AXIS2_MSG_CTX_SET_OP(msg_ctx, env, op);
        }
    }

    svc = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);

    if (!svc)
    {
        svc_ctx = AXIS2_MSG_CTX_GET_SVC_CTX(msg_ctx, env);
        if (svc_ctx)
        {
            axis2_svc_t *tsvc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
            if (tsvc)
                AXIS2_MSG_CTX_SET_SVC(msg_ctx, env, tsvc);
        }
    }

    endpoint_ref = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
    if (endpoint_ref)
        address = AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env);

    svc = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);
    if (!svc)
    {
        AXIS2_LOG_INFO(env->log, "Service Not found. Endpoint reference is : %s", (address) ? address : "NULL");
		if (AXIS2_MSG_CTX_GET_IS_SOAP_11 (msg_ctx, env))
		{
			soap_version = AXIOM_SOAP11;
		}
		soap_envelope = axiom_soap_envelope_create_default_soap_envelope (env, soap_version); 
		soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
		soap_fault = axiom_soap_fault_create_default_fault (env, soap_body, "Receiver", "Service Not Found", soap_version);
		AXIS2_MSG_CTX_SET_FAULT_SOAP_ENVELOPE(msg_ctx, env, soap_envelope);
        return AXIS2_FAILURE;
    }

    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (!op)
    {
        AXIS2_LOG_INFO(env->log, "Operation Not found. Endpoint reference is : %s", (address) ? address : "NULL");
		soap_envelope = axiom_soap_envelope_create_default_soap_envelope (env, soap_version); 
		soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
		soap_fault = axiom_soap_fault_create_default_fault (env, soap_body, "Receiver", "Operation Not Found", soap_version);
		AXIS2_MSG_CTX_SET_FAULT_SOAP_ENVELOPE(msg_ctx, env, soap_envelope);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
