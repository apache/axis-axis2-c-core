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
#include <axis2_conf_ctx.h>
#include <axis2_addr.h>
#include <axis2_utils.h>

axis2_status_t AXIS2_CALL 
axis2_addr_disp_invoke (struct axis2_handler * handler, 
                        const axis2_env_t *env,
                        struct axis2_msg_ctx *msg_ctx);
                        
axis2_svc_t* AXIS2_CALL 
axis2_addr_disp_find_svc(axis2_msg_ctx_t *msg_ctx,
                        const axis2_env_t *env);
                        
axis2_op_t* AXIS2_CALL 
axis2_addr_disp_find_op(axis2_msg_ctx_t *msg_ctx, 
                        const axis2_env_t *env,
                        axis2_svc_t *svc);
                        


axis2_disp_t* AXIS2_CALL 
axis2_addr_disp_create(const axis2_env_t *env) 
{
    axis2_disp_t *disp = NULL;
    axis2_handler_t *handler = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);

    qname = axis2_qname_create(env, "addressing_based_dispatcher", 
                                                "http://axis.ws.apache.org",
                                                NULL);
    
    disp = axis2_disp_create(env, qname);
    if (!disp)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    handler = AXIS2_DISP_GET_BASE(disp, env);
    if (!handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
        return NULL;        
    }

    handler->ops->invoke = axis2_addr_disp_invoke;
    
    AXIS2_QNAME_FREE(qname, env);
    
    return disp;
}

    
/** The struct that inherits from this struct
    should implement the find_service and find_op methods and assing the 
    respective function pointers in the base struct.
    Here we have only the dummy implementation to gauard against erros due to 
    the failure to provide an impl version by mistake.
 */

/**
 * finds the service
 *
 * @param messageContext
 * @return
 */
axis2_svc_t* AXIS2_CALL 
axis2_addr_disp_find_svc(axis2_msg_ctx_t *msg_ctx, 
                         const axis2_env_t *env) 
{    
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_svc_t *svc = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);    
    
    endpoint_ref = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
    
    if (endpoint_ref)
    {
        axis2_char_t *address = NULL;        

        address = AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env);
        if (address)
        {
            axis2_char_t **url_tokens = NULL;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                "Checking for service using WSA enpoint address : %s", address);

            if ( (AXIS2_STRCMP(AXIS2_WSA_ANONYMOUS_URL, address) == 0 ) || 
                  (AXIS2_STRCMP(AXIS2_WSA_NAMESPACE_SUBMISSION, address) == 0 ))
            {
                return NULL;
            }
            
            url_tokens = axis2_parse_request_url_for_svc_and_op(env, address);
            
            if (url_tokens)
            {                
                if (url_tokens[0])
                {                    
                    axis2_conf_ctx_t *conf_ctx = NULL;
                    
                    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
                    if (conf_ctx)
                    {
                        axis2_conf_t *conf = NULL;
                        conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
                        if (conf)
                        {
                            axis2_qname_t *qname = axis2_qname_create(env, url_tokens[0], NULL, NULL);
                
                            svc = AXIS2_CONF_GET_SVC(conf, env, AXIS2_QNAME_GET_LOCALPART(qname, env));
                            
                            AXIS2_QNAME_FREE(qname, env);
                            if(svc)
                                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                                        "Service found using WSA enpoint address");
                        }
                    }                    
                    AXIS2_FREE(env->allocator, url_tokens[0]);
                }
                if(url_tokens[1])
                {
                    AXIS2_FREE(env->allocator, url_tokens[1]);
                }
                AXIS2_FREE(env->allocator, url_tokens);
                url_tokens = NULL; 
            }
        }
    }
        
    return svc;
}

/**
 * finds the op
 *
 * @param service
 * @param msg_ctx
 * @return
 */
axis2_op_t* AXIS2_CALL 
axis2_addr_disp_find_op(axis2_msg_ctx_t *msg_ctx, 
                        const axis2_env_t *env,
                        axis2_svc_t *svc)
{
    axis2_char_t *action = NULL;
    axis2_qname_t *qname = NULL;
    axis2_op_t *op = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, svc, NULL);
    
    action = AXIS2_MSG_CTX_GET_WSA_ACTION(msg_ctx, env);
    
    if (action)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                "Checking for operation using WSA Action : %s", action);
        
        qname = axis2_qname_create(env, action, NULL, NULL);
        op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, qname);
        if (op)
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                    "Operation found using WSA Action");
        AXIS2_QNAME_FREE(qname, env);
    }
    
    return op;
}


axis2_status_t AXIS2_CALL 
axis2_addr_disp_invoke(struct axis2_handler * handler, 
                       const axis2_env_t *env,
                       struct axis2_msg_ctx *msg_ctx)
{
    axis2_relates_to_t *relates_to = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);    
    
    relates_to = AXIS2_MSG_CTX_GET_RELATES_TO(msg_ctx, env);
    
    /** first check if we can dispatch using the relates_to */
    if (relates_to)
    {
        axis2_char_t *relates_to_value = NULL;
        relates_to_value = AXIS2_RELATES_TO_GET_VALUE(relates_to, env);
        if (relates_to_value && AXIS2_STRCMP(relates_to_value, "") != 0 )
        {
            axis2_conf_ctx_t *conf_ctx = NULL;
            
            conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
            if (conf_ctx)
            {
                axis2_op_ctx_t *op_ctx = NULL;
                axis2_char_t *msg_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);
                op_ctx = AXIS2_CONF_CTX_GET_OP_CTX(conf_ctx, env, msg_id);
                if (op_ctx)
                {
                    axis2_op_t *op = NULL;
                    op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
                    if (op)
                    {
                        axis2_svc_ctx_t *svc_ctx = NULL;
                        AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_ctx);
                        AXIS2_MSG_CTX_SET_OP(msg_ctx, env, op);
                        AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, op_ctx);
                        
                        svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
                        if (svc_ctx)
                        {
                            axis2_svc_t *svc = NULL;
                            axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
                            AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx);
                            svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
                            if (svc)
                            {
                                AXIS2_MSG_CTX_SET_SVC(msg_ctx, env, svc);
                            }
                            svc_grp_ctx = AXIS2_SVC_CTX_GET_PARENT(svc_ctx, env);
                            if (svc_grp_ctx)
                            {
                                AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env, 
                                    AXIS2_SVC_GRP_CTX_GET_ID(svc_grp_ctx, env) );
                            }
                        }                      
                        
                    }
                }
            }            
        }
        return AXIS2_SUCCESS;
    }
    
    msg_ctx->ops->find_svc = axis2_addr_disp_find_svc;
    msg_ctx->ops->find_op = axis2_addr_disp_find_op;
    
    return axis2_disp_invoke(handler, env, msg_ctx);
}
