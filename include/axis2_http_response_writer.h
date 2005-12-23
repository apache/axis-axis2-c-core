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

#ifndef AXIS2_HTTP_RESPONSE_WRITER_H
#define AXIS2_HTTP_RESPONSE_WRITER_H


/**
  * @file axis2_http_response_writer.h
  * @brief axis2 Response Writer
  */
  
#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

    typedef struct axis2_http_response_writer_ops 
                                        axis2_http_response_writer_ops_t;
    typedef struct axis2_http_response_writer axis2_http_response_writer_t;

    
/** 
 * @brief HTTP Response Writer ops struct
 * Encapsulator struct for ops of axis2_http_response_writer
 */  
AXIS2_DECLARE_DATA struct axis2_http_response_writer_ops
{
    axis2_char_t* (AXIS2_CALL *get_encoding) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *close) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *flush) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *write_char) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env, char c);
    
    axis2_status_t (AXIS2_CALL *write_buf) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env, char *buf, int offset, 
                    axis2_ssize_t len);

    axis2_status_t (AXIS2_CALL *print_str) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env, char *str);
    
    axis2_status_t (AXIS2_CALL *print_int) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env, int i);
    
    axis2_status_t (AXIS2_CALL *println_str) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env, char *str);
    
    axis2_status_t (AXIS2_CALL *println) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *free) 
                    (axis2_http_response_writer_t *response_writer, 
                    axis2_env_t **env);
};

/** 
 * @brief HTTP Response Writer struct
 *    Axis2 HTTP Response Writer
 */
AXIS2_DECLARE_DATA struct axis2_http_response_writer
{
    axis2_http_response_writer_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_response_writer_t *) 
axis2_http_response_writer_create (axis2_env_t **env, axis2_stream_t *stream);

AXIS2_DECLARE(axis2_http_response_writer_t *) 
axis2_http_response_writer_create_with_encoding(axis2_env_t **env,
                    axis2_stream_t *stream, const axis2_char_t *encoding);
    
/************************** Start of function macros **************************/

#define AXIS2_HTTP_RESPONSE_WRITER_GET_ENCODING(response_writer, env) \
                    ((response_writer)->ops->get_encoding(response_writer, env))
#define AXIS2_HTTP_RESPONSE_WRITER_CLOSE(response_writer, env) \
                    ((response_writer)->ops->close(response_writer, env))
#define AXIS2_HTTP_RESPONSE_WRITER_FLUSH(response_writer, env) \
                    ((response_writer)->ops->flush(response_writer, env))
#define AXIS2_HTTP_RESPONSE_WRITER_WRITE_CHAR(response_writer, env, c) \
                    ((response_writer)->ops->write_char(response_writer, env, c))
#define AXIS2_HTTP_RESPONSE_WRITER_WRITE_BUF(response_writer, env, buf, offset,\
                    len) ((response_writer)->ops->write_buf(response_writer, \
                    env, buf, offset, len))
#define AXIS2_HTTP_RESPONSE_WRITER_PRINT_STR(response_writer, env, str) \
                    ((response_writer)->ops->print_str(response_writer, env, str))
#define AXIS2_HTTP_RESPONSE_WRITER_PRINT_INT(response_writer, env, i)\
                    ((response_writer)->ops->print_int(response_writer, env))
#define AXIS2_HTTP_RESPONSE_WRITER_PRINTLN_STR(response_writer, env, str) \
                    ((response_writer)->ops->println_str(response_writer, env,\
                    str))
#define AXIS2_HTTP_RESPONSE_WRITER_PRINTLN(response_writer, env) \
                    ((response_writer)->ops->println(response_writer, env))
#define AXIS2_HTTP_RESPONSE_WRITER_FREE(response_writer, env)\
                    ((response_writer)->ops->free(response_writer, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_RESPONSE_WRITER_H */
