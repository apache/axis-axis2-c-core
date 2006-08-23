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

#ifndef AXIS2_HTTP_CHUNKED_STREAM_H
#define AXIS2_HTTP_CHUNKED_STREAM_H

/**
 * @defgroup axis2_http_chunked_stream http chunked stream
 * @ingroup axis2_core_trans_http 
 * Description
 * @{
 */

/**
 * @file axis2_http_chunked_stream.h
 * @brief axis2 HTTP Chunked Stream
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_stream.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_chunked_stream_ops */
    typedef struct axis2_http_chunked_stream_ops axis2_http_chunked_stream_ops_t;
    /** Type name for struct axis2_http_chunked_stream */
    typedef struct axis2_http_chunked_stream axis2_http_chunked_stream_t;


    /**
     * HTTP Chunked Stream ops struct
     * Encapsulator struct for ops of axis2_http_chunked_stream
     */
    struct axis2_http_chunked_stream_ops
    {
        /**
        * @param chunked_stream pointer to chunked stream
        * @param env pointer to environment struct
        * @param buffer 
        * @param count
        */
        int (AXIS2_CALL *
                read)(
                    axis2_http_chunked_stream_t *chunked_stream,
                    const axis2_env_t *env,
                    void *buffer,
                    size_t count);

        /**
        * @param env pointer to environment struct
        * @param buffer
        * @param count
        */
        int (AXIS2_CALL *
                write)(
                    axis2_http_chunked_stream_t *chunked_stream,
                    const axis2_env_t *env,
                    const void *buffer,
                    size_t count);

        /**
        * @param chunked_stream pointer to chunked stream
        * @param env pointer to environment struct
        */
        int (AXIS2_CALL *
                get_current_chunk_size)(
                    const axis2_http_chunked_stream_t *chunked_stream,
                    const axis2_env_t *env);

        /**
        * @param chunked_stream pointer to chunked stream
        * @param env pointer to environment struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
                write_last_chunk)(
                    axis2_http_chunked_stream_t *chunked_stream,
                    const axis2_env_t *env);

        /**
        * @param chunked_stream pointer to chunked stream
        * @param env pointer to environment struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_http_chunked_stream_t *chunked_stream,
                    const axis2_env_t *env);
    };

    /**
     * axis2 http chunked stream
     */
    struct axis2_http_chunked_stream
    {
        /** operations of axis2 http chucnked stream */
        axis2_http_chunked_stream_ops_t *ops;
    };


    /**
     * @param env pointer to environment struct
     * @param stream pointer to stream
     */
    AXIS2_EXTERN axis2_http_chunked_stream_t * AXIS2_CALL
    axis2_http_chunked_stream_create(
        const axis2_env_t *env,
        axis2_stream_t* stream);

/********************* Start of function macros   ***************************/

/** Read.
    @sa axis2_http_chunked_stream_ops#read */
#define AXIS2_HTTP_CHUNKED_STREAM_READ(chunked_stream, env, buffer, count) \
                        ((chunked_stream)->ops->read(chunked_stream, env, \
                  buffer, count))

/** Write.
    @sa axis2_http_chunked_stream_ops#write */
#define AXIS2_HTTP_CHUNKED_STREAM_WRITE(chunked_stream, env, buffer, count) \
                  ((chunked_stream)->ops->write(chunked_stream, env, \
                  buffer, count))

/** Gets the current chunk size.
    @sa axis2_http_chunked_stream_ops#get_current_chunk_size */
#define AXIS2_HTTP_CHUNKED_GET_CURRENT_CHUNK_SIZE(chunked_stream, env) \
                        ((chunked_stream)->ops->get_current_chunk_size \
                  (chunked_stream, env))

/** Writes the last chunk.
    @sa axis2_http_chunked_stream_ops#write_last_chunk */
#define AXIS2_HTTP_CHUNKED_STREAM_WRITE_LAST_CHUNK(chunked_stream, env) \
                  ((chunked_stream)->ops->write_last_chunk(chunked_stream\
                  , env))

/** Free.
    @sa axis2_http_chunked_stream_ops#free */
#define AXIS2_HTTP_CHUNKED_STREAM_FREE(chunked_stream, env) \
                        ((chunked_stream)->ops->free(chunked_stream, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_HTTP_CHUNKED_STREAM_H */
