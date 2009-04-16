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

#include <string.h>
#include <stdlib.h>
#include <axutil_stream.h>
#include <platforms/axutil_platform_auto_sense.h>

/** basic stream operatons **/
int AXIS2_CALL axutil_stream_write_basic(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count);

int AXIS2_CALL axutil_stream_read_basic(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count);

int AXIS2_CALL axutil_stream_skip_basic(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count);

/** file stream operations **/
int AXIS2_CALL axutil_stream_write_file(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count);

int AXIS2_CALL axutil_stream_read_file(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count);

int AXIS2_CALL axutil_stream_skip_file(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count);

/** socket stream operations **/
int AXIS2_CALL axutil_stream_write_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count);

int AXIS2_CALL axutil_stream_read_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count);

int AXIS2_CALL axutil_stream_skip_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count);

AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_internal(
    const axutil_env_t *env)
{
    axutil_stream_t *stream = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    stream =
        (axutil_stream_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axutil_stream_t));

    if (!stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    stream->buffer = NULL;
    stream->buffer_head = NULL;
    stream->fp = NULL;
    stream->socket = -1;
    stream->len = -1;
    stream->max_len = -1;
    stream->axis2_eof = EOF;

    return stream;
}

void AXIS2_CALL
axutil_stream_free(
    axutil_stream_t *stream,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    switch (stream->stream_type)
    {
    case AXIS2_STREAM_BASIC:
        {
            if (stream->buffer_head)
            {
                AXIS2_FREE(env->allocator, stream->buffer_head);
            }
            stream->buffer = NULL;
            stream->len = -1;
            break;
        }
    case AXIS2_STREAM_FILE:
        {
            stream->fp = NULL;
            stream->len = -1;
            break;
        }
    case AXIS2_STREAM_SOCKET:
        {
            if (stream->fp)
            {
                fclose(stream->fp);
            }
            stream->socket = -1;
            stream->len = -1;
            break;
        }
    default:
        break;
    }

    AXIS2_FREE(env->allocator, stream);

    return;
}

