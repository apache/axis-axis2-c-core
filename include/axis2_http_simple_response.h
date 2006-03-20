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

#ifndef AXIS2_HTTP_SIMPLE_RESPONSE_H
#define AXIS2_HTTP_SIMPLE_RESPONSE_H


/**
  * @file axis2_simple_http_response.h
  * @brief axis2 HTTP Simple Response
  */

#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_array_list.h>
#include <axis2_http_status_line.h>
#include <axis2_http_header.h>
#include <axis2_stream.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */
    typedef struct axis2_http_simple_response_ops 
            axis2_http_simple_response_ops_t;
    typedef struct axis2_http_simple_response axis2_http_simple_response_t; 
    
/** 
 * @brief HTTP Simple Response ops struct
 * Encapsulator struct for ops of axis2_http_simple_response
 */  
AXIS2_DECLARE_DATA struct axis2_http_simple_response_ops
{
    axis2_status_t (AXIS2_CALL *
	set_status_line)(struct axis2_http_simple_response *simple_response, 
                    axis2_env_t **env, 
	                const axis2_char_t *http_ver, 
                    const int status_code, 
	                const axis2_char_t *phrase);
    
    axis2_char_t* (AXIS2_CALL *
	get_phrase)(axis2_http_simple_response_t *simple_response, 
                axis2_env_t **env);
    
    int (AXIS2_CALL *
	get_status_code)(axis2_http_simple_response_t *simple_response, 
                    axis2_env_t **env);

    axis2_char_t* (AXIS2_CALL *
	get_http_version)(axis2_http_simple_response_t *simple_response, 
                      axis2_env_t **env);

    axis2_char_t* (AXIS2_CALL *
	get_status_line)(axis2_http_simple_response_t *simple_response, 
                     axis2_env_t **env);

    axis2_bool_t (AXIS2_CALL *
	contains_header)(axis2_http_simple_response_t *simple_response, 
                     axis2_env_t **env, 
					 axis2_char_t *name);

    axis2_array_list_t* (AXIS2_CALL *
	get_headers)(axis2_http_simple_response_t *simple_response, 
                 axis2_env_t **env);
    
    axis2_http_header_t* (AXIS2_CALL *
	get_first_header)(axis2_http_simple_response_t *simple_response, 
                      axis2_env_t **env, 
					  axis2_char_t *str);
    
    axis2_status_t (AXIS2_CALL *
	remove_headers)(axis2_http_simple_response_t *simple_response, 
                    axis2_env_t **env, 
					const axis2_char_t *str);
    
    axis2_status_t (AXIS2_CALL *
	set_header)(axis2_http_simple_response_t *simple_response, 
                axis2_env_t **env, 
				axis2_http_header_t* header);

    axis2_status_t (AXIS2_CALL *
	set_headers)(axis2_http_simple_response_t *simple_response, 
                 axis2_env_t **env, 
				 axis2_http_header_t** headers, 
                 axis2_ssize_t array_size);
    
    axis2_char_t* (AXIS2_CALL *
	get_charset)(axis2_http_simple_response_t *simple_response, 
                 axis2_env_t **env);
    
    axis2_ssize_t (AXIS2_CALL *
	get_content_length)(axis2_http_simple_response_t *simple_response, 
                        axis2_env_t **env);
    
    axis2_char_t* (AXIS2_CALL *
	get_content_type)(axis2_http_simple_response_t *simple_response, 
                      axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
	set_body_string)(axis2_http_simple_response_t *simple_response, 
                     axis2_env_t **env, 
					 axis2_char_t *str);
    
    axis2_status_t (AXIS2_CALL *
	set_body_stream)(axis2_http_simple_response_t *simple_response, 
                     axis2_env_t **env, 
					 axis2_stream_t *stream);    

    axis2_stream_t* (AXIS2_CALL *
	get_body)(axis2_http_simple_response_t *simple_response, 
              axis2_env_t **env);

    axis2_ssize_t (AXIS2_CALL *
	get_body_bytes)(axis2_http_simple_response_t *simple_response, 
                    axis2_env_t **env, 
					axis2_char_t **buf);

    axis2_status_t (AXIS2_CALL *
	free)(axis2_http_simple_response_t *simple_response, 
          axis2_env_t **env);
    
};

/** 
  * @brief HTTP Simple Response struct
  *    Axis2 HTTP Simple Response
  */
AXIS2_DECLARE_DATA struct axis2_http_simple_response
{
    axis2_http_simple_response_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_simple_response_t *) 
axis2_http_simple_response_create (axis2_env_t **env,
                        axis2_http_status_line_t *status_line,
                        const axis2_http_header_t **http_headers,
                        const axis2_ssize_t http_hdr_count,
                        axis2_stream_t *content);

AXIS2_DECLARE(axis2_http_simple_response_t *) 
axis2_http_simple_response_create_default(axis2_env_t **env);
    
/************************** Start of function macros **************************/


#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_STAUTUS_LINE(\
					simple_response, env, http_version, status_code, phrase)\
                    ((simple_response)->ops->set_status_line\
                    (simple_response, env, http_version, status_code, phrase))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_PHRASE\
                    (simple_response, env) ((simple_response)->ops->get_phrase\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(simple_response, env)\
                    ((simple_response)->ops->get_http_version\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_STATUS_CODE(simple_response, env)\
                    ((simple_response)->ops->get_status_code\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_STAUTUS_LINE(simple_response, env) \
                    ((simple_response)->ops->get_status_line\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER(simple_response, env, name)\
                    ((simple_response)->ops->contains_header\
                    (simple_response, env, name))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_HEADERS(simple_response, env) \
                    ((simple_response)->ops->get_headers\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_FIRST_HEADER(simple_response, env, str) \
                    ((simple_response)->ops->get_first_header\
                    (simple_response, env, str))
#define AXIS2_HTTP_SIMPLE_RESPONSE_REMOVE_HEADERS(simple_response, env, str)\
                    ((simple_response)->ops->remove_headers\
                    (simple_response, env, str))
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env, header) \
                    ((simple_response)->ops->set_header\
                    (simple_response, env, header))
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADERS(simple_response, env, headers) \
                    ((simple_response)->ops->set_headers\
                    (simple_response, env, headers))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_CHARSET(simple_response, env) \
                    ((simple_response)->ops->get_charset(simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(simple_response, env) \
                    ((simple_response)->ops->get_content_length\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_TYPE(simple_response, env) \
                    ((simple_response)->ops->get_content_type\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STRING(simple_response, env, str)\
                    ((simple_response)->ops->set_body_string\
                    (simple_response, env, str))
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STREAM(simple_response, env, stream)\
                    ((simple_response)->ops->set_body_stream\
                    (simple_response, env, stream))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY(simple_response, env)\
                    ((simple_response)->ops->get_body\
                    (simple_response, env))
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY_BYTES(simple_response, env, buf)\
                    ((simple_response)->ops->get_body_bytes\
                    (simple_response, env, buf))
#define AXIS2_HTTP_SIMPLE_RESPONSE_FREE(simple_response, env)\
                    ((simple_response)->ops->free(simple_response, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SIMPLE_RESPONSE_H */
