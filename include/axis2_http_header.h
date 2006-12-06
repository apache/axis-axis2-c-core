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

#ifndef AXIS2_HTTP_HEADER_H
#define AXIS2_HTTP_HEADER_H

/**
 * @defgroup axis2_http_header http header
 * @ingroup axis2_core_trans_http
 * Description
 * @{
 */


/**
 * @file axis2_http_header.h
 * @brief axis2 HTTP Header name:value pair implementation
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_header_ops */
    typedef struct axis2_http_header_ops axis2_http_header_ops_t;
    /** Type name for struct axis2_http_header */
    typedef struct axis2_http_header axis2_http_header_t;


    /**
     * HTTP Header ops struct
     * Encapsulator struct for ops of axis2_http_header
     */
    struct axis2_http_header_ops
    {
        /**
         * @param header pointer to header
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                to_external_form)(
                    axis2_http_header_t *header,
                    const axis2_env_t *env);

        /**
         * @param header pointer to header
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_http_header_t *header,
                    const axis2_env_t *env);

        /**
         * @param header pointer to header
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_value)(
                    const axis2_http_header_t *header,
                    const axis2_env_t *env);

        /**
         * @param header pointer to header
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_http_header_t *header,
                    const axis2_env_t *env);
    };

    /**
     * HTTP Header struct
     * Axis2 HTTP Header
     */
     struct axis2_http_header
    {
        /** Operations of http sender */
        axis2_http_header_ops_t *ops;
    };


    /**
     * @param env pointer to environment struct
     * @param name pointer to name
     * @param value pointer to value
     */
    AXIS2_EXTERN axis2_http_header_t *AXIS2_CALL
    axis2_http_header_create (
        const axis2_env_t *env,
        const axis2_char_t *name,
        const axis2_char_t *value);

    /**
     * @param env pointer to environment struct
     * @param str pointer to str
     */
    AXIS2_EXTERN axis2_http_header_t *AXIS2_CALL AXIS2_CALL
    axis2_http_header_create_by_str (
        const axis2_env_t *env,
        const axis2_char_t *str);

/************************** Start of function macros **************************/

/** To external from.
    @sa axis2_http_header_ops#to_external_form */
#define AXIS2_HTTP_HEADER_TO_EXTERNAL_FORM(header, env) \
                                ((header)->ops->to_external_form(header, env))

/** Gets the name.
    @sa axis2_http_header_ops#get_name */
#define AXIS2_HTTP_HEADER_GET_NAME(header, env) \
                                ((header)->ops->get_name(header, env))

/** Get value.
    @sa axis2_http_header_ops#get_value */
#define AXIS2_HTTP_HEADER_GET_VALUE(header, env) \
                                ((header)->ops->get_value(header, env))

/** Frees the http header.
    @sa axis2_http_header_ops#free */
#define AXIS2_HTTP_HEADER_FREE(header, env) \
                                ((header)->ops->free(header, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_HEADER_H */
