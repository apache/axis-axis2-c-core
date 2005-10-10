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

#ifndef AXIS2_ENVIRONMENT_H
#define AXIS2_ENVIRONMENT_H

#include <axis2_allocator.h>
#include <axis2_error.h>
#include <axis2_stream.h>
#include <axis2_log.h>

struct axis2_environment;
struct axis2_environment_ops;

typedef struct axis2_environment_ops {
} axis2_environment_ops_t;

typedef struct axis2_environment {
    struct axis2_environment_ops *ops;
    axis2_allocator_t *allocator;   /* memory allocation routines */
    axis2_error_t *error;           /* error handling */
    axis2_stream_t *stream;         /* IO routines */
    axis2_log_t *log;               /* logging routines */
} axis2_environment_t;

axis2_environment_t *axis2_environment_create(axis2_allocator_t *allocator, axis2_error_t *error, axis2_stream_t *stream, axis2_log_t *log);

#endif /* AXIS2_ENVIRONMENT_H */
