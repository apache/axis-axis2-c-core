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

#include <axis2_allocator.h>
#include <axis2_utils.h>
#include <stdlib.h>

void *AXIS2_CALL
axis2_allocator_malloc_impl(axis2_allocator_t *allocator, size_t size);

void *AXIS2_CALL
axis2_allocator_realloc_impl(axis2_allocator_t *allocator, void *ptr, size_t size);

void AXIS2_CALL
axis2_allocator_free_impl(axis2_allocator_t *allocator, void *ptr);


AXIS2_EXTERN axis2_allocator_t * AXIS2_CALL
axis2_allocator_init (axis2_allocator_t *allocator)
{
    if (allocator)
        return allocator;

    else
    {
        allocator = (axis2_allocator_t *) malloc (sizeof (axis2_allocator_t));
        if (allocator)
        {
            allocator->malloc = axis2_allocator_malloc_impl;
            allocator->realloc = axis2_allocator_realloc_impl;
            allocator->free = axis2_allocator_free_impl;
            return allocator;
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_allocator_free(axis2_allocator_t *allocator)
{
    if(allocator)
    {
        allocator->free(allocator, allocator);
    }
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_allocator_malloc_impl(axis2_allocator_t *allocator, size_t size)
{
    return malloc(size);
}

void *AXIS2_CALL
axis2_allocator_realloc_impl(axis2_allocator_t *allocator, void *ptr, size_t size)
{
    return realloc(ptr, size);
}

void AXIS2_CALL
axis2_allocator_free_impl(axis2_allocator_t *allocator, void *ptr)
{
    return free(ptr);
}
