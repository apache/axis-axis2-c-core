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

#ifndef AXIS2_OM_DOCTYPE_H
#define AXIS2_OM_DOCTYPE_H

/**
 *@file axis2_om_doctype.h	
 *@brief defines struct representing xml DTD and its manipulation functions
 */

#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup axis2_om_text OM Text
 * @ingroup axis2_om 
 * @{
 */

/** @cond */
struct axis2_om_doctype;
struct axis2_om_doctype_ops;	
/** @endcond */
    
/** @struct axis2_om_doctype_ops
    @brief OM doctype operations struct

    Encapsulator struct for operations of axis2_om_doctype_t
*/
typedef struct axis2_om_doctype_ops
{
	/**
	 *	free the axis2_om_doctype_t struct
	 * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
	 * @param om_doc pointer to axis2_om_doctype_t struct
	 * @returns status code
	 */
	axis2_status_t (*axis2_om_doctype_ops_free)(axis2_environment_t *environment, struct axis2_om_doctype *om_doctype);
} axis2_om_doctype_ops_t;

/** \struct axis2_om_doctype
    \brief OM doctype struct

    Handles the XML document type in OM
*/
typedef struct axis2_om_doctype
{
	axis2_om_doctype_ops_t* ops;
	char *value;
}axis2_om_doctype_t;

/**
 * Create a doctype struct and stores in in a node struct and returns a pointer
 * to the axis2_axis2_om_doctype_t struct
 * the data_element field of node struct points to the acctual axis2_doctype_t struct
 * The element type of axis2_om_node_t struct will be of type AXIS2_OM_DOCTYPE
 * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
 * @param node This is an outparameter
 * @return pointer to a axis2_om_node_t struct containing the doctype struct
 */

axis2_om_doctype_t *axis2_om_doctype_create(axis2_environment_t *environment, axis2_om_node_t *parent, const axis2_char_t *value, axis2_om_node_t **node);

#define axis2_om_doctype_free(environment, doctype) ((doctype)->ops->axis2_om_doctype_ops_free(environment, doctype))

/** @} */
#ifdef __cplusplus
}
#endif

#endif				/* AXIS2_OM_DOCTYPE_H */
