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

#include <axutil_http_chunked_stream.h>
#include <axutil_string.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define AXIS2_HTTP_CRLF "\r\n"

struct axutil_http_chunked_stream
{
    axutil_stream_t *stream;
    int current_chunk_size;
    int unread_len;
    axis2_bool_t end_of_chunks;
    axis2_bool_t chunk_started;
};

static axis2_status_t
axutil_http_chunked_stream_start_chunk(
    axutil_http_chunked_stream_t * chunked_stream,
    const axutil_env_t *env);

AXIS2_EXTERN axutil_http_chunked_stream_t *AXIS2_CALL
axutil_http_chunked_stream_create(
    const axutil_env_t *env,
    axutil_stream_t *stream)
{
    axutil_http_chunked_stream_t *chunked_stream = NULL;
    AXIS2_PARAM_CHECK(env->error, stream, NULL);

    chunked_stream = (axutil_http_chunked_stream_t *)AXIS2_MALLOC(env->allocator,
        sizeof(axutil_http_chunked_stream_t));
    if(!chunked_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    chunked_stream->stream = stream;
    chunked_stream->current_chunk_size = -1;
    chunked_stream->unread_len = -1;
    chunked_stream->end_of_chunks = AXIS2_FALSE;
    chunked_stream->chunk_started = AXIS2_FALSE;

    return chunked_stream;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_http_chunked_stream_free(
    axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env)
{
    AXIS2_FREE(env->allocator, chunked_stream);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_http_chunked_stream_read(
    axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    int len = -1;
    int yet_to_read = 0;
    axutil_stream_t *stream = chunked_stream->stream;

    if(!buffer)
    {
        return -1;
    }
    if(!stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM, AXIS2_FAILURE);
        return -1;
    }
    if(AXIS2_TRUE == chunked_stream->end_of_chunks)
    {
        return 0;
    }
    if(AXIS2_FALSE == chunked_stream->chunk_started)
    {
        axutil_http_chunked_stream_start_chunk(chunked_stream, env);
    }
    yet_to_read = (int)count;
    /* We are sure that the difference lies within the int range */
    while(AXIS2_FALSE == chunked_stream->end_of_chunks && yet_to_read > 0)
    {
        if(chunked_stream->unread_len < yet_to_read)
        {
            len = axutil_stream_read(chunked_stream->stream, env, (axis2_char_t *)buffer + count
                - yet_to_read, chunked_stream->unread_len);
            /* A peer that closes mid-chunk returns 0, and an error returns -1;
             * subtracting either leaves yet_to_read positive -- -1 raises it --
             * so the loop would never finish. */
            if(len <= 0)
            {
                break;
            }
            yet_to_read -= len;
            chunked_stream->unread_len -= len;
            if(chunked_stream->unread_len <= 0)
            {
                axutil_http_chunked_stream_start_chunk(chunked_stream, env);
            }
        }
        else
        {
            len = axutil_stream_read(chunked_stream->stream, env, (axis2_char_t *)buffer + count
                - yet_to_read, yet_to_read);
            if(len <= 0)
            {
                break;
            }
            yet_to_read -= len;
            chunked_stream->unread_len -= len;
        }
    }
    return ((int)count - yet_to_read);
    /* We are sure that the difference lies within the int range */
}

AXIS2_EXTERN int AXIS2_CALL
axutil_http_chunked_stream_write(
    axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count)
{
    axutil_stream_t *stream = chunked_stream->stream;
    int len = -1;
    axis2_char_t tmp_buf[10];

    if(!buffer)
    {
        return -1;
    }
    if(!stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM, AXIS2_FAILURE);
        return -1;
    }
    sprintf(tmp_buf, "%x%s", (unsigned int)count, AXIS2_HTTP_CRLF);
    axutil_stream_write(stream, env, tmp_buf, axutil_strlen(tmp_buf));

    /* The header above has already announced count bytes, so exactly count
     * have to follow it or the framing is wrong for everything after this
     * chunk. axutil_stream_write is under no obligation to take them all in
     * one call -- a socket accepts what fits in its send buffer and reports
     * the rest back -- so keep going until the announced body is complete.
     *
     * Returning a short count instead was worse than it looks: callers cannot
     * recover from it. Two of them respond by calling again with the whole
     * buffer, which emits a second chunk header in the middle of a chunk that
     * was never finished, and repeats if the writes keep coming up short. */
    {
        size_t body_written = 0;

        while(body_written < count)
        {
            len = axutil_stream_write(stream, env, (const axis2_char_t *)buffer + body_written,
                count - body_written);
            if(len <= 0)
            {
                return -1;
            }
            body_written += (size_t)len;
        }
        len = (int)body_written;
    }

    axutil_stream_write(stream, env, AXIS2_HTTP_CRLF, 2);
    return len;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_http_chunked_stream_get_current_chunk_size(
    const axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env)
{
    return chunked_stream->current_chunk_size;
}

static axis2_status_t
axutil_http_chunked_stream_start_chunk(
    axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env)
{
    axis2_char_t tmp_buf[3] = "";
    axis2_char_t str_chunk_len[512] = "";
    axis2_char_t *tmp = NULL;
    size_t len_pos = 0;
    int read = -1;

    /* remove the last CRLF of the previous chunk if any */
    if(AXIS2_TRUE == chunked_stream->chunk_started)
    {
        read = axutil_stream_read(chunked_stream->stream, env, tmp_buf, 2);
        chunked_stream->chunk_started = AXIS2_FALSE;
    }
    /* Read the len and chunk extension.
     *
     * The accumulation is indexed and capped: the old strcat appended into a
     * fixed 512-byte stack buffer for as long as the peer withheld a CRLF, so
     * a long chunk-size line simply ran off the end of it. Reaching the cap
     * without a CRLF is a malformed line, not something to keep reading. */
    while(len_pos + 1 < sizeof(str_chunk_len)
        && (read = axutil_stream_read(chunked_stream->stream, env, tmp_buf, 1)) > 0)
    {
        str_chunk_len[len_pos++] = tmp_buf[0];
        str_chunk_len[len_pos] = '\0';
        if(0 != strstr(str_chunk_len, AXIS2_HTTP_CRLF))
        {
            break;
        }
    }

    if(0 == strstr(str_chunk_len, AXIS2_HTTP_CRLF))
    {
        /* No terminated size line: either the peer went away mid-line or it
         * exceeded the cap. Callers ignore this function's return value, so
         * end the stream here or the read loop keeps calling back. */
        chunked_stream->end_of_chunks = AXIS2_TRUE;
        chunked_stream->current_chunk_size = 0;
        chunked_stream->unread_len = 0;
        return AXIS2_FAILURE;
    }

    /* check whether we have extensions */
    tmp = strchr(str_chunk_len, ';');
    if(tmp)
    {
        /* we don't use extensions right now */
        *tmp = '\0';
    }

    /* strtol accepted a leading '-', so a chunk size of "-1" became a negative
     * current_chunk_size and then a negative read count. Parse unsigned,
     * reject a sign outright, require at least one hex digit, and refuse a
     * value that will not fit the int this is stored in. */
    {
        char *endp = NULL;
        unsigned long parsed = 0;

        if('-' == str_chunk_len[0] || '+' == str_chunk_len[0])
        {
            chunked_stream->end_of_chunks = AXIS2_TRUE;
            chunked_stream->current_chunk_size = 0;
            chunked_stream->unread_len = 0;
            return AXIS2_FAILURE;
        }

        parsed = strtoul(str_chunk_len, &endp, 16);
        if(endp == str_chunk_len || parsed > (unsigned long)INT_MAX)
        {
            chunked_stream->end_of_chunks = AXIS2_TRUE;
            chunked_stream->current_chunk_size = 0;
            chunked_stream->unread_len = 0;
            return AXIS2_FAILURE;
        }
        chunked_stream->current_chunk_size = (int)parsed;
    }
    if(0 == chunked_stream->current_chunk_size)
    {
        /* Read the last CRLF */
        read = axutil_stream_read(chunked_stream->stream, env, tmp_buf, 2);
        chunked_stream->end_of_chunks = AXIS2_TRUE;
    }
    else
    {
        chunked_stream->chunk_started = AXIS2_TRUE;
        chunked_stream->unread_len = chunked_stream->current_chunk_size;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_http_chunked_stream_write_last_chunk(
    axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env)
{
    axutil_stream_t *stream = NULL;

    stream = chunked_stream->stream;
    if(axutil_stream_write(stream, env, "0\r\n\r\n", 5) == 5)
    {
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axutil_http_chunked_stream_get_end_of_chunks(
    axutil_http_chunked_stream_t *chunked_stream,
    const axutil_env_t *env)
{
    return chunked_stream->end_of_chunks;
}

