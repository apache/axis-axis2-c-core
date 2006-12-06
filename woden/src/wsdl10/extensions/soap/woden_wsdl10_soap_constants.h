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
 
#ifndef WODEN_WSDL10_SOAP_CONSTANTS_H
#define WODEN_WSDL10_SOAP_CONSTANTS_H

/**
 * @file axiom_soap_constants.h
 * @brief Axis2 Xml Soap Constants
 *          Constants for XML Soap elements, attributes and URIs.
 */
 
#include <axis2_utils.h>

#ifdef __cplusplus
extern "C" 
{
#endif
    
/** @defgroup woden_soap_constants 
  * @ingroup woden_soap
  * @{
  */
/*********************************** Constansts********************************/

/* Namespace URIs.*/
#define WODEN_WSDL10_NS_URI_SOAP "http://schemas.xmlsoap.org/wsdl/soap/"

/* Protocol URIs.*/
#define WODEN_WSDL10_PROTOCOL_URI_HTTP "http://www.w3.org/2003/05/soap/bindings/HTTP/"

/* Element names. */
#define WODEN_WSDL10_ELEM_MODULE "module"
#define WODEN_WSDL10_ELEM_HEADER "header"

/* Attribute names. */
#define WODEN_WSDL10_ATTR_VERSION "version"
#define WODEN_WSDL10_ATTR_PROTOCOL "protocol"
#define WODEN_WSDL10_ATTR_MEPDEFAULT "mepDefault"
#define WODEN_WSDL10_ATTR_CODE "code"
#define WODEN_WSDL10_ATTR_SUBCODES "subcodes"
#define WODEN_WSDL10_ATTR_MEP "mep"
#define WODEN_WSDL10_ATTR_ACTION "soapAction"
#define WODEN_WSDL10_ATTR_LOCATION "location"
#define WODEN_WSDL10_ATTR_MUSTUNDERSTAND "mustUnderstand"

/* Prefixes */
#define WODEN_WSDL10_PFX_WSOAP "wsoap"
#define WODEN_WSDL10_PFX_WHTTP "whttp"   /* TODO move to HTTPConstants? */

/* Qualified element names.*/

#define WODEN_WSDL10_Q_ELEM_SOAP_MODULE "module|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ELEM_SOAP_HEADER "header|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

/* Qualified attribute names.*/

#define WODEN_WSDL10_Q_ATTR_SOAP_VERSION "version|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ATTR_SOAP_PROTOCOL "protocol|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ATTR_SOAP_MEPDEFAULT "mepDefault|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ATTR_SOAP_CODE "ATTR_CODE|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ATTR_SOAP_SUBCODES "subcodes|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ATTR_SOAP_MEP "mep|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"

#define WODEN_WSDL10_Q_ATTR_SOAP_ACTION "action|http://schemas.xmlsoap.org/wsdl/soap|wsoap/"



/*********************************** Constants*********************************/   
   
   
#ifdef __cplusplus
}
#endif

#endif /* WODEN_WSDL10_SOAP_CONSTANTS_H */


