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

#ifndef AXIS2C_NODE_H
#define AXIS2C_NODE_H

#include <apr.h>
#include <apr_pools.h>
#include <xmlpullparser.h>

static apr_pool_t *om_pool; // a memory pool to be used for this module


enum omtypes{
	OM_DOCUMENT=10,
	OM_ELEMENT=20,
	OM_DOCTYPE=30,
	OM_COMMENT=40,
	OM_ATTRIBUTE=50,
	OM_NAMESPACE=60,
	OM_PI=70,
	OM_TEXT=80
};

/*
* This is the structure that defines a node in om tree 
* parent   - parent node if one is available
* parser   - carries a pointer to the XML_PullParser 
* element_type - the type of the element one of omtypes
* data_element  - stores the structs created for storing xml 
*/
typedef struct axis2c_node_t{
	struct axis2c_node_t *parent;
	struct axis2c_node_t *prev_sibling;
	struct axis2c_node_t *next_sibling;
	struct axis2c_node_t *first_child;
	struct axis2c_node_t *last_child;
	XML_PullParser *parser;
	int element_type;
	int done;
	void* data_element;
}axis2c_node_t;


//create a node and allocate memory
axis2c_node_t *axis2c_create_node();
//free a given nod
void axis2c_free_node(axis2c_node_t *);
// add a node as a child of parent node
void axis2c_node_add_child(axis2c_node_t *parent,axis2c_node_t *child);
// detach a node form the parent and reset the other links
axis2c_node_t *axis2c_node_detach(axis2c_node_t *node_to_detach);
// insert a sibling node 
void axis2c_node_insert_sibling_after(axis2c_node_t *current_ele,axis2c_node_t *nodeto_insert);
void axis2c_node_insert_sibling_before(axis2c_node_t *current_ele,axis2c_node_t *nodeto_insert);

// build the tree 
int axis2c_node_build(axis2c_node_t *node);



void axis2c_node_set_parent(axis2c_node_t *parent);

axis2c_node_t *axis2c_node_get_next_sibling(axis2c_node_t *node);

axis2c_node_t *axis2c_node_set_next_sibling(axis2c_node_t *node);










#endif // AXIS2C_NODE
