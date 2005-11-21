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

#include <guththila_environment.h>
#include <stdlib.h>

GUTHTHILA_DECLARE (guththila_environment_t *)
guththila_environment_create (guththila_allocator_t * allocator,
                              guththila_error_t * error,
                              guththila_log_t * log)
{
    guththila_environment_t *environment;
    if (!allocator)
        return NULL;

    environment = (guththila_environment_t *) GUTHTHILA_MALLOC (allocator,
                                          sizeof(guththila_environment_t));

    if (!environment)
        return NULL;

    environment->allocator = allocator;

    if (!error)
        environment->error = guththila_error_create (allocator);
    else
        environment->error = error;

    if (!log)
        environment->log = guththila_log_create (allocator, NULL);
    else
        environment->log = log;
   return environment;
}

GUTHTHILA_DECLARE(guththila_status_t)
guththila_environment_free(guththila_environment_t *environment)
{
    if(!environment) return GUTHTHILA_FAILURE;
    if(environment->allocator)
        free(environment->allocator);
    if(environment->error)
        GUTHTHILA_ERROR_FREE(environment->error);
    if(environment->log)
        GUTHTHILA_LOG_FREE(environment->log);
    free(environment);    
    return GUTHTHILA_SUCCESS;
}
