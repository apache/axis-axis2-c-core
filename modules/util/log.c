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

#include <axis2_log_default.h>
#include <stdio.h>

axis2_status_t AXIS2_CALL
axis2_log_impl_free (axis2_log_t *log)
{
    if (NULL != log && NULL != log->ops)
    {
        free (log->ops);
    }
    if (NULL != log)
    {
        free (log); 
    }
    return 0;
}

axis2_status_t AXIS2_CALL axis2_log_impl_write (struct axis2_log *log, const axis2_char_t *buffer, axis2_log_levels_t level);

AXIS2_DECLARE(axis2_log_t *)
axis2_log_create (axis2_allocator_t * allocator, axis2_log_ops_t * ops)
{
    axis2_log_t *log;
    if (!allocator)
        return NULL;

    log = (axis2_log_t *) AXIS2_MALLOC (allocator, sizeof (axis2_log_t));

    if (!log)
        return NULL;

    if (ops)
        log->ops = ops;
    else
    {
        log->ops =
            (axis2_log_ops_t *) AXIS2_MALLOC (allocator,
                                              sizeof (axis2_log_ops_t));

        if (!log->ops)
        {
            AXIS2_FREE (allocator, log);
            return NULL;
        }
        log->ops->free = axis2_log_impl_free;
        log->ops->write = axis2_log_impl_write;
    }

    return log;
}

axis2_status_t AXIS2_CALL
axis2_log_impl_write (struct axis2_log *log, const axis2_char_t *buffer, axis2_log_levels_t level)
{
    if (!log || !buffer)
        return -1;
    if (level <= log->level)
    {
        char *level_str = "";
        switch (level)
        {
            case AXIS2_LOG_LEVEL_CRITICAL:
                level_str = "[critical] ";
                break;
            case AXIS2_LOG_LEVEL_ERROR:
                level_str = "[error] ";
                break;
            case AXIS2_LOG_LEVEL_WARNING:
                level_str = "[warning] ";
                break;
            case AXIS2_LOG_LEVEL_INFO:
                level_str = "[info] ";
                break;
            case AXIS2_LOG_LEVEL_DEBUG:
                level_str = "[debug] ";
                break;
        }
        fprintf (stderr, "%s", level_str);
        fprintf (stderr, "%s\n", buffer);
    }
    
    return 0;
}

