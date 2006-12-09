/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 *
 */


/* #include "guththila_environment.h" */
#include <string.h>
#include "guththila_buffer.h"
#include <axis2_env.h>

AXIS2_EXTERN guththila_buffer_t * AXIS2_CALL
guththila_buffer_create(axis2_env_t * environment, int size)
{
    guththila_buffer_t *name;
    name = NULL;
    name  = AXIS2_MALLOC(environment->allocator,
            sizeof(guththila_buffer_t));
    if (name)
    {
        name->size = size;
        name->offset = 0;
        name->last = 0;
        name->next = 0;
        name->is_memory = 0;
        name->buff = NULL;
        if (size != 0)
            name->buff = (guththila_char_t *) AXIS2_MALLOC(
                        environment->allocator, size);
    }
    return name;
}

AXIS2_EXTERN guththila_buffer_t *
guththila_buffer_create_for_buffer(axis2_env_t * environment, char *buffer, int size)
{
    guththila_buffer_t *name;
    name = NULL;
    name  = AXIS2_MALLOC(environment->allocator,
            sizeof(guththila_buffer_t));
    if (name)
    {
        name->size = size;
        name->offset = 0;
        name->is_memory = 1;
        name->last = size;
        name->next = 0;
        name->buff = NULL;
        if (buffer)
            name->buff = buffer;
    }
    return name;
}

AXIS2_EXTERN void
guththila_buffer_free(axis2_env_t * environment,
        guththila_buffer_t * name)
{
    if (name)
    {
        if (name->buff && !name->is_memory)
        {
            AXIS2_FREE(environment->allocator, name->buff);
            name->buff = NULL;
        }
        AXIS2_FREE(environment->allocator, name);
        name = NULL;
    }
}


AXIS2_EXTERN guththila_buffer_t *
guththila_buffer_grow(axis2_env_t * environment,
        guththila_buffer_t * name)
{

    guththila_char_t *x = NULL;
    name->size <<= 1;
    x = (guththila_char_t *) AXIS2_MALLOC(environment->allocator, name->size);
    if (x)
	{
		memcpy (x, name->buff, strlen (name->buff));
        name->buff = x;
	}
    else
        name->size >>= 1;
    return name;
}
