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

#ifndef AXIS2_LOG_H
#define AXIS2_LOG_H

#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum axis2_log_levels
{
    AXIS2_LOG_DEBUG = 0,
    AXIS2_LOG_INFO,
    AXIS2_LOG_WARNING,
    AXIS2_LOG_ERROR,
    AXIS2_LOG_CRITICAL
} axis2_log_levels_t;

struct axis2_log;
struct axis2_log_ops;

typedef struct axis2_log_ops
{
    int (*write) (const void *buffer, size_t count);
} axis2_log_ops_t;

typedef struct axis2_log
{
    struct axis2_log_ops *ops;
    axis2_log_levels_t level;
    int enabled;                /*boolean */
} axis2_log_t;

axis2_log_t *axis2_log_create (axis2_allocator_t * allocator,
                               axis2_log_ops_t * operations);

#define axis2_log_write(log, buffer, count) ((log)->ops->write(buffer, count))

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_LOG_H */
