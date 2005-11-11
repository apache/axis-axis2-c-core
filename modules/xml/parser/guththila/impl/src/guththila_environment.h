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

#ifndef GUTHTHILA_ENVIRONMENT_H
#define GUTHTHILA_ENVIRONMENT_H

#include <guththila_allocator.h>
#include <guththila_error.h>
#include <guththila_stream.h>
#include <guththila_log.h>
#include "guththila_defines.h"
#include <guththila_string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct guththila_environment;
    struct guththila_environment_ops;

    typedef struct guththila_environment_ops
    {
        int test;
    } guththila_environment_ops_t;

    typedef struct guththila_environment
    {
        struct guththila_environment_ops *ops;
        guththila_allocator_t *allocator;   /* memory allocation routines */
        guththila_error_t *error;   /* error handling */
        guththila_stream_t *stream; /* IO routines */
        guththila_log_t *log;   /* logging routines */
        guththila_string_t *string; /* string routines */
    } guththila_environment_t;

      GUTHTHILA_DECLARE (guththila_environment_t *)
        guththila_environment_create (guththila_allocator_t * allocator,
                                      guththila_error_t * error,
                                      guththila_stream_t * stream,
                                      guththila_log_t * log,
                                      guththila_string_t * string);

#ifdef __cplusplus
}
#endif

#endif                          /* GUTHTHILA_ENVIRONMENT_H */
