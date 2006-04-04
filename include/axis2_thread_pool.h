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

#ifndef AXIS2_THREAD_POOL_H
#define AXIS2_THREAD_POOL_H

/**
 * @file axis2_thread_pool.h
 * @brief Axis2 thread pool interface
 */

#include <axis2_defines.h>
#include <axis2_allocator.h>
#include <axis2_thread.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_thread_pool Thread Pool
 * @ingroup axis2_util 
 * @{
 */
 
typedef struct axis2_thread_pool_ops axis2_thread_pool_ops_t;
typedef struct axis2_thread_pool axis2_thread_pool_t;
struct axis2_env;

/** 
  * \brief Axis2 thread_pool
  *
  * Encapsulator for thread pooling routines
  */
AXIS2_DECLARE_DATA struct axis2_thread_pool_ops
{
  /**
	* Retrives a thread from the thread pool
	* @param func function to be executed in the new thread
	* @param data arguments to be passed to the function
	* @return pointer to a thread in ready state.
	*/
	axis2_thread_t * (AXIS2_CALL *
	get_thread) (axis2_thread_pool_t *pool, 
				 axis2_thread_start_t func, 
	              void *data);
  /**
	* Blocks until the desired thread stops executing.
	* @param thd The thread to joined
	* @return status of the operation
	*/
	axis2_status_t (AXIS2_CALL *
	join_thread) (axis2_thread_pool_t *pool, 
				  axis2_thread_t *thd);
  /**
	* Stop the execution of current thread
	* @param thd thread to be stopped
	* @return status of the operation
	*/
	axis2_status_t (AXIS2_CALL *
	exit_thread) (axis2_thread_pool_t *pool, 
				  axis2_thread_t *thd);
  /**
	* Detaches a thread
	* @param thd thread to be detached
	* @return status of the operation
	*/
	axis2_status_t (AXIS2_CALL *
	thread_detach) (axis2_thread_pool_t *pool, 
					axis2_thread_t *thd);
  /**
	* Frees resources used by thread_pool
	* @param pool thread_pool to be freed
	*/
	axis2_status_t (AXIS2_CALL *
	free) (axis2_thread_pool_t *pool);
};

/**
 * @brief Thread Pool struct
 * Axis2 Thread Pool
 */
AXIS2_DECLARE_DATA struct axis2_thread_pool
{
	axis2_thread_pool_ops_t *ops;
};
/**
* Initializes (creates) an thread_pool.
* @param allocator user defined allocator for the memory allocation.
* @return initialized thread_pool. NULL on error.
*/
AXIS2_DECLARE(axis2_thread_pool_t *) axis2_thread_pool_init(
					axis2_allocator_t *allocator);

/**
 * This function can be used to initialize the environment in case of 
 * spawning a new thread via a thread function
 */
AXIS2_DECLARE (struct axis2_env *)
axis2_init_thread_env(struct axis2_env **system_env);

#define AXIS2_THREAD_POOL_GET_THREAD(thread_pool, func, data) \
		((thread_pool)->ops->get_thread(thread_pool, func, data))

#define AXIS2_THREAD_POOL_JOIN_THREAD(thread_pool, thd) \
		((thread_pool)->ops->join_thread(thread_pool, thd))

#define AXIS2_THREAD_POOL_EXIT_THREAD(thread_pool, thd) \
		((thread_pool)->ops->exit_thread(thread_pool, thd))

#define AXIS2_THREAD_POOL_THREAD_DETACH(thread_pool, thd) \
		((thread_pool)->ops->thread_detach(thread_pool, thd))

#define AXIS2_THREAD_POOL_FREE(thread_pool) \
		((thread_pool)->ops->free(thread_pool))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_THREAD_POOL_H */
