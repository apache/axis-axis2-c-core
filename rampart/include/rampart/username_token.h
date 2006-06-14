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

#ifndef USERNAME_TOKEN_H
#define USERNAME_TOKEN_H


#ifdef __cplusplus
extern "C"
{
#endif


#include <axis2_env.h>
#include <rampart/username_token.h>
#include <rampart/rampart_constants.h>
#include <rampart/rampart_crypto_util.h>

/**
 * Get the password for given outflow security configuration
 * @param env pointer to environment struct
 * @param ctx axis2 context
 * @param Outflow security parameter
 * @return password
 */ 
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_get_password( const axis2_env_t *env,
                   axis2_ctx_t *ctx,
           const axis2_param_t *param_out_flow_security);     

  
/**
 * Get the value for a given key from the axis2 context. The key is the parameter and 
 * value is the property value.
 * @param env pointer to environment struct
 * @param ctx axis2 context
 * @return property value
 */         
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_get_value( const axis2_env_t *env,
                   axis2_ctx_t *ctx,
		           const axis2_char_t *key);
                   
  
/**
 * Build username token 
 * @param env pointer to environment struct
 * @param ctx axis2 context 
 * @param param_action action parameter
 * @param sec_node security node
 * @param sec_ns_obj security namespace object
 * @return UsernameToken as a node
 */              
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
rampart_build_username_token(const axis2_env_t *env,
                                axis2_ctx_t *ctx,
                                axis2_param_t *param_action,
                                axiom_node_t *sec_node,
                                axiom_namespace_t *sec_ns_obj
                                );
  
/**
 * Validates the given username token 
 * @param env pointer to environment struct
 * @param msg_ctx axis2 message context
 * @param sopa_header SOAP Header
 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
 */   
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_validate_username_token(const axis2_env_t *env,
                                axis2_msg_ctx_t *msg_ctx,
                                axiom_soap_header_t *soap_header,
                                axis2_param_t *param_action);

/** @} */
#ifdef __cplusplus
}
#endif


#endif /*USERNAME_TOKEN_H*/
