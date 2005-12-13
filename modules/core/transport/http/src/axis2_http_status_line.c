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
 
#include <axis2_http_status_line.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>
#include <string.h>

/** 
 * @brief HTTP Status Line struct impl
 *	Axis2 HTTP Status Line impl  
 */
typedef struct axis2_http_status_line_impl axis2_http_status_line_impl_t;  
  
struct axis2_http_status_line_impl
{
	axis2_http_status_line_t status_line;
    axis2_char_t *line;
	axis2_char_t *http_version;
    axis2_char_t *status_code;
    axis2_char_t *reason_phrase;
};

#define AXIS2_INTF_TO_IMPL(status_line) \
                    ((axis2_http_status_line_impl_t *)(status_line))

/***************************** Function headers *******************************/
int AXIS2_CALL 
axis2_http_status_line_get_status_code (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_http_status_line_get_http_version (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_http_status_line_get_reason_phrase (axis2_http_status_line_t *status_line,
                    axis2_env_t **env);

axis2_bool_t AXIS2_CALL 
axis2_http_status_line_starts_with_http (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_http_status_line_to_string (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_http_status_line_free (axis2_http_status_line_t *status_line, 
                                    axis2_env_t **env);	
/***************************** End of function headers ************************/

axis2_http_status_line_t * AXIS2_CALL 
axis2_http_status_line_create(axis2_env_t **env, axis2_char_t *str)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_FUNC_PARAM_CHECK(str, env, NULL);
    axis2_char_t *tmp_status_line = NULL;
    axis2_char_t *reason_phrase = NULL;
    axis2_char_t *status_code= NULL;
    axis2_char_t *http_version = NULL;
    int i = 0;
    axis2_char_t *tmp = NULL;
    
    axis2_http_status_line_impl_t *status_line_impl = 
                        (axis2_http_status_line_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_http_status_line_impl_t));
	
    if(NULL == status_line_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    status_line_impl->line = (axis2_char_t *)AXIS2_STRDUP(str, env);
    status_line_impl->http_version = NULL;
    status_line_impl->reason_phrase = NULL;
    
    
    tmp = strstr(str, AXIS2_HTTP_CRLF);
    if(NULL == tmp)
    {
        axis2_http_status_line_free((axis2_http_status_line_t*)
                         status_line_impl, env);
        AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE, 
                        NULL);
    }
    i = tmp - str;
    tmp_status_line = AXIS2_MALLOC((*env)->allocator, 
                        i * sizeof(axis2_char_t) +1);
    if(NULL == tmp_status_line)
    {
        axis2_http_status_line_free((axis2_http_status_line_t*)
                         status_line_impl, env);
       AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
    }
    memcpy(tmp_status_line, str, i * sizeof(axis2_char_t));
    tmp_status_line[i] = '\0';
    tmp = tmp_status_line;
    
    http_version = tmp;
    tmp = strchr(tmp, ' ');
    if(NULL == tmp)
    {
        AXIS2_FREE((*env)->allocator, tmp_status_line);
        axis2_http_status_line_free((axis2_http_status_line_t*)
                         status_line_impl, env);
        AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE, 
                        NULL);
    }
    *tmp++ = '\0';
    status_code = tmp;
    tmp = strchr(tmp, ' ');
    if(NULL == tmp)
    {
        AXIS2_FREE((*env)->allocator, tmp_status_line);
        axis2_http_status_line_free((axis2_http_status_line_t*)
                         status_line_impl, env);
        AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE, 
                        NULL);
    }
    *tmp++ = '\0';
    reason_phrase = tmp;
    status_line_impl->http_version = (axis2_char_t *)
                        AXIS2_STRDUP(http_version, env);
    status_line_impl->status_code = (axis2_char_t *)
                        AXIS2_STRDUP(status_code, env);
    status_line_impl->reason_phrase = (axis2_char_t *)
                        AXIS2_STRDUP(reason_phrase, env);
    
    if(NULL == status_line_impl->http_version || 
                        NULL == status_line_impl->reason_phrase)
    {
        AXIS2_FREE((*env)->allocator, tmp_status_line);
        axis2_http_status_line_free((axis2_http_status_line_t*)
                         status_line_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    AXIS2_FREE((*env)->allocator, tmp_status_line);
    
    status_line_impl->status_line.ops = AXIS2_MALLOC((*env)->allocator,
                        sizeof(axis2_http_status_line_ops_t));
    if(NULL == status_line_impl->status_line.ops)
	{
		axis2_http_status_line_free((axis2_http_status_line_t*)
                         status_line_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    status_line_impl->status_line.ops->get_status_code =
                        axis2_http_status_line_get_status_code;
    status_line_impl->status_line.ops->get_http_version =
                        axis2_http_status_line_get_http_version;
    status_line_impl->status_line.ops->get_reason_phrase =
                        axis2_http_status_line_get_reason_phrase;
    status_line_impl->status_line.ops->starts_with_http =
                        axis2_http_status_line_starts_with_http;
    status_line_impl->status_line.ops->to_string =
                        axis2_http_status_line_to_string;
    status_line_impl->status_line.ops->free =
                        axis2_http_status_line_free;
	return &(status_line_impl->status_line);
}

axis2_status_t AXIS2_CALL 
axis2_http_status_line_free (axis2_http_status_line_t *status_line, 
                                    axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(status_line, env, AXIS2_FAILURE);
    axis2_http_status_line_impl_t *status_line_impl =
                        AXIS2_INTF_TO_IMPL(status_line);
    if(NULL != status_line_impl->line)
    {
        AXIS2_FREE((*env)->allocator, status_line_impl->line);
        status_line_impl->line = NULL;
    }
    if(NULL != status_line_impl->http_version)
    {
        AXIS2_FREE((*env)->allocator, status_line_impl->http_version);
        status_line_impl->http_version = NULL;
    }
    if(NULL != status_line_impl->status_code)
    {
        AXIS2_FREE((*env)->allocator, status_line_impl->status_code);
        status_line_impl->status_code = NULL;
    }
    if(NULL != status_line_impl->reason_phrase)
    {
        AXIS2_FREE((*env)->allocator, status_line_impl->reason_phrase);
        status_line_impl->reason_phrase = NULL;
    }     
    if(NULL != status_line->ops)
        AXIS2_FREE((*env)->allocator, status_line->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(status_line));
	return AXIS2_SUCCESS;
}


int AXIS2_CALL 
axis2_http_status_line_get_status_code (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(status_line, env, AXIS2_CRTICAL_FAILURE);
    if(NULL != AXIS2_INTF_TO_IMPL(status_line)->status_code)
    {
        return atoi(AXIS2_INTF_TO_IMPL(status_line)->status_code);
    }
    else
    {
        return AXIS2_CRTICAL_FAILURE;
    }
    
}


axis2_char_t* AXIS2_CALL 
axis2_http_status_line_get_http_version (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(status_line, env, NULL);
    return AXIS2_INTF_TO_IMPL(status_line)->http_version;
}


axis2_char_t* AXIS2_CALL 
axis2_http_status_line_get_reason_phrase (axis2_http_status_line_t *status_line,
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(status_line, env, NULL);
    return AXIS2_INTF_TO_IMPL(status_line)->reason_phrase;
}


axis2_bool_t AXIS2_CALL 
axis2_http_status_line_starts_with_http (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(status_line, env, AXIS2_FAILURE);
    if(0 == AXIS2_STRNCASECMP(AXIS2_INTF_TO_IMPL(status_line)->line, "HTTP", 4))
    {
        return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}

axis2_char_t *AXIS2_CALL
axis2_http_status_line_to_string (axis2_http_status_line_t *status_line, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(status_line, env, NULL);
    return AXIS2_INTF_TO_IMPL(status_line)->line;
}
