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

/**
 * @file axis2_json_rpc_msg_recv.h
 * @brief Revolutionary JsonRpcMessageReceiver - Core Axis2/C Framework Component
 *
 * Equivalent of Axis2/Java's org.apache.axis2.json.gson.rpc.JsonRpcMessageReceiver
 * This is a CORE Axis2/C framework component that any service can use for JSON processing.
 */

#ifndef AXIS2_JSON_RPC_MSG_RECV_H
#define AXIS2_JSON_RPC_MSG_RECV_H

#include <axis2_defines.h>
#include <axutil_env.h>
#include <axis2_msg_recv.h>
#include <axis2_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Type name for struct axis2_json_rpc_msg_recv */
typedef struct axis2_json_rpc_msg_recv axis2_json_rpc_msg_recv_t;

/**
 * Create Revolutionary JsonRpcMessageReceiver (Core Framework Component)
 *
 * This is the Axis2/C equivalent of Axis2/Java's JsonRpcMessageReceiver that:
 * - Completely bypasses SOAP/AXIOM processing
 * - Provides direct JSON-to-POJO conversion
 * - Supports HTTP/2 optimized streaming
 * - Works with any service that has JSON operations
 * - Part of core Axis2/C framework (not service-specific)
 *
 * Revolutionary Features:
 * - Pure JSON request/response processing
 * - Direct service method invocation with JSON
 * - HTTP/2 streaming optimization for large payloads
 * - Framework-level JSON support (like Axis2/Java)
 *
 * @param env Environment. Must not be NULL
 * @return pointer to newly created JsonRpcMessageReceiver or NULL on error
 */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_json_rpc_msg_recv_create(const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_JSON_RPC_MSG_RECV_H */