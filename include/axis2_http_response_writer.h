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

#ifndef AXIS2_HTTP_RESPONSE_WRITER_H
#define AXIS2_HTTP_RESPONSE_WRITER_H

/**
 * @defgroup axis2_http_response_writer http response writer 
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_http_response_writer.h
 * @brief axis2 Response Writer
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axis2_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_response_writer */
    typedef struct axis2_http_response_writer axis2_http_response_writer_t;


    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_response_writer_get_encoding(const axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_close(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_flush(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @param c
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_write_char(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env,
        char c);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @param buf pointer to buf
     * @param offset
     * @param len
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_write_buf(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env,
        char *buf,
        int offset,
        axis2_ssize_t len);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @param str pointer to str
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_print_str(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env,
        const char *str);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @param i
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_print_int(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env,
        int i);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @param str pointer to str
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_println_str(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env,
        const char *str);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_response_writer_println(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env);

    /**
     * @param response_writer pointer to response writer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_http_response_writer_free(axis2_http_response_writer_t *response_writer,
        const axutil_env_t *env);

    /**
     * @param env pointer to environment struct
     * @param stream pointer to stream
     */
    AXIS2_EXTERN axis2_http_response_writer_t *AXIS2_CALL
    axis2_http_response_writer_create (
        const axutil_env_t *env,
        axis2_stream_t *stream);

    /**
     * @param env pointer to environment struct
     * @param stream pointer to stream
     * @param encoding pointer to encoding
     */
    AXIS2_EXTERN axis2_http_response_writer_t *AXIS2_CALL
    axis2_http_response_writer_create_with_encoding(
        const axutil_env_t *env,
        axis2_stream_t *stream,
        const axis2_char_t *encoding);

/** Get encoding.
    @sa axis2_http_response_writer_ops#get_encoding */
#define AXIS2_HTTP_RESPONSE_WRITER_GET_ENCODING(response_writer, env) \
                    axis2_http_response_writer_get_encoding(response_writer, env)

/** Close.
    @sa axis2_http_response_writer_ops#close */
#define AXIS2_HTTP_RESPONSE_WRITER_CLOSE(response_writer, env) \
                    axis2_http_response_writer_close(response_writer, env)

/** Flush.
    @sa axis2_http_response_writer_ops#flush */
#define AXIS2_HTTP_RESPONSE_WRITER_FLUSH(response_writer, env) \
                    axis2_http_response_writer_flush(response_writer, env)

/** Write char.
    @sa axis2_http_response_writer_ops#write_char */
#define AXIS2_HTTP_RESPONSE_WRITER_WRITE_CHAR(response_writer, env, c) \
                    axis2_http_response_writer_write_char(response_writer, env, c)

/** Write buf.
    @sa axis2_http_response_writer_ops#write_buf */
#define AXIS2_HTTP_RESPONSE_WRITER_WRITE_BUF(response_writer, env, buf, offset,\
                    len) axis2_http_response_writer_write_buf(response_writer, \
                    env, buf, offset, len)

/** Print str.
    @sa axis2_http_response_writer_ops#print_str */
#define AXIS2_HTTP_RESPONSE_WRITER_PRINT_STR(response_writer, env, str) \
                    axis2_http_response_writer_print_str(response_writer, env, str)

/** Print int.
    @sa axis2_http_response_writer_ops#print_int */
#define AXIS2_HTTP_RESPONSE_WRITER_PRINT_INT(response_writer, env, i)\
                    axis2_http_response_writer_print_int(response_writer, env)

/** Println str.
    @sa axis2_http_response_writer_ops#println_str */
#define AXIS2_HTTP_RESPONSE_WRITER_PRINTLN_STR(response_writer, env, str) \
                    axis2_http_response_writer_println_str(response_writer, env,\
                    str)

/** Println.
    @sa axis2_http_response_writer_ops#println */
#define AXIS2_HTTP_RESPONSE_WRITER_PRINTLN(response_writer, env) \
                    axis2_http_response_writer_println(response_writer, env)

/** Frees the http response writer.
    @sa axis2_http_response_writer_ops#free */
#define AXIS2_HTTP_RESPONSE_WRITER_FREE(response_writer, env)\
                    axis2_http_response_writer_free(response_writer, env)

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_RESPONSE_WRITER_H */
