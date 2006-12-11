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

#ifndef OXS_BUFFER_H
#define OXS_BUFFER_H


/**
  * @file oxs_buffer.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_util.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup  
 * @ingroup 
 * @{
 */

#define OXS_BUFFER_INITIAL_SIZE 1024

    /**
    * Allocate mode for the buffer
    * oxs_alloc_mode_exact : Minimizes the allocated memory size
    * oxs_alloc_mode_double : Minimizes number of Malloc calls
    */
    typedef enum {
        oxs_alloc_mode_exact = 0,
        oxs_alloc_mode_double
    } oxs_AllocMode;

    /** Type name for struct  oxs_buffer_ops */
    typedef struct oxs_buffer_ops oxs_buffer_ops_t;

    /** Type name for struct  oxs_buffer */
    typedef struct oxs_buffer oxs_buffer_t;

    struct oxs_buffer_ops
    {
		/**
		*Free function of the buffer
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/		
        axis2_status_t (AXIS2_CALL *
                free)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );
		/**
		*Removes the first (size) charcters from the buffer
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@size number of characters to be removed
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                remove_head)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );
		/**
		*Removes the last (size) charcters from the buffer
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@size number of characters to be removed		
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                remove_tail)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );
		/**
		*populates the buffer using the @data set the @size as the useful length
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@data the data for the buffer
		*@size the effective length of data
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	        
        axis2_status_t (AXIS2_CALL *
                populate)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    unsigned char *data,
                    int size
                );
		/**
		*Append data (to the end)
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@data the data for the buffer
		*@size the effective length of data
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                append)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    unsigned char *data,
                    int size
                );
		/**
		*Prepends data (to the front of the buffer)
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@data the data for the buffer
		*@size the effective length of data
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                prepend)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    unsigned char *data,
                    int size
                );
		/**
		*Reads a file specified by @filename
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@filename The name of the file
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                read_file)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    const axis2_char_t *filename
                );
		/**
		*Sets the size
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@size the value of the size
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                set_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );
		/**
		*Sets the maximum size of the buffer. Usually this will be allocated dynamically
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@size the maximum size of the buffer
		*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE
		*/	
        axis2_status_t (AXIS2_CALL *
                set_max_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );
		/**
		*Returns data
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@return data in the buffer
		*/	        
        unsigned char* (AXIS2_CALL *
                get_data)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );
		/**
		*Returns the effective length of the buffer
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@return the effective length of the buffer as int
		*/	
        int (AXIS2_CALL *
                get_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );
		/**
		*Returns the maximum size of the buffer
		*@buffer pointer to the OMXMLSec buffer struct
		*@env pointer to environment struct
		*@return the maximum size of the buffer
		*/	
        int (AXIS2_CALL *
                get_max_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );
    };

    struct oxs_buffer
    {
        oxs_buffer_ops_t *ops;
    };

AXIS2_EXTERN oxs_buffer_t *AXIS2_CALL
oxs_buffer_create(const axis2_env_t *env);

/*Macros*/

#define OXS_BUFFER_FREE(buffer,env)\
    ((buffer)->ops->free(buffer,env))

#define OXS_BUFFER_REMOVE_HEAD(buffer,env, size)\
    ((buffer)->ops->remove_head(buffer,env, size))

#define OXS_BUFFER_REMOVE_TAIL(buffer,env, size)\
    ((buffer)->ops->remove_tail(buffer,env, size))

#define OXS_BUFFER_POPULATE(buffer,env, data, size)\
    ((buffer)->ops->populate(buffer,env, data, size))

#define OXS_BUFFER_APPEND(buffer,env, data, size)\
    ((buffer)->ops->append(buffer,env, data, size))

#define OXS_BUFFER_PREPEND(buffer,env, data, size)\
    ((buffer)->ops->prepend(buffer,env, data, size))

#define OXS_BUFFER_READ_FILE(buffer,env, file_name)\
    ((buffer)->ops->read_file(buffer,env, file_name))

#define OXS_BUFFER_SET_SIZE(buffer,env, size)\
    ((buffer)->ops->set_size(buffer,env, size))

#define OXS_BUFFER_SET_MAX_SIZE(buffer, env, size)\
    ((buffer)->ops->set_max_size(buffer, env, size))

#define OXS_BUFFER_GET_DATA(buffer,env)\
    ((buffer)->ops->get_data(buffer,env))

#define OXS_BUFFER_GET_SIZE(buffer,env)\
    ((buffer)->ops->get_size(buffer,env))

#define OXS_BUFFER_GET_MAX_SIZE(buffer,env)\
    ((buffer)->ops->get_max_size(buffer,env))


/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_BUFFER_H */
