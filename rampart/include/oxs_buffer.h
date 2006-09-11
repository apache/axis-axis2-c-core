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
        axis2_status_t (AXIS2_CALL *
                free)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );

        axis2_status_t (AXIS2_CALL *
                remove_head)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );

        axis2_status_t (AXIS2_CALL *
                remove_tail)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );
        
        axis2_status_t (AXIS2_CALL *
                populate)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    unsigned char *data,
                    int size
                );

        axis2_status_t (AXIS2_CALL *
                append)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    unsigned char *data,
                    int size
                );

        axis2_status_t (AXIS2_CALL *
                prepend)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    unsigned char *data,
                    int size
                );

        axis2_status_t (AXIS2_CALL *
                read_file)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    const axis2_char_t *filename
                );

        axis2_status_t (AXIS2_CALL *
                set_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );

        axis2_status_t (AXIS2_CALL *
                set_max_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env,
                    int size
                );
        
        unsigned char* (AXIS2_CALL *
                get_data)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );

        int (AXIS2_CALL *
                get_size)(
                    oxs_buffer_t *buffer,
                    const axis2_env_t *env
                );

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
