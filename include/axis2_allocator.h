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

#ifndef AXIS2_ALLOCATOR_H
#define AXIS2_ALLOCATOR_H

#include <axis2_defines.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct axis2_allocator
{
    void *(*axis2_allocator_malloc)(size_t size);
    void *(*axis2_allocator_realloc)(void *ptr,size_t size);
    void (*axis2_allocator_free)(void *ptr);
}axis2_allocator_t;

/**
*   if the parsed allocator is null a default allocator is created
*   otherwise the parsed allocator is returned. If there isn't enough 
*   memory for allocation NULL is returned.
*   @param allocator user defined allcator
*/

axis2_allocator_t *
    axis2_allocator_init(axis2_allocator_t *allocator);
    
#define axis2_malloc(allocator, size) ((allocator)->axis2_allocator_malloc(size))
#define axis2_realloc(allocator, ptr, size) ((allocator)->axis2_allocator_realloc(ptr, size))
#define axis2_free(allocator, ptr) ((allocator)->axis2_allocator_free(ptr))

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_ALLOCATOR_H */
