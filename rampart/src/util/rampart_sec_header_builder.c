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

#include <stdio.h>
#include <rampart_encryption.h>
#include <rampart_constants.h>
#include <rampart_sec_header_builder.h>
#include <rampart_username_token.h>
#include <rampart_timestamp_token.h>
#include <rampart_util.h>
#include <rampart_sec_processed_result.h>
#include <rampart_handler_util.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_utility.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_data_reference.h>
#include <oxs_token_cipher_data.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_key_info.h>
#include <oxs_token_key_name.h>
#include <oxs_key.h>
#include <oxs_axiom.h>
#include <oxs_asym_ctx.h>
#include <oxs_token_reference_list.h>
#include <axis2_utils.h>
#include <axis2_array_list.h>
#include <rampart_signature.h>
/*Private functions*/
axis2_status_t AXIS2_CALL
rampart_interchange_nodes(const axis2_env_t *env,
    axiom_node_t *node_to_move,
    axiom_node_t *node_before)
{
    axis2_status_t status = AXIS2_FAILURE;

    axiom_node_t *temp_node = NULL;

    temp_node = axiom_node_detach(node_to_move,env);
    status = axiom_node_insert_sibling_before(node_before,env,temp_node);

    return status;
}



/*Public functions*/


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shb_build_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope)
{
   
    axis2_status_t status = AXIS2_FAILURE;
    axiom_soap_header_t *soap_header = NULL;
    axiom_node_t *soap_header_node = NULL;
    axiom_element_t *soap_header_ele = NULL;
    axiom_soap_header_block_t *sec_header_block = NULL;
    axiom_namespace_t *sec_ns_obj = NULL;
    axiom_node_t *sec_node =  NULL;
    axiom_element_t *sec_ele = NULL;
    axis2_bool_t is_encrypt_before_sign = AXIS2_FALSE;
    axiom_node_t *sig_node = NULL;        
    axiom_node_t *enc_key_node = NULL;

    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);    
    soap_header  = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
    soap_header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
    soap_header_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                            soap_header_node, env);


    sec_ns_obj =  axiom_namespace_create(env, RAMPART_WSSE_XMLNS,
                RAMPART_WSSE);

    sec_header_block = AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK(soap_header,
                env, RAMPART_SECURITY, sec_ns_obj);

    if(!sec_header_block)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shb] Security header block is NULL");
        return AXIS2_SUCCESS;
    }

    AXIOM_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_BOOL(sec_header_block,
                env, AXIS2_TRUE);

    sec_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(sec_header_block, env);
    sec_ele = (axiom_element_t *)
                    AXIOM_NODE_GET_DATA_ELEMENT(sec_node, env);
        
    /*check the binding*/
    if((rampart_context_get_binding_type(rampart_context,env)) == RP_BINDING_ASYMMETRIC)
    {
        /*Do Asymmetric Binding specific things*/

        /*Timestamp Inclusion*/
        if(rampart_context_is_include_timestamp(rampart_context,env))
        {
            rampart_timestamp_token_t *timestamp_token = NULL;
            int ttl = -1;
            AXIS2_LOG_INFO(env->log, "[rampart][shb]  building Timestamp Token");
            AXIS2_LOG_INFO(env->log, "[rampart][shb]  Using default timeToLive value %d",
                RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE);
           /*ttl = RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE;*/
            ttl = rampart_context_get_ttl(rampart_context,env);
            
            timestamp_token = rampart_timestamp_token_create(env);
            status = RAMPART_TIMESTAMP_TOKEN_BUILD(timestamp_token, env,
                             sec_node, sec_ns_obj, ttl);
            if (status == AXIS2_FAILURE)
            {
                AXIS2_LOG_INFO(env->log, "[rampart][shb] Timestamp Token build failed. ERROR");
                return AXIS2_FAILURE;
            }
        } 
        
        /*Check whether we need username token*/
        /*User name tokens includes in messages sent from client to server*/
        if(!axis2_msg_ctx_get_server_side(msg_ctx,env))
        {            
            if(rampart_context_is_include_username_token(rampart_context,env))
            {
                rampart_username_token_t *username_token = NULL;

                username_token = rampart_username_token_create(env);
        /*Now we are passing rampart_context here so inside this method  
        relevant parameters are extracted. */

                AXIS2_LOG_INFO(env->log, "[rampart][shb]  building UsernmaeToken");
                status = RAMPART_USERNAME_TOKEN_BUILD(username_token,
                                env,
                                rampart_context,
                                sec_node,
                                sec_ns_obj);
                if (status == AXIS2_FAILURE)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shb] UsernmaeToken build failed. ERROR");
                    return AXIS2_FAILURE;
                }
            }            
        }                
        /*Check the encryption and signature order*/
        if(rampart_context_is_encrypt_before_sign(rampart_context,env))
        {
            is_encrypt_before_sign = AXIS2_TRUE;
            /*Check what are the parts to encrypt and send them to the encrypt method*/
            status = rampart_enc_encrypt_message(env, msg_ctx,rampart_context,soap_envelope,sec_node);
            if(status != AXIS2_SUCCESS)            
                return AXIS2_FAILURE;       
            
            /*Then do signature specific things*/
            status = rampart_sig_sign_message(env,msg_ctx,rampart_context,soap_envelope,sec_node);
            if(status != AXIS2_SUCCESS)
                return AXIS2_FAILURE;

            /*Then Handle Supporting token stuff  */
        }  
        else
        {
            is_encrypt_before_sign = AXIS2_FALSE;
            /*First do signature specific stuff*/
            status = rampart_sig_sign_message(env,msg_ctx,rampart_context,soap_envelope,sec_node);
            if(status != AXIS2_SUCCESS)
                return AXIS2_FAILURE;
            
            /*Then Handle Encryption stuff*/
            
            status = rampart_enc_encrypt_message(env, msg_ctx,rampart_context,soap_envelope,sec_node);
            if(status!=AXIS2_SUCCESS )
                return AXIS2_FAILURE;
        }            

            /*If both encryption and signature is done we should intercgange them.
             * because the action done last should appear first in the header. */
        sig_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_SIGNATURE);
        enc_key_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_ENCRYPTED_KEY);
        if(sig_node && enc_key_node)
        {
            if(is_encrypt_before_sign)
            {
                status = rampart_interchange_nodes(env,sig_node,enc_key_node);
                if(status!=AXIS2_SUCCESS)
                {
                    AXIS2_LOG_INFO(env->log,"[rampart][shb]Node interchange failed.");
                    return status;
                }
            }
            else
            {
                status = rampart_interchange_nodes(env,enc_key_node,sig_node);
                if(status!=AXIS2_SUCCESS)
                {
                    AXIS2_LOG_INFO(env->log,"[rampart][shb]Node interchange failed.");
                    return status;
                }
            }
        }            
        return AXIS2_SUCCESS;        
    }
    else if((rampart_context_get_binding_type(rampart_context,env)) == RP_BINDING_SYMMETRIC)
    {
        /*Do Symmetric_binding specific things*/
        AXIS2_LOG_INFO(env->log, "[rampart][shb] Symmetric Binding. We do not support yet");
        return AXIS2_FAILURE;
    }        
    else if((rampart_context_get_binding_type(rampart_context,env)) == RP_BINDING_TRANSPORT)        
    {
        /*Do Transport Binding specific things */
        AXIS2_LOG_INFO(env->log, "[rampart][shb] Transport Binding. We do not support yet");
        return AXIS2_FAILURE;
    }
    else
        return AXIS2_FAILURE;
   /*Timestamp Inclusion*/
    
    /*username Token inclusion*/

    /*check the protection order*/


}
/*    
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shb_build_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope)
{
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    if(soap_envelope){
        axis2_ctx_t *ctx = NULL;
        axiom_soap_header_t *soap_header = NULL;
        axiom_node_t *soap_header_node = NULL;
        axiom_element_t *soap_header_ele = NULL;
        axis2_char_t *items = NULL;
        axiom_soap_header_block_t *sec_header_block = NULL;
        axiom_namespace_t *sec_ns_obj = NULL;

        soap_header  = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
        soap_header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        soap_header_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                            soap_header_node, env);

        items = AXIS2_STRDUP(RAMPART_ACTIONS_GET_ITEMS(actions, env), env);

        if (!items)
        {
            AXIS2_LOG_INFO(env->log,
                "[rampart][shb] No action items defined. Nothing to do");
            return AXIS2_SUCCESS;
        }

        sec_ns_obj =  axiom_namespace_create(env, RAMPART_WSSE_XMLNS,
                RAMPART_WSSE);

        sec_header_block = AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK(soap_header,
                env, RAMPART_SECURITY, sec_ns_obj);

        AXIOM_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_BOOL(sec_header_block,
                env, AXIS2_TRUE);

        if (sec_header_block)
        {
            axis2_char_t* item = NULL;
            axiom_node_t *sec_node =  NULL;
            axiom_element_t *sec_ele = NULL;
            axis2_array_list_t *string_list = NULL;
            int i = 0, size = 0;

            sec_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(sec_header_block, env);
            sec_ele = (axiom_element_t *)
                    AXIOM_NODE_GET_DATA_ELEMENT(sec_node, env);

            string_list = axis2_tokenize(env, items, ' ');
            if (string_list)
            {
                size = axis2_array_list_size(string_list, env);
            }

            ctx = axis2_msg_ctx_get_base(msg_ctx, env);
            for (i = 0; i < size; i++)
            {
                item = axis2_array_list_get(string_list, env, i);
                if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_USERNAMETOKEN ,
                        AXIS2_STRTRIM(env, item, NULL)))
                {
                    rampart_username_token_t *username_token = NULL;
                    username_token = rampart_username_token_create(env);

                    AXIS2_LOG_INFO(env->log, "[rampart][shb]  building UsernmaeToken");
                    status = RAMPART_USERNAME_TOKEN_BUILD(username_token,
                            env,
                            ctx,
                            actions,
                            sec_node,
                            sec_ns_obj);
                    if (status == AXIS2_FAILURE)
                    {
                        AXIS2_LOG_INFO(env->log, "[rampart][shb] UsernmaeToken build failed. ERROR");
                        return AXIS2_FAILURE;
                    }
                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP,
                        AXIS2_STRTRIM(env, item, NULL)))
                {
                    rampart_timestamp_token_t *timestamp_token = NULL;
                    axis2_char_t *ttl_str = NULL;
                    int ttl = -1;

                    AXIS2_LOG_INFO(env->log, "[rampart][shb]  building Timestamp Token");
                    ttl_str = RAMPART_ACTIONS_GET_TIME_TO_LIVE(actions, env);
                    if(!ttl_str){
                        AXIS2_LOG_INFO(env->log, "[rampart][shb]  Using default timeToLive value %d",
                                RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE);
                        ttl = RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE;
                    }else {
                        ttl = atoi(RAMPART_ACTIONS_GET_TIME_TO_LIVE(actions, env));
                    }

                    timestamp_token = rampart_timestamp_token_create(env);
                    status = RAMPART_TIMESTAMP_TOKEN_BUILD(timestamp_token, env,
                            ctx, sec_node, sec_ns_obj, ttl);
                    if (status == AXIS2_FAILURE){
                        AXIS2_LOG_INFO(env->log, "[rampart][shb] Timestamp Token build failed. ERROR");
                        return AXIS2_FAILURE;
                    }
                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_ENCRYPT,
                        AXIS2_STRTRIM(env, item, NULL)))
                {

                    AXIS2_LOG_INFO(env->log, "[rampart][shb] Encrypting we do not support yet");
                    status = rampart_enc_encrypt_message(env, msg_ctx, actions, soap_envelope, sec_node);
                    if (status == AXIS2_FAILURE)
                    {
                        AXIS2_LOG_INFO(env->log, "[rampart][shb] Message encryption failed. ERROR");
                        return AXIS2_FAILURE;
                    }

                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_SIGNATURE,
                        AXIS2_STRTRIM(env, item, NULL)))
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shb] Signing message. We do not support yet");

                }
                else
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shb] We do not support %s item yet" , item);
                }
            }

        }
        else
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shb] Security header block is NULL");
        }
    }else{
        return AXIS2_FAILURE;
    }

    
    return AXIS2_SUCCESS;
}
*/
