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
 * @file axis2_json_streaming_msg_formatter.c
 * @brief Streaming JSON Message Formatter for Axis2/C
 *
 * Drop-in replacement for axis2_json_msg_formatter that writes JSON
 * responses in chunks with periodic stream flushes. This is the C
 * equivalent of the Java MoshiStreamingMessageFormatter /
 * JSONStreamingMessageFormatter.
 *
 * Architecture:
 *   1. Extract JSON response string from MessageContext property
 *   2. Write it in blocks of AXIS2_JSON_STREAMING_FLUSH_INTERVAL bytes
 *   3. Call axutil_stream_flush() between blocks
 *   4. When running under Apache httpd + mod_h2, each flush triggers
 *      an HTTP/2 DATA frame (via ap_rflush internally)
 *
 * The JSON response is produced by the service handler (e.g.,
 * bigdata_h2_service_handler.c) as a complete json_object string
 * stored in the MessageContext. This formatter does NOT change how
 * the JSON is produced — it only changes how it is delivered to the
 * HTTP transport.
 *
 * @since 2.0.1
 */

#include "axis2_json_streaming_msg_formatter.h"
#include <axutil_property.h>
#include <string.h>

/**
 * @brief Streaming JSON Message Formatter structure
 */
struct axis2_json_streaming_msg_formatter
{
    axis2_msg_formatter_t msg_formatter;
};

/* Forward declarations */
static axis2_status_t AXIS2_CALL
streaming_json_write_to(
    axis2_msg_formatter_t* msg_formatter,
    const axutil_env_t* env,
    axis2_msg_ctx_t* msg_ctx,
    axutil_stream_t* out_stream,
    axis2_bool_t preserve);

static axis2_char_t* AXIS2_CALL
streaming_json_get_content_type(
    axis2_msg_formatter_t* msg_formatter,
    const axutil_env_t* env,
    axis2_msg_ctx_t* msg_ctx);

static void AXIS2_CALL
streaming_json_free(
    axis2_msg_formatter_t* msg_formatter,
    const axutil_env_t* env);

/**
 * @brief Create Streaming JSON Message Formatter
 */
AXIS2_EXTERN axis2_msg_formatter_t* AXIS2_CALL
axis2_json_streaming_msg_formatter_create(const axutil_env_t* env)
{
    struct axis2_json_streaming_msg_formatter* formatter = NULL;
    axis2_msg_formatter_t* msg_formatter = NULL;

    AXIS2_LOG_INFO(env->log,
                   "Creating Streaming JSON Message Formatter (flush interval: %d bytes)",
                   AXIS2_JSON_STREAMING_FLUSH_INTERVAL);

    formatter = (struct axis2_json_streaming_msg_formatter*)AXIS2_MALLOC(
        env->allocator, sizeof(struct axis2_json_streaming_msg_formatter));

    if (!formatter) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    memset(formatter, 0, sizeof(struct axis2_json_streaming_msg_formatter));

    msg_formatter = &(formatter->msg_formatter);

    axis2_msg_formatter_set_write_to(msg_formatter, env, streaming_json_write_to);
    axis2_msg_formatter_set_get_content_type(msg_formatter, env, streaming_json_get_content_type);
    axis2_msg_formatter_set_free(msg_formatter, env, streaming_json_free);

    return msg_formatter;
}

/**
 * @brief Write JSON response in chunks with periodic flushing
 *
 * Instead of writing the entire JSON string in one axutil_stream_write()
 * call (which buffers the full body before sending), this writes in
 * blocks of AXIS2_JSON_STREAMING_FLUSH_INTERVAL bytes and flushes
 * the stream between blocks.
 *
 * When the transport is Apache httpd + mod_axis2:
 *   axutil_stream_write() → ap_rwrite() → bucket brigade append
 *   axutil_stream_flush() → ap_rflush() → brigade flush → mod_h2 DATA frame
 *
 * This means each flush pushes buffered data to the client as an
 * HTTP/2 DATA frame (or HTTP/1.1 chunk). The reverse proxy sees a
 * stream of small chunks, never the full response body.
 */
