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

#ifndef AXIS2_MEMORY_ALLOCATOR_H
#define AXIS2_MEMORY_ALLOCATOR_H

#include <stdlib.h>
#include <axis2_defines.h>

typedef struct 
{
    void *(*malloc)(size_t size);
    void *(*realloc)(void *ptr,size_t size);
    void (*free)(void *ptr);
}axis2_allocator_t;

/**
*   if the parsed allocator is null a default allocator is created
*   otherwise the parsed allocator is returned. If there isn't enough 
*   memory for allocation NULL is returned.
*   @param allocator user defined allcator
*/

axis2_allocator_t *
    axis2_allocator_init(axis2_allocator_t *allocator);
    
#define axis2_malloc(allocator, size) ((allocator)->malloc(size))
#define axis2_realloc(allocator, ptr, size) ((allocator)->realloc(ptr, size))
#define axis2_free(allocator, ptr) ((allocator)->free(ptr))

#endif /* AXIS2_MEMORY_ALLOCATOR_H */
