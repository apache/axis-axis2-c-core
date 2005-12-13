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
 
#include <axis2_http_simple_request.h>
#include <axis2_array_list.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>
#include <string.h>

/** 
 * @brief HTTP Simple Request struct impl
 *	Axis2 HTTP Simple Request impl  
 */
typedef struct axis2_http_simple_request_impl axis2_http_simple_request_impl_t;  
  
struct axis2_http_simple_request_impl
{
	axis2_http_simple_request_t simple_request;
	axis2_http_request_line_t *request_line;
    axis2_array_list_t *header_group;
    axis2_stream_t *stream;
};

#define AXIS2_INTF_TO_IMPL(simple_request) \
                 ((axis2_http_simple_request_impl_t *)(simple_request))

/***************************** Function headers *******************************/

axis2_http_request_line_t* AXIS2_CALL 
axis2_http_simple_request_get_request_line
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_simple_request_set_request_line
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, 
                        axis2_http_request_line_t *request_line);

axis2_bool_t AXIS2_CALL 
axis2_http_simple_request_contains_header
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, axis2_char_t *name);

axis2_array_list_t* AXIS2_CALL 
axis2_http_simple_request_get_headers
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);

axis2_http_header_t* AXIS2_CALL 
axis2_http_simple_request_get_first_header
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, const axis2_char_t *str);

axis2_status_t AXIS2_CALL 
axis2_http_simple_request_remove_headers
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, const axis2_char_t *str);

axis2_status_t AXIS2_CALL 
axis2_http_simple_request_add_header
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, axis2_http_header_t *header);

axis2_char_t* AXIS2_CALL 
axis2_http_simple_request_get_content_type
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_http_simple_request_get_charset
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);

axis2_ssize_t AXIS2_CALL 
axis2_http_simple_request_get_content_length
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);

axis2_stream_t* AXIS2_CALL 
axis2_http_simple_request_get_body(axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);

axis2_ssize_t AXIS2_CALL 
axis2_http_simple_request_get_body_bytes
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, axis2_char_t *buf);

axis2_status_t AXIS2_CALL 
axis2_http_simple_request_free(axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env);
								
/***************************** End of function headers ************************/

