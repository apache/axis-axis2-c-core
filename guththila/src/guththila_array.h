/* Copyright 2000-2005 The Apache Software Foundation or its licensors, as
 * applicable.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */  
    
#ifndef GUTHTHILA_ARRAY_H
#define GUTHTHILA_ARRAY_H
    
/**
 * @file guththila_array.h
 * @brief GUTHTHILA  array and table 
 */ 
    
#include "guththila_defines.h"
#include "guththila_environment.h"
#include "guththila_defines.h"
    
#include <stdarg.h>     /* for va_list */
    
#ifdef __cplusplus
extern "C"
{
    
#endif  /* __cplusplus */
    
/**
 * @defgroup guththila_array (stack) functions
 * @ingroup GUTHTHILA 
 * Tables are used to store entirely opaque structures 
 * for applications, while Arrays are usually used to
 * deal with string lists.
 * @{
 */ 
    
/** @see guththila_array_header_t */ 
    typedef struct guththila_array_header_t guththila_array_header_t;
     
/** An opaque array type */ 
        struct guththila_array_header_t
    {
        
    /** The environment pointer of guththila */ 
        guththila_environment_t * environment;
        
    /** The amount of memory allocated for each element of the array */ 
        int elt_size;
         
    /** The number of active elements in the array */ 
        int nelts;
         
    /** The number of elements allocated in the array */ 
        int nalloc;
         
    /** The elements in the array */ 
          guththila_char_t * elts;
     };
     
/**
 * Determine if the array is empty
 * @param a The array to check
 * @return True if empty, False otherwise
 */ 
     
        GUTHTHILA_DECLARE (int) 
        guththila_is_empty_array (const guththila_array_header_t * a);
     
/**
 * Create an array
 * @param environment guththila_environment pointer
 * @param nelts the number of elements in the initial array
 * @param elt_size The size of each element in the array.
 * @return The new array
 */ 
     
        GUTHTHILA_DECLARE (guththila_array_header_t *)
        guththila_array_make (guththila_environment_t * environment,
                              int nelts, int elt_size);
     
/**
 * Add a new element to an array (as a first-in, last-out stack)
 * @param arr The array to add an element to.
 * @return Location for the new element in the array.
 * @remark If there are no free spots in the array, then this function will
 *         allocate new space for the new element.
 */ 
     
        GUTHTHILA_DECLARE (void *)
        guththila_array_push (guththila_array_header_t * arr);
     
/**
 * Remove an element from an array (as a first-in, last-out stack)
 * @param arr The array to remove an element from.
 * @return Location of the element in the array.
 * @remark If there are no elements in the array, NULL is returned.
 */ 
     
        GUTHTHILA_DECLARE (void *)
        guththila_array_pop (guththila_array_header_t * arr);
     
/**
 * Concatenate two arrays together
 * @param dst The destination array, and the one to go first in the combined 
 *            array
 * @param src The source array to add to the destination array
 */ 
     
        GUTHTHILA_DECLARE (void) 
        guththila_array_cat (guththila_array_header_t *dst,
                             const guththila_array_header_t * src);
     
/**
 * Copy the entire array
 * @param environment guththila_environment
 * @param arr The array to copy
 * @return An exact copy of the array passed in
 * @remark The alternate guththila_array_copy_hdr copies only the header, and arranges 
 *         for the elements to be copied if (and only if) the code subsequently
 *         does a push or arraycat.
 */ 
     
        GUTHTHILA_DECLARE (guththila_array_header_t *)
        guththila_array_copy (guththila_environment_t * environment,
                              const guththila_array_header_t * arr);
     
/**
 * Copy the headers of the array, and arrange for the elements to be copied if
 * and only if the code subsequently does a push or arraycat.
 * @param environment guththila_environment
 * @param arr The array to copy
 * @return An exact copy of the array passed in
 * @remark The alternate guththila_array_copy copies the *entire* array.
 */ 
     
        GUTHTHILA_DECLARE (guththila_array_header_t *)
        guththila_array_copy_hdr (guththila_environment_t * environment,
                                  const guththila_array_header_t * arr);
     
/**
 * Append one array to the end of another, creating a new array in the process.
 * @param environment guththila_environment
 * @param first The array to put first in the new array.
 * @param second The array to put second in the new array.
 * @return A new array containing the data from the two arrays passed in.
*/ 
             
        GUTHTHILA_DECLARE (guththila_array_header_t *)
        guththila_array_append (guththila_environment_t * environment,
                                const guththila_array_header_t * first,
                                const guththila_array_header_t * second);
     
/**
 * Generates a new string containing the concatenated 
 * sequence of substrings referenced as elements within the array.  The string 
 * will be empty if all substrings are empty or null, or if there are no 
 * elements in the array.  If sep is non-NUL, it will be inserted between 
 * elements as a separator.
 * @param environment guththila_environment
 * @param arr The array to generate the string from
 * @param sep The separator to use
 * @return A string containing all of the data in the array.
 */ 
     
        GUTHTHILA_DECLARE (guththila_char_t *)
        guththila_array_pstrcat (guththila_environment_t * p,
                                 const guththila_array_header_t * arr,
                                 const guththila_char_t sep);
                                             GUTHTHILA_DECLARE (guththila_status_t)
        guththila_array_free (guththila_environment_t * environment,
                              guththila_array_header_t * header);
     
/** @} */ 
     
#ifdef __cplusplus
} 
#endif  /*  */
 
#endif  /* ! GUTHTHILA__ARRAY_H */
