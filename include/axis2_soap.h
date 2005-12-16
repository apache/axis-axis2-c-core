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
 
 #ifndef AXIS2_SOAP_H
 #define AXIS2_SOAP_H
 
 /**
 * @file axis2_soap.h
 * @brief defines SOAP constants
 */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2 soap constants
 * @ingroup axis2_soap 
 * @{
 */

/** soap 11 constants */

#define AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI "http://schemas.xmlsoap.org/soap/envelope/"

#define AXIS2_SOAP11_ATTR_ACTOR "actor"

#define AXIS2_SOAP11_SOAP_FAULT_CODE_LOCAL_NAME "faultcode"

#define AXIS2_SOAP11_SOAP_FAULT_STRING_LOCAL_NAME "faultstring"

#define AXIS2_SOAP11_SOAP_FAULT_ACTOR_LOCAL_NAME "faultactor"

#define AXIS2_SOAP11_SOAP_FAULT_DETAIL_LOCAL_NAME "detail"

#define AXIS2_SOAP_11_CONTENT_TYPE "text/xml"

#define AXIS2_SOAP11_FAULT_CODE_SENDER "Client"

#define AXIS2_SOAP11_FAULT_CODE_RECEIVER "Server"

#define AXIS2_SOAP11_SOAP_ACTOR_NEXT "http://schemas.xmlsoap.org/soap/actor/next"


/** soap12 constants */

#define AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI "http://www.w3.org/2003/05/soap-envelope"

#define AXIS2_SOAP12_SOAP_ROLE "role"

#define AXIS2_SOAP12_SOAP_RELAY "relay"

#define AXIS2_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME "Code"

#define AXIS2_SOAP12_SOAP_FAULT_SUB_CODE_LOCAL_NAME "Subcode"

#define AXIS2_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME "Value"

#define AXIS2_SOAP12_SOAP_FAULT_VALUE_VERSION_MISMATCH "VersionMismatch"

#define AXIS2_SOAP12_SOAP_FAULT_VALUE_MUST_UNDERSTAND "MustUnderstant"

#define AXIS2_SOAP12_SOAP_FAULT_VALUE_DATA_ENCODING_UKNOWN "DataEncodingUnknown"

#define AXIS2_SOAP12_SOAP_FAULT_VALUE_SENDER "Sender"

#define AXIS2_SOAP12_SOAP_FAULT_VALUE_RECEIVER "Receiver"

/** SOAP Fault Reason */

#define AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME "Reason"

#define AXIS2_SOAP12_SOAP_FAULT_TEXT_LOCAL_NAME "Text"

#define AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_LOCAL_NAME "lang"

#define AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_URI "http://www.w3.org/XML/1998/namespace"

#define AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_PREFIX "xml"

#define AXIS2_SOAP12_SOAP_FAULT_NODE_LOCAL_NAME "Node"

#define AXIS2_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME "Detail"

#define AXIS2_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME "Role"

#define AXIS2_SOAP_12_CONTENT_TYPE "application/soap+xml"

/** soap fault codes */

#define AXIS2_SOAP12_FAULT_CODE_SENDER "Sender"

#define AXIS2_SOAP12_FAULT_CODE_RECEIVER "Receiver"

#define AXIS2_SOAP12_SOAP_ROLE_NEXT "http://www.w3.org/2003/05/soap-envelope/role/next"

#define AXIS2_SOAP12_SOAP_ROLE_NONE "http://www.w3.org/2003/05/soap-envelope/role/none"

#define SOAP12_SOAP_ROLE_ULTIMATE_RECEIVER "http://www.w3.org/2003/05/soap-envelope/role/ultimateReceiver"

#define AXIS2_SOAP_DEFAULT_NAMESPACE_PREFIX "soapenv"

#define AXIS2_SOAP_ENVELOPE_LOCAL_NAME "Envelope"

#define AXIS2_SOAP_HEADER_LOCAL_NAME "Header"

#define AXIS2_SOAP_BODY_LOCAL_NAME "Body"

#define AXIS2_SOAP_BODY_NAMESPACE_PREFIX AXIS2_SOAP_DEFAULT_NAMESPACE_PREFIX

#define AXIS2_SOAP_BODY_FAULT_LOCAL_NAME "Fault"

/** attribute must understand */

#define AXIS2_SOAP_ATTR_MUST_UNDERSTAND "mustUnderstand"

#define AXIS2_SOAP_ATTR_MUST_UNDERSTAND_TRUE "true"

#define AXIS2_SOAP_ATTR_MUST_UNDERSTAND_FALSE "false"

#define AXIS2_SOAP_ATTR_MUST_UNDERSTAND_0 "0"

#define AXIS2_SOAP_ATTR_MUST_UNDERSTAND_1 "1"

#define AXIS2_SOAP_FAULT_LOCAL_NAME "Fault"

#define AXIS2_SOAP_FAULT_DETAIL_LOCAL_NAME "detail"

#define AXIS2_SOAP_FAULT_NAMESPACE_PREFIX AXIS2_SOAP_DEFAULT_NAMESPACE_PREFIX

#define AXIS2_SOAP_FAULT_DETAIL_EXCEPTION_ENTRY "Exception"

#define AXIS2_SOAP_FAULT_CODE_VERSION_MISMATCH "env:VersionMismatch"

#define AXIS2_SOAP_FAULT_CODE_MUST_UNDERSTAND "env:MustUnderstand"

#define AXIS2_SOAP_FAULT_CODE_DATA_ENCODING_UNKNOWN "env:DataEncodingUnknown"

#define AXIS2_SOAP_FAULT_CODE_SENDER ""

#define AXIS2_SOAP_FAULT_CODE_RECEIVER ""

/** @} */

#ifdef __cplusplus
}
#endif 

#endif /* AXIS2_SOAP_CONSTANTS_H */