#ifndef AXIS2_H
#define AXIS2_H

/**
 * @file axis2.h
 * @brief Axis2c specific global declarations
 */

#include <stdlib.h>
#include <stdio.h>
#include <axis2_env.h>

#define AXIS2_FUNC_PARAMETER_CHECK(object, env, error_return) \
    AXIS2_ENV_CHECK(env, error_return);\
    if (!object) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAMETER); \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE); \
        return error_return; \
    }\
    else \
    { \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_SUCCESS); \
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
