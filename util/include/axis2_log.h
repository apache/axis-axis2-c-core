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

    typedef struct axis2_log_ops axis2_log_ops_t;
	 typedef struct axis2_log axis2_log_t;


#define AXIS2_LOG_SI __FILE__,__LINE__

/**
 * @defgroup axis2_log log
 * @ingroup axis2_util 
 * @{
 */

/*TODO:log_xml*/

   /**
     *Examples
     *To write debug information to log
     *AXIS2_LOG_DEBUG(log,AXIS2_LOG_SI,"log this %s %d","test",123);
     *This would log
     *"log this test 123" into the log file
     *
     *similar macros are defined for different log levels: CRITICAL,ERROR,WARNING and INFO
     *
     *CRITICAL and ERROR logs are always written to file and other logs are written 
     *depending on the log level set (log->level)
     */
/** 
  * \brief Axis2 log levels
  */
    typedef enum axis2_log_levels
    {
        /** Critical level, logs only critical errors */
        AXIS2_LOG_LEVEL_CRITICAL = 0,
        /** Error level, logs only errors */
        AXIS2_LOG_LEVEL_ERROR,
        /** Warning level, logs only warnings */
        AXIS2_LOG_LEVEL_WARNING,
        /** Info level, logs information */
        AXIS2_LOG_LEVEL_INFO,
        /** Debug level, logs everything */
        AXIS2_LOG_LEVEL_DEBUG,
        /** Trace level, Enable with compiler time option AXIS2_TRACE */
        AXIS2_LOG_LEVEL_TRACE
    } axis2_log_levels_t;







  /** 
    * \brief Axis2 log ops struct
    *
    * Encapsulator struct for ops of axis2_log
    */
    struct axis2_log_ops
    {
    
      /**
       * deletes the log
       * @return axis2_status_t AXIS2_SUCCESS on success else AXIS2_FAILURE
       */

       void (AXIS2_CALL *
      free) (axis2_allocator_t *allocator, 
            struct axis2_log *log);

      /**
        * writes to the log
        * @param buffer buffer to be written to log
        * @param size size of the buffer to be written to log
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        void (AXIS2_CALL *
       write) (axis2_log_t *log, 
             const axis2_char_t *buffer, 
             axis2_log_levels_t level,
             const axis2_char_t *file,
             const int line);
    };

  /** 
    * \brief Axis2 Log struct
    *
    * Log is the encapsulating struct for all log related data and ops
    */
    struct axis2_log
    {
        /** Log related ops */
        struct axis2_log_ops *ops;
        /** Log level */
        axis2_log_levels_t level;
        /** Is logging enabled? */
        axis2_bool_t enabled;

    };

AXIS2_EXTERN void AXIS2_CALL 
axis2_log_impl_log_critical(axis2_log_t *log, 
                      const axis2_char_t *filename, 
                     const int linenumber,
                     const axis2_char_t *format,...);

AXIS2_EXTERN void AXIS2_CALL 
axis2_log_impl_log_error(axis2_log_t *log,
                   const axis2_char_t *filename,
                   const int linenumber,
                   const axis2_char_t *format,...);

AXIS2_EXTERN void AXIS2_CALL 
axis2_log_impl_log_warning(axis2_log_t *log,
                     const axis2_char_t *filename,
                          const int linenumber,
                          const axis2_char_t *format,...);

AXIS2_EXTERN void AXIS2_CALL 
axis2_log_impl_log_info(axis2_log_t *log, 
                  const axis2_char_t *format,...);

AXIS2_EXTERN void AXIS2_CALL 
axis2_log_impl_log_debug(axis2_log_t *log,
                   const axis2_char_t *filename,
                   const int linenumber,
                   const axis2_char_t *format,...);

AXIS2_EXTERN void AXIS2_CALL 
axis2_log_impl_log_trace(axis2_log_t *log,
                         const axis2_char_t *filename,
                   const int linenumber,
                   const axis2_char_t *format,...);

#define AXIS2_LOG_FREE(allocator, log) \
      ((log->ops)->free(allocator, log))

#define AXIS2_LOG_WRITE(log, buffer, level) \
      ((log)->ops->write(log, buffer, level,AXIS2_LOG_SI))

#define AXIS2_LOG_DEBUG axis2_log_impl_log_debug 
#define AXIS2_LOG_INFO axis2_log_impl_log_info 
#define AXIS2_LOG_WARNING axis2_log_impl_log_warning 
#define AXIS2_LOG_ERROR axis2_log_impl_log_error 
#define AXIS2_LOG_CRITICAL axis2_log_impl_log_critical 

#ifdef AXIS2_TRACE
#define AXIS2_LOG_TRACE axis2_log_impl_log_trace
#else
  #ifdef WIN32
  #define AXIS2_LOG_TRACE axis2_log_impl_log_trace
  #else
  #define AXIS2_LOG_TRACE(params, args ...)
  #endif
#endif

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_LOG_H */




