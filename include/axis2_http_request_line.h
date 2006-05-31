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
 
#ifndef AXIS2_HTTP_REQUEST_LINE_H
#define AXIS2_HTTP_REQUEST_LINE_H

/**
 * @file axis2_http_request_line.h
 * @brief axis2 HTTP Request Line
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>


#ifdef __cplusplus
extern "C" 
{
#endif
/** 
  * @ingroup axis2_core_transport_http
  * @{
  */

typedef struct axis2_http_request_line_ops axis2_http_request_line_ops_t;
typedef struct axis2_http_request_line axis2_http_request_line_t;
    
    
/** 
 * @brief HTTP Request Line ops struct
 * Encapsulator struct for ops of axis2_http_request_line
 */
AXIS2_DECLARE_DATA struct axis2_http_request_line_ops
{
	axis2_char_t* (AXIS2_CALL *get_method) 
                                (axis2_http_request_line_t *request_line, 
                                const axis2_env_t *env);
	
	axis2_char_t* (AXIS2_CALL *get_http_version) 
                                (axis2_http_request_line_t *request_line, 
                                const axis2_env_t *env);
    
	axis2_char_t* (AXIS2_CALL *get_uri) 
                                (axis2_http_request_line_t *request_line, 
                                const axis2_env_t *env);
    
	axis2_char_t* (AXIS2_CALL *to_string)
                                (axis2_http_request_line_t *request_line, 
                                const axis2_env_t *env);
	
	axis2_status_t (AXIS2_CALL *free) 
                                (axis2_http_request_line_t *request_line, 
                                const axis2_env_t *env);
};

/**
 * @brief HTTP Reqest Line struct
 * Axis2 HTTP Request Line
 */
AXIS2_DECLARE_DATA struct axis2_http_request_line
{
	axis2_http_request_line_ops_t *ops;
};


AXIS2_DECLARE(axis2_http_request_line_t *) 
axis2_http_request_line_create(const axis2_env_t *env, axis2_char_t *method, 
                                axis2_char_t *uri, axis2_char_t *http_version);

AXIS2_DECLARE(axis2_http_request_line_t*)
axis2_http_request_line_parse_line(const axis2_env_t *env, const axis2_char_t *str);
/********************* Start of function macros	***************************/

#define AXIS2_HTTP_REQUEST_LINE_GET_METHOD(request_line, env) \
                        ((request_line)->ops->get_method (request_line, env))
#define AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION(request_line, env) \
                        ((request_line)->ops->get_http_version \
                        (request_line, env))
#define AXIS2_HTTP_REQUEST_LINE_GET_URI(request_line, env) \
                        ((request_line)->ops->get_uri (request_line, env))
#define AXIS2_HTTP_REQUEST_LINE_TO_STRING(request_line, env) \
                        ((request_line)->ops->to_string (request_line, env))
#define AXIS2_HTTP_REQUEST_LINE_FREE(request_line, env) \
                        ((request_line)->ops->free(request_line, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_HTTP_REQUEST_LINE_H */
