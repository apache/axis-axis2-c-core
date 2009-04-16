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
#include "mtom_callback.h"
#include <axiom.h>
#include <stdio.h>
#include <axis2_op_ctx.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>
#include <axis2_const.h>
#include <axutil_param.h>

axiom_node_t *build_response(
    const axutil_env_t *env,
    axiom_data_handler_t *data_handler);

axis2_status_t process_attachments(
    axis2_msg_ctx_t *msg_ctx,
    const axutil_env_t * env,
    void *user_param,
    axis2_char_t *callback_name);

axiom_node_t*
axis2_mtom_callback_mtom(
    const axutil_env_t * env,
    axiom_node_t * node,
    axis2_msg_ctx_t *msg_ctx)
{
    axiom_node_t *attachment_node = NULL;
    axiom_node_t *binary_node = NULL;
    axiom_node_t *ret_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    void *user_param = NULL;
    axutil_param_t *callback_name_param = NULL;
    axis2_char_t *callback_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    /* Expected request format is :-
     * <ns1:mtomSample xmlns:ns1="http://ws.apache.org/axis2/c/samples">
     <ns1:attachment>
     <xop:Include xmlns:xop="http://www.w3.org/2004/08/xop/include" href="cid:1.dd5183d4-d58a-1da1-2578-001125b4c063@apache.org"></xop:Include>
     </ns1:attachment>
     </ns1:mtomSample>
     */
    if (!node)                  /* 'mtomSample' node */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL,
                        AXIS2_FAILURE);
        printf("Echo client ERROR: input parameter NULL\n");
        return NULL;
    }

    attachment_node = axiom_node_get_first_child(node, env);
    if (!attachment_node)        /* 'text' node */
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST,
                        AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }

    /* This is the paramter specified in the services.xml 
     * or in axis2.xml
     */

    callback_name_param = axis2_msg_ctx_get_parameter (msg_ctx, env, 
        AXIS2_MTOM_CACHING_CALLBACK);

    if(callback_name_param)
    {
        callback_name = (axis2_char_t *) axutil_param_get_value (callback_name_param, env);
    }

    /* This will process the attachment data in the stream */

    status = process_attachments(msg_ctx, env, user_param, callback_name);
    if(status == AXIS2_FAILURE)
    {
        return NULL;
    }
                
    binary_node = axiom_node_get_first_child(attachment_node, env);
    if (binary_node)
    {
        axiom_data_handler_t *data_handler = NULL;
        axiom_text_t *bin_text = NULL;
        axis2_char_t *attachment_id = NULL;
        axiom_data_handler_t *data_handler_res = NULL;        
        axis2_char_t *location = NULL;

        bin_text = (axiom_text_t *)axiom_node_get_data_element(binary_node, env);
        if(bin_text)
        {
            data_handler = axiom_text_get_data_handler(bin_text, env);
            if(!data_handler)
            {
                return NULL;
            }
        }

        attachment_id = axiom_data_handler_get_mime_id(data_handler, env);

        if(!attachment_id)
        {
            return NULL;
        }
        
        /* This is the case where the attachment resides in memory */

        if (!axiom_data_handler_get_cached(data_handler, env))
        {
           /* axis2_byte_t *input_buff = NULL;
            axis2_byte_t *buff = NULL;
            int buff_len = 0;*/
            
            axiom_data_handler_set_file_name(data_handler, env, attachment_id);
            axiom_data_handler_write_to(data_handler, env);
            location = attachment_id;
        }

        else 
        {
            axiom_data_handler_type_t data_handler_type;
            data_handler_type = axiom_data_handler_get_data_handler_type(data_handler, env);

            if(data_handler_type == AXIOM_DATA_HANDLER_TYPE_CALLBACK)
            {
                /* The service implementer should be aware of how to deal with the attachment 
                 * Becasue it was stored using the callback he provided. */

                  /*axis2_char_t command[1000];

                  sprintf(command, "rm -f /opt/tmp/%s", attachment_id);  
                  system(command);*/  
  
                /*location = attachment_id;*/
                  /*location = "/home/manjula/axis2/mtom/resources/song.MP3"; */
                    
                    location = "/opt/manjula-mtom.MPG"; 
            }
            else if(data_handler_type == AXIOM_DATA_HANDLER_TYPE_FILE)
            {
                axis2_char_t *file_name = NULL;

                file_name = axiom_data_handler_get_file_name(data_handler, env);
                if(file_name)
                {
                    location = axutil_strdup(env, file_name);
                }
            }
        }

        /* The samples sends back an attachment to the client. Hence we are creating the 
         * response data_handler as a type CALLBACK. So the sender callback should be
         * specified either in the services.xml or in the axis2.xml.  
         */

        data_handler_res = axiom_data_handler_create(env, NULL, NULL);
        axiom_data_handler_set_data_handler_type(data_handler_res, env, AXIOM_DATA_HANDLER_TYPE_CALLBACK);
        axiom_data_handler_set_user_param(data_handler_res, env, (void *)location);

        axis2_msg_ctx_set_doing_mtom (msg_ctx, env, AXIS2_TRUE);
        ret_node = build_response(env, data_handler_res);
    }
    else
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST,
                        AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }

    return ret_node;
}

/* Builds the response content */


axiom_node_t *build_response(
    const axutil_env_t *env,
    axiom_data_handler_t *data_handler)
{
    axiom_node_t *mtom_om_node = NULL;
    axiom_element_t *mtom_om_ele = NULL;
    axiom_node_t *text_node = NULL;
    axiom_namespace_t *ns1 = NULL;

    ns1 =
        axiom_namespace_create(env, "http://ws.apache.org/axis2/c/samples",
                               "ns1");
    mtom_om_ele =
        axiom_element_create(env, NULL, "response", ns1, &mtom_om_node);

    axiom_text_create_with_data_handler(env, mtom_om_node, data_handler,
        &text_node);

    return mtom_om_node;
}

axis2_status_t process_attachments(
    axis2_msg_ctx_t *msg_ctx,
    const axutil_env_t * env,
    void *user_param,
    axis2_char_t *callback_name)
{
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_msg_ctx_t *in_msg_ctx = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;

    op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);
    if(op_ctx)
    {
        in_msg_ctx = axis2_op_ctx_get_msg_ctx(op_ctx, env, AXIS2_WSDL_MESSAGE_LABEL_IN);
        if(in_msg_ctx)
        {
            soap_envelope = axis2_msg_ctx_get_soap_envelope(in_msg_ctx, env);        
            if(soap_envelope)
            {
                soap_body = axiom_soap_envelope_get_body(soap_envelope, env);
                if(soap_body)
                {
                    return axiom_soap_body_process_attachments(soap_body, env, user_param, callback_name);
                }
                else
                {
                    return AXIS2_FAILURE;
                }
            }
            else 
            {
                return AXIS2_FAILURE;
            }
        }
        else
        {
            return AXIS2_FAILURE;
        }
    }    
    else
    {
        return AXIS2_FAILURE;
    }
}

