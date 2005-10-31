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

#ifndef AXIS2_LOG_H
#define AXIS2_LOG_H

#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_log;
    struct axis2_log_ops;

/**
 * @defgroup axis2_log Log
 * @ingroup axis2_util 
 * @{
 */

/** 
  * \brief Axis2 log levels
  */
    typedef enum axis2_log_levels
    {
        /** Debug level, logs everything */
        AXIS2_LOG_DEBUG = 0,
        /** Info level, logs information */
        AXIS2_LOG_INFO,
        /** Warning level, logs only warnings */
        AXIS2_LOG_WARNING,
        /** Error level, logs only errors */
        AXIS2_LOG_ERROR,
        /** Critical level, logs only critical errors */
        AXIS2_LOG_CRITICAL
    } axis2_log_levels_t;

  /** 
    * \brief Axis2 log operations struct
    *
    * Encapsulator struct for operations of axis2_log
    */
    typedef struct axis2_log_ops
    {
    
      /**
       * deletes the log
       * @return axis2_status_t AXIS2_SUCCESS on success else AXIS2_FAILURE
       */

       axis2_status_t (AXIS2_CALL *free) (struct axis2_log *log);

      /**
        * writes to the log
        * @param buffer buffer to be written to log
        * @param size size of the buffer to be written to log
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_log_ops_write) (const void *buffer, size_t count);
    } axis2_log_ops_t;

  /** 
    * \brief Axis2 Log struct
    *
    * Log is the encapsulating struct for all log related data and operations
    */
    typedef struct axis2_log
    {
        /** Log related operations */
        struct axis2_log_ops *ops;
        /** Log level */
        axis2_log_levels_t level;
        /** Is logging enabled? */
        axis2_bool_t enabled;
    } axis2_log_t;
 
  /**
    * Creates a log struct
    * @param allocator allocator to be used. Mandatory, cannot be NULL    
    * @return pointer to the newly created log struct 
    */
   /* AXIS2_DECLARE(axis2_log_t *) axis2_log_create (axis2_allocator_t * allocator,
                                   axis2_log_ops_t * operations);*/

#define AXIS2_LOG_FREE(log) ((log->ops)->free(log))
#define AXIS22_LOG_WRITE(log, buffer, count) ((log)->ops->axis2_log_ops_write(buffer, count))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_LOG_H */
