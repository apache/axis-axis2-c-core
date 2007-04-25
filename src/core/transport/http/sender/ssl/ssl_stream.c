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
#include "ssl_stream.h"
#include "ssl_utils.h"


/**
 * @brief Stream struct impl
 *   Streaming mechanisms for SSL
 */
typedef struct ssl_stream_impl ssl_stream_impl_t;

struct ssl_stream_impl
{
    axutil_stream_t stream;
    axutil_stream_type_t stream_type;
    SSL *ssl;
    SSL_CTX *ctx;
    axis2_socket_t socket;
};

#define AXIS2_INTF_TO_IMPL(stream) ((ssl_stream_impl_t *)(stream))

void AXIS2_CALL
axis2_ssl_stream_free(
    axutil_stream_t *stream, 
    const axutil_env_t *env
    );

axutil_stream_type_t AXIS2_CALL
axis2_ssl_stream_get_type(
    axutil_stream_t *stream, 
    const axutil_env_t *env
    );

int AXIS2_CALL
axis2_ssl_stream_write(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count
    );

int AXIS2_CALL
axis2_ssl_stream_read(
    axutil_stream_t *stream, 
    const axutil_env_t *env,
    void *buffer,
    size_t count
    );

int AXIS2_CALL
axis2_ssl_stream_skip(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count
    );

int AXIS2_CALL
axis2_ssl_stream_get_char(
    axutil_stream_t *stream,
    const axutil_env_t *env
    );


AXIS2_EXTERN axutil_stream_t * AXIS2_CALL
axutil_stream_create_ssl(
    const axutil_env_t *env,
    axis2_socket_t socket,
    axis2_char_t *server_cert,
    axis2_char_t *key_file,
    axis2_char_t *ssl_pp
    )
{
    ssl_stream_impl_t *stream_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    stream_impl = (ssl_stream_impl_t *)AXIS2_MALLOC(
                env->allocator, sizeof(ssl_stream_impl_t));

    if (! stream_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    stream_impl->socket = socket;
    stream_impl->ctx = NULL;
    stream_impl->ssl = NULL;

    stream_impl->ctx = axis2_ssl_utils_initialize_ctx(env, server_cert,
            key_file, ssl_pp);
    if (! stream_impl->ctx)
    {
        axis2_ssl_stream_free((axutil_stream_t*)stream_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SSL_ENGINE, AXIS2_FAILURE);
        return NULL;
    }
    stream_impl->ssl = axis2_ssl_utils_initialize_ssl(env, stream_impl->ctx,
            stream_impl->socket);
    if (! stream_impl->ssl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SSL_ENGINE, AXIS2_FAILURE);
        return NULL;
    }
    stream_impl->stream_type = AXIS2_STREAM_MANAGED;
    
    axutil_stream_set_read(&(stream_impl->stream), env, axis2_ssl_stream_read);
    axutil_stream_set_write(&(stream_impl->stream), env, axis2_ssl_stream_write);
    axutil_stream_set_skip(&(stream_impl->stream), env, axis2_ssl_stream_skip);

    return &(stream_impl->stream);
}


void AXIS2_CALL
axis2_ssl_stream_free(axutil_stream_t *stream,
    const axutil_env_t *env)
{
    ssl_stream_impl_t *stream_impl = NULL;
    AXIS2_ENV_CHECK(env, void);

    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    axis2_ssl_utils_cleanup_ssl(env, stream_impl->ctx, stream_impl->ssl);
    AXIS2_FREE(env->allocator, stream_impl);

    return;
}

int AXIS2_CALL
axis2_ssl_stream_read(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count
    )
{
    ssl_stream_impl_t *stream_impl = NULL;
    int read = -1;
    int len = -1;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    read = SSL_read(stream_impl->ssl , buffer, count);
    switch (SSL_get_error(stream_impl->ssl , read))
    {
        case SSL_ERROR_NONE:
            len = read;
            break;
        case SSL_ERROR_ZERO_RETURN:
            len = -1;
            break;
        case SSL_ERROR_SYSCALL:
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "SSL Error: Premature close");
            len = -1;
            break;
        default:
            len = -1;
            break;
    }
    return len;
}

int AXIS2_CALL
axis2_ssl_stream_write(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buf,
    size_t count
    )
{
    ssl_stream_impl_t *stream_impl = NULL;
    int write = -1;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    AXIS2_PARAM_CHECK(env->error, buf, AXIS2_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    write = SSL_write(stream_impl->ssl, buf, count);

    switch (SSL_get_error(stream_impl->ssl , write))
    {
        case SSL_ERROR_NONE:
            if (count != write)
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Incomplete SSL write!");
            break;
        default:
            return -1;
    }
    return write;
}


int AXIS2_CALL
axis2_ssl_stream_skip(
    axutil_stream_t *stream, 
    const axutil_env_t *env, 
    int count
    )
{
    ssl_stream_impl_t *stream_impl = NULL;
    axis2_char_t *tmp_buffer = NULL;
    int len = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    tmp_buffer = AXIS2_MALLOC(env->allocator, count * sizeof(axis2_char_t));
    if (tmp_buffer == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                AXIS2_FAILURE);
        return -1;
    }
    len = SSL_read(stream_impl->ssl, tmp_buffer, count);
    AXIS2_FREE(env->allocator, tmp_buffer);
    return len;

}

int AXIS2_CALL
axis2_ssl_stream_get_char(
    axutil_stream_t *stream, 
    const axutil_env_t *env
    )
{
    int ret = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    return ret;
}

axutil_stream_type_t AXIS2_CALL
axis2_ssl_stream_get_type(
    axutil_stream_t *stream,
    const axutil_env_t *env
    )
{
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(stream)->stream_type;
}

