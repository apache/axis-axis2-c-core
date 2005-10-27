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

#ifndef GUTHTHILA_LOG_H
#define GUTHTHILA_LOG_H

#include <guththila_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum guththila_log_levels
    {
        GUTHTHILA_LOG_DEBUG = 0,
        GUTHTHILA_LOG_INFO,
        GUTHTHILA_LOG_WARNING,
        GUTHTHILA_LOG_ERROR,
        GUTHTHILA_LOG_CRITICAL
    } guththila_log_levels_t;

    struct guththila_log;
    struct guththila_log_ops;

    GUTHTHILA_DECLARE_DATA typedef struct guththila_log_ops
    {
        int (GUTHTHILA_CALL * guththila_log_ops_write) (const void *buffer,
                                                        size_t count);
    } guththila_log_ops_t;

    typedef struct guththila_log
    {
        struct guththila_log_ops *ops;
        guththila_log_levels_t level;
        int enabled;            /*boolean */
    } guththila_log_t;

      GUTHTHILA_DECLARE (guththila_log_t *)
        guththila_log_create (guththila_allocator_t * allocator,
                              guththila_log_ops_t * operations);

#define guththila_log_write(log, buffer, count) ((log)->ops->guththila_log_ops_write(buffer, count))

#ifdef __cplusplus
}
#endif

#endif                          /* GUTHTHILA_LOG_H */
