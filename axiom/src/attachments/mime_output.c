/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *mime_output
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "axiom_mime_output.h"
#include "axiom_data_handler.h"
#include "axiom_mime_body_part.h"
#include <axis2_string.h>
#include <axiom_text.h>
#include <axiom_mime_const.h>

axis2_char_t AXIS2_CRLF[] =  { 13, 10 };

typedef struct axiom_mime_output_impl
{
    axiom_mime_output_t mime_output;
}
axiom_mime_output_impl_t;

#define AXIS2_INTF_TO_IMPL(mime_output) ((axiom_mime_output_impl_t *)(mime_output))


axis2_status_t axis2_char_2_byte(const axis2_env_t *env, axis2_char_t *char_buffer, axis2_byte_t **byte_buffer, int *byte_buffer_size);

axis2_status_t
axis2_char_2_byte(const axis2_env_t *env, axis2_char_t *char_buffer, axis2_byte_t **byte_buffer, int *byte_buffer_size)
{
    int length;
    int i = 0;
    axis2_byte_t *bytes;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    length = AXIS2_STRLEN(char_buffer);
    bytes = AXIS2_MALLOC(env->allocator, length * sizeof(axis2_byte_t));

    for (i = 0; i < length; i++)
    {
        bytes[i] = (axis2_byte_t) char_buffer[i];
    }
    *byte_buffer = bytes;
    *byte_buffer_size = length;
    return AXIS2_SUCCESS;
}

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axiom_mime_output_free(axiom_mime_output_t *mime_output, const axis2_env_t *env);

axis2_byte_t * AXIS2_CALL
axiom_mime_output_complete(axiom_mime_output_t *mime_output,
        const axis2_env_t *env,
        axis2_byte_t **output_stream,
        int *output_stream_size,
        axis2_char_t *soap_body_buffer,
        axis2_array_list_t *binary_node_list,
        axis2_char_t *boundary,
        axis2_char_t *content_id,
        axis2_char_t *char_set_encoding,
        const axis2_char_t *soap_content_type);
axis2_status_t AXIS2_CALL
axis2_start_writing_mime(axiom_mime_output_t *mime_output,
        const axis2_env_t *env, axis2_byte_t **output_stream,
        int *output_stream_size, axis2_char_t *boundary);

