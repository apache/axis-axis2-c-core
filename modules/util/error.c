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

#include <stdlib.h>
#include "axis2_error_default.h"

axis2_char_t * AXIS2_CALL axis2_error_impl_get_message (axis2_error_t *error);
		
axis2_status_t AXIS2_CALL
axis2_error_impl_set_error_number (axis2_error_t *error, axis2_error_codes_t error_number);

axis2_status_t AXIS2_CALL
axis2_error_impl_set_status_code (axis2_error_t *error, axis2_status_codes_t status_code);

axis2_status_t AXIS2_CALL
axis2_error_impl_get_status_code (axis2_error_t *error);

/* array to hold error messages */
axis2_char_t* axis2_error_messages[AXIS2_ERROR_LAST];

axis2_status_t AXIS2_CALL
axis2_error_init()
{
    int i = 0;
    for (i = 0; i < AXIS2_ERROR_LAST; i++)
    {
        axis2_error_messages[i] = "Unknown Error :(";
    }
    
    axis2_error_messages[AXIS2_ERROR_NONE] = "No Error";
    axis2_error_messages[AXIS2_ERROR_NO_MEMORY] = "Out of memory";
    axis2_error_messages[AXIS2_ERROR_INVALID_NULL_PARAM] = 
        "NULL paramater was passed when a non NULL parameter was expected";
    axis2_error_messages[AXIS2_ERROR_INVALID_ITERATOR_STATE] = 
        "Iterator state invalid e.g. next called before calling first";
    axis2_error_messages[AXIS2_ERROR_INVALID_NODE_TYPE] = 
        "Node type is different from what is expected";
    axis2_error_messages[AXIS2_ERROR_XML_READER_ELEMENT_NULL] = 
        "axis2_xml_reader returned NULL element";
    axis2_error_messages[AXIS2_ERROR_XML_READER_VALUE_NULL] = 
        "axis2_xml_reader returned NULL value";
    axis2_error_messages[AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL] = 
        "Builder done with pulling. Cannot pull any more";
    axis2_error_messages[AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL] = 
        "Bulder's last node is NULL when it is not supposed to be NULL";
    axis2_error_messages[AXIS2_ERROR_BUILDER_NOT_ASSOCIATED_WITH_DOCUMENT] = 
        "A document not associated with builder";
    axis2_error_messages[AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD] = 
        "Discard faile because the builder state is invalid";
    axis2_error_messages[AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL] = 
        "Document root is NULL] =  when it is not supposed to be NULL";
    axis2_error_messages[AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE] = 
        "Undefined namespace used";
    axis2_error_messages[AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED] = 
        "Trying to release unallocated memory";
    axis2_error_messages[AXIS2_ERROR_STRUCT_NOT_INITIALIZED] = 
        "Struct is not properly initialized";
    axis2_error_messages[AXIS2_ERROR_COULD_NOT_OPEN_FILE] = 
        "Could not open the file";
    axis2_error_messages[AXIS2_ERROR_ENVIRONMENT_IS_NULL] = 
        "Environment passed is null";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PARENT_NULL] = 
        "Parent requested, but parent is null";
    axis2_error_messages[AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE] = 
        "Parameter locked, Cannot override";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER] = 
        "Parameter container not set";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PARAM] = 
        "Parameter not set";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_OP] = 
        "Operation accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_SVC] = 
        "Service accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_WSDL_SVC] = 
        "Wsdl Service accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_SVC_GRP] = 
        "Service group accessed has invalid state"; 
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_WSDL_OP] = 
        "Wsdl op accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_MODULE_DESC] = 
        "Module description accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INDEX_OUT_OF_BOUNDS] = 
        "Array list index out of bounds";
    axis2_error_messages[AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME] = 
        "Both before and after handlers cannot be the same";
    axis2_error_messages[AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET] = 
        "First handler of phase already set";
    axis2_error_messages[AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER] = 
        "Invalid first handler for phase";
    axis2_error_messages[AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET] = 
        "Last handler of phase already set";
    axis2_error_messages[AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER] = 
        "Invalid last handler for phase";
    axis2_error_messages[AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID] = 
        "Only one handler allowed for phase, adding another handler is not allowed";
    axis2_error_messages[AXIS2_ERROR_INVALID_HANDLER_RULES] = 
        "Invalid handler rules";
    axis2_error_messages[AXIS2_ERROR_INVALID_HANDLER_STATE] = 
        "Invalid handler state";
    axis2_error_messages[AXIS2_ERROR_NO_SUCH_ELEMENT] = 
        "No such element";
    axis2_error_messages[AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT] = 
        "Could not Map the MEP URI to a axis MEP constant value";
    axis2_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP] = 
        "module has alredy engaged to the op op terminated !!!";
    axis2_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC] = 
        "module has alredy been engaged on the service.Operation terminated !!!";
    axis2_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC_GRP] = 
        "module has alredy been engaged on the service. Group Operation terminated !!!";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_MSG_CTX] = 
        "Invalid message context state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_CONF] = 
        "Invalid engine config state";
    axis2_error_messages[AXIS2_ERROR_SERVICE_NOT_YET_FOUND] = 
        "Service not yet found";
    axis2_error_messages[AXIS2_ERROR_CREATING_XML_STREAM_WRITER] = 
        "error occured creating xml stream writer";
    axis2_error_messages[AXIS2_ERROR_CREATING_XML_STREAM_READER] = 
        "error occured creating xml stream writer";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODULE_REF] = 
        "AXIS2_ERROR_INVALID_MODULE_REF";
    axis2_error_messages[AXIS2_ERROR_PHASE_IS_NOT_SPECIFED] = 
        "AXIS2_ERROR_PHASE_IS_NOT_SPECIFED";
    axis2_error_messages[AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE] = 
        "AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE";
    axis2_error_messages[AXIS2_ERROR_INVALID_PHASE] = 
        "AXIS2_ERROR_INVALID_PHASE";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT] = 
        "AXIS2_ERROR_WRITING_START_ELEMENT";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_DOCUMENT] = 
        "AXIS2_ERROR_WRITING_START_DOCUMENT";
    axis2_error_messages[AXIS2_ERROR_WRITING_END_ELEMENT] = 
        "AXIS2_ERROR_WRITING_END_ELEMENT";
    axis2_error_messages[AXIS2_ERROR_WRITING_END_DOCUMENT] = 
        "AXIS2_ERROR_WRITING_END_DOCUMENT";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE] = 
        "AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX] = 
        "AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX";
    axis2_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT] = 
        "AXIS2_ERROR_WRITING_EMPTY_ELEMENT";
    axis2_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE] = 
        "AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE";
    axis2_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX] = 
        "AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX";
    axis2_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE] = 
        "AXIS2_ERROR_WRITING_ATTRIBUTE";
    axis2_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE] = 
        "AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE";
    axis2_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX] = 
        "AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX";
    axis2_error_messages[AXIS2_ERROR_WRITING_COMMENT] = 
        "AXIS2_ERROR_WRITING_COMMENT";
    axis2_error_messages[AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION] = 
        "AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION";
    axis2_error_messages[AXIS2_ERROR_WTRING_CDATA] = 
        "AXIS2_ERROR_WTRING_CDATA";
    axis2_error_messages[AXIS2_ERROR_WRITING_DTD] = 
        "AXIS2_ERROR_WRITING_DTD";
    axis2_error_messages[AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE] = 
        "AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE";
    axis2_error_messages[AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME] = 
        "Two service can not have same name, a service with same name alredy";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY] = 
        "Message context processing a fault already";
    axis2_error_messages[AXIS2_ERROR_NOWHERE_TO_SEND_FAULT] = 
        "fault to field not specified in message context";
    axis2_error_messages[AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE] = 
        "Invalid status line or invalid request line";
    axis2_error_messages[AXIS2_ERROR_CANNOT_CORRELATE_MSG] = 
        "Cannot correlate message";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_DESC_BUILDER] = 
        "AXIS2_ERROR_INVALID_STATE_DESC_BUILDER";
    axis2_error_messages[AXIS2_ERROR_CONF_NOT_FOUND] = 
        "AXIS2_ERROR_CONF_NOT_FOUND";
    axis2_error_messages[AXIS2_ERROR_MODULE_NOT_FOUND] = 
        "AXIS2_ERROR_MODULE_NOT_FOUND";
    axis2_error_messages[AXIS2_ERROR_DLL_CREATE_FAILED] = 
        "AXIS2_ERROR_DLL_CREATE_FAILED";
    axis2_error_messages[AXIS2_ERROR_DLL_LOADING_FAILED] = 
        "AXIS2_ERROR_DLL_LOADING_FAILED";
    axis2_error_messages[AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED] = 
        "AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED";
    axis2_error_messages[AXIS2_ERROR_HANDLER_CREATION_FAILED] = 
        "AXIS2_ERROR_HANDLER_CREATION_FAILED";
    axis2_error_messages[AXIS2_ERROR_MSG_RECV_CREATION_FAILED] = 
        "AXIS2_ERROR_MSG_RECV_CREATION_FAILED";
    axis2_error_messages[AXIS2_ERROR_OPERATION_CANNOT_BE_NULL_IN_MEP_CLIENT] = 
        "Operation cannot be NULL in MEP client";
    axis2_error_messages[AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT] = 
        "MEP cannot be NULL in MEP client";
    axis2_error_messages[AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT] = 
        "MEP Mismatch";
    axis2_error_messages[AXIS2_ERROR_CANNOT_INFER_TRANSPORT] = 
        "cannot infer transport from URL";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOAP_VERSION] = 
        "Invalid SOAP version";
    axis2_error_messages[AXIS2_ERROR_NULL_HTTP_VERSION] = 
        "HTTP version cannot be null in the status/request line";
    axis2_error_messages[AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT] = 
        "A valid conf_ctx is reqd for the http worker";
    axis2_error_messages[AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL] = 
        "We need transport_info in msg_ctx";
    axis2_error_messages[AXIS2_ERROR_INVALID_BASE_TYPE] = 
        "invalid type passed";
    axis2_error_messages[AXIS2_ERROR_RESPONSE_TIMED_OUT] = 
        "Response timed out";
    axis2_error_messages[AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE] = 
        "Blocking invocation expects response";
    axis2_error_messages[AXIS2_ERROR_CLIENT_SIDE_SUPPORT_ONLY_ONE_CONF_CTX] = 
        "AXIS2_ERROR_CLIENT_SIDE_SUPPORT_ONLY_ONE_CONF_CTX";
    axis2_error_messages[AXIS2_ERROR_UNKNOWN_TRANSPORT] = 
        "Unknown Transport";
    axis2_error_messages[AXIS2_ERROR_TWO_WAY_CHANNEL_NEEDS_ADDRESSING] = 
        "Two way channel needs addressing module to be engaged";
    axis2_error_messages[AXIS2_ERROR_OM_ELEMENT_EXPECTED] = 
        "om element is expected";
    axis2_error_messages[AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE] = 
        "AXIS2_ERROR_INVALID_HTTP_INVALID_HEADER_START_LINE";
    axis2_error_messages[AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL] = 
        "SOAP envelope or SOAP body NULL";
    axis2_error_messages[AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE] = 
        "AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE";
    axis2_error_messages[AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND] = 
        "AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND";
    axis2_error_messages[AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY] = 
        "only one SOAP fault allowed in SOAP body";
    axis2_error_messages[AXIS2_ERROR_SOCKET_STREAM_CREATION] = 
        "Error in creating the stream for the socket";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOCKET] = 
        "Trying to do operation on closed/not opened socket";
    axis2_error_messages[AXIS2_ERROR_INVALID_FD] = 
        "Trying to do operation on invalid file descriptor";
    axis2_error_messages[AXIS2_ERROR_SOCKET_ERROR] = 
        "Error creating a socket. Most probably error returned by OS";
    axis2_error_messages[AXIS2_ERROR_INVALID_ADDRESS] = 
        "Invalid IP or hostname";
    axis2_error_messages[AXIS2_ERROR_SOCKET_BIND_FAILED] = 
        "Socket bind failed. Another process may be already using this port";
    axis2_error_messages[AXIS2_ERROR_NULL_URL] = 
        "URL NULL in http client";
    axis2_error_messages[AXIS2_ERROR_HTTP_REQUEST_NOT_SENT] = 
        "A read attempt(HTTP) for the reply without sending the request";
    axis2_error_messages[AXIS2_ERROR_INVALID_HEADER] = 
        "Invalid string passed as a http header";
    axis2_error_messages[AXIS2_ERROR_NULL_BODY] = 
        "No body present in the request or the response";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOAP_ENVELOPE_STATE] = 
        "Invalid SOAP envelope state";
    axis2_error_messages[AXIS2_ERROR_SERVICE_XML_NOT_FOUND] = 
        "Service xml file is not found in the given path";
    axis2_error_messages[AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE] = 
        "Module xml file is not found in the given path";
    axis2_error_messages[AXIS2_ERROR_OPERATION_NAME_MISSING] = 
        "AXIS2_ERROR_OPERATION_NAME_MISSING";
    axis2_error_messages[AXIS2_ERROR_SVC_NAME_ERROR] = 
        "AXIS2_ERROR_SVC_NAME_ERROR";
    axis2_error_messages[AXIS2_ERROR_OP_NAME_MISSING] = 
        "AXIS2_ERROR_OP_NAME_MISSING";
    axis2_error_messages[AXIS2_ERROR_OUTFLOW_NOT_ALLOWED_IN_TRS_IN] = 
        "AXIS2_ERROR_OUTFLOW_NOT_ALLOWED_IN_TRS_IN";
    axis2_error_messages[AXIS2_ERROR_INFLOW_NOT_ALLOWED_IN_TRS_OUT] = 
        "AXIS2_ERROR_INFLOW_NOT_ALLOWED_IN_TRS_OUT";
    axis2_error_messages[AXIS2_ERROR_NO_DISPATCHER_FOUND] = 
        "AXIS2_ERROR_NO_DISPATCHER_FOUND";
    axis2_error_messages[AXIS2_ERROR_TRANSPORT_SENDER_ERROR] = 
        "AXIS2_ERROR_TRANSPORT_SENDER_ERROR";
    axis2_error_messages[AXIS2_ERROR_REPO_CAN_NOT_BE_NULL] = 
        "AXIS2_ERROR_REPO_CAN_NOT_BE_NULL";
    axis2_error_messages[AXIS2_ERROR_REPOSITORY_NOT_EXIST] = 
        "AXIS2_ERROR_REPOSITORY_NOT_EXIST";
    axis2_error_messages[AXIS2_ERROR_CONFIG_NOT_FOUND] = 
        "AXIS2_ERROR_CONFIG_NOT_FOUND";
    axis2_error_messages[AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL] = 
        "AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL";
    axis2_error_messages[AXIS2_ERROR_MODULE_VAL_FAILED] = 
        "AXIS2_ERROR_MODULE_VAL_FAILED";
    axis2_error_messages[AXI2_ERROR_IN_VALID_PHASE] = 
        "AXI2_ERROR_IN_VALID_PHASE";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODUELE_REF] = 
        "AXIS2_ERROR_INVALID_MODUELE_REF";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP] = 
        "AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP";
    axis2_error_messages[AXIS2_ERROR_INVALID_SVC] = 
        "AXIS2_ERROR_INVALID_SVC";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODULE] = 
        "AXIS2_ERROR_INVALID_MODULE";
    axis2_error_messages[AXIS2_ERROR_MODULE_CREATION_FAILED] = 
        "AXIS2_ERROR_MODULE_CREATION_FAILED";
    axis2_error_messages[AXIS2_ERROR_SOAP_ENVELOPE_MUST_HAVE_BODY_ELEMENT] = 
        "AXIS2_ERROR_SOAP_ENVELOPE_MUST_HAVE_BODY_ELEMENT";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODULE_CONF] = 
        "AXIS2_ERROR_INVALID_MODULE_CONF";
    axis2_error_messages[AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME] = 
        "AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED] = 
        "AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER] = 
        "AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED] = 
        "AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY] = 
        "AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY";
    axis2_error_messages[AXIS2_ERROR_INVALID_MESSAGE_ADDITION] = 
        "Invalid messge addition operation context completed";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI] = 
        "AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI";
    axis2_error_messages[AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE] = 
        "AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE";
    axis2_error_messages[AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP] = 
        "AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP";
    axis2_error_messages[AXIS2_ERROR_WRITING_RESPONSE] = 
        "Error in writing the response in response writer";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED] = 
        "AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED] = 
        "AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED";
    axis2_error_messages[AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED] = 
        "AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED";
    axis2_error_messages[AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE] =
        "AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE";
    axis2_error_messages[AXIS2_ERROR_SOAP_FAULT_SUB_CODE_DOES_NOT_HAVE_A_VALUE] = 
        "AXIS2_ERROR_SOAP_FAULT_SUB_CODE_DOES_NOT_HAVE_A_VALUE";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED] =
        "AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED";
    /*   
    axis2_error_messages[AXIS2_ERROR_REASON_ELEMENT_SHOULD_HAVE_A_TEXT] = 
        "AXIS2_ERROR_REASON_ELEMENT_SHOULD_HAVE_A_TEXT";
    */
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED] = 
        "AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED";
    axis2_error_messages[AXIS2_ERROR_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT] = 
        "AXIS2_ERROR_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT";
    axis2_error_messages[AXIS2_ERROR_NULL_OM_OUTPUT] = 
        "OM output is NULL";
    axis2_error_messages[AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY] = 
        "We got a NULL strem in the response body";
}

