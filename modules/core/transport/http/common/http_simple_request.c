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

#include <axis2_http_simple_request.h>
#include <axis2_array_list.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>
#include <string.h>
#include <axis2_types.h>

/**
 * @brief HTTP Simple Request struct impl
 *   Axis2 HTTP Simple Request impl
 */

typedef struct axis2_http_simple_request_impl
{
    axis2_http_simple_request_t simple_request;
    axis2_http_request_line_t *request_line;
    axis2_array_list_t *header_group;
    axis2_stream_t *stream;
    axis2_bool_t owns_stream;
}
axis2_http_simple_request_impl_t;

#define AXIS2_INTF_TO_IMPL(simple_request) \
                 ((axis2_http_simple_request_impl_t *)(simple_request))

/***************************** Function headers *******************************/

axis2_http_request_line_t *AXIS2_CALL
axis2_http_simple_request_get_request_line(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_simple_request_set_request_line(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    axis2_http_request_line_t *request_line);

axis2_bool_t AXIS2_CALL
axis2_http_simple_request_contains_header(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_array_list_t *AXIS2_CALL
axis2_http_simple_request_get_headers(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

axis2_http_header_t *AXIS2_CALL
axis2_http_simple_request_get_first_header(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    const axis2_char_t *str);

axis2_status_t AXIS2_CALL
axis2_http_simple_request_remove_headers(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    const axis2_char_t *str);

axis2_status_t AXIS2_CALL
axis2_http_simple_request_add_header(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    axis2_http_header_t *header);

const axis2_char_t *AXIS2_CALL
axis2_http_simple_request_get_content_type(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_http_simple_request_get_charset(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

axis2_ssize_t AXIS2_CALL
axis2_http_simple_request_get_content_length(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

axis2_stream_t *AXIS2_CALL
axis2_http_simple_request_get_body(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

axis2_ssize_t AXIS2_CALL
axis2_http_simple_request_get_body_bytes(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    char **buf);

axis2_status_t AXIS2_CALL
axis2_http_simple_request_set_body_string(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    void *str,
    unsigned int str_len);

axis2_status_t AXIS2_CALL
axis2_http_simple_request_free(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env);

/***************************** End of function headers ************************/

axis2_http_simple_request_t *AXIS2_CALL
axis2_http_simple_request_create(
    const axis2_env_t *env,
    axis2_http_request_line_t *request_line,
    axis2_http_header_t **http_headers,
    axis2_ssize_t http_hdr_count,
    axis2_stream_t *content)
{
    axis2_http_simple_request_impl_t *simple_request_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_ENV_CHECK(env, NULL);

    simple_request_impl = (axis2_http_simple_request_impl_t *)AXIS2_MALLOC
            (env->allocator, sizeof(
                        axis2_http_simple_request_impl_t));

    if (NULL == simple_request_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    simple_request_impl->request_line = request_line;
    simple_request_impl->stream = content;
    simple_request_impl->header_group = NULL;
    simple_request_impl->owns_stream = AXIS2_FALSE;

    if (!(simple_request_impl->stream))
    {
        simple_request_impl->stream = axis2_stream_create_basic(env);
        if (NULL == simple_request_impl->stream)
        {
            axis2_http_simple_request_free((axis2_http_simple_request_t *)
                    simple_request_impl, env);
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        simple_request_impl->owns_stream = AXIS2_TRUE;
    }

    if (http_hdr_count > 0 &&  http_headers)
    {
        int i = 0;
        simple_request_impl->header_group = axis2_array_list_create(env,
                http_hdr_count);

        for (i = 0; i < http_hdr_count; i++)
        {
            axis2_array_list_add(simple_request_impl->header_group, env,
                    (void *)http_headers[i]);
        }
    }
    /*
        TODO : handle chunked streams
    */
    simple_request_impl->simple_request.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_http_simple_request_ops_t));
    if (NULL == simple_request_impl->simple_request.ops)
    {
        axis2_http_simple_request_free((axis2_http_simple_request_t*)
                simple_request_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_request_impl->simple_request.ops->get_request_line =
        axis2_http_simple_request_get_request_line;
    simple_request_impl->simple_request.ops->set_request_line =
        axis2_http_simple_request_set_request_line;
    simple_request_impl->simple_request.ops->contains_header =
        axis2_http_simple_request_contains_header;
    simple_request_impl->simple_request.ops->get_headers =
        axis2_http_simple_request_get_headers;
    simple_request_impl->simple_request.ops->get_first_header =
        axis2_http_simple_request_get_first_header;
    simple_request_impl->simple_request.ops->remove_headers =
        axis2_http_simple_request_remove_headers;
    simple_request_impl->simple_request.ops->add_header =
        axis2_http_simple_request_add_header;
    simple_request_impl->simple_request.ops->get_content_type =
        axis2_http_simple_request_get_content_type;
    simple_request_impl->simple_request.ops->get_charset =
        axis2_http_simple_request_get_charset;
    simple_request_impl->simple_request.ops->get_content_length =
        axis2_http_simple_request_get_content_length;
    simple_request_impl->simple_request.ops->get_body =
        axis2_http_simple_request_get_body;
    simple_request_impl->simple_request.ops->get_body_bytes =
        axis2_http_simple_request_get_body_bytes;
    simple_request_impl->simple_request.ops->set_body_string =
        axis2_http_simple_request_set_body_string;
    simple_request_impl->simple_request.ops->free =
        axis2_http_simple_request_free;

    return &(simple_request_impl->simple_request);
}

axis2_status_t AXIS2_CALL
axis2_http_simple_request_free(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    axis2_http_simple_request_impl_t *simple_request_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    simple_request_impl = AXIS2_INTF_TO_IMPL(simple_request);
    if (AXIS2_TRUE == simple_request_impl->owns_stream)
    {
        AXIS2_STREAM_FREE(simple_request_impl->stream, env);
        simple_request_impl->stream = NULL;
    }
    /*
        Don't free the stream since it belongs to the socket
        TODO : if chunked remove the chunked stream.
    */
    if (simple_request_impl->request_line)
    {
        AXIS2_HTTP_REQUEST_LINE_FREE(simple_request_impl->request_line, env);
        simple_request_impl->request_line = NULL;
    }
    if (simple_request_impl->header_group)
    {
        int i = 0;
        axis2_http_header_t *tmp = NULL;
        for (i = 0; i < axis2_array_list_size(simple_request_impl->header_group,
                env); i++)
        {
            tmp = (axis2_http_header_t *)axis2_array_list_get(
                        simple_request_impl->header_group, env, i);
            AXIS2_HTTP_HEADER_FREE(tmp, env);

        }
        axis2_array_list_free(simple_request_impl->header_group, env);
        simple_request_impl->header_group = NULL;
    }

    if (simple_request->ops)
        AXIS2_FREE(env->allocator, simple_request->ops);

    AXIS2_FREE(env->allocator, simple_request_impl);

    return AXIS2_SUCCESS;
}

axis2_http_request_line_t *AXIS2_CALL
axis2_http_simple_request_get_request_line(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_request)->request_line;
}


axis2_status_t AXIS2_CALL
axis2_http_simple_request_set_request_line(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    axis2_http_request_line_t *request_line)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, request_line, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(simple_request)->request_line = request_line;
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
axis2_http_simple_request_contains_header(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    int i = 0;
    axis2_char_t *header_name = NULL;
    axis2_http_simple_request_impl_t *simple_request_impl = NULL;
    int count = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    simple_request_impl = AXIS2_INTF_TO_IMPL(simple_request);

    if (NULL == simple_request_impl->header_group)
    {
        return AXIS2_FALSE;
    }

    count = axis2_array_list_size(simple_request_impl->header_group, env);

    if (0 == count)
    {
        return AXIS2_FALSE;
    }

    for (i = 0; i < count; i++)
    {
        header_name = AXIS2_HTTP_HEADER_GET_NAME((axis2_http_header_t *)
                axis2_array_list_get(simple_request_impl->header_group,
                        env, i), env);
        if (0 == axis2_strcasecmp(name, header_name))
            return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}


axis2_array_list_t *AXIS2_CALL
axis2_http_simple_request_get_headers(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_request)->header_group;
}


axis2_http_header_t *AXIS2_CALL
axis2_http_simple_request_get_first_header(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    const axis2_char_t *str)
{
    axis2_http_simple_request_impl_t *simple_request_impl = NULL;
    axis2_array_list_t *header_group = NULL;
    int i = 0;
    int count = 0;
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, str, NULL);

    simple_request_impl = AXIS2_INTF_TO_IMPL(simple_request);

    header_group = simple_request_impl->header_group;
    if (NULL == simple_request_impl->header_group)
    {
        return NULL;
    }
    if (0 == axis2_array_list_size(header_group, env))
    {
        return NULL;
    }


    count = axis2_array_list_size(header_group, env);


    for (i = 0; i < count; i++)
    {

        tmp_header = (axis2_http_header_t *)axis2_array_list_get(header_group,
                env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if (0 == axis2_strcasecmp(str, tmp_name))
        {
            return tmp_header;
        }
    }
    return NULL;
}


axis2_status_t AXIS2_CALL
axis2_http_simple_request_remove_headers(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    const axis2_char_t *str)
{
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_name = NULL;
    int i = 0;
    int count = 0;
    axis2_array_list_t *header_group = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, str, AXIS2_FAILURE);

    header_group = AXIS2_INTF_TO_IMPL(simple_request)->header_group;

    if (NULL == header_group)
    {
        /* Even though we couldn't complete the op, we are sure that the
         * requred header is no more in the request. So we can proceed without a
         * problem.
         */
        return AXIS2_SUCCESS;
    }

    count = axis2_array_list_size(header_group, env);

    for (i = 0; i < count; i++)
    {
        tmp_header = (axis2_http_header_t *)axis2_array_list_get(header_group,
                env, i);
        tmp_name = AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env);
        if (0 == axis2_strcasecmp(str, tmp_name))
        {
            AXIS2_HTTP_HEADER_FREE(tmp_header, env);
            axis2_array_list_remove(header_group, env, i);
            break;
        }
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_simple_request_add_header(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    axis2_http_header_t *header)
{
    axis2_http_simple_request_impl_t *simple_request_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, header, AXIS2_FAILURE);

    simple_request_impl = AXIS2_INTF_TO_IMPL(
                simple_request);
    if (NULL == simple_request_impl->header_group)
    {
        simple_request_impl->header_group = axis2_array_list_create(env, 1);
    }
    return axis2_array_list_add(AXIS2_INTF_TO_IMPL(simple_request)->header_group,
            env, header);
}


const axis2_char_t *AXIS2_CALL
axis2_http_simple_request_get_content_type(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    tmp_header = axis2_http_simple_request_get_first_header
            (simple_request, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (tmp_header)
        return AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);

    return AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
}


const axis2_char_t *AXIS2_CALL
axis2_http_simple_request_get_charset(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    axis2_http_header_t *tmp_header  = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    tmp_header = axis2_http_simple_request_get_first_header
            (simple_request, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (tmp_header)
    {
        axis2_char_t *value = AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);
        axis2_char_t *charset = (axis2_char_t *)strstr((char *)value,
                (char *)AXIS2_HTTP_CHAR_SET_ENCODING);
        if (charset)
        {
            charset = strchr((char *)charset, '=');
            return charset;
        }
    }

    return AXIS2_HTTP_DEFAULT_CONTENT_CHARSET;
}


axis2_ssize_t AXIS2_CALL
axis2_http_simple_request_get_content_length(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    tmp_header = axis2_http_simple_request_get_first_header
            (simple_request, env, AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if (tmp_header)
    {
        return AXIS2_ATOI(AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env));
    }
    return -1;
}


axis2_stream_t *AXIS2_CALL
axis2_http_simple_request_get_body(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_request)->stream;
}


axis2_ssize_t AXIS2_CALL
axis2_http_simple_request_get_body_bytes(
    const axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    char **buf)
{
    axis2_stream_t *body = NULL;
    char *tmp_buf = NULL;
    char *tmp_buf2 = NULL;
    char *tmp_buf3 = NULL;
    int length = 0;
    int read_len = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    body = AXIS2_INTF_TO_IMPL(simple_request)->stream;
    if (NULL == body)
    {
        *buf = (char*)AXIS2_MALLOC(env->allocator, 1);
        *buf[0] = '\0';
        return 0;
    }
    length = AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_LENGTH(simple_request, env);
    if (length > 0)
    {
        *buf = (char*)AXIS2_MALLOC(env->allocator, length + 1);
        read_len = AXIS2_STREAM_READ(body, env, *buf, length + 1);
        return read_len;
    }
    tmp_buf2 = AXIS2_MALLOC(env->allocator, 128 * sizeof(char));
    while (AXIS2_STREAM_READ(body, env, tmp_buf2, 128) > 0)
    {
        tmp_buf3 = axis2_stracat(tmp_buf, tmp_buf2, env);
        if (tmp_buf)
        {
            AXIS2_FREE(env->allocator, tmp_buf);
            tmp_buf = NULL;
        }
        tmp_buf = tmp_buf3;

    }
    /*
        TODO :STREAM_READ => STREAM_READ_BYTES
    */
    if (tmp_buf)
    {
        *buf = tmp_buf;
        return axis2_strlen(tmp_buf);
    }
    return -1;
}

axis2_status_t AXIS2_CALL
axis2_http_simple_request_set_body_string(
    axis2_http_simple_request_t *simple_request,
    const axis2_env_t *env,
    void *str,
    unsigned int str_len)
{
    axis2_stream_t *body_stream = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, str, AXIS2_FAILURE);

    body_stream = AXIS2_INTF_TO_IMPL(simple_request)->stream;
    if (NULL == body_stream)
    {
        body_stream = axis2_stream_create_basic(env);
        if (NULL == body_stream)
        {
            return AXIS2_FAILURE;
        }
        AXIS2_INTF_TO_IMPL(simple_request)->stream = body_stream;
        AXIS2_INTF_TO_IMPL(simple_request)->owns_stream = AXIS2_TRUE;
    }
    AXIS2_STREAM_WRITE(body_stream, env, str, str_len);
    return AXIS2_SUCCESS;
}
