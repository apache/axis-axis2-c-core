#ifndef _AXIS2C_ERRNO_H_
#define _AXIS2C_ERRNO_H_

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

#endif /* _AXIS2C_ERRNO_H_ */