void AXIS2_CALL
axutil_stream_free_void_arg(
    void *stream,
    const axutil_env_t *env)
{
    axutil_stream_t *stream_l = NULL;

    stream_l = (axutil_stream_t *) stream;
    axutil_stream_free(stream_l, env);
    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_stream_flush(
    axutil_stream_t *stream,
    const axutil_env_t *env)
{
    if (stream->fp)
    {
        if (fflush(stream->fp))
        {
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_stream_close(
    axutil_stream_t *stream,
    const axutil_env_t *env)
{
    switch (stream->stream_type)
    {
    case AXIS2_STREAM_BASIC:
        {
            if (stream->buffer_head)
            {
                AXIS2_FREE(env->allocator, stream->buffer_head);
            }
            stream->buffer = NULL;
            stream->len = -1;
            break;
        }
    case AXIS2_STREAM_FILE:
        {
            if (stream->fp)
            {
                if (fclose(stream->fp))
                {
                    return AXIS2_FAILURE;
                }
            }
            stream->fp = NULL;
            stream->len = -1;
            break;
        }
    case AXIS2_STREAM_SOCKET:
        {
            if (stream->fp)
            {
                if (fclose(stream->fp))
                {
                    return AXIS2_FAILURE;
                }
            }
            stream->socket = -1;
            stream->len = -1;
            break;
        }
    default:
        break;
    }

    return AXIS2_SUCCESS;
}

/************************ Basic Stream Operations *****************************/
AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_basic(
    const axutil_env_t *env)
{
    axutil_stream_t *stream = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    stream = axutil_stream_create_internal(env);
    if (!stream)
    {
        /*
         * We leave the error returned by the 
         * axutil_stream_create_internal intact
         */
        return NULL;
    }
    stream->stream_type = AXIS2_STREAM_BASIC;
    stream->read = axutil_stream_read_basic;
    stream->write = axutil_stream_write_basic;
    stream->skip = axutil_stream_skip_basic;
    stream->buffer =
        (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                      AXIS2_STREAM_DEFAULT_BUF_SIZE *
                                      sizeof(axis2_char_t));
    stream->buffer_head = stream->buffer;
    stream->len = 0;
    stream->max_len = AXIS2_STREAM_DEFAULT_BUF_SIZE;

    if (!stream->buffer)
    {
        axutil_stream_free(stream, env);
        return NULL;
    }
    return stream;
}

int AXIS2_CALL
axutil_stream_read_basic(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    int len = 0;
    char *buf = NULL;

    buf = stream->buffer;
    if (!buf)
    {
        return -1;
    }
    if (!buffer)
    {
        return -1;
    }
    if ((int)(count - 1) > stream->len)
    /* We are sure that the difference lies within the int range */
    {
        len = stream->len;
    }
    else
    {
        len = (int)(count - 1);
        /* We are sure that the difference lies within the int range */
    }
    memcpy(buffer, buf, len);
    /*
     * Finally we need to remove the read bytes from the stream
     * adjust the length of the stream.
     */
    stream->len -= len;
    stream->buffer = buf + len;
    ((axis2_char_t *) buffer)[len] = '\0';
    return len;
}

int AXIS2_CALL
axutil_stream_write_basic(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count)
{
    int new_len = 0;

    if (!buffer)
        return -1;

    new_len = (int)(stream->len + count);
    /* We are sure that the difference lies within the int range */
    if (new_len > stream->max_len)
    {
        axis2_char_t *tmp = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                                          sizeof(axis2_char_t) *
                                                          (new_len +
                                                           AXIS2_STREAM_DEFAULT_BUF_SIZE));
        if (!tmp)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return -1;
        }
        /*
         * pre allocation: extra AXIS2_STREAM_DEFAULT_BUF_SIZE more bytes 
         * allocated 
         */
        stream->max_len = new_len + AXIS2_STREAM_DEFAULT_BUF_SIZE;
        memcpy(tmp, stream->buffer, sizeof(axis2_char_t) * stream->len);
        AXIS2_FREE(env->allocator, stream->buffer_head);
        stream->buffer = tmp;
        stream->buffer_head = tmp;
    }
    memcpy(stream->buffer + (stream->len * sizeof(axis2_char_t)), buffer,
           count);
    stream->len += (int)count;
    /* We are sure that the difference lies within the int range */
    return (int)count;
}

int AXIS2_CALL
axutil_stream_get_len(
    axutil_stream_t *stream,
    const axutil_env_t *env)
{
    return stream->len;
}

int AXIS2_CALL
axutil_stream_skip_basic(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count)
{
    int del_len = 0;

    if (count > 0)
    {
        if (count <= stream->len)
        {
            del_len = count;
        }
        else
        {
            del_len = stream->len;
        }
        stream->len -= del_len;
        stream->buffer += del_len;
        return del_len;
    }
    return -1;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_stream_get_buffer(
    const axutil_stream_t *stream,
    const axutil_env_t *env)
{
    return stream->buffer;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_stream_flush_buffer(
    axutil_stream_t *stream,
    const axutil_env_t *env)
{
    stream->len = 0;
    return AXIS2_SUCCESS;
}

/********************* End of Basic Stream Operations *************************/

/************************** File Stream Operations ****************************/
AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_file(
    const axutil_env_t *env,
    FILE * fp)
{
    axutil_stream_t *stream = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    stream = axutil_stream_create_internal(env);
    if (!stream)
    {
        /*
         * We leave the error returned by the 
         * axutil_stream_create_internal intact
         */
        return NULL;
    }
    stream->stream_type = AXIS2_STREAM_FILE;
    stream->fp = fp;

    stream->read = axutil_stream_read_file;
    stream->write = axutil_stream_write_file;
    stream->skip = axutil_stream_skip_file;

    return stream;
}

int AXIS2_CALL
axutil_stream_read_file(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    FILE *fp = NULL;

    if (!stream->fp)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_FD, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on invalid file descriptor");

        return -1;
    }
    fp = stream->fp;
    if (!buffer)
    {
        return -1;
    }
    return (int)fread(buffer, sizeof(axis2_char_t), count, fp);
    /* We are sure that the difference lies within the int range */
}

int AXIS2_CALL
axutil_stream_write_file(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count)
{
    int len = 0;
    FILE *fp = NULL;

    if (!(stream->fp))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_FD, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on invalid file descriptor");

        return -1;
    }
    fp = stream->fp;
    if (!buffer)
        return -1;
    len = (int)fwrite(buffer, sizeof(axis2_char_t), count, fp);
    /* We are sure that the difference lies within the int range */
    return len;
}

int AXIS2_CALL
axutil_stream_skip_file(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count)
{
    int c = -1;
    int i = count;
    if (!(stream->fp))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_FD, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on invalid file descriptor");
        return -1;
    }
    while (EOF != (c = fgetc(stream->fp)) && i > 0)
    {
        i--;
    }
    return count - i;
}

/********************** End of File Stream Operations *************************/

/************************** Socket Stream Operations **************************/
AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_socket(
    const axutil_env_t *env,
    int socket)
{
    axutil_stream_t *stream = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    stream = axutil_stream_create_internal(env);
    if (!stream)
    {
        /*
         * We leave the error returned by the 
         * axutil_stream_create_internal intact
         */
        return NULL;
    }

    stream->read = axutil_stream_read_socket;
    stream->write = axutil_stream_write_socket;
    stream->skip = axutil_stream_skip_socket;

    stream->stream_type = AXIS2_STREAM_SOCKET;
    stream->socket = socket;
    stream->fp = NULL;

    return stream;
}

int AXIS2_CALL
axutil_stream_read_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    int len = 0;
#ifdef AXIS2_TCPMON
    axis2_char_t *temp = NULL;
#endif

    if (-1 == stream->socket)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on closed/not-opened socket");
        return -1;
    }
    if (!buffer)
    {
        return -1;
    }

    len = (int)recv(stream->socket, buffer, (int)count, 0);
    /* We are sure that the difference lies within the int range */
