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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <axis2_stream_default.h>

axis2_status_t AXIS2_CALL axis2_stream_impl_free (axis2_stream_t *stream);

axis2_status_t AXIS2_CALL axis2_stream_impl_write (const void *buffer, size_t count);

axis2_status_t AXIS2_CALL axis2_stream_impl_read (void *buffer, size_t count);

void* AXIS2_CALL axis2_stream_impl_file_open(const char *file_name, const char *options);
		
axis2_status_t AXIS2_CALL axis2_stream_impl_file_close(void *file_ptr);

axis2_char_t AXIS2_CALL axis2_stream_impl_file_get_char(void *file_ptr);

axis2_status_t AXIS2_CALL axis2_stream_impl_file_unget_char(const char chr, void *file_ptr);

AXIS2_DECLARE(axis2_stream_t *)
axis2_stream_create (axis2_allocator_t *allocator
                     , axis2_stream_t *stream)
{
	if(stream)
	{
		if(stream->ops)
		{
            if(!stream->ops->free)
                (stream->ops)->free = axis2_stream_impl_free;
			if(!stream->ops->read)
				stream->ops->read = axis2_stream_impl_read;
			if(!stream->ops->write)
				stream->ops->write = axis2_stream_impl_write;
			if(!stream->ops->file_open)
				stream->ops->file_open = axis2_stream_impl_file_open;
			if(!stream->ops->file_close)
				stream->ops->file_close = axis2_stream_impl_file_close;
			if(!stream->ops->file_get_char)
				stream->ops->file_get_char 
				= axis2_stream_impl_file_get_char;
			if(!stream->ops->file_unget_char)
				stream->ops->file_unget_char 
				= axis2_stream_impl_file_unget_char;
		}
		else if (allocator)
		{
			stream->ops =
            (axis2_stream_ops_t *) AXIS2_MALLOC (allocator,
                                                 sizeof (axis2_stream_ops_t));

			if (!stream->ops)
			{
				AXIS2_FREE (allocator, stream->ops);
				return NULL;
			}
            
            stream->ops->free = axis2_stream_impl_free;
			stream->ops->read = axis2_stream_impl_read;
			stream->ops->write = axis2_stream_impl_write;
			stream->ops->file_open = axis2_stream_impl_file_open;
			stream->ops->file_close = axis2_stream_impl_file_close;
			stream->ops->file_get_char 
				= axis2_stream_impl_file_get_char;
			stream->ops->file_unget_char 
				= axis2_stream_impl_file_unget_char;
		}
		else 
			return NULL;
		
		if(!stream->axis2_eof)
			stream->axis2_eof = EOF;
		
		return stream;
	}
	else if(allocator)
	{
		stream =
        (axis2_stream_t *) AXIS2_MALLOC (allocator, sizeof (axis2_stream_t));

		if (!stream)
			return NULL;
		stream->ops =
            (axis2_stream_ops_t *) AXIS2_MALLOC (allocator,
                                                 sizeof (axis2_stream_ops_t));

        if (!stream->ops)
        {
            AXIS2_FREE (allocator, stream);
            return NULL;
        }
        
        stream->ops->free = axis2_stream_impl_free;
		stream->ops->read = axis2_stream_impl_read;
        stream->ops->write = axis2_stream_impl_write;
		stream->ops->file_open = axis2_stream_impl_file_open;
		stream->ops->file_close = axis2_stream_impl_file_close;
		stream->ops->file_get_char 
			= axis2_stream_impl_file_get_char;
		stream->ops->file_unget_char 
			= axis2_stream_impl_file_unget_char;
		stream->axis2_eof = EOF;
		
		return stream;
	}
	
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_stream_impl_free (axis2_stream_t *stream)
{
    if (NULL != stream && NULL != stream->ops)
    {
        free (stream->ops);
    }
    if (NULL != stream)
    {
        free (stream); 
    }
    return 0;
}

axis2_status_t AXIS2_CALL
axis2_stream_impl_read (void *buffer, size_t count)
{
    int i;
    if (!buffer)
        return -1;

    i = 0;
    for (i = 0; i < count - 1; i++)
    {
        ((axis2_char_t *) buffer)[i] = 'a';
    }
    ((axis2_char_t *) buffer)[i] = '\0';
    return 0;
}

axis2_status_t AXIS2_CALL
axis2_stream_impl_write (const void *buffer, size_t count)
{
    int i;
    if (!buffer)
        return -1;

    i = 0;
    for (i = 0; i < count; i++)
        printf ("%c", ((axis2_char_t *) buffer)[i]);
		
    return 0;
}

void* AXIS2_CALL axis2_stream_impl_file_open(const char *file_name, const char *options)
{
    FILE *file_ptr;
    char *f_opt;
	char *f_name = (char*) strdup(file_name);
	if(!f_name) return NULL;
	    f_opt = (char*) strdup(options);
	if(!f_opt) return NULL;
	
	file_ptr = fopen (f_name, f_opt);
	return file_ptr;
}

axis2_status_t AXIS2_CALL axis2_stream_impl_file_close(void *file_ptr)
{
	if(!file_ptr) return -1;
	return (axis2_status_t) fclose(file_ptr);
}

axis2_char_t AXIS2_CALL axis2_stream_impl_file_get_char(void *file_ptr)
{
	if(!file_ptr) return -1;
	return (axis2_char_t) fgetc(file_ptr);
}

axis2_status_t AXIS2_CALL axis2_stream_impl_file_unget_char(const char chr, void *file_ptr)
{
	if(!file_ptr) return -1;
	return (axis2_status_t) ungetc(chr, file_ptr);
}
