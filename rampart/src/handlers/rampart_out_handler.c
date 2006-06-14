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
#include <rampart/rampart_util.h>
#include <axis2_endpoint_ref.h>
#include <axis2_property.h>
#include <rampart/rampart_constants.h>
#include <rampart/username_token.h>
#include <rampart/rampart_handler_util.h>
#include <rampart/timestamp_token.h>

/*********************** Function headers *********************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_out_handler_invoke (struct axis2_handler *handler,
                               const axis2_env_t * env,
                               struct axis2_msg_ctx *msg_ctx);
                                   

 
/**********************end of header functions ****************************/

AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
rampart_out_handler_create(const axis2_env_t *env, axis2_qname_t *qname)
{
    axis2_handler_t *handler = NULL;
    axis2_qname_t *handler_qname = NULL;

    AXIS2_ENV_CHECK (env, NULL);

    if (qname)
    {
        handler_qname = AXIS2_QNAME_CLONE (qname, env);
        if (!(handler_qname))
        {
            AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY,
                             AXIS2_FAILURE);
            return NULL;
        }
    }
    else
    {
        /* create default qname */
        handler_qname = axis2_qname_create (env, "rampart_out_handler",
                                            "http://axis.ws.apache.org",
                                            NULL);
        if (!handler_qname)
        {
            return NULL;
        }
    }

    handler = axis2_handler_create (env);
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
rampart_out_handler_invoke (struct axis2_handler * handler,
                               const axis2_env_t * env, axis2_msg_ctx_t * msg_ctx)
{
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_ctx_t *ctx = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_node_t *soap_header_node = NULL;
    axiom_element_t *soap_header_ele = NULL;
    axis2_param_t *param_out_flow_security = NULL;
    axiom_node_t *sec_node =  NULL;    
    axiom_element_t *sec_ele = NULL;
	axis2_array_list_t *action_list = NULL;
    axis2_param_t *param_action = NULL;
    axis2_char_t *items = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);

    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS (msg_ctx, env);\

    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (!soap_envelope)
    {
        return AXIS2_FAILURE;
    }
    
    /*We are checking for the soap header element*/
    soap_header  = AXIOM_SOAP_ENVELOPE_GET_HEADER (soap_envelope, env);
  
    if (!soap_header)
    {
        /*No SOAP header, so no point of proceeding*/
        return AXIS2_SUCCESS; 
    }
    
    /*if the soap header is available then add the security header*/
    if (soap_header)
    {
        axiom_soap_header_block_t *sec_header_block = NULL;
        axiom_namespace_t *sec_ns_obj = NULL;
        soap_header_node=AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        soap_header_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT (soap_header_node, env);

         
        ctx = AXIS2_MSG_CTX_GET_BASE (msg_ctx, env);   
        param_out_flow_security = rampart_get_security_param( env,msg_ctx, RAMPART_OUTFLOW_SECURITY);

        if(!param_out_flow_security)
        {
            rampart_print_info(env,"No Outflow Security");
            return AXIS2_SUCCESS;
        }

        /*Get actions*/ 
        action_list = rampart_get_actions(env, ctx, param_out_flow_security);
        
        if(!action_list)
        {
            rampart_print_info(env,"action_list is empty");
            return AXIS2_SUCCESS;
        }
        
        if(AXIS2_ARRAY_LIST_IS_EMPTY(action_list, env))
        {
            rampart_print_info(env,"No actions defined");
            return AXIS2_SUCCESS; 
        }

        /*Now we support only one action.*/
        param_action = (axis2_param_t*) AXIS2_ARRAY_LIST_GET(action_list, env, 0);
        
        if(!param_action)
        {
            rampart_print_info(env,"Cannot find first action element");
            return AXIS2_FAILURE;
        }
      
        items= rampart_get_action_params(env, ctx,param_action,RAMPART_ACTION_ITEMS);

        if(!items)
        {
            rampart_print_info(env,"No items ... ");
	        return AXIS2_FAILURE;
    	}
  
        sec_ns_obj =  axiom_namespace_create (env, RAMPART_WSSE_XMLNS,
                                      RAMPART_WSSE);   
            
        sec_header_block = AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK (soap_header,
                                                                 env, RAMPART_SECURITY,
                                                                 sec_ns_obj);
        if(sec_header_block)
        {           
            axis2_char_t* item = NULL;
            sec_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE (sec_header_block, env);    
            sec_ele = (axiom_element_t *) 
                 AXIOM_NODE_GET_DATA_ELEMENT (sec_node, env);

            AXIOM_ELEMENT_DECLARE_NAMESPACE (sec_ele, env,
                                           sec_node, sec_ns_obj);

            item = strtok (items," ");

            while (item != NULL)
            {
                if(0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_USERNAMETOKEN , AXIS2_STRTRIM(env, item, NULL)))
                {
                    sec_node = rampart_build_username_token(env, ctx, param_action,  sec_node, sec_ns_obj);
    
                }else if(0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP, AXIS2_STRTRIM(env, item, NULL)))
                {
                    sec_node = rampart_build_timestamp_token(env, ctx, sec_node, sec_ns_obj, 300);
                }else
                {
                    rampart_print_info(env," Rampart happy to see usernametokens and timestamps at the moment");
                }
                item = strtok (NULL, " ");
           }     
        }else{
            rampart_print_info(env,"security header block is NULL");
        }

    }
    return AXIS2_SUCCESS;
}
