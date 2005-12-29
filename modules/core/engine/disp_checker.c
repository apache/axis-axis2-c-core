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

#include <axis2_disp_checker.h>
#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_relates_to.h>
#include <axis2_svc.h>
#include <axis2.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_svc_ctx.h>
#include <axis2_endpoint_ref.h>

typedef struct axis2_disp_checker_impl 
{
    /** phase */
	axis2_disp_checker_t disp_checker;
    /** base class, inherits from handler */
    axis2_handler_t *base;
    /** phase name */
    axis2_qname_t *qname;    
} axis2_disp_checker_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(disp_checker) ((axis2_disp_checker_impl_t *)disp_checker)

axis2_status_t AXIS2_CALL axis2_disp_checker_invoke (struct axis2_handler * handler, 
                                                axis2_env_t **env,
                                                struct axis2_msg_ctx *msg_ctx);
axis2_handler_t* AXIS2_CALL axis2_disp_checker_get_base(struct axis2_disp_checker *disp_checker, 
                                               axis2_env_t **env);
axis2_qname_t* AXIS2_CALL axis2_disp_checker_get_qname(struct axis2_disp_checker *disp_checker, 
                                       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_disp_checker_set_qname(struct axis2_disp_checker *disp_checker, 
                                       axis2_env_t **env, axis2_qname_t *qname);
axis2_status_t AXIS2_CALL axis2_disp_checker_free (struct axis2_disp_checker * disp_checker, 
                                            axis2_env_t **env);

axis2_disp_checker_t* AXIS2_CALL axis2_disp_checker_create(axis2_env_t **env, axis2_qname_t *qname) 
{
    axis2_disp_checker_impl_t *disp_checker_impl = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    disp_checker_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_disp_checker_impl_t) );
    if (!disp_checker_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    disp_checker_impl->disp_checker.ops = NULL;
    disp_checker_impl->qname = NULL;
    disp_checker_impl->base = NULL;
    
    /* create default qname */
    disp_checker_impl->qname = axis2_qname_create(env, "dispatch_post_conditions_evaluator", 
                                            "http://axis.ws.apache.org",
                                            NULL);
    if (!(disp_checker_impl->qname))
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
    handler_desc = axis2_handler_desc_create_with_qname(env, disp_checker_impl->qname);
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
    disp_checker_impl->disp_checker.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_disp_checker_ops_t) );
    if (!disp_checker_impl->disp_checker.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_disp_checker_free(&(disp_checker_impl->disp_checker), env);
        return NULL;        
    }

    disp_checker_impl->disp_checker.ops->get_base = axis2_disp_checker_get_base;
    disp_checker_impl->disp_checker.ops->get_qname = axis2_disp_checker_get_qname;
    disp_checker_impl->disp_checker.ops->set_qname = axis2_disp_checker_set_qname;
    disp_checker_impl->disp_checker.ops->free = axis2_disp_checker_free;

    return &(disp_checker_impl->disp_checker);
}

axis2_handler_t* AXIS2_CALL axis2_disp_checker_get_base(struct axis2_disp_checker *disp_checker, 
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(disp_checker, env, NULL);
    return AXIS2_INTF_TO_IMPL(disp_checker)->base;
}

axis2_qname_t* AXIS2_CALL axis2_disp_checker_get_qname(struct axis2_disp_checker *disp_checker, 
                                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(disp_checker, env, NULL);
    return AXIS2_INTF_TO_IMPL(disp_checker)->qname;
}

axis2_status_t AXIS2_CALL axis2_disp_checker_set_qname(struct axis2_disp_checker *disp_checker, 
                                       axis2_env_t **env, axis2_qname_t *qname)
{
    axis2_disp_checker_impl_t *disp_checker_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(disp_checker, env, AXIS2_FAILURE);
    
    disp_checker_impl = AXIS2_INTF_TO_IMPL(disp_checker);
    
    if (disp_checker_impl->qname)
    {
        AXIS2_QNAME_FREE(disp_checker_impl->qname, env);
        disp_checker_impl->qname = NULL;
    }
    
    if (qname)
    {
        disp_checker_impl->qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(disp_checker_impl->qname))
            return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_disp_checker_free(struct axis2_disp_checker * disp_checker, 
                    axis2_env_t **env)
{
    axis2_disp_checker_impl_t *disp_checker_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(disp_checker, env, AXIS2_FAILURE);
    disp_checker_impl = AXIS2_INTF_TO_IMPL(disp_checker);
    
    if (disp_checker_impl->qname)
    {
        AXIS2_QNAME_FREE(disp_checker_impl->qname, env);
        disp_checker_impl->qname = NULL;
    }
    
    if (disp_checker_impl->base)
    {
        axis2_handler_desc_t *handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(disp_checker_impl->base, env);
        AXIS2_HANDLER_DESC_FREE(handler_desc, env); /* freeing handler desc frees handler */
        disp_checker_impl->base = NULL;
    }
    
    if (disp_checker_impl->disp_checker.ops)
    {
        AXIS2_FREE((*env)->allocator, disp_checker_impl->disp_checker.ops);
        disp_checker_impl->disp_checker.ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, disp_checker_impl);
    disp_checker_impl = NULL;
    
    return AXIS2_SUCCESS;    
}
    

/**
 * Method invoke.
 *
 * @param msgctx
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL axis2_disp_checker_invoke(axis2_handler_t* handler, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_t *op = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_t *svc = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;

    AXIS2_FUNC_PARAM_CHECK(handler, env, AXIS2_FAILURE);
    
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);

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
    
    svc = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);
    if (!svc)
    {
        AXIS2_LOG(env, "Service Not found. Endpoint reference is ");
        if (endpoint_ref)
        {
            AXIS2_LOG(env, AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env));
        }
       
        return AXIS2_FAILURE;
    }
    
    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (!op)
    {
        AXIS2_LOG(env, "Operation Not found. Endpoint reference is ");
        if (endpoint_ref)
        {
            AXIS2_LOG(env, AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env));
        }
        AXIS2_LOG(env, " and WSA Action = ");
        AXIS2_LOG(env, AXIS2_MSG_CTX_GET_WSA_ACTION(msg_ctx, env));
        
        return AXIS2_FAILURE;
    }    
    return AXIS2_SUCCESS;
}
