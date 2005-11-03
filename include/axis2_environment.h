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

#ifndef AXIS2_ENVIRONMENT_H
#define AXIS2_ENVIRONMENT_H

/**
 * @file axis2_environment.h
 * @brief Axis2 environment that acts as a container for error, log and memory 
 *  allocator routines
 */

#include <axis2_allocator.h>
#include <axis2_error.h>
#include <axis2_stream.h>
#include <axis2_log.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_util Axis2 utilities
  * @ingroup axis2
  * @{
  */

/** @} */

    struct axis2_environment;
    struct axis2_environment_ops;

/**
 * @defgroup axis2_environment Environment Container
 * @ingroup axis2_util 
 * @{
 */

  /** 
    * \brief Axis2 Environment struct
    *
    * Environment acts as a container for error, log, memory allocator and other
    * routines
    */
    typedef struct axis2_environment
    {
        /** Memory allocation routines */
        axis2_allocator_t *allocator;
        /** Error handling */
        axis2_error_t *error;
        /** IO routines */
        axis2_stream_t *stream;
        /** Logging routines */
        axis2_log_t *log;
		/** This flag indicate whether logging is enabled or not */
		axis2_bool_t log_enabled;
		
    } axis2_env_t;

	/**
    * Creates an environment struct. Users of axis2 should not use this function
	* to create an environment. He should use the other two create functions.
    * @param allocator pointer to an instance of allocator struct. Must be non-NULL    
    * @return pointer to the newly created environment struct 
    */
    AXIS2_DECLARE(axis2_env_t *) axis2_environment_create (axis2_allocator_t
                                                   *allocator);                                                   
	
  /**
    * Creates an environment struct
    * @param allocator pointer to an instance of allocator struct. Must be non-NULL
    * @param error pointer to an instance of error struct. Must be non-NULL.
    * @param stream pointer to an instance of stream struct. Must be non-NULL.
    * it would be taken as a flag for no logging.    
    * @return pointer to the newly created environment struct 
    */
    AXIS2_DECLARE(axis2_env_t *) axis2_environment_create_with_error_stream 
													(axis2_allocator_t *allocator
                                                   , axis2_error_t *error
                                                   , axis2_stream_t *stream);
	
	/**
    * Creates an environment struct
    * @param allocator pointer to an instance of allocator struct. Must be non-NULL
    * @param error pointer to an instance of error struct. Must be non-NULL.
    * @param stream pointer to an instance of stream struct. Must be non-NULL.
    * @param log pointer to an instance of log struct. May be NULL. If NULL
    * it would be taken as a flag for no logging.    
    * @return pointer to the newly created environment struct 
    */
    AXIS2_DECLARE(axis2_env_t *) axis2_environment_create_with_error_stream_log 
													(axis2_allocator_t *allocator
                                                   , axis2_error_t *error
                                                   , axis2_stream_t *stream
                                                   , axis2_log_t *log);

  /**
    * Creates an environment struct
    * @param allocator pointer to an instance of allocator struct. Mandatory, cannot be NULL
    * @param error pointer to an instance of error struct. Optional, can be NULL. If NULL default error handler would be used.
    * @param stream pointer to an instance of stream struct. Optional, can be NULL. If NULL default stream handler would be used.
    * @param log pointer to an instance of log struct. Optional, can be NULL. If NULL default log handler would be used.
    * @param string pointer to an instance of string struct. Optional, can be NULL. If NULL default string handler would be used.
    * @return pointer to the newly created environment struct 
    */
    AXIS2_DECLARE(axis2_status_t) axis2_environment_free (axis2_env_t *env);

    /**
      *
      */
    AXIS2_DECLARE(axis2_status_t) axis2_environment_check_status (axis2_env_t *env);

    #define AXIS2_ENV_CHECK(env, error_return) \
    if(!env || !(*env))  \
	{ \
		axis2_allocator_t *allocator = axis2_allocator_init (NULL); \
        *env = axis2_environment_create (allocator, NULL, NULL, NULL); \
		AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_CRTICAL_FAILURE);  \
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, \
		        AXIS2_ERROR_ENVIRONMENT_IS_NULL); \
		return error_return; \
	} \
    else \
    { \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_SUCCESS); \
    }
    

/** @} */



    





#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENVIRONMENT_H */
