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
#include <rampart/rampart_constants.h>
#include <rampart/username_token.h>
#include <rampart/rampart_handler_util.h>
#include <rampart/timestamp_token.h>
#include <rampart/rampart_util.h>

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
    
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    
    rampart_print_info(env," Starting rampart in handler ");    
    
    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    
    if (soap_envelope)
    {
        soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
        if (soap_header)
        { 
            axis2_char_t* item = NULL;
            rampart_print_info(env,"soap header found");
            /*Check InFlowSecurity parameters*/

            ctx = AXIS2_MSG_CTX_GET_BASE (msg_ctx, env);
            param_in_flow_security = rampart_get_security_param( env,msg_ctx,RAMPART_INFLOW_SECURITY);

            if(!param_in_flow_security)
            {
                rampart_print_info(env,"No Inflow Security. So nothing to do");
                return AXIS2_SUCCESS;
            }else
            {
                rampart_print_info(env,"Inflow Security found");
            }

            /*Get actions*/
            action_list = rampart_get_actions(env, ctx, param_in_flow_security);

            rampart_print_info(env,"checking action list...");
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

            /*Now we support only one action.i.e. Only the first action*/
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
                 
            
            item = strtok (items," ");
            while (item != NULL)
            {
                if( 0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_USERNAMETOKEN, AXIS2_STRTRIM(env, item, NULL)) )
                {
                        axis2_status_t valid_user = AXIS2_FAILURE;
                        rampart_print_info(env,"Validate usernametoken ");
                        valid_user = rampart_validate_username_token(env, msg_ctx,soap_header, param_action);
                        if(valid_user)
                        {
                            rampart_print_info(env,"I know this user ");
                            status = AXIS2_SUCCESS;
                        }else{
                            rampart_print_info(env,"I don't know this user ");
                            return AXIS2_FAILURE;
                        }
                    
                }else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP, AXIS2_STRTRIM(env, item, NULL))){
                         axis2_qname_t *qname = NULL;
                         axis2_status_t valid_ts = AXIS2_FAILURE;
                         rampart_print_info(env,"Validate timestamp ");
                         sec_node = rampart_get_security_token(env, msg_ctx, soap_header);
                         sec_ele = AXIOM_NODE_GET_DATA_ELEMENT(sec_node, env);
                    
                        
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
                         valid_ts = rampart_validate_timestamp(env, ts_node);               
                         if(valid_ts)
                        {
                            rampart_print_info(env,"Timestamp is valid ");
                            status = AXIS2_SUCCESS;
                        }else{
                            rampart_print_info(env,"Timestamp is not valid");
                            return AXIS2_FAILURE;
                        }   
                }else{
                    rampart_print_info(env," Rampart validates UsernameTokensOnly");
                    return AXIS2_SUCCESS;
                }
                
                item = strtok (NULL, " ");

            } /* End of While */
        } /* End of sec_header */
        
    }/* End of soap_envelope */
    return status;
}
