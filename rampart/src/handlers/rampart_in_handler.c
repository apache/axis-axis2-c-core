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
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axiom_soap_const.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_header.h>
#include <axiom_soap_header_block.h>
#include <axis2_op.h>
#include <axis2_msg_ctx.h>
#include <axis2_msg_info_headers.h>
#include <axis2_property.h>
#include <rampart_constants.h>
#include <rampart_username_token.h>
#include <rampart_handler_util.h>
#include <rampart_timestamp_token.h>
#include <rampart_util.h>
#include <rampart_sec_header_processor.h>
#include <rampart_sec_processed_result.h>
#include <rp_policy_creator.h>
#include <rp_secpolicy_builder.h>
#include <rampart_context.h>
#include <rampart_engine.h>

/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
rampart_in_handler_invoke(struct axis2_handler *handler,
        const axis2_env_t *env,
        struct axis2_msg_ctx *msg_ctx);


/**********************end of header functions ****************************/

AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
rampart_in_handler_create(const axis2_env_t *env,
         axis2_string_t *name)
{
    axis2_handler_t *handler = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }
    if (handler->ops)
        handler->ops->invoke = rampart_in_handler_invoke;

    return handler;
}


axis2_status_t AXIS2_CALL
rampart_in_handler_invoke(struct axis2_handler *handler,
        const axis2_env_t *env,
        struct axis2_msg_ctx *msg_ctx)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axiom_node_t *sec_node = NULL;
    rampart_context_t *rampart_context = NULL;
    axis2_bool_t serverside =  AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    /*since rampart in_handler is a global handler we should
    first check whether the rampart module is engaged.If not engaged we 
    should not process the message but return success.*/    

    /*This method is implemented in rampart_handler utils.*/
    if(!rampart_is_rampart_engaged(env,msg_ctx))
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_in_handler] Not intended for processing in Rampart");
        return AXIS2_SUCCESS;
    }
 
    serverside = axis2_msg_ctx_get_server_side(msg_ctx,env);

    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if(!soap_envelope)
    {
       AXIS2_LOG_INFO(env->log, "[rampart][rampart_in_handler] No SOAP envelope found. ERROR");
       return AXIS2_FAILURE;
    }        
        
    soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
    if (!soap_header)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No SOAP header found. ERROR");
        /*No SOAP header, so no point of proceeding*/
        return AXIS2_SUCCESS;
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "SOAP header found");

    sec_node = rampart_get_security_token(env, msg_ctx, soap_header);
    
    if(!sec_node)
    {
        AXIS2_LOG_INFO(env->log, 
		    "[rampart][rampart_in_handler] No security header element.");
        return AXIS2_FAILURE;
    }

    status = rampart_set_security_processed_results_property(env, msg_ctx);
    
    if(AXIS2_FAILURE == status)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
				"[rampart][rampart_in_handler] Unable to set the security processed results");
    }

    rampart_context = rampart_engine_init(env,msg_ctx,AXIS2_TRUE);
    if(!rampart_context)
        return AXIS2_FAILURE;

    status = rampart_shp_process_message(env, msg_ctx, rampart_context, 
						soap_envelope, sec_node);
    
    if(status!=AXIS2_SUCCESS)
    {
        AXIS2_LOG_INFO(env->log,
            "[rampart][rampart_in_handler] Security Header processing failed.");
        return status;
    }        

    /*This method will free the rampart_context*/
    status = rampart_engine_shutdown(env,rampart_context);

/*    
    rampart_context_free(rampart_context,env);
    rampart_context = NULL;
*/   
    return status;
}


/*
axis2_status_t AXIS2_CALL
rampart_in_handler_invoke(struct axis2_handler *handler,
        const axis2_env_t *env,
        struct axis2_msg_ctx *msg_ctx)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_param_t *param_in_flow_security = NULL;
    axis2_ctx_t *ctx = NULL;
    axis2_array_list_t *action_list = NULL;
    axis2_param_t *param_action = NULL;
    axiom_node_t *sec_node = NULL;
    rampart_actions_t *actions = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);

    if (soap_envelope)
    {

        soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
        if (soap_header)
        {
            AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "SOAP header found");
            

            actions = rampart_actions_create(env);


            ctx = AXIS2_MSG_CTX_GET_BASE(msg_ctx, env);
            param_in_flow_security = rampart_get_security_param(env, msg_ctx, 
									RAMPART_INFLOW_SECURITY);

            if (param_in_flow_security)
            {
                AXIS2_LOG_INFO(env->log, 
				"[rampart][rampart_in_handler]Inflow Security found");


                action_list = rampart_get_actions(env, ctx, 
								param_in_flow_security);
    
                if (action_list)
                {

                    if (AXIS2_ARRAY_LIST_IS_EMPTY(action_list, env))
                    {
                        AXIS2_LOG_INFO(env->log, 
						"[rampart][rampart_in_handler] No actions defined.");
                    }

                    param_action = (axis2_param_t*) AXIS2_ARRAY_LIST_GET(action_list,
						env, 0);

                    if (param_action)
                    {
                        status = RAMPART_ACTIONS_POPULATE_FROM_PARAMS(actions, 
									env, param_action);
                    }else{
                        AXIS2_LOG_INFO(env->log, 
						"[rampart][rampart_in_handler] Cannot find first action element from the InflowSecurityParameter");
                    }
                }
            }else{
                AXIS2_LOG_INFO(env->log, 
				"[rampart][rampart_in_handler] No Inflow Security in the paramter list.");
                return AXIS2_SUCCESS;
            }

            status = RAMPART_ACTIONS_POPULATE_FROM_CTX(actions, env, ctx);            

            sec_node = rampart_get_security_token(env, msg_ctx, soap_header);


            status = rampart_set_security_processed_results_property(env, msg_ctx);
            if(AXIS2_FAILURE == status){
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
					"[rampart][rampart_in_handler] Unable to set the security processed results");
            }

            if(!sec_node){
                AXIS2_LOG_INFO(env->log, 
					"[rampart][rampart_in_handler] No security header element.");
                return AXIS2_SUCCESS;
            
            }

    
            status = rampart_shp_process_message(env, msg_ctx, actions, 
						soap_envelope, sec_node);
            if (AXIS2_FAILURE == status)
            {                
                return AXIS2_FAILURE;
            }                

           

        }else{ 
            
            return AXIS2_SUCCESS;
        }
    }
    return status;}*/
