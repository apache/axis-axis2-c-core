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
 * @file axis2_native_json_msg_recv.h
 * @brief Native JSON Message Receiver Header
 *
 * Revolutionary AXIOM-Free JSON processing for Axis2/C following the Axis2/Java Native approach.
 */

#ifndef AXIS2_NATIVE_JSON_MSG_RECV_H
#define AXIS2_NATIVE_JSON_MSG_RECV_H

#include <axis2_msg_recv.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Create Native JSON Message Receiver
 *
 * Revolutionary approach that eliminates AXIOM/SOAP overhead by using:
 * - axis2_msg_ctx_set_doing_json() native flag
 * - Direct JSON stream processing
 * - Pure JSON responses without SOAP envelopes
 * - HTTP/2 optimized for large payloads
 *
 * @param env Environment. Must not be NULL
 * @return pointer to newly created message receiver or NULL on error
 */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_native_json_msg_recv_create(const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_NATIVE_JSON_MSG_RECV_H */