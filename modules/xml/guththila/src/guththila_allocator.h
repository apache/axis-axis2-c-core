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

#ifndef GUTHTHILA_ALLOCATOR_H
#define GUTHTHILA_ALLOCATOR_H

#include <guththila_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif


    GUTHTHILA_DECLARE_DATA typedef struct guththila_allocator
    {
        void *(*guththila_allocator_malloc) (size_t size);
        void *(*guththila_allocator_realloc) (void *ptr, size_t size);
        void (*guththila_allocator_free) (void *ptr);
        void *(*guththila_allocator_calloc) (size_t nelem, size_t elsize);
    } guththila_allocator_t;

/**
*   if the parsed allocator is null a default allocator is created
*   otherwise the parsed allocator is returned. If there isn't enough 
*   memory for allocation NULL is returned.
*   @param allocator user defined allcator
*/

      GUTHTHILA_DECLARE (guththila_allocator_t *)
        guththila_allocator_init (guththila_allocator_t * allocator);

#define guththila_malloc(allocator, size) ((allocator)->guththila_allocator_malloc(size))
#define guththila_realloc(allocator, ptr, size) ((allocator)->guththila_allocator_realloc(ptr, size))
#define guththila_free(allocator, ptr) ((allocator)->guththila_allocator_free(ptr))
#define guththila_calloc(allocator,size1,size2) ((allocator)->guththila_allocator_calloc((size1),(size2)));
#ifdef __cplusplus
}
#endif

#endif                          /* GUTHTHILA_ALLOCATOR_H */
