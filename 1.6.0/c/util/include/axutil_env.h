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

#ifndef AXUTIL_ENV_H
#define AXUTIL_ENV_H

/**
 * @file axutil_env.h
 * @brief Axis2 environment that acts as a container for error, log and memory
 * allocator routines
 */

#include <axutil_allocator.h>
#include <axutil_error.h>
#include <axutil_log.h>
#include <axutil_thread_pool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** 
     * @defgroup axis2_util utilities
     * @ingroup axis2
     * @{
     * @}
     */

    struct axutil_env;
    struct axutil_env_ops;

    /**
     * @defgroup axutil_env environment
     * @ingroup axis2_util
     * @{
     */

    /**
      * \brief Axis2 Environment struct
      *
      * Environment acts as a container for error, log, memory allocator and 
      * threading routines
      */
    typedef struct axutil_env
    {

        /** Memory allocation routines */
        axutil_allocator_t *allocator;

        /** Error handling */
        axutil_error_t *error;

        /** Logging routines */
        axutil_log_t *log;

        /** This flag indicate whether logging is enabled or not */
        axis2_bool_t log_enabled;

        /** Thread pool routines */
        axutil_thread_pool_t *thread_pool;

        int ref;
    }
    axutil_env_t;

    /**
     * Creates an environment struct. Would include a default log and error 
     * structs within the created environment. By default, logging would be enabled 
     * and the default log level would be debug.
     * @param allocator pointer to an instance of allocator struct. Must not be NULL   
     * @return pointer to the newly created environment struct 
     */
    AXIS2_EXTERN axutil_env_t *AXIS2_CALL
    axutil_env_create(
        axutil_allocator_t * allocator);

    /**
     * Creates an environment struct with all of its default parts,
     * that is an allocator, error, log and a thread pool.
     * @param log_file name of the log file. If NULL, a default log would be created.
     * @param log_level log level to be used. If not valid, debug would be 
     * used as the default log level
     * @return pointer to the newly created environment struct 
     */
    AXIS2_EXTERN axutil_env_t *AXIS2_CALL
    axutil_env_create_all(
        const axis2_char_t * log_file,
        const axutil_log_levels_t log_level);


    /**
      * Creates an environment struct with given error struct.
      * @param allocator pointer to an instance of allocator struct. Must not be NULL
      * @param error pointer to an instance of error struct. Must not be NULL
      * @return pointer to the newly created environment struct 
      */
    AXIS2_EXTERN axutil_env_t *AXIS2_CALL
    axutil_env_create_with_error(
        axutil_allocator_t * allocator,
        axutil_error_t * error);

    /**
     * Creates an environment struct with given error and log structs.
     * @param allocator pointer to an instance of allocator struct. Must not be NULL
     * @param error pointer to an instance of error struct. Must not be NULL
     * @param log pointer to an instance of log struct. If NULL it would be 
     * assumed that logging is disabled.
     * @return pointer to the newly created environment struct 
     */
    AXIS2_EXTERN axutil_env_t *AXIS2_CALL
    axutil_env_create_with_error_log(
        axutil_allocator_t * allocator,
        axutil_error_t * error,
        axutil_log_t * log);

    /**
     * Creates an environment struct with given error, log and thread pool structs.
     * @param allocator pointer to an instance of allocator struct. Must not be NULL
     * @param error pointer to an instance of error struct. Must not be NULL
     * @param log pointer to an instance of log struct. If NULL it would be
     * assumed that logging is disabled.
     * @param pool pointer to an instance of thread_pool. Must not be NULL
     * @return pointer to the newly created environment struct 
     */
    AXIS2_EXTERN axutil_env_t *AXIS2_CALL
    axutil_env_create_with_error_log_thread_pool(
        axutil_allocator_t * allocator,
        axutil_error_t * error,
        axutil_log_t * log,
        axutil_thread_pool_t * pool);

    /**
     * Enable or disable logging.
     * @param env pointer to environment struct
     * @param enable AXIS2_TRUE to enable logging and AXIS2_FALSE to 
     * disable logging
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_env_enable_log(
        axutil_env_t * env,
        axis2_bool_t enable);

    /**
     * Checks the status code of environment stored within error struct.
     * @param env pointer to environment struct
     * @return error status code or AXIS2_CRITICAL_FAILURE in case of
     * a failure
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_env_check_status(
        const axutil_env_t * env);

    /**
     * Frees an environment struct instance.
     * @param env pointer to environment struct instance to be freed.
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_env_free(
        axutil_env_t * env);


    #define AXIS_ENV_FREE_LOG        0x1
    #define AXIS_ENV_FREE_ERROR      0x2
    #define AXIS_ENV_FREE_THREADPOOL 0x4

    /**
     * Frees the environment components based on the mask.
     * @param env pointer to environment struct to be freed
     * @param mask bit pattern indicating which components of the env 
     * struct are to be freed
     *       AXIS_ENV_FREE_LOG        - Frees the log
     *       AXIS_ENV_FREE_ERROR      - Frees the error
     *       AXIS_ENV_FREE_THREADPOOL - Frees the thread pool
     *       You can use combinations to free multiple components as well
     *       E.g : AXIS_ENV_FREE_LOG | AXIS_ENV_FREE_ERROR frees both log and error, but not the thread pool
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_env_free_masked(
        axutil_env_t * env,
        char mask);

    /**
     * Incrent the reference count.This is used when objects are created 
     * using this env and keeping this for future use.
     * @param env pointer to environment struct instance to be freed.
     * @return void
     */
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_env_increment_ref(
        axutil_env_t * env);
    


/* AXIS2_ENV_CHECK is a macro to check environment pointer.
   Currently this is set to an empty value.
   But it was used to be defined as:
   #define AXIS2_ENV_CHECK(env, error_return) \
       if(!env) \
       { \
           return error_return; \
       }
*/
#define AXIS2_ENV_CHECK(env, error_return)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENV_H */
