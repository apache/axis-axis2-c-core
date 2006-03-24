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

#include <axis2_core_utils.h>
#include <axis2_string.h>
#include <axis2_conf.h>
#include <axis2_relates_to.h>
#include <axis2_addr.h>
#include <axis2_http_transport.h>
#include <axis2_uuid_gen.h>
#include <axis2_property.h>

AXIS2_DECLARE(axis2_msg_ctx_t *)
axis2_core_utils_create_out_msg_ctx(axis2_env_t **env,
                                axis2_msg_ctx_t *in_msg_ctx)
{
    axis2_msg_ctx_t *new_msg_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_transport_in_desc_t *transport_in = NULL;
    axis2_transport_out_desc_t *transport_out = NULL;
    axis2_msg_info_headers_t *old_msg_info_headers = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *to = NULL;
    axis2_char_t *msg_id = NULL;
    axis2_relates_to_t *relates_to = NULL;
    axis2_char_t *action = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_bool_t doing_rest = AXIS2_FALSE;
    axis2_bool_t doing_mtom = AXIS2_FALSE;
    axis2_bool_t server_side = AXIS2_FALSE;
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
    axis2_property_t *property = NULL;
    axis2_char_t *msg_uuid = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, in_msg_ctx, NULL);
    
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(in_msg_ctx, env);
    transport_in = AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(in_msg_ctx, env);
    transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(in_msg_ctx, env);
    
    new_msg_ctx = axis2_msg_ctx_create(env, conf_ctx, transport_in, transport_out);
    if(!new_msg_ctx)
    {
        return NULL;
    }
    old_msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(in_msg_ctx, env);
    if(!old_msg_info_headers)
    {
        return NULL;
    }
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(new_msg_ctx, env);
    if(!msg_info_headers)
    {
        /* if there is no msg info header in ew msg ctx, then create one */
        msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
        if (!msg_info_headers)
            return NULL;
        AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(new_msg_ctx, env, msg_info_headers);
    }

    msg_uuid =  axis2_uuid_gen(env);
    AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, msg_uuid);
    if(NULL != msg_uuid)
    {
        AXIS2_FREE((*env)->allocator, msg_uuid);
        msg_uuid = NULL;
    }
    reply_to = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, reply_to);
    
    fault_to = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, fault_to);
    
    to = AXIS2_MSG_INFO_HEADERS_GET_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, to);
    
    msg_id = AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(old_msg_info_headers, env);
    relates_to = axis2_relates_to_create(env, msg_id, 
        AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE);
    AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to);
    
    action = AXIS2_MSG_INFO_HEADERS_GET_ACTION(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, action);
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_OP_CTX(new_msg_ctx, env, op_ctx);
    
    svc_ctx = AXIS2_MSG_CTX_GET_SVC_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SVC_CTX(new_msg_ctx, env, svc_ctx);
    
    property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env, 
            AXIS2_TRANSPORT_OUT, AXIS2_FALSE);
    if(property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_TRANSPORT_OUT, property,
            AXIS2_FALSE);
        property = NULL;
    }
            
    
    property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env, 
            AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_FALSE);
    if(property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
            property, AXIS2_FALSE);
        property = NULL;
    }        

    /* Setting the charater set encoding */
    
    property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env, 
            AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);         
    if(property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING, 
            property, AXIS2_FALSE);
        property = NULL;
    }
            
    doing_rest = AXIS2_MSG_CTX_GET_DOING_REST(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_DOING_REST(new_msg_ctx, env, doing_rest);
    
    doing_mtom = AXIS2_MSG_CTX_GET_DOING_MTOM(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_DOING_MTOM(new_msg_ctx, env, doing_mtom);
    
    server_side = AXIS2_MSG_CTX_GET_SERVER_SIDE(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SERVER_SIDE(new_msg_ctx, env, server_side);
    
    svc_grp_ctx = AXIS2_MSG_CTX_GET_SVC_GRP_CTX(new_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SVC_GRP_CTX(new_msg_ctx, env, svc_grp_ctx);
    
    return new_msg_ctx;
}

AXIS2_DECLARE(void)
axis2_core_utils_reset_out_msg_ctx(axis2_env_t **env,
                                axis2_msg_ctx_t *out_msg_ctx)
{
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    if (!out_msg_ctx)
        return;
    
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(out_msg_ctx, env);
    if(msg_info_headers)
    {
        AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, NULL);
        AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, NULL);
        AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, NULL);
    } 
    
    AXIS2_MSG_CTX_SET_OP_CTX(out_msg_ctx, env, NULL);
    AXIS2_MSG_CTX_SET_SVC_CTX(out_msg_ctx, env, NULL);
    AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_TRANSPORT_OUT, NULL,
            AXIS2_FALSE);
    AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
            NULL, AXIS2_FALSE);
    AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING, 
            NULL, AXIS2_FALSE);
            
    AXIS2_MSG_CTX_SET_SVC_GRP_CTX(out_msg_ctx, env, NULL);
    
    return;
}

