#ifndef AXIS2_ERRNO_H
#define AXIS2_ERRNO_H

/**
 * @file axis2_errno.h
 * @brief Axis2c Error Codes
 */

//#include <apr_errno.h>

/**
 * AXIS2_START_ERROR is where the AXIS2 specific error values start.
 * In apr_errno.h APR_OS_START_USERERR is defined as start of the 
 * error codes for applications using apr.
 */
#define AXIS2_START_ERROR    20000

#define AXIS2_ERROR_SPACE    50000


#define AXIS2_START_STATUS         AXIS2_START_ERROR + AXIS2_ERROR_SPACE

/** 
 * @defgroup AXIS2_Error AXIS2 Error Values
 * <PRE>
 * <b>AXIS2 ERROR VALUES</b>
 * AXIS2_ERROR_OM_MEMORY_ALLOCATION      AXIS2 was unable to
 *                                        allocate the requested memory for the OM 
 * AXIS2_ERROR_NULL_MEMORY_ACCESS        trying to access unallocated memory
 *
 *</PRE>
 * <PRE>
 * <b>AXIS2 STATUS VALUES</b>
 * AXIS2_SUCCESS                         Success status code
 * AXIS2_FAILIURE                        Failure status code
 *
 * </PRE>
 # @addtogroup AXIS2 Error codes
 * @{
 */
/** @see AXIS2_ERROR_OM_MEMORY_ALLOCATION */
#define AXIS2_ERROR_INVALID_POINTER_PARAMATERS        (AXIS2_START_ERROR + 1)

#define AXIS2_ERROR_OM_MEMORY_ALLOCATION        (AXIS2_START_ERROR + 2)
#define AXIS2_ERROR_NULL_MEMORY_ACCESS        (AXIS2_START_ERROR + 3)
#define AXIS2_ERROR_MEMORY_ALLOCATION        (AXIS2_START_ERROR + 4)


/** @} */

/** 
 * @addtogroup AXIS2 Status codes
 * @{
 */

#define AXIS2_SUCCESS        (AXIS2_START_STATUS + 1)
#define AXIS2_FAILURE       (AXIS2_START_STATUS + 2)


/** @} */

#endif /* AXIS2_ERRNO_H */
