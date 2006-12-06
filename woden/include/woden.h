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

#ifndef WODEN_H
#define WODEN_H

/**
 * @file woden.h
 * @brief Axis2c specific global declarations
 */

#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup woden Woden/C project
  * @ingroup woden
  * @{
  */
/** @defgroup axis2 Woden/C project
  *  @{
 */

/** 
    * \brief Axis2 types
    *
    * All Woden/C types
    */
    typedef enum woden_obj_types
    {
        WODEN_SCHEMA = 0,
        WODEN_IMPORTED_SCHEMA,
        WODEN_INLINED_SCHEMA,
        WODEN_XML_ATTR,
        WODEN_QNAME_ATTR,
        WODEN_URI_ATTR,
        WODEN_QNAME_LIST_ATTR,
        WODEN_STRING_ATTR,
        WODEN_QNAME_LIST_OR_TOKEN_ANY_ATTR,
        WODEN_QNAME_OR_TOKEN_ANY_ATTR,
        WODEN_BOOL_ATTR,
        WODEN_WSDL_ELEMENT,
        WODEN_ATTR_EXTENSIBLE,
        WODEN_ELEMENT_EXTENSIBLE,
        WODEN_FEATURE,
        WODEN_PROPERTY,
        WODEN_DESC,
        WODEN_TYPES,
        WODEN_DOCUMENTATION,
        WODEN_TYPE_DEF,
        WODEN_INTERFACE,
        WODEN_INTERFACE_FAULT,
        WODEN_INTERFACE_FAULT_REF,
        WODEN_INTERFACE_MSG_REF,
        WODEN_INTERFACE_OP,
        WODEN_BINDING,
        WODEN_BINDING_FAULT,
        WODEN_BINDING_FAULT_REF,
        WODEN_BINDING_MSG_REF,
        WODEN_BINDING_OP,
        WODEN_ENDPOINT,
        WODEN_SVC,
        WODEN_ELEMENT_DECL,
        WODEN_SOAP_BINDING_EXTS,
        WODEN_SOAP_BINDING_FAULT_EXTS,
        WODEN_SOAP_BINDING_FAULT_REF_EXTS,
        WODEN_SOAP_BINDING_MSG_REF_EXTS,
        WODEN_SOAP_BINDING_OP_EXTS,
        WODEN_IMPORT,
        WODEN_INCLUDE,
        AXIS2_QNAME,
        AXIS2_QNAME_LIST,
        WODEN_SOAP_HEADER_BLOCK,
        WODEN_SOAP_MODULE,
        WODEN_SOAP_MODULE_DESERIALIZER,
        WODEN_SOAP_HEADER_BLOCK_DESERIALIZER,
        /* Wsdl 10 */
        WODEN_WSDL10_ELEMENT,
        WODEN_WSDL10_ATTR_EXTENSIBLE,
        WODEN_WSDL10_ELEMENT_EXTENSIBLE,
        WODEN_WSDL10_FEATURE,
        WODEN_WSDL10_PROPERTY,
        WODEN_WSDL10_DESC,
        WODEN_WSDL10_TYPES,
        WODEN_WSDL10_DOCUMENTATION,
        WODEN_WSDL10_TYPE_DEF,
        WODEN_WSDL10_INTERFACE,
        WODEN_WSDL10_INTERFACE_FAULT,
        WODEN_WSDL10_INTERFACE_FAULT_REF,
        WODEN_WSDL10_INTERFACE_MSG_REF,
        WODEN_WSDL10_MSG_REF,
        WODEN_WSDL10_PART,
        WODEN_WSDL10_INTERFACE_OP,
        WODEN_WSDL10_BINDING,
        WODEN_WSDL10_BINDING_FAULT,
        WODEN_WSDL10_BINDING_FAULT_REF,
        WODEN_WSDL10_BINDING_MSG_REF,
        WODEN_WSDL10_BINDING_OP,
        WODEN_WSDL10_ENDPOINT,
        WODEN_WSDL10_SVC,
        WODEN_WSDL10_ELEMENT_DECL,
        WODEN_WSDL10_IMPORT,
        WODEN_WSDL10_INCLUDE,
        WODEN_WSDL10_SOAP_BINDING_EXTS,
        WODEN_WSDL10_SOAP_BINDING_FAULT_EXTS,
        WODEN_WSDL10_SOAP_BINDING_FAULT_REF_EXTS,
        WODEN_WSDL10_SOAP_BINDING_MSG_REF_EXTS,
        WODEN_WSDL10_SOAP_BINDING_OP_EXTS,
        WODEN_WSDL10_SOAP_ADDRESS_EXTS,
        WODEN_WSDL10_SOAP_HEADER_BLOCK,
        WODEN_WSDL10_SOAP_MODULE,
        WODEN_WSDL10_SOAP_MODULE_DESERIALIZER,
        WODEN_WSDL10_SOAP_HEADER_BLOCK_DESERIALIZER
    } woden_obj_types_t;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* WODEN_H */
