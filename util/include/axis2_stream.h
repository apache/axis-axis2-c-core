/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain count copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_STREAM_H
#define AXIS2_STREAM_H

#include <axis2_utils.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_stream axis2_stream_t;
    typedef struct axis2_stream_ops axis2_stream_ops_t;

#define AXIS2_STREAM_DEFAULT_BUF_SIZE 2048
    /**
     * @defgroup axis2_stream stream
     * @ingroup axis2_util
     * @{
     */

    /**
    * \brief Axis2 stream types
    *
    * This is used to create a stream to correspond to
    * particular i/o mtd
    */
    enum axis2_stream_type
    {
        AXIS2_STREAM_BASIC = 0,
        AXIS2_STREAM_FILE,
        AXIS2_STREAM_SOCKET,
        AXIS2_STREAM_MANAGED /* Example Wrapper stream for Apache2 read mechanism */
    };

    typedef enum axis2_stream_type axis2_stream_type_t;

    /**
    * \brief Axis2 stream ops struct
    *
    * Encapsulator struct for ops of axis2_stream
    */
    struct axis2_stream_ops
    {

        /**
          * Deletes the stream
        * @return axis2_status_t AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t(AXIS2_CALL *
        free_fn)(axis2_stream_t *stream,
            const axis2_env_t *env);

        axis2_status_t(AXIS2_CALL *
            free_void_arg)(void *stream,
            const axis2_env_t *env);

        /**
        * reads from stream
        * @param buffer buffer into which the content is to be read
        * @param count size of the buffer
        * @return no: of bytes read
        */

        int(AXIS2_CALL *
        read)(axis2_stream_t *stream,
            const axis2_env_t *env,
            void *buffer,
            size_t count);
        /**
         * writes into stream
         * @param buffer buffer to be written
         * @param count size of the buffer
         * @return no: of bytes actually written
         */
        int(AXIS2_CALL *
        write)(axis2_stream_t *stream,
            const axis2_env_t *env,
            const void *buffer,
            size_t count);
        /**
        * Skips over and discards n bytes of data from this input stream.
        * @param count number of bytes to be discarded
        * @return no: of bytes actually skipped
        */
        int(AXIS2_CALL *
        skip)(axis2_stream_t *stream,
            const axis2_env_t *env,
            int count);

        /**
         * Returns the length of the stream (applicable only to basic stream)
         * @return Length of the buffer if its type is basic, else -1
         * (we can't define a length of a stream unless it is just a buffer)
         */
        int(AXIS2_CALL *
        get_len)(axis2_stream_t *stream,
            const axis2_env_t *env);
    };

    /**
    * \brief Axis2 Stream struct
    *
    * Stream is the encapsulating struct for all stream related ops
    */
    struct axis2_stream
    {
        /** Stream related ops */
        axis2_stream_ops_t *ops;
        /** Stream End of File */
        int axis2_eof;
    };

    /** \brief Constructor for creating an in memory stream
      * @return axis2_stream (in memory)
      */
    AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
    axis2_stream_create_basic(const axis2_env_t *env);

    /** \brief Constructor for creating a file stream
      * @param valid file pointer (opened file)
      * @return axis2_stream (file)
      */
    AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
    axis2_stream_create_file(const axis2_env_t *env, FILE *fp);

    /** \brief Constructor for creating a file stream
      * @param valid socket (opened socket)
      * @return axis2_stream (socket)
      */
    AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
    axis2_stream_create_socket(const axis2_env_t *env, int socket);

    /**
	 *Free stream
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_stream_free(axis2_stream_t *stream, const axis2_env_t *env);	
	
	/**
     * Free stream passed as void pointer. This will be
     * cast into appropriate type and then pass the cast object
     * into the module_desc structure's free method
     */

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_free_void_arg(void *stream,
        const axis2_env_t *env);

    /**
     * Gets the buffer
     */
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_stream_get_buffer(const axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_flush_buffer (const axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_stream_peek_socket(axis2_stream_t *stream, const axis2_env_t *env,
        void *buffer, size_t count);

#define AXIS2_STREAM_FREE(stream, env) ((stream->ops)->free_fn(stream, env))

#define AXIS2_STREAM_FREE_VOID_ARG(stream, env) \
        ((stream->ops)->free_void_arg(stream, env))

#define AXIS2_STREAM_READ(stream, env, buffer, count) \
      ((stream)->ops->read(stream, env, buffer, count))

#define AXIS2_STREAM_WRITE(stream, env, buffer, count) \
      ((stream)->ops->write(stream, env, buffer, count))

#define AXIS2_STREAM_SKIP(stream, env, count) \
      ((stream)->ops->write(stream, env, count))

#define AXIS2_STREAM_BASIC_GET_LEN(stream, env) \
      ((stream)->ops->get_len(stream, env))

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STREAM_H */
