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
    * \brief Axis2 error ops struct
    *
    * Encapsulator struct for ops of axis2_error
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
         axis2_char_t * (AXIS2_CALL *get_message) (struct axis2_error *error);
		
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
        /** error related ops */
        struct axis2_error_ops *ops;
        /** last error number */
        int error_number;

        int status_code;
    } axis2_error_t;

AXIS2_DECLARE(axis2_status_t) axis2_error_init();

#define AXIS2_ERROR_FREE(error) ((error->ops)->free(error))

#define AXIS2_ERROR_GET_MESSAGE(error) \
    ((error)->ops->get_message(error))

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
        /** No error. 
            This must be the first error all the time and the assigned value of 0 
            must not be changed as it is assumed in the error message array that 
            the error list starts with a value of 0.
            Further, none of the error codes in this enum should not be initialized 
            to an arbitrary value as it is assumed in the implementation when mapping 
            error codes to error messages that the error codes are contiguous and 
            the last error value is always AXIS2_ERROR_LAST.
          */
        AXIS2_ERROR_NONE = 0,
        /** Out of memory */
        AXIS2_ERROR_NO_MEMORY,
        /** NULL paramater was passed when a non NULL parameter was expected */
        AXIS2_ERROR_INVALID_NULL_PARAM,
        /** Iterator state invalid e.g. next called before calling first */
        AXIS2_ERROR_INVALID_ITERATOR_STATE,
        /** Node type is different from what is expected */
        AXIS2_ERROR_INVALID_NODE_TYPE,
        /** axis2_xml_reader returned NULL element */
        AXIS2_ERROR_XML_READER_ELEMENT_NULL,
        /** axis2_xml_reader returned NULL value */
        AXIS2_ERROR_XML_READER_VALUE_NULL,
        /** Builder done with pulling. Cannot pull any more */
        AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL,
        /** Bulder's last node is NULL when it is not supposed to be NULL */
        AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL,
        /** A document not associated with builder */
        AXIS2_ERROR_BUILDER_NOT_ASSOCIATED_WITH_DOCUMENT,
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
        /** Parameter not set */
        AXIS2_ERROR_INVALID_STATE_PARAM,
        /** Operation accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_OP,
        /** Service accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_SVC,
        /** Wsdl Service accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_WSDL_SVC,
        /** Service group accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_SVC_GRP,
        /** Wsdl op accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_WSDL_OP,
        /** Module description accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_MODULE_DESC,
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
        AXIS2_ERROR_INVALID_HANDLER_STATE,
        /** No such element */
        AXIS2_ERROR_NO_SUCH_ELEMENT,
        /** Could not Map the MEP URI to a axis MEP constant value */
        AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT,
        /** module has alredy engaged to the op op terminated !!! */
        AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP,
        /** module has alredy been engaged on the service.Operation terminated !!! */
        AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC,
        /** module has alredy been engaged on the service. Group Operation terminated !!! */
        AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC_GRP,
        /** Invalid message context state  */
        AXIS2_ERROR_INVALID_STATE_MSG_CTX,
        /** Invalid engine config state */
        AXIS2_ERROR_INVALID_STATE_CONF,
        /** Service not yet found */
        AXIS2_ERROR_SERVICE_NOT_YET_FOUND,
        /** error occured creating xml stream writer */
        AXIS2_ERROR_CREATING_XML_STREAM_WRITER,
        /** error occured creating xml stream writer */
        AXIS2_ERROR_CREATING_XML_STREAM_READER,
        
        AXIS2_ERROR_INVALID_MODULE_REF,
        
        AXIS2_ERROR_PHASE_IS_NOT_SPECIFED,
        
        AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE,
        
        AXIS2_ERROR_INVALID_PHASE,
        
        AXIS2_ERROR_WRITING_START_ELEMENT,
    
        AXIS2_ERROR_WRITING_START_DOCUMENT,
        
        AXIS2_ERROR_WRITING_END_ELEMENT,
        
        AXIS2_ERROR_WRITING_END_DOCUMENT,
        
        AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE,
        
        AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX,
        
        AXIS2_ERROR_WRITING_EMPTY_ELEMENT,
        
        AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE,
        
        AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX,
        
        AXIS2_ERROR_WRITING_ATTRIBUTE,
        
        AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE,
        
        AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX,
        
        AXIS2_ERROR_WRITING_COMMENT,
        
        AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION,
        
        AXIS2_ERROR_WTRING_CDATA,
        
        AXIS2_ERROR_WRITING_DTD,
        
        AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE,
        /**Two service can not have same name, a service with same name alredy 
          * exist in the system
          */
        AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME,
        /** Message context processing a fault already */
        AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY,
        /** fault to field not specified in message context */
        AXIS2_ERROR_NOWHERE_TO_SEND_FAULT,
        /* Invalid status line or invalid request line */
        AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE,
        /* Cannot correlate message*/
        AXIS2_ERROR_CANNOT_CORRELATE_MSG,
        AXIS2_ERROR_INVALID_STATE_DESC_BUILDER,
        AXIS2_ERROR_CONF_NOT_FOUND,
        AXIS2_ERROR_MODULE_NOT_FOUND,
        AXIS2_ERROR_DLL_CREATE_FAILED,
        AXIS2_ERROR_DLL_LOADING_FAILED,
        AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED,
        AXIS2_ERROR_HANDLER_CREATION_FAILED,
        AXIS2_ERROR_MSG_RECV_CREATION_FAILED,
        /* Operation cannot be NULL in MEP client */
        AXIS2_ERROR_OPERATION_CANNOT_BE_NULL_IN_MEP_CLIENT,
        /* MEP cannot be NULL in MEP client */
        AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT,
        /* MEP Mismatch */
        AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT,
        /** cannot infer transport from URL */
        AXIS2_ERROR_CANNOT_INFER_TRANSPORT,
        /** Invalid SOAP version */
        AXIS2_ERROR_INVALID_SOAP_VERSION,
        /* HTTP version cannot be null in the status/request line */ 
        AXIS2_ERROR_NULL_HTTP_VERSION,
        /* A valid conf_ctx is reqd for the http worker */
        AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
        /* We need transport_info in msg_ctx*/
        AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL,
        /** invalid type passed */
        AXIS2_ERROR_INVALID_BASE_TYPE,
        /** Response timed out */
        AXIS2_ERROR_RESPONSE_TIMED_OUT,
        /** Blocking invocation expects response */
        AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE,
        /** Client side support only one configuration context */
        AXIS2_ERROR_CLIENT_SIDE_SUPPORT_ONLY_ONE_CONF_CTX,
        /** Unknown Transport */
        AXIS2_ERROR_UNKNOWN_TRANSPORT,
        /** Two way channel needs addressing module to be engaged */
        AXIS2_ERROR_TWO_WAY_CHANNEL_NEEDS_ADDRESSING,
        /** om element is expected */
        AXIS2_ERROR_OM_ELEMENT_EXPECTED,

        AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE,
        /** SOAP envelope or SOAP body NULL */
        AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
        /* must understatnd attribute should have values of true or false */
        AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE,
        
        AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND,
        /** only one SOAP fault allowed in SOAP body */
        AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY,
		/** Error in creating the stream for the socket */
		AXIS2_ERROR_SOCKET_STREAM_CREATION,
		/** Trying to do operation on closed/not opened socket */
		AXIS2_ERROR_INVALID_SOCKET,
		/** Trying to do operation on invalid file descriptor */
		AXIS2_ERROR_INVALID_FD,
		/** Error creating a socket. Most probably error returned by OS */
		AXIS2_ERROR_SOCKET_ERROR,
		/** Invalid IP or hostname */
		AXIS2_ERROR_INVALID_ADDRESS,
		/** Socket bind failed. Another process may be already using this port*/
		AXIS2_ERROR_SOCKET_BIND_FAILED,
		/** URL NULL in http client */
		AXIS2_ERROR_NULL_URL,
        /** A read attempt(HTTP) for the reply without sending the request */
        AXIS2_ERROR_HTTP_REQUEST_NOT_SENT,
        /** Invalid string passed as a http header */
        AXIS2_ERROR_INVALID_HEADER,
        /** No body present in the request or the response */
        AXIS2_ERROR_NULL_BODY,
        /** Invalid SOAP envelope state */
        AXIS2_ERROR_INVALID_SOAP_ENVELOPE_STATE,
        /** Service xml file is not found in the given path */
        AXIS2_ERROR_SERVICE_XML_NOT_FOUND,
        /** Module xml file is not found in the given path */
        AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE,
        
        AXIS2_ERROR_OPERATION_NAME_MISSING,
        
        AXIS2_ERROR_SVC_NAME_ERROR,
        
        AXIS2_ERROR_OP_NAME_MISSING,
        
        AXIS2_ERROR_OUTFLOW_NOT_ALLOWED_IN_TRS_IN,
        
        AXIS2_ERROR_INFLOW_NOT_ALLOWED_IN_TRS_OUT,
        
        AXIS2_ERROR_NO_DISPATCHER_FOUND,
        AXIS2_ERROR_TRANSPORT_SENDER_ERROR,
        AXIS2_ERROR_REPO_CAN_NOT_BE_NULL,
        AXIS2_ERROR_REPOSITORY_NOT_EXIST,
        AXIS2_ERROR_CONFIG_NOT_FOUND,
        AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL,
        AXIS2_ERROR_MODULE_VALIDATION_FAILED,
        AXI2_ERROR_INVALID_PHASE,
        AXIS2_ERROR_INVALID_MODUELE_REF,
        AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP,
        AXIS2_ERROR_INVALID_SVC,
        AXIS2_ERROR_INVALID_MODULE,
        AXIS2_ERROR_MODULE_CREATION_FAILED,
        AXIS2_ERROR_SOAP_ENVELOPE_MUST_HAVE_BODY_ELEMENT,

        AXIS2_ERROR_INVALID_MODULE_CONF,
        
        AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME,
        
        AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED,
        
        AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER,
        
        AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED,
        
        AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY,

        /* Invalid messge addition , operation context completed */
        AXIS2_ERROR_INVALID_MESSAGE_ADDITION,
        
        AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI,
        
        AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE,
       
        AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP,
        
        /* Error in writing the response in response writer */
        AXIS2_ERROR_WRITING_RESPONSE,
        
        AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED,
        
        AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED,
        
        AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED,
        
        AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE,
        
        AXIS2_ERROR_SOAP_FAULT_SUB_CODE_DOES_NOT_HAVE_A_VALUE,
        
        AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED,
        
        AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT,
        
        AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED,
        
        AXIS2_ERROR_SOAP_FAULT_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT,
        
        AXIS2_ERROR_MULTIPLE_DETAIL_ELEMENTS_ENCOUNTERED,
        
        AXIS2_ERROR_UNSUPPORTED_ELEMENT_IN_SOAP_FAULT_ELEMENT,
        
        AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED,
        
        AXIS2_ERROR_MULTIPLE_VALUE_ENCOUNTERED_IN_CODE_ELEMENT,
        
        AXIS2_ERROR_SOAP_FAULT_VALUE_SHOULD_BE_PRESENT_BEFORE_SUB_CODE,
        
        AXIS2_ERROR_THIS_LOCALNAME_NOT_SUPPORTED_INSIDE_THE_CODE_ELEMENT,
        
        AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_REASON_ELEMENT,
        
        AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_SUB_CODE_ELEMENT,

        AXIS2_ERROR_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT,
        
        AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS,
    
        /* OM output is NULL */
        AXIS2_ERROR_NULL_OM_OUTPUT,
        /* We got a NULL strem in the response body */
        AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY,
        AXIS2_ERROR_OM_ELEMENT_MISMATCH,
        AXIS2_ERROR_OM_ELEMENT_INVALID_STATE,
        AXIS2_ERROR_RPC_NEED_MATCHING_CHILD,
        AXIS2_ERROR_UNKNOWN_STYLE,
        /* processing soap11 fault value element should have only
           text as its children */
        AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE,

        AXIS2_ERROR_TRANSPORT_RECV_CREATION_FAILED,
        AXIS2_ERROR_TRANSPORT_SENDER_CREATION_FAILED,
        AXIS2_ERROR_NO_TRANSPORT_IN_CONFIGURED,
        AXIS2_ERROR_NO_TRANSPORT_OUT_CONFIGURED,
        /* Listen failed for the server socket */
        AXIS2_ERROR_SOCKET_LISTEN_FAILED,
        AXIS2_ERROR_FILE_NAME_NOT_SET,
        AXIS2_ERROR_REPOS_LISTENER_INIT_FAILED,
        AXIS2_ERROR_INVALID_STATE_DLL_DESC,
        AXIS2_ERROR_UNSUPPORTED_TYPE,
        /* Null soap envelope in msg_ctx */
        AXIS2_ERROR_NULL_SOAP_ENVELOPE_IN_MSG_CTX,
        /* Generation of platform dependent uuid failed */
        AXIS2_ERROR_UUID_GEN_FAILED,
        /** Invalid XML format in request */
        AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST,
        /** Input OM node NULL, Probably error in SOAP request */
        AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL,
        /** Invalid parameters for service operation in SOAP request */
        AXIS2_ERROR_SVC_SKEL_INVALID_OPERATION_PARAMETERS_IN_SOAP_REQUEST,
        /* Input stream is NULL in msg_ctx */
        AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX,
        /* NULL stream in the http chunked stream */
        AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM,
        /* Flow type of PhaseOrder element cannot be null */
        AXIS2_ERROR_FLOW_TYPE_CANNOT_BE_NULL,
        /* Data element of the OM Node is null */
        AXIS2_ERROR_DATA_ELEMENT_IS_NULL,

        /** The following has to be the last error value all the time.
            All other error codes should appear above this.
            AXIS2_ERROR_LAST is used to track the number of error codes present
            for the purpose of sizing the error message array.
          */
        AXIS2_ERROR_LAST
    };

/** @} */
    
#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_ERROR_H */
