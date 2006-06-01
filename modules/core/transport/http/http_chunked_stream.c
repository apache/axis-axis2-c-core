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
 
#include <axis2_http_chunked_stream.h>
#include <axis2_string.h>
#include <string.h>
#include <axis2_http_transport.h>
#include <stdlib.h>

/** 
 * @brief HTTP Chunked Stream struct impl
 *	Axis2 HTTP Chunked Stream impl  
 */
typedef struct axis2_http_chunked_stream_impl axis2_http_chunked_stream_impl_t;  
  
struct axis2_http_chunked_stream_impl
{
	axis2_http_chunked_stream_t chunked_stream;
	axis2_stream_t *stream;
	int current_chunk_size;
	int unread_len;
	axis2_bool_t end_of_chunks;
	axis2_bool_t chunk_started;
};

#define AXIS2_INTF_TO_IMPL(chunked_stream) \
                 ((axis2_http_chunked_stream_impl_t *)(chunked_stream))

/***************************** Function headers *******************************/
int AXIS2_CALL 
axis2_http_chunked_stream_read (axis2_http_chunked_stream_t *chunked_stream,
						const axis2_env_t *env, void *buffer, size_t count);
int AXIS2_CALL 
axis2_http_chunked_stream_write (axis2_http_chunked_stream_t *chunked_stream, 
						const axis2_env_t *env, const void *buffer, 
						size_t count);
int AXIS2_CALL 
axis2_http_chunked_stream_get_current_chunk_size
                        (axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_chunked_stream_start_chunk(
						axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env);
						
axis2_status_t AXIS2_CALL
axis2_http_chunked_stream_write_last_chunk(
						axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_http_chunked_stream_free (axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env);				
/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_http_chunked_stream_t * AXIS2_CALL
axis2_http_chunked_stream_create(const axis2_env_t *env, axis2_stream_t *stream)
{
    axis2_http_chunked_stream_impl_t *chunked_stream_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, stream, NULL);
        
    chunked_stream_impl = (axis2_http_chunked_stream_impl_t *)AXIS2_MALLOC 
                        (env->allocator, sizeof(
                        axis2_http_chunked_stream_impl_t));
	
    if(NULL == chunked_stream_impl)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    chunked_stream_impl->stream = stream;
    chunked_stream_impl->current_chunk_size = -1;
	chunked_stream_impl->unread_len = -1;
	chunked_stream_impl->end_of_chunks = AXIS2_FALSE;
	chunked_stream_impl->chunk_started = AXIS2_FALSE;
	
    chunked_stream_impl->chunked_stream.ops = AXIS2_MALLOC(env->allocator,
        				sizeof(axis2_http_chunked_stream_ops_t));
    if(NULL == chunked_stream_impl->chunked_stream.ops)
	{
		axis2_http_chunked_stream_free((axis2_http_chunked_stream_t*)
                         chunked_stream_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    chunked_stream_impl->chunked_stream.ops->read =
                        axis2_http_chunked_stream_read;
    chunked_stream_impl->chunked_stream.ops->write =
                        axis2_http_chunked_stream_write;
    chunked_stream_impl->chunked_stream.ops->get_current_chunk_size =
                        axis2_http_chunked_stream_get_current_chunk_size;
	chunked_stream_impl->chunked_stream.ops->write_last_chunk =
						axis2_http_chunked_stream_write_last_chunk;
    chunked_stream_impl->chunked_stream.ops->free =
                        axis2_http_chunked_stream_free;
	return &(chunked_stream_impl->chunked_stream);
}

axis2_status_t AXIS2_CALL
axis2_http_chunked_stream_free(axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(NULL != chunked_stream->ops)
        AXIS2_FREE(env->allocator, chunked_stream->ops);
    
	AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(chunked_stream));
	return AXIS2_SUCCESS;
}

int AXIS2_CALL 
axis2_http_chunked_stream_read (axis2_http_chunked_stream_t *chunked_stream,
						const axis2_env_t *env, void *buffer, size_t count)
{
	int len = -1;
	int yet_to_read = 0;
	axis2_http_chunked_stream_impl_t *chunked_stream_impl = AXIS2_INTF_TO_IMPL
						(chunked_stream);
    axis2_stream_t *stream = chunked_stream_impl->stream;
   	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	if(NULL == buffer)
	{
		return -1;
	}
	if(NULL == stream)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM
						, AXIS2_FAILURE);
		return -1;
	}
    if(AXIS2_TRUE == chunked_stream_impl->end_of_chunks)
	{
		return 0;
	}
	if(AXIS2_FALSE == chunked_stream_impl->chunk_started)
	{
		axis2_http_chunked_stream_start_chunk(chunked_stream, env);
	}
	yet_to_read = count;
	while(AXIS2_FALSE == chunked_stream_impl->end_of_chunks && yet_to_read > 0)
	{
		if(chunked_stream_impl->unread_len < yet_to_read)
		{
			len = AXIS2_STREAM_READ(chunked_stream_impl->stream, env, 
						(axis2_char_t*)buffer + count - yet_to_read, 
						chunked_stream_impl->unread_len);
			yet_to_read -= len;
			chunked_stream_impl->unread_len -= len;
			if(chunked_stream_impl->unread_len <= 0)
			{
				axis2_http_chunked_stream_start_chunk(chunked_stream, env);
			}
		}
		else
		{
			len = AXIS2_STREAM_READ(chunked_stream_impl->stream, env, 
						(axis2_char_t*)buffer + count - yet_to_read, 
						yet_to_read);
			yet_to_read -= len;	
			chunked_stream_impl->unread_len -= len;
		}	
	}
    return count - yet_to_read;
}

