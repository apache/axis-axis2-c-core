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

#ifndef AXUTIL_ALLOCATOR_H
#define AXUTIL_ALLOCATOR_H

/**
 * @file axutil_allocator.h
 * @brief Axis2 memory allocator interface
 */

#include <axutil_utils_defines.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axutil_allocator allocator
     * @ingroup axis2_util
     * @{
     */

    /**
      * \brief Axis2 memory allocator
      *
      * Encapsulator for memory allocating routines
      */
    typedef struct axutil_allocator
    {

        /**
         * Function pointer representing the function that allocates memory.
         * @param allocator pointer to allocator struct. In the default 
         * implementation this is not used, however this parameter is useful 
         * when the allocator implementation is dealing with a memory pool.
         * @param size size of the memory block to be allocated
         * @return pointer to the allocated memory block
         */
        void *(
            AXIS2_CALL
            * malloc_fn)(
                struct axutil_allocator * allocator,
                size_t size);

        /**
         * Function pointer representing the function that re-allocates memory.
         * @param allocator pointer to allocator struct. In the default 
         * implementation this is not used, however this parameter is useful 
         * when the allocator implementation is dealing with a memory pool.
         * @param ptr memory block who's size to be changed
         * @param size size of the memory block to be allocated
         * @return pointer to the allocated memory block
         */
        void *(
            AXIS2_CALL
            * realloc)(
                struct axutil_allocator * allocator,
                void *ptr,
                size_t size);

        /**
         * Function pointer representing the function that frees memory.
         * @param allocator pointer to allocator struct. In the default 
         * implementation this is not used, however this parameter is useful 
         * when the allocator implementation is dealing with a memory pool.
         * @param ptr pointer to be freed
         * @return void
         */
        void(
            AXIS2_CALL
            * free_fn)(
                struct axutil_allocator * allocator,
                void *ptr);

        /** 
         * Local memory pool. Local pool is used to allocate per request 
         * local values. 
         */
        void *local_pool;

        /** 
         * Global memory pool. Global pool is used to allocate values that 
         * live beyond a request 
         */
        void *global_pool;

        /** 
         * Memory pool currently in use. The functions
         * axutil_allocator_switch_to_global_pool and
         * axutil_allocator_switch_to_local_pool should be used to 
         * set the current pool to global pool or to local pool respectively. 
         */
        void *current_pool;

    }
    axutil_allocator_t;

    /**
     * Initializes (creates) a memory allocator.
     * @param allocator user defined allocator. If NULL, a default allocator 
     * will be returned.
     * @return initialized allocator. NULL on error.
     */
    AXIS2_EXTERN axutil_allocator_t *AXIS2_CALL
    axutil_allocator_init(
        axutil_allocator_t * allocator);

    /**
     * This function should be used to deallocate memory if the default 
     * allocator was provided by the axutil_allocator_init() call.
     * @param allocator allocator struct to be freed
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_allocator_free(
        axutil_allocator_t * allocator);

    /**
     * Swaps the local_pool and global_pool and makes the global pool the 
     * current pool.
     * In case of using pools, local_pool is supposed to hold the pool out of which
     * local values are allocated. In case of values that live beyond a request 
     * global pool should be used, hence this method has to be called to switch to 
     * global pool for allocation. 
     * @param allocator allocator whose memory pools are to be switched
     * @return void
      */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_allocator_switch_to_global_pool(
        axutil_allocator_t * allocator);

    /**
     * Swaps the local_pool and global_pool and makes the local pool the 
     * current pool. 
     * In case of using pools, local_pool is supposed to hold the pool out of which
     * local values are allocated. In case of values that live beyond a request 
     * global pool should be used. This method can be used to inverse the switching 
     * done by axutil_allocator_switch_to_global_pool, to start using the local pool again.
     * @param allocator allocator whose memory pools are to be switched
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_allocator_switch_to_local_pool(
        axutil_allocator_t * allocator);

#define AXIS2_MALLOC(allocator, size) \
     ((allocator)->malloc_fn(allocator, size))

#define AXIS2_REALLOC(allocator, ptr, size) \
      ((allocator)->realloc(allocator, ptr, size))

#define AXIS2_FREE(allocator, ptr) \
    ((allocator)->free_fn(allocator, ptr))

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ALLOCATOR_H */
