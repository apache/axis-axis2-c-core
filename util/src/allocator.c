/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axutil_allocator.h>
#include <axutil_utils.h>
#include <stdlib.h>
#include <string.h>

void *AXIS2_CALL
axutil_allocator_malloc_impl(
    axutil_allocator_t * allocator,
    size_t size)
{
    if (allocator)
        return malloc(size);
    return NULL;
}

void *AXIS2_CALL
axutil_allocator_realloc_impl(
    axutil_allocator_t * allocator,
    void *ptr,
    size_t size)
{
    if (allocator)
        return realloc(ptr, size);
    return NULL;
}

void AXIS2_CALL
axutil_allocator_free_impl(
    axutil_allocator_t * allocator,
    void *ptr)
{
    if (allocator && ptr)
        free(ptr);
}

AXIS2_EXTERN axutil_allocator_t *AXIS2_CALL
axutil_allocator_init(
    axutil_allocator_t * allocator)
{
    if (!allocator)
    {
        allocator = (axutil_allocator_t *)malloc(sizeof(axutil_allocator_t));
        if (allocator)
        {
            memset(allocator, 0, sizeof(axutil_allocator_t));

            allocator->malloc_fn = axutil_allocator_malloc_impl;
            allocator->realloc = axutil_allocator_realloc_impl;
            allocator->free_fn = axutil_allocator_free_impl;
            allocator->global_pool_ref = 0;
        }
    }

    return allocator;
}

AXIS2_EXTERN axutil_allocator_t *AXIS2_CALL
axutil_allocator_clone(
    axutil_allocator_t * allocator)
{
    axutil_allocator_t* clone = NULL;

    if (allocator)
    {
        clone = (axutil_allocator_t *)malloc(sizeof(axutil_allocator_t));
        if (clone)
        {
            memset(clone, 0, sizeof(axutil_allocator_t));

            clone->malloc_fn = allocator->malloc_fn;
            clone->realloc = allocator->realloc;
            clone->free_fn = allocator->free_fn;
            clone->current_pool = allocator->current_pool;
            clone->global_pool = allocator->global_pool;
            clone->local_pool = allocator->local_pool;
            clone->global_pool_ref = 0;
        }
    }
    return clone;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_allocator_free(
    axutil_allocator_t * allocator)
{
    if (allocator)
        allocator->free_fn(allocator, allocator);

    return;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_allocator_switch_to_global_pool(
    axutil_allocator_t * allocator)
{
    if (allocator)
    {
        allocator->global_pool_ref++;
        allocator->current_pool = allocator->global_pool;
    }
}

AXIS2_EXTERN void AXIS2_CALL
axutil_allocator_switch_to_local_pool(
    axutil_allocator_t * allocator)
{
    if (allocator)
    {
        if (allocator->global_pool_ref > 0)
            allocator->global_pool_ref--;

        if (allocator->global_pool_ref == 0)
            allocator->current_pool = allocator->local_pool;
    }
}

