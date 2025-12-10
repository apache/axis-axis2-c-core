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

#ifndef AXIS2_H2_TRANSPORT_UTILS_H
#define AXIS2_H2_TRANSPORT_UTILS_H

/* HTTP/2 Pure JSON Architecture - HTTP/2-specific transport utilities header */
/* This header provides JSON-only processing utilities for HTTP/2 transport, */
/* completely eliminating axiom_ and om_ dependencies */

#include <axutil_utils.h>
#include <axutil_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Process HTTP POST request with pure JSON (no SOAP envelope)
 * @param env Environment
 * @param in_stream Input stream containing JSON data
 * @param content_type Content type header value
 * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_h2_transport_utils_process_http_post_request(
    const axutil_env_t* env,
    axutil_stream_t* in_stream,
    const axis2_char_t* content_type);

/**
 * @brief Process HTTP GET request with pure JSON (no SOAP envelope)
 * @param env Environment
 * @param content_type Content type header value
 * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_h2_transport_utils_process_http_get_request(
    const axutil_env_t* env,
    const axis2_char_t* content_type);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_H2_TRANSPORT_UTILS_H */