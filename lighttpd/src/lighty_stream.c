
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

#include <string.h>
#include <stdlib.h>
#include "lighty_stream.h"

typedef struct lighty_stream_impl
{
    axutil_stream_t stream;
    axutil_stream_type_t stream_type;
    connection *con;
} lighty_stream_impl_t;

#define AXIS2_INTF_TO_IMPL(stream) ((lighty_stream_impl_t *)(stream))

axutil_stream_type_t AXIS2_CALL lighty_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env);

int AXIS2_CALL lighty_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buffer,
    size_t count);

int AXIS2_CALL lighty_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count);

int AXIS2_CALL lighty_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count);

int AXIS2_CALL lighty_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env);

AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_lighty(
    const axutil_env_t * env,
    connection *con)
{
    lighty_stream_impl_t *stream_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, con, NULL);

    stream_impl =
        (lighty_stream_impl_t *) AXIS2_MALLOC(env->allocator,
                                               sizeof(lighty_stream_impl_t));

    if (!stream_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(&(stream_impl->stream), 0, sizeof(axutil_stream_t));

    stream_impl->con = con;
    stream_impl->stream_type = AXIS2_STREAM_MANAGED;

    axutil_stream_set_read(&(stream_impl->stream), env, lighty_stream_read);
    axutil_stream_set_write(&(stream_impl->stream), env, lighty_stream_write);
    axutil_stream_set_skip(&(stream_impl->stream), env, lighty_stream_skip);

    return &(stream_impl->stream);
}

int AXIS2_CALL
lighty_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *sbuffer,
    size_t count)
{
    lighty_stream_impl_t *stream_impl = NULL;
    size_t len = 0;
    char *buff = NULL;
    connection *con;
    chunkqueue *cq;
    buffer *b;
    chunk *c;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    buff = (char *)sbuffer;

    con = stream_impl->con;

    cq = con->request_content_queue;

    b = buffer_init ();
    buffer_prepare_copy (b, count);
        
    for (c = cq->first; c; c = c->next) 
    {
        if ((len + c->mem->used) > count)
        {
            buffer_copy_string_len(b, c->mem->ptr, count - len);
            len += (count - len);
            break;
        }

        if (count > len)
        {
            buffer_copy_string_buffer (b, c->mem);
            len += (c->mem->used - 1);
        }

        if (len > count)
            break;
    }
    
    memcpy (buff, b->ptr, len);
    buff [len] = 0;

    AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "read length %d", len);
    return len;
}

int AXIS2_CALL
lighty_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buf,
    size_t count)
{
    lighty_stream_impl_t *stream_impl = NULL;
    axis2_char_t *sbuffer = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    AXIS2_PARAM_CHECK(env->error, buf, AXIS2_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    sbuffer = (axis2_char_t *) buf;
    if (count <= 0)
    {
        AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "read length count");
        return (int)count;
        /* We are sure that the difference lies within the int range */
    }
    /* assume that sbuffer is not null terminated */
    AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "write length out");
    return 0;
    /* We are sure that the difference lies within the int range */
}

int AXIS2_CALL
lighty_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count)
{
    lighty_stream_impl_t *stream_impl = NULL;
    axis2_char_t *tmp_buffer = NULL;
    int len = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    return len;

}

int AXIS2_CALL
lighty_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    int ret = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    return ret;
}

axutil_stream_type_t AXIS2_CALL
lighty_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(stream)->stream_type;
}


