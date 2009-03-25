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

#include <axutil_thread_pool.h>
#include <axutil_env.h>
#include <axutil_error_default.h>
#include <axutil_pool_queue.h>
#include <stdio.h>

typedef enum 
{
    ALL_RUN, ALL_EXIT
} poolstate_t;

struct axutil_thread_pool
{
    axutil_allocator_t *allocator;
    /* Mutex to synchronize the read/write operations */
    axutil_thread_mutex_t *mutex;
    int poolsize;
    axutil_thread_cond_t *job_posted; /* dispatcher: "Hey guys, there's a job!" */
    axutil_thread_cond_t *job_taken; /* a worker: "Got it!" */
    poolstate_t state; /* Threads check this before getting job. */
    int arrsz; /* Number of entries in array. */
    int live; /* Number of live threads in pool (when pool is being destroyed, live<=arrsz) */
    axutil_pool_queue_head_t *queue; /* queue of work orders */
    struct timeval created; /* When the threadpool was created. */
    axutil_thread_t **array; /* threads themselves. */
};

/* Define the life of a working thread.*/

static void * axutil_thread_pool_do_work(
        axutil_thread_t *axis_thd,
        void * owning_pool) 
{
    /* Convert pointer to owning pool to proper type. */
    axutil_thread_pool_t *pool = (axutil_thread_pool_t *) owning_pool;
  
    /* Remember my creation sequence number */
    /*int myid = pool->live;*/

    /* When we get a posted job, we copy it into these local vars. */
    axutil_thread_start_t myjob;
    void *myarg;  
    axutil_thread_start_t mycleaner;
    void *mycleanarg;

    /*pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    pthread_cleanup_push(pthread_mutex_unlock, (void *) &pool->mutex);*/

    /* Grab mutex so we can begin waiting for a job */
    axutil_thread_mutex_lock(pool->mutex);
  
    /* Main loop: wait for job posting, do job(s) ... forever */
    for( ; ; ) 
    {
        while(axutil_pool_queue_is_job_available(pool->queue) == 0)
        {
	        axutil_thread_cond_wait(pool->job_posted, pool->mutex);
        }
  
        /* We've just woken up and we have the mutex.  Check pool's state */
        if (ALL_EXIT == pool->state)
        {
            break;
        }
    
        /* while we find work to do */
        axutil_pool_queue_get_work_order(pool->queue, &myjob, &myarg, &mycleaner, &mycleanarg);
        axutil_thread_cond_signal(pool->job_taken);
    
        /* Yield mutex so other jobs can be posted */
        axutil_thread_mutex_unlock(pool->mutex);
    
        /* Run the job we've taken */
        /*if(mycleaner)
        {
            pthread_cleanup_push(mycleaner,mycleanarg);
            myjob(axis_thd, myarg);
            pthread_cleanup_pop(1);
        }
        else*/
        {
	        myjob(axis_thd, myarg);
        }
    
        /* Grab mutex so we can grab posted job, or (if no job is posted) begin waiting for next 
         * posting. 
         */
        axutil_thread_mutex_lock(pool->mutex);
    }
  
    /* If we get here, we broke from loop because state is ALL_EXIT. */
    --pool->live;
  
    /* We're not really taking a job ... but this signals the destroyer that one thread has exited, 
     * so it can keep on destroying. pthread_cond_signal(&pool->job_taken);
     */
  
    axutil_thread_mutex_unlock(pool->mutex);

    /*pthread_cleanup_pop(1);*/
    return NULL;
}  


