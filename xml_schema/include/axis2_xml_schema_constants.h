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
 
#include <axis2_utils.h>

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
#define AXIS2_XML_SCHEMA_XSD_STRING     "string"
#define AXIS2_XML_SCHEMA_XSD_BOOLEAN    "boolean"
#define AXIS2_XML_SCHEMA_XSD_DOUBLE     "double"
#define AXIS2_XML_SCHEMA_XSD_FLOAT      "float"
#define AXIS2_XML_SCHEMA_XSD_INT        "int"
#define AXIS2_XML_SCHEMA_XSD_INTEGER    "integer"
#define AXIS2_XML_SCHEMA_XSD_LONG       "long"
#define AXIS2_XML_SCHEMA_XSD_SHORT      "short"
#define AXIS2_XML_SCHEMA_XSD_BYTE       "byte"
#define AXIS2_XML_SCHEMA_XSD_DECIMAL    "decimal"
#define AXIS2_XML_SCHEMA_XSD_BASE64     "base64Binary"
#define AXIS2_XML_SCHEMA_XSD_HEXBIN     "hexBinary"
#define AXIS2_XML_SCHEMA_XSD_ANYSIMPLETYPE "anySimpleType"
#define AXIS2_XML_SCHEMA_XSD_ANYTYPE    "anyType"
#define AXIS2_XML_SCHEMA_XSD_ANY        "any"
#define AXIS2_XML_SCHEMA_XSD_QNAME      "QName"
#define AXIS2_XML_SCHEMA_XSD_DATETIME   "dateTime"
#define AXIS2_XML_SCHEMA_XSD_DATE       "date"
#define AXIS2_XML_SCHEMA_XSD_TIME       "time"

#define AXIS2_XML_SCHEMA_XSD_NORMALIZEDSTRING "normalizedString"
#define AXIS2_XML_SCHEMA_XSD_TOKEN          "token"

#define AXIS2_XML_SCHEMA_XSD_UNSIGNEDLONG       "unsignedLong"
#define AXIS2_XML_SCHEMA_XSD_UNSIGNEDINT        "unsignedInt"
#define AXIS2_XML_SCHEMA_XSD_UNSIGNEDSHORT      "unsignedShort"
#define AXIS2_XML_SCHEMA_XSD_UNSIGNEDBYTE       "unsignedByte"
#define AXIS2_XML_SCHEMA_XSD_POSITIVEINTEGER    "positiveInteger"
#define AXIS2_XML_SCHEMA_XSD_NEGATIVEINTEGER    "negativeInteger"
#define AXIS2_XML_SCHEMA_XSD_NONNEGATIVEINTEGER "nonNegativeInteger"
#define AXIS2_XML_SCHEMA_XSD_NONPOSITIVEINTEGER "nonPositiveInteger"

#define AXIS2_XML_SCHEMA_XSD_YEARMONTH          "gYearMonth"
#define AXIS2_XML_SCHEMA_XSD_MONTHDAY           "gMonthDay"
#define AXIS2_XML_SCHEMA_XSD_YEAR               "gYear"
#define AXIS2_XML_SCHEMA_XSD_MONTH              "gMonth"
#define AXIS2_XML_SCHEMA_XSD_DAY                "gDay"
#define AXIS2_XML_SCHEMA_XSD_DURATION           "duration"

#define AXIS2_XML_SCHEMA_XSD_NAME               "Name"
#define AXIS2_XML_SCHEMA_XSD_NCNAME             "NCName"
#define AXIS2_XML_SCHEMA_XSD_NMTOKEN            "NMTOKEN"
#define AXIS2_XML_SCHEMA_XSD_NMTOKENS           "NMTOKENS"
#define AXIS2_XML_SCHEMA_XSD_NOTATION           "NOTATION"
#define AXIS2_XML_SCHEMA_XSD_ENTITY             "ENTITY"
#define AXIS2_XML_SCHEMA_XSD_ENTITIES           "ENTITIES"
#define AXIS2_XML_SCHEMA_XSD_IDREF              "IDREF"
#define AXIS2_XML_SCHEMA_XSD_IDREFS             "IDREFS"
#define AXIS2_XML_SCHEMA_XSD_ANYURI             "anyURI"
#define AXIS2_XML_SCHEMA_XSD_LANGUAGE           "language"
#define AXIS2_XML_SCHEMA_XSD_ID                 "ID"
#define AXIS2_XML_SCHEMA_XSD_SCHEMA             "schema"

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_XML_SCHEMA_CONSTANTS_H */
