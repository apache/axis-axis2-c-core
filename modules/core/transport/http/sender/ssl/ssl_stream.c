/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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


/** 
 * @brief Stream struct impl
 *	Streaming mechanisms for SSL 
 */
typedef struct ssl_stream_impl ssl_stream_impl_t;
  
struct ssl_stream_impl
{
	axis2_stream_t stream;
    axis2_stream_type_t stream_type;
    SSL *ssl;
    SSL_CTX *ctx;
    axis2_socket_t socket;
};

#define AXIS2_INTF_TO_IMPL(stream) ((ssl_stream_impl_t *)(stream))

/********************************Function headers******************************/
axis2_status_t AXIS2_CALL 
axis2_ssl_stream_free (axis2_stream_t *stream, axis2_env_t **env);

axis2_stream_type_t AXIS2_CALL
axis2_ssl_stream_get_type (axis2_stream_t *stream, axis2_env_t **env);

int AXIS2_CALL
axis2_ssl_stream_write(axis2_stream_t *stream, axis2_env_t **env, 
						const void *buffer, size_t count);
int AXIS2_CALL 
axis2_ssl_stream_read(axis2_stream_t *stream, axis2_env_t **env, 
						void *buffer, size_t count);
int AXIS2_CALL 
axis2_ssl_stream_skip(axis2_stream_t *stream, axis2_env_t **env, int count);

int AXIS2_CALL 
axis2_ssl_stream_get_char(axis2_stream_t *stream, axis2_env_t **env);


AXIS2_DECLARE(axis2_stream_t *)
axis2_stream_create_ssl(axis2_env_t **env, axis2_socket_t socket)
{
    ssl_stream_impl_t *stream_impl = NULL;
	AXIS2_ENV_CHECK(env, NULL);
    	
    stream_impl = (ssl_stream_impl_t *)AXIS2_MALLOC(
                        (*env)->allocator, sizeof(ssl_stream_impl_t));
	
	if(NULL == stream_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    stream_impl->stream.ops = NULL;
    stream_impl->socket = socket;
    stream_impl->ctx = NULL;
    stream_impl->ssl = NULL;
    
    stream_impl->ctx = axis2_ssl_utils_initialize_ctx(env);
    if(NULL == stream_impl->ctx)
    {
        axis2_ssl_stream_free((axis2_stream_t*)stream_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SSL_ENGINE, AXIS2_FAILURE);
        return NULL;
    }
    stream_impl->ssl = axis2_ssl_utils_initialize_ssl(env, stream_impl->ctx, 
                        stream_impl->socket);
    if(NULL == stream_impl->ssl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SSL_ENGINE, AXIS2_FAILURE);
        return NULL;
    }                    
    stream_impl->stream_type = AXIS2_STREAM_MANAGED;
	stream_impl->stream.ops = (axis2_stream_ops_t *) AXIS2_MALLOC (
                        (*env)->allocator, sizeof (axis2_stream_ops_t));
	if (NULL == stream_impl->stream.ops)
	{
		axis2_ssl_stream_free(&(stream_impl->stream), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}
    	
	stream_impl->stream.ops->free = axis2_ssl_stream_free;
    stream_impl->stream.ops->read = axis2_ssl_stream_read;
    stream_impl->stream.ops->write = axis2_ssl_stream_write;
    stream_impl->stream.ops->skip = axis2_ssl_stream_skip;
    stream_impl->stream.ops->get_char = axis2_ssl_stream_get_char;
    stream_impl->stream.ops->get_type = axis2_ssl_stream_get_type;
    
	return &(stream_impl->stream);
}


axis2_status_t AXIS2_CALL
axis2_ssl_stream_free (axis2_stream_t *stream, axis2_env_t **env)
{
    ssl_stream_impl_t *stream_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	stream_impl = AXIS2_INTF_TO_IMPL(stream);
	if (NULL != stream_impl->stream.ops)
    {
        AXIS2_FREE ((*env)->allocator, stream_impl->stream.ops);
    }
    axis2_ssl_utils_cleanup_ssl(env, stream_impl->ctx, stream_impl->ssl);
   	AXIS2_FREE((*env)->allocator, stream_impl);
	
    return AXIS2_SUCCESS;
}

int AXIS2_CALL 
axis2_ssl_stream_read(axis2_stream_t *stream, axis2_env_t **env, 
						void *buffer, size_t count)
{
    ssl_stream_impl_t *stream_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    
    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    return SSL_read(stream_impl->ssl ,buffer,count);
}

int AXIS2_CALL
axis2_ssl_stream_write(axis2_stream_t *stream, axis2_env_t **env, 
						const void *buf, size_t count)
{
	ssl_stream_impl_t *stream_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, buf, AXIS2_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    return SSL_write(stream_impl->ssl, buf, count);

}


int AXIS2_CALL 
axis2_ssl_stream_skip(axis2_stream_t *stream, axis2_env_t **env, int count)
{
	ssl_stream_impl_t *stream_impl = NULL;
    axis2_char_t *tmp_buffer = NULL;
    int len = -1;
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
	stream_impl = AXIS2_INTF_TO_IMPL(stream);
    
    tmp_buffer = AXIS2_MALLOC((*env)->allocator, count * sizeof(axis2_char_t));
    if(tmp_buffer == NULL)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, 
                          AXIS2_FAILURE);
        return -1;
    }
    len = SSL_read(stream_impl->ssl, tmp_buffer, count);
    AXIS2_FREE((*env)->allocator, tmp_buffer);
    return len;
    
}

int AXIS2_CALL 
axis2_ssl_stream_get_char(axis2_stream_t *stream, axis2_env_t **env)
{
	int ret = -1;
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    
    /* TODO implement this */
    return ret;
}

axis2_stream_type_t AXIS2_CALL
axis2_ssl_stream_get_type (axis2_stream_t *stream, axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(stream)->stream_type;
}

