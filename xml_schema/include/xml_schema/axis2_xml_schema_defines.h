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

#ifndef AXIS2_XML_SCHEMA_DEFINES_H
#define AXIS2_XML_SCHEMA_DEFINES_H

/**
 * @file axis2_xml_schema_defines.h
 * @brief Axis2c specific global declarations
 */

#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup axis2_xml_schema_defines Xml Schema/C project
  * @ingroup axis2_xml_schema_defines
  * @{
  */
/** @defgroup axis2 Xml Schema/C project
  *  @{
 */

/** 
    * \brief Axis2 types
    *
    * All Xml Schema/C types
    */
    typedef enum axis2_xml_schema_types
    {
        AXIS2_XML_SCHEMA = 0,
        AXIS2_XML_SCHEMA_ENUM,
        AXIS2_XML_SCHEMA_USE,
        AXIS2_XML_SCHEMA_CONTENT_PROCESSING,
        AXIS2_XML_SCHEMA_FORM,
        AXIS2_XML_SCHEMA_CONTENT_TYPE,
        AXIS2_XML_SCHEMA_DERIVATION_METHOD,
        AXIS2_XML_SEVERITY_TYPE,
        AXIS2_XML_TOKENIZED_TYPE,
        AXIS2_XML_SCHEMA_OBJ, 
        AXIS2_XML_SCHEMA_ANNOTATION,
        AXIS2_XML_SCHEMA_APP_INFO,
        AXIS2_XML_SCHEMA_DOCUMENTATION,
        AXIS2_XML_SCHEMA_ANNOTATED,
        AXIS2_XML_SCHEMA_FACET,
        AXIS2_XML_SCHEMA_ENUMARATION_FACET,
        AXIS2_XML_SCHEMA_FRACTION_DIGITS_FACET,
        AXIS2_XML_SCHEMA_LENGTH_FACET,
        AXIS2_XML_SCHEMA_MAX_EXCLUSIVE_FACET,
        AXIS2_XML_SCHEMA_MAX_INCLUSIVE_FACET,
        AXIS2_XML_SCHEMA_MAX_LENGTH_FACET,
        AXIS2_XML_SCHEMA_MIN_EXCLUSIVE_FACET,
        AXIS2_XML_SCHEMA_MIN_INCLUSIVE_FACET,
        AXIS2_XML_SCHEMA_MIN_LENGTH_FACET,
        AXIS2_XML_SCHEMA_NUMERAIC_FACET,
        AXIS2_XML_SCHEMA_PATTERN_FACET,
        AXIS2_XML_SCHEMA_TATAL_DIGITS_FACET,
        AXIS2_XML_SCHEMA_WHITE_SPACE_FACET,
        AXIS2_XML_SCHEMA_PARTICLE,
        AXIS2_XML_SCHEMA_ANY,
        AXIS2_XML_SCHEMA_GROUP_BASE,
        AXIS2_XML_SCHEMA_CHOICE,
        AXIS2_XML_SCHEMA_ALL,
        AXIS2_XML_SCHEMA_SEQUENCE,
        AXIS2_XML_SCHEMA_GROUP_REF,
        AXIS2_XML_SCHEMA_ELEMENT,
        AXIS2_XML_SCHEMA_XPATH,
        AXIS2_XML_SCHEMA_GROUP,
        AXIS2_XML_SCHEMA_ANY_ATTRIBUTE,
        AXIS2_XML_SCHEMA_TYPE,
        AXIS2_XML_SCHEMA_SIMPLE_CONTENT_EXTENSION,
        AXIS2_XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION,
        AXIS2_XML_SCHEMA_COMPLEX_CONTENT_EXTENSION,
        AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION,
        AXIS2_XML_SCHEMA_SIMPLE_CONTENT,
        AXIS2_XML_SCHEMA_COMPLEX_CONTENT,
        AXIS2_XML_SCHEMA_SIMPLE_TYPE,
        AXIS2_XML_SCHEMA_COMPLEX_TYPE,
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT,
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_LIST,
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION,
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION,
        AXIS2_XML_SCHEMA_EXTERNAL,
        AXIS2_XML_SCHEMA_IMPORT,
        AXIS2_XML_SCHEMA_INCLUDE,
        AXIS2_XML_SCHEMA_REDEFINE,
        AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT,
        AXIS2_XML_SCHEMA_UNIQUE,
        AXIS2_XML_SCHEMA_KEY,
        AXIS2_XML_SCHEMA_KEYREF,
        AXIS2_XML_SCHEMA_ATTRIBUTE,
        AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP,
        AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF
    } axis2_xml_schema_types_t;
    

#define AXIS2_XML_SCHEMA_CONST_ALL "all"

#define AXIS2_XML_SCHEMA_CONST_NONE "none"

#define AXIS2_XML_SCHEMA_CONST_EXTENSION "extension"

#define AXIS2_XML_SCHEMA_CONST_LIST "list"

#define AXIS2_XML_SCHEMA_CONST_RESTRICTION "restricton"

#define AXIS2_XML_SCHEMA_CONST_SUBSTITUTION "substitution"

#define AXIS2_XML_SCHEMA_CONST_UNION "union"

#define AXIS2_XML_SCHEMA_CONST_STRING "string"

#define AXIS2_XML_SCHEMA_CONST_UNION "union"

#define AXIS2_XML_SCHEMA_CONST_EMPTY "empty"

#define AXIS2_XML_SCHEMA_CONST_ELEMENT_ONLY "elementOnly"

#define AXIS2_XML_SCHEMA_CONST_MIXED "mixed"

#define AXIS2_XML_SCHEMA_CONST_TEXT_ONLY "textOnly"

#define AXIS2_XML_SCHEMA_CONST_LAX "lax"

#define AXIS2_XML_SCHEMA_CONST_SKIP "skip"

#define AXIS2_XML_SCHEMA_CONST_STRICT "strict"

#define AXIS2_XML_SCHEMA_CONST_OPTIONAL "optional"

#define AXIS2_XML_SCHEMA_CONST_PROHIBITED "prohibited"

#define AXIS2_XML_SCHEMA_CONST_REQUIRED "required"

#define AXIS2_XML_SCHEMA_CONST_ERROR "error"

#define AXIS2_XML_SCHEMA_CONST_WARNING "warning"

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_XML_SCHEMA_DEFINES_H */