static axis2_status_t AXIS2_CALL
streaming_json_write_to(
    axis2_msg_formatter_t* msg_formatter,
    const axutil_env_t* env,
    axis2_msg_ctx_t* msg_ctx,
    axutil_stream_t* out_stream,
    axis2_bool_t preserve)
{
    axutil_property_t* json_response_prop = NULL;
    axis2_char_t* json_response = NULL;
    int json_len = 0;
    int offset = 0;
    int total_written = 0;
    int flush_count = 0;

    if (!msg_ctx || !out_stream) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Streaming JSON formatter: msg_ctx or out_stream is null");
        return AXIS2_FAILURE;
    }

    /* Extract JSON response from MessageContext (same as non-streaming) */
    json_response_prop = axis2_msg_ctx_get_property(msg_ctx, env,
        AXIS2_JSON_STREAMING_RESPONSE_PROPERTY);

    if (!json_response_prop) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Streaming JSON formatter: JSON_RESPONSE property not found");

        const axis2_char_t* error_json =
            "{\"error\": \"No JSON response generated by service\"}";
        axutil_stream_write(out_stream, env, error_json, strlen(error_json));
        return AXIS2_FAILURE;
    }

    json_response = (axis2_char_t*)axutil_property_get_value(json_response_prop, env);
    if (!json_response) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Streaming JSON formatter: JSON_RESPONSE value is null");
        return AXIS2_FAILURE;
    }

    json_len = strlen(json_response);

    /* Small responses: write in one shot, no chunking overhead */
    if (json_len <= AXIS2_JSON_STREAMING_FLUSH_INTERVAL) {
        int written = axutil_stream_write(out_stream, env, json_response, json_len);
        if (written > 0) {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "Streaming JSON formatter: wrote %d bytes (small response, no chunking)",
                            written);
            return AXIS2_SUCCESS;
        }
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Streaming JSON formatter: failed to write small response");
        return AXIS2_FAILURE;
    }

    /* Large responses: write in chunks with flush between each */
    AXIS2_LOG_INFO(env->log,
                   "Streaming JSON formatter: streaming %d bytes in %d-byte chunks",
                   json_len, AXIS2_JSON_STREAMING_FLUSH_INTERVAL);

    while (offset < json_len) {
        int chunk_size = json_len - offset;
        if (chunk_size > AXIS2_JSON_STREAMING_FLUSH_INTERVAL) {
            chunk_size = AXIS2_JSON_STREAMING_FLUSH_INTERVAL;
        }

        int written = axutil_stream_write(out_stream, env,
                                          json_response + offset, chunk_size);
        if (written <= 0) {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Streaming JSON formatter: write failed at offset %d "
                            "(wrote %d of %d bytes so far)",
                            offset, total_written, json_len);
            return AXIS2_FAILURE;
        }

        total_written += written;
        offset += written;

        /* Flush after each chunk — triggers HTTP/2 DATA frame via ap_rflush */
        if (offset < json_len) {
            axutil_stream_flush(out_stream, env);
            flush_count++;
        }
    }

    AXIS2_LOG_INFO(env->log,
                   "Streaming JSON formatter: completed %d bytes in %d chunks (%d flushes)",
                   total_written, flush_count + 1, flush_count);

    return AXIS2_SUCCESS;
}

/**
 * @brief Get content type
 */
static axis2_char_t* AXIS2_CALL
streaming_json_get_content_type(
    axis2_msg_formatter_t* msg_formatter,
    const axutil_env_t* env,
    axis2_msg_ctx_t* msg_ctx)
{
    return axutil_strdup(env, AXIS2_JSON_STREAMING_CONTENT_TYPE);
}

/**
 * @brief Free formatter
 */
static void AXIS2_CALL
streaming_json_free(
    axis2_msg_formatter_t* msg_formatter,
    const axutil_env_t* env)
{
    if (msg_formatter) {
        AXIS2_FREE(env->allocator, msg_formatter);
    }
}
