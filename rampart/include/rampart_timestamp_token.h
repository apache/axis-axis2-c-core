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

#ifndef TIMESTAMP_TOKEN_H
#define TIMESTAMP_TOKEN_H


#ifdef __cplusplus
extern "C"
{
#endif

#define RAMPART_TIMESTAMP_TOKEN_DEFAULT_TIME_TO_LIVE 300    
 
#include <axis2_env.h>
#include <rampart_constants.h>

    /** Type name for struct rampart_timestamp_token_ops */
    typedef struct rampart_timestamp_token_ops rampart_timestamp_token_ops_t;
    /** Type name for struct rampart_timestamp_token */
    typedef struct rampart_timestamp_token rampart_timestamp_token_t;
    /**
     * Test ops struct
     * Encapsulator struct for ops of rampart_timestamp_token
     */
    struct rampart_timestamp_token_ops
    {
        axis2_status_t (AXIS2_CALL *
        free)(rampart_timestamp_token_t *timestamp_token,
            const axis2_env_t *env);
        /**
         * Builds timestamp token.
         * @param env pointer to environment struct
         * @param ctx axis2 context
         * @param sec_node security node
         * @param sec_ns_obj Security namespace object
         * @param ttl Time to live. The time difference btwn Created and Expired
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
       
        axis2_status_t (AXIS2_CALL *
        build)(rampart_timestamp_token_t *timestamp_token,
            const axis2_env_t *env,
            const axis2_ctx_t *ctx,
            axiom_node_t *sec_node,
            const  axiom_namespace_t *sec_ns_obj,
            int ttl
            );
        /**
         * Validates time stamp token. Validation is based in expiration time of the
         * Expired element.
         * @param env pointer to environment struct
         * @param ts_node Timestamp node
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        validate)(rampart_timestamp_token_t *timestamp_token,
            const axis2_env_t *env,
            axis2_msg_ctx_t *msg_ctx,
            axiom_node_t *ts_node,
            axis2_array_list_t *sub_codes);
    };

    /**
     * rampart_timestamp_token struct
     */
    struct rampart_timestamp_token
    {
        /** Operations of timestamp_token */
        rampart_timestamp_token_ops_t *ops;
    };

    /**
     * Creates timestamp_token struct
     * @param env pointer to environment struct
     * @return pointer to newly created timestamp_token
     */
    AXIS2_EXTERN rampart_timestamp_token_t *AXIS2_CALL
    rampart_timestamp_token_create (
        const axis2_env_t *env);

/*************************** Function macros **********************************/
#define RAMPART_TIMESTAMP_TOKEN_FREE(timestamp_token, env) \
        ((timestamp_token)->ops->free(timestamp_token, env))

#define RAMPART_TIMESTAMP_TOKEN_BUILD(timestamp_token, env, ctx, sec_node, sec_ns_obj, ttl) \
        ((timestamp_token)->ops->build(timestamp_token, env, ctx,  sec_node, sec_ns_obj, ttl))

#define RAMPART_TIMESTAMP_TOKEN_VALIDATE(timestamp_token, env, msg_ctx, ts_node, sub_codes) \
        ((timestamp_token)->ops->validate(timestamp_token, env, msg_ctx, ts_node, sub_codes))

/** @} */
#ifdef __cplusplus
}
#endif


#endif /*TIMESTAMP_TOKEN_H*/
