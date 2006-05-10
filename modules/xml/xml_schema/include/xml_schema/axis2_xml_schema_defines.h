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
        AXIS2_XML_SCHEMA_ALL,
        AXIS2_XML_SCHEMA_ANNOTATED,
        AXIS2_XML_SCHEMA_ANNOTATION,
        AXIS2_XML_SCHEMA_OBJ,
        AXIS2_XML_SCHEMA_PARTICLE,
        AXIS2_XML_SCHEMA_ENUM,
        AXIS2_XML_SCHEMA_USE,
        AXIS2_XML_SCHEMA_ANY,
        AXIS2_XML_SCHEMA_CONTENT_PROCESSING,
        AXIS2_XML_SCHEMA_APP_INFO,
        AXIS2_XML_SCHEMA_GROUP,
        AXIS2_XML_SCHEMA_GROUP_BASE,
        AXIS2_XML_SCHEMA_GROUP_REF
    } axis2_xml_schema_types_t;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_XML_SCHEMA_DEFINES_H */
