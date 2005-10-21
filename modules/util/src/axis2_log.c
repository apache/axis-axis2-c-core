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

#include <axis2_log.h>
#include <stdio.h>

axis2_status_t axis2_log_impl_write (const void *buffer, size_t count);

axis2_log_t *
axis2_log_create (axis2_allocator_t * allocator, axis2_log_ops_t * operations)
{
    axis2_log_t *log;
    if (!allocator)
        return NULL;

    log = (axis2_log_t *) axis2_malloc (allocator, sizeof (axis2_log_t));

    if (!log)
        return NULL;

    if (operations)
        log->ops = operations;
    else
    {
        log->ops =
            (axis2_log_ops_t *) axis2_malloc (allocator,
                                              sizeof (axis2_log_ops_t));

        if (!log->ops)
        {
            axis2_free (allocator, log);
            return NULL;
        }

        log->ops->axis2_log_ops_write = axis2_log_impl_write;
    }

    return log;
}

axis2_status_t
axis2_log_impl_write (const void *buffer, size_t count)
{
    int i;
    if (!buffer)
        return -1;

    i = 0;
    for (i = 0; i < count; i++)
        fprintf (stderr, "%c", ((axis2_char_t *) buffer)[i]);
    printf ("\n");
    return 0;
}
