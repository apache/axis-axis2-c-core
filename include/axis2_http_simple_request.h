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

#ifndef AXIS2_HTTP_SIMPLE_REQUEST_H
#define AXIS2_HTTP_SIMPLE_REQUEST_H

/**
 * @defgroup axis2_http_simple_request http simple request
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_http_simple_request.h
 * @brief axis2 HTTP Simple Request
 */

#include <axis2_const.h>
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

    /** Type name for struct axis2_http_simple_request_ops */
    typedef struct axis2_http_simple_request_ops
                axis2_http_simple_request_ops_t;
    /** Type name for struct axis2_http_simple_request */
    typedef struct axis2_http_simple_request axis2_http_simple_request_t;

    /**
     * HTTP Simple Request ops struct
     * Encapsulator struct for ops of axis2_http_simple_request
     */
    struct axis2_http_simple_request_ops
    {
        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         */
        axis2_http_request_line_t *(AXIS2_CALL *
                get_request_line)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param request_line pointer to request line
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_request_line)(
                    axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    axis2_http_request_line_t *request_line);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param name pointer to name
         */
        axis2_bool_t (AXIS2_CALL *
                contains_header)(
                    axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_headers)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param str pointer to str
         */
        axis2_http_header_t *(AXIS2_CALL *
                get_first_header)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    const axis2_char_t *str);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param str pointer to str
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                remove_headers)(
                    axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    const axis2_char_t *str);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param header pointer to header
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_header)(
                    axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    axis2_http_header_t *header);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         */
        const axis2_char_t *(AXIS2_CALL *
                get_content_type)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         */
        const axis2_char_t *(AXIS2_CALL *
                get_charset)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         */
        axis2_ssize_t (AXIS2_CALL *
                get_content_length)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         */
        axis2_stream_t *(AXIS2_CALL *
                get_body)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param buf double pointer to buf
         */
        axis2_ssize_t (AXIS2_CALL *
                get_body_bytes)(
                    const axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    char **buf);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @param str pointer to str
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_body_string)(
                    axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env,
                    void *str,
                    unsigned int str_len);

        /**
         * @param simple_request pointer to simple request
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_http_simple_request_t *simple_request,
                    const axis2_env_t *env);

    };

    /**
     * axis2 http simple request
     */
    struct axis2_http_simple_request
    {
        /** operations of axis2 http simple request */
        axis2_http_simple_request_ops_t *ops;
    };

    /**
     * @param env pointer to environment struct
     * @param request_line pointer to request line
     * @param http_headers double pointer to http headers
     * @param http_hdr_count
     * @param content pointer to content
     */
    AXIS2_EXTERN axis2_http_simple_request_t *AXIS2_CALL
    axis2_http_simple_request_create (
        const axis2_env_t *env,
        axis2_http_request_line_t *request_line,
        axis2_http_header_t **http_headers,
        axis2_ssize_t http_hdr_count,
        axis2_stream_t *content);

/************************** Start of function macros **************************/

/** Gets the request line.
    @sa axis2_http_simple_request_ops#get_request_line */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(simple_request, env) \
                            ((simple_request)->ops->get_request_line\
                            (simple_request, env))

/** Set request line.
    @sa axis2_http_simple_request_ops#set_request_line */
#define AXIS2_HTTP_SIMPLE_REQUEST_SET_REQUEST_LINE\
                            (simple_request, env, request_line) \
                            ((simple_request)->ops->set_request_line\
                            (simple_request, env, request_line))

/** Contains header.
    @sa axis2_http_simple_request_ops#contains_header */
#define AXIS2_HTTP_SIMPLE_REQUEST_CONTAINS_HEADER(simple_request, env, name)\
                            ((simple_request)->ops->contains_header\
                            (simple_request, env, name))

/** Get headers.
    @sa axis2_http_simple_request_ops#get_headers */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_HEADERS(simple_request, env)\
                            ((simple_request)->ops->get_headers(simple_request, env))

/** Gets the first header.
    @sa axis2_http_simple_request_ops#get_first_header */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request, env, str) \
                            ((simple_request)->ops->get_first_header\
                            (simple_request, env, str))

/** Remove headers.
    @sa axis2_http_simple_request_ops#remove_headers */
#define AXIS2_HTTP_SIMPLE_REQUEST_REMOVE_HEADERS(simple_request, env, str) \
                            ((simple_request)->ops->remove_headers\
                            (simple_request, env, str))

/** Add header.
    @sa axis2_http_simple_request_ops#add_header */
#define AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(simple_request, env, header) \
                            ((simple_request)->ops->add_header\
                            (simple_request, env, header))

/** Gets the content type.
    @sa axis2_http_simple_request_ops#get_content_type */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(simple_request, env)\
                            ((simple_request)->ops->get_content_type\
                            (simple_request, env))

/** Gets the charset.
    @sa axis2_http_simple_request_ops#get_charset */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CHARSET(simple_request, env) \
                            ((simple_request)->ops->get_charset\
                            (simple_request, env))

/** Gets the content length.
    @sa axis2_http_simple_request_ops#get_content_length */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_LENGTH(simple_request, env)\
                            ((simple_request)->ops->get_content_length\
                            (simple_request, env))

/** Gets the body.
    @sa axis2_http_simple_request_ops#get_body */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY(simple_request, env)\
                            ((simple_request)->ops->get_body\
                            (simple_request, env))

/** Get body bytes.
    @sa axis2_http_simple_request_ops#get_body_bytes */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY_BYTES(simple_request, env, buf)\
                            ((simple_request)->ops->get_body_bytes\
                            (simple_request, env, buf))

/** Sets the body string.
    @sa axis2_http_simple_request_ops#set_body_string */
#define AXIS2_HTTP_SIMPLE_REQUEST_SET_BODY_STRING(simple_request, env, str, str_len)\
                            ((simple_request)->ops->set_body_string\
                            (simple_request, env, str, str_len))

/** Frees the http simple request.
    @sa axis2_http_simple_request_ops#free */
#define AXIS2_HTTP_SIMPLE_REQUEST_FREE(simple_request, env) \
                            ((simple_request)->ops->free(simple_request, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SIMPLE_REQUEST_H */
