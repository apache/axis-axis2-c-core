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

#include <axis2_stream.h>
#include <stdio.h>

int axis2_stream_ops_read (void *buffer, size_t count);
int axis2_stream_ops_write(const void *buffer, size_t count);

axis2_stream_t *axis2_stream_create(axis2_allocator_t* allocator, axis2_stream_ops_t* operations)
{
    if (!allocator)
        return NULL;
    axis2_stream_t *stream = (axis2_stream_t*)axis2_malloc(allocator, sizeof(axis2_stream_t));

    if (!stream)
        return NULL;
    
    if (operations)
        stream->ops = operations;
    else
    {
        stream->ops = (axis2_stream_ops_t*)axis2_malloc(allocator, sizeof(axis2_stream_ops_t));

        if (!stream->ops)
            return NULL;
        
        stream->ops->read = axis2_stream_ops_read;
        stream->ops->write = axis2_stream_ops_write;
    }
    
    return stream;
}

int axis2_stream_ops_read (void *buffer, size_t count)
{
    int i = 0;
    for(i = 0; i < count -1; i++ )
    {
        ((axis2_char*)buffer)[i] = 'a';
    }
    ((axis2_char*)buffer)[i] = '\0';
    return 0;
}

int axis2_stream_ops_write(const void *buffer, size_t count)
{
    int i =0;
    for(i = 0; i < count; i++)
        printf("%c", ((axis2_char*)buffer)[i]);
    return 0;
}

