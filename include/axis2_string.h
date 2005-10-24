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

/** 
  * \brief Axis2 string operations
  *
  * Encapsulator for string handling routines
  */
    typedef struct axis2_string
    {
      /**
        * duplicates the given string
        * @param ptr string to be duplicated
        * @return pointer to the duplicated string
        */
        AXIS2_DECLARE(void *) (*axis2_string_strdup) (const void *ptr);
      /**
        * compares the given two strings
        * @param s1 first string to be compared
        * @param s2 second string to be compared
        * @return 0 if the two strings are equal, else non zero
        */
        AXIS2_DECLARE(int) (*axis2_string_strcmp) (const axis2_char_t * s1,
                                    const axis2_char_t * s2);
    } axis2_string_t;

 /**
    * Creates) a string struct
    * @param allocator allocator to be used. Mandatory, cannot be NULL
    * @param string user defined string. Optional, can be NULL. If NULL, a default string will be returned.
    * @return pointer to newly created string. NULL on error.
    */
    AXIS2_DECLARE(axis2_string_t *) axis2_string_create (axis2_allocator_t * allocator,
                                         axis2_string_t * string);

#define axis2_strdup(string, ptr) ((string)->axis2_string_strdup(ptr))
#define axis2_strcmp(string, s1, s2) ((string)->axis2_string_strcmp(s1, s2))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STRING_H */