axiom_mime_body_part_t * AXIS2_CALL
axis2_create_mime_body_part(axiom_text_t *text, const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_write_body_part(axiom_mime_output_t *mime_output, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size,
        axiom_mime_body_part_t *part, axis2_char_t *boundary);

axis2_status_t AXIS2_CALL
axis2_write_mime_boundary(axiom_mime_output_t *mime_output, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size,
        axis2_char_t *boundary);

axis2_status_t AXIS2_CALL
axis2_write_finish_writing_mime(axiom_mime_output_t *mime_output, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size, axis2_char_t *boundary);

const axis2_char_t * AXIS2_CALL
axiom_mime_output_get_content_type_for_mime(axiom_mime_output_t *mime_output,
        const axis2_env_t *env,
        axis2_char_t *boundary,
        axis2_char_t *content_id,
        axis2_char_t *char_set_encoding,
        const axis2_char_t *soap_content_type);

/************************** End of Function headers ************************/

AXIS2_EXTERN axiom_mime_output_t * AXIS2_CALL
axiom_mime_output_create(const axis2_env_t *env)
{
    axiom_mime_output_impl_t *mime_output_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mime_output_impl = (axiom_mime_output_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axiom_mime_output_impl_t));

    if (NULL == mime_output_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mime_output_impl->mime_output.ops = NULL;

    mime_output_impl->mime_output.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axiom_mime_output_ops_t));
    if (NULL == mime_output_impl->mime_output.ops)
    {
        axiom_mime_output_free(&(mime_output_impl->mime_output), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mime_output_impl->mime_output.ops->free =  axiom_mime_output_free;
    mime_output_impl->mime_output.ops->complete = axiom_mime_output_complete;
    mime_output_impl->mime_output.ops->get_content_type_for_mime = axiom_mime_output_get_content_type_for_mime;
    return &(mime_output_impl->mime_output);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL

axiom_mime_output_free(axiom_mime_output_t *mime_output, const axis2_env_t *env)
{
    axiom_mime_output_impl_t *mime_output_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mime_output_impl = AXIS2_INTF_TO_IMPL(mime_output);

    if (mime_output->ops)
    {
        AXIS2_FREE(env->allocator, mime_output->ops);
        mime_output->ops = NULL;
    }

    if (mime_output_impl)
    {
        AXIS2_FREE(env->allocator, mime_output_impl);
        mime_output_impl = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_byte_t * AXIS2_CALL
axiom_mime_output_complete(axiom_mime_output_t *mime_output,
        const axis2_env_t *env,
        axis2_byte_t **output_stream,
        int *output_stream_size,
        axis2_char_t *soap_body_buffer,
        axis2_array_list_t *binary_node_list,
        axis2_char_t *boundary,
        axis2_char_t *content_id,
        axis2_char_t *char_set_encoding,
        const axis2_char_t *soap_content_type)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *header_value = NULL;
    axis2_char_t *content_id_string = NULL;
    axiom_mime_body_part_t *root_mime_body_part = NULL;
    axis2_byte_t *output_stream_start = NULL;
    int output_stream_start_size = 0;
    axis2_byte_t *output_stream_body = NULL;
    int output_stream_body_size = 0;
    axis2_byte_t *output_stream_body_parts = NULL;
    int output_stream_body_parts_size = 0;
    axis2_byte_t *boundary_stream = NULL;
    int boundary_stream_size = 0;
    axis2_byte_t *stream_buffer = NULL;
    int stream_buffer_size = 0;
    int soap_body_buffer_size = 0;

    AXIS2_ENV_CHECK(env, NULL);

    status = axis2_start_writing_mime(mime_output, env, &output_stream_start, &output_stream_start_size, boundary);
    if (status == AXIS2_FAILURE)
        return NULL;


    root_mime_body_part = axiom_mime_body_part_create(env);

    /* content-type */
    header_value = AXIS2_STRDUP("application/xop+xml; charset=", env);
    header_value = AXIS2_STRACAT(header_value, char_set_encoding, env);
    header_value = AXIS2_STRACAT(header_value, "; type=\"", env);
    header_value = AXIS2_STRACAT(header_value, soap_content_type, env);
    header_value = AXIS2_STRACAT(header_value, "\";", env);
    AXIOM_MIME_BODY_PART_ADD_HEADER(root_mime_body_part, env, "content-type", header_value);

    /* content-transfer-encoding */
    AXIOM_MIME_BODY_PART_ADD_HEADER(root_mime_body_part, env, "content-transfer-encoding", "binary");

    /* content-id */
    content_id_string = (axis2_char_t *)"<";
    content_id_string = AXIS2_STRACAT(content_id_string, content_id, env);
    content_id_string = AXIS2_STRACAT(content_id_string, ">", env);
    AXIOM_MIME_BODY_PART_ADD_HEADER(root_mime_body_part, env, "content-id", content_id_string);

    axis2_write_body_part(mime_output, env, &output_stream_body, &output_stream_body_size, root_mime_body_part, boundary);

    if (binary_node_list)
    {
        int j = 0;
        axis2_byte_t *temp_stream = NULL;
        int temp_stream_size = 0;
        axis2_byte_t *temp = NULL;
        int temp_size = 0;
        for (j = 0; j < AXIS2_ARRAY_LIST_SIZE(binary_node_list, env); j++)
        {
            axiom_text_t *text = (axiom_text_t *)AXIS2_ARRAY_LIST_GET(binary_node_list, env, j);
            if (text)
            {
                axiom_mime_body_part_t *mime_body_part = NULL;
                mime_body_part = axis2_create_mime_body_part(text, env);
                axis2_write_body_part(mime_output, env, &temp_stream,
                        &temp_stream_size, mime_body_part, boundary);
                temp = output_stream_body_parts;
                temp_size = output_stream_body_parts_size;
                output_stream_body_parts_size = temp_size +
                        output_stream_start_size + temp_stream_size;
                output_stream_body_parts = AXIS2_MALLOC(env->allocator,
                        output_stream_body_parts_size * sizeof(axis2_byte_t));
                if (!output_stream_body_parts)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                    return NULL;
                }

                if (temp)
                {
                    memcpy(output_stream_body_parts, temp, temp_size);
                    AXIS2_FREE(env->allocator, temp);
                    temp = NULL;
                }

                if (output_stream_start)
                {
                    memcpy(output_stream_body_parts + temp_size,
                            output_stream_start, output_stream_start_size);
                }

                if (temp_stream)
                {
                    memcpy(output_stream_body_parts + temp_size +
                            output_stream_start_size, temp_stream, temp_stream_size);
                    AXIS2_FREE(env->allocator, temp_stream);
                    temp_stream = NULL;
                }

                temp_size = 0;
                temp_stream_size = 0;
            }
        }
    }

    axis2_write_finish_writing_mime(mime_output,  env, &boundary_stream,
            &boundary_stream_size, boundary);

    if (soap_body_buffer)
    {
        soap_body_buffer = AXIS2_STRACAT(soap_body_buffer, "\r\n", env);
        soap_body_buffer_size = AXIS2_STRLEN(soap_body_buffer);
    }

    stream_buffer_size = output_stream_start_size + output_stream_body_size +
            soap_body_buffer_size + output_stream_body_parts_size +
            boundary_stream_size;

    stream_buffer = AXIS2_MALLOC(env->allocator,
            stream_buffer_size * sizeof(axis2_byte_t));

    if (output_stream_start)
    {
        memcpy(stream_buffer, output_stream_start, output_stream_start_size);
        AXIS2_FREE(env->allocator, output_stream_start);
        output_stream_start = NULL;
    }

    if (output_stream_body)
    {
        memcpy(stream_buffer + output_stream_start_size, output_stream_body,
                output_stream_body_size);
        AXIS2_FREE(env->allocator, output_stream_body);
        output_stream_body = NULL;
    }

    if (soap_body_buffer)
    {
        memcpy(stream_buffer + output_stream_start_size + output_stream_body_size,
                soap_body_buffer, soap_body_buffer_size);
    }

    if (output_stream_body_parts)
    {
        memcpy(stream_buffer + output_stream_start_size + output_stream_body_size +
                soap_body_buffer_size,
                output_stream_body_parts, output_stream_body_parts_size);
        AXIS2_FREE(env->allocator, output_stream_body_parts);
        output_stream_body_parts = NULL;
    }

    if (boundary_stream)
    {
        memcpy(stream_buffer + output_stream_start_size + output_stream_body_size +
                soap_body_buffer_size + output_stream_body_parts_size,
                boundary_stream, boundary_stream_size);
        AXIS2_FREE(env->allocator, boundary_stream);
        boundary_stream = NULL;
    }

    *output_stream = stream_buffer;
    *output_stream_size = stream_buffer_size;
    return stream_buffer;
}

axis2_status_t AXIS2_CALL
axis2_start_writing_mime(axiom_mime_output_t *mime_output,
        const axis2_env_t *env, axis2_byte_t **output_stream,
        int *output_stream_size, axis2_char_t *boundary)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return axis2_write_mime_boundary(mime_output, env, output_stream, output_stream_size, boundary);
}

axis2_status_t AXIS2_CALL
axis2_write_mime_boundary(axiom_mime_output_t *mime_output, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size,
        axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer = NULL;
    axis2_byte_t *byte_stream = NULL;
    int size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    axis2_char_2_byte(env, boundary, &byte_buffer, &size);
    if (!byte_buffer)
        return AXIS2_FAILURE;
    byte_stream = AXIS2_MALLOC(env->allocator, (size + 2) * sizeof(axis2_byte_t));
    if (!byte_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    byte_stream[0] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[1] = AXIOM_MIME_BOUNDARY_BYTE;
    if (byte_buffer)
    {
        memcpy(byte_stream + 2, byte_buffer, size);
        AXIS2_FREE(env->allocator, byte_buffer);
        byte_buffer = NULL;
    }

    /*AXIS2_FREE(env->allocator, byte_buffer);*/

    *output_stream = byte_stream;
    *output_stream_size = size + 2;

    return AXIS2_SUCCESS;
}

axiom_mime_body_part_t * AXIS2_CALL
axis2_create_mime_body_part(axiom_text_t *text, const axis2_env_t *env)
{
    axiom_data_handler_t *data_handler = NULL;
    const axis2_char_t *content_type = "application/octet-stream";
    axiom_mime_body_part_t * mime_body_part = axiom_mime_body_part_create(env);
    axis2_char_t *content_id = (axis2_char_t *)"<";
    if (!mime_body_part)
        return NULL;
    data_handler = AXIOM_TEXT_GET_DATA_HANDLER(text, env);

    if (data_handler)
    {
        content_type = AXIOM_DATA_HANDLER_GET_CONTENT_TYPE(data_handler, env);
    }

    AXIOM_MIME_BODY_PART_SET_DATA_HANDLER(mime_body_part, env,
            data_handler);
    content_id = AXIS2_STRACAT(content_id,
            AXIOM_TEXT_GET_CONTENT_ID(text, env), env);
    content_id = AXIS2_STRACAT(content_id, ">", env);
    AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env, "content-id", content_id);
    AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env, "content-type", content_type);
    AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env, "content-transfer-encoding", "binary");

    return mime_body_part;
}

