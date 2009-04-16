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
#ifdef AXIS2_LIBCURL_ENABLED

#include "libcurl_stream.h"
#include <string.h>

typedef struct libcurl_stream_impl
{
    axutil_stream_t stream;
    axutil_stream_type_t stream_type;
    axis2_char_t *buffer;
    int size;
    int read_len;
}
libcurl_stream_impl_t;

#define AXIS2_INTF_TO_IMPL(stream) ((libcurl_stream_impl_t *)(stream))

/********************************Function headers******************************/
axutil_stream_type_t AXIS2_CALL libcurl_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env);

int AXIS2_CALL libcurl_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buffer,
    size_t count);

int AXIS2_CALL libcurl_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count);

int AXIS2_CALL libcurl_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count);

int AXIS2_CALL libcurl_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env);

/************************* End of function headers ****************************/

/*
 * Internal function. Not exposed to outside
 */
AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_libcurl(
    const axutil_env_t * env,
    axis2_char_t * buffer,
    unsigned int size)
{
    libcurl_stream_impl_t *stream_impl = NULL;

    AXIS2_PARAM_CHECK(env->error, buffer, NULL);

    stream_impl =
        (libcurl_stream_impl_t *) AXIS2_MALLOC(env->allocator,
                                               sizeof(libcurl_stream_impl_t));

    if (!stream_impl)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    stream_impl->buffer = buffer;
    stream_impl->size = size;
    stream_impl->read_len = 0;
    stream_impl->stream_type = AXIS2_STREAM_MANAGED;

    axutil_stream_set_read(&(stream_impl->stream), env, libcurl_stream_read);
    axutil_stream_set_write(&(stream_impl->stream), env, libcurl_stream_write);
    axutil_stream_set_skip(&(stream_impl->stream), env, libcurl_stream_skip);

    return &(stream_impl->stream);
}

void AXIS2_CALL
libcurl_stream_free(
    void * stream,
    const axutil_env_t * env)
{
    libcurl_stream_impl_t *stream_impl = NULL;

    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    AXIS2_FREE(env->allocator, stream_impl);

    return;
}

int AXIS2_CALL
libcurl_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count)
{
    libcurl_stream_impl_t *stream_impl = NULL;
    int read = 0;
    int unread = 0;

    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    if (stream_impl->size >= (int)count)
    /* We are sure that the difference lies within the int range */
    {
        if (buffer && (stream_impl->size > stream_impl->read_len))
        {
            unread = (stream_impl->size - stream_impl->read_len);
            if (unread > (int)count)
            /* We are sure that the difference lies within the int range */
            {
                memcpy(buffer, &stream_impl->buffer[stream_impl->read_len],
                       count);
                read = (int)count;
                /* We are sure that the difference lies within the int range */
                stream_impl->read_len += read;
            }
            else
            {
                memcpy(buffer, &stream_impl->buffer[stream_impl->read_len],
                       unread);
                read = unread;
                stream_impl->read_len += read;
            }
        }
        else
            read = 0;
    }
    else
    {
        if (buffer && (stream_impl->size > stream_impl->read_len))
        {
            memcpy(buffer, &stream_impl->buffer[stream_impl->read_len],
                   stream_impl->size - stream_impl->read_len);
            read = stream_impl->size - stream_impl->read_len;
            stream_impl->read_len += read;
        }
        else
            read = 0;
    }
    return read;
}

int AXIS2_CALL
libcurl_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buffer,
    size_t count)
{
    return (int)count;
    /* We are sure that the difference lies within the int range */
}

int AXIS2_CALL
libcurl_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count)
{
    return 0;
}

int AXIS2_CALL
libcurl_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    return 0;
}

#endif                          /* AXIS2_LIBCURL_ENABLED */

