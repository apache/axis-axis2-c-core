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
 
 #ifndef AXIS2_SOAP_CONSTANTS_H
 #define AXIS2_SOAP_CONSTANTS_H
 
 /**
 * @file axis2_soap_constants.h
 * @brief defines SOAP constants
 */
#ifdef __cplusplus
extern "C"
{
#endif

/** soap 11 constants */

#define SOAP11_SOAP_ENVELOPE_NAMESPACE_URI "http://schemas.xmlsoap.org/soap/envelope/"

#define SOAP11_ATTR_ACTOR "actor"

#define SOAP11_SOAP_FAULT_CODE_LOCAL_NAME "faultcode"

#define SOAP11_SOAP_FAULT_STRING_LOCAL_NAME "faultstring"

#define SOAP11_SOAP_FAULT_ACTOR_LOCAL_NAME "faultactor"

#define SOAP11_SOAP_FAULT_DETAIL_LOCAL_NAME "detail"

#define SOAP_11_CONTENT_TYPE "text/xml"

#define SOAP11_FAULT_CODE_SENDER "Client"

#define SOAP11_FAULT_CODE_RECEIVER "Server"

#define SOAP11_SOAP_ACTOR_NEXT "http://schemas.xmlsoap.org/soap/actor/next"


/** soap12 constants */

#define SOAP12_SOAP_ENVELOPE_NAMESPACE_URI "http://www.w3.org/2003/05/soap-envelope"

#define SOAP12_SOAP_ROLE "role"

#define SOAP12_SOAP_RELAY "relay"

#define SOAP12_SOAP_FAULT_CODE_LOCAL_NAME "Code"

#define SOAP12_SOAP_FAULT_SUB_CODE_LOCAL_NAME "Subcode"

#define SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME "Value"

#define SOAP12_SOAP_FAULT_VALUE_VERSION_MISMATCH "VersionMismatch"

#define SOAP12_SOAP_FAULT_VALUE_MUST_UNDERSTAND "MustUnderstant"

#define SOAP12_SOAP_FAULT_VALUE_DATA_ENCODING_UKNOWN "DataEncodingUnknown"

#define SOAP12_SOAP_FAULT_VALUE_SENDER "Sender"

#define SOAP12_SOAP_FAULT_VALUE_RECEIVER "Receiver"

/** SOAP Fault Reason */

#define SOAP12_SOAP_FAULT_REASON_LOCAL_NAME "Reason"

#define SOAP12_SOAP_FAULT_TEXT_LOCAL_NAME "Text"

#define SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_LOCAL_NAME "lang"

#define SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_URI "http://www.w3.org/XML/1998/namespace"

#define SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_PREFIX "xml"

#define SOAP12_SOAP_FAULT_NODE_LOCAL_NAME "Node"

#define SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME "Detail"

#define SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME "Role"

#define SOAP_12_CONTENT_TYPE "application/soap+xml"

/** soap fault codes */

#define SOAP12_FAULT_CODE_SENDER "Sender"

#define SOAP12_FAULT_CODE_RECEIVER "Receiver"

#define SOAP12_SOAP_ROLE_NEXT "http://www.w3.org/2003/05/soap-envelope/role/next"

#define SOAP12_SOAP_ROLE_NONE "http://www.w3.org/2003/05/soap-envelope/role/none"

#define SOAP12_SOAP_ROLE_ULTIMATE_RECEIVER "http://www.w3.org/2003/05/soap-envelope/role/ultimateReceiver"

#ifdef __cplusplus
}
#endif 

#endif /* AXIS2_SOAP_CONSTANTS_H */