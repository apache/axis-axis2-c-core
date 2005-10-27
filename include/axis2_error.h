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

#ifndef AXIS2_ERROR_H
#define AXIS2_ERROR_H

#include <axis2_defines.h>
#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_error;
    struct axis2_error_ops;

/**
 * @defgroup axis2_error Error
 * @ingroup axis2_util 
 * @{
 */

  /** 
    * \brief Axis2 error operations struct
    *
    * Encapsulator struct for operations of axis2_error
    */
   AXIS2_DECLARE_DATA typedef struct axis2_error_ops
    {
      /**
        * get error message for the last error
        * @return error message for the last error. NULL on error.
        */
         axis2_char_t * (AXIS2_CALL *axis2_error_ops_get_message) ();
    } axis2_error_ops_t;

  /** 
    * \brief Axis2 Error struct
    *
    * Error holds the last errorno
    */
    typedef struct axis2_error
    {
        /** error related operations */
        struct axis2_error_ops *ops;
        /** last error number */
        int errorno;
    } axis2_error_t;

  /**
    * Creates an error struct
    * @param allocator allocator to be used. Mandatory, cannot be NULL    
    * @return pointer to the newly created error struct 
    */
    AXIS2_DECLARE(axis2_error_t *) axis2_error_create (axis2_allocator_t * allocator);

#define axis2_error_get_message(error) ((error)->ops->axis2_error_ops_get_message())

  /** 
    * \brief Axis2 status codes
    *
    * Possible status values for Axis2
    */
    typedef enum axis2_status_codes
    {
        /** Failure state */
        AXIS2_FAILURE = 0,
        /** Success state */
        AXIS2_SUCCESS
    } axis2_status_codes_t;

  /** 
    * \brief Axis2 error codes
    *
    * Set of error codes for Axis2
    */
    typedef enum axis2_error_codes
    {
        /** No error */
        AXIS2_ERROR_NONE = 0,
        /** Out of memory */
        AXIS2_ERROR_NO_MEMORY,
        /** NULL paramater was passed when a non NULL parameter was expected */
        AXIS2_ERROR_INVALID_NULL_PARAMETER,
        /** Iterator state invalid e.g. next called before calling first */
        AXIS2_ERROR_INVALID_ITERATOR_STATE,
        /** Node type is different from what is expected */
        AXIS2_ERROR_INVALID_NODE_TYPE,
        /** Pull parser returned NULL element */
        AXIS2_ERROR_PULL_PARSER_ELEMENT_NULL,
        /** Pull parser returned NULL value */
        AXIS2_ERROR_PULL_PARSER_VALUE_NULL,
        /** Builder done with pulling. Cannot pull any more */
        AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL,
        /** Bulder's last node is NULL when it is not supposed to be NULL */
        AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL,
        /** Discard faile because the builder state is invalid */
        AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD,
        /** Document root is NULL, when it is not supposed to be NULL */
        AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL,
        /** Undefined namespace used */
        AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE,
        /** Trying to release unallocated memory */
        AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED,
		/** Struct is not properly initialized */
		AXIS2_ERROR_STRUCT_NOT_INITIALIZED,
		/** Could not open the file */
		AXIS2_ERROR_COULD_NOT_OPEN_FILE
    } axis2_error_codes_t;

/** @} */
    
#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_ERROR_H */
