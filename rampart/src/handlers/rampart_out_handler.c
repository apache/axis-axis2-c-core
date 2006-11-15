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

/*********************** Function headers *********************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_out_handler_invoke(struct axis2_handler *handler,
        const axis2_env_t * env,
        struct axis2_msg_ctx *msg_ctx);



/**********************end of header functions ****************************/

AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
rampart_out_handler_create(const axis2_env_t *env, axis2_qname_t *qname)
{
    axis2_handler_t *handler = NULL;
    axis2_qname_t *handler_qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (qname)
    {
        handler_qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(handler_qname))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                    AXIS2_FAILURE);
            return NULL;
        }
    }
    else
    {
        /* create default qname */
        handler_qname = axis2_qname_create(env, "rampart_out_handler",
                "http://axis.ws.apache.org",
                NULL);
        if (!handler_qname)
        {
            return NULL;
        }
    }

    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }

    /* set the base struct's invoke op */
    if (handler->ops)
        handler->ops->invoke = rampart_out_handler_invoke;

    AXIS2_QNAME_FREE(handler_qname, env);

    return handler;
}

axis2_status_t AXIS2_CALL
rampart_out_handler_invoke(struct axis2_handler * handler,
        const axis2_env_t * env, axis2_msg_ctx_t * msg_ctx)
{
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_ctx_t *ctx = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_node_t *soap_header_node = NULL;
    axiom_element_t *soap_header_ele = NULL;
    axis2_param_t *param_out_flow_security = NULL;
    axis2_array_list_t *action_list = NULL;
    axis2_param_t *param_action = NULL;
    axis2_char_t *items = NULL;
    rampart_actions_t *actions = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env);
    

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
        axiom_soap_header_block_t *sec_header_block = NULL;
        axiom_namespace_t *sec_ns_obj = NULL;
        soap_header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        soap_header_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(soap_header_node, env);

        /*Create and populate rampart actions*/
        actions = rampart_actions_create(env);

        ctx = AXIS2_MSG_CTX_GET_BASE(msg_ctx, env);
        param_out_flow_security = rampart_get_security_param(env, msg_ctx, RAMPART_OUTFLOW_SECURITY);

        if (!param_out_flow_security)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No Outflow Security. So nothing to do");
            /*return AXIS2_SUCCESS;*/
        }else{
            
            /*Get actions*/
            action_list = rampart_get_actions(env, ctx, param_out_flow_security);

            if (!action_list)
            {
                AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No actions defined in outflow security");
                return AXIS2_SUCCESS;
            }

            if (AXIS2_ARRAY_LIST_IS_EMPTY(action_list, env))
            {
                AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No actions defined in outflow security");
                return AXIS2_SUCCESS;
            }

            /*Now we support only one action.*/
            param_action = (axis2_param_t*) AXIS2_ARRAY_LIST_GET(action_list, env, 0);

            if (!param_action)
            {
                AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] Cannot find first action element ERROR");
                return AXIS2_FAILURE;
            }

            status = RAMPART_ACTIONS_POPULATE_FROM_PARAMS(actions, env, param_action);
        }
        /*Then re-populate using the axis2_ctx. 
         *This is a hack to facilitate PHP extension to pass security parameters using options
         *In future we should change this to get the param_out_flow_security from a PROPERTY if it is not available
         *in the msg_ctx as as a parameter.*/
        status = RAMPART_ACTIONS_POPULATE_FROM_CTX(actions, env, ctx);

        items = AXIS2_STRDUP(RAMPART_ACTIONS_GET_ITEMS(actions, env), env);

        if (!items)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] No action items defined. Nothing to do");
            return AXIS2_SUCCESS;
        }

        sec_ns_obj =  axiom_namespace_create(env, RAMPART_WSSE_XMLNS,
                RAMPART_WSSE);

        sec_header_block = AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK(soap_header,
                env, RAMPART_SECURITY, sec_ns_obj);

        /*Set mustUnderstand=1*/
        AXIOM_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_BOOL(sec_header_block, env, AXIS2_TRUE);

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

                    AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler]  building UsernmaeToken");
                    status = RAMPART_USERNAME_TOKEN_BUILD(username_token,
                            env,
                            ctx,
                            actions,
                            sec_node,
                            sec_ns_obj);
                    if (status == AXIS2_FAILURE)
                    {
                        AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] UsernmaeToken build failed. ERROR");
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

                    AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler]  building Timestamp Token");
                    ttl_str = RAMPART_ACTIONS_GET_TIME_TO_LIVE(actions, env);
                    ttl = atoi(RAMPART_ACTIONS_GET_TIME_TO_LIVE(actions, env));
                    if (ttl <= 0)
                    {
                        AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler]  Using default timeToLive value %s",
                                RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE);
                        ttl = RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE;
                    }
                    timestamp_token = rampart_timestamp_token_create(env);
                    status = RAMPART_TIMESTAMP_TOKEN_BUILD(timestamp_token, env,
                            ctx, sec_node, sec_ns_obj, ttl);
                    if (status == AXIS2_FAILURE)
                    {
                        AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] Timestamp Token build failed. ERROR");
                        return AXIS2_FAILURE;
                    }
                    /*Encrypt*/
                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_ENCRYPT,
                        AXIS2_STRTRIM(env, item, NULL)))
                {

                    AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] Encrypting we do not support yet");
                    /*rampart_enc_encrypt_message(env, msg_ctx, actions, soap_envelope, sec_node);*/
                    
                    /*Signature*/
                }
                else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_SIGNATURE,
                        AXIS2_STRTRIM(env, item, NULL)))
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] Signing message. We do not support yet");

                    /*Any other type of action*/
                }
                else
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] We do not support %s item yet" , item);
                }
            }/*End of for*/

        }
        else
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_out_handler] Security header block is NULL");
        }

    }
    return AXIS2_SUCCESS;
}



