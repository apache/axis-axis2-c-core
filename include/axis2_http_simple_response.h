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

#ifndef AXIS2_HTTP_SIMPLE_RESPONSE_H
#define AXIS2_HTTP_SIMPLE_RESPONSE_H

/**
 * @defgroup axis2_http_simple_response http simple response
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_simple_http_response.h
 * @brief axis2 HTTP Simple Response
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axutil_array_list.h>
#include <axis2_http_status_line.h>
#include <axis2_http_header.h>
#include <axis2_stream.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_simple_response */
    typedef struct axis2_http_simple_response axis2_http_simple_response_t;

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param http_ver pointer to http_ver
    * @param status_code pointer to status code
    * @param phrase pointer to phrase
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_response_set_status_line(struct axis2_http_simple_response *simple_response,
        const axis2_env_t *env,
        const axis2_char_t *http_ver,
        const int status_code,
        const axis2_char_t *phrase);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_http_simple_response_get_phrase(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_http_simple_response_get_status_code(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_http_simple_response_get_http_version(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_http_simple_response_get_status_line(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param name pointer to name
    */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_http_simple_response_contains_header(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        const axis2_char_t *name);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN axutil_array_list_t* AXIS2_CALL
    axis2_http_simple_response_get_headers(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param str pointer to str
    */
    AXIS2_EXTERN axis2_http_header_t* AXIS2_CALL
    axis2_http_simple_response_get_first_header(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        const axis2_char_t *str);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param str pointer to str
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_response_remove_headers(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        const axis2_char_t *str);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param header pointer to header
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_http_simple_response_set_header(axis2_http_simple_response_t *simple_response,
		const axis2_env_t *env,
		axis2_http_header_t* header);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param headers double pointer to headers
    * @param array_size
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_response_set_headers(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        axis2_http_header_t** headers,
        axis2_ssize_t array_size);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
    axis2_http_simple_response_get_charset(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
	AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
	axis2_http_simple_response_get_content_length(axis2_http_simple_response_t *simple_response,
		const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
    axis2_http_simple_response_get_content_type(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param str pointer to str
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_response_set_body_string(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        axis2_char_t *str);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param stream pointer to stream
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_response_set_body_stream(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        axis2_stream_t *stream);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN axis2_stream_t* AXIS2_CALL
    axis2_http_simple_response_get_body(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @param buf double pointer to buf
    */
    AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
    axis2_http_simple_response_get_body_bytes(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env,
        axis2_char_t **buf);

   /**
    * @param simple_response pointer to simple response struct
    * @param env pointer to environment struct
    * @return void
    */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_http_simple_response_free(axis2_http_simple_response_t *simple_response,
        const axis2_env_t *env);

    /**
     * @param env pointer to environment struct
     * @param status_line pointer to status line
     * @param http_headers double pointer to http_headers
     * @param http_hdr_count
     * @param content pointer to content
     */
    AXIS2_EXTERN axis2_http_simple_response_t * AXIS2_CALL
    axis2_http_simple_response_create (
            const axis2_env_t *env,
            axis2_http_status_line_t *status_line,
            const axis2_http_header_t **http_headers,
            const axis2_ssize_t http_hdr_count,
            axis2_stream_t *content);

    /**
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_http_simple_response_t * AXIS2_CALL
    axis2_http_simple_response_create_default(
        const axis2_env_t *env);

/** Sets the status line. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(\
               simple_response, env, http_version, status_code, phrase)\
                    axis2_http_simple_response_set_status_line\
                    (simple_response, env, http_version, status_code, phrase)

/** Gets the phrase. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_PHRASE\
                    (simple_response, env) axis2_http_simple_response_get_phrase\
                    (simple_response, env)

/** Gets the http version. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(simple_response, env)\
                    axis2_http_simple_response_get_http_version\
                    (simple_response, env)

/** Gets the status code. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_STATUS_CODE(simple_response, env)\
                    axis2_http_simple_response_get_status_code\
                    (simple_response, env)

/** Gets the status line. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_STATUS_LINE(simple_response, env) \
                    axis2_http_simple_response_get_status_line\
                    (simple_response, env)

/** Contains header. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER(simple_response, env, name)\
                    axis2_http_simple_response_contains_header\
                    (simple_response, env, name)

/** Get headers. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_HEADERS(simple_response, env) \
                    axis2_http_simple_response_get_headers\
                    (simple_response, env)

/** Gets the first header. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_FIRST_HEADER(simple_response, env, str) \
                    axis2_http_simple_response_get_first_header\
                    (simple_response, env, str)

/** Removes the headers. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_REMOVE_HEADERS(simple_response, env, str)\
                    axis2_http_simple_response_remove_headers\
                    (simple_response, env, str)

/** Set header. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env, header) \
                    axis2_http_simple_response_set_header\
                    (simple_response, env, header)

/** Set headers. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADERS(simple_response, env, headers) \
                    axis2_http_simple_response_set_headers\
                    (simple_response, env, headers)

/** Gets the charset. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_CHARSET(simple_response, env) \
                    axis2_http_simple_response_get_charset(simple_response, env)

/** Gets the content length. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(simple_response, env) \
                    axis2_http_simple_response_get_content_length\
                    (simple_response, env)

/** Gets the content type. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_TYPE(simple_response, env) \
                    axis2_http_simple_response_get_content_type\
                    (simple_response, env)

/** Sets the body string. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STRING(simple_response, env, str)\
                    axis2_http_simple_response_set_body_string\
                    (simple_response, env, str)

/** Sets the body stream. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STREAM(simple_response, env, stream)\
                    axis2_http_simple_response_set_body_stream\
                    (simple_response, env, stream)

/** Get body. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY(simple_response, env)\
                    axis2_http_simple_response_get_body\
                    (simple_response, env)

/** Get body bytes. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY_BYTES(simple_response, env, buf)\
                    axis2_http_simple_response_get_body_bytes\
                    (simple_response, env, buf)

/** Frees the http simple response. */
#define AXIS2_HTTP_SIMPLE_RESPONSE_FREE(simple_response, env)\
                    axis2_http_simple_response_free(simple_response, env)

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SIMPLE_RESPONSE_H */
