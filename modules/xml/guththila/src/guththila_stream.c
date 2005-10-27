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

#include <guththila_stream.h>
#include <stdio.h>

int GUTHTHILA_CALL guththila_stream_ops_read (void *buffer, size_t count);
int GUTHTHILA_CALL guththila_stream_ops_write(const void *buffer, size_t count);

GUTHTHILA_DECLARE(guththila_stream_t*) guththila_stream_create(guththila_allocator_t* allocator, guththila_stream_ops_t* operations)
{
    guththila_stream_t *stream;
    if (!allocator)
        return NULL;
    stream = (guththila_stream_t*)guththila_malloc(allocator, sizeof(guththila_stream_t));

    if (!stream)
        return NULL;
    
    if (operations)
        stream->ops = operations;
    else
    {
        stream->ops = (guththila_stream_ops_t*)guththila_malloc(allocator, sizeof(guththila_stream_ops_t));

        if (!stream->ops)
        {
            guththila_free(allocator, stream);
            return NULL;
        }
        
        stream->ops->guththila_stream_read = guththila_stream_ops_read;
        stream->ops->guththila_stream_write = guththila_stream_ops_write;
    }
    
    return stream;
}

int GUTHTHILA_CALL guththila_stream_ops_read (void *buffer, size_t count)
{
    int i;
    if (!buffer)
        return -1;

    i = 0;
    for(i = 0; i < count -1; i++ )
    {
        ((guththila_char_t*)buffer)[i] = 'a';
    }
    ((guththila_char_t*)buffer)[i] = '\0';
    return 0;
}

int GUTHTHILA_CALL guththila_stream_ops_write(const void *buffer, size_t count)
{
    int i ;
    if (!buffer)
        return -1;
    
    i =0;
    for(i = 0; i < count; i++)
        printf("%c", ((guththila_char_t*)buffer)[i]);
    return 0;
}
