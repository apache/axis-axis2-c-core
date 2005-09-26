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

#ifndef _AXISC_NODE_H_
#define _AXISC_NODE_H_

#include <apr.h>
#include <apr_pools.h>
#include <xmlpullparser.h>

#define TRUE  1                    // to be used as boolean values
#define FALSE 0



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

struct node_s;
typedef struct node_s node_t;

/*
* This is the structure that defines a node in om tree 
* parent   - parent node if one is available
* parser   - carries a pointer to the XML_PullParser 
* element_type - the type of the element one of omtypes
* data_element  - stores the structs created for storing xml 
*/


struct node_s{
	struct node_s *parent;
	struct node_s *prev_sibling;
	struct node_s *next_sibling;
	struct node_s *first_child;
	struct node_s *last_child;
	XML_PullParser *parser;
	int element_type;
	int done;
	void* data_element;
};


//create a node and allocate memory
node_t *create_node();
//free a given nod
void free_node(node_t *);
// add a node as a child of parent node
void node_add_child(node_t *parent,node_t *child);
// detach a node form the parent and reset the other links
node_t *node_detach(node_t *node_to_detach);
// insert a sibling node 
void node_insert_sibling_after(node_t *current_ele,node_t *nodeto_insert);
void node_insert_sibling_before(node_t *current_ele,node_t *nodeto_insert);

// build the tree 
int node_build(node_t *node);



void node_set_parent(node_t *parent);

node_t *node_get_next_sibling(node_t *node);

node_t *node_set_next_sibling(node_t *node);










#endif // _AXISC_NODE_
