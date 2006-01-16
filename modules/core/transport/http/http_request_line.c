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
 
#include <axis2_http_request_line.h>
#include <axis2_string.h>
#include <string.h>
#include <axis2_http_transport.h>

/** 
 * @brief HTTP Request Line struct impl
 *	Axis2 HTTP Request Line impl  
 */
typedef struct axis2_http_request_line_impl axis2_http_request_line_impl_t;  
  
struct axis2_http_request_line_impl
{
	axis2_http_request_line_t request_line;
	axis2_char_t *http_version;
    axis2_char_t *method;
    axis2_char_t *uri;
};

#define AXIS2_INTF_TO_IMPL(request_line) \
                 ((axis2_http_request_line_impl_t *)(request_line))

/***************************** Function headers *******************************/

/*  
 *  This is just a function
 *  Not assciated with an ops struct
 */
axis2_http_request_line_t* AXIS2_CALL 
axis2_http_request_line_parse_line(axis2_env_t **env, const axis2_char_t *str);

axis2_char_t* AXIS2_CALL 
axis2_http_request_line_get_method(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_http_request_line_get_http_version
                            (axis2_http_request_line_t *request_line, 
                            axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_http_request_line_get_uri(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_http_request_line_to_string(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_request_line_free(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env);								
/***************************** End of function headers ************************/

axis2_http_request_line_t * AXIS2_CALL 
axis2_http_request_line_create(axis2_env_t **env, axis2_char_t *method, 
                                axis2_char_t *uri, axis2_char_t *http_version)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, method, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    AXIS2_PARAM_CHECK((*env)->error, http_version, NULL);
        
    axis2_http_request_line_impl_t *request_line_impl = 
                        (axis2_http_request_line_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_http_request_line_impl_t));
	
    if(NULL == request_line_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    request_line_impl->method = (axis2_char_t *)AXIS2_STRDUP(method, env);
    request_line_impl->uri = (axis2_char_t *)AXIS2_STRDUP(uri, env);
    request_line_impl->http_version = (axis2_char_t *)AXIS2_STRDUP(
                         http_version, env);
     
    request_line_impl->request_line.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_http_request_line_ops_t));
    if(NULL == request_line_impl->request_line.ops)
	{
		axis2_http_request_line_free((axis2_http_request_line_t*)
                         request_line_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    request_line_impl->request_line.ops->get_method =
                        axis2_http_request_line_get_method;
    request_line_impl->request_line.ops->get_http_version =
                        axis2_http_request_line_get_http_version;
    request_line_impl->request_line.ops->get_uri =
                        axis2_http_request_line_get_uri;
    request_line_impl->request_line.ops->to_string =
                        axis2_http_request_line_to_string;
    request_line_impl->request_line.ops->free =
                        axis2_http_request_line_free;
	return &(request_line_impl->request_line);
}

axis2_status_t AXIS2_CALL
axis2_http_request_line_free(axis2_http_request_line_t *request_line, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(request_line, env, AXIS2_FAILURE);
    axis2_http_request_line_impl_t *request_line_impl =
                        AXIS2_INTF_TO_IMPL(request_line);
    if(NULL != request_line_impl->method)
    {
        AXIS2_FREE((*env)->allocator, request_line_impl->method);
        request_line_impl->method = NULL;
    }
    if(NULL != request_line_impl->uri)
    {
        AXIS2_FREE((*env)->allocator, request_line_impl->uri);
        request_line_impl->uri = NULL;
    }
    if(NULL != request_line_impl->http_version)
    {
        AXIS2_FREE((*env)->allocator, request_line_impl->http_version);
        request_line_impl->http_version = NULL;
    }        
    if(NULL != request_line->ops)
        AXIS2_FREE((*env)->allocator, request_line->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(request_line));
	return AXIS2_SUCCESS;
}

axis2_http_request_line_t* AXIS2_CALL 
axis2_http_request_line_parse_line(axis2_env_t **env, const axis2_char_t *str)
{
    AXIS2_PARAM_CHECK((*env)->error, str, NULL);
    axis2_char_t *tmp = strstr(str, AXIS2_HTTP_CRLF);
    axis2_char_t *req_line = NULL;
    axis2_char_t *method = NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *http_version = NULL;
    axis2_http_request_line_t *ret = NULL;
    
    int i = 0;
    
    if(NULL == tmp)
    {
        AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE, 
                        AXIS2_FAILURE);
        return NULL;
    }
    i = tmp - str;
    req_line = AXIS2_MALLOC((*env)->allocator, i * sizeof(axis2_char_t) +1);
    if(NULL == req_line)
    {
       AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    memcpy(req_line, str, i * sizeof(axis2_char_t));
    req_line[i] = '\0';
    tmp = req_line;
    
    method = tmp;
    tmp = strchr(tmp, ' ');
    if(NULL == tmp)
    {
        AXIS2_FREE((*env)->allocator, req_line);
        AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE, 
                    AXIS2_FAILURE);
        return NULL;
    }
    *tmp++ = '\0';
    uri = tmp;
    tmp = strchr(tmp, ' ');
    if(NULL == tmp)
    {
        AXIS2_FREE((*env)->allocator, req_line);
        AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE, 
                    AXIS2_FAILURE);
        return NULL;
    }
    *tmp++ = '\0';
    http_version = tmp;
    ret = axis2_http_request_line_create(env, method, uri, http_version);
    AXIS2_FREE((*env)->allocator, req_line);
    
    return ret;    
}


axis2_char_t* AXIS2_CALL 
axis2_http_request_line_get_method(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(request_line, env, NULL);
    return AXIS2_INTF_TO_IMPL(request_line)->method;
}


axis2_char_t* AXIS2_CALL 
axis2_http_request_line_get_http_version
                            (axis2_http_request_line_t *request_line, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(request_line, env, NULL);
    return AXIS2_INTF_TO_IMPL(request_line)->http_version;
}


axis2_char_t* AXIS2_CALL 
axis2_http_request_line_get_uri(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(request_line, env, NULL);
    return AXIS2_INTF_TO_IMPL(request_line)->uri;
}


axis2_char_t* AXIS2_CALL 
axis2_http_request_line_to_string(axis2_http_request_line_t *request_line, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(request_line, env, NULL);
    
    axis2_http_request_line_impl_t *req_line_impl = AXIS2_INTF_TO_IMPL(
                            request_line);
    int alloc_len = AXIS2_STRLEN(req_line_impl->method) + 
                    AXIS2_STRLEN(req_line_impl->uri)+
                    AXIS2_STRLEN(req_line_impl->http_version) + 5;
    /* 5 = 2 * spaces + CR + LF + '\0' */

    axis2_char_t *ret = AXIS2_MALLOC((*env)->allocator, 
                    alloc_len * sizeof(axis2_char_t));
    if(NULL == ret)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    sprintf(ret, "%s %s %s%s", req_line_impl->method, req_line_impl->uri, 
                    req_line_impl->http_version, AXIS2_HTTP_CRLF);
    return ret;
}