#ifdef AXIS2_TCPMON
    if (len > 1)
    {
        temp =
            (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                          (len + 1) * sizeof(axis2_char_t));
        if (temp)
        {
            memcpy(temp, buffer, len * sizeof(axis2_char_t));
            temp[len] = '\0';
            fprintf(stderr, "%s", temp);
            AXIS2_FREE(env->allocator, temp);
        }
    }
#endif
    return len;
}

int AXIS2_CALL
axutil_stream_write_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count)
{
    int len = 0;
#ifdef AXIS2_TCPMON
    axis2_char_t *temp = NULL;
#endif

    if (-1 == stream->socket)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on closed/not-opened socket");
        return -1;
    }
    if (!buffer)
        return -1;
    len = (int)send(stream->socket, buffer, (int)count, 0);
    /* We are sure that the difference lies within the int range */
#ifdef AXIS2_TCPMON
    if (len > 0)
    {
        temp =
            (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                          (len + 1) * sizeof(axis2_char_t));
        if (temp)
        {
            memcpy(temp, buffer, len * sizeof(axis2_char_t));
            temp[len] = '\0';
            fprintf(stderr, "%s", temp);
            AXIS2_FREE(env->allocator, temp);
        }
    }
#endif
    return len;

}

int AXIS2_CALL
axutil_stream_skip_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count)
{
    int len = 0;
	int received = 0;
    char buffer[2];

    if (-1 == stream->socket)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on closed/not-opened socket");
        return -1;
    }
    while (len < count)
    {
        received = recv(stream->socket, buffer, 1, 0);
		if (received == 0)
		{
			AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Socket has being shutdown");
			return -1;
		}
		if (received < 0)
		{
			AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Error while trying to read the socke");        
			return -1;
		}
		len += received;
    }
    return len;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_stream_peek_socket(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    int len = 0;

    /* Added to prevent a segfault */
    AXIS2_PARAM_CHECK(env->error, stream, -1);

    if (-1 == stream->socket)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Trying to do operation on closed/not-opened socket");
        return -1;
    }
    if (!buffer)
    {
        return -1;
    }

    len = (int)recv(stream->socket, buffer, (int)count, MSG_PEEK);
    /* We are sure that the difference lies within the int range */

    return len;
}

/********************** End of Socket Stream Operations ***********************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_stream_set_read(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    AXUTIL_STREAM_READ func)
{
    stream->read = func;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_stream_set_write(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    AXUTIL_STREAM_WRITE func)
{
    stream->write = func;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_stream_set_skip(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    AXUTIL_STREAM_SKIP func)
{
    stream->skip = func;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_stream_read(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    return stream->read(stream, env, buffer, count);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_stream_write(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count)
{
    return stream->write(stream, env, buffer, count);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_stream_skip(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count)
{
    return stream->skip(stream, env, count);
}
