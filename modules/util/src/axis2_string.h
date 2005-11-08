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
#include <axis2_error.h>
#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_string String
 * @ingroup axis2_util 
 * @{
 */

void* 
axis2_strdup (const void *ptr);

int 
axis2_strcmp (const axis2_char_t * s1, const axis2_char_t * s2);

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STRING_H */
