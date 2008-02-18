
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

#ifndef AXUTIL_HTTP_CHUNKED_STREAM_H
#define AXUTIL_HTTP_CHUNKED_STREAM_H

/**
 * @defgroup axutil_http_chunked_stream http chunked stream
 * @ingroup axis2_core_trans_http
 * Description
 * @{
 */

/**
 * @file axutil_http_chunked_stream.h
 * @brief axis2 HTTP Chunked Stream
 */

#include <axutil_env.h>
#include <axutil_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axutil_http_chunked_stream */
    typedef struct axutil_http_chunked_stream axutil_http_chunked_stream_t;

    struct axis2_callback_info
    {
        const axutil_env_t *env;
        void *in_stream;
        int content_length;
        int unread_len;
        axutil_http_chunked_stream_t *chunked_stream;
    };
    typedef struct axis2_callback_info axis2_callback_info_t;


    /**
    * @param chunked_stream pointer to chunked stream
    * @param env pointer to environment struct
    * @param buffer 
    * @param count
    */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_http_chunked_stream_read(
        axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env,
        void *buffer,
        size_t count);

    /**
    * @param env pointer to environment struct
    * @param buffer
    * @param count
    */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_http_chunked_stream_write(
        axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env,
        const void *buffer,
        size_t count);

    /**
    * @param chunked_stream pointer to chunked stream
    * @param env pointer to environment struct
    */
    AXIS2_EXTERN int AXIS2_CALL

    axutil_http_chunked_stream_get_current_chunk_size(
        const axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env);

    /**
    * @param chunked_stream pointer to chunked stream
    * @param env pointer to environment struct
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axutil_http_chunked_stream_write_last_chunk(
        axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env);

    /**
    * @param chunked_stream pointer to chunked stream
    * @param env pointer to environment struct
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_http_chunked_stream_free(
        axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env);

    /**
     * @param env pointer to environment struct
     * @param stream pointer to stream
     */
    AXIS2_EXTERN axutil_http_chunked_stream_t *AXIS2_CALL

    axutil_http_chunked_stream_create(
        const axutil_env_t * env,
        axutil_stream_t * stream);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axutil_http_chunked_stream_get_end_of_chunks(
        axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env);
    

    /** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXUTIL_HTTP_CHUNKED_STREAM_H */