int AXIS2_CALL
axis2_http_chunked_stream_write (axis2_http_chunked_stream_t *chunked_stream
						, const axis2_env_t *env, const void *buffer, 
						size_t count)
{
	axis2_stream_t *stream = AXIS2_INTF_TO_IMPL(chunked_stream)->stream;
	int len = -1;
	axis2_char_t tmp_buf[10];
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
		
	if(NULL == buffer)
	{
		return -1;
	}
	if(NULL == stream)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM
						, AXIS2_FAILURE);
		return -1;
	}
	sprintf(tmp_buf, "%x%s", count, AXIS2_HTTP_CRLF);
	len = AXIS2_STREAM_WRITE(stream, env, tmp_buf, AXIS2_STRLEN(tmp_buf));
	len = AXIS2_STREAM_WRITE(stream, env, buffer, count);
	AXIS2_STREAM_WRITE(stream, env, AXIS2_HTTP_CRLF, 2);
    return len;
}


int AXIS2_CALL 
axis2_http_chunked_stream_get_current_chunk_size
                        (axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(chunked_stream)->current_chunk_size;
}


axis2_status_t AXIS2_CALL
axis2_http_chunked_stream_start_chunk(
						axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env)
{
	axis2_char_t tmp_buf[3] = "";
	axis2_char_t str_chunk_len[512] = "";
	axis2_char_t *tmp = NULL;
	int read = -1;
	axis2_http_chunked_stream_impl_t *chunked_stream_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	chunked_stream_impl = AXIS2_INTF_TO_IMPL(chunked_stream);
	/* remove the last CRLF of the previous chunk if any */
	if(AXIS2_TRUE == chunked_stream_impl->chunk_started)
	{
		read = AXIS2_STREAM_READ(chunked_stream_impl->stream, env, tmp_buf, 2);
		chunked_stream_impl->chunk_started = AXIS2_FALSE;
	}
	/* read the len and chunk extension */
	while((read = AXIS2_STREAM_READ(chunked_stream_impl->stream, env, tmp_buf, 
						1)) > 0)
	{
		tmp_buf[read] = '\0';
		strcat(str_chunk_len, tmp_buf);
		if(0 != strstr(str_chunk_len, AXIS2_HTTP_CRLF))
		{
			break;
		}
	}
	/* check whether we have extensions */
	tmp = strchr(str_chunk_len, ';');
	if(NULL != tmp)
	{
		/* we don't use extensions right now */
		*tmp = '\0';
	}
	chunked_stream_impl->current_chunk_size = strtol(str_chunk_len, NULL, 16);
	if(0 == chunked_stream_impl->current_chunk_size)
	{
        /* Read the last CRLF */
        read = AXIS2_STREAM_READ(chunked_stream_impl->stream, env, tmp_buf, 2);
		chunked_stream_impl->end_of_chunks = AXIS2_TRUE;
	}
	else
	{
		chunked_stream_impl->chunk_started = AXIS2_TRUE;
		chunked_stream_impl->unread_len = 
						chunked_stream_impl->current_chunk_size;
	}
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_chunked_stream_write_last_chunk(
						axis2_http_chunked_stream_t *chunked_stream, 
                        const axis2_env_t *env)
{
	axis2_stream_t *stream = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	stream = AXIS2_INTF_TO_IMPL(chunked_stream)->stream;
	if(AXIS2_STREAM_WRITE(stream, env, "0\r\n\r\n", 5) == 5)
	{
		return AXIS2_SUCCESS;
	}
	return AXIS2_FAILURE;
}