AXIS2_EXTERN axutil_thread_pool_t *AXIS2_CALL
axutil_thread_pool_init(
    axutil_allocator_t *allocator,
    int poolsize)
{
    axutil_thread_pool_t *pool = NULL;
    int i = 0;

    if ((poolsize <= 0) || (poolsize > MAXT_IN_POOL))
    {
        return NULL;
    }

    pool = (axutil_thread_pool_t *) AXIS2_MALLOC(allocator, sizeof(axutil_thread_pool_t));

    if (!pool)
    {
        return NULL;
    }
    pool->allocator = allocator;
    pool->mutex = axutil_thread_mutex_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);
    pool->job_posted = axutil_thread_cond_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);
    pool->job_taken = axutil_thread_cond_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);
    pool->arrsz = poolsize;
    pool->state = ALL_RUN;
    pool->queue = axutil_pool_queue_make_queue(poolsize);
    gettimeofday(pool->created, NULL);

    /* create the array of threads within the pool */
    pool->array = (axutil_thread_t **) AXIS2_MALLOC(allocator, pool->arrsz * sizeof(axutil_thread_t *));
    if (!pool->array) 
    {
        AXIS2_FREE(allocator, pool);
        pool = NULL;
        return NULL;
    }

    /* bring each thread to life (update counters in loop so threads can
     * access pool->live to find out their ID#
     */
    for (i = 0; i < pool->arrsz; ++i) 
    {
        axutil_thread_t *axis2_thd = NULL;

        axis2_thd = axutil_thread_create(allocator, NULL, axutil_thread_pool_do_work, (void *) pool);
        if(!axis2_thd)
        {
            AXIS2_FREE(allocator, pool);
            return NULL;
        }

        pool->array[i] = axis2_thd;

        pool->live = i+1;

        axutil_thread_detach(pool->array[i]); /* automatic cleanup when thread exits. */
    }

    return pool;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_thread_pool_free(
    axutil_thread_pool_t *pool)
{
    if (!pool)
    {
        return;
    }
    if (!pool->allocator)
    {
        return;
    }
    AXIS2_FREE(pool->allocator, pool);
    return;

  }

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_thread_pool_dispatch(
    axutil_thread_pool_t *pool,
    axutil_thread_start_t func,
    void *data)
{
    /*int old_cancel;*/

    if (!pool)
    {
        return AXIS2_FAILURE;
    }
    if (!pool->allocator)
    {
        return AXIS2_FAILURE;
    }

    if(pool == (axutil_thread_pool_t *) data)
    {
    }
    else
    {
        /*pthread_cleanup_push(pthread_mutex_unlock, (void *) &pool->mutex);*/
      
        /* Grab the mutex */
        axutil_thread_mutex_lock(pool->mutex);

        while(!axutil_pool_queue_can_accept_work(pool->queue))
        {
            axutil_thread_cond_signal(pool->job_posted);
            axutil_thread_cond_wait(pool->job_taken, pool->mutex);
        }
      
        /* Finally, there's room to post a job. Do so and signal workers. */
        axutil_pool_queue_add_work_order(pool->queue, func, data, NULL, NULL);

        axutil_thread_cond_signal(pool->job_posted);
      
        /* Yield mutex so a worker can pick up the job */
        axutil_thread_mutex_unlock(pool->mutex);
        /*pthread_cleanup_pop(1);*/
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_thread_pool_join_thread(
    axutil_thread_pool_t *pool,
    axutil_thread_t *thd)
{
    if (!pool || !thd)
    {
        return AXIS2_FAILURE;
    }
    return axutil_thread_join(thd);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_thread_pool_exit_thread(
    axutil_thread_pool_t *pool,
    axutil_thread_t *thd)
{
    if (!pool || !thd)
    {
        return AXIS2_FAILURE;
    }
    return axutil_thread_exit(thd, pool->allocator);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_thread_pool_thread_detach(
    axutil_thread_pool_t *pool,
    axutil_thread_t *thd)
{
    if (!pool || !thd)
    {
        return AXIS2_FAILURE;
    }
    return axutil_thread_detach(thd);
}

AXIS2_EXTERN axutil_env_t *AXIS2_CALL
axutil_init_thread_env(
    const axutil_env_t *system_env)
{
    axutil_error_t *error = axutil_error_create(system_env->allocator);
    return axutil_env_create_with_error_log_thread_pool(system_env->allocator,
                                                        error, system_env->log,
                                                        system_env->
                                                        thread_pool);
}

AXIS2_EXTERN void AXIS2_CALL
axutil_free_thread_env(
    struct axutil_env *thread_env)
{
    if (!thread_env)
    {
        return;
    }

    if (--(thread_env->ref) > 0)
    {
        return;
    }

    /* log, thread_pool and allocator are shared, so do not free them */
    thread_env->log = NULL;
    thread_env->thread_pool = NULL;
    if (thread_env->error)
    {
        AXIS2_ERROR_FREE(thread_env->error);
    }
    AXIS2_FREE(thread_env->allocator, thread_env);
}
