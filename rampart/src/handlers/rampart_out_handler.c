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


#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_svc.h>
#include <axiom_soap.h>
#include <rampart_util.h>
#include <axis2_endpoint_ref.h>
#include <axis2_property.h>
#include <rampart_constants.h>
#include <rampart_username_token.h>
#include <rampart_handler_util.h>
#include <rampart_timestamp_token.h>
#include <rampart_action.h>
#include <rampart_encryption.h>
#include <rampart_sec_header_builder.h>

/*********************** Function headers *********************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_out_handler_invoke(struct axis2_handler *handler,
        const axis2_env_t * env,
        struct axis2_msg_ctx *msg_ctx);



/**********************end of header functions ****************************/

AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
rampart_out_handler_create(const axis2_env_t *env,  axis2_string_t *name)
{
    axis2_handler_t *handler = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }

    /* set the base struct's invoke op */
    if (handler->ops)
        handler->ops->invoke = rampart_out_handler_invoke;

    return handler;
}

axis2_status_t AXIS2_CALL
rampart_out_handler_invoke(struct axis2_handler * handler,
        const axis2_env_t * env, axis2_msg_ctx_t * msg_ctx)
{
    axis2_ctx_t *ctx = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_node_t *soap_header_node = NULL;
    axiom_element_t *soap_header_ele = NULL;
    axis2_param_t *param_out_flow_security = NULL;
    axis2_array_list_t *action_list = NULL;
    axis2_param_t *param_action = NULL;
    rampart_actions_t *actions = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    
    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (!soap_envelope)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No SOAP envelope found. ERROR");
        return AXIS2_FAILURE;
    }

    /*We are checking for the soap header element*/
    soap_header  = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);

    if (!soap_header)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No SOAP header found. ERROR");
        /*No SOAP header, so no point of proceeding*/
        return AXIS2_SUCCESS;
    }

    /*if the soap header is available then add the security header*/
    if (soap_header)
    {
        soap_header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        soap_header_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
							soap_header_node, env);

        /*Create and populate rampart actions*/
        actions = rampart_actions_create(env);

        ctx = AXIS2_MSG_CTX_GET_BASE(msg_ctx, env);
        param_out_flow_security = rampart_get_security_param(env, msg_ctx, 
									RAMPART_OUTFLOW_SECURITY);

        if (!param_out_flow_security)
        {
            AXIS2_LOG_INFO(env->log, 
			"[rampart][rampart_out_handler] No Outflow Security. So nothing to do");
            return AXIS2_SUCCESS;
        }else{
            
            /*Get actions*/
            action_list = rampart_get_actions(env, ctx, param_out_flow_security);

            
            if (!action_list)
            {
                AXIS2_LOG_INFO(env->log, 
				"[rampart][rampart_out_handler] No actions defined in outflow security");
                return AXIS2_SUCCESS;
            }

            if (AXIS2_ARRAY_LIST_IS_EMPTY(action_list, env))
            {
                AXIS2_LOG_INFO(env->log, 
				"[rampart][rampart_out_handler] No actions defined in outflow security");
                return AXIS2_SUCCESS;
            }

            /*Now we support only one action.*/
            param_action = (axis2_param_t*) AXIS2_ARRAY_LIST_GET(action_list, 
							env, 0);

            if (!param_action)
            {
                AXIS2_LOG_INFO(env->log, 
					"[rampart][rampart_out_handler] Cannot find first action element ERROR");
                return AXIS2_FAILURE;
            }

            status = RAMPART_ACTIONS_POPULATE_FROM_PARAMS(actions, env, 
						param_action);
            
        }

        
        /*Then re-populate using the axis2_ctx. 
         *This is a hack to facilitate PHP extension to pass security parameters using options
         *In future we should change this to get the param_out_flow_security from a PROPERTY if it is not available
         *in the msg_ctx as as a parameter.*/
        status = RAMPART_ACTIONS_POPULATE_FROM_CTX(actions, env, ctx);

        /*We call the security header builder*/
        status = rampart_shb_build_message(env, msg_ctx, actions, soap_envelope);
        if(AXIS2_FAILURE == status){
                AXIS2_LOG_INFO(env->log, 
					"[rampart][rampart_out_handler] Security header building failed ERROR");
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}
