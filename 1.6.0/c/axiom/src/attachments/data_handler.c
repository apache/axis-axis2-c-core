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

#include <axiom_data_handler.h>
#include <stdio.h>
#include <sys/stat.h>
#include <axiom_mime_part.h>

struct axiom_data_handler
{
    /* The content type */
    axis2_char_t *mime_type;
    
    /* If in a file then the file name*/
    axis2_char_t *file_name;

    /* If it is in a buffer then the buffer */
    axis2_byte_t *buffer;

    /* The length of the buffer */
    int buffer_len;

    /* Is this a data_handler with a file name or a buffer*/
    axiom_data_handler_type_t data_handler_type;

    /* When parsing whether we have cached it or not */
    axis2_bool_t cached;

    /* The Content Id */
    axis2_char_t *mime_id;

    /* In the case of sending callback this is required */
    void *user_param;    

};


/* Creates the data_handler. The file name is not mandatory */

AXIS2_EXTERN axiom_data_handler_t *AXIS2_CALL
axiom_data_handler_create(
    const axutil_env_t *env,
    const axis2_char_t *file_name,
    const axis2_char_t *mime_type)
{
    axiom_data_handler_t *data_handler = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    data_handler = (axiom_data_handler_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axiom_data_handler_t));

    if (!data_handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create data handler");
        return NULL;
    }

    data_handler->mime_type = NULL;
    data_handler->file_name = NULL;
    data_handler->buffer = NULL;
    data_handler->buffer_len = 0;
    /* By default, a Data Handler is of type Buffer */
    data_handler->data_handler_type = AXIOM_DATA_HANDLER_TYPE_BUFFER;
    data_handler->cached = AXIS2_FALSE;
    data_handler->mime_id = NULL;
    data_handler->user_param = NULL;

    if (mime_type)
    {
        data_handler->mime_type = axutil_strdup(env, mime_type);
        if (!(data_handler->mime_type))
        {
            axiom_data_handler_free(data_handler, env);
            return NULL;
        }
    }
    if (file_name)
    {
        data_handler->file_name = axutil_strdup(env, file_name);
        if (!(data_handler->file_name))
        {
            axiom_data_handler_free(data_handler, env);
            return NULL;
        }
        data_handler->data_handler_type = AXIOM_DATA_HANDLER_TYPE_FILE;
    }

    return data_handler;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_data_handler_free(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    if (data_handler->file_name)
    {
        AXIS2_FREE(env->allocator, data_handler->file_name);
    }

    if (data_handler->mime_type)
    {
        AXIS2_FREE(env->allocator, data_handler->mime_type);
    }

    if (data_handler->buffer)
    {
        AXIS2_FREE(env->allocator, data_handler->buffer);
    }

    if(data_handler->mime_id)
    {
        AXIS2_FREE(env->allocator, data_handler->mime_id);
    }

    if (data_handler)
    {
        AXIS2_FREE(env->allocator, data_handler);
    }

    return;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_data_handler_get_content_type(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->mime_type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_set_content_type(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
	const axis2_char_t *mime_type)
{
	if(data_handler->mime_type)
	{
		AXIS2_FREE(env->allocator, data_handler->mime_type);
	}
	data_handler->mime_type = axutil_strdup(env, mime_type);
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_data_handler_get_cached(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->cached;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_data_handler_set_cached(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
	axis2_bool_t cached)
{
    data_handler->cached = cached;
}


AXIS2_EXTERN axis2_byte_t *AXIS2_CALL
axiom_data_handler_get_input_stream(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->buffer;
}

AXIS2_EXTERN int AXIS2_CALL
axiom_data_handler_get_input_stream_len(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->buffer_len;
}


/* With MTOM caching support this function is no longer used
 * Because this will load whole file in to buffer. So for large 
 * attachment this is not wise */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_read_from(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
    axis2_byte_t **output_stream,
    int *output_stream_size)
{
    if (data_handler->data_handler_type == AXIOM_DATA_HANDLER_TYPE_BUFFER)
    {
        *output_stream = data_handler->buffer;
        *output_stream_size = data_handler->buffer_len;
    }
    else if (data_handler->data_handler_type == AXIOM_DATA_HANDLER_TYPE_FILE
             && data_handler->file_name)
    {
        FILE *f = NULL;
        axis2_byte_t *byte_stream = NULL;
        axis2_byte_t *temp_byte_stream = NULL;
        axis2_byte_t *read_stream = NULL;
        int byte_stream_size = 0;
        int temp_byte_stream_size = 0;
        int read_stream_size = 0;
        int count = 0;
        struct stat stat_p;

        f = fopen(data_handler->file_name, "rb");
        if (!f)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Error opening file %s for reading",
                data_handler->file_name);
            return AXIS2_FAILURE;
        }

        if (stat(data_handler->file_name, &stat_p) == -1)
        {
	    fclose(f);
            return AXIS2_FAILURE;
        }
        else if (stat_p.st_size == 0)
        {
            fclose(f);
            *output_stream = NULL;
            *output_stream_size = 0;
            return AXIS2_SUCCESS;
        }

        do
        {
            read_stream_size = stat_p.st_size;
            read_stream = AXIS2_MALLOC(env->allocator,
                (read_stream_size) * sizeof(axis2_byte_t));
            if (!read_stream)
            {
                AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "No memory. Cannot create binary stream");
                if (byte_stream)
                {
                    AXIS2_FREE(env->allocator, byte_stream);
                }
                fclose(f);
                return AXIS2_FAILURE;
            }
            count = (int)fread(read_stream, 1, read_stream_size, f);
            /* The count lies within the int range */
            if (ferror(f))
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in reading file %s", data_handler->file_name);
                if (byte_stream)
                {
                    AXIS2_FREE(env->allocator, byte_stream);
                }
                if (read_stream)
                {
                    AXIS2_FREE(env->allocator, read_stream);
                }
                fclose(f);
                return AXIS2_FAILURE;
            }

            /* copy the read bytes */
            if (count > 0)
            {
                if (byte_stream)
                {
                    temp_byte_stream = byte_stream;
                    temp_byte_stream_size = byte_stream_size;
                    byte_stream_size = temp_byte_stream_size + count;
                    byte_stream = AXIS2_MALLOC(env->allocator,
                        (byte_stream_size) * sizeof(axis2_byte_t));
                    if (!byte_stream)
                    {
                        AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "No memory. Cannot create binary stream");
                        if (read_stream)
                        {
                            AXIS2_FREE(env->allocator, read_stream);
                        }
                        if (temp_byte_stream)
                        {
                            AXIS2_FREE(env->allocator, temp_byte_stream);
                        }
                        fclose(f);
                        return AXIS2_FAILURE;
                    }

                    memcpy(byte_stream,
                           temp_byte_stream, temp_byte_stream_size);
                    memcpy(byte_stream + temp_byte_stream_size,
                           read_stream, count);

                    if (read_stream)
                    {
                        AXIS2_FREE(env->allocator, read_stream);
                        read_stream_size = 0;
                    }
                    if (temp_byte_stream)
                    {
                        AXIS2_FREE(env->allocator, temp_byte_stream);
                        temp_byte_stream = NULL;
                        temp_byte_stream_size = 0;
                    }
                }
                else
                {
                    byte_stream = read_stream;
                    byte_stream_size = read_stream_size;
                    read_stream = NULL;
                    read_stream_size = 0;
                }
            }
            else if (read_stream)
            {
                AXIS2_FREE(env->allocator, read_stream);
            }
        }
        while (!feof(f));

        fclose(f);
        data_handler->buffer = byte_stream;
        data_handler->buffer_len = byte_stream_size;
        *output_stream = byte_stream;
        *output_stream_size = byte_stream_size;
    }
    else
    {
        /* Data Handler File Name is missing */
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_set_binary_data(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
    axis2_byte_t *input_stream,
    int input_stream_len)
{
    data_handler->buffer = input_stream;
    data_handler->buffer_len = input_stream_len;
    return AXIS2_SUCCESS;
}

/* This function will write the data in the buffer 
 * to a file. When caching is being used this will 
 * not be called , because the parser it self cache 
 * the attachment while parsing */


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_write_to(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    if (data_handler->file_name)
    {
        FILE *f = NULL;
        int count = 0;

        f = fopen(data_handler->file_name, "wb");
        if (!f)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
               "Error opening file %s for writing", data_handler->file_name);
            return AXIS2_FAILURE;
        }

        count = (int)fwrite(data_handler->buffer, 1,
            data_handler->buffer_len, f);
        /* The count lies within the int range */

        if (ferror(f))
        {
            fclose(f);
            return AXIS2_FAILURE;
        }
        fflush(f);
        fclose(f);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_set_file_name(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
    axis2_char_t *file_name)
{
    if (data_handler->file_name)
    {
        AXIS2_FREE(env->allocator, data_handler->file_name);
        data_handler->file_name = NULL;
    }

    if (file_name)
    {
        data_handler->file_name = axutil_strdup(env, file_name);
        if (!(data_handler->file_name))
        {
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_data_handler_get_file_name(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    if (data_handler->file_name)
    {
        return data_handler->file_name;
    }
    else
    {
        return NULL;
    }
}


/* This method will add the data_handler binary data to the array_list.
 * If it is a buffer the part type is buffer. otherwise it is a file. In the
 * case of file the array_list have just the file name and the size. The content
 * is not loaded to the memory.
 */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_add_binary_data(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
    axutil_array_list_t *list)
    
{
    axiom_mime_part_t *binary_part = NULL;
    
    binary_part = axiom_mime_part_create(env);
    
    if(!binary_part)
    {
        return AXIS2_FAILURE;
    }    
    
    if (data_handler->data_handler_type == AXIOM_DATA_HANDLER_TYPE_BUFFER)
    {
        binary_part->part = (axis2_byte_t *)AXIS2_MALLOC(env->allocator, 
            (data_handler->buffer_len) * sizeof(axis2_byte_t));
        memcpy(binary_part->part, data_handler->buffer, data_handler->buffer_len);

        binary_part->part_size = data_handler->buffer_len;
        binary_part->type = AXIOM_MIME_PART_BUFFER;
    }

    /* In the case of file we first calculate the file size
     * and then add the file name */
    
    else if (data_handler->data_handler_type == AXIOM_DATA_HANDLER_TYPE_FILE
             && data_handler->file_name)
    {
        struct stat stat_p;

        if (stat(data_handler->file_name, &stat_p) == -1)
        {
	    return AXIS2_FAILURE;
        }
        else if (stat_p.st_size == 0)
        {
            return AXIS2_SUCCESS;
        }
        else
        {
            binary_part->file_name = (axis2_char_t *)axutil_strdup(env, data_handler->file_name);
            binary_part->part_size = stat_p.st_size;
            binary_part->type = AXIOM_MIME_PART_FILE;
        }    
    }
    /* In the case of Callback the user should specify the callback name in the
     * configuration file. We just set the correct type. Inside the transport 
     * it will load the callback and send the attachment appropriately */

    else if(data_handler->data_handler_type == AXIOM_DATA_HANDLER_TYPE_CALLBACK)
    {
        binary_part->type = AXIOM_MIME_PART_CALLBACK;
        binary_part->user_param = data_handler->user_param;
    }

    else
    {
        /* Data Handler File Name is missing */
        return AXIS2_FAILURE;
    }

    /* Finaly we add the binary details to the list */    

    axutil_array_list_add(list, env, binary_part);

    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_data_handler_get_mime_id(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->mime_id;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_data_handler_set_mime_id(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
	const axis2_char_t *mime_id)
{
	if(data_handler->mime_id)
	{
		AXIS2_FREE(env->allocator, data_handler->mime_id);
	}
	data_handler->mime_id = axutil_strdup(env, mime_id);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_data_handler_type_t AXIS2_CALL
axiom_data_handler_get_data_handler_type(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->data_handler_type;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_data_handler_set_data_handler_type(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
	axiom_data_handler_type_t data_handler_type)
{
	data_handler->data_handler_type = data_handler_type;
    return;
}

AXIS2_EXTERN void *AXIS2_CALL
axiom_data_handler_get_user_param(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env)
{
    return data_handler->user_param;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_data_handler_set_user_param(
    axiom_data_handler_t *data_handler,
    const axutil_env_t *env,
	void *user_param)
{
	data_handler->user_param = user_param;
    return;
}

