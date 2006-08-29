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
#include <axis2_array_list.h>
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
#include <rampart_crypto_engine.h>

/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
rampart_in_handler_invoke(struct axis2_handler *handler, 
                         const axis2_env_t *env,
                         struct axis2_msg_ctx *msg_ctx);


/**********************end of header functions ****************************/

AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
rampart_in_handler_create(const axis2_env_t *env, 
                         axis2_qname_t *qname) 
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
    axis2_param_t *param_in_flow_security = NULL;
    axis2_ctx_t *ctx = NULL;
    axis2_array_list_t *action_list = NULL;
    axis2_param_t *param_action = NULL;
    axis2_char_t *items = NULL;
    axiom_node_t *sec_node, *ts_node = NULL;
    axiom_element_t *sec_ele, *ts_ele = NULL;
    axis2_status_t enc_status = AXIS2_FAILURE;
    rampart_actions_t *actions = NULL;
 
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
     
    rampart_print_info(env," Starting rampart in handler ");    
    /*Get SOAP envelope*/ 
    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    
    if (soap_envelope)
    {
        /*Get SOAP header*/
        soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
        if (soap_header)
        { 
            axis2_char_t* item = NULL;
            AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "SOAP header found");
            /*Check InFlowSecurity parameters*/

            ctx = AXIS2_MSG_CTX_GET_BASE (msg_ctx, env);
            param_in_flow_security = rampart_get_security_param( env,msg_ctx,RAMPART_INFLOW_SECURITY);

            if(!param_in_flow_security)
            {
                /*This check ensures that user doesnt need to have security checking in his message flow*/
                AXIS2_LOG_INFO(env->log, "[rampart][rampart_in_handler] No Inflow Security. So nothing to do");
                return AXIS2_SUCCESS;
            }else
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler]Inflow Security found");
            }

            /*Get actions*/
            action_list = rampart_get_actions(env, ctx, param_in_flow_security);

            if(!action_list)
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler]action_list is empty");
                return AXIS2_SUCCESS;
            }

            if(AXIS2_ARRAY_LIST_IS_EMPTY(action_list, env))
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] No actions defined");
                return AXIS2_SUCCESS;
            }

            /*Now we support only one action.i.e. Only the first action*/
            param_action = (axis2_param_t*) AXIS2_ARRAY_LIST_GET(action_list, env, 0);

            if(!param_action)
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Cannot find first action element");
                return AXIS2_FAILURE;
            }
            /*Create and populate rampart actions*/
            actions = rampart_actions_create(env);
            status = RAMPART_ACTIONS_POPULATE_FROM_PARAMS(actions, env, param_action);
            /*Then re-populate using the axis2_ctx*/
            status = RAMPART_ACTIONS_POPULATE_FROM_CTX(actions, env, ctx);

            items = AXIS2_STRDUP(RAMPART_ACTIONS_GET_ITEMS(actions, env), env);
    
            if(!items)
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] No items defined");
                return AXIS2_FAILURE;
            }
                 
            
            item = strtok (items," ");
            while (item != NULL)
            {
                sec_node = rampart_get_security_token(env, msg_ctx, soap_header);
                /*If no sec_node return fault*/
                if(!sec_node){
                    axis2_array_list_t *sub_codes = NULL;
                    sub_codes = axis2_array_list_create(env, 1);
                    if (sub_codes)
                    {
                        AXIS2_ARRAY_LIST_ADD(sub_codes, env, RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE);
                    }

                    rampart_create_fault_envelope(env, "wsse:Security", "Security header element is unavailable",sub_codes, msg_ctx);
                    return AXIS2_FAILURE;
                }
                sec_ele = AXIOM_NODE_GET_DATA_ELEMENT(sec_node, env);
                
                /*UsernameToken*/
                if( 0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_USERNAMETOKEN, AXIS2_STRTRIM(env, item, NULL)) ){       
                        rampart_username_token_t *username_token = NULL;
                        axis2_status_t valid_user = AXIS2_FAILURE;
                        
                        username_token = rampart_username_token_create(env);
                        AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Validating UsernameToken");
                        valid_user = RAMPART_USERNAME_TOKEN_VALIDATE(username_token, env, 
                                                        msg_ctx,soap_header, actions);
                        if(valid_user)
                        {
                            AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Validating UsernameToken SUCCESS");
                            status = AXIS2_SUCCESS;
                        }else{
                            axis2_array_list_t *sub_codes = NULL;
                            sub_codes = axis2_array_list_create(env, 1);
                            if (sub_codes)
                            {
                                AXIS2_ARRAY_LIST_ADD(sub_codes, env, RAMPART_FAULT_FAILED_AUTHENTICATION);
                            }

                            AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Validating UsernameToken FAILED");
                            rampart_create_fault_envelope(env, "wsse:UsernameToken", "Username is not valid",sub_codes, msg_ctx);
                            return AXIS2_FAILURE;
                        }
                /*Encrypt*/    
                }else if( 0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_ENCRYPT, AXIS2_STRTRIM(env, item, NULL)) ){
                        /*Do useful to verify encrypt*/    
                        
                        rampart_crypto_engine_t *engine = NULL;
                        AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Decrypting message");
                        
                        engine = rampart_crypto_engine_create(env);
                        enc_status = RAMPART_CRYPTO_ENGINE_DECRYPT_MESSAGE(engine, env, msg_ctx, actions, soap_envelope, sec_node);

                        RAMPART_CRYPTO_ENGINE_FREE(engine, env);   
                        if(enc_status != AXIS2_SUCCESS){
                            AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Decryption FAILED");
                            return AXIS2_FAILURE;
                        }
                        
                        enc_status = AXIS2_SUCCESS; /*TODO remove*/
                        status = AXIS2_SUCCESS;
                        AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Decryption SUCCESS");
                /*Signature*/        
                }else if( 0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_SIGNATURE, AXIS2_STRTRIM(env, item, NULL)) ){
                        AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Verfying signature... NOT IMPLEMENTED YET.. SORRY");
                /*Timestamp Token*/
                }else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP, AXIS2_STRTRIM(env, item, NULL))){
                        axis2_qname_t *qname = NULL;
                        axis2_status_t valid_ts = AXIS2_FAILURE;
                        rampart_timestamp_token_t *timestamp_token = NULL;
                        
                        AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Validating Timestamp");
                        
                        qname = axis2_qname_create(env,
                                     RAMPART_SECURITY_TIMESTAMP,
                                     RAMPART_WSU_XMLNS,
                                     RAMPART_WSU);
                        if(qname)
                        {
                            ts_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(sec_ele, env, qname, sec_node, &ts_node);
                            if(!ts_ele)
                            {
                                AXIS2_LOG_INFO(env->log,"Cannot find Timestamp in Security element...");
                                return AXIS2_FAILURE;
                            }
                        }
                        timestamp_token = rampart_timestamp_token_create(env);
                        valid_ts = RAMPART_TIMESTAMP_TOKEN_VALIDATE(timestamp_token, env, ts_node);               
                        /*TODO free*/
                        if(valid_ts)
                        {
                            AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Validating Timestamp is SUCCESS ");
                            status = AXIS2_SUCCESS;
                        }else{
                            /*TODO return a fault*/
                            AXIS2_LOG_INFO(env->log,"[rampart][rampart_in_handler] Timestamp is not valid ");
                            axis2_array_list_t *sub_codes = NULL;
                            sub_codes = axis2_array_list_create(env, 1);
                            if (sub_codes)
                            {
                                AXIS2_ARRAY_LIST_ADD(sub_codes, env, RAMPART_FAULT_FAILED_AUTHENTICATION);
                            }

                            rampart_create_fault_envelope(env, "wsse:Timestamptoken", "Timestamp is not valid",sub_codes, msg_ctx);
                            return AXIS2_FAILURE;

                        }   
                }else{
                    return AXIS2_SUCCESS;
                }
                
                item = strtok (NULL, " ");

            } /* End of While */
        } /* End of sec_header */
        
    }/* End of soap_envelope */
    return status;
}
