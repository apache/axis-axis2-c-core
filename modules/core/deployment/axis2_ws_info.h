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

#ifndef AXIS2_WS_INFO_H
#define AXIS2_WS_INFO_H

/** 
 * @defgroup axis2_ws_info Ws Info
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_ws_info.h
 * @brief Axis2 Ws Info interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_conf;

    /** Type name for struct axis2_ws_info */
    typedef struct axis2_ws_info axis2_ws_info_t;
    /** Type name for struct axis2_ws_info_ops */
    typedef struct axis2_ws_info_ops axis2_ws_info_ops_t;


    /**
     * Ws Info ops struct
     * Encapsulator struct for ops of axis2_ws_info
     */
    struct axis2_ws_info_ops
    {
        /** 
         * De-allocate memory
         * @param ws_info pointer to ws info
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_ws_info_t *ws_info,
                    const axis2_env_t *env);

        /** 
         * @param ws_info pointer to ws info
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_file_name)(
                    const axis2_ws_info_t *ws_info,
                    const axis2_env_t *env);

        /** 
         * @param ws_info pointer to ws info
         * @param env pointer to environment struct
         * @param file_name pointer to file name
         */
        axis2_status_t (AXIS2_CALL *
                set_file_name)(
                    axis2_ws_info_t *ws_info,
                    const axis2_env_t *env,
                    axis2_char_t *file_name);

        /** 
         * @param ws_info pointer to ws info
         * @param env pointer to environment struct
         */
        long (AXIS2_CALL *
                get_last_modified_date)(
                    const axis2_ws_info_t *ws_info,
                    const axis2_env_t *env);

        /** 
         * @param ws_info pointer to ws info
         * @param env pointer to environment struct
         * @param modified_data pointer to modified date
         */
        axis2_status_t (AXIS2_CALL *
                set_last_modified_date)(
                    axis2_ws_info_t *ws_info,
                    const axis2_env_t *env,
                    long last_modified_date);

        /** 
         * @param ws_info pointer to ws info
         * @param env pointer to environment struct
         */
        int (AXIS2_CALL *
                get_type)(
                    const axis2_ws_info_t *ws_info,
                    const axis2_env_t *env);

    };

    /**
     * Ws Info struct 
     */
    struct axis2_ws_info
    {
        /** Operations of ws info */
        axis2_ws_info_ops_t *ops;
    };

    /**
     * Creates description builder struct
     * @param env pointer to environment struct
     * @param file_name pointer to file name
     * @param last_modified_date pointer to last modified date
     * @return pointer to newly created ws info
     */
    AXIS2_EXTERN axis2_ws_info_t *AXIS2_CALL
    axis2_ws_info_create_with_file_name_and_last_modified_date(
        const axis2_env_t *env,
        axis2_char_t *file_name,
        long last_modified_date);

    /**
     * @param env pointer to environment struct
     * @param file_name pointer to file name
     * @param last_modifile_date last modified date
     * @param type type
     */
    AXIS2_EXTERN axis2_ws_info_t *AXIS2_CALL
    axis2_ws_info_create_with_file_name_and_last_modified_date_and_type(
        const axis2_env_t *env,
        axis2_char_t *file_name,
        long last_modified_date,
        int type);

/*************************** Function macros **********************************/

/** Frees the ws info.
    @sa axis2_ws_info_ops#free */
#define AXIS2_WS_INFO_FREE(ws_info, env) \
      ((ws_info)->ops->free (ws_info, env))

/** Gets the file name.
    @sa axis2_ws_info_ops#get_file_name */
#define AXIS2_WS_INFO_GET_FILE_NAME(ws_info, env) \
      ((ws_info)->ops->get_file_name (ws_info, env))

/** Set file name.
    @sa axis2_ws_info_ops#set_file_name */
#define AXIS2_WS_INFO_SET_FILE_NAME(ws_info, env, file_name) \
      ((ws_info)->ops->set_file_name (ws_info, env, file_name))

/** Gets the last modified date.
    @sa axis2_ws_info_ops#get_last_modified_date */
#define AXIS2_WS_INFO_GET_LAST_MODIFIED_DATE(ws_info, env) \
      ((ws_info)->ops->get_last_modified_date (ws_info, env))

/** Sets the last modified date.
    @sa axis2_ws_info_ops#set_last_modified_date */
#define AXIS2_WS_INFO_SET_LAST_MODIFIED_DATE(ws_info, env, last_modified_date) \
      ((ws_info)->ops->set_last_modified_date (ws_info, env, last_modified_date))

/** Get type.
    @sa axis2_ws_info_ops#get_type */
#define AXIS2_WS_INFO_GET_TYPE(ws_info, env) \
      ((ws_info)->ops->get_type (ws_info, env))


/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WS_INFO_H */
