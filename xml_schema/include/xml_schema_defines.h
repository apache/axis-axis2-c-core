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

#ifndef XML_SCHEMA_DEFINES_H
#define XML_SCHEMA_DEFINES_H

/**
 * @file xml_schema_defines.h
 * @brief Axis2c specific global declarations
 */

#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup xml_schema_defines Xml Schema/C project
  * @ingroup xml_schema_defines
  * @{
  */

/** 
    * \brief Axis2 types
    *
    * All Xml Schema/C types
    */
    typedef enum xml_schema_types
    {
        XML_SCHEMA = 0,
        XML_SCHEMA_ENUM,
        XML_SCHEMA_USE,
        XML_SCHEMA_CONTENT_PROCESSING,
        XML_SCHEMA_FORM,
        XML_SCHEMA_CONTENT_TYPE,
        XML_SCHEMA_DERIVATION_METHOD,
        XML_SCHEMA_SEVERITY_TYPE,
        XML_SCHEMA_TOKENIZED_TYPE,
        XML_SCHEMA_OBJ, 
        XML_SCHEMA_ANNOTATION,
        XML_SCHEMA_APP_INFO,
        XML_SCHEMA_DOCUMENTATION,
        XML_SCHEMA_ANNOTATED,
        XML_SCHEMA_FACET,
        XML_SCHEMA_ENUMARATION_FACET,
        XML_SCHEMA_FRACTION_DIGITS_FACET,
        XML_SCHEMA_LENGTH_FACET,
        XML_SCHEMA_MAX_EXCLUSIVE_FACET,
        XML_SCHEMA_MAX_INCLUSIVE_FACET,
        XML_SCHEMA_MAX_LENGTH_FACET,
        XML_SCHEMA_MIN_EXCLUSIVE_FACET,
        XML_SCHEMA_MIN_INCLUSIVE_FACET,
        XML_SCHEMA_MIN_LENGTH_FACET,
        XML_SCHEMA_NUMERAIC_FACET,
        XML_SCHEMA_PATTERN_FACET,
        XML_SCHEMA_TATAL_DIGITS_FACET,
        XML_SCHEMA_WHITE_SPACE_FACET,
        XML_SCHEMA_PARTICLE,
        XML_SCHEMA_ANY,
        XML_SCHEMA_GROUP_BASE,
        XML_SCHEMA_CHOICE,
        XML_SCHEMA_ALL,
        XML_SCHEMA_SEQUENCE,
        XML_SCHEMA_GROUP_REF,
        XML_SCHEMA_ELEMENT,
        XML_SCHEMA_XPATH,
        XML_SCHEMA_GROUP,
        XML_SCHEMA_ANY_ATTRIBUTE,
        XML_SCHEMA_TYPE,
        XML_SCHEMA_SIMPLE_CONTENT_EXTENSION,
        XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION,
        XML_SCHEMA_COMPLEX_CONTENT_EXTENSION,
        XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION,
        XML_SCHEMA_SIMPLE_CONTENT,
        XML_SCHEMA_COMPLEX_CONTENT,
        XML_SCHEMA_SIMPLE_TYPE,
        XML_SCHEMA_COMPLEX_TYPE,
        XML_SCHEMA_SIMPLE_TYPE_CONTENT,
        XML_SCHEMA_SIMPLE_TYPE_LIST,
        XML_SCHEMA_SIMPLE_TYPE_RESTRICTION,
        XML_SCHEMA_SIMPLE_TYPE_UNION,
        XML_SCHEMA_EXTERNAL,
        XML_SCHEMA_IMPORT,
        XML_SCHEMA_INCLUDE,
        XML_SCHEMA_REDEFINE,
        XML_SCHEMA_IDENTITY_CONSTRAINT,
        XML_SCHEMA_UNIQUE,
        XML_SCHEMA_KEY,
        XML_SCHEMA_KEYREF,
        XML_SCHEMA_ATTRIBUTE,
        XML_SCHEMA_ATTRIBUTE_GROUP,
        XML_SCHEMA_ATTRIBUTE_GROUP_REF
    } xml_schema_types_t;
    

#define XML_SCHEMA_CONST_ALL "all"

#define XML_SCHEMA_CONST_NONE "none"

#define XML_SCHEMA_CONST_EXTENSION "extension"

#define XML_SCHEMA_CONST_LIST "list"

#define XML_SCHEMA_CONST_RESTRICTION "restricton"

#define XML_SCHEMA_CONST_SUBSTITUTION "substitution"

#define XML_SCHEMA_CONST_UNION "union"

#define XML_SCHEMA_CONST_STRING "string"

#define XML_SCHEMA_CONST_UNION "union"

#define XML_SCHEMA_CONST_EMPTY "empty"

#define XML_SCHEMA_CONST_ELEMENT_ONLY "elementOnly"

#define XML_SCHEMA_CONST_MIXED "mixed"

#define XML_SCHEMA_CONST_TEXT_ONLY "textOnly"

#define XML_SCHEMA_CONST_LAX "lax"

#define XML_SCHEMA_CONST_SKIP "skip"

#define XML_SCHEMA_CONST_STRICT "strict"

#define XML_SCHEMA_CONST_OPTIONAL "optional"

#define XML_SCHEMA_CONST_PROHIBITED "prohibited"

#define XML_SCHEMA_CONST_REQUIRED "required"

#define XML_SCHEMA_CONST_ERROR "error"

#define XML_SCHEMA_CONST_WARNING "warning"

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* XML_SCHEMA_DEFINES_H */
