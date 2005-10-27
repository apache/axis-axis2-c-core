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

#include <axis2_environment.h>

AXIS2_DECLARE(axis2_environment_t *)
axis2_environment_create (axis2_allocator_t * allocator,
                          axis2_error_t * error, axis2_stream_t * stream,
                          axis2_log_t * log, axis2_string_t * string)
{
    axis2_environment_t *environment;
    if (!allocator)
        return NULL;

    environment =
        (axis2_environment_t *) axis2_malloc (allocator,
                                              sizeof (axis2_environment_t));

    if (!environment)
        return NULL;

    environment->allocator = allocator;

    if (!error)
        environment->error = axis2_error_create (allocator);
    else
        environment->error = error;

    environment->stream = axis2_stream_create (allocator, stream);    

    if (!log)
        environment->log = axis2_log_create (allocator, NULL);
    else
        environment->log = log;

    environment->string = axis2_string_create (allocator, string);

    return environment;
}
