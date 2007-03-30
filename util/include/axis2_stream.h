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
    typedef struct axis2_stream axis2_stream_t;

    struct axis2_stream
    {
        axis2_stream_type_t stream_type;
        int len;
        int max_len;
        /* Only one of these is used for a perticlar
         * instance depending on the type
         */
        axis2_char_t *buffer;
        axis2_char_t *buffer_head;
        FILE *fp;
        int socket;

        int axis2_eof;

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
    };

   /**
    * Deletes the stream
    * @return axis2_status_t AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_stream_free(axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN void  AXIS2_CALL
    axis2_stream_free_void_arg(void *stream,
        const axis2_env_t *env);

    /**
    * reads from stream
    * @param buffer buffer into which the content is to be read
    * @param count size of the buffer
    * @return no: of bytes read
    */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_stream_read(axis2_stream_t *stream,
        const axis2_env_t *env,
        void *buffer,
        size_t count);

    /**
     * writes into stream
     * @param buffer buffer to be written
     * @param count size of the buffer
     * @return no: of bytes actually written
     */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_stream_write(axis2_stream_t *stream,
        const axis2_env_t *env,
        const void *buffer,
        size_t count);

    /**
    * Skips over and discards n bytes of data from this input stream.
    * @param count number of bytes to be discarded
    * @return no: of bytes actually skipped
    */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_stream_skip(axis2_stream_t *stream,
        const axis2_env_t *env,
        int count);

    /**
     * Returns the length of the stream (applicable only to basic stream)
     * @return Length of the buffer if its type is basic, else -1
     * (we can't define a length of a stream unless it is just a buffer)
     */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_stream_get_len(axis2_stream_t *stream,
        const axis2_env_t *env);

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
	AXIS2_EXTERN void AXIS2_CALL
	axis2_stream_free(axis2_stream_t *stream, const axis2_env_t *env);	
	
	/**
     * Free stream passed as void pointer. This will be
     * cast into appropriate type and then pass the cast object
     * into the module_desc structure's free method
     */

    AXIS2_EXTERN void AXIS2_CALL
    axis2_stream_free_void_arg(void *stream,
        const axis2_env_t *env);

    /**
     * Gets the buffer
     */
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_stream_get_buffer(const axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_flush_buffer(axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_stream_peek_socket(axis2_stream_t *stream, const axis2_env_t *env,
        void *buffer, size_t count);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_flush(axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_close(axis2_stream_t *stream,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_set_read(axis2_stream_t *stream,
        const axis2_env_t *env, 
        void *func);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_set_write(axis2_stream_t *stream,
        const axis2_env_t *env, 
        void *func);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stream_set_skip(axis2_stream_t *stream,
        const axis2_env_t *env, 
        void *func);


#define AXIS2_STREAM_FREE(stream, env) axis2_stream_free(stream, env)

#define AXIS2_STREAM_FREE_VOID_ARG(stream, env) \
        axis2_stream_free_void_arg(stream, env)

#define AXIS2_STREAM_READ(stream, env, buffer, count) \
      axis2_stream_read(stream, env, buffer, count)

#define AXIS2_STREAM_WRITE(stream, env, buffer, count) \
      axis2_stream_write(stream, env, buffer, count)

#define AXIS2_STREAM_SKIP(stream, env, count) \
      axis2_stream_write(stream, env, count)

#define AXIS2_STREAM_BASIC_GET_LEN(stream, env) \
      axis2_stream_get_len(stream, env)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STREAM_H */
