/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *mime_body_part
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axiom_mime_body_part.h>
#include <axiom_data_handler.h>
#include <axis2_hash.h>

typedef struct axiom_mime_body_part_impl
{
    axiom_mime_body_part_t mime_body_part;
    /** hash map to hold header name, value pairs */
    axis2_hash_t *header_map;
    axiom_data_handler_t *data_handler;
}
axiom_mime_body_part_impl_t;

extern axis2_char_t AXIS2_CRLF[];


#define AXIS2_INTF_TO_IMPL(mime_body_part) ((axiom_mime_body_part_impl_t *)(mime_body_part))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axiom_mime_body_part_free(axiom_mime_body_part_t *mime_body_part, const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axiom_mime_body_part_add_header(axiom_mime_body_part_t *mime_body_part,
        const axis2_env_t *env,
        const axis2_char_t *name,
        const axis2_char_t *value);

axis2_status_t AXIS2_CALL
axiom_mime_body_part_set_data_handler(axiom_mime_body_part_t *mime_body_part, const axis2_env_t *env, axiom_data_handler_t *data_handler);

axis2_status_t AXIS2_CALL
axiom_mime_body_part_write_to(axiom_mime_body_part_t *mime_body_part, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size);
/************************** End of Function headers ************************/

AXIS2_EXTERN axiom_mime_body_part_t * AXIS2_CALL
axiom_mime_body_part_create(const axis2_env_t *env)
{
    axiom_mime_body_part_impl_t *mime_body_part_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mime_body_part_impl = (axiom_mime_body_part_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axiom_mime_body_part_impl_t));

    if (NULL == mime_body_part_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mime_body_part_impl->header_map = NULL;
    mime_body_part_impl->data_handler = NULL;
    mime_body_part_impl->mime_body_part.ops = NULL;

    mime_body_part_impl->header_map = axis2_hash_make(env);
    if (!(mime_body_part_impl->header_map))
    {
        axiom_mime_body_part_free(&(mime_body_part_impl->mime_body_part), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mime_body_part_impl->mime_body_part.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axiom_mime_body_part_ops_t));
    if (NULL == mime_body_part_impl->mime_body_part.ops)
    {
        axiom_mime_body_part_free(&(mime_body_part_impl->mime_body_part), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mime_body_part_impl->mime_body_part.ops->free =  axiom_mime_body_part_free;
    mime_body_part_impl->mime_body_part.ops->add_header = axiom_mime_body_part_add_header;
    mime_body_part_impl->mime_body_part.ops->set_data_handler = axiom_mime_body_part_set_data_handler;
    mime_body_part_impl->mime_body_part.ops->write_to = axiom_mime_body_part_write_to;

    return &(mime_body_part_impl->mime_body_part);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axiom_mime_body_part_free(axiom_mime_body_part_t *mime_body_part, const axis2_env_t *env)
{
    axiom_mime_body_part_impl_t *mime_body_part_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mime_body_part_impl = AXIS2_INTF_TO_IMPL(mime_body_part);

    if (mime_body_part_impl->header_map)
    {
        axis2_hash_free(mime_body_part_impl->header_map, env);
        mime_body_part_impl->header_map = NULL;
    }

    if (NULL != mime_body_part->ops)
    {
        AXIS2_FREE(env->allocator, mime_body_part->ops);
        mime_body_part->ops = NULL;
    }

    if (mime_body_part_impl)
    {
        AXIS2_FREE(env->allocator, mime_body_part_impl);
        mime_body_part_impl = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axiom_mime_body_part_add_header(axiom_mime_body_part_t *mime_body_part,
        const axis2_env_t *env,
        const axis2_char_t *name,
        const axis2_char_t *value)
{
    axiom_mime_body_part_impl_t *mime_body_part_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    mime_body_part_impl = AXIS2_INTF_TO_IMPL(mime_body_part);

    axis2_hash_set(mime_body_part_impl->header_map, name,
            AXIS2_HASH_KEY_STRING, value);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_mime_body_part_set_data_handler(axiom_mime_body_part_t *mime_body_part,
        const axis2_env_t *env, axiom_data_handler_t *data_handler)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(mime_body_part)->data_handler = data_handler;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_mime_body_part_write_to(axiom_mime_body_part_t *mime_body_part, const axis2_env_t *env,
        axis2_byte_t **output_stream, int *output_stream_size)
{
    axiom_mime_body_part_impl_t *mime_body_part_impl = NULL;
    axis2_hash_index_t *hash_index = NULL;
    const void *key = NULL;
    void *value = NULL;
    axis2_char_t *header_str = NULL;
    int header_str_size = 0;
    axis2_byte_t *data_handler_stream = NULL;
    int data_handler_stream_size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_byte_t *byte_stream = NULL;
    int size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    mime_body_part_impl = AXIS2_INTF_TO_IMPL(mime_body_part);

    for (hash_index = axis2_hash_first(mime_body_part_impl->header_map, env);
            hash_index; hash_index = axis2_hash_next(env, hash_index))
    {
        axis2_hash_this(hash_index, &key, NULL, &value);
        if (key && value)
        {
            header_str = AXIS2_STRACAT(header_str, (axis2_char_t*)key, env);
            header_str = AXIS2_STRACAT(header_str, ": ", env);
            header_str = AXIS2_STRACAT(header_str, (axis2_char_t*)value, env);
            header_str = AXIS2_STRACAT(header_str, "\r\n", env);
        }
    }

    if (mime_body_part_impl->data_handler)
        header_str = AXIS2_STRACAT(header_str, "\r\n", env);

    if (header_str)
        header_str_size = AXIS2_STRLEN(header_str);

    /* TODO encoding needs to be done, we only support binary as of now,
       Java supports "base64", "uuencode", "x-uuencode", "x-uue", 
       "quoted-printable" 
    */
    if (mime_body_part_impl->data_handler)
    {
        status = AXIOM_DATA_HANDLER_READ_FROM(mime_body_part_impl->data_handler, env,
                &data_handler_stream, &data_handler_stream_size);
        if (status == AXIS2_FAILURE)
            return AXIS2_FAILURE;
    }

    size = header_str_size + data_handler_stream_size;
    byte_stream = AXIS2_MALLOC(env->allocator, (size) * sizeof(axis2_byte_t));
    if (!byte_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        if (data_handler_stream)
        {
            AXIS2_FREE(env->allocator, data_handler_stream);
            data_handler_stream = NULL;
        }
        if (header_str)
        {
            AXIS2_FREE(env->allocator, header_str);
            header_str = NULL;
        }
        return AXIS2_FAILURE;
    }
    /*TODO char2byte header_str */
    if (header_str)
        memcpy(byte_stream, header_str, header_str_size);
    if (data_handler_stream)
        memcpy(byte_stream + header_str_size, data_handler_stream, data_handler_stream_size);

    *output_stream = byte_stream;
    *output_stream_size = size;

    return AXIS2_SUCCESS;
}
