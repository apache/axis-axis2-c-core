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
#include <rampart_action.h>
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

/*Private functions*/


/*Public functions*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shb_build_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_actions_t *actions,
    axiom_soap_envelope_t *soap_envelope)
{
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    /*We need a SOAP envelope.*/
    if(soap_envelope){
        axis2_ctx_t *ctx = NULL;
        axiom_soap_header_t *soap_header = NULL;
        axiom_node_t *soap_header_node = NULL;
        axiom_element_t *soap_header_ele = NULL;
        axis2_char_t *items = NULL;
        axiom_soap_header_block_t *sec_header_block = NULL;
        axiom_namespace_t *sec_ns_obj = NULL;

/*>>*/
        /*We are checking for the soap header element*/
        soap_header  = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
        soap_header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        soap_header_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                            soap_header_node, env);

/*<<*/
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

        /*Set mustUnderstand=1*/
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

            /*Get action items seperated by spaces*/
            string_list = axis2_tokenize(env, items, ' ');
            if (string_list)
            {
                size = AXIS2_ARRAY_LIST_SIZE(string_list, env);
            }

            ctx = AXIS2_MSG_CTX_GET_BASE(msg_ctx, env);
            /*Iterate thru items. Eg. Usernmaetoken, Timestamp, Encrypt, Signature*/
            for (i = 0; i < size; i++)
            {
                item = AXIS2_ARRAY_LIST_GET(string_list, env, i);
                /*Username token*/
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
                    /*TODO free*/
                    /*Timestamp token*/
                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP,
                        AXIS2_STRTRIM(env, item, NULL)))
                {
                    rampart_timestamp_token_t *timestamp_token = NULL;
                    axis2_char_t *ttl_str = NULL;
                    int ttl = -1;

                    AXIS2_LOG_INFO(env->log, "[rampart][shb]  building Timestamp Token");
                    ttl_str = RAMPART_ACTIONS_GET_TIME_TO_LIVE(actions, env);
                    /*Check for the ttl. If not specified use the default*/
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
                    /*Encrypt*/
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

                    /*Signature*/
                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_SIGNATURE,
                        AXIS2_STRTRIM(env, item, NULL)))
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shb] Signing message. We do not support yet");

                    /*Any other type of action*/
                }
                else
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shb] We do not support %s item yet" , item);
                }
            }/*End of for*/

        }
        else
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shb] Security header block is NULL");
        }
    }else{/*No SOAP message*/
        return AXIS2_FAILURE;
    }

    
    return AXIS2_SUCCESS;
}

