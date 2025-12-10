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

/* HTTP/2 Pure JSON Architecture - HTTP/2-specific transport utilities */
/* This file provides JSON-only processing utilities for HTTP/2 transport, */
/* completely eliminating axiom_ and om_ patterns */

#include <axutil_utils.h>
#include <axutil_stream.h>
#include <json-c/json.h>
#include "axis2_h2_transport_utils.h"

/* HTTP/2 Pure JSON Architecture - Process JSON POST request without SOAP envelope */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_h2_transport_utils_process_http_post_request(
    const axutil_env_t* env,
    axutil_stream_t* in_stream,
    const axis2_char_t* content_type)
{
    axis2_char_t* json_string = NULL;
    json_object* json_obj = NULL;
    int stream_len = 0;

    /* HTTP/2 Pure JSON Architecture - Read JSON directly using json-c, no XML conversion */
    stream_len = axutil_stream_get_len(in_stream, env);
    if (stream_len <= 0)
        return AXIS2_FAILURE;

    json_string = (axis2_char_t*)AXIS2_MALLOC(env->allocator, stream_len + 1);
    if (!json_string)
        return AXIS2_FAILURE;

    if (axutil_stream_read(in_stream, env, json_string, stream_len) != stream_len)
    {
        AXIS2_FREE(env->allocator, json_string);
        return AXIS2_FAILURE;
    }
    json_string[stream_len] = '\0';

    /* Parse JSON using json-c directly */
    json_obj = json_tokener_parse(json_string);
    AXIS2_FREE(env->allocator, json_string);

    if (!json_obj)
        return AXIS2_FAILURE;

    /* HTTP/2 Pure JSON Architecture - No SOAP message context needed for pure JSON processing */
    /* All msg_ctx calls removed - HTTP/2 JSON services handle requests directly without SOAP patterns */

    /* Clean up JSON object */
    json_object_put(json_obj);

    return AXIS2_SUCCESS;
}

/* HTTP/2 Pure JSON Architecture - Process HTTP GET request without SOAP envelope */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_h2_transport_utils_process_http_get_request(
    const axutil_env_t* env,
    const axis2_char_t* content_type)
{
    /* HTTP/2 Pure JSON Architecture - GET requests don't have request body to process */
    /* No SOAP message context needed - HTTP/2 JSON services handle requests directly */

    return AXIS2_SUCCESS;
}