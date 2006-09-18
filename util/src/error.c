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

const axis2_char_t * AXIS2_CALL
axis2_error_impl_get_message(const axis2_error_t *error);

axis2_status_t AXIS2_CALL
axis2_error_impl_set_error_number(axis2_error_t *error, axis2_error_codes_t error_number);

axis2_status_t AXIS2_CALL
axis2_error_impl_set_status_code(axis2_error_t *error, axis2_status_codes_t status_code);

axis2_status_t AXIS2_CALL
axis2_error_impl_get_status_code(axis2_error_t *error);

/* array to hold error messages */
const axis2_char_t* axis2_error_messages[AXIS2_ERROR_LAST];

axis2_status_t AXIS2_CALL
axis2_error_init()
{
    int i = 0;
    for (i = 0; i < AXIS2_ERROR_LAST; i++)
    {
        axis2_error_messages[i] = "Unknown Error :(";
    }

    /* Common Errors */
    axis2_error_messages[AXIS2_ERROR_NONE] = "No Error";
    axis2_error_messages[AXIS2_ERROR_NO_MEMORY] = "Out of memory";
    axis2_error_messages[AXIS2_ERROR_INVALID_NULL_PARAM] =
        "NULL paramater was passed when a non NULL parameter was expected";
    /* core:addr */

    /* core:clientapi */
    axis2_error_messages[AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE] =
        "Blocking invocation expects response";
    axis2_error_messages[AXIS2_ERROR_CANNOT_INFER_TRANSPORT] =
        "cannot infer transport from URL";
    axis2_error_messages[AXIS2_ERROR_CLIENT_SIDE_SUPPORT_ONLY_ONE_CONF_CTX] =
        "Client side support only one configuration context";
    axis2_error_messages[AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT] =
        "MEP cannot be NULL in MEP client";
    axis2_error_messages[AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT] =
        "MEP Mismatch";
    axis2_error_messages[AXIS2_ERROR_TWO_WAY_CHANNEL_NEEDS_ADDRESSING] =
        "Two way channel needs addressing module to be engaged";
    axis2_error_messages[AXIS2_ERROR_UNKNOWN_TRANSPORT] =
        "Unknown Transport";
    axis2_error_messages[AXIS2_ERROR_UNSUPPORTED_TYPE] =
        "type is not supported";
    axis2_error_messages[AXIS2_ERROR_OPTIONS_OBJECT_IS_NOT_SET] =
        "Options object is not set";
    /* core:clientapi:diclient */

    /* core:context */
    axis2_error_messages[AXIS2_ERROR_INVALID_SOAP_ENVELOPE_STATE] =
        "Invalid SOAP envelope state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_MSG_CTX] =
        "Invalid message context state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_SVC] =
        "Service accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_SVC_GRP] =
        "Service group accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_SERVICE_NOT_YET_FOUND] =
        "Service not yet found";
    /* core:deployment */
    axis2_error_messages[AXI2_ERROR_INVALID_PHASE] =
        "Invalid phase found in phase validation*";
    axis2_error_messages[AXIS2_ERROR_CONFIG_NOT_FOUND] =
        "axis2.xml cannot be not found";
    axis2_error_messages[AXIS2_ERROR_DATA_ELEMENT_IS_NULL] =
        "Data element of the OM Node is null";
    axis2_error_messages[AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT] =
        "In transport sender, Inflow is not allowed";
    axis2_error_messages[AXIS2_ERROR_INVALID_HANDLER_STATE] =
        "Invalid handler state";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODUELE_REF] =
        "Invalid Module Ref encountered";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP] =
        "Invalid Module Reference by Operation";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODULE_CONF] =
        "Invalid Module Configuration";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_DESC_BUILDER] =
        "Description Builder is found to be in invalid state";
    axis2_error_messages[AXIS2_ERROR_MODULE_NOT_FOUND] =
        "Module Not Found";
    axis2_error_messages[AXIS2_ERROR_MODULE_VALIDATION_FAILED] =
        "Module Validation Failed";
    axis2_error_messages[AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE] =
        "Module xml file is not found in the given path";
    axis2_error_messages[AXIS2_ERROR_NO_DISPATCHER_FOUND] =
        "No dispatcher found";
    axis2_error_messages[AXIS2_ERROR_OP_NAME_MISSING] =
        "Operation name is missing";
    axis2_error_messages[AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN] =
        "In transport Receiver, Outflow is not allowed";
    axis2_error_messages[AXIS2_ERROR_REPO_CAN_NOT_BE_NULL] =
        "Repository name cannot be NULL";
    axis2_error_messages[AXIS2_ERROR_REPOSITORY_NOT_EXIST] =
        "Repository in path does not exist";
    axis2_error_messages[AXIS2_ERROR_REPOS_LISTENER_INIT_FAILED] =
        "Repository Listener initialization failed";
    axis2_error_messages[AXIS2_ERROR_SERVICE_XML_NOT_FOUND] =
        "Service xml file is not found in the given path";
    axis2_error_messages[AXIS2_ERROR_SVC_NAME_ERROR] =
        "Service Name Error";
    axis2_error_messages[AXIS2_ERROR_TRANSPORT_SENDER_ERROR] =
        "Transport Sender Error";
    axis2_error_messages[AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL] =
        "Path to Config can not be NULL";
    axis2_error_messages[AXIS2_ERROR_INVALID_SVC] =
        "Invalid Service";
    /* core:description */
    axis2_error_messages[AXIS2_ERROR_CANNOT_CORRELATE_MSG] =
        "Cannot correlate message";
    axis2_error_messages[AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT] =
        "Could not Map the MEP URI to a axis MEP constant value";
    axis2_error_messages[AXIS2_ERROR_INVALID_MESSAGE_ADDITION] =
        "Invalid messge addition operation context completed";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_MODULE_DESC] =
        "Module description accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER] =
        "Parameter container not set";
    axis2_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP] =
        "module has alredy engaged to the op op terminated !!!";
    axis2_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC] =
        "module has alredy been engaged on the service.Operation terminated !!!";
    axis2_error_messages[AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC_GRP] =
        "module has alredy been engaged on the service. Group Operation terminated !!!";
    axis2_error_messages[AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE] =
        "Parameter locked, Cannot override";
    axis2_error_messages[AXIS2_ERROR_EMPTY_SCHEMA_LIST] =
        "schema list is empty or NULL in svc ";
    /* core:engine */
    axis2_error_messages[AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME] =
        "Both before and after handlers cannot be the same";
    axis2_error_messages[AXIS2_ERROR_INVALID_HANDLER_RULES] =
        "Invalid handler rules";
    axis2_error_messages[AXIS2_ERROR_INVALID_MODULE] =
        "Invalid Module";
    axis2_error_messages[AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER] =
        "Invalid first handler for phase";
    axis2_error_messages[AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER] =
        "Invalid last handler for phase";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_CONF] =
        "Invalid engine config state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY] =
        "Message context processing a fault already";
    axis2_error_messages[AXIS2_ERROR_NOWHERE_TO_SEND_FAULT] =
        "fault to field not specified in message context";
    axis2_error_messages[AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID] =
        "Only one handler allowed for phase, adding another handler is not allowed";
    axis2_error_messages[AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET] =
        "First handler of phase already set";
    axis2_error_messages[AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET] =
        "Last handler of phase already set";
    axis2_error_messages[AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME] =
        "Two service can not have same name, a service with same name alredy";
    /* core:phaseresolver */
    axis2_error_messages[AXIS2_ERROR_INVALID_MODULE_REF] =
        "Invalid Module Ref";
    axis2_error_messages[AXIS2_ERROR_INVALID_PHASE] =
        "Invalid Phase";
    axis2_error_messages[AXIS2_ERROR_NO_TRANSPORT_IN_CONFIGURED] =
        "There are no in transport chains configured";
    axis2_error_messages[AXIS2_ERROR_NO_TRANSPORT_OUT_CONFIGURED] =
        "There are no out transport chains configured";
    axis2_error_messages[AXIS2_ERROR_PHASE_IS_NOT_SPECIFED] =
        "Phase is not specified";
    axis2_error_messages[AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE] =
        "Service module can not refer global phase";
    /* core:wsdl*/
    axis2_error_messages[AXIS2_ERROR_WSDL_SCHEMA_IS_NULL] =
        "Schema is NULL";
    /* core:receivers */
    axis2_error_messages[AXIS2_ERROR_OM_ELEMENT_INVALID_STATE] =
        "Om Element has invalid state";
    axis2_error_messages[AXIS2_ERROR_OM_ELEMENT_MISMATCH] =
        "Om Elements do not match";
    axis2_error_messages[AXIS2_ERROR_RPC_NEED_MATCHING_CHILD] =
        "RPC style soap body don't have a child element";
    axis2_error_messages[AXIS2_ERROR_UNKNOWN_STYLE] =
        "Operation Description has unknown operation style";
    axis2_error_messages[AXIS2_ERROR_STRING_DOES_NOT_REPRESENT_A_VALID_NC_NAME] =
        "String does not represent a valid NCName";
    /* core:transport */

    /* core:transport:http */
    axis2_error_messages[AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR] =
        "Error occured in transport";
    axis2_error_messages[AXIS2_ERROR_HTTP_REQUEST_NOT_SENT] =
        "A read attempt(HTTP) for the reply without sending the request";
    axis2_error_messages[AXIS2_ERROR_INVALID_HEADER] =
        "Invalid string passed as a http header";
    axis2_error_messages[AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE] =
        "Invalid status line or invalid request line";
    axis2_error_messages[AXIS2_ERROR_INVALID_TRANSPORT_PROTOCOL] =
        "Transport protocol is unsupported by axis2";
    axis2_error_messages[AXIS2_ERROR_NULL_BODY] =
        "No body present in the request or the response";
    axis2_error_messages[AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT] =
        "A valid conf_ctx is reqd for the http worker";
    axis2_error_messages[AXIS2_ERROR_NULL_HTTP_VERSION] =
        "HTTP version cannot be null in the status/request line";
    axis2_error_messages[AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX] =
        "Input stream is NULL in msg_ctx";
    axis2_error_messages[AXIS2_ERROR_NULL_OM_OUTPUT] =
        "OM output is NULL";
    axis2_error_messages[AXIS2_ERROR_NULL_SOAP_ENVELOPE_IN_MSG_CTX] =
        "Null soap envelope in msg_ctx";
    axis2_error_messages[AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM] =
        "NULL stream in the http chunked stream";
    axis2_error_messages[AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY] =
        "We got a NULL strem in the response body";
    axis2_error_messages[AXIS2_ERROR_NULL_URL] =
        "URL NULL in http client";
    axis2_error_messages[AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL] =
        "We need transport_info in msg_ctx";
    axis2_error_messages[AXIS2_ERROR_RESPONSE_CONTENT_TYPE_MISSING] =
        "Content-Type header missing in HTTP response";
    axis2_error_messages[AXIS2_ERROR_RESPONSE_TIMED_OUT] =
        "Response timed out";
    axis2_error_messages[AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL] =
        "SOAP envelope or SOAP body NULL";
    axis2_error_messages[AXIS2_ERROR_SSL_ENGINE] =
        "Error occured in SSL engine";
    axis2_error_messages[AXIS2_ERROR_SSL_NO_CA_FILE] =
        "Either axis2c cannot find certificates or the env variable is not set";
    axis2_error_messages[AXIS2_ERROR_WRITING_RESPONSE] =
        "Error in writing the response in response writer";
    axis2_error_messages[AXIS2_ERROR_REQD_PARAM_MISSING] =
        "Required parameter is missing in url encoded request";
    axis2_error_messages[AXIS2_ERROR_UNSUPPORTED_SCHEMA_TYPE] =
        " Unsuppoted schema type in REST";
    axis2_error_messages[AXIS2_ERROR_SVC_OR_OP_NOT_FOUND] =
        "Service or operation not found";
    /* mod_addr */

    /* platforms */

    /* utils */
    axis2_error_messages[AXIS2_ERROR_COULD_NOT_OPEN_FILE] =
        "Could not open the file";
    axis2_error_messages[AXIS2_ERROR_DLL_CREATE_FAILED] =
        "Failed in creating DLL";
    axis2_error_messages[AXIS2_ERROR_DLL_LOADING_FAILED] =
        "DLL loading failed";
    axis2_error_messages[AXIS2_ERROR_ENVIRONMENT_IS_NULL] =
        "Environment passed is null";
    axis2_error_messages[AXIS2_ERROR_FILE_NAME_NOT_SET] =
        "Axis2 File does not have a file name";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_DLL_DESC] =
        "dll description has invalid state of not having valid dll create function, \
        of valid delete function or valid dll_handler";
    axis2_error_messages[AXIS2_ERROR_HANDLER_CREATION_FAILED] =
        "Failed in creating Handler";
    axis2_error_messages[AXIS2_ERROR_INDEX_OUT_OF_BOUNDS] =
        "Array list index out of bounds";
    axis2_error_messages[AXIS2_ERROR_INVALID_ADDRESS] =
        "Invalid IP or hostname";
    axis2_error_messages[AXIS2_ERROR_INVALID_FD] =
        "Trying to do operation on invalid file descriptor";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOCKET] =
        "Trying to do operation on closed/not opened socket";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_PARAM] =
        "Parameter not set";
    axis2_error_messages[AXIS2_ERROR_MODULE_CREATION_FAILED] =
        "Module create failed";
    axis2_error_messages[AXIS2_ERROR_MSG_RECV_CREATION_FAILED] =
        "Failed in creating Message Receiver";
    axis2_error_messages[AXIS2_ERROR_NO_SUCH_ELEMENT] =
        "No such element";
    axis2_error_messages[AXIS2_ERROR_SOCKET_BIND_FAILED] =
        "Socket bind failed. Another process may be already using this port";
    axis2_error_messages[AXIS2_ERROR_SOCKET_ERROR] =
        "Error creating a socket. Most probably error returned by OS";
    axis2_error_messages[AXIS2_ERROR_SOCKET_LISTEN_FAILED] =
        "Listen failed for the server socket";
    axis2_error_messages[AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED] =
        "Failed in creating Service Skeleton";
    axis2_error_messages[AXIS2_ERROR_TRANSPORT_RECV_CREATION_FAILED] =
        "Failed in creating Transport Receiver";
    axis2_error_messages[AXIS2_ERROR_TRANSPORT_SENDER_CREATION_FAILED] =
        "Failed in creating Transport Sender";
    axis2_error_messages[AXIS2_ERROR_UUID_GEN_FAILED] =
        "Generation of platform dependent uuid failed";
    /* wsdl */
    axis2_error_messages[AXIS2_ERROR_INTERFACE_OR_PORT_TYPE_NOT_FOUND_FOR_THE_BINDING] =
        "Interface or Port Type not found for the binding";
    axis2_error_messages[AXIS2_ERROR_INTERFACES_OR_PORTS_NOT_FOUND_FOR_PARTIALLY_BUILT_WOM] =
        "Interfaces or Ports not found for the partially built WOM";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_WSDL_OP] =
        "Wsdl op accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_INVALID_STATE_WSDL_SVC] =
        "Wsdl Service accessed has invalid state";
    axis2_error_messages[AXIS2_ERROR_MEP_CANNOT_DETERMINE_MEP] =
        "Cannot determine MEP";
    axis2_error_messages[AXIS2_ERROR_WSDL_BINDING_NAME_IS_REQUIRED] =
        "Wsdl binding name is cannot be NULL(Is required)";
    axis2_error_messages[AXIS2_ERROR_WSDL_INTERFACE_NAME_IS_REQUIRED] =
        " PortType/Interface name cannot be null(Required)";
    axis2_error_messages[AXIS2_ERROR_WSDL_PARSER_INVALID_STATE] =
        "Wsdl parsing has resulted in an invalid state";
    axis2_error_messages[AXIS2_ERROR_WSDL_SVC_NAME_IS_REQUIRED] =
        " Wsdl svc name cannot be null(Required)";
    /* xml */

    /* xml:attachments */

    /* xml:om */
    axis2_error_messages[AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL] =
        "Builder done with pulling. Cannot pull any more";
    axis2_error_messages[AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD] =
        "Discard faile because the builder state is invalid";
    axis2_error_messages[AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL] =
        "Bulder's last node is NULL when it is not supposed to be NULL";
    axis2_error_messages[AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL] =
        "Document root is NULL] =  when it is not supposed to be NULL";
    axis2_error_messages[AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE] =
        "Undefined namespace used";
    axis2_error_messages[AXIS2_ERROR_INVALID_EMPTY_NAMESPACE_URI] =
        "Error a namespace should have a valid uri";
    axis2_error_messages[AXIS2_ERROR_ITERATOR_NEXT_METHOD_HAS_NOT_YET_BEEN_CALLED] =
        "next method has not been called so cannot remove"
        "an element before calling next valid for any om iterator";
    axis2_error_messages[AXIS2_ERROR_ITERATOR_REMOVE_HAS_ALREADY_BEING_CALLED] =
        "Document root is NULL, when it is not supposed to be NULL";
    axis2_error_messages[AXIS2_ERROR_XML_READER_ELEMENT_NULL] =
        "axiom_xml_reader returned NULL element";
    axis2_error_messages[AXIS2_ERROR_XML_READER_VALUE_NULL] =
        "axiom_xml_reader returned NULL value";
    /* xml:parser */
    axis2_error_messages[AXIS2_ERROR_CREATING_XML_STREAM_READER] =
        "error occured creating xml stream writer";
    axis2_error_messages[AXIS2_ERROR_CREATING_XML_STREAM_WRITER] =
        "error occured creating xml stream writer";
    axis2_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE] =
        "error in writing attribute";
    axis2_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE] =
        "error in writing attribute with namespace";
    axis2_error_messages[AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX] =
        "error in writing attribute with namespace prefix";
    axis2_error_messages[AXIS2_ERROR_WRITING_COMMENT] =
        "error in writing comment";
    axis2_error_messages[AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE] =
        "error in writing default namespace";
    axis2_error_messages[AXIS2_ERROR_WRITING_DTD] =
        "error in writing dtd";
    axis2_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT] =
        "error occured in writing empty element";
    axis2_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE] =
        "error occured in writing empty element with namespace";
    axis2_error_messages[AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX] =
        "error in writing empty element with namespace prefix";
    axis2_error_messages[AXIS2_ERROR_WRITING_END_DOCUMENT] =
        "error occured in writing end document in xml writer";
    axis2_error_messages[AXIS2_ERROR_WRITING_END_ELEMENT] =
        "error occured in writing end element in xml writer";
    axis2_error_messages[AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION] =
        "error in writing processing instruction";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_DOCUMENT] =
        "error occured in writing start element in start document in xml writer";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT] =
        "error occured in writing start element in xml writer";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE] =
        "error occured in writing start element with namespace in xml writer";
    axis2_error_messages[AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX] =
        "error occured in writing start element with namespace prefix";
    axis2_error_messages[AXIS2_ERROR_WRITING_CDATA] =
        "error in writing cdata section";
    axis2_error_messages[AXIS2_ERROR_XML_PARSER_INVALID_MEM_TYPE] =
        "AXIS2_XML_PARSER_TYPE_BUFFER or AXIS2_XML_PARSER_TYPE_DOC is expected";

    /* invalid type passed */
    axis2_error_messages[AXIS2_ERROR_INVALID_BASE_TYPE] =
        "invalid type passed";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI] =
        "invalid soap namespace uri found";
    axis2_error_messages[AXIS2_ERROR_INVALID_SOAP_VERSION] =
        "Invalid SOAP version";
    axis2_error_messages[AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND] =
        "invalid value found in must understand";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED] =
        "multiple  fault code elements encountered in soap fault";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_DETAIL_ELEMENTS_ENCOUNTERED] =
        "multiple fault detail elements encountered in soap fault";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED] =
        "multiple fault node elements encountered in soap fault";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED] =
        "multiple fault reason elements encountered in soap fault";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED] =
        "multiple fault role elements encountered in soap fault ";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED] =
        "multiple fault subcode value elements encountered";
    axis2_error_messages[AXIS2_ERROR_MULTIPLE_VALUE_ENCOUNTERED_IN_CODE_ELEMENT] =
        "multiple fault value elements encounterd";
    axis2_error_messages[AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE] =
        "must understatnd attribute should have values of true or false";
    axis2_error_messages[AXIS2_ERROR_OM_ELEMENT_EXPECTED] =
        "om element is expected";
    axis2_error_messages[AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE] =
        "processing soap11 fault value element should have only"
        "text as its children";
    axis2_error_messages[AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY] =
        "only one SOAP fault allowed in SOAP body";
    axis2_error_messages[AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS] =
        "AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY] =
        "soap builder found a child element other than header or body in envelope"
        "element";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER] =
        "soap builder encounterd body element first and header next";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED] =
        "soap builder multiple body elements encounterd";
    axis2_error_messages[AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED] =
        "soap builder encountered multiple headers";
    axis2_error_messages[AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE] =
        "AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE";
    axis2_error_messages[AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT] =
        "AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT";
    axis2_error_messages[AXIS2_ERROR_SOAP_FAULT_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT] =
        "soap fault role element should have a text value";
    axis2_error_messages[AXIS2_ERROR_SOAP_FAULT_VALUE_SHOULD_BE_PRESENT_BEFORE_SUB_CODE] =
        "soap fault value should be present before subcode element in soap fault code";
    axis2_error_messages[AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE] =
        "soap message does not contain a soap envelope element";
    axis2_error_messages[AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME] =
        "soap messgae first element should have a localname";
    axis2_error_messages[AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_REASON_ELEMENT] =
        "localname not supported inside a reason element";
    axis2_error_messages[AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_SUB_CODE_ELEMENT] =
        "localname not supported inside the subcode element";
    axis2_error_messages[AXIS2_ERROR_THIS_LOCALNAME_NOT_SUPPORTED_INSIDE_THE_CODE_ELEMENT] =
        "localname not supported inside the code element";
    axis2_error_messages[AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP] =
        "transport identified soap version does not match with soap message version ";
    axis2_error_messages[AXIS2_ERROR_UNSUPPORTED_ELEMENT_IN_SOAP_FAULT_ELEMENT] =
        "unsupported element found in soap fault element";
    axis2_error_messages[AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED] =
        "wrong element order encountered ";
    /* services */
    axis2_error_messages[AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST] =
        "Invalid XML format in request";
    axis2_error_messages[AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL] =
        "Input OM node NULL, Probably error in SOAP request";
    axis2_error_messages[AXIS2_ERROR_SVC_SKEL_INVALID_OPERATION_PARAMETERS_IN_SOAP_REQUEST] =
        "Invalid parameters for service operation in SOAP request";

    axis2_error_messages[AXIS2_ERROR_WSDL_SCHEMA_IS_NULL] =
        "Schema is NULL";

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_error_impl_free(axis2_error_t *error)
{
    if (NULL != error && NULL != error->ops)
    {
        free(error->ops);
    }
    if (NULL != error)
    {
        free(error);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_error_t* AXIS2_CALL
axis2_error_create(axis2_allocator_t * allocator)
{
    axis2_error_t *error;
    if (!allocator)
        return NULL;

    error =
        (axis2_error_t *) AXIS2_MALLOC(allocator, sizeof(axis2_error_t));

    if (!error)
        return NULL;

    error->ops =
        (axis2_error_ops_t *) AXIS2_MALLOC(allocator,
                sizeof(axis2_error_ops_t));

    if (!error->ops)
    {
        AXIS2_FREE(allocator, error);
        return NULL;
    }

    error->ops->get_message = axis2_error_impl_get_message;
    error->ops->get_extended_message = axis2_error_impl_get_message;
    error->ops->set_error_number = axis2_error_impl_set_error_number;
    error->ops->set_status_code = axis2_error_impl_set_status_code;
    error->ops->get_status_code = axis2_error_impl_get_status_code;
    error->ops->free            = axis2_error_impl_free;

    return error;
}

const axis2_char_t * AXIS2_CALL
axis2_error_impl_get_message(const axis2_error_t *error)
{
    if (error && error->error_number >= AXIS2_ERROR_NONE && error->error_number < AXIS2_ERROR_LAST)
        return axis2_error_messages[error->error_number];

    return "Invalid Error Number";
}

axis2_status_t AXIS2_CALL
axis2_error_impl_set_error_number(axis2_error_t *error, axis2_error_codes_t error_number)
{
    error->error_number = error_number;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_error_impl_set_status_code(axis2_error_t *error, axis2_status_codes_t status_code)
{
    error->status_code = status_code;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_error_impl_get_status_code(axis2_error_t *error)
{
    return error->status_code;
}
