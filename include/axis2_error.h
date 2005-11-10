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
	typedef enum axis2_status_codes axis2_status_codes_t;
	typedef enum axis2_error_codes axis2_error_codes_t;

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
        * deallocate memory of a error struct
        * @return axis2_status_t status code
        */

        axis2_status_t (AXIS2_CALL *free)(struct axis2_error *error);
        
      /**
        * get error message for the last error
        * @return error message for the last error. NULL on error.
        */
         axis2_char_t * (AXIS2_CALL *get_message) (struct axis2_error *error
				, axis2_error_codes_t error_number);
		
		 axis2_status_t  (AXIS2_CALL *set_error_number) (struct axis2_error *error
				,  axis2_error_codes_t error_number);
		
         axis2_status_t  (AXIS2_CALL *set_status_code) (struct axis2_error *error
				, axis2_status_codes_t status_code);
		
         axis2_status_t  (AXIS2_CALL *get_status_code) (struct axis2_error *error);
			 
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
        int error_number;

        int status_code;
    } axis2_error_t;


#define AXIS2_ERROR_FREE(error) ((error->ops)->free(error))

#define AXIS2_ERROR_GET_MESSAGE(error, status_code) ((error)->ops->get_message \
        (error, status_code))

#define AXIS2_ERROR_SET_ERROR_NUMBER(error, error_number) \
        ((error)->ops->set_error_number(error, error_number))
	
#define AXIS2_ERROR_SET_STATUS_CODE(error, status_code) \
        ((error)->ops->set_status_code(error, status_code))
        
#define AXIS2_ERROR_GET_STATUS_CODE(error) ((error)->ops->get_status_code(error))

  /** 
    * \brief Axis2 status codes
    *
    * Possible status values for Axis2
    */
    enum axis2_status_codes
    {
        /** Critical Failure state */
        AXIS2_CRTICAL_FAILURE = -1,
        /** Failure state */
        AXIS2_FAILURE,
        /** Success state */
        AXIS2_SUCCESS
    };

  /** 
    * \brief Axis2 error codes
    *
    * Set of error codes for Axis2
    */
    enum axis2_error_codes
    {
        /** No error */
        AXIS2_ERROR_NONE = 0,
        /** Out of memory */
        AXIS2_ERROR_NO_MEMORY,
        /** NULL paramater was passed when a non NULL parameter was expected */
        AXIS2_ERROR_INVALID_NULL_PARAM,
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
		AXIS2_ERROR_COULD_NOT_OPEN_FILE,
		/** Environment passed is null */
		AXIS2_ERROR_ENVIRONMENT_IS_NULL,
        /** Parent requested, but parent is null */
        AXIS2_ERROR_INVALID_STATE_PARENT_NULL,
        /** Parameter locked, Cannot override */
        AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
        /** Parameter container not set */
        AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER,
        /** Operation accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_OPERATION,
        /** Service accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_SVC,
        /** Service group accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_SVC_GRP,
        /** Wsdl operation accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_WSDL_OPERATION,
        /** Array list index out of bounds */
        AXIS2_ERROR_INDEX_OUT_OF_BOUNDS,
        /** Both before and after handlers cannot be the same */
        AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME,
        /** First handler of phase already set */
        AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET,
        /** Invalid first handler for phase  */
        AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER,
        /** Last handler of phase already set */
        AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET,
        /** Invalid last handler for phase  */
        AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER,
        /** Only one handler allowed for phase, adding handler is not allowed  */
        AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID,
        /** Invalid handler rules  */
        AXIS2_ERROR_INVALID_HANDLER_RULES,
        /** Invalid handler state */
        AXIS2_ERROR_INVALID_HANDLER_STATE
    };

/** @} */
    
#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_ERROR_H */
