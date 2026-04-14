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

#ifndef AXIS2_JSON_STREAMING_MSG_FORMATTER_H
#define AXIS2_JSON_STREAMING_MSG_FORMATTER_H

/**
 * @file axis2_json_streaming_msg_formatter.h
 * @brief Streaming JSON Message Formatter for Axis2/C
 *
 * Drop-in replacement for axis2_json_msg_formatter that writes JSON
 * responses in chunks with periodic flushes. This prevents reverse
 * proxy body-size rejections on large responses by converting a
 * single buffered body into a stream of HTTP/2 DATA frames or
 * HTTP/1.1 chunks.
 *
 * The formatter writes the JSON response in blocks of
 * AXIS2_JSON_STREAMING_FLUSH_INTERVAL bytes (default 64 KB), calling
 * axutil_stream_flush() between blocks. The proxy sees a stream of
 * small chunks, never the full response body.
 *
 * Problem solved (response-side): large JSON responses that cause
 * reverse proxy 502 Bad Gateway due to body-size limits.
 *
 * NOT solved (request-side): large HTTP request bodies are a
 * client-side problem.
 *
 * For the Axis2/Java equivalent, see:
 *   org.apache.axis2.json.streaming.MoshiStreamingMessageFormatter
 *   org.apache.axis2.json.streaming.JSONStreamingMessageFormatter
 *
 * @since 2.0.1
 */

#include <axis2_msg_formatter.h>
#include <axis2_msg_ctx.h>
#include <axutil_env.h>
#include <axutil_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Default flush interval: 64 KB (matches Java FlushingOutputStream) */
    #define AXIS2_JSON_STREAMING_FLUSH_INTERVAL (64 * 1024)

    /** Property name for JSON response (same as non-streaming formatter) */
    #define AXIS2_JSON_STREAMING_RESPONSE_PROPERTY "JSON_RESPONSE"

    /** Content type (same as non-streaming formatter) */
    #define AXIS2_JSON_STREAMING_CONTENT_TYPE "application/json"

    /**
     * @brief Create Streaming JSON Message Formatter
     *
     * The returned formatter is a drop-in replacement for
     * axis2_json_msg_formatter. Configure in axis2.xml:
     *
     *   <messageFormatter contentType="application/json"
     *       class="axis2_json_streaming_msg_formatter"/>
     *
     * @param env Axis2 environment
     * @return Streaming JSON message formatter, or NULL on allocation failure
     */
    AXIS2_EXTERN axis2_msg_formatter_t* AXIS2_CALL
    axis2_json_streaming_msg_formatter_create(const axutil_env_t* env);

    /**
     * @brief Write JSON response to HTTP stream with periodic flushing
     *
     * Writes the JSON response from the MessageContext in chunks of
     * AXIS2_JSON_STREAMING_FLUSH_INTERVAL bytes, calling
     * axutil_stream_flush() between chunks. When running behind
     * Apache httpd with mod_h2, each flush triggers an HTTP/2 DATA
     * frame. With mod_proxy, each flush pushes a chunk to the
     * upstream client.
     *
     * @param msg_formatter  Formatter instance
     * @param env            Axis2 environment
     * @param msg_ctx        Message context (must contain JSON_RESPONSE property)
     * @param out_stream     HTTP transport output stream
     * @param preserve       Ignored (JSON mode, no SOAP headers)
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_json_streaming_msg_formatter_write_to(
        axis2_msg_formatter_t* msg_formatter,
        const axutil_env_t* env,
        axis2_msg_ctx_t* msg_ctx,
        axutil_stream_t* out_stream,
        axis2_bool_t preserve);

    /**
     * @brief Get content type
     * @return "application/json"
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_json_streaming_msg_formatter_get_content_type(
        axis2_msg_formatter_t* msg_formatter,
        const axutil_env_t* env,
        axis2_msg_ctx_t* msg_ctx);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_JSON_STREAMING_MSG_FORMATTER_H */
