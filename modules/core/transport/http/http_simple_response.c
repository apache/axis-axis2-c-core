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
#include <axis2_http_simple_response.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>
#include <stdio.h>
#include <string.h>
#include <axis2_types.h>

/**
 * @brief HTTP Simple Response struct impl
 *   Axis2 HTTP Simple Response impl
 */
#define READ_SIZE 32


typedef struct axis2_http_simple_response_impl
{
    axis2_http_simple_response_t simple_response;
    axis2_http_status_line_t *status_line;
    axis2_array_list_t *header_group;
    axis2_stream_t *stream;
}
axis2_http_simple_response_impl_t;

#define AXIS2_INTF_TO_IMPL(simple_response) \
                 ((axis2_http_simple_response_impl_t *)(simple_response))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_status_line(
    struct axis2_http_simple_response *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *http_ver,
    const int status_code,
    const axis2_char_t *phrase);

axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_phrase(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

int AXIS2_CALL
axis2_http_simple_response_get_status_code(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_http_version(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_status_line(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_http_simple_response_contains_header(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_array_list_t *AXIS2_CALL
axis2_http_simple_response_get_headers(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_http_header_t *AXIS2_CALL
axis2_http_simple_response_get_first_header(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *str);

axis2_status_t AXIS2_CALL
axis2_http_simple_response_remove_headers(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *str);

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_header(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_http_header_t *header);

const axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_charset(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_ssize_t AXIS2_CALL
axis2_http_simple_response_get_content_length(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_content_type(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_body_string(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_char_t *str);

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_body_stream(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_stream_t *stream);

axis2_stream_t *AXIS2_CALL
axis2_http_simple_response_get_body(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

axis2_ssize_t AXIS2_CALL
axis2_http_simple_response_get_body_bytes(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_char_t **buf);

axis2_status_t AXIS2_CALL
axis2_http_simple_response_free(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env);

/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_http_simple_response_t *AXIS2_CALL
axis2_http_simple_response_create(
    const axis2_env_t *env,
    axis2_http_status_line_t *status_line,
    const axis2_http_header_t **http_headers,
    const axis2_ssize_t http_hdr_count,
    axis2_stream_t *content)
{
    axis2_http_simple_response_t *ret = NULL;
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_ENV_CHECK(env, NULL);

    ret = axis2_http_simple_response_create_default(env);
    if (NULL == ret)
    {
        return NULL;
    }
    simple_response_impl = AXIS2_INTF_TO_IMPL(ret);

    if (NULL == simple_response_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    simple_response_impl->status_line = status_line;
    if (http_hdr_count > 0 && NULL != http_headers)
    {
        int i = 0;
        simple_response_impl->header_group = axis2_array_list_create(env,
                http_hdr_count);

        for (i = 0; i < http_hdr_count; i++)
        {
            AXIS2_ARRAY_LIST_ADD(simple_response_impl->header_group, env,
                    (void *)http_headers[i]);
        }
    }
    simple_response_impl->stream = content;

    return ret;
}


AXIS2_EXTERN axis2_http_simple_response_t *AXIS2_CALL
axis2_http_simple_response_create_default(
    const axis2_env_t *env)
{
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    simple_response_impl = (axis2_http_simple_response_impl_t *)AXIS2_MALLOC
            (env->allocator, sizeof(
                        axis2_http_simple_response_impl_t));

    simple_response_impl->simple_response.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_http_simple_response_ops_t));
    if (NULL == simple_response_impl->simple_response.ops)
    {
        axis2_http_simple_response_free((axis2_http_simple_response_t *)
                simple_response_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    simple_response_impl->status_line = NULL;
    simple_response_impl->header_group = NULL;
    simple_response_impl->stream = NULL;

    simple_response_impl->simple_response.ops->set_status_line =
        axis2_http_simple_response_set_status_line;
    simple_response_impl->simple_response.ops->get_phrase =
        axis2_http_simple_response_get_phrase;
    simple_response_impl->simple_response.ops->get_status_code =
        axis2_http_simple_response_get_status_code;
    simple_response_impl->simple_response.ops->get_http_version =
        axis2_http_simple_response_get_http_version;
    simple_response_impl->simple_response.ops->get_status_line =
        axis2_http_simple_response_get_status_line;
    simple_response_impl->simple_response.ops->contains_header =
        axis2_http_simple_response_contains_header;
    simple_response_impl->simple_response.ops->get_headers =
        axis2_http_simple_response_get_headers;
    simple_response_impl->simple_response.ops->get_first_header =
        axis2_http_simple_response_get_first_header;
    simple_response_impl->simple_response.ops->remove_headers =
        axis2_http_simple_response_remove_headers;
    simple_response_impl->simple_response.ops->set_header =
        axis2_http_simple_response_set_header;
    simple_response_impl->simple_response.ops->get_charset =
        axis2_http_simple_response_get_charset;
    simple_response_impl->simple_response.ops->get_content_type =
        axis2_http_simple_response_get_content_type;
    simple_response_impl->simple_response.ops->get_content_length =
        axis2_http_simple_response_get_content_length;
    simple_response_impl->simple_response.ops->set_body_string =
        axis2_http_simple_response_set_body_string;
    simple_response_impl->simple_response.ops->set_body_stream =
        axis2_http_simple_response_set_body_stream;
    simple_response_impl->simple_response.ops->get_body =
        axis2_http_simple_response_get_body;
    simple_response_impl->simple_response.ops->get_body_bytes =
        axis2_http_simple_response_get_body_bytes;
    simple_response_impl->simple_response.ops->free =
        axis2_http_simple_response_free;

    return &(simple_response_impl->simple_response);
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_free(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    simple_response_impl = AXIS2_INTF_TO_IMPL(simple_response);

    if (NULL != simple_response_impl->status_line)
    {
        AXIS2_HTTP_STATUS_LINE_FREE(simple_response_impl->status_line, env);
        simple_response_impl->status_line = NULL;
    }
    if (NULL != simple_response_impl->header_group)
    {
        int i = 0;
        axis2_http_header_t *tmp = NULL;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(simple_response_impl->header_group,
                env); i++)
        {
            tmp = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(
                        simple_response_impl->header_group, env, i);
            if (NULL != tmp)
            {
                AXIS2_HTTP_HEADER_FREE(tmp, env);
            }
        }
        AXIS2_ARRAY_LIST_FREE(simple_response_impl->header_group, env);
        simple_response_impl->header_group = NULL;
    }
    if (NULL != simple_response->ops)
        AXIS2_FREE(env->allocator, simple_response->ops);

    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(simple_response));
    /* Stream is not freed
     * Assumption : stream doesn't belong to the response
     */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_status_line(
    struct axis2_http_simple_response *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *http_ver,
    const int status_code,
    const axis2_char_t *phrase)
{
    axis2_char_t *tmp_status_line_str = NULL;
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, http_ver, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, status_code, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phrase, AXIS2_FAILURE);

    tmp_status_line_str = AXIS2_MALLOC(env->allocator,
            (AXIS2_STRLEN(http_ver) + AXIS2_STRLEN(phrase) + 8) *
            sizeof(axis2_char_t *));
    sprintf(tmp_status_line_str, "%s %3d %s%s", http_ver, status_code, phrase,
            AXIS2_HTTP_CRLF);
    simple_response_impl = AXIS2_INTF_TO_IMPL
            (simple_response);
    if (NULL != simple_response_impl->status_line)
    {
        AXIS2_HTTP_STATUS_LINE_FREE(simple_response_impl->status_line, env);
        simple_response_impl->status_line = NULL;
    }
    simple_response_impl->status_line = axis2_http_status_line_create(env,
            tmp_status_line_str);
    AXIS2_FREE(env->allocator, tmp_status_line_str);

    if (NULL == simple_response_impl->status_line)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_phrase(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (NULL == AXIS2_INTF_TO_IMPL(simple_response)->status_line)
    {
        return NULL;
    }
    return AXIS2_HTTP_STATUS_LINE_GET_REASON_PHRASE(AXIS2_INTF_TO_IMPL(
                simple_response)->status_line, env);
}


int AXIS2_CALL
axis2_http_simple_response_get_status_code(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (NULL == AXIS2_INTF_TO_IMPL(simple_response)->status_line)
    {
        return -1;
    }
    return AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(AXIS2_INTF_TO_IMPL(
                simple_response)->status_line, env);
}


axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_http_version(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (NULL == AXIS2_INTF_TO_IMPL(simple_response)->status_line)
    {
        return NULL;
    }
    return AXIS2_HTTP_STATUS_LINE_GET_HTTP_VERSION(AXIS2_INTF_TO_IMPL(
                simple_response)->status_line, env);
}


axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_status_line(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (NULL == AXIS2_INTF_TO_IMPL(simple_response)->status_line)
    {
        return NULL;
    }
    return AXIS2_HTTP_STATUS_LINE_TO_STRING(AXIS2_INTF_TO_IMPL(
                simple_response)->status_line, env);
}

axis2_array_list_t *AXIS2_CALL
axis2_http_simple_response_get_headers(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_response)->header_group;
}

axis2_http_header_t *AXIS2_CALL
axis2_http_simple_response_get_first_header(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *str)
{
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    int i = 0;
    int count = 0;
    axis2_array_list_t *header_group = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, str, NULL);

    simple_response_impl = AXIS2_INTF_TO_IMPL(simple_response);
    header_group = simple_response_impl->header_group;
    if (NULL == simple_response_impl->header_group)
    {
        return NULL;
    }
    if (0 == AXIS2_ARRAY_LIST_SIZE(header_group, env))
    {
        return NULL;
    }

    count = AXIS2_ARRAY_LIST_SIZE(header_group, env);

    for (i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(header_group,
                env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if (0 == AXIS2_STRCASECMP(str, tmp_name))
        {
            return tmp_header;
        }
    }
    return NULL;

}


axis2_status_t AXIS2_CALL
axis2_http_simple_response_remove_headers(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *str)
{
    axis2_array_list_t *header_group = NULL;
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    int i = 0;
    int count = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, str, AXIS2_FAILURE);

    header_group = AXIS2_INTF_TO_IMPL(simple_response)->
            header_group;
    if (NULL == header_group)
    {
        /* Even though we couldn't complete the op, we are sure that the
         * requred header is no more in the request. So we can proceed without a
         * problem.
         */
        return AXIS2_SUCCESS;
    }

    count = AXIS2_ARRAY_LIST_SIZE(header_group, env);


    for (i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(header_group,
                env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if (0 == AXIS2_STRCASECMP(str, tmp_name))
        {
            AXIS2_HTTP_HEADER_FREE(tmp_header, env);
            AXIS2_ARRAY_LIST_REMOVE(header_group, env, i);
            break;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_header(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_http_header_t *header)
{
    int i = 0;
    int count = 0;
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    axis2_array_list_t *header_group = NULL;
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, header, AXIS2_FAILURE);

    simple_response_impl = AXIS2_INTF_TO_IMPL
            (simple_response);

    if (NULL == simple_response_impl->header_group)
    {
        simple_response_impl->header_group = axis2_array_list_create(env, 10);
        AXIS2_ARRAY_LIST_ADD(simple_response_impl->header_group, env, header);
        return AXIS2_SUCCESS;
    }

    /* If a header with the same name exists
     * search and remove the old header
     */
    header_group = AXIS2_INTF_TO_IMPL(simple_response)->header_group;

    count = AXIS2_ARRAY_LIST_SIZE(header_group, env);
    for (i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(header_group,
                env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if (0 == AXIS2_STRCASECMP(AXIS2_HTTP_HEADER_GET_NAME(header, env),
                tmp_name))
        {
            AXIS2_HTTP_HEADER_FREE(tmp_header, env);
            AXIS2_ARRAY_LIST_REMOVE(header_group, env, i);
            break;
        }
    }
    AXIS2_ARRAY_LIST_ADD(simple_response_impl->header_group, env,
            (void *)header);
    return AXIS2_SUCCESS;
}


const axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_charset(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    tmp_header = axis2_http_simple_response_get_first_header
            (simple_response, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (NULL != tmp_header)
    {
        axis2_char_t *value = AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);
        axis2_char_t *charset = (axis2_char_t *)strstr((char *)value,
                (char *)AXIS2_HTTP_CHAR_SET_ENCODING);
        if (NULL != charset)
        {
            charset = strchr((char *)charset, '=');
            return charset;
        }
    }

    return AXIS2_HTTP_DEFAULT_CONTENT_CHARSET;
}


axis2_ssize_t AXIS2_CALL
axis2_http_simple_response_get_content_length(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    tmp_header = axis2_http_simple_response_get_first_header
            (simple_response, env, AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if (NULL != tmp_header)
    {
        return AXIS2_ATOI(AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env));
    }
    return -1;
}


const axis2_char_t *AXIS2_CALL
axis2_http_simple_response_get_content_type(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    tmp_header =  axis2_http_simple_response_get_first_header
            (simple_response, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (NULL != tmp_header)
        return AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);

    return AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
}


axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_body_string(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_char_t *str)
{
    axis2_stream_t *body_stream = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, str, AXIS2_FAILURE);

    body_stream = AXIS2_INTF_TO_IMPL(simple_response)->stream;
    if (NULL == body_stream)
    {
        body_stream = axis2_stream_create_basic(env);
        if (NULL == body_stream)
        {
            return AXIS2_FAILURE;
        }
        AXIS2_INTF_TO_IMPL(simple_response)->stream = body_stream;
    }
    AXIS2_STREAM_WRITE(body_stream, env, str, AXIS2_STRLEN(str));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_response_set_body_stream(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_stream_t *stream)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    /*
     * We don't free the stream
     * Problem in freeing is most of the time the stream doesn't belong
     * to the http_simple_response
     */
    AXIS2_INTF_TO_IMPL(simple_response)->stream = stream;
    return AXIS2_SUCCESS;
}

axis2_stream_t *AXIS2_CALL
axis2_http_simple_response_get_body(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_response)->stream;
}


axis2_ssize_t AXIS2_CALL
axis2_http_simple_response_get_body_bytes(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    axis2_char_t **buffer)
{
    axis2_http_simple_response_impl_t *response_impl = NULL;
    axis2_stream_t *tmp_stream = NULL;
    int return_size = -1;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);


    response_impl = AXIS2_INTF_TO_IMPL(simple_response);
    if (NULL == response_impl->stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_BODY, AXIS2_FAILURE);
        return -1;
    }
    tmp_stream = axis2_stream_create_basic(env);
    while (1)
    {
        int read = 0;
        int write = 0;
        /*   int READ_SIZE = 32; */
        char buf[READ_SIZE];
        read = AXIS2_STREAM_READ(response_impl->stream, env, buf, READ_SIZE);
        if (read < 0)
        {
            break;
        }
        write = AXIS2_STREAM_WRITE(tmp_stream, env, buf, read);
        if (read < (READ_SIZE - 1))
        {
            break;
        }
    }
    return_size = AXIS2_STREAM_BASIC_GET_LEN(tmp_stream, env);

    if (return_size > 0)
    {
        *buffer = (char *)AXIS2_MALLOC(env->allocator, sizeof(char) *
                (return_size + 1));
        return_size = AXIS2_STREAM_READ(tmp_stream, env, *buffer,
                return_size + 1);
    }
    AXIS2_STREAM_FREE(tmp_stream, env);
    return return_size;
}

axis2_bool_t AXIS2_CALL
axis2_http_simple_response_contains_header(
    axis2_http_simple_response_t *simple_response,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    axis2_char_t *header_name = NULL;
    axis2_http_simple_response_impl_t *simple_response_impl = NULL;
    int count = 0;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    simple_response_impl = AXIS2_INTF_TO_IMPL(
                simple_response);
    if (NULL == simple_response_impl->header_group)
    {
        return AXIS2_FALSE;
    }
    count = AXIS2_ARRAY_LIST_SIZE(simple_response_impl->header_group, env);
    if (0 == count)
    {
        return AXIS2_FALSE;
    }

    for (i = 0; i < count; i++)
    {
        header_name = AXIS2_HTTP_HEADER_GET_NAME((axis2_http_header_t *)
                AXIS2_ARRAY_LIST_GET(simple_response_impl->header_group,
                        env, i), env);
        if (0 == AXIS2_STRCASECMP(name, header_name))
            return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}
