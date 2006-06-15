/**
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



#ifndef RAMPART_HANDLER_UTIL_H
#define RAMPART_HANDLER_UTIL_H


#ifdef __cplusplus
extern "C"
{
#endif
    
      
 /**
 * Get parameter for the given parameter name
 * @param env pointer to environment struct    
 * @param msg_ctx message context
 * @param parameter the parameter name
 * @return parameter 
 */   
                                            
AXIS2_EXTERN axis2_param_t* AXIS2_CALL
rampart_get_security_param( const axis2_env_t *env,
		                 axis2_msg_ctx_t *msg_ctx,
                         const axis2_char_t *parameter);
    
 /**
 * Get actions specified in In/OutflowSecurity
 * @param env pointer to environment struct    
 * @param ctx context
 * @param Outflowsecurity
 * @return parameters in an array list
 */   
AXIS2_EXTERN axis2_array_list_t* AXIS2_CALL
rampart_get_actions( const axis2_env_t *env,
                                            const axis2_ctx_t *ctx,
		                                    const axis2_param_t *param_x_flow_security);
    

 
                               
 /**
 * Get the value of the action parameter given the key name
 * Here the key name means the name of the subelement of the 
 * action element.
 * @param env pointer to environment struct
 * @param ctx context
 * @param param_action action parameter
 * @param key element name as a string
 * @return the value of the element
 */   
 
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_get_action_params( const axis2_env_t *env,
                   const axis2_ctx_t *ctx,
                   const axis2_param_t *param_action,
                   const axis2_char_t *key);

                               
 /**
 * Get the security token from the header block
 * @param env pointer to environment struct
 * @param msg_ctx message context
 * @param soap heamsg_ctxder
 * @return soap_header security element node
 */    
AXIS2_EXTERN axiom_node_t *AXIS2_CALL
rampart_get_security_token(const axis2_env_t *env,
                                const axis2_msg_ctx_t *msg_ctx,
                                axiom_soap_header_t *soap_header);

 /**
 * Get password using callback
 * @param env pointer to environment struct
 * @return password
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_callback_pw( const axis2_env_t *env,
                     const axis2_char_t *callback_module_name,
                     const axis2_char_t *username);
    
/** @} */
#ifdef __cplusplus
}
#endif


#endif /*RAMPART_HANDLER_UTIL_H*/
