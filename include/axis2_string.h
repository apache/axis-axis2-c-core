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

#ifndef AXIS2_STRING_H
#define AXIS2_STRING_H

#include <axis2_defines.h>
#include <axis2_allocator.h>

typedef struct axis2_string
{
    void* (*axis2_string_strdup)(const void *ptr);
    int (*axis2_string_strcmp)(const axis2_char_t *s1, const axis2_char_t *s2);
}axis2_string_t;

/**
*   if the parsed string is null a default string is created
*   otherwise the parsed string is returned. If there isn't enough 
*   memory for allocation NULL is returned.
*   @param string user defined allcator
*/

axis2_string_t *
    axis2_string_create(axis2_allocator_t *allocator, axis2_string_t *string);
    
#define axis2_strdup(string, ptr) ((string)->axis2_string_strdup(ptr))
#define axis2_strcmp(string, s1, s2) ((string)->axis2_string_strcmp(s1, s2))

#endif /* AXIS2_STRING_H */
