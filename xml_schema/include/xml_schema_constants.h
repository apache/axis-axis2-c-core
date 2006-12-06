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
 
#ifndef XML_SCHEMA_CONSTANTS_H
#define XML_SCHEMA_CONSTANTS_H

/**
 * @file axis2_schema_constants.h
 * @brief Axis2 Xml Schema Constants
 */
 
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <xml_schema_defines.h>

#ifdef __cplusplus
extern "C" 
{
#endif
    
/** @defgroup xml_schema_constants 
  * @ingroup xml_schema
  * @{
  */
/*********************************** Constansts********************************/
#define XML_SCHEMANS_URI "http://www.w3.org/XML/1998/namespace"

#define XML_SCHEMANS_PREFIX "xml"

/*
 * Schema Namespaces
 */
#define AXIS2_URI_2001_SCHEMA_XSD "http://www.w3.org/2001/XMLSchema"
#define AXIS2_URI_2001_SCHEMA_XSI "http://www.w3.org/2001/XMLSchema-instance"

/* Define qnames for the all of the XSD and SOAP-ENC encodings */
#define XML_SCHEMA_XSD_STRING     "string"
#define XML_SCHEMA_XSD_BOOLEAN    "boolean"
#define XML_SCHEMA_XSD_DOUBLE     "double"
#define XML_SCHEMA_XSD_FLOAT      "float"
#define XML_SCHEMA_XSD_INT        "int"
#define XML_SCHEMA_XSD_INTEGER    "integer"
#define XML_SCHEMA_XSD_LONG       "long"
#define XML_SCHEMA_XSD_SHORT      "short"
#define XML_SCHEMA_XSD_BYTE       "byte"
#define XML_SCHEMA_XSD_DECIMAL    "decimal"
#define XML_SCHEMA_XSD_BASE64     "base64Binary"
#define XML_SCHEMA_XSD_HEXBIN     "hexBinary"
#define XML_SCHEMA_XSD_ANYSIMPLETYPE "anySimpleType"
#define XML_SCHEMA_XSD_ANYTYPE    "anyType"
#define XML_SCHEMA_XSD_ANY        "any"
#define XML_SCHEMA_XSD_QNAME      "QName"
#define XML_SCHEMA_XSD_DATETIME   "dateTime"
#define XML_SCHEMA_XSD_DATE       "date"
#define XML_SCHEMA_XSD_TIME       "time"

#define XML_SCHEMA_XSD_NORMALIZEDSTRING "normalizedString"
#define XML_SCHEMA_XSD_TOKEN          "token"

#define XML_SCHEMA_XSD_UNSIGNEDLONG       "unsignedLong"
#define XML_SCHEMA_XSD_UNSIGNEDINT        "unsignedInt"
#define XML_SCHEMA_XSD_UNSIGNEDSHORT      "unsignedShort"
#define XML_SCHEMA_XSD_UNSIGNEDBYTE       "unsignedByte"
#define XML_SCHEMA_XSD_POSITIVEINTEGER    "positiveInteger"
#define XML_SCHEMA_XSD_NEGATIVEINTEGER    "negativeInteger"
#define XML_SCHEMA_XSD_NONNEGATIVEINTEGER "nonNegativeInteger"
#define XML_SCHEMA_XSD_NONPOSITIVEINTEGER "nonPositiveInteger"

#define XML_SCHEMA_XSD_YEARMONTH          "gYearMonth"
#define XML_SCHEMA_XSD_MONTHDAY           "gMonthDay"
#define XML_SCHEMA_XSD_YEAR               "gYear"
#define XML_SCHEMA_XSD_MONTH              "gMonth"
#define XML_SCHEMA_XSD_DAY                "gDay"
#define XML_SCHEMA_XSD_DURATION           "duration"

#define XML_SCHEMA_XSD_NAME               "Name"
#define XML_SCHEMA_XSD_NCNAME             "NCName"
#define XML_SCHEMA_XSD_NMTOKEN            "NMTOKEN"
#define XML_SCHEMA_XSD_NMTOKENS           "NMTOKENS"
#define XML_SCHEMA_XSD_NOTATION           "NOTATION"
#define XML_SCHEMA_XSD_ENTITY             "ENTITY"
#define XML_SCHEMA_XSD_ENTITIES           "ENTITIES"
#define XML_SCHEMA_XSD_IDREF              "IDREF"
#define XML_SCHEMA_XSD_IDREFS             "IDREFS"
#define XML_SCHEMA_XSD_ANYURI             "anyURI"
#define XML_SCHEMA_XSD_LANGUAGE           "language"
#define XML_SCHEMA_XSD_ID                 "ID"
#define XML_SCHEMA_XSD_SCHEMA             "schema"


typedef axis2_hash_t* (AXIS2_CALL *XML_SCHEMA_SUPER_OBJS_FN)(void *obj,
                                    const axis2_env_t *env);
                                    
typedef xml_schema_types_t (AXIS2_CALL *XML_SCHEMA_GET_TYPE_FN)(void *obj, const axis2_env_t *env);

typedef int (AXIS2_CALL *XML_SCHEMA_FREE_FN)(void *obj, const axis2_env_t *env);                                   

#ifdef __cplusplus
}
#endif

#endif /* XML_SCHEMA_CONSTANTS_H */