axis2_status_t AXIS2_CALL
axis2_write_body_part(axiom_mime_output_t *mime_output, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size,
        axiom_mime_body_part_t *part, axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer = NULL;
    axis2_byte_t *byte_stream = NULL;
    int byte_buffer_size = 0;
    axis2_byte_t *body_stream = NULL;
    int body_stream_size = 0;
    int size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIOM_MIME_BODY_PART_WRITE_TO(part, env, &body_stream, &body_stream_size);

    size = byte_buffer_size + body_stream_size + 4;

    byte_stream = AXIS2_MALLOC(env->allocator, size * sizeof(axis2_byte_t));
    if (!byte_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    byte_stream[0] = AXIS2_CRLF[0];
    byte_stream[1] = AXIS2_CRLF[1];

    if (body_stream)
    {
        memcpy(byte_stream + 2, body_stream, body_stream_size);
        AXIS2_FREE(env->allocator, body_stream);
        body_stream = NULL;
    }

    byte_stream[body_stream_size + 2] = AXIS2_CRLF[0];
    byte_stream[body_stream_size + 3] = AXIS2_CRLF[1];

    if (byte_buffer)
    {
        memcpy(byte_stream + 2 + body_stream_size + 2, byte_buffer, byte_buffer_size);
        AXIS2_FREE(env->allocator, byte_buffer);
        byte_buffer = NULL;
    }

    *output_stream = byte_stream;
    *output_stream_size = size;

    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL
axis2_write_finish_writing_mime(axiom_mime_output_t *mime_output, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size, axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer;
    axis2_byte_t *byte_stream;
    int size;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    axis2_char_2_byte(env, boundary, &byte_buffer, &size);
    byte_stream = AXIS2_MALLOC(env->allocator, (size + 4) * sizeof(axis2_byte_t));

    byte_stream[0] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[1] = AXIOM_MIME_BOUNDARY_BYTE;
    if (byte_buffer)
    {
        memcpy(byte_stream + 2, byte_buffer, size);
        AXIS2_FREE(env->allocator, byte_buffer);
        byte_buffer = NULL;
    }

    byte_stream[size + 2] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[size + 3] = AXIOM_MIME_BOUNDARY_BYTE;

    AXIS2_FREE(env->allocator, byte_buffer);

    *output_stream = byte_stream;
    *output_stream_size = size + 4;

    return AXIS2_SUCCESS;

}

const axis2_char_t * AXIS2_CALL
axiom_mime_output_get_content_type_for_mime(axiom_mime_output_t *mime_output,
        const axis2_env_t *env,
        axis2_char_t *boundary,
        axis2_char_t *content_id,
        axis2_char_t *char_set_encoding,
        const axis2_char_t *soap_content_type)
{
    axis2_char_t *content_type_string = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    content_type_string = AXIS2_STRDUP("multipart/related", env);
    content_type_string = AXIS2_STRACAT(content_type_string, "; ", env);
    if (boundary)
    {
        content_type_string = AXIS2_STRACAT(content_type_string, "boundary=", env);
        content_type_string = AXIS2_STRACAT(content_type_string, boundary, env);
        content_type_string = AXIS2_STRACAT(content_type_string, "; ", env);
    }
    content_type_string = AXIS2_STRACAT(content_type_string, "type=\"application/xop+xml\"", env);
    content_type_string = AXIS2_STRACAT(content_type_string, "; ", env);
    if (content_id)
    {
        content_type_string = AXIS2_STRACAT(content_type_string, "start=\"<", env);
        content_type_string = AXIS2_STRACAT(content_type_string, content_id , env);
        content_type_string = AXIS2_STRACAT(content_type_string, ">\"", env);
        content_type_string = AXIS2_STRACAT(content_type_string, "; ", env);
    }
    if (soap_content_type)
    {
        content_type_string = AXIS2_STRACAT(content_type_string, "start-info=\"", env);
        content_type_string = AXIS2_STRACAT(content_type_string, soap_content_type, env);
        content_type_string = AXIS2_STRACAT(content_type_string, "\"; ", env);
    }
    if (char_set_encoding)
    {
        content_type_string = AXIS2_STRACAT(content_type_string, "charset=\"", env);
        content_type_string = AXIS2_STRACAT(content_type_string, char_set_encoding, env);
        content_type_string = AXIS2_STRACAT(content_type_string, "\"", env);
    }

    return content_type_string;
}
