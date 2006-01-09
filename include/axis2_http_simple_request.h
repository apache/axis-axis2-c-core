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

#ifndef AXIS2_HTTP_SIMPLE_REQUEST_H
#define AXIS2_HTTP_SIMPLE_REQUEST_H


/**
  * @file axis2_http_simple_request.h
  * @brief axis2 HTTP Simple Request
  */

#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_http_request_line.h>
#include <axis2_http_header.h>
#include <axis2_stream.h>
#include <axis2_array_list.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */
    typedef struct axis2_http_simple_request_ops 
            axis2_http_simple_request_ops_t;
    typedef struct axis2_http_simple_request axis2_http_simple_request_t; 
    
/** 
 * @brief HTTP Simple Request ops struct
 * Encapsulator struct for ops of axis2_http_simple_request
 */  
AXIS2_DECLARE_DATA struct axis2_http_simple_request_ops
{
    axis2_http_request_line_t* (AXIS2_CALL *get_request_line)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);
	    
    axis2_status_t (AXIS2_CALL *set_request_line)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env, 
                            axis2_http_request_line_t *request_line);
    
    axis2_bool_t (AXIS2_CALL *contains_header)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env, axis2_char_t *name);

    axis2_array_list_t* (AXIS2_CALL *get_headers)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);

    axis2_http_header_t* (AXIS2_CALL *get_first_header)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env, const axis2_char_t *str);

    axis2_status_t (AXIS2_CALL *remove_headers)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env, const axis2_char_t *str);

    axis2_status_t (AXIS2_CALL *add_header)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env, axis2_http_header_t *header);

    axis2_char_t* (AXIS2_CALL *get_content_type)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);

    axis2_char_t* (AXIS2_CALL *get_charset)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);

    axis2_ssize_t (AXIS2_CALL *get_content_length)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);

    axis2_stream_t* (AXIS2_CALL *get_body)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);

    axis2_ssize_t (AXIS2_CALL *get_body_bytes)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env, char **buf);

    axis2_status_t (AXIS2_CALL *free)
                            (axis2_http_simple_request_t *simple_request, 
                            axis2_env_t **env);
    
};

/** 
 * @brief HTTP Simple Request struct
  *    Axis2 HTTP Simple Request
 */
AXIS2_DECLARE_DATA struct axis2_http_simple_request
{
    axis2_http_simple_request_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_simple_request_t *) 
axis2_http_simple_request_create (axis2_env_t **env,
                        axis2_http_request_line_t *request_line,
                        axis2_http_header_t **http_headers,
                        axis2_ssize_t http_hdr_count,
                        axis2_stream_t *content);
    
/************************** Start of function macros **************************/


#define AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(simple_request, env) \
                            ((simple_request)->ops->get_request_line\
                            (simple_request, env))
#define AXIS2_HTTP_SIMPLE_REQUEST_SET_REQUEST_LINE\
                            (simple_request, env, request_line) \
                            ((simple_request)->ops->set_request_line\
                            (simple_request, env, request_line))
#define AXIS2_HTTP_SIMPLE_REQUEST_CONTAINS_HEADER(simple_request, env, name)\
                            ((simple_request)->ops->contains_header\
                            (simple_request, env, name))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_HEADERS(simple_request, env)\
                            ((simple_request)->ops->get_headers(simple_request, env))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request, env, str) \
                            ((simple_request)->ops->get_first_header\
                            (simple_request, env, str))
#define AXIS2_HTTP_SIMPLE_REQUEST_REMOVE_HEADERS(simple_request, env, str) \
                            ((simple_request)->ops->remove_headers\
                            (simple_request, env, str))
#define AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(simple_request, env, header) \
                            ((simple_request)->ops->add_header\
                            (simple_request, env, header))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(simple_request, env)\
                            ((simple_request)->ops->get_content_type\
                            (simple_request, env))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CHARSET(simple_request, env) \
                            ((simple_request)->ops->get_charset\
                            (simple_request, env))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_LENGTH(simple_request, env)\
                            ((simple_request)->ops->get_content_length\
                            (simple_request, env))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY(simple_request, env)\
                            ((simple_request)->ops->get_body\
                            (simple_request, env))
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY_BYTES(simple_request, env, buf)\
                            ((simple_request)->ops->get_body_bytes\
                            (simple_request, env, buf))
#define AXIS2_HTTP_SIMPLE_REQUEST_FREE(simple_request, env) \
                            ((simple_request)->ops->free(simple_request, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SIMPLE_REQUEST_H */