axis2_status_t AXIS2_CALL
axis2_error_impl_free (axis2_error_t *error)
{
    if (NULL != error && NULL != error->ops)
    {
        free (error->ops);
    }
    if (NULL != error)
    {
        free (error); 
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_error_t*)
axis2_error_create (axis2_allocator_t * allocator)
{
    axis2_error_t *error;
    if (!allocator)
        return NULL;

    error =
        (axis2_error_t *) AXIS2_MALLOC (allocator, sizeof (axis2_error_t));

    if (!error)
        return NULL;

    error->ops =
        (axis2_error_ops_t *) AXIS2_MALLOC (allocator,
                                            sizeof (axis2_error_ops_t));

    if (!error->ops)
    {
        AXIS2_FREE (allocator, error);
        return NULL;
    }

    error->ops->get_message = axis2_error_impl_get_message;
	error->ops->set_error_number = axis2_error_impl_set_error_number;
	error->ops->set_status_code = axis2_error_impl_set_status_code;
	error->ops->get_status_code = axis2_error_impl_get_status_code;
    error->ops->free            = axis2_error_impl_free;

    return error;
}

axis2_char_t * AXIS2_CALL
axis2_error_impl_get_message (axis2_error_t *error)
{
    if (error && error->error_number >= AXIS2_ERROR_NONE && error->error_number < AXIS2_ERROR_LAST)
        return axis2_error_messages[error->error_number];
    
    return "Invalid Error Number";
}

axis2_status_t AXIS2_CALL
axis2_error_impl_set_error_number (axis2_error_t *error, axis2_error_codes_t error_number)
{
    error->error_number = error_number; 
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_error_impl_set_status_code (axis2_error_t *error, axis2_status_codes_t status_code)
{
    error->status_code = status_code; 
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_error_impl_get_status_code (axis2_error_t *error)
{
    return error->status_code;
}
