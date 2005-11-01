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

/**
 * @file axis2_allocator.h
 * @brief Axis2 memory allocator interface
 */

#include <axis2_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_allocator Memory Allocator
 * @ingroup axis2_util 
 * @{
 */
 
 

/** 
  * \brief Axis2 memory allocator
  *
  * Encapsulator for memory allocating routines
  */
   AXIS2_DECLARE_DATA typedef struct axis2_allocator
    {
    

      /**
        * allocates memory
        * @param size size of the memory block to be allocated
        * @return pointer to the allocated memory block
        */
         void * (*malloc) (size_t size);
      /**
        * re-llocates memory
        * @param ptr memory block who's size to be changed
        * @param size size of the memory block to be allocated
        * @return pointer to the allocated memory block
        */
         void * (*realloc) (void *ptr, size_t size);
      /**
        * frees memory
        * @param ptr pointer to be freed
        */
         void (*free) (void *ptr);
    } axis2_allocator_t;

  /**
    * Initializes (creates) an allocator.
    * @param allocator user defined allcator. Optional, can be NULL. If NULL, a default allocator will be returned.
    * @return initialized allocator. NULL on error.
    */
    AXIS2_DECLARE(axis2_allocator_t *) axis2_allocator_init (axis2_allocator_t * allocator);

#define AXIS2_MALLOC(allocator, size) ((allocator)->malloc(size))
#define AXIS2_REALLOC(allocator, ptr, size) ((allocator)->realloc(ptr, size))
#define AXIS2_FREE(allocator, ptr) ((allocator)->free(ptr))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ALLOCATOR_H */
