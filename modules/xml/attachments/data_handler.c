/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *data_handler
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_data_handler.h>
#include <stdio.h>

typedef struct axis2_data_handler_impl
{
	axis2_data_handler_t data_handler;
	axis2_char_t* mime_type;
    axis2_char_t* file_name;
    axis2_byte_t* buffer;
    int buffer_len;
} axis2_data_handler_impl_t;



#define AXIS2_INTF_TO_IMPL(data_handler) ((axis2_data_handler_impl_t *)(data_handler))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_data_handler_free (axis2_data_handler_t *data_handler, axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_data_handler_get_content_type (axis2_data_handler_t *data_handler, axis2_env_t **env);

axis2_byte_t * AXIS2_CALL
axis2_data_handler_get_input_stream (axis2_data_handler_t *data_handler, axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_data_handler_read_from(axis2_data_handler_t *data_handler, axis2_env_t **env, 
                                axis2_byte_t** output_stream, int *output_stream_size);

axis2_status_t AXIS2_CALL
axis2_data_handler_set_binary_data(axis2_data_handler_t *data_handler, axis2_env_t **env, 
                                axis2_byte_t* input_stream, int input_stream_len);

axis2_status_t AXIS2_CALL
axis2_data_handler_write_to(axis2_data_handler_t *data_handler, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_data_handler_set_file_name(axis2_data_handler_t *data_handler, axis2_env_t **env, 
                                axis2_char_t* file_name);

/************************** End of Function headers ************************/

AXIS2_DECLARE(axis2_data_handler_t *)
axis2_data_handler_create (axis2_env_t **env, axis2_char_t *file_name, axis2_char_t *mime_type)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	data_handler_impl = (axis2_data_handler_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_data_handler_impl_t));
		
	if(NULL == data_handler_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    data_handler_impl->data_handler.ops = NULL;
    data_handler_impl->mime_type = NULL;
    data_handler_impl->file_name = NULL;
    data_handler_impl->buffer = NULL;
    data_handler_impl->buffer_len = 0;

    if (mime_type)
    {
        data_handler_impl->mime_type = AXIS2_STRDUP(mime_type, env);
        if (!(data_handler_impl->mime_type))
        {
            axis2_data_handler_free(&(data_handler_impl->data_handler), env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    if (file_name)
    {
        data_handler_impl->file_name = AXIS2_STRDUP(file_name, env);
        if (!(data_handler_impl->file_name))
        {
            axis2_data_handler_free(&(data_handler_impl->data_handler), env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    
	data_handler_impl->data_handler.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_data_handler_ops_t));
	if(NULL == data_handler_impl->data_handler.ops)
    {
        axis2_data_handler_free(&(data_handler_impl->data_handler), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	data_handler_impl->data_handler.ops->free =  axis2_data_handler_free;
    data_handler_impl->data_handler.ops->get_content_type = axis2_data_handler_get_content_type;
    data_handler_impl->data_handler.ops->get_input_stream = axis2_data_handler_get_input_stream;
    data_handler_impl->data_handler.ops->read_from = axis2_data_handler_read_from;
    data_handler_impl->data_handler.ops->write_to = axis2_data_handler_write_to;
    data_handler_impl->data_handler.ops->set_binary_data = axis2_data_handler_set_binary_data;
    data_handler_impl->data_handler.ops->set_file_name = axis2_data_handler_set_file_name;
	return &(data_handler_impl->data_handler);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_data_handler_free (axis2_data_handler_t *data_handler, axis2_env_t **env)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    data_handler_impl = AXIS2_INTF_TO_IMPL(data_handler);
    
    if(NULL != data_handler->ops)
    {
        AXIS2_FREE((*env)->allocator, data_handler->ops);
        data_handler->ops = NULL;
    }
    
    if(data_handler_impl)
    {
        AXIS2_FREE((*env)->allocator, data_handler_impl);
        data_handler_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_data_handler_get_content_type (axis2_data_handler_t *data_handler, axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);       
	return AXIS2_INTF_TO_IMPL(data_handler)->mime_type;
}

axis2_byte_t * AXIS2_CALL
axis2_data_handler_get_input_stream (axis2_data_handler_t *data_handler, axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);       
	
	/*@TODO implement the following*/
	return "";
}

axis2_status_t AXIS2_CALL
axis2_data_handler_read_from(axis2_data_handler_t *data_handler, axis2_env_t **env, 
							axis2_byte_t** output_stream, int *output_stream_size)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    data_handler_impl = AXIS2_INTF_TO_IMPL(data_handler);
    
    if (data_handler_impl->file_name)
    {
        FILE *f = NULL;
        axis2_byte_t *byte_stream = NULL;
        axis2_byte_t *temp_byte_stream = NULL;
        axis2_byte_t *read_stream = NULL;
        int byte_stream_size = 0;
        int temp_byte_stream_size = 0;
        int read_stream_size = 0;
        int count = 0;
        
        f = fopen(data_handler_impl->file_name, "rb");
        if (!f)
            return AXIS2_FAILURE;
        
        do {
            read_stream_size = 1024;
            read_stream = AXIS2_MALLOC((*env)->allocator, (read_stream_size) * sizeof(axis2_byte_t));
            if (!read_stream)
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                if (byte_stream)
                {
                    AXIS2_FREE((*env)->allocator, byte_stream);
                    byte_stream = NULL;
                }
                return AXIS2_FAILURE;
            }
            count = fread(read_stream, 1, 1024, f);
            if (ferror(f) != 0)
            {
                /*TODO : need to set the correct error code */
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                if (byte_stream)
                {
                    AXIS2_FREE((*env)->allocator, byte_stream);
                    byte_stream = NULL;
                }
                if (read_stream)
                {
                    AXIS2_FREE((*env)->allocator, read_stream);
                    read_stream = NULL;
                }
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
                    byte_stream = AXIS2_MALLOC((*env)->allocator, (byte_stream_size) * sizeof(axis2_byte_t));
                    if (!byte_stream)
                    {
                        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                        if (read_stream)
                        {
                            AXIS2_FREE((*env)->allocator, read_stream);
                            read_stream = NULL;
                        }
                        if (temp_byte_stream)
                        {
                            AXIS2_FREE((*env)->allocator, temp_byte_stream);
                            temp_byte_stream = NULL;
                        }
                        return AXIS2_FAILURE;
                    }
                    
                    memcpy(byte_stream, temp_byte_stream, temp_byte_stream_size);
                    memcpy(byte_stream + temp_byte_stream_size, read_stream, count);
                    
                    if (read_stream)
                    {
                        AXIS2_FREE((*env)->allocator, read_stream);
                        read_stream = NULL;
                        read_stream_size = 0;
                    }
                    if (temp_byte_stream)
                    {
                        AXIS2_FREE((*env)->allocator, temp_byte_stream);
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
        }while (!feof(f));
        
        fclose(f);
        
        *output_stream = byte_stream;
        *output_stream_size = byte_stream_size;
    }
	
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_data_handler_set_binary_data(axis2_data_handler_t *data_handler, axis2_env_t **env, 
                                axis2_byte_t* input_stream, int input_stream_len)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    data_handler_impl = AXIS2_INTF_TO_IMPL(data_handler);
    
    data_handler_impl->buffer = input_stream;
    data_handler_impl->buffer_len = input_stream_len;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_data_handler_write_to(axis2_data_handler_t *data_handler, axis2_env_t **env)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    data_handler_impl = AXIS2_INTF_TO_IMPL(data_handler);
    
    if (data_handler_impl->file_name)
    {
        FILE *f = NULL;
        axis2_byte_t *byte_stream = NULL;
        axis2_byte_t *temp_byte_stream = NULL;
        axis2_byte_t *read_stream = NULL;
        int byte_stream_size = 0;
        int temp_byte_stream_size = 0;
        int read_stream_size = 0;
        int count = 0;
        
        f = fopen(data_handler_impl->file_name, "wb");
        if (!f)
            return AXIS2_FAILURE;
            
        count = fwrite(data_handler_impl->buffer, 1, data_handler_impl->buffer_len, f);
        
        if (ferror(f) != 0)
        {
            /*TODO : need to set the correct error code */
            return AXIS2_FAILURE;
        }
        fclose(f);
    }

    return AXIS2_SUCCESS;        
}

axis2_status_t AXIS2_CALL
axis2_data_handler_set_file_name(axis2_data_handler_t *data_handler, axis2_env_t **env, 
                                axis2_char_t* file_name)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    data_handler_impl = AXIS2_INTF_TO_IMPL(data_handler);
    
    if(data_handler_impl->file_name)
    {
        AXIS2_FREE((*env)->allocator, data_handler_impl->file_name);
        data_handler_impl->file_name = NULL;
    }
    
    if (file_name)
    {
        data_handler_impl->file_name = AXIS2_STRDUP(file_name, env);
        if (!(data_handler_impl->file_name))
        {
            axis2_data_handler_free(&(data_handler_impl->data_handler), env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

