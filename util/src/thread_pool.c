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

#include <axis2_thread_pool.h>
#include <axis2_env.h>
#include <axis2_error_default.h>

/** 
 * @brief Stream struct impl
 *	Axis2 Stream impl  
 */
typedef struct axis2_thread_pool_impl axis2_thread_pool_impl_t;  
  
struct axis2_thread_pool_impl
{
	axis2_thread_pool_t thread_pool;
	axis2_allocator_t *allocator;
};

#define AXIS2_INTF_TO_IMPL(thread_pool) \
						((axis2_thread_pool_impl_t *)(thread_pool))

/********************************Function headers******************************/
axis2_status_t AXIS2_CALL 
axis2_thread_pool_free (axis2_thread_pool_t *pool);

axis2_thread_t* AXIS2_CALL
axis2_thread_pool_get_thread (axis2_thread_pool_t *pool, 
						axis2_thread_start_t func, void *data);

axis2_status_t AXIS2_CALL 
axis2_thread_pool_join_thread(axis2_thread_pool_t *pool, 
						axis2_thread_t *thd);

axis2_status_t AXIS2_CALL 
axis2_thread_pool_exit_thread (axis2_thread_pool_t *pool, 
						axis2_thread_t *thd);

axis2_status_t AXIS2_CALL 
axis2_thread_pool_thread_detach (axis2_thread_pool_t *pool, 
						axis2_thread_t *thd);
						
/************************* End of function headers ****************************/
AXIS2_EXTERN axis2_thread_pool_t * AXIS2_CALL 
axis2_thread_pool_init(axis2_allocator_t *allocator)
{
    axis2_thread_pool_impl_t *pool_impl = NULL;
	    	
	pool_impl = (axis2_thread_pool_impl_t *)AXIS2_MALLOC(allocator, 
						sizeof(axis2_thread_pool_impl_t));
	
	if(NULL == pool_impl)
	{
        return NULL;
	}
	pool_impl->allocator = allocator;
	pool_impl->thread_pool.ops = (axis2_thread_pool_ops_t *) AXIS2_MALLOC (
						allocator, sizeof (axis2_thread_pool_ops_t));
	if (NULL == pool_impl->thread_pool.ops)
	{
		axis2_thread_pool_free(&(pool_impl->thread_pool));
		return NULL;
	}
	pool_impl->thread_pool.ops->get_thread = axis2_thread_pool_get_thread;
	pool_impl->thread_pool.ops->join_thread = axis2_thread_pool_join_thread;
	pool_impl->thread_pool.ops->exit_thread = axis2_thread_pool_exit_thread;
	pool_impl->thread_pool.ops->thread_detach = axis2_thread_pool_thread_detach;
	pool_impl->thread_pool.ops->free = axis2_thread_pool_free;
	return &(pool_impl->thread_pool);
}


axis2_status_t AXIS2_CALL
axis2_thread_pool_free (axis2_thread_pool_t *pool)
{
    axis2_thread_pool_impl_t *pool_impl = AXIS2_INTF_TO_IMPL(pool);
	if(NULL == pool)
	{
		return AXIS2_FAILURE;
	}
	if(NULL == pool_impl->allocator)
	{
		return AXIS2_FAILURE;
	}
	pool_impl = AXIS2_INTF_TO_IMPL(pool);
	if (NULL != pool_impl->thread_pool.ops)
    {
        AXIS2_FREE (pool_impl->allocator, pool_impl->thread_pool.ops);
    }
   	AXIS2_FREE(pool_impl->allocator, pool_impl);
    return AXIS2_SUCCESS;
}

axis2_thread_t* AXIS2_CALL
axis2_thread_pool_get_thread (axis2_thread_pool_t *pool, 
						axis2_thread_start_t func, void *data)
{
	axis2_thread_pool_impl_t *pool_impl = AXIS2_INTF_TO_IMPL(pool);
	if(NULL == pool)
	{
		return NULL;
	}
	if(NULL == pool_impl->allocator)
	{
		return NULL;
	}
	return axis2_thread_create(pool_impl->allocator, NULL, func, data);
}


axis2_status_t AXIS2_CALL 
axis2_thread_pool_join_thread(axis2_thread_pool_t *pool, 
						axis2_thread_t *thd)
{
	if(NULL == pool || NULL == thd)
	{
		return AXIS2_FAILURE;
	}
	return axis2_thread_join(thd);
}

axis2_status_t AXIS2_CALL 
axis2_thread_pool_exit_thread (axis2_thread_pool_t *pool, 
						axis2_thread_t *thd)
{
    axis2_thread_pool_impl_t *pool_impl = AXIS2_INTF_TO_IMPL(pool);
	if(NULL == pool || NULL == thd)
	{
		return AXIS2_FAILURE;
	}
	return axis2_thread_exit(thd, pool_impl->allocator);
}


axis2_status_t AXIS2_CALL 
axis2_thread_pool_thread_detach (axis2_thread_pool_t *pool, 
						axis2_thread_t *thd)
{
	if(NULL == pool || NULL == thd)
	{
		return AXIS2_FAILURE;
	}
	return axis2_thread_detach(thd);
}

AXIS2_EXTERN axis2_env_t *AXIS2_CALL
axis2_init_thread_env(const axis2_env_t *system_env)
{
	axis2_error_t *error = axis2_error_create(system_env->allocator);
	return axis2_env_create_with_error_log_thread_pool(system_env->allocator, error, 
						system_env->log, system_env->thread_pool);
}


