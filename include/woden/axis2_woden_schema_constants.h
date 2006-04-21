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
 
#ifndef AXIS2_WODEN_SCHEMA_CONSTANTS_H
#define AXIS2_WODEN_SCHEMA_CONSTANTS_H

/**
 * @file axis2_schema_constants.h
 * @brief Axis2 Xml Schema Constants
 *          Constants for XML Schema elements, attributes and URIs.
 */
 
#include <axis2.h>

#ifdef __cplusplus
extern "C" 
{
#endif
    
/** @defgroup axis2_woden_schema_constants 
  * @ingroup axis2_woden_schema
  * @{
  */
/*********************************** Constansts********************************/


/* Schema attribute names */
#define AXIS2_ATTR_ID "id"
#define AXIS2_ATTR_SCHEMA_LOCATION "schemaLocation"

/* Schema element names */
#define AXIS2_ELEM_SCHEMA "schema"
#define AXIS2_ELEM_SCHEMA_IMPORT "import"
#define AXIS2_ELEM_SCHEMA_INCLUDE "include"
#define AXIS2_ELEM_SCHEMA_REDEFINE "redefine"

/* Schema uri */
#define AXIS2_NS_URI_XSD_1999 "http://www.w3.org/1999/XMLSchema"
#define AXIS2_NS_URI_XSD_2000 "http://www.w3.org/2000/10/XMLSchema"
#define AXIS2_NS_URI_XSD_2001 "http://www.w3.org/2001/XMLSchema"

/* <xs:schema> qnames */
#define AXIS2_Q_ELEM_XSD_1999 "schema|http://www.w3.org/1999/XMLSchema"
#define AXIS2_Q_ELEM_XSD_2000 "schema|http://www.w3.org/2000/10/XMLSchema"
#define AXIS2_Q_ELEM_XSD_2001 "schema|http://www.w3.org/2001/XMLSchema"

/* <xs:import> qnames */
#define AXIS2_Q_ELEM_IMPORT_XSD_1999 "import|http://www.w3.org/1999/XMLSchema"
#define AXIS2_Q_ELEM_IMPORT_XSD_2000 "import|http://www.w3.org/2000/10/XMLSchema"
#define AXIS2_Q_ELEM_IMPORT_XSD_2001 "import|http://www.w3.org/2001/XMLSchema"

/* TODO remove <include> if not used in Woden */
/* <xs:include> qnames */
#define AXIS2_Q_ELEM_INCLUDE_XSD_1999 "include|http://www.w3.org/1999/XMLSchema"
#define AXIS2_Q_ELEM_INCLUDE_XSD_2000 "include|http://www.w3.org/2000/10/XMLSchema"
#define AXIS2_Q_ELEM_INCLUDE_XSD_2001 "include|http://www.w3.org/2001/XMLSchema"

/* TODO remove <redefine> if not used in Woden */
/* <xs:redefine> qnames */
#define AXIS2_Q_ELEM_REDEFINE_XSD_1999 "redefine|http://www.w3.org/1999/XMLSchema"
#define AXIS2_Q_ELEM_REDEFINE_XSD_2000 "redefine|http://www.w3.org/2000/10/XMLSchema"
#define AXIS2_Q_ELEM_REDEFINE_XSD_2001 "redefine|http://www.w3.org/2001/XMLSchema"


/*********************************** Constants*********************************/	
	
	
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_WODEN_SCHEMA_CONSTANTS_H */


