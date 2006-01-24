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
 
#include <axis2_http_response_writer.h>
#include <axis2_array_list.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>

/** 
 * @brief HTTP Response Writer struct impl
 *	Axis2 HTTP Response Writer impl  
 */
typedef struct axis2_http_response_writer_impl 
                    axis2_http_response_writer_impl_t;  
  
struct axis2_http_response_writer_impl
{
	axis2_http_response_writer_t response_writer;
    axis2_stream_t *stream;
    axis2_char_t *encoding;
};

#define AXIS2_INTF_TO_IMPL(response_writer) \
                 ((axis2_http_response_writer_impl_t *)(response_writer))

/***************************** Function headers *******************************/
axis2_char_t* AXIS2_CALL 
axis2_http_response_writer_get_encoding
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_close(axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_flush(axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_write_char
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char c);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_write_buf
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char *buf, int offset, 
                axis2_ssize_t len);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_print_str
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char *str);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_print_int
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, int i);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_println_str
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char *str);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_println
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_response_writer_free(axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env);

								
/***************************** End of function headers ************************/
axis2_http_response_writer_t* AXIS2_CALL
axis2_http_response_writer_create (axis2_env_t **env, axis2_stream_t *stream)
{
    AXIS2_FUNC_PARAM_CHECK(stream, env, NULL);
    return axis2_http_response_writer_create_with_encoding(env, stream, 
                AXIS2_HTTP_DEFAULT_CONTENT_CHARSET);
    
}

axis2_http_response_writer_t* AXIS2_CALL
axis2_http_response_writer_create_with_encoding(axis2_env_t **env,
                    axis2_stream_t *stream, const axis2_char_t *encoding)
{
    AXIS2_FUNC_PARAM_CHECK(stream, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, encoding, NULL);
    
    axis2_http_response_writer_impl_t *response_writer_impl = 
                        (axis2_http_response_writer_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_http_response_writer_impl_t));
	
    if(NULL == response_writer_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    response_writer_impl->stream = stream;
    response_writer_impl->encoding = (axis2_char_t *)AXIS2_STRDUP(encoding, env);
    
    response_writer_impl->response_writer.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_http_response_writer_ops_t));
    if(NULL == response_writer_impl->response_writer.ops)
	{
		axis2_http_response_writer_free((axis2_http_response_writer_t*)
                                        response_writer_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    response_writer_impl->response_writer.ops->get_encoding = 
                        axis2_http_response_writer_get_encoding;
    response_writer_impl->response_writer.ops->close =
                        axis2_http_response_writer_close;
    response_writer_impl->response_writer.ops->flush =
                        axis2_http_response_writer_flush;
    response_writer_impl->response_writer.ops->write_char = 
                        axis2_http_response_writer_write_char;
    response_writer_impl->response_writer.ops->write_buf =
                        axis2_http_response_writer_write_buf;
    response_writer_impl->response_writer.ops->print_str =
                        axis2_http_response_writer_print_str;
    response_writer_impl->response_writer.ops->print_int =
                        axis2_http_response_writer_print_int;
    response_writer_impl->response_writer.ops->println_str =
                        axis2_http_response_writer_println_str;
    response_writer_impl->response_writer.ops->println =
                        axis2_http_response_writer_println;
    response_writer_impl->response_writer.ops->free =
                        axis2_http_response_writer_free;
                        
	return &(response_writer_impl->response_writer);
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_free(axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    AXIS2_FREE((*env)->allocator, 
                AXIS2_INTF_TO_IMPL(response_writer)->encoding);
    
    if(NULL != response_writer->ops)
        AXIS2_FREE((*env)->allocator, response_writer->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(response_writer));
    
	return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL 
axis2_http_response_writer_get_encoding
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, NULL);
    return AXIS2_INTF_TO_IMPL(response_writer)->encoding;
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_close(axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_flush(axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    /*
        TODO if bufferring is added flush the buffer
    */
    return AXIS2_FALSE;
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_write_char
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char c)
{
	axis2_http_response_writer_impl_t *writer_impl = NULL;
	int write = -1;
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
	
	writer_impl = AXIS2_INTF_TO_IMPL(response_writer);
	if(NULL == writer_impl->stream)
	{
		return AXIS2_FAILURE;
	}
    write = AXIS2_STREAM_WRITE(writer_impl->stream, env, &c, 1);
	if(write < 0)
	{
		return AXIS2_FAILURE;
	}
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_write_buf
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char *buf, int offset, 
                axis2_ssize_t len)
{
	axis2_http_response_writer_impl_t *writer_impl = NULL;
	int write = -1;
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, buf, AXIS2_FAILURE);
	
    writer_impl = AXIS2_INTF_TO_IMPL(response_writer);
	if(NULL == writer_impl->stream)
	{
		return AXIS2_FAILURE;
	}
    write = AXIS2_STREAM_WRITE(writer_impl->stream, env, buf, len);
	if(write < 0)
	{
		return AXIS2_FAILURE;
	}
	/*
        TODO handle offset
    */
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_print_str
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char *str)
{
	axis2_http_response_writer_impl_t *writer_impl = NULL;
	int write = -1;
	int len = -1;
	
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, str, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(response_writer);
	
	len = AXIS2_STRLEN(str);
	if(NULL == writer_impl->stream)
	{
		return AXIS2_FAILURE;
	}
    write = AXIS2_STREAM_WRITE(writer_impl->stream, env, str, len);
	if(write < 0)
	{
		return AXIS2_FAILURE;
	}
    return AXIS2_SUCCESS;   
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_print_int
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, int i)
{
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    axis2_char_t int_str[10];
    sprintf(int_str, "%10d", i);
    return axis2_http_response_writer_print_str(response_writer, env, int_str);
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_println_str
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env, char *str)
{
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, str, AXIS2_FAILURE);
    
    if (AXIS2_SUCCESS == axis2_http_response_writer_print_str(response_writer, 
                env, str))
    {
        return axis2_http_response_writer_print_str(response_writer, env, 
                AXIS2_HTTP_CRLF);
    }
    return AXIS2_FAILURE;
}


axis2_status_t AXIS2_CALL 
axis2_http_response_writer_println
                (axis2_http_response_writer_t *response_writer, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(response_writer, env, AXIS2_FAILURE);
    return axis2_http_response_writer_print_str(response_writer, env, 
                AXIS2_HTTP_CRLF);
}
