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
#include <axis2_http_simple_response.h>
#include <axis2_http_transport.h>
#include <axutil_string.h>
#include <stdio.h>
#include <string.h>
#include <axutil_types.h>
#include <axiom_mime_part.h>

#define AXIS2_HTTP_SIMPLE_RESPONSE_READ_SIZE 2048

struct axis2_http_simple_response
{
    axis2_http_status_line_t *status_line;
    axutil_array_list_t *header_group;
    axutil_stream_t *stream;
    axutil_array_list_t *mime_parts;
    axis2_char_t *mtom_sending_callback_name;
};

AXIS2_EXTERN axis2_http_simple_response_t *AXIS2_CALL
axis2_http_simple_response_create(
    const axutil_env_t * env,
    axis2_http_status_line_t * status_line,
    const axis2_http_header_t ** http_headers,
    const axis2_ssize_t http_hdr_count,
    axutil_stream_t * content)
{
    axis2_http_simple_response_t *ret = NULL;
    ret = axis2_http_simple_response_create_default(env);
    if(!ret)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "axis2 http simple response creation failed");
        return NULL;
    }

    ret->status_line = status_line;
    if(http_hdr_count > 0 && http_headers)
    {
        int i = 0;
        ret->header_group = axutil_array_list_create(env, http_hdr_count);

        for(i = 0; i < (int)http_hdr_count; i++)
        /* We are sure that the difference lies within the int range */
        {
            axutil_array_list_add(ret->header_group, env, (void *)http_headers[i]);
        }
    }
    ret->stream = content;
    return ret;
}

AXIS2_EXTERN axis2_http_simple_response_t *AXIS2_CALL
axis2_http_simple_response_create_default(
    const axutil_env_t * env)
{
    axis2_http_simple_response_t *simple_response = NULL;
    simple_response = (axis2_http_simple_response_t *)AXIS2_MALLOC(env->allocator,
        sizeof(axis2_http_simple_response_t));
    if(!simple_response)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset((void *)simple_response, 0, sizeof(axis2_http_simple_response_t));
    return simple_response;
}

void AXIS2_CALL
axis2_http_simple_response_free(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    if(simple_response->status_line)
    {
        axis2_http_status_line_free(simple_response->status_line, env);
    }

    if(simple_response->header_group)
    {
        int i = 0;
        for(i = 0; i < axutil_array_list_size(simple_response->header_group, env); i++)
        {
            void *tmp = NULL;
            tmp = axutil_array_list_get(simple_response-> header_group, env, i);
            if(tmp)
            {
                axis2_http_header_free((axis2_http_header_t *)tmp, env);
            }
        }
        axutil_array_list_free(simple_response->header_group, env);
    }

    if(simple_response->mime_parts)
    {
        int i = 0;
        for(i = 0; i < axutil_array_list_size(simple_response->mime_parts, env); i++)
        {
            void *mime_part = NULL;
            mime_part = axutil_array_list_get(simple_response->mime_parts, env, i);
            if(mime_part)
            {
                axiom_mime_part_free((axiom_mime_part_t *)mime_part, env);
            }
        }
        axutil_array_list_free(simple_response->mime_parts, env);
    }

     /* Stream is not freed. Assumption : stream doesn't belong to the response */

    AXIS2_FREE(env->allocator, simple_response);
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_status_line(
    struct axis2_http_simple_response * simple_response,
    const axutil_env_t * env,
    const axis2_char_t * http_ver,
    const int status_code,
    const axis2_char_t * phrase)
{
    if(!http_ver || !phrase || !status_code)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "invalid parameter given");
        return AXIS2_FAILURE;
    }

    if(simple_response->status_line)
    {
        axis2_http_status_line_free(simple_response->status_line, env);
    }

    simple_response->status_line = axis2_http_status_line_create_with_values(
        env, http_ver, status_code, phrase);
    if(!simple_response->status_line)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "http status line creation failed for string %s %3d %s", http_ver, status_code, phrase);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_phrase(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    if(!(simple_response->status_line))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "axis2 simple response , status line is not available");
        return NULL;
    }

    return axis2_http_status_line_get_reason_phrase(simple_response-> status_line, env);
}

int AXIS2_CALL
axis2_http_simple_response_get_status_code(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    if(!(simple_response->status_line))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "axis2 simple response , status line is not available");
        return -1;
    }
    return axis2_http_status_line_get_status_code(simple_response->status_line, env);
}

axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_http_version(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    if(!(simple_response->status_line))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "axis2 simple response , status line is not available");

        return NULL;
    }
    return axis2_http_status_line_get_http_version(simple_response->status_line, env);
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_http_version(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axis2_char_t *http_version)
{
    if(!(simple_response->status_line))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "axis2 simple response , status line is not available");

        return AXIS2_FAILURE;
    }
    axis2_http_status_line_set_http_version(simple_response->status_line, env, http_version);
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_status_line(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{

    if(!(simple_response->status_line))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "axis2 simple response , status line is not available");

        return NULL;
    }
    return axis2_http_status_line_to_string(simple_response->status_line, env);
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_http_simple_response_get_headers(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    return simple_response->header_group;
}

axutil_array_list_t *AXIS2_CALL
axis2_http_simple_response_extract_headers(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    axutil_array_list_t *temp = NULL;
    temp = simple_response->header_group;
    if(temp)
    {
        simple_response->header_group = NULL;
    }
    return temp;
}

axis2_http_header_t *AXIS2_CALL
axis2_http_simple_response_get_first_header(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    const axis2_char_t * str)
{
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    int i = 0;
    int count = 0;
    axutil_array_list_t *header_group = NULL;

    AXIS2_PARAM_CHECK(env->error, str, NULL);

    header_group = simple_response->header_group;
    if(!simple_response->header_group)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "axis2 simple response , headers not available");
        return NULL;
    }

    if(0 == axutil_array_list_size(header_group, env))
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "axis2 simple response , contains zero headers");

        return NULL;
    }

    count = axutil_array_list_size(header_group, env);

    for(i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)axutil_array_list_get(header_group, env, i);
        tmp_name = axis2_http_header_get_name(tmp_header, env);
        if(0 == axutil_strcasecmp(str, tmp_name))
        {
            return tmp_header;
        }
    }
    return NULL;

}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_remove_headers(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    const axis2_char_t * str)
{
    axutil_array_list_t *header_group = NULL;
    int i = 0;
    int count = 0;

    AXIS2_PARAM_CHECK(env->error, str, AXIS2_FAILURE);

    header_group = simple_response->header_group;
    if(!header_group)
    {
        /* Even though we couldn't complete the op, we are sure that the
         * required header is no more in the request. So we can proceed without a
         * problem.
         */
        return AXIS2_SUCCESS;
    }

    count = axutil_array_list_size(header_group, env);
    for(i = 0; i < count; i++)
    {
        axis2_http_header_t *tmp_header = NULL;
        axis2_char_t *tmp_name = NULL;
        tmp_header = (axis2_http_header_t *)axutil_array_list_get(header_group, env, i);
        tmp_name = axis2_http_header_get_name(tmp_header, env);
        if(0 == axutil_strcasecmp(str, tmp_name))
        {
            axis2_http_header_free(tmp_header, env);
            axutil_array_list_remove(header_group, env, i);
            break;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_header(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axis2_http_header_t * header)
{
    int i = 0;
    int count = 0;
    axutil_array_list_t *header_group = NULL;

    AXIS2_PARAM_CHECK(env->error, header, AXIS2_FAILURE);

    if(!simple_response->header_group)
    {
        simple_response->header_group = axutil_array_list_create(env, 10);
        axutil_array_list_add(simple_response->header_group, env, header);
        return AXIS2_SUCCESS;
    }

    /* If a header with the same name exists search and remove the old header */
    header_group = simple_response->header_group;
    count = axutil_array_list_size(header_group, env);
    for(i = 0; i < count; i++)
    {
        axis2_http_header_t *tmp_header = NULL;
        axis2_char_t *tmp_name = NULL;
        tmp_header = (axis2_http_header_t *)axutil_array_list_get(header_group, env, i);
        tmp_name = axis2_http_header_get_name(tmp_header, env);
        if(0 == axutil_strcasecmp(axis2_http_header_get_name(header, env), tmp_name))
        {
            axis2_http_header_free(tmp_header, env);
            axutil_array_list_set(header_group, env, i, header);
            return AXIS2_SUCCESS;
        }
    }

    /* if header is not found, then we have to add it */
    axutil_array_list_add(header_group, env, header);
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_charset(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header = NULL;

    tmp_header = axis2_http_simple_response_get_first_header(simple_response, env,
        AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if(tmp_header)
    {
        axis2_char_t *value = axis2_http_header_get_value(tmp_header, env);
        axis2_char_t *charset = (axis2_char_t *)strstr((char *)value,
            (char *)AXIS2_HTTP_CHAR_SET_ENCODING);
        if(charset)
        {
            charset = strchr((char *)charset, AXIS2_EQ);
            return charset;
        }
    }

    return AXIS2_HTTP_DEFAULT_CONTENT_CHARSET;
}

axis2_ssize_t AXIS2_CALL
axis2_http_simple_response_get_content_length(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header = NULL;
    int error_return = -1;

    tmp_header = axis2_http_simple_response_get_first_header(simple_response, env,
        AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if(tmp_header)
    {
        return AXIS2_ATOI(axis2_http_header_get_value(tmp_header, env));
    }
    return error_return;
}

const axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_content_type(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header = NULL;

    tmp_header = axis2_http_simple_response_get_first_header(simple_response, env,
        AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if(tmp_header)
    {
        return axis2_http_header_get_value(tmp_header, env);
    }

    return AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_body_string(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axis2_char_t * str)
{
    axutil_stream_t *body_stream = NULL;

    AXIS2_PARAM_CHECK(env->error, str, AXIS2_FAILURE);

    body_stream = simple_response->stream;
    if(!body_stream)
    {
        body_stream = axutil_stream_create_basic(env);
        if(!body_stream)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "unable to create basic stream for string %s",
                str);
            return AXIS2_FAILURE;
        }
        simple_response->stream = body_stream;
    }
    axutil_stream_write(body_stream, env, str, axutil_strlen(str));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_body_stream(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axutil_stream_t * stream)
{
    /*
     * We don't free the stream
     * Problem in freeing is most of the time the stream doesn't belong
     * to the http_simple_response
     */
    simple_response->stream = stream;
    return AXIS2_SUCCESS;
}

axutil_stream_t *AXIS2_CALL
axis2_http_simple_response_get_body(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    return simple_response->stream;
}

axis2_ssize_t AXIS2_CALL
axis2_http_simple_response_get_body_bytes(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axis2_char_t ** buffer)
{
    axutil_stream_t *tmp_stream = NULL;
    axis2_bool_t loop_state = AXIS2_TRUE;
    int return_size = -1;

    if(!simple_response->stream)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_BODY, AXIS2_FAILURE);
        return return_size;
    }
    tmp_stream = axutil_stream_create_basic(env);
    while(loop_state)
    {
        int read = 0;
        int write = 0;
        char buf[AXIS2_HTTP_SIMPLE_RESPONSE_READ_SIZE];

        read = axutil_stream_read(simple_response->stream, env, buf,
            AXIS2_HTTP_SIMPLE_RESPONSE_READ_SIZE);
        if(read < 0)
        {
            break;
        }
        write = axutil_stream_write(tmp_stream, env, buf, read);
        if(read < (AXIS2_HTTP_SIMPLE_RESPONSE_READ_SIZE - 1))
        {
            break;
        }
    }
    return_size = axutil_stream_get_len(tmp_stream, env);

    if(return_size > 0)
    {
        *buffer = (char *)AXIS2_MALLOC(env->allocator, sizeof(char) * (return_size + 1));

        if(!buffer)
        {
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return -1;
        }

        return_size = axutil_stream_read(tmp_stream, env, *buffer, return_size + 1);
    }
    axutil_stream_free(tmp_stream, env);
    return return_size;
}

axis2_bool_t AXIS2_CALL
axis2_http_simple_response_contains_header(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    const axis2_char_t * name)
{
    axis2_char_t *header_name = NULL;
    int count = 0;
    int i = 0;

    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    if(!simple_response->header_group)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "axis2 simple response , headers not available");
        return AXIS2_FALSE;
    }

    count = axutil_array_list_size(simple_response->header_group, env);
    if(0 == count)
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "axis2 simple response , contains zero headers");
        return AXIS2_FALSE;
    }

    for(i = 0; i < count; i++)
    {
        axis2_http_header_t *header = (axis2_http_header_t *)axutil_array_list_get(
            simple_response->header_group, env, i);
        header_name = axis2_http_header_get_name(header, env);
        if(0 == axutil_strcasecmp(name, header_name))
        {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_http_simple_response_get_mime_parts(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{

    return simple_response->mime_parts;

}

void AXIS2_EXTERN AXIS2_CALL
axis2_http_simple_response_set_mime_parts(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axutil_array_list_t *mime_parts)
{

    simple_response->mime_parts = mime_parts;

}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_mtom_sending_callback_name(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env)
{
    return simple_response->mtom_sending_callback_name;
}

void AXIS2_EXTERN AXIS2_CALL
axis2_http_simple_response_set_mtom_sending_callback_name(
    axis2_http_simple_response_t * simple_response,
    const axutil_env_t * env,
    axis2_char_t *mtom_sending_callback_name)
{
    simple_response->mtom_sending_callback_name =
    mtom_sending_callback_name;
}
