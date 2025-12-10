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

#ifndef AXIS2_JSON_RPC_MSG_RECV_NATIVE_H
#define AXIS2_JSON_RPC_MSG_RECV_NATIVE_H

/**
 * @file axis2_json_rpc_msg_recv_native.h
 * @brief Native JSON RPC Message Receiver - Axis2/C equivalent of Axis2/Java JsonRpcMessageReceiver
 *
 * Key Architecture (Following Axis2/Java JsonRpcMessageReceiver):
 * - Check IS_JSON_STREAM property (set by JSONBuilder)
 * - If true: Process pure JSON, bypass all SOAP/XML processing
 * - If false: Fall back to standard SOAP processing (axis2_raw_xml_in_out_msg_recv)
 * - Use enableJSONOnly parameter for JSON-only services
 * - Direct JSON response writing (no SOAP envelope)
 *
 * Performance Benefits:
 * - No SOAP/XML processing overhead for JSON requests
 * - Direct JSON-to-service method invocation
 * - Pure JSON responses without AXIOM conversion
 * - HTTP/2 streaming support
 */

#include <axis2_msg_recv.h>
#include <axis2_msg_ctx.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Create Native JSON RPC Message Receiver
     * Following Axis2/Java JsonRpcMessageReceiver pattern for pure JSON processing
     * @param env Environment
     * @return JSON RPC message receiver instance
     */
    AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
    axis2_json_rpc_msg_recv_native_create(const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_JSON_RPC_MSG_RECV_NATIVE_H */