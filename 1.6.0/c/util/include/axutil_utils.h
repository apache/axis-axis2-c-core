
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

#ifndef AXUTIL_UTILS_H
#define AXUTIL_UTILS_H

#include <axutil_utils_defines.h>
#include <axutil_error.h>
#include <axutil_env.h>
#include <axutil_date_time.h>
#include <axutil_base64_binary.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axutil_utils utils
     * @ingroup axis2_util
     * @{
     */

    /**
     * @file axutil_utils.h
     */

#define AXUTIL_LOG_FILE_SIZE 1024 * 1024 * 32
#define AXUTIL_LOG_FILE_NAME_SIZE 512

    /** This macro is called to check whether structure on which function is called
     *  is NULL and to check whether the environment structure passed is valid.
     * @param object structure on which function is called
     * @param env environment to be checked for validity
     * @param error_return If function return a status it should pass here
     *        AXIS2_FAILURE. If function return a type pointer it should
     *        pass NULL
     * @return If function return a status code return AXIS2_SUCCESS. Else if
     *         function return a type pointer return NULL
     */
#define AXIS2_FUNC_PARAM_CHECK(object, env, error_return)               \
    if (!object)                                                        \
    {                                                                   \
        AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);         \
        return error_return;                                            \
    }                                                                   \
    else                                                                \
    {                                                                   \
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_SUCCESS);              \
    }


    /**This macro is called to check whether an object is NULL.
     * if object is NULL error number and status code is set
     * @param object object to be check for NULL
     * @param error_return If function return a status it should pass here
     *        AXIS2_FAILURE. If function return a type pointer it should
     *        pass NULL
     * @return If function return a status code return AXIS2_SUCCESS. Else if
     *         function return a type pointer return NULL
     */
#define AXIS2_PARAM_CHECK(error, object, error_return)                  \
    if (!object)                                                        \
    {                                                                   \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_FAILURE);              \
        return error_return;                                            \
    }                                                                   \
    else                                                                \
    {                                                                   \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_SUCCESS);              \
    }



#define AXIS2_PARAM_CHECK_VOID(error, object)                           \
    if (!object)                                                        \
    {                                                                   \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_FAILURE);              \
        return;                                                         \
    }



    /**This macro is used to handle error situation.
     * @param error_number Error number for the error occured
     * @param error_return If function return a status it should pass here
     *        AXIS2_FAILURE. If function return a type pointer it should
     *        pass NULL
     * @return If function return a status code return AXIS2_SUCCESS. Else if
     *         function return a type pointer return NULL
     */
#define AXIS2_ERROR_SET(error, error_number, status_code)   \
    {                                                       \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, error_number);  \
        AXIS2_ERROR_SET_STATUS_CODE(error, status_code);    \
    }

    /**
     * This macro is used to set and error, and log it. In addition to that
     * you are capable of specifying the file name and line number
     * @param env Reference to env struct
     * @param error_number Error number for the error occured
     * @param status_code The Error Status to be set
     * @param file_name_line_no File name and line number constant
     */
#define AXIS2_HANDLE_ERROR_WITH_FILE(env, error_number,          \
            status_code, file_name_line_no)                      \
    {                                                            \
        AXIS2_ERROR_SET(env->error, error_number, status_code);  \
        AXIS2_LOG_ERROR(env->log, file_name_line_no,             \
            AXIS2_ERROR_GET_MESSAGE(env->error));                \
    } 

    /**
     * This macro is used to set and error, and log it
     * @param env Reference to env struct
     * @param error_number Error number for the error occured
     * @param status_code The Error Status to be set
     */
#define AXIS2_HANDLE_ERROR(env, error_number, status_code)               \
            AXIS2_HANDLE_ERROR_WITH_FILE(env, error_number, status_code, \
            AXIS2_LOG_SI)                                                \

    /** Method names in the loadable libraries */

#define AXIS2_CREATE_FUNCTION "axis2_get_instance"
#define AXIS2_DELETE_FUNCTION "axis2_remove_instance"

    typedef void(
        AXIS2_CALL
        * AXIS2_FREE_VOID_ARG)(
            void *obj_to_be_freed,
            const axutil_env_t * env);

    /* Function pointer typedef for read callback */
    typedef int(
        AXIS2_CALL
        * AXIS2_READ_INPUT_CALLBACK)(
            char *buffer,
            int size,
            void *ctx);

    /* Function pointer typedef for close callback */
    typedef int(
        AXIS2_CALL
        * AXIS2_CLOSE_INPUT_CALLBACK)(
            void *ctx);

    /**
     * \brief Axis2 scopes
     *
     * Possible scope value for Axis2
     */
    enum axis2_scopes
    {

        /** Request scope */
        AXIS2_SCOPE_REQUEST = 0,

        /** Session scope */
        AXIS2_SCOPE_SESSION,

        /** Application scope */
        AXIS2_SCOPE_APPLICATION
    };

#define AXIS2_TARGET_EPR "target_epr"
#define AXIS2_DUMP_INPUT_MSG_TRUE "dump"

    /** 
     * This function allows the user match a REST URL template with the
     * Request URL. It returns a 3-dimensional array with pairs of elements
     * of axis2_char_t arrays (strings). The caller is responsible to free
     * the memory allocated by the function for the return value.
     * @param env pointer to environment struct
     * @param tmpl Template to Match
     * @param url Request URL
     * @param match_count variable to store match count
     * @param matches axis2_char_t *** <code>axis2_char_t ***<code>
     * @return AXIS2_SUCCESS if all matches were found or AXIS2_FAILURE.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_parse_rest_url_for_params(
        const axutil_env_t * env,
        const axis2_char_t * tmpl,
        const axis2_char_t * url,
        int * match_count,
        axis2_char_t **** matches);

    /**
     * This function allows users to resolve the service and op from the 
     * url. It returns an array of 2 elements of axis2_char_t arrays (strings).
     * The caller is responsible to free the memory allocated by the function
     * for the return value.
     * @param env pointer to environment struct
     * @param request url
     * @return axis2_char_t ** <code>axis2_char_t **<code>
     */
    AXIS2_EXTERN axis2_char_t **AXIS2_CALL
    axutil_parse_request_url_for_svc_and_op(
        const axutil_env_t * env,
        const axis2_char_t * request);

    /**
     * Quotes an XML string.
     * Replace '<', '>', and '&' with '&lt;', '&gt;', and '&amp;'.
     * If quotes is true, then replace '"' with '&quot;'.
     * @param env pointer to environment struct
     * @param s string to be quoted
     * @param quotes if AXIS2_TRUE then replace '"' with '&quot;'.
     * quotes is typically set to true for XML strings that will occur within
     * double quotes -- attribute values.
     * @return Encoded string if there are characters to be encoded, else NULL. 
     * The caller is responsible to free the memory allocated by the function
     * for the return value
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axutil_xml_quote_string(
        const axutil_env_t * env,
        const axis2_char_t * s,
        axis2_bool_t quotes);

    AXIS2_EXTERN int AXIS2_CALL
    axutil_hexit(axis2_char_t c);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_url_decode(
        const axutil_env_t * env,
        axis2_char_t * dest,
        axis2_char_t * src);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_char_2_byte(
        const axutil_env_t *env,
        axis2_char_t *char_buffer,
        axis2_byte_t **byte_buffer,
        int *byte_buffer_size);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_UTILS_H */
