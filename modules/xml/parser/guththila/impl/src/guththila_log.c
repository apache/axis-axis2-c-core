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
#include <guththila_defines.h>
#include <stdio.h>
#include <stdlib.h>

int GUTHTHILA_CALL guththila_log_impl_write (const void *buffer,
                                             size_t count);
                                             
int GUTHTHILA_CALL
guththila_log_free(guththila_log_t *log)
{
    if(!log) return 0;
    if(log->ops)
        free(log->ops);
    free(log);
    return 1;    
}                                             

GUTHTHILA_DECLARE (guththila_log_t *)
guththila_log_create (guththila_allocator_t * allocator,
                      guththila_log_ops_t * ops)
{
    guththila_log_t *log;
    if (!allocator)
        return NULL;

    log =
        (guththila_log_t *) GUTHTHILA_MALLOC (allocator,
                                              sizeof (guththila_log_t));

    if (!log)
        return NULL;

    if (ops)
        log->ops = ops;
    else
    {
        log->ops =
            (guththila_log_ops_t *) GUTHTHILA_MALLOC (allocator,
                                            sizeof(guththila_log_ops_t));

        if (!log->ops)
        {
            GUTHTHILA_FREE (allocator, log);
            return NULL;
        }

        log->ops->write = guththila_log_impl_write;
    }
    log->ops->free = guththila_log_free;

    return log;
}

int GUTHTHILA_CALL
guththila_log_impl_write (const void *buffer, size_t count)
{
    size_t i;
    if (!buffer)
        return -1;

    i = 0;
    for (i = 0; i < count; i++)
        fprintf (stderr, "%c", ((guththila_char_t *) buffer)[i]);
    printf ("\n");
    return 0;
}
