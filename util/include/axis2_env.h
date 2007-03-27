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

#ifndef AXIS2_ENV_H
#define AXIS2_ENV_H

/**
 * @file axis2_env.h
 * @brief Axis2 environment that acts as a container for error, log and memory
 *  allocator routines
 */

#include <axis2_allocator.h>
#include <axis2_error.h>
#include <axis2_log.h>
#include <axis2_thread_pool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @defgroup axis2_util utilities
      * @ingroup axis2
      * @{
      * @}
      */


    struct axis2_env;
    struct axis2_env_ops;

    /**
     * @defgroup axis2_env environment
     * @ingroup axis2_util
     * @{
     */

    /**
      * \brief Axis2 Environment struct
      *
      * Environment acts as a container for error, log, memory allocator and other
      * routines
      */
    typedef struct axis2_env
    {
        /** Memory allocation routines */
        axis2_allocator_t *allocator;
        /** Error handling */
        axis2_error_t *error;
        /** Logging routines */
        axis2_log_t *log;
        /** This flag indicate whether logging is enabled or not */
        axis2_bool_t log_enabled;
        /** Thread pooling routines */
        axis2_thread_pool_t *thread_pool;
    }
    axis2_env_t;

    /**
    * Creates an environment struct with all of its default parts,
    * that is an allocator, error, log and a thread pool.
    * @param log_file name of the log file
    * @param log_level log level to be used
    * @return pointer to the newly created environment struct 
    */
    AXIS2_EXTERN axis2_env_t * AXIS2_CALL 
    axis2_env_create_all(const axis2_char_t *log_file,
        const axis2_log_levels_t log_level);

    /**
     * Creates an environment struct. Users of axis2 should not use this function
    * to create an environment. He should use the other two create functions.
     * @param allocator pointer to an instance of allocator struct. Must be non-NULL   
     * @return pointer to the newly created environment struct 
     */
    AXIS2_EXTERN axis2_env_t * AXIS2_CALL 
    axis2_env_create(axis2_allocator_t *allocator);

    /**
      * Creates an environment struct
      * @param allocator pointer to an instance of allocator struct. Must be non-NULL
      * @param error pointer to an instance of error struct. Must be non-NULL.
      * it would be taken as a flag for no logging.  
      * @return pointer to the newly created environment struct 
      */
    AXIS2_EXTERN axis2_env_t * AXIS2_CALL 
    axis2_env_create_with_error(axis2_allocator_t *allocator, 
        axis2_error_t *error);

    /**
     * Creates an environment struct
     * @param allocator pointer to an instance of allocator struct. Must be non-NULL
     * @param error pointer to an instance of error struct. Must be non-NULL.
     * @param log pointer to an instance of log struct. May be NULL. If NULL
     * it would be taken as a flag for no logging.   
     * @return pointer to the newly created environment struct 
     */
    AXIS2_EXTERN axis2_env_t * AXIS2_CALL 
    axis2_env_create_with_error_log(axis2_allocator_t *allocator, 
        axis2_error_t *error, 
        axis2_log_t *log);

    /**
    * Creates an environment struct
    * @param allocator pointer to an instance of allocator struct. Must be non-NULL
    * @param error pointer to an instance of error struct. Must be non-NULL.
    * @param log pointer to an instance of log struct. May be NULL. If NULL
    * it would be taken as a flag for no logging.
    * @param pool pointer to an instance of thread_pool. May be NULL. If NULL
    * @return pointer to the newly created environment struct 
    */
    AXIS2_EXTERN axis2_env_t * AXIS2_CALL 
    axis2_env_create_with_error_log_thread_pool(axis2_allocator_t *allocator, 
        axis2_error_t *error, 
        axis2_log_t *log, 
        axis2_thread_pool_t *pool);

    /**
      * Creates an environment struct
      * @param allocator pointer to an instance of allocator struct. 
      * @param error pointer to an instance of error struct. 
      * Optional, can be NULL. If NULL default error handler would be used.
      * @param log pointer to an instance of log struct. Optional, can be NULL.
      * If NULL default log handler would be used.
      * @param string pointer to an instance of string struct. 
      * Optional, can be NULL. If NULL default string handler would be used.
      * @return pointer to the newly created environment struct 
      */
    AXIS2_EXTERN void AXIS2_CALL 
    axis2_env_free(axis2_env_t *env);

    /**
    * Frees the environment
    * @param env environment to be freed
    * @param mask bit pattern to according to which the components of the env 
    * struct are freed
    *       0x1 - Frees the log
    *       0x2 - Frees the error
    *       0x4 - Frees the thread pool
    *       You can use combinations to free multiple components as well
    *       Eg : 0x3 frees both log and error
    * @return status of the operation
    */
    AXIS2_EXTERN void AXIS2_CALL 
    axis2_env_free_masked(axis2_env_t *env, 
        char mask);
    /**
     * Enables logging
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_env_enable_log(axis2_env_t *env, 
        axis2_bool_t enable);

    /**
      * Checks the status code of environment
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_env_check_status(const axis2_env_t *env);

    /**
      * Writes given message to the log
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_env_write_log(const axis2_env_t *env, 
        const char* message, 
        axis2_log_levels_t level);
        
#define AXIS2_LOG(env, message, level) axis2_env_write_log (env,message, level)

/*#define AXIS2_ENV_CHECK(env, error_return) \
    if(!env) \
    { \
        return error_return; \
    }*/

#define AXIS2_ENV_CHECK(env, error_return) 

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENV_H */


