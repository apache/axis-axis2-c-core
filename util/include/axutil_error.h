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

#ifndef AXUTIL_ERROR_H
#define AXUTIL_ERROR_H

#include <axutil_utils_defines.h>
#include <axutil_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define AXUTIL_ERROR_MESSAGE_BLOCK_SIZE 512
#define AXUTIL_ERROR_LAST AXUTIL_ERROR_MESSAGE_BLOCK_SIZE
#define NEETHI_ERROR_CODES_START AXIS2_ERROR_LAST
#define RAMPART_ERROR_CODES_START (NEETHI_ERROR_CODES_START + AXUTIL_ERROR_MESSAGE_BLOCK_SIZE)
#define SANDESHA2_ERROR_CODES_START (RAMPART_ERROR_CODES_START + AXUTIL_ERROR_MESSAGE_BLOCK_SIZE)
#define SAVAN_ERROR_CODES_START (SANDESHA2_ERROR_CODES_START + AXUTIL_ERROR_MESSAGE_BLOCK_SIZE)
#define USER_ERROR_CODES_START (SAVAN_ERROR_CODES_START + AXUTIL_ERROR_MESSAGE_BLOCK_SIZE)

/* AXUTIL_ERROR_MAX define the maximum size of the error array */
#define AXUTIL_ERROR_MAX (USER_ERROR_CODES_START + AXUTIL_ERROR_MESSAGE_BLOCK_SIZE)


    /**
     * \brief Axis2 status codes
     *
     * Possible status values for Axis2
     */
    enum axis2_status_codes
    {
        /** Critical Failure state */
        AXIS2_CRITICAL_FAILURE = -1,

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
    enum axutil_error_codes
    {

        /** 
         * No error.
         * This must be the first error all the time and the assigned value of 0 
         * must not be changed as it is assumed in the error message array that 
         * the error list starts with a value of 0.
         * Further, none of the error codes in this enum should not be initialized 
         * to an arbitrary value as it is assumed in the implementation when mapping 
         * error codes to error messages that the error codes are contiguous and 
         * the last error value is always AXIS2_ERROR_LAST.
         */
        AXIS2_ERROR_NONE = 0,

        /*
         * Group - Common Errors
         */
        
        /** Out of memory */
        AXIS2_ERROR_NO_MEMORY,

        /** NULL parameter was passed when a non NULL parameter was expected */
        AXIS2_ERROR_INVALID_NULL_PARAM,

        /*
         * Group - core:addr
         */

        /*
         * Group - core:clientapi
         */

        /** Blocking invocation expects response */
        AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE,

        /** cannot infer transport from URL */
        AXIS2_ERROR_CANNOT_INFER_TRANSPORT,

        /** Client side support only one configuration context */
        AXIS2_ERROR_CLIENT_SIDE_SUPPORT_ONLY_ONE_CONF_CTX,
        /* MEP cannot be NULL in MEP client */
        AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT,
        /* MEP Mismatch */
        AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT,

        /** Two way channel needs addressing module to be engaged */
        AXIS2_ERROR_TWO_WAY_CHANNEL_NEEDS_ADDRESSING,

        /** Unknown Transport */
        AXIS2_ERROR_UNKNOWN_TRANSPORT,
        /* Unsupported SOAP style */
        AXIS2_ERROR_UNSUPPORTED_TYPE,
        /* Options object is not set */
        AXIS2_ERROR_OPTIONS_OBJECT_IS_NOT_SET,
        /*
         * Group - core:clientapi:diclient
         */

        /*
         * Group - core:context
         */

        /** Invalid SOAP envelope state */
        AXIS2_ERROR_INVALID_SOAP_ENVELOPE_STATE,

        /** Invalid message context state  */
        AXIS2_ERROR_INVALID_STATE_MSG_CTX,

        /** Service accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_SVC,

        /** Service group accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_SVC_GRP,

        /** Service not yet found */
        AXIS2_ERROR_SERVICE_NOT_YET_FOUND,

        /*
         * Group - core:deployment
         */
        /* Invalid phase found in phase validation */
        AXI2_ERROR_INVALID_PHASE,
        /* axis2.xml cannot be found */
        AXIS2_ERROR_CONFIG_NOT_FOUND,
        /* Data element of the OM Node is null */
        AXIS2_ERROR_DATA_ELEMENT_IS_NULL,
        /* In transport sender, Inflow is not allowed */
        AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT,

        /** Invalid handler state */
        AXIS2_ERROR_INVALID_HANDLER_STATE,
        /* Invalid Module Ref encountered */
        AXIS2_ERROR_INVALID_MODUELE_REF,
        /* Invalid Module Reference by Operation */
        AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP,
        /* Invalid Module Configuration */
        AXIS2_ERROR_INVALID_MODULE_CONF,
        /* Description Builder is found to be in invalid state */
        AXIS2_ERROR_INVALID_STATE_DESC_BUILDER,
        /* Module Not Found */
        AXIS2_ERROR_MODULE_NOT_FOUND,
        /* Module Validation Failed */
        AXIS2_ERROR_MODULE_VALIDATION_FAILED,

        /** Module xml file is not found in the given path */
        AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE,
        /* No dispatcher found */
        AXIS2_ERROR_NO_DISPATCHER_FOUND,
        /* Operation name is missing */
        AXIS2_ERROR_OP_NAME_MISSING,
        /* In transport Receiver, Outflow is not allowed */
        AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN,
        /* Repository name cannot be NULL */
        AXIS2_ERROR_REPO_CAN_NOT_BE_NULL,
        /* Repository in path does not exist */
        AXIS2_ERROR_REPOSITORY_NOT_EXIST,
        /* Repository Listener initialization failed */
        AXIS2_ERROR_REPOS_LISTENER_INIT_FAILED,

        /** Service xml file is not found in the given path */
        AXIS2_ERROR_SERVICE_XML_NOT_FOUND,
        /* Service Name Error */
        AXIS2_ERROR_SVC_NAME_ERROR,
        /* Transport Sender Error */
        AXIS2_ERROR_TRANSPORT_SENDER_ERROR,
        /* Path to Config can not be NULL */
        AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL,
        /* Invalid Service */
        AXIS2_ERROR_INVALID_SVC,


        /*
         * Group - core:description
         */
        /* Cannot correlate message */
        AXIS2_ERROR_CANNOT_CORRELATE_MSG,

        /** Could not Map the MEP URI to a axis MEP constant value */
        AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT,
        /* Invalid message addition , operation context completed */
        AXIS2_ERROR_INVALID_MESSAGE_ADDITION,

        /** Module description accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_MODULE_DESC,

        /** Parameter container not set */
        AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER,

        /** module has already engaged to the op op terminated !!! */
        AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP,

        /** module has already been engaged on the service.Operation terminated !!! */
        AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC,

        /** module has already been engaged on the service. Group Operation terminated !!! */
        AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC_GRP,

        /** Parameter locked, Cannot override */
        AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
        /* schema list is empty or NULL in svc */
        AXIS2_ERROR_EMPTY_SCHEMA_LIST,
        /*
         * Group - core:engine
         */

        /** Both before and after handlers cannot be the same */
        AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME,

        /** Invalid handler rules  */
        AXIS2_ERROR_INVALID_HANDLER_RULES,
        /* Invalid Module */
        AXIS2_ERROR_INVALID_MODULE,

        /** Invalid first handler for phase  */
        AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER,

        /** Invalid last handler for phase  */
        AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER,

        /** Invalid engine config state */
        AXIS2_ERROR_INVALID_STATE_CONF,

        /** Message context processing a fault already */
        AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY,

        /** fault to field not specified in message context */
        AXIS2_ERROR_NOWHERE_TO_SEND_FAULT,

        /** Only one handler allowed for phase, adding handler is not allowed  */
        AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID,

        /** First handler of phase already set */
        AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET,

        /** Last handler of phase already set */
        AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET,

        /**Two service can not have same name, a service with same name already
           exist in the system */
        AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME,
        /*
         * Group - core:phase resolver
         */
        /* Invalid Module Ref */
        AXIS2_ERROR_INVALID_MODULE_REF,
        /* Invalid Phase */
        AXIS2_ERROR_INVALID_PHASE,
        /* No Transport Receiver is configured */
        AXIS2_ERROR_NO_TRANSPORT_IN_CONFIGURED,
        /* No Transport Sender is configured */
        AXIS2_ERROR_NO_TRANSPORT_OUT_CONFIGURED,
        /* Phase is not specified */
        AXIS2_ERROR_PHASE_IS_NOT_SPECIFED,
        /* Service module can not refer global phase */
        AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE,

        /*
         * Group - core:wsdl
         */

        /**  Schema is NULL */
        AXIS2_ERROR_WSDL_SCHEMA_IS_NULL,
        /*
         * Group - core:receivers
         */
        /* Om Element has invalid state */
        AXIS2_ERROR_OM_ELEMENT_INVALID_STATE,
        /* Om Elements do not match */
        AXIS2_ERROR_OM_ELEMENT_MISMATCH,
        /* RPC style SOAP body don't have a child element */
        AXIS2_ERROR_RPC_NEED_MATCHING_CHILD,
        /* Operation Description has unknown operation style  */
        AXIS2_ERROR_UNKNOWN_STYLE,
        /* String does not represent a valid NCName */
        AXIS2_ERROR_STRING_DOES_NOT_REPRESENT_A_VALID_NC_NAME,
        /*
         * Group - core:transport
         */

        /*
         * Group - core:transport:http
         */
        /* Error occurred in transport */
        AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,

        /** A read attempt(HTTP) for the reply without sending the request */
        AXIS2_ERROR_HTTP_REQUEST_NOT_SENT,

        /** Invalid string passed as a http header */
        AXIS2_ERROR_INVALID_HEADER,
        /* Invalid header start line (request line or response line) */
        AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE,
        /* Transport protocol is unsupported by axis2 */
        AXIS2_ERROR_INVALID_TRANSPORT_PROTOCOL,

        /** No body present in the request or the response */
        AXIS2_ERROR_NULL_BODY,
        /* A valid conf_ctx is reqd for the http worker */
        AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
        /* HTTP version cannot be null in the status/request line */
        AXIS2_ERROR_NULL_HTTP_VERSION,
        /* Input stream is NULL in msg_ctx */
        AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX,
        /* OM output is NULL */
        AXIS2_ERROR_NULL_OM_OUTPUT,
        /* Null SOAP envelope in msg_ctx */
        AXIS2_ERROR_NULL_SOAP_ENVELOPE_IN_MSG_CTX,
        /* NULL stream in the http chucked stream */
        AXIS2_ERROR_NULL_STREAM_IN_CHUNKED_STREAM,
        /* We got a NULL stream in the response body */
        AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY,

        /** URL NULL in http client */
        AXIS2_ERROR_NULL_URL,
        /** Invalid URL format */
        AXIS2_ERROR_INVALID_URL_FORMAT,
        /** Duplicate URL REST Mapping */
        AXIS2_ERROR_DUPLICATE_URL_REST_MAPPING,
        /* We need transport_info in msg_ctx */
        AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL,
        /*Content-Type header missing in HTTP response" */
        AXIS2_ERROR_RESPONSE_CONTENT_TYPE_MISSING,

        /** Response timed out */
        AXIS2_ERROR_RESPONSE_TIMED_OUT,

        /** Server shutdown*/
        AXIS2_ERROR_RESPONSE_SERVER_SHUTDOWN,

        /** SOAP envelope or SOAP body NULL */
        AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
        /* Error occurred in SSL engine */
        AXIS2_ERROR_SSL_ENGINE,
        /* Either axis2c cannot find certificates or the env variable is not set */
        AXIS2_ERROR_SSL_NO_CA_FILE,
        /* Error in writing the response in response writer */
        AXIS2_ERROR_WRITING_RESPONSE,
        /* Required parameter is missing in URL encoded request */
        AXIS2_ERROR_REQD_PARAM_MISSING,
        /* Unsupported schema type in REST */
        AXIS2_ERROR_UNSUPPORTED_SCHEMA_TYPE,
        /* Service or operation not found */
        AXIS2_ERROR_SVC_OR_OP_NOT_FOUND,
        /*
         * Group - mod_addr
         */
        AXIS2_ERROR_NO_MSG_INFO_HEADERS,
        /*
         * Group - platforms
         */

        /*
         * Group - utils
         */

        /** Could not open the axis2 file */
        AXIS2_ERROR_COULD_NOT_OPEN_FILE,
        /* Failed in creating DLL */
        AXIS2_ERROR_DLL_CREATE_FAILED,
        /* DLL loading failed */
        AXIS2_ERROR_DLL_LOADING_FAILED,

        /** Environment passed is null */
        AXIS2_ERROR_ENVIRONMENT_IS_NULL,
        /* Axis2 File does not have a file name */
        AXIS2_ERROR_FILE_NAME_NOT_SET,
        /* DLL Description Info Object has invalid state */
        AXIS2_ERROR_INVALID_STATE_DLL_DESC,
        /* Failed in creating Handler */
        AXIS2_ERROR_HANDLER_CREATION_FAILED,

        /** Array list index out of bounds */
        AXIS2_ERROR_INDEX_OUT_OF_BOUNDS,

        /** Invalid IP or host name */
        AXIS2_ERROR_INVALID_ADDRESS,

        /** Trying to do operation on invalid file descriptor */
        AXIS2_ERROR_INVALID_FD,

        /** Trying to do operation on closed/not opened socket */
        AXIS2_ERROR_INVALID_SOCKET,

        /** Parameter not set */
        AXIS2_ERROR_INVALID_STATE_PARAM,
        /* Module create failed */
        AXIS2_ERROR_MODULE_CREATION_FAILED,
        /* Failed in creating Message Receiver */
        AXIS2_ERROR_MSG_RECV_CREATION_FAILED,

        /** No such element */
        AXIS2_ERROR_NO_SUCH_ELEMENT,

        /** Socket bind failed. Another process may be already using this port*/
        AXIS2_ERROR_SOCKET_BIND_FAILED,

        /** Error creating a socket. Most probably error returned by OS */
        AXIS2_ERROR_SOCKET_ERROR,
        /* Listen failed for the server socket */
        AXIS2_ERROR_SOCKET_LISTEN_FAILED,
        /* Failed in creating Service Skeleton */
        AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED,
        /* Failed in creating Transport Receiver */
        AXIS2_ERROR_TRANSPORT_RECV_CREATION_FAILED,
        /* Failed in creating Transport Sender */
        AXIS2_ERROR_TRANSPORT_SENDER_CREATION_FAILED,
        /* Generation of platform dependent uuid failed */
        AXIS2_ERROR_UUID_GEN_FAILED,
        /* Possible deadlock */
        AXIS2_ERROR_POSSIBLE_DEADLOCK,
        /*
         * Group - WSDL
         */
        /* Interface or Port Type not found for the binding */
        AXIS2_ERROR_INTERFACE_OR_PORT_TYPE_NOT_FOUND_FOR_THE_BINDING,
        /* Interfaces or Ports not found for the partially built WOM */
        AXIS2_ERROR_INTERFACES_OR_PORTS_NOT_FOUND_FOR_PARTIALLY_BUILT_WOM,

        /** Wsdl op accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_WSDL_OP,

        /** Wsdl Service accessed has invalid state */
        AXIS2_ERROR_INVALID_STATE_WSDL_SVC,
        /* Cannot determine MEP */
        AXIS2_ERROR_MEP_CANNOT_DETERMINE_MEP,
        /* Wsdl binding name cannot be NULL(Is required) */
        AXIS2_ERROR_WSDL_BINDING_NAME_IS_REQUIRED,
        /* PortType/Interface name cannot be null(Required) */
        AXIS2_ERROR_WSDL_INTERFACE_NAME_IS_REQUIRED,
        /* Wsdl parsing has resulted in an invalid state */
        AXIS2_ERROR_WSDL_PARSER_INVALID_STATE,
        /* Wsdl svc name cannot be null(Required) */
        AXIS2_ERROR_WSDL_SVC_NAME_IS_REQUIRED,
        /*
         * Group - xml
         */

        /*
         * Group - xml:attachments
         */
        /** Attachment is missing */
        AXIS2_ERROR_ATTACHMENT_MISSING,

        /*
         * Group - xml:om
         */

        /** Builder done with pulling. Cannot pull any more */
        AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL,

        /** Discard failed because the builder state is invalid */
        AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD,

        /** Builder's last node is NULL when it is not supposed to be NULL */
        AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL,

        /** Document root is NULL, when it is not supposed to be NULL */
        AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL,

        /** Undefined namespace used */
        AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE,

        /** error a namespace should have a valid uri */
        AXIS2_ERROR_INVALID_EMPTY_NAMESPACE_URI,

        /** next method has not been called so cannot remove
        an element before calling next valid for any om iterator */
        AXIS2_ERROR_ITERATOR_NEXT_METHOD_HAS_NOT_YET_BEEN_CALLED,

        /** remove method has already been called once so cannot remove till
        next method is called valid for any om children iterator*/
        AXIS2_ERROR_ITERATOR_REMOVE_HAS_ALREADY_BEING_CALLED,

        /** axiom_xml_reader returned NULL element */
        AXIS2_ERROR_XML_READER_ELEMENT_NULL,

        /** axiom_xml_reader returned NULL value */
        AXIS2_ERROR_XML_READER_VALUE_NULL,
        /*
         * Group - xml:parser
         */

        /** error occurred creating xml stream reader */
        AXIS2_ERROR_CREATING_XML_STREAM_READER,

        /** error occurred creating xml stream writer */
        AXIS2_ERROR_CREATING_XML_STREAM_WRITER,

        /** error in writing attribute */
        AXIS2_ERROR_WRITING_ATTRIBUTE,

        /** error in writing attribute with namespace */
        AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE,

        /** error in writing attribute with namespace prefix */
        AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX,

        /** error in writing comment */
        AXIS2_ERROR_WRITING_COMMENT,

        /** error in writing data source*/
        AXIS2_ERROR_WRITING_DATA_SOURCE,

        /** error in writing default namespace */
        AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE,

        /** error in writing DDT */
        AXIS2_ERROR_WRITING_DTD,

        /** error occurred in writing empty element */
        AXIS2_ERROR_WRITING_EMPTY_ELEMENT,

        /** error occurred in writing empty element with namespace */
        AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE,

        /** error in writing empty element with namespace prefix */
        AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX,

        /** error occurred in writing end document in xml writer */
        AXIS2_ERROR_WRITING_END_DOCUMENT,

        /** error occurred in writing end element in xml writer */
        AXIS2_ERROR_WRITING_END_ELEMENT,

        /** error in writing processing instruction */
        AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION,

        /** error occurred in writing start element in start document in xml writer */
        AXIS2_ERROR_WRITING_START_DOCUMENT,

        /** error occurred in writing start element in xml writer */
        AXIS2_ERROR_WRITING_START_ELEMENT,

        /** error occurred in writing start element with namespace in xml writer*/
        AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE,

        /** error occurred in writing start element with namespace prefix */
        AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX,

        /** error in writing cdata section */
        AXIS2_ERROR_WRITING_CDATA,

        /** AXIS2_XML_PARSER_TYPE_BUFFER or AXIS2_XML_PARSER_TYPE_DOC is expected */
        AXIS2_ERROR_XML_PARSER_INVALID_MEM_TYPE,

        /*
         * Group - xml:SOAP
         */

        /** invalid type passed */
        AXIS2_ERROR_INVALID_BASE_TYPE,

        /** invalid SOAP namespace uri found */
        AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI,

        /** Invalid SOAP version */
        AXIS2_ERROR_INVALID_SOAP_VERSION,
        /* invalid value found in must understand attribute */
        AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND,
        /*multiple code elements encountered in SOAP fault element */
        AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED,
        /*multiple detail elements encountered in SOAP fault element */
        AXIS2_ERROR_MULTIPLE_DETAIL_ELEMENTS_ENCOUNTERED,
        /*multiple node elements encountered in SOAP fault element */
        AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED,
        /*multiple reason elements encountered in SOAP fault element */
        AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED,
        /*multiple role elements encountered in SOAP fault element */
        AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED,
        /*multiple sub code values encountered in SOAP fault element */
        AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED,
        /* multiple value elements encountered */
        AXIS2_ERROR_MULTIPLE_VALUE_ENCOUNTERED_IN_CODE_ELEMENT,
        /* must understand attribute should have values of true or false */
        AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE,

        /** om element is expected */
        AXIS2_ERROR_OM_ELEMENT_EXPECTED,
        /* processing SOAP 1.1 fault value element should have only
           text as its children */
        AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE,

        /** only one SOAP fault allowed in SOAP body */
        AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY,
        /*SOAP 1.1 fault actor element should not have child elements */
        AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS,

        /** SOAP builder found a child element other than header or body in envelope
        element */
        AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY,

        /** SOAP builder encountered body element first and header next */
        AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER,

        /** SOAP builder multiple body elements encountered */
        AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED,

        /** SOAP builder encountered multiple headers */
        AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED,
        /*SOAP fault code element should a mandatory fault value element */
        AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE,
        /*SOAP fault reason element should have a text */
        AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT,
        /*SOAP fault role element should have a text value */
        AXIS2_ERROR_SOAP_FAULT_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT,
        /* SOAP fault value should be present before sub code element in SOAP fault code */
        AXIS2_ERROR_SOAP_FAULT_VALUE_SHOULD_BE_PRESENT_BEFORE_SUB_CODE,

        /** SOAP message does not have an envelope element */
        AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE,
        /*SOAP message first element must contain a localname */
        AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME,
        /* this localname is not supported inside a reason element */
        AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_REASON_ELEMENT,
        /*this localname is not supported inside a sub code element */
        AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_SUB_CODE_ELEMENT,
        /*this localname is not supported inside the code element */
        AXIS2_ERROR_THIS_LOCALNAME_NOT_SUPPORTED_INSIDE_THE_CODE_ELEMENT,
        /*transport level identified SOAP version does not match with the SOAP version */
        AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP,
        /*unsupported element in SOAP fault element */
        AXIS2_ERROR_UNSUPPORTED_ELEMENT_IN_SOAP_FAULT_ELEMENT,
        /*wrong element order encountered */
        AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED,
        /*
         * Group - services
         */

        /** Invalid XML format in request */
        AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST,

        /** Input OM node NULL, Probably error in SOAP request */
        AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL,

        /** Invalid parameters for service operation in SOAP request */
        AXIS2_ERROR_SVC_SKEL_INVALID_OPERATION_PARAMETERS_IN_SOAP_REQUEST,

        /*
         * Group - repos
         */
        /* not authenticated */
        AXIS2_ERROR_REPOS_NOT_AUTHENTICATED,
        /* unsupported mode */
        AXIS2_ERROR_REPOS_UNSUPPORTED_MODE,
        /* expired */
        AXIS2_ERROR_REPOS_EXPIRED,
        /* not implemented */
        AXIS2_ERROR_REPOS_NOT_IMPLEMENTED,
        /* not found */
        AXIS2_ERROR_REPOS_NOT_FOUND,
        /* bad search text */
        AXIS2_ERROR_REPOS_BAD_SEARCH_TEXT,

        /*
         *Group - Neethi
         */
        /*  No Namespace */
        AXIS2_ERROR_NEETHI_ELEMENT_WITH_NO_NAMESPACE,
        /*Policy cannot be created from element */
        AXIS2_ERROR_NEETHI_POLICY_CREATION_FAILED_FROM_ELEMENT,
        /*All Cannot be created from element */
        AXIS2_ERROR_NEETHI_ALL_CREATION_FAILED_FROM_ELEMENT,
        /*Exactly one Cannot be created element */
        AXIS2_ERROR_NEETHI_EXACTLYONE_CREATION_FAILED_FROM_ELEMENT,
        /*Reference Cannot be created from element */
        AXIS2_ERROR_NEETHI_REFERENCE_CREATION_FAILED_FROM_ELEMENT,
        /*Assertion Cannot be created from element */
        AXIS2_ERROR_NEETHI_ASSERTION_CREATION_FAILED_FROM_ELEMENT,
        /*All creation failed */
        AXIS2_ERROR_NEETHI_ALL_CREATION_FAILED,
        /*Exactly one creation failed */
        AXIS2_ERROR_NEETHI_EXACTLYONE_CREATION_FAILED,
        /*Policy Creation failed */
        AXIS2_ERROR_NEETHI_POLICY_CREATION_FAILED,
        /*Normalization failed */
        AXIS2_ERROR_NEETHI_NORMALIZATION_FAILED,
        /*Merging Failed. Wrong Input */
        AXIS2_ERROR_NEETHI_WRONG_INPUT_FOR_MERGE,
        /*Merging Failed. Cross Product failed */
        AXIS2_ERROR_NEETHI_CROSS_PRODUCT_FAILED,
        /*No Children Policy Components */
        AXIS2_ERROR_NEETHI_NO_CHILDREN_POLICY_COMPONENTS,
        /*Uri Not specified */
        AXIS2_ERROR_NEETHI_URI_NOT_SPECIFIED,
        /*Policy NULL for the given uri */
        AXIS2_ERROR_NEETHI_NO_ENTRY_FOR_THE_GIVEN_URI,
        /*Exactly one not found in Normalized policy */
        AXIS2_ERROR_NEETHI_EXACTLYONE_NOT_FOUND_IN_NORMALIZED_POLICY,
        /*Exactly one is Empty */
        AXIS2_ERROR_NEETHI_EXACTLYONE_IS_EMPTY,
        /*Exactly one not found while getting cross product */
        AXIS2_ERROR_NEETHI_ALL_NOT_FOUND_WHILE_GETTING_CROSS_PRODUCT,
        /*Unknown Assertion*/
        AXIS2_ERROR_NEETHI_UNKNOWN_ASSERTION,
        /** 
         * The following has to be the last error value all the time.
         * All other error codes should appear above this.
         * AXIS2_ERROR_LAST is used to track the number of error codes present
         * for the purpose of sizing the error messages array.
         */
        AXIS2_ERROR_LAST
    };

    struct axutil_error;
    typedef enum axis2_status_codes axis2_status_codes_t;
    typedef enum axutil_error_codes axutil_error_codes_t;

    /**
     * @defgroup axutil_error error
     * @ingroup axis2_util
     * @{
     */

    /**
     * Axutil error struct.
     * Error holds the last error number, the status code as well as the 
     * last error message.
     */
    typedef struct axutil_error
    {
        /** 
         * Memory allocator associated with the error struct. 
         * It is this allocator that would be used to allocate memory 
         * for the error struct instance in create method.
         */
        axutil_allocator_t *allocator;

        /** Last error number. */
        int error_number;
        /** Last status code. */
        int status_code;
        /** 
         * Error message. This could be set to a custom message to be 
         * returned, instead of standard errors set in the error messages 
         * array by the axutil_error_init function call.
         */
        axis2_char_t *message;
    }
    axutil_error_t;

    /**
     * Gets the error message corresponding to the last error occurred.
     * @param error pointer to error struct 
     * @return string representing the error message for the last error occurred
     */
    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axutil_error_get_message(
        const struct axutil_error *error);

    /**
     * This function is supposed to be overridden in an extended error structure.
     * For example in Sandesha error structure this function is overridden so that
     * errors of axis2 range call the get_message function of error struct but
     * errors of sandesha2 range get the messages from an array of that struct.
     * @return error message for the extended struct.
     * @deprecated this function is not in use, so should be removed.
     */
    /*AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axutil_error_get_extended_message(
        const struct axutil_error *error);*/

    /**
     * Sets the error number.
     * @param error pointer to error struct 
     * @param error_number error number to be set
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_error_set_error_number(
        struct axutil_error *error,
        axutil_error_codes_t error_number);

    /**
     * Sets the status code.
     * @param error pointer to error struct 
     * @param status_code status code to be set
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_error_set_status_code(
        struct axutil_error *error,
        axis2_status_codes_t status_code);

    /**
     * Gets the status code.
     * @param error pointer to error struct 
     * @return last status code set on error struct
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_error_get_status_code(
        struct axutil_error *error);

    /**
     * Sets error message to the given value.
     * @param error pointer to error struct 
     * @param message error message to be set
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_error_set_error_message(
        struct axutil_error *error,
        axis2_char_t *message);

    /**
     * Initializes the axutil_error_messages array. This array holds the 
     * error messages that corresponds to the error codes. This function
     * must be call before using the error struct instance.
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_error_init(void);

    /**
     * De-allocates an error struct instance.
     * @param error pointer to error struct instance to be freed.
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axutil_error_free(
        struct axutil_error *error);

/** 
 * @deprecated The following macros are no longer useful as we can use the 
 * function calls directly. Hence these macros should be removed 
 */
  
#define AXIS2_ERROR_FREE(error) axutil_error_free(error)

#define AXIS2_ERROR_GET_MESSAGE(error) \
    axutil_error_get_message(error)

#define AXIS2_ERROR_SET_MESSAGE(error, message) \
    axutil_error_set_error_message(error, message)

#define AXIS2_ERROR_SET_ERROR_NUMBER(error, error_number) \
        axutil_error_set_error_number(error, error_number)

#define AXIS2_ERROR_SET_STATUS_CODE(error, status_code) \
        axutil_error_set_status_code(error, status_code)

#define AXIS2_ERROR_GET_STATUS_CODE(error) axutil_error_get_status_code(error)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ERROR_H */
