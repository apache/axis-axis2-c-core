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
axiom_soap_action_disp_invoke (struct axis2_handler * handler, 
                               const axis2_env_t *env,
                               struct axis2_msg_ctx *msg_ctx);

                               
axis2_svc_t* AXIS2_CALL 
axiom_soap_action_disp_find_svc(axis2_msg_ctx_t *msg_ctx,
                                const axis2_env_t *env);

                                
axis2_op_t* AXIS2_CALL 
axiom_soap_action_disp_find_op(axis2_msg_ctx_t *msg_ctx, 
                                const axis2_env_t *env,
                                axis2_svc_t *svc);


axis2_disp_t* AXIS2_CALL 
axiom_soap_action_disp_create(const axis2_env_t *env) 
{
    axis2_disp_t *disp = NULL;
    axis2_handler_t *handler = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);

    qname = axis2_qname_create(env, "soap_action_based_dispatcher", 
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

    handler->ops->invoke = axiom_soap_action_disp_invoke;
    
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
axiom_soap_action_disp_find_svc(axis2_msg_ctx_t *msg_ctx, 
                                const axis2_env_t *env) 
{    
    AXIS2_ENV_CHECK(env, NULL);    

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Checking for service using SOAPAction is a TODO item");

    return NULL;
}

/**
 * finds the op
 *
 * @param service
 * @param msg_ctx
 * @return
 */
axis2_op_t* AXIS2_CALL 
axiom_soap_action_disp_find_op(axis2_msg_ctx_t *msg_ctx, 
                                const axis2_env_t *env,
                                axis2_svc_t *svc)
{
    axis2_char_t *action = NULL;
    axis2_op_t *op = NULL;        
    
    AXIS2_ENV_CHECK(env, NULL);  
    AXIS2_PARAM_CHECK(env->error, svc, NULL);    
    
    action = AXIS2_MSG_CTX_GET_SOAP_ACTION(msg_ctx, env);
    
    if (action)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
            "Checking for operation using SOAPAction : %s", action);

        op = AXIS2_SVC_GET_OP_BY_SOAP_ACTION(svc, env, action);
        if (!op)
        {
            axis2_char_t * op_name = NULL;
            op_name = AXIS2_RINDEX(action, '/');

            if (op_name)
            {
                op_name += 1;
            }
            else
            {
                op_name = action;
            }

            if (op_name)
            {
                axis2_qname_t *op_qname = axis2_qname_create(env, op_name, NULL, NULL);
            
                op = AXIS2_SVC_GET_OP_WITH_NAME(svc, env, 
                    AXIS2_QNAME_GET_LOCALPART(op_qname, env));
                
                AXIS2_QNAME_FREE(op_qname, env);
            }
        }

        if (op)
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Operation found using SOAPAction");
    }
    
    return op;
}
    
axis2_status_t AXIS2_CALL 
axiom_soap_action_disp_invoke(struct axis2_handler * handler, 
                              const axis2_env_t *env,
                              struct axis2_msg_ctx *msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);    
    
    msg_ctx->ops->find_svc = axiom_soap_action_disp_find_svc;
    msg_ctx->ops->find_op = axiom_soap_action_disp_find_op;
    
    return axis2_disp_invoke(handler, env, msg_ctx);
}