axis2_http_simple_request_t * AXIS2_CALL 
axis2_http_simple_request_create 
                        (axis2_env_t **env,
                        axis2_http_request_line_t *request_line,
                        axis2_http_header_t **http_headers,
                        axis2_ssize_t http_hdr_count,
                        axis2_stream_t *content)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_FUNC_PARAM_CHECK(request_line, env, NULL);
    
    axis2_http_simple_request_impl_t *simple_request_impl = 
                        (axis2_http_simple_request_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_http_simple_request_impl_t));
	
    if(NULL == simple_request_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    simple_request_impl->request_line = request_line;
    simple_request_impl->stream = content;
    simple_request_impl->header_group = NULL;
    if(http_hdr_count > 0 && NULL != http_headers)
    {
        simple_request_impl->header_group = axis2_array_list_create(env, 
                        http_hdr_count);
        int i = 0;
        for(i = 0; i < http_hdr_count; i++)
        {
            AXIS2_ARRAY_LIST_ADD(simple_request_impl->header_group, env, 
                        (void*)http_headers[i]);
        }
    }
    /*
        TODO : handle chunked streams
    */
    simple_request_impl->simple_request.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_http_simple_request_ops_t));
    if(NULL == simple_request_impl->simple_request.ops)
	{
		axis2_http_simple_request_free((axis2_http_simple_request_t*)
                        simple_request_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    simple_request_impl->simple_request.ops->get_request_line = 
                        axis2_http_simple_request_get_request_line;
    simple_request_impl->simple_request.ops->set_request_line =
                        axis2_http_simple_request_set_request_line;
    simple_request_impl->simple_request.ops->contains_header = 
                        axis2_http_simple_request_contains_header;
    simple_request_impl->simple_request.ops->get_headers = 
                        axis2_http_simple_request_get_headers;
    simple_request_impl->simple_request.ops->get_first_header =
                        axis2_http_simple_request_get_first_header;
    simple_request_impl->simple_request.ops->remove_headers =
                        axis2_http_simple_request_remove_headers;
    simple_request_impl->simple_request.ops->add_header = 
                        axis2_http_simple_request_add_header;
    simple_request_impl->simple_request.ops->get_content_type =
                        axis2_http_simple_request_get_content_type;
    simple_request_impl->simple_request.ops->get_charset =
                        axis2_http_simple_request_get_charset;
    simple_request_impl->simple_request.ops->get_content_length =
                        axis2_http_simple_request_get_content_length;
    simple_request_impl->simple_request.ops->get_body =
                        axis2_http_simple_request_get_body;
    simple_request_impl->simple_request.ops->get_body_bytes =
                        axis2_http_simple_request_get_body_bytes;
    simple_request_impl->simple_request.ops->free = 
                        axis2_http_simple_request_free;
    
	return &(simple_request_impl->simple_request);
}

axis2_status_t AXIS2_CALL 
axis2_http_simple_request_free(axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    axis2_http_simple_request_impl_t *simple_request_impl = 
                        AXIS2_INTF_TO_IMPL(simple_request);
    /*
        Don't free the stream since it belongs to the socket
        TODO : if chunked remove the chunked stream.
    */
    if(NULL != simple_request_impl->request_line)
    {
        AXIS2_HTTP_REQUEST_LINE_FREE(simple_request_impl->request_line, env);
        simple_request_impl->request_line = NULL;
    }
    if(NULL != simple_request_impl->header_group)
    {
        int i = 0;
        axis2_http_header_t *tmp = NULL;
        for(i = 0; i< AXIS2_ARRAY_LIST_SIZE(simple_request_impl->header_group, 
                        env); i++)
        {
            tmp = (axis2_http_header_t *)AXIS2_ARRAY_LIST_REMOVE(
                            simple_request_impl->header_group, env, i);
            AXIS2_HTTP_HEADER_FREE(tmp, env);        
        }
        AXIS2_ARRAY_LIST_FREE(simple_request_impl->header_group, env);
        simple_request_impl->header_group = NULL;
    }
    
    if(NULL != simple_request->ops)
        AXIS2_FREE((*env)->allocator, simple_request->ops);
    
	AXIS2_FREE((*env)->allocator, simple_request_impl);
    
	return AXIS2_SUCCESS;
}

axis2_http_request_line_t* AXIS2_CALL 
axis2_http_simple_request_get_request_line
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_request)->request_line;
}


axis2_status_t AXIS2_CALL 
axis2_http_simple_request_set_request_line
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, 
                        axis2_http_request_line_t *request_line)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, request_line, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(simple_request)->request_line = request_line;
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL 
axis2_http_simple_request_contains_header
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    axis2_http_simple_request_impl_t *simple_request_impl = AXIS2_INTF_TO_IMPL(
                        simple_request);                 
    if(NULL == simple_request_impl->header_group)
    {
        return AXIS2_FALSE;
    }
    int count = AXIS2_ARRAY_LIST_SIZE(simple_request_impl->header_group, env);
    if(0 == count)
    {
        return AXIS2_FALSE;
    }
    int i = 0;
    axis2_char_t *header_name = NULL;
    for(i = 0; i < count; i++)
    {
        header_name = AXIS2_HTTP_HEADER_GET_NAME((axis2_http_header_t *)
                        AXIS2_ARRAY_LIST_GET(simple_request_impl->header_group,
                        env, i), env);
        if(0 == AXIS2_STRCASECMP(name,header_name))
            return AXIS2_TRUE;            
    }
    return AXIS2_FALSE;
}


axis2_array_list_t* AXIS2_CALL 
axis2_http_simple_request_get_headers
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_request)->header_group;
}


