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

#ifndef AXIS2_NODE_H
#define AXIS2_NODE_H

/**
 * @file axis2_om_node.h
 * @brief defines node data structure, used for constructing the om tree and its 
 *	manipulation functions
 */


#include <axis2.h>
#include <apr.h>
#include <apr_pools.h>
#include <xmlpullparser.h>

typedef struct axis2_stax_om_builder_s axis2_stax_om_builder_t;


typedef enum axis2_om_types_t {
    AXIS2_OM_INVALID = -1,
	AXIS2_OM_DOCUMENT = 10,
	AXIS2_OM_ELEMENT = 20,
	AXIS2_OM_DOCTYPE = 30,
	AXIS2_OM_COMMENT = 40,
	AXIS2_OM_ATTRIBUTE = 50,
	AXIS2_OM_NAMESPACE = 60,
	AXIS2_OM_PROCESSING_INSTRUCTION = 70,
	AXIS2_OM_TEXT = 80
} axis2_om_types_t;

/**
* This is the structure that defines a node in om tree 
* @param parent   - parent node if one is available
* @param parser   - carries a pointer to the XML_PullParser 
* @param element_type - the type of the element one of omtypes
* @param data_element  - stores the structs created for storing xml elements
*						e.g axis2_om_element_t axis2_om_text_t  
*
* we keep pointers parent , previous sibling , next sibling , 
* first child and last child for constructing and navigating the tree
*
*/
typedef struct axis2_om_node_t {
	struct axis2_om_node_t *parent;
	struct axis2_om_node_t *prev_sibling;
	struct axis2_om_node_t *next_sibling;
	struct axis2_om_node_t *first_child;
	struct axis2_om_node_t *current_child;
	struct axis2_om_node_t *last_child;
	axis2_stax_om_builder_t *builder;
	axis2_om_types_t element_type;
	int done;
	void* data_element;
} axis2_om_node_t;

/**
 * Create a node struct.
 * @return a node or NULL if there isn't enough memory
 */

axis2_om_node_t *axis2_om_node_create();

/**
 * destroy the node .
 * @param node to free
 */
void axis2_om_node_free(axis2_om_node_t *node);

/**
 * adds a child node to this node .
 * @param parent  parent node
 * @param child   child node
 */


void axis2_om_node_add_child(axis2_om_node_t *parent,axis2_om_node_t *child);

/**
 *	detach a node from the tree and resets the links
 *	@param node_to_detach the node to be detached
 *
 */

axis2_om_node_t *axis2_om_node_detach(axis2_om_node_t *node_to_detach);

/**
 *	inserts a sibling node after the current node
 *	@param current_node  the node in consideration 
 *  @param node_to_insert the node that will be inserted 
 */

void axis2_om_node_insert_sibling_after(axis2_om_node_t *current_nodee,axis2_om_node_t *node_to_insert);

void axis2_om_node_insert_sibling_before(axis2_om_node_t *current_ele,axis2_om_node_t *nodeto_insert);


//int axis2_om_node_build(axis2_om_node_t *node);

/**
 *	set a parent node to a given node
 * @param node 
 * @param parent the node that will be set as parent
 */

void axis2_om_node_set_parent(axis2_om_node_t *node,axis2_om_node_t *parent);



axis2_om_node_t *axis2_om_node_get_first_child(axis2_om_node_t *parent_node);

axis2_om_node_t *axis2_om_node_get_next_child(axis2_om_node_t *parent_node);


#endif // AXIS2_NODE_H
