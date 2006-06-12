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
 
#ifndef WODEN_CONSTANTS_H
#define WODEN_CONSTANTS_H

/**
 * @file woden_constants.h
 * @brief Axis2 Xml Woden Constants
 * Constants for WSDL 2.0 elements, attributes and URIs.
 */
 
#include <axis2_utils.h>

#ifdef __cplusplus
extern "C" 
{
#endif
    
/** @defgroup woden_constants 
  * @ingroup woden
  * @{
  */
/*********************************** Constansts********************************/
/* Namespace URIs. */
#define WODEN_NS_URI_WSDL20 "http://www.w3.org/2006/01/wsdl"
#define WODEN_NS_URI_XMLNS "http://www.w3.org/2000/xmlns/"
#define WODEN_NS_URI_XSI "http://www.w3.org/2001/XMLSchema-instance"

/* Top-level WSDL 2.0 element names. */
#define WODEN_ELEM_DESCRIPTION "description"
#define WODEN_ELEM_DOCUMENTATION "documentation"
#define WODEN_ELEM_IMPORT "import"
#define WODEN_ELEM_INCLUDE "include"
#define WODEN_ELEM_TYPES "types"
#define WODEN_ELEM_INTERFACE "interface"
#define WODEN_ELEM_BINDING "binding"
#define WODEN_ELEM_SERVICE "service"

/* Nested WSDL 2.0 element names. */
#define WODEN_ELEM_FAULT "fault"
#define WODEN_ELEM_OPERATION "operation"
#define WODEN_ELEM_INPUT "input"
#define WODEN_ELEM_OUTPUT "output"
#define WODEN_ELEM_INFAULT "infault"
#define WODEN_ELEM_OUTFAULT "outfault"
#define WODEN_ELEM_FEATURE "feature"
#define WODEN_ELEM_PROPERTY "property"
#define WODEN_ELEM_VALUE "value"
#define WODEN_ELEM_CONSTRAINT "constraint"
#define WODEN_ELEM_ENDPOINT "endpoint"

/* Top-level WSDL 2.0 qualified element names. */
#define WODEN_Q_ELEM_DESCRIPTION "description|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_DOCUMENTATION "documentation|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_IMPORT "import|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_INCLUDE "include|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_TYPES "types|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_INTERFACE "interface|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_BINDING "binding|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_SERVICE "service|http://www.w3.org/2006/01/wsdl"

/* Nested WSDL 2.0 qualified element names. */
#define WODEN_Q_ELEM_FAULT "fault|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_OPERATION "operation|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_INPUT "input|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_OUTPUT "output|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_INFAULT "infault|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_OUTFAULT "outfault|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_FEATURE "feature|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_PROPERTY "property|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_VALUE "value|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_CONSTRAINT "constraint|http://www.w3.org/2006/01/wsdl"
#define WODEN_Q_ELEM_ENDPOINT "endpoint|http://www.w3.org/2006/01/wsdl"

/* Attribute names. */
#define WODEN_ATTR_ID "id"
#define WODEN_ATTR_NAME "name"
#define WODEN_ATTR_TARGET_NAMESPACE "targetNamespace"
#define WODEN_ATTR_NAMESPACE "namespace"
#define WODEN_ATTR_XMLNS "xmlns"
#define WODEN_ATTR_EXTENDS "extends"
#define WODEN_ATTR_STYLE_DEFAULT "styleDefault"
#define WODEN_ATTR_ELEMENT "element"
#define WODEN_ATTR_PATTERN "pattern"
#define WODEN_ATTR_STYLE "style"
#define WODEN_ATTR_MESSAGE_LABEL "messageLabel"
#define WODEN_ATTR_REF "ref"
#define WODEN_ATTR_REQUIRED "required"
#define WODEN_ATTR_INTERFACE "interface"
#define WODEN_ATTR_TYPE "type"
#define WODEN_ATTR_BINDING "binding"
#define WODEN_ATTR_ADDRESS "address"
#define WODEN_ATTR_LOCATION "location"

/* Attribute values and NMTokens */
#define WODEN_VALUE_EMPTY_STRING ""
#define WODEN_VALUE_TRUE "true"
#define WODEN_VALUE_FALSE "false"
#define WODEN_NMTOKEN_VALUE "#value"
#define WODEN_NMTOKEN_ANY "#any"
#define WODEN_NMTOKEN_NONE "#none"
#define WODEN_NMTOKEN_OTHER "#other"
#define WODEN_NMTOKEN_ELEMENT "#element"

/* Constants representing the values of the properties used to 
 * configure the Woden runtime (i.e. different to WSDL 2.0 properties).
 * These typically describe the standards, APIs, etc, supported by this 
 * implementation of the Woden API.
 * 
 * The first part of the constant name indicates its usage:
 * TYPE_  describes a supported type system, such as the W3C XML Schema.
 * API_   describes a supported external API, such as DOM
 */ 
#define WODEN_TYPE_XSD_2001 "http://www.w3.org/2001/XMLSchema"
#define WODEN_API_W3C_DOM "org.w3c.dom"
#define WODEN_API_W3C_XS "http://www.w3.org/Submission/xmlschema-api/"   /*XML Schema API implemented in Xerces */
#define WODEN_API_APACHE_WS_XS "org.apache.ws.commons.schema"   /*Apache WS-Commons XmlSchema */


/* Qualified attribute names.*/
#define WODEN_Q_ATTR_REQUIRED "required|http://www.w3.org/2006/01/wsdl"

/* XML Declaration string.*/
#define WODEN_XML_DECL_DEFAULT "UTF-8"
#define WODEN_XML_DECL_START "<?xml version=\"1.0\" encoding=\""
#define WODEN_XML_DECL_END "\"?>"

/* Feature names. */
/* TODO org.apache and import optionality */
#define WODEN_FEATURE_VERBOSE "verbose"
#define WODEN_FEATURE_IMPORT_DOCUMENTS "import_documents"



/*********************************** Constants*********************************/	
	
	
#ifdef __cplusplus
}
#endif

#endif /* WODEN_CONSTANTS_H */


