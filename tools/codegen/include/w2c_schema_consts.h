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
  
#ifndef W2C_SCHEMA_CONSTS_H
#define W2C_SCHEMA_CONSTS_H

/**
 * @file w2c_schema_consts.h
 * @brief command line option constants
 */

#include <stdio.h>
#include <axis2_utils.h>
#include <xml_schema_constants.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_schema_consts keep command line optons consts 
 * @ingroup w2c_wsdl_utils
 * @{
 */    

/** schema constants */


#define W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD   AXIS2_URI_2001_SCHEMA_XSD
    
#define W2C_SCHEMA_XSD_STRING               "string" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_BOOLEAN              "boolean" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_DOUBLE               "double" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_FLOAT                "float" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_INT                  "int" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_INTEGER              "integer" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_LONG                 "long" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_SHORT                "short" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_BYTE                 "byte" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_DECIMAL              "decimal" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_BASE64               "base64Binary" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_HEXBIN               "hexBinary" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ANYSIMPLETYPE        "anySimpleType" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ANYTYPE              "anyType" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ANY                  "any" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_QNAME                "QName" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_DATETIME             "dateTime" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_DATE                 "date" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_TIME                 "time" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_UNSIGNEDLONG         "unsignedLong" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_UNSIGNEDINT          "unsignedInt" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_UNSIGNEDSHORT        "unsignedShort" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_UNSIGNEDBYTE         "unsignedByte" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_POSITIVEINTEGER      "positiveInteger" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NEGATIVEINTEGER      "negativeInteger" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NONNEGATIVEINTEGER   "nonNegativeInteger" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NONPOSITIVEINTEGER   "nonPositiveInteger" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_YEARMONTH            "gYearMonth" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_MONTHDAY             "gMonthDay" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_YEAR                 "gYear" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_MONTH                "gMonth" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_DAY                  "gDay" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_DURATION             "duration" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NAME                 "Name" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NCNAME               "NCName" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NMTOKEN              "NMTOKEN" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NMTOKENS             "NMTOKENS" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NOTATION             "NOTATION" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ENTITY               "ENTITY" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ENTITIES             "ENTITIES" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_IDREF                "IDREF" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_IDREFS               "IDREFS" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ANYURI               "anyURI" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_LANGUAGE             "language" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_ID                   "ID" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_SCHEMA               "schema" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_NORMALIZEDSTRING     "normalizedString" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD
#define W2C_SCHEMA_XSD_TOKEN                "token" "|" W2C_SCHEMA_URI_DEFAULT_SCHEMA_XSD

/*it patterns for the types*/

#define W2C_SCHEMA_CONSTS_ATTRIBUTE_TYPE           0x01
/*#define W2C_SCHEMA_CONSTS_ANY                    0x02*/
#define W2C_SCHEMA_CONSTS_ELEMENT_TYPE             0x02
/*#define W2C_SCHEMA_CONSTS_ANY_ATTRIBUTE_TYPE     0x08*/
#define W2C_SCHEMA_CONSTS_ARRAY_TYPE               0x04
#define W2C_SCHEMA_CONSTS_ANY_TYPE                 0x08
#define W2C_SCHEMA_CONSTS_BINARY_TYPE              0x10


#define W2C_SCHEMA_COMPILER_CONSTS_EXTRA_ATTRIBUTE_FIELD_NAME "extraAttributes"
#define W2C_SCHEMA_CONSTS_ANY_ELEMENT_FIELD_NAME "extraElement"
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_SCHEMA_CONSTS_H */
