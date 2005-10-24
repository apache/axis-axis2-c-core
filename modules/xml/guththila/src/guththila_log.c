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

#include <guththila_log.h>
#include <stdio.h>

int guththila_log_impl_write(const void *buffer, size_t count);

guththila_log_t *guththila_log_create(guththila_allocator_t* allocator, guththila_log_ops_t* operations)
{   
    guththila_log_t *log;
    if (!allocator)
        return NULL;

    log = (guththila_log_t*)guththila_malloc(allocator, sizeof(guththila_log_t));

    if (!log)
        return NULL;
    
    if (operations)
        log->ops = operations;
    else
    {
        log->ops = (guththila_log_ops_t*)guththila_malloc(allocator, sizeof(guththila_log_ops_t));

        if (!log->ops)
        {
            guththila_free(allocator, log);
            return NULL;
        }
        
        log->ops->guththila_log_ops_write = guththila_log_impl_write;
    }
    
    return log;
}

int guththila_log_impl_write(const void *buffer, size_t count)
{
    int i;
    if (!buffer)
        return -1;
    
    i =0;
    for(i = 0; i < count; i++)
        fprintf(stderr, "%c", ((guththila_char_t*)buffer)[i]);
    printf("\n");
    return 0;
}

