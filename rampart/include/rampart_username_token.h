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
#include <rampart_constants.h>
#include <rampart_crypto_util.h>
#include <rampart_action.h>

    /** Type name for struct rampart_username_token_ops */
    typedef struct rampart_username_token_ops rampart_username_token_ops_t;
    /** Type name for struct rampart_username_token */
    typedef struct rampart_username_token rampart_username_token_t;
    /**
     * Rampart username token ops struct
     * Encapsulator struct for ops of rampart_username_token
     */
    struct rampart_username_token_ops
    {
        axis2_status_t (AXIS2_CALL *
        free)(rampart_username_token_t *username_token,
            const axis2_env_t *env);
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
        axis2_status_t (AXIS2_CALL *
        build)(rampart_username_token_t *username_token,
            const axis2_env_t *env,
            axis2_ctx_t *ctx,
            rampart_actions_t *actions,
            axiom_node_t *sec_node,
            axiom_namespace_t *sec_ns_obj
            );
        /**
         * Validates the given username token
         * @param env pointer to environment struct
         * @param msg_ctx axis2 message context
         * @param soap_header SOAP Header
         * @param actions collection of actions
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        validate)(rampart_username_token_t *username_token,
            const axis2_env_t *env,
            axis2_msg_ctx_t *msg_ctx,
            axiom_soap_header_t *soap_header,
            rampart_actions_t *actions);
    };
    
    /**
     * rampart_username_token struct
     */
    struct rampart_username_token
    {
        /** Operations of username_token */
        rampart_username_token_ops_t *ops;
    };

    /**
     * Creates username_token struct
     * @param env pointer to environment struct
     * @return pointer to newly created username_token
     */
    AXIS2_EXTERN rampart_username_token_t *AXIS2_CALL
    rampart_username_token_create (
        const axis2_env_t *env);

/*************************** Function macros **********************************/
#define RAMPART_USERNAME_TOKEN_FREE(username_token, env) \
        ((username_token)->ops->free(username_token, env))    

#define RAMPART_USERNAME_TOKEN_BUILD(username_token, env, ctx, actions, sec_node, sec_ns_obj) \
        ((username_token)->ops->build(username_token, env, ctx, actions, sec_node, sec_ns_obj))    

#define RAMPART_USERNAME_TOKEN_VALIDATE(username_token, env, msg_ctx, soap_header, actions) \
        ((username_token)->ops->validate(username_token, env, msg_ctx, soap_header, actions))    

/** @} */
#ifdef __cplusplus
}
#endif


#endif /*USERNAME_TOKEN_H*/
