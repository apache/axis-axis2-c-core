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

#include <stdlib.h>
#include <string.h>
#include <axutil_env.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_string.h>

AXIS2_EXTERN axutil_env_t *AXIS2_CALL
axutil_env_create(
    axutil_allocator_t *allocator)
{
    axutil_env_t *env;

    if (!allocator)
        return NULL;

    env = (axutil_env_t *) AXIS2_MALLOC(allocator, sizeof(axutil_env_t));

    if (!env)
        return NULL;

    memset(env, 0, sizeof(axutil_env_t));

    env->allocator = allocator;
    
    env->log = axutil_log_create_default(allocator);
    env->log->level = AXIS2_LOG_LEVEL_DEBUG; /* default log level is debug */
    env->log_enabled = AXIS2_TRUE;

    /* Create default error struct */
    env->error = axutil_error_create(allocator);
    if (!env->error)
    {
        AXIS2_FREE(allocator, env);
        return NULL;
    }
    
    /* Call error init to fill in the axutil_error_messages array.
       This array holds the error messages with respect to error codes */
    axutil_error_init();

    env->ref = 1;

    return env;
}

AXIS2_EXTERN axutil_env_t *AXIS2_CALL
axutil_env_create_with_error_log(
    axutil_allocator_t *allocator,
    axutil_error_t *error,
    axutil_log_t *log)
{
    axutil_env_t *env;
    if (!allocator || !error)
        return NULL;

    env = (axutil_env_t *) AXIS2_MALLOC(allocator, sizeof(axutil_env_t));

    if (!env)
        return NULL;

    memset(env, 0, sizeof(axutil_env_t));

    env->allocator = allocator;
    env->error = error;
    env->log = log;

    if (env->log)
        env->log_enabled = AXIS2_TRUE;
    else
        env->log_enabled = AXIS2_FALSE;

    axutil_error_init();

    env->ref = 1;

    return env;
}

AXIS2_EXTERN axutil_env_t *AXIS2_CALL
axutil_env_create_with_error(
    axutil_allocator_t *allocator,
    axutil_error_t *error)
{
    return axutil_env_create_with_error_log(allocator, error, NULL);
}

AXIS2_EXTERN axutil_env_t *AXIS2_CALL
axutil_env_create_with_error_log_thread_pool(
    axutil_allocator_t *allocator,
    axutil_error_t *error,
    axutil_log_t *log,
    axutil_thread_pool_t *pool)
{
    axutil_env_t *env;
    if (!allocator || !error || !pool)
        return NULL;

    env = (axutil_env_t *)AXIS2_MALLOC(allocator ,sizeof(axutil_env_t));
	

    if (!env)
        return NULL;

    memset(env, 0, sizeof(axutil_env_t));

    env->allocator = allocator;
    env->error = error;
    env->log = log;
    
    env->thread_pool = pool;

    if (env->log)
        env->log_enabled = AXIS2_TRUE;
    else
        env->log_enabled = AXIS2_FALSE;

    axutil_error_init();

    env->ref = 1;

    return env;
}

AXIS2_EXTERN axutil_env_t *AXIS2_CALL
axutil_env_create_all(
    const axis2_char_t *log_file,
    const axutil_log_levels_t log_level)
{
    axutil_env_t *env = NULL;
    axutil_error_t *error = NULL;
    axutil_log_t *log = NULL;
    axutil_allocator_t *allocator = NULL;
    axutil_thread_pool_t *thread_pool = NULL;

    allocator = axutil_allocator_init(NULL);
    error = axutil_error_create(allocator);

    if (log_file)
    {
        log = axutil_log_create(allocator, NULL, log_file);
    }
    
    /* if log file name was not given or the log could not be create with 
       given name, create default log */
    if (!log)
    {
        log = axutil_log_create_default(allocator);
    }
    
    thread_pool = axutil_thread_pool_init(allocator);
    
    env = axutil_env_create_with_error_log_thread_pool(allocator, error, log,
                                                       thread_pool);
    if (env->log)
    {
        if (AXIS2_LOG_LEVEL_CRITICAL <= log_level && log_level <= AXIS2_LOG_LEVEL_TRACE)
        {
            env->log->level = log_level;
        }
        else
        {
            env->log->level = AXIS2_LOG_LEVEL_DEBUG; /* default log level is debug */
        }
    }

    env->ref = 1;

    return env;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_env_check_status(
    const axutil_env_t *env)
{
    if (env && env->error)
        return AXIS2_ERROR_GET_STATUS_CODE(env->error);

    return AXIS2_CRITICAL_FAILURE;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_env_enable_log(
    axutil_env_t *env,
    axis2_bool_t enable)
{
    if (env)
    {
        env->log_enabled = enable;
        return AXIS2_SUCCESS;
    }

    return AXIS2_CRITICAL_FAILURE;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_env_free(
    axutil_env_t *env)
{
    axutil_allocator_t *allocator = NULL;

    if (!env)
        return;

    if (--(env->ref) > 0)
    {
        return;
    }
    
    allocator = env->allocator;

    if (env->log)
    {
        AXIS2_LOG_FREE(env->allocator, env->log);
    }
    
    if (env->error)
    {
        AXIS2_ERROR_FREE(env->error);
    }
    
    if (env->thread_pool)
    {
        axutil_thread_pool_free(env->thread_pool);
    }
    
    if (env->allocator)
    {
        AXIS2_FREE(env->allocator, env);
    }
    
    if (allocator)
    {
        AXIS2_FREE(allocator, allocator);
    }

    return;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_env_free_masked(
    axutil_env_t *env,
    char mask)
{
    if (!env)
        return;

    if (--(env->ref) > 0)
    {
        return;
    }

    if (mask & AXIS_ENV_FREE_LOG)
    {
        AXIS2_LOG_FREE(env->allocator, env->log);
    }
    
    if (mask & AXIS_ENV_FREE_ERROR)
    {
        AXIS2_ERROR_FREE(env->error);
    }
    
    if (mask & AXIS_ENV_FREE_THREADPOOL)
    {
        axutil_thread_pool_free(env->thread_pool);
    }
    
    if (env)
        AXIS2_FREE(env->allocator, env);

    return;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_env_increment_ref(
    axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    env->ref++;
    return AXIS2_SUCCESS;
}

