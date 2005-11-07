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

#ifndef AXIS2_H
#define AXIS2_H

/**
 * @file axis2.h
 * @brief Axis2c specific global declarations
 */

#include <stdlib.h>
#include <stdio.h>
#include <axis2_env.h>

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
#define AXIS2_FUNC_PARAM_CHECK(object, env, error_return) \
    AXIS2_ENV_CHECK(env, error_return);\
    if (!object) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE); \
        return error_return; \
    } \
    else \
    { \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_SUCCESS); \
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
#define AXIS2_PARAM_CHECK(error, object, error_return) \
    if (!object) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_FAILURE); \
        return error_return; \
    } \
    else \
    { \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_SUCCESS); \
    } 

/**This macro is used to handle error situation. 
 * @param error_number Error number for the error occured
 * @param error_return If function return a status it should pass here 
 *        AXIS2_FAILURE. If function return a type pointer it should
 *        pass NULL
 * @return If function return a status code return AXIS2_SUCCESS. Else if
 *         function return a type pointer return NULL
 */    
#define AXIS2_ERROR_SET(error, error_number, error_return) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, error_number); \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_FAILURE); \
        return error_return; \
    }      

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup axis2 Axis2/C project
  *  @{
 */


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_H */
