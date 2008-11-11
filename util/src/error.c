/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
#include <string.h>
#include "axutil_error_default.h"

#define AXIS2_ERROR_MESSAGE_ARRAY_SIZE AXUTIL_ERROR_MAX


/**
 * Array to hold error messages corresponding to the pre-defined error codes.
 * Note that array has capacity for additional error messages. These are 
 * reserved for modules.
 *
 * TODO: We have to review and change the following definition and also 
 * need to come up with a reserve strategy for module error code blocks.
 *
 * In writing a module following steps must be followed in extending Axis2/C
 * errors
 * 1. Declare and register the start of error messages for the new
 *    module. 
 * 2. New module can use up to 1000 messages for its errors.
 * 3. In axis2c documentation an entry about new modules error range must
 *    be inserted so that another new module can know about the already
 *    occupied spaces. 
 */

AXIS2_EXPORT const axis2_char_t*
axutil_error_messages[AXIS2_ERROR_MESSAGE_ARRAY_SIZE];

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_error_init()
{
    int i = 0;
    for (i = 0; i < AXIS2_ERROR_MESSAGE_ARRAY_SIZE ; i++)
    {
        axutil_error_messages[i] = "Unknown Error :(";
    }

    /* Common Errors */
    axutil_error_messages[AXIS2_ERROR_NONE] = "No Error";
    axutil_error_messages[AXIS2_ERROR_NO_MEMORY] = "Out of memory";
    axutil_error_messages[AXIS2_ERROR_INVALID_NULL_PARAM] =
        "NULL parameter was passed when a non NULL parameter was expected";
    /* core:addr */

    /* core:clientapi */
    axutil_error_messages[AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE] =
        "Blocking invocation expects response";
    axutil_error_messages[AXIS2_ERROR_CANNOT_INFER_TRANSPORT] =
        "Cannot infer transport from URL";
    axutil_error_messages[AXIS2_ERROR_CLIENT_SIDE_SUPPORT_ONLY_ONE_CONF_CTX] =
        "Client side support only one configuration context";
    axutil_error_messages[AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT] =
        "MEP cannot be NULL in MEP client";
    axutil_error_messages[AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT] =
        "MEP mismatch";
    axutil_error_messages[AXIS2_ERROR_TWO_WAY_CHANNEL_NEEDS_ADDRESSING] =
        "Two way channel needs addressing module to be engaged";
    axutil_error_messages[AXIS2_ERROR_UNKNOWN_TRANSPORT] = "Unknown transport";
    axutil_error_messages[AXIS2_ERROR_UNSUPPORTED_TYPE] =
        "Type is not supported";
    axutil_error_messages[AXIS2_ERROR_OPTIONS_OBJECT_IS_NOT_SET] =
        "Options object is not set";
    /* core:clientapi:diclient */

    /* core:context */
    axutil_error_messages[AXIS2_ERROR_INVALID_SOAP_ENVELOPE_STATE] =
        "Invalid SOAP envelope state";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_MSG_CTX] =
        "Invalid message context state";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_SVC] =
        "Service accessed has invalid state";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_SVC_GRP] =
        "Service group accessed has invalid state";
    axutil_error_messages[AXIS2_ERROR_SERVICE_NOT_YET_FOUND] =
        "Service not yet found";
    /* core:deployment */
    axutil_error_messages[AXI2_ERROR_INVALID_PHASE] =
        "Invalid phase found in phase validation*";
    axutil_error_messages[AXIS2_ERROR_CONFIG_NOT_FOUND] =
        "Configuration file cannot be found";
    axutil_error_messages[AXIS2_ERROR_DATA_ELEMENT_IS_NULL] =
        "Data element of the OM Node is NULL";
    axutil_error_messages[AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT] =
        "In transport sender, Inflow is not allowed";
    axutil_error_messages[AXIS2_ERROR_INVALID_HANDLER_STATE] =
        "Invalid handler state";
    axutil_error_messages[AXIS2_ERROR_INVALID_MODUELE_REF] =
        "Invalid module reference encountered";
    axutil_error_messages[AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP] =
        "Invalid module referenced by operation";
    axutil_error_messages[AXIS2_ERROR_INVALID_MODULE_CONF] =
        "Invalid module configuration";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_DESC_BUILDER] =
        "Description builder is found to be in invalid state";
    axutil_error_messages[AXIS2_ERROR_MODULE_NOT_FOUND] = "Module not found";
    axutil_error_messages[AXIS2_ERROR_MODULE_VALIDATION_FAILED] =
        "Module validation failed";
    axutil_error_messages[AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE] =
        "Module XML file is not found in the given path";
    axutil_error_messages[AXIS2_ERROR_NO_DISPATCHER_FOUND] =
        "No dispatcher found";
    axutil_error_messages[AXIS2_ERROR_OP_NAME_MISSING] =
        "Operation name is missing";
    axutil_error_messages[AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN] =
        "In transport receiver, outflow is not allowed";
    axutil_error_messages[AXIS2_ERROR_REPO_CAN_NOT_BE_NULL] =
        "Repository name cannot be NULL";
    axutil_error_messages[AXIS2_ERROR_REPOSITORY_NOT_EXIST] =
        "Repository in path does not exist";
    axutil_error_messages[AXIS2_ERROR_REPOS_LISTENER_INIT_FAILED] =
        "Repository listener initialization failed";
    axutil_error_messages[AXIS2_ERROR_SERVICE_XML_NOT_FOUND] =
        "Service XML file is not found in the given path";
    axutil_error_messages[AXIS2_ERROR_SVC_NAME_ERROR] = "Service name error";
    axutil_error_messages[AXIS2_ERROR_TRANSPORT_SENDER_ERROR] =
        "Transport sender error";
    axutil_error_messages[AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL] =
        "Path to configuration file can not be NULL";
    axutil_error_messages[AXIS2_ERROR_INVALID_SVC] = "Invalid service";
    /* core:description */
    axutil_error_messages[AXIS2_ERROR_CANNOT_CORRELATE_MSG] =
        "Cannot correlate message";
    axutil_error_messages[AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT] =
        "Could not map the MEP URI to an Axis2/C MEP constant value";
    axutil_error_messages[AXIS2_ERROR_INVALID_MESSAGE_ADDITION] =
        "Invalid message addition operation context completed";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_MODULE_DESC] =
        "Module description accessed has invalid state";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER] =
        "Parameter container not set";
    axutil_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP] =
        "Module has already been engaged to the operation";
    axutil_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC] =
        "Module has already been engaged on the service.";
    axutil_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC_GRP] =
        "Module has already been engaged on the service.";
    axutil_error_messages[AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE] =
        "Parameter locked, cannot override";
    axutil_error_messages[AXIS2_ERROR_EMPTY_SCHEMA_LIST] =
        "Schema list is empty or NULL in service";
    /* core:engine */
    axutil_error_messages[AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME] =
        "Both before and after handlers cannot be the same";
    axutil_error_messages[AXIS2_ERROR_INVALID_HANDLER_RULES] =
        "Invalid handler rules";
    axutil_error_messages[AXIS2_ERROR_INVALID_MODULE] = "Invalid module";
    axutil_error_messages[AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER] =
        "Invalid first handler for phase";
    axutil_error_messages[AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER] =
        "Invalid last handler for phase";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_CONF] =
        "Invalid engine configuration state";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY] =
        "Message context processing a fault already";
    axutil_error_messages[AXIS2_ERROR_NOWHERE_TO_SEND_FAULT] =
        "fault to field not specified in message context";
    axutil_error_messages[AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID] =
        "Only one handler allowed for phase, adding another handler is not allowed";
    axutil_error_messages[AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET] =
        "First handler of phase already set";
    axutil_error_messages[AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET] =
        "Last handler of phase already set";
    axutil_error_messages[AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME] =
        "Two service can not have same name, a service with same name already";
    /* core:phaseresolver */
    axutil_error_messages[AXIS2_ERROR_INVALID_MODULE_REF] =
        "Invalid module reference";
    axutil_error_messages[AXIS2_ERROR_INVALID_PHASE] = "Invalid Phase";
    axutil_error_messages[AXIS2_ERROR_NO_TRANSPORT_IN_CONFIGURED] =
        "There are no in transport chains configured";
    axutil_error_messages[AXIS2_ERROR_NO_TRANSPORT_OUT_CONFIGURED] =
        "There are no out transport chains configured";
    axutil_error_messages[AXIS2_ERROR_PHASE_IS_NOT_SPECIFED] =
        "Phase is not specified";
    axutil_error_messages[AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE]
        = "Service module can not refer global phase";
    /* core:wsdl */
    axutil_error_messages[AXIS2_ERROR_WSDL_SCHEMA_IS_NULL] = "Schema is NULL";
    /* core:receivers */
    axutil_error_messages[AXIS2_ERROR_OM_ELEMENT_INVALID_STATE] =
        "AXIOM element has invalid state";
    axutil_error_messages[AXIS2_ERROR_OM_ELEMENT_MISMATCH] =
        "AXIOM elements do not match";
    axutil_error_messages[AXIS2_ERROR_RPC_NEED_MATCHING_CHILD] =
        "RPC style SOAP body don't have a child element";
    axutil_error_messages[AXIS2_ERROR_UNKNOWN_STYLE] =
        "Operation description has unknown operation style";
    axutil_error_messages[AXIS2_ERROR_STRING_DOES_NOT_REPRESENT_A_VALID_NC_NAME]
        = "String does not represent a valid NCName";
    /* core:transport */

    /* core:transport:http */
    axutil_error_messages[AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR] =
        "Error occurred in transport";
    axutil_error_messages[AXIS2_ERROR_HTTP_REQUEST_NOT_SENT] =
        "A read attempt(HTTP) for the reply without sending the request";
    axutil_error_messages[AXIS2_ERROR_INVALID_HEADER] =
        "Invalid string passed as a HTTP header";
    axutil_error_messages[AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE] =
        "Invalid status line or invalid request line";
    axutil_error_messages[AXIS2_ERROR_INVALID_TRANSPORT_PROTOCOL] =
        "Transport protocol is unsupported by axis2";
    axutil_error_messages[AXIS2_ERROR_NULL_BODY] =
        "No body present in the request or the response";
    axutil_error_messages[AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT] =
        "A valid configuration context is required for the HTTP worker";
    axutil_error_messages[AXIS2_ERROR_NULL_HTTP_VERSION] =
        "HTTP version cannot be NULL in the status/request line";
    axutil_error_messages[AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX] =
        "Input stream is NULL in message context";
    axutil_error_messages[AXIS2_ERROR_NULL_OM_OUTPUT] = "OM output is NULL";
    axutil_error_messages[AXIS2_ERROR_NULL_SOAP_ENVELOPE_IN_MSG_CTX] =
        "NULL SOAP envelope in message context";
    axutil_error_messages[AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM] =
        "NULL stream in the HTTP chucked stream";
    axutil_error_messages[AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY] =
        "NULL stream in the response body";
    axutil_error_messages[AXIS2_ERROR_NULL_URL] = "URL NULL in HTTP client";
    axutil_error_messages[AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL] =
        "Transport information must be set in message context";
    axutil_error_messages[AXIS2_ERROR_RESPONSE_CONTENT_TYPE_MISSING] =
        "Content-Type header missing in HTTP response";
    axutil_error_messages[AXIS2_ERROR_RESPONSE_TIMED_OUT] =
        "Response timed out";
    axutil_error_messages[AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL] =
        "SOAP envelope or SOAP body NULL";
    axutil_error_messages[AXIS2_ERROR_SSL_ENGINE] =
        "Error occurred in SSL engine";
    axutil_error_messages[AXIS2_ERROR_SSL_NO_CA_FILE] =
        "Cannot find certificates";
    axutil_error_messages[AXIS2_ERROR_WRITING_RESPONSE] =
        "Error in writing the response in response writer";
    axutil_error_messages[AXIS2_ERROR_REQD_PARAM_MISSING] =
        "Required parameter is missing in URL encoded request";
    axutil_error_messages[AXIS2_ERROR_UNSUPPORTED_SCHEMA_TYPE] =
        " Unsupported schema type in REST";
    axutil_error_messages[AXIS2_ERROR_SVC_OR_OP_NOT_FOUND] =
        "Service or operation not found";
    /* mod_addr */
    axutil_error_messages[AXIS2_ERROR_NO_MSG_INFO_HEADERS] =
            "No messgae info headers";
    /* platforms */

    /* utils */
    axutil_error_messages[AXIS2_ERROR_COULD_NOT_OPEN_FILE] =
        "Could not open the file";
    axutil_error_messages[AXIS2_ERROR_DLL_CREATE_FAILED] =
        "Failed in creating DLL";
    axutil_error_messages[AXIS2_ERROR_DLL_LOADING_FAILED] =
        "DLL loading failed";
    axutil_error_messages[AXIS2_ERROR_ENVIRONMENT_IS_NULL] =
        "Environment passed is NULL";
    axutil_error_messages[AXIS2_ERROR_FILE_NAME_NOT_SET] =
        "Axis2 File does not have a file name";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_DLL_DESC] =
        "DLL description has invalid state of not having valid DLL create function, \
        of valid delete function or valid dll_handler";
    axutil_error_messages[AXIS2_ERROR_HANDLER_CREATION_FAILED] =
        "Failed in creating Handler";
    axutil_error_messages[AXIS2_ERROR_INDEX_OUT_OF_BOUNDS] =
        "Array list index out of bounds";
    axutil_error_messages[AXIS2_ERROR_INVALID_ADDRESS] =
        "Invalid IP or hostname";
    axutil_error_messages[AXIS2_ERROR_INVALID_FD] =
        "Trying to do operation on invalid file descriptor";
    axutil_error_messages[AXIS2_ERROR_INVALID_SOCKET] =
        "Trying to do operation on closed/not opened socket";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_PARAM] =
        "Parameter not set";
    axutil_error_messages[AXIS2_ERROR_MODULE_CREATION_FAILED] =
        "Module create failed";
    axutil_error_messages[AXIS2_ERROR_MSG_RECV_CREATION_FAILED] =
        "Failed in creating Message Receiver";
    axutil_error_messages[AXIS2_ERROR_NO_SUCH_ELEMENT] = "No such element";
    axutil_error_messages[AXIS2_ERROR_SOCKET_BIND_FAILED] =
        "Socket bind failed. Another process may be already using this port";
    axutil_error_messages[AXIS2_ERROR_SOCKET_ERROR] =
        "Error creating a socket. Most probably error returned by OS";
    axutil_error_messages[AXIS2_ERROR_SOCKET_LISTEN_FAILED] =
        "Listen failed for the server socket";
    axutil_error_messages[AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED] =
        "Failed in creating Service Skeleton";
    axutil_error_messages[AXIS2_ERROR_TRANSPORT_RECV_CREATION_FAILED] =
        "Failed in creating Transport Receiver";
    axutil_error_messages[AXIS2_ERROR_TRANSPORT_SENDER_CREATION_FAILED] =
        "Failed in creating Transport Sender";
    axutil_error_messages[AXIS2_ERROR_UUID_GEN_FAILED] =
        "Generation of platform dependent UUID failed";
    axutil_error_messages[AXIS2_ERROR_POSSIBLE_DEADLOCK] = "Possible deadlock";
    /* WSDL */
    axutil_error_messages
        [AXIS2_ERROR_INTERFACE_OR_PORT_TYPE_NOT_FOUND_FOR_THE_BINDING] =
        "Interface or Port Type not found for the binding";
    axutil_error_messages
        [AXIS2_ERROR_INTERFACES_OR_PORTS_NOT_FOUND_FOR_PARTIALLY_BUILT_WOM] =
        "Interfaces or Ports not found for the partially built WOM";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_WSDL_OP] =
        "WSDL operation accessed has invalid state";
    axutil_error_messages[AXIS2_ERROR_INVALID_STATE_WSDL_SVC] =
        "WSDL Service accessed has invalid state";
    axutil_error_messages[AXIS2_ERROR_MEP_CANNOT_DETERMINE_MEP] =
        "Cannot determine MEP";
    axutil_error_messages[AXIS2_ERROR_WSDL_BINDING_NAME_IS_REQUIRED] =
        "WSDL binding name is cannot be NULL";
    axutil_error_messages[AXIS2_ERROR_WSDL_INTERFACE_NAME_IS_REQUIRED] =
        "PortType/Interface name cannot be NULL";
    axutil_error_messages[AXIS2_ERROR_WSDL_PARSER_INVALID_STATE] =
        "WSDL parsing has resulted in an invalid state";
    axutil_error_messages[AXIS2_ERROR_WSDL_SVC_NAME_IS_REQUIRED] =
        "WSDL service name cannot be NULL";
    /* XML */

    /* XML:attachments */
    axutil_error_messages[AXIS2_ERROR_ATTACHMENT_MISSING] =
        "Attachment is missing";
    /* XML:om */
    axutil_error_messages[AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL] =
        "XML builder done with pulling. Pull parser cannot pull any more";
    axutil_error_messages[AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD] =
        "Discard failed because the builder state is invalid";
    axutil_error_messages[AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL] =
        "Invalid builder state; Builder's last node is NULL";
    axutil_error_messages[AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL] =
        "Invalid document state; Document root is NULL";
    axutil_error_messages
        [AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE] =
        "Undefined namespace used";
    axutil_error_messages[AXIS2_ERROR_INVALID_EMPTY_NAMESPACE_URI] =
        "Namespace should have a valid URI";
    axutil_error_messages
        [AXIS2_ERROR_ITERATOR_NEXT_METHOD_HAS_NOT_YET_BEEN_CALLED] =
        "Next method has not been called so cannot remove"
        "an element before calling next valid for any om iterator";
    axutil_error_messages[AXIS2_ERROR_ITERATOR_REMOVE_HAS_ALREADY_BEING_CALLED]
        = "Document root is NULL, when it is not supposed to be NULL";
    axutil_error_messages[AXIS2_ERROR_XML_READER_ELEMENT_NULL] =
        "AXIOM XML reader returned NULL element";
    axutil_error_messages[AXIS2_ERROR_XML_READER_VALUE_NULL] =
        "AXIOM XML reader returned NULL value";
    /* XML:parser */
    axutil_error_messages[AXIS2_ERROR_CREATING_XML_STREAM_READER] =
        "Error occurred creating XML stream reader";
    axutil_error_messages[AXIS2_ERROR_CREATING_XML_STREAM_WRITER] =
        "Error occurred creating XML stream writer";
    axutil_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE] =
        "Error in writing attribute";
    axutil_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE] =
        "Error in writing attribute with namespace";
    axutil_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX] =
        "Error in writing attribute with namespace prefix";
    axutil_error_messages[AXIS2_ERROR_WRITING_COMMENT] =
        "Error in writing comment";
    axutil_error_messages[AXIS2_ERROR_WRITING_DATA_SOURCE] =
        "Error in writing data source";
    axutil_error_messages[AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE] =
        "Error in writing default namespace";
    axutil_error_messages[AXIS2_ERROR_WRITING_DTD] = "Error in writing DDT";
    axutil_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT] =
        "Error occurred in writing empty element";
    axutil_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE] =
        "Error occurred in writing empty element with namespace";
    axutil_error_messages
        [AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX] =
        "Error in writing empty element with namespace prefix";
    axutil_error_messages[AXIS2_ERROR_WRITING_END_DOCUMENT] =
        "Error occurred in writing end document in XML writer";
    axutil_error_messages[AXIS2_ERROR_WRITING_END_ELEMENT] =
        "Error occurred in writing end element in XML writer";
    axutil_error_messages[AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION] =
        "Error in writing processing instruction";
    axutil_error_messages[AXIS2_ERROR_WRITING_START_DOCUMENT] =
        "Error occurred in writing start element in start document in XML writer";
    axutil_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT] =
        "Error occurred in writing start element in XML writer";
    axutil_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE] =
        "Error occurred in writing start element with namespace in XML writer";
    axutil_error_messages
        [AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX] =
        "Error occurred in writing start element with namespace prefix";
    axutil_error_messages[AXIS2_ERROR_WRITING_CDATA] =
        "Error in writing CDATA section";
    axutil_error_messages[AXIS2_ERROR_XML_PARSER_INVALID_MEM_TYPE] =
        "AXIS2_XML_PARSER_TYPE_BUFFER or AXIS2_XML_PARSER_TYPE_DOC is expected";

    /* invalid type passed */
    axutil_error_messages[AXIS2_ERROR_INVALID_BASE_TYPE] =
        "Invalid base type passed";
    axutil_error_messages[AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI] =
        "Invalid SOAP namespace URI found";
    axutil_error_messages[AXIS2_ERROR_INVALID_SOAP_VERSION] =
        "Invalid SOAP version";
    axutil_error_messages[AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND] =
        "Invalid value found in must understand";
    axutil_error_messages[AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED] =
        "Multiple  fault code elements encountered in SOAP fault";
    axutil_error_messages[AXIS2_ERROR_MULTIPLE_DETAIL_ELEMENTS_ENCOUNTERED] =
        "Multiple fault detail elements encountered in SOAP fault";
    axutil_error_messages[AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED] =
        "Multiple fault node elements encountered in SOAP fault";
    axutil_error_messages[AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED] =
        "Multiple fault reason elements encountered in SOAP fault";
    axutil_error_messages[AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED] =
        "Multiple fault role elements encountered in SOAP fault ";
    axutil_error_messages[AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED] =
        "Multiple fault sub-code value elements encountered";
    axutil_error_messages
        [AXIS2_ERROR_MULTIPLE_VALUE_ENCOUNTERED_IN_CODE_ELEMENT] =
        "Multiple fault value elements encountered";
    axutil_error_messages[AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE]
        = "Must understand attribute should have a value of either true or false";
    axutil_error_messages[AXIS2_ERROR_OM_ELEMENT_EXPECTED] =
        "AXIOM element is expected";
    axutil_error_messages[AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE] =
        "Processing SOAP 1.1 fault value element should have only"
        "text as its children";
    axutil_error_messages[AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY] =
        "Only one SOAP fault is allowed in SOAP body";
    axutil_error_messages
        [AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS] =
        "SOAP 1.1 fault actor should not have any child elements";
    axutil_error_messages
        [AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY] =
        "SOAP builder found a child element other than header or body in envelope"
        "element";
    axutil_error_messages[AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER] =
        "SOAP builder encountered body element first and header next";
    axutil_error_messages
        [AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED] =
        "SOAP builder multiple body elements encountered";
    axutil_error_messages[AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED]
        = "SOAP builder encountered multiple headers";
    axutil_error_messages[AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE] =
        "SOAP fault code does not have a value";
    axutil_error_messages
        [AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT] =
        "SOAP fault reason element should have a text value";
    axutil_error_messages
        [AXIS2_ERROR_SOAP_FAULT_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT] =
        "SOAP fault role element should have a text value";
    axutil_error_messages
        [AXIS2_ERROR_SOAP_FAULT_VALUE_SHOULD_BE_PRESENT_BEFORE_SUB_CODE] =
        "SOAP fault value should be present before sub-code element in SOAP fault code";
    axutil_error_messages[AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE]
        = "SOAP message does not contain a SOAP envelope element";
    axutil_error_messages
        [AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME] =
        "SOAP message's first element should have a localname";
    axutil_error_messages
        [AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_REASON_ELEMENT]
        = "Localname not supported inside a reason element";
    axutil_error_messages
        [AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_SUB_CODE_ELEMENT]
        = "Localname not supported inside the sub-code element";
    axutil_error_messages
        [AXIS2_ERROR_THIS_LOCALNAME_NOT_SUPPORTED_INSIDE_THE_CODE_ELEMENT] =
        "Localname not supported inside the code element";
    axutil_error_messages
        [AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP] =
        "Transport identified SOAP version does not match with SOAP message version";
    axutil_error_messages[AXIS2_ERROR_UNSUPPORTED_ELEMENT_IN_SOAP_FAULT_ELEMENT]
        = "Unsupported element found in SOAP fault element";
    axutil_error_messages[AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED] =
        "Wrong element order encountered ";
    /* services */
    axutil_error_messages[AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST] =
        "Invalid XML format in request";
    axutil_error_messages[AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL] =
        "Input OM node NULL, Probably error in SOAP request";
    axutil_error_messages
        [AXIS2_ERROR_SVC_SKEL_INVALID_OPERATION_PARAMETERS_IN_SOAP_REQUEST] =
        "Invalid parameters for service operation in SOAP request";

    axutil_error_messages[AXIS2_ERROR_WSDL_SCHEMA_IS_NULL] = "Schema is NULL";

    /* repos */
    axutil_error_messages[AXIS2_ERROR_REPOS_NOT_AUTHENTICATED] =
        "Not authenticated";
    axutil_error_messages[AXIS2_ERROR_REPOS_UNSUPPORTED_MODE] =
        "Unsupported mode";
    axutil_error_messages[AXIS2_ERROR_REPOS_EXPIRED] = "Expired";
    axutil_error_messages[AXIS2_ERROR_REPOS_NOT_IMPLEMENTED] =
        "Not implemented";
    axutil_error_messages[AXIS2_ERROR_REPOS_NOT_FOUND] = "Not found";
    axutil_error_messages[AXIS2_ERROR_REPOS_BAD_SEARCH_TEXT] =
        "Bad search text";

    /* neethi */
    axutil_error_messages[AXIS2_ERROR_NEETHI_ELEMENT_WITH_NO_NAMESPACE] =
        "Element with no namespace";
    axutil_error_messages
        [AXIS2_ERROR_NEETHI_POLICY_CREATION_FAILED_FROM_ELEMENT] =
        "Policy creation failed from element";
    axutil_error_messages[AXIS2_ERROR_NEETHI_ALL_CREATION_FAILED_FROM_ELEMENT] =
        "All creation failed from element";
    axutil_error_messages
        [AXIS2_ERROR_NEETHI_EXACTLYONE_CREATION_FAILED_FROM_ELEMENT] =
        "Exactly one creation failed from element";
    axutil_error_messages
        [AXIS2_ERROR_NEETHI_REFERENCE_CREATION_FAILED_FROM_ELEMENT] =
        "Reference creation failed from element";
    axutil_error_messages
        [AXIS2_ERROR_NEETHI_ASSERTION_CREATION_FAILED_FROM_ELEMENT] =
        "Assertion creation failed from element";
    axutil_error_messages[AXIS2_ERROR_NEETHI_ALL_CREATION_FAILED] =
        "All creation failed";
    axutil_error_messages[AXIS2_ERROR_NEETHI_EXACTLYONE_CREATION_FAILED] =
        "Exactly one creation failed";
    axutil_error_messages[AXIS2_ERROR_NEETHI_POLICY_CREATION_FAILED] =
        "Policy creation failed";
    axutil_error_messages[AXIS2_ERROR_NEETHI_NORMALIZATION_FAILED] =
        "Normalization failed";
    axutil_error_messages[AXIS2_ERROR_NEETHI_WRONG_INPUT_FOR_MERGE] =
        "Wrong input for merge";
    axutil_error_messages[AXIS2_ERROR_NEETHI_CROSS_PRODUCT_FAILED] =
        "Cross product failed";
    axutil_error_messages[AXIS2_ERROR_NEETHI_NO_CHILDREN_POLICY_COMPONENTS] =
        "No children policy components";
    axutil_error_messages[AXIS2_ERROR_NEETHI_URI_NOT_SPECIFIED] =
        "Reference URI not specified";
    axutil_error_messages[AXIS2_ERROR_NEETHI_NO_ENTRY_FOR_THE_GIVEN_URI] =
        "No entry for the given URI";
    axutil_error_messages
        [AXIS2_ERROR_NEETHI_EXACTLYONE_NOT_FOUND_IN_NORMALIZED_POLICY] =
        "Exactly one not found in normalized policy";
    axutil_error_messages[AXIS2_ERROR_NEETHI_EXACTLYONE_IS_EMPTY] =
        "Exactly one is empty";
    axutil_error_messages
        [AXIS2_ERROR_NEETHI_ALL_NOT_FOUND_WHILE_GETTING_CROSS_PRODUCT] =
        "All not found while getting cross product";
    axutil_error_messages[AXIS2_ERROR_NEETHI_UNKNOWN_ASSERTION] = 
        "Unknown Assertion";

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_error_t *AXIS2_CALL
axutil_error_create(
    axutil_allocator_t * allocator)
{
    axutil_error_t *error;
    if (!allocator)
        return NULL;

    error = (axutil_error_t *) AXIS2_MALLOC(allocator, sizeof(axutil_error_t));
    memset(error, 0,  sizeof(axutil_error_t));

    if (!error)
        return NULL;

    error->allocator = allocator;

    return error;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axutil_error_get_message(
    const axutil_error_t * error)
{
    const axis2_char_t *message = NULL;
    if (error)
    {
        if (error->error_number > AXIS2_ERROR_NONE &&
            error->error_number < AXUTIL_ERROR_MAX) /* TODO; This needs to be 
            fixed to include module defined and user defined errors */
            message = axutil_error_messages[error->error_number];
        else
        {
            if (error->message)
            {
                message = error->message;
            }
            else if (error->error_number == AXIS2_ERROR_NONE)
            {
                message = axutil_error_messages[AXIS2_ERROR_NONE];
            }
            else
            {
                message = "Undefined error returned by business logic implementation";
            }
        }
    }

    return message;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_error_set_error_number(
    axutil_error_t * error,
    axutil_error_codes_t error_number)
{
    if (!error)
        return AXIS2_FAILURE;
    error->error_number = error_number;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_error_set_status_code(
    axutil_error_t * error,
    axis2_status_codes_t status_code)
{
    if (!error)
        return AXIS2_FAILURE;
    error->status_code = status_code;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_error_get_status_code(
    axutil_error_t * error)
{
    if (error)
        return error->status_code;
    else
        return AXIS2_CRITICAL_FAILURE;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_error_set_error_message(
    axutil_error_t * error,
    axis2_char_t * message)
{
    if (message && error)
    {
        error->message = message;
        return AXIS2_SUCCESS;
    }
    
    return AXIS2_FAILURE;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_error_free(
    axutil_error_t * error)
{
    if (error)
    {
        AXIS2_FREE(error->allocator, error);
    }
    return;
}


