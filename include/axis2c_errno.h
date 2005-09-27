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

#ifndef AXIS2C_ERRNO_H
#define AXIS2C_ERRNO_H

/**
 * @file axis2c_errno.h
 * @brief Axis2c Error Codes
 */

#include <apr_errno.h>

/**
 * AXIS2C_START_ERROR is where the AXIS2C specific error values start.
 * In apr_errno.h APR_OS_START_USERERR is defined as start of the 
 * error codes for applications using apr.
 */
#define AXIS2C_START_ERROR    APR_OS_START_USERERR

#define AXIS2C_START_SPACE    50000


#define AXIS2C_STATUS         AXIS2C_START_ERROR + AXIS2C_ERROR_SPACE

/** 
 * @defgroup AXIS2C_Error AXIS2C Error Values
 * <PRE>
 * <b>AXIS2C ERROR VALUES</b>
 * AXIS2C_ERROR_OM_MEMORY_ALLOCATION      AXIS2C was unable to
 *                                        allocate the requested memory for the OM 
 *</PRE>
 * <PRE>
 * <b>AXIS2C STATUS VALUES</b>
 * AXIS2C_SUCCESS                         Success status code
 * AXIS2C_FAILIURE                        Failure status code
 *
 * </PRE>
 # @addtogroup AXIS2C Error codes
 * @{
 */
/** @see AXIS2C_ERROR_OM_MEMORY_ALLOCATION */
#define AXIS2C_ERROR_OM_MEMORY_ALLOCATION        (AXIS2C_START_ERROR + 1)

/** @} */

/** 
 * @addtogroup AXIS2C Status codes
 * @{
 */

#define AXIS2C_SUCCESS        (AXIS2C_START_STATUS + 1)
#define AXIS2C_FAILURE       (APR_OS_START_STATUS + 2)


/** @} */

#endif /* AXIS2C_ERRNO_H */
