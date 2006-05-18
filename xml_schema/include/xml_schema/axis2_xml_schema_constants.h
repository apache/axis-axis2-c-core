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
 
#ifndef AXIS2_XML_SCHEMA_CONSTANTS_H
#define AXIS2_XML_SCHEMA_CONSTANTS_H

/**
 * @file axis2_schema_constants.h
 * @brief Axis2 Xml Schema Constants
 */
 
#include <axis2_const.h>

#ifdef __cplusplus
extern "C" 
{
#endif
    
/** @defgroup axis2_xml_schema_constants 
  * @ingroup axis2_xml_schema
  * @{
  */
/*********************************** Constansts********************************/
#define AXIS2_XMLNS_URI "http://www.w3.org/XML/1998/namespace"

#define AXIS2_XMLNS_PREFIX "xml"

/*
 * Schema Namespaces
 */
#define AXIS2_URI_2001_SCHEMA_XSD "http://www.w3.org/2001/XMLSchema"
#define AXIS2_URI_2001_SCHEMA_XSI "http://www.w3.org/2001/XMLSchema-instance"

/* Define qnames for the all of the XSD and SOAP-ENC encodings */
#define XSD_STRING "URI_2001_SCHEMA_XSD|string"
#define XSD_BOOLEAN "URI_2001_SCHEMA_XSD|boolean"
#define XSD_DOUBLE "URI_2001_SCHEMA_XSD|double"
#define XSD_FLOAT "URI_2001_SCHEMA_XSD|float"
#define XSD_INT "URI_2001_SCHEMA_XSD|int"
#define XSD_INTEGER "URI_2001_SCHEMA_XSD|integer"
#define XSD_LONG "URI_2001_SCHEMA_XSD|long"
#define XSD_SHORT "URI_2001_SCHEMA_XSD|short"
#define XSD_BYTE "URI_2001_SCHEMA_XSD|byte"
#define XSD_DECIMAL "URI_2001_SCHEMA_XSD|decimal"
#define XSD_BASE64 "URI_2001_SCHEMA_XSD|base64Binary"
#define XSD_HEXBIN "URI_2001_SCHEMA_XSD|hexBinary"
#define XSD_ANYSIMPLETYPE "URI_2001_SCHEMA_XSD|anySimpleType"
#define XSD_ANYTYPE "URI_2001_SCHEMA_XSD|anyType"
#define XSD_ANY "URI_2001_SCHEMA_XSD|any"
#define XSD_QNAME "URI_2001_SCHEMA_XSD|QName"
#define XSD_DATETIME "URI_2001_SCHEMA_XSD|dateTime"
#define XSD_DATE "URI_2001_SCHEMA_XSD|date"
#define XSD_TIME "URI_2001_SCHEMA_XSD|time"

#define XSD_NORMALIZEDSTRING "URI_2001_SCHEMA_XSD|normalizedString"
#define XSD_TOKEN "URI_2001_SCHEMA_XSD|token"

#define XSD_UNSIGNEDLONG "URI_2001_SCHEMA_XSD|unsignedLong"
#define XSD_UNSIGNEDINT "URI_2001_SCHEMA_XSD|unsignedInt"
#define XSD_UNSIGNEDSHORT "URI_2001_SCHEMA_XSD|unsignedShort"
#define XSD_UNSIGNEDBYTE "URI_2001_SCHEMA_XSD|unsignedByte"
#define XSD_POSITIVEINTEGER "URI_2001_SCHEMA_XSD|positiveInteger"
#define XSD_NEGATIVEINTEGER "URI_2001_SCHEMA_XSD|negativeInteger"
#define XSD_NONNEGATIVEINTEGER "URI_2001_SCHEMA_XSD|nonNegativeInteger"
#define XSD_NONPOSITIVEINTEGER "URI_2001_SCHEMA_XSD|nonPositiveInteger"

#define XSD_YEARMONTH "URI_2001_SCHEMA_XSD|gYearMonth"
#define XSD_MONTHDAY "URI_2001_SCHEMA_XSD|gMonthDay"
#define XSD_YEAR "URI_2001_SCHEMA_XSD|gYear"
#define XSD_MONTH "URI_2001_SCHEMA_XSD|gMonth"
#define XSD_DAY "URI_2001_SCHEMA_XSD|gDay"
#define XSD_DURATION "URI_2001_SCHEMA_XSD|duration"

#define XSD_NAME "URI_2001_SCHEMA_XSD|Name"
#define XSD_NCNAME "URI_2001_SCHEMA_XSD|NCName"
#define XSD_NMTOKEN "URI_2001_SCHEMA_XSD|NMTOKEN"
#define XSD_NMTOKENS "URI_2001_SCHEMA_XSD|NMTOKENS"
#define XSD_NOTATION "URI_2001_SCHEMA_XSD|NOTATION"
#define XSD_ENTITY "URI_2001_SCHEMA_XSD|ENTITY"
#define XSD_ENTITIES "URI_2001_SCHEMA_XSD|ENTITIES"
#define XSD_IDREF "URI_2001_SCHEMA_XSD|IDREF"
#define XSD_IDREFS "URI_2001_SCHEMA_XSD|IDREFS"
#define XSD_ANYURI "URI_2001_SCHEMA_XSD|anyURI"
#define XSD_LANGUAGE "URI_2001_SCHEMA_XSD|language"
#define XSD_ID "URI_2001_SCHEMA_XSD|ID"
#define XSD_SCHEMA "URI_2001_SCHEMA_XSD|schema"

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_XML_SCHEMA_CONSTANTS_H */
