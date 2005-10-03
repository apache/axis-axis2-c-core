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

#include <axis2_node.h>

typedef struct axis2_om_doctype_t
{
	char *value;
}axis2_om_doctype_t;

/**
 * Create a doctype struct and stores in in a node struct and returns a pointer
 * to the axis2_node_t struct
 * the data_element field of node struct points to the acctual axis2_doctype_t struct
 * The element type of axis2_node_t struct will be of type AXIS2_OM_DOCTYPE
 * @return pointer to a axis2_node_t struct containing the doctype struct
 */

axis2_node_t *axis2_om_doctype_create(axis2_node_t *parent,const char *value);

/**
 *	create an axis2_om_doctype_t struct with parent 
 *  value field will be set to null
 *  @return poniter to a axis2_node_t 
 */


axis2_node_t *axis2_om_doctype_create_empty_doctype(axis2_node_t *parent);

/**
 *	free the axis2_om_doctype_t struct
 *	@param om_doc pointer to axis2_om_doctype_t struct
 */
void axis2_om_doctype_free(axis2_om_doctype_t *om_doc);

/**
 *	accessor function to get value of doctype
 *	@param om_doc pointer to axis2_om_doctype_t struct
 */
char *om_doctype_get_value(axis2_node_t *doctype_node);

/**
 *	mutator function to get value of doctype
 *	@param om_doc pointer to axis2_om_doctype_t struct
 *  @param value value to be set in om_doctype
 */
void om_doctype_set_value(axis2_node_t *doctype_node,const char *value);

#endif				//  AXIS2_OM_DOCTYPE_H
