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

#ifndef AXIS2_WODEN_H
#define AXIS2_WODEN_H

/**
 * @file axis2_woden.h
 * @brief Axis2c specific global declarations
 */

#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup axis2_woden Woden/C project
  * @ingroup axis2_woden
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
    typedef enum axis2_woden_obj_types
    {
        AXIS2_WODEN_SCHEMA = 0,
        AXIS2_WODEN_IMPORTED_SCHEMA,
        AXIS2_WODEN_INLINED_SCHEMA,
        AXIS2_WODEN_XML_ATTR,
        AXIS2_WODEN_QNAME_ATTR,
        AXIS2_WODEN_URI_ATTR,
        AXIS2_WODEN_QNAME_LIST_ATTR,
        AXIS2_WODEN_STRING_ATTR,
        AXIS2_WODEN_QNAME_LIST_OR_TOKEN_ANY_ATTR,
        AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR,
        AXIS2_WODEN_BOOL_ATTR,
        AXIS2_WODEN_WSDL_ELEMENT,
        AXIS2_WODEN_ATTR_EXTENSIBLE,
        AXIS2_WODEN_ELEMENT_EXTENSIBLE,
        AXIS2_WODEN_FEATURE,
        AXIS2_WODEN_PROPERTY,
        AXIS2_WODEN_TYPES,
        AXIS2_WODEN_DOCUMENTATION,
        AXIS2_WODEN_TYPE_DEF,
        AXIS2_WODEN_INTERFACE,
        AXIS2_WODEN_INTERFACE_FAULT,
        AXIS2_WODEN_INTERFACE_FAULT_REF,
        AXIS2_WODEN_INTERFACE_MSG_REF,
        AXIS2_WODEN_INTERFACE_OP,
        AXIS2_WODEN_BINDING,
        AXIS2_WODEN_BINDING_FAULT,
        AXIS2_WODEN_BINDING_FAULT_REF,
        AXIS2_WODEN_BINDING_MSG_REF,
        AXIS2_WODEN_BINDING_OP,
        AXIS2_WODEN_ENDPOINT,
        AXIS2_WODEN_SVC,
        AXIS2_WODEN_ELEMENT_DECL,
        AXIS2_WODEN_SOAP_BINDING_EXTS,
        AXIS2_WODEN_SOAP_BINDING_FAULT_EXTS,
        AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS,
        AXIS2_WODEN_SOAP_BINDING_MSG_REF_EXTS,
        AXIS2_WODEN_SOAP_BINDING_OP_EXTS,
        AXIS2_WODEN_IMPORT,
        AXIS2_WODEN_INCLUDE,
        AXIS2_QNAME,
        AXIS2_QNAME_LIST
    } axis2_woden_obj_types_t;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_WODEN_H */
