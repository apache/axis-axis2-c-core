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

#ifndef RAMPART_USERNAME_TOKEN_H
#define RAMPART_USERNAME_TOKEN_H

/**
  * @file rampart_username_token.h
  * @brief The Usernametoken
  */

#ifdef __cplusplus
extern "C"
{
#endif


#include <axis2_env.h>
#include <rampart_constants.h>
#include <rampart_crypto_util.h>
#include <rampart_action.h>
#include <rampart_context.h>
    
        /**
         * Build a username token according to the given parameters.
         * These parameter are taken from the action collection and message context
         * @param env pointer to environment struct
         * @param ctx axis2 context
         * @param actions collection of actions
         * @param sec_node security node
         * @param sec_ns_obj security namespace object
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */ 
/*      axis2_status_t (AXIS2_CALL *
        build)(rampart_username_token_t *username_token,
            const axis2_env_t *env,
            axis2_ctx_t *ctx,
            rampart_actions_t *actions,
            axiom_node_t *sec_node,
            axiom_namespace_t *sec_ns_obj
            );
*/        

axis2_status_t AXIS2_CALL
rampart_username_token_build(
        const axis2_env_t *env,
        rampart_context_t *rampart_context,
        axiom_node_t *sec_node,
        axiom_namespace_t *sec_ns_obj);
        /*                            
         * Validates the given username token
         * @param env pointer to environment struct
         * @param msg_ctx axis2 message context
         * @param soap_header SOAP Header
         * @param actions collection of actions
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
axis2_status_t AXIS2_CALL
rampart_username_token_validate(
        const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx,
        axiom_node_t *ut_node,
        rampart_context_t *rampart_context);

#ifdef __cplusplus
}
#endif


#endif /*RAMPART_USERNAME_TOKEN_H*/