axis2_http_header_t* AXIS2_CALL 
axis2_http_simple_request_get_first_header
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, const axis2_char_t *str)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, str, NULL);
    
    axis2_http_simple_request_impl_t *simple_request_impl = 
                        AXIS2_INTF_TO_IMPL(simple_request);
    
    axis2_array_list_t *header_group = simple_request_impl->header_group;
    if(NULL == simple_request_impl->header_group)
    {
        return NULL;
    }
    if(0 == AXIS2_ARRAY_LIST_SIZE(header_group, env))
    {
        return NULL;
    }
    
    int i = 0;
    int count = AXIS2_ARRAY_LIST_SIZE(header_group, env);
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    
    for(i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(header_group, 
                        env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if(0 == AXIS2_STRCASECMP(str, tmp_name))
        {
            return tmp_header;
        }
    }
    return NULL;
}


axis2_status_t AXIS2_CALL 
axis2_http_simple_request_remove_headers
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, const axis2_char_t *str)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, str, AXIS2_FAILURE);
    
    axis2_array_list_t *header_group = AXIS2_INTF_TO_IMPL(simple_request)->
                        header_group;
    if(NULL == header_group)
    {
        /* Even though we couldn't complete the op, we are sure that the 
         * requred header is no more in the request. So we can proceed without a
         * problem.
         */
        return AXIS2_SUCCESS;
    }
    int i = 0;
    int count = AXIS2_ARRAY_LIST_SIZE(header_group, env);
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    
    for(i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(header_group, 
                        env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if(0 == AXIS2_STRCASECMP(str, tmp_name))
        {
            AXIS2_HTTP_HEADER_FREE(tmp_header, env);
            AXIS2_ARRAY_LIST_REMOVE(header_group, env, i);
            break;
        }
    }
    return AXIS2_SUCCESS;    
}


axis2_status_t AXIS2_CALL 
axis2_http_simple_request_add_header
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, axis2_http_header_t *header)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, header, AXIS2_FAILURE);
    
    axis2_http_simple_request_impl_t *simple_request_impl = AXIS2_INTF_TO_IMPL(
                        simple_request);
    if(NULL == simple_request_impl->header_group)
    {
        simple_request_impl->header_group = axis2_array_list_create(env, 1);
    }
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(simple_request)->header_group,
                        env, header);
}


axis2_char_t* AXIS2_CALL 
axis2_http_simple_request_get_content_type
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, NULL);
    axis2_http_header_t *tmp_header = axis2_http_simple_request_get_first_header
                        (simple_request, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if(NULL != tmp_header)
        return AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);
    
    return AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
}


axis2_char_t* AXIS2_CALL 
axis2_http_simple_request_get_charset
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, NULL);
    axis2_http_header_t *tmp_header = axis2_http_simple_request_get_first_header
                        (simple_request, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if(NULL != tmp_header)
    {
        axis2_char_t *value = AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);
        axis2_char_t *charset = (axis2_char_t *)strstr((char *)value, 
                        (char *)AXIS2_HTTP_CHAR_SET_ENCODING);
        if(NULL != charset)
        {
            charset = strchr((char *)charset, '=');
            return charset;
        }
    }
    
    return AXIS2_HTTP_DEFAULT_CONTENT_CHARSET;
}


axis2_ssize_t AXIS2_CALL 
axis2_http_simple_request_get_content_length
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    axis2_http_header_t *tmp_header = axis2_http_simple_request_get_first_header
                        (simple_request, env, AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if(NULL != tmp_header)
    {
        return atoi(AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env));
    }
    return -1;
}


axis2_stream_t* AXIS2_CALL 
axis2_http_simple_request_get_body(axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_request)->stream;
}


axis2_ssize_t AXIS2_CALL 
axis2_http_simple_request_get_body_bytes
                        (axis2_http_simple_request_t *simple_request, 
                        axis2_env_t **env, axis2_char_t *buf)
{
    AXIS2_FUNC_PARAM_CHECK(simple_request, env, AXIS2_FAILURE);
    /*
        TODO stream dependent
    */
    return -1;    
}
