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

#ifndef AXIS2_JSON_RPC_MSG_RECV_H
#define AXIS2_JSON_RPC_MSG_RECV_H

/**
 * @file axis2_json_rpc_msg_recv.h
 * @brief Axis2/C JSON RPC Message Receiver - Pure JSON Processing (No AXIOM)
 *
 * Following Axis2/Java JsonRpcMessageReceiver pattern:
 * - Processes pure JSON requests without XML conversion
 * - Uses json-c library directly for JSON processing
 * - Bypasses AXIOM entirely when enableJSONOnly=true
 * - Supports URI-based operation dispatching
 */

#include <axis2_msg_recv.h>
#include <axis2_msg_ctx.h>
#include <axutil_env.h>
#include <json-c/json.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief JSON RPC Message Receiver
     * Processes pure JSON requests following Axis2/Java pattern
     */
    typedef struct axis2_json_rpc_msg_recv axis2_json_rpc_msg_recv_t;

    /**
     * @brief Create JSON RPC message receiver
     * Following Axis2/Java JsonRpcMessageReceiver.java pattern
     * @param env Environment
     * @return JSON RPC message receiver instance
     */
    AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
    axis2_json_rpc_msg_recv_create(
        const axutil_env_t* env);

    /**
     * @brief Process JSON request and invoke service (pure JSON, no AXIOM)
     * Following Axis2/Java JsonRpcMessageReceiver.invokeService() pattern
     * @param msg_recv Message receiver
     * @param env Environment
     * @param in_msg_ctx Input message context containing JSON stream
     * @param out_msg_ctx Output message context for JSON response
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_json_rpc_msg_recv_invoke_business_logic(
        axis2_msg_recv_t* msg_recv,
        const axutil_env_t* env,
        axis2_msg_ctx_t* in_msg_ctx,
        axis2_msg_ctx_t* out_msg_ctx);

    /**
     * @brief Check if enableJSONOnly is configured for the service
     * Following Axis2/Java pattern of checking service parameter
     * @param env Environment
     * @param msg_ctx Message context
     * @return AXIS2_TRUE if enableJSONOnly=true, AXIS2_FALSE otherwise
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_json_rpc_msg_recv_is_json_only_enabled(
        const axutil_env_t* env,
        axis2_msg_ctx_t* msg_ctx);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_JSON_RPC_MSG_RECV_H */