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

#include "node.h"

node_t *create_node()
{
	node_t *node = (node_t*)malloc(sizeof(node_t));
	if(!node)
	{
		return NULL;	
	}
	node->first_child	=NULL;
    node->last_child	=NULL;
    node->next_sibling	=NULL;
	node->prev_sibling	=NULL;
    node->parent		=NULL;
	node->element_type	=0;
	node->done			=FALSE;
	node->parser		=NULL;
	node->data_element	=NULL;
	return node;
}


void free_node(node_t *node)
{
	if(!node)
		return;
	//if(node->
}

void node_add_child(node_t *parent,node_t *child)
{
	if(!parent || !child)
		return;
	if(parent->first_child == NULL)
	{
		parent->first_child = child;
	}else
	{
		parent->last_child->next_sibling = child;
		child->prev_sibling	= parent->last_child;
	}
	child->parent = parent;
	parent->last_child = child;
}



node_t *detach_node(node_t *node_to_detach)
{
	node_t *node=NULL;
	if(!node_to_detach)
	{
		return NULL;
	}
	if(!(node_to_detach->parent))
	{
			// elements that do not have a parent can't be detached
		return NULL;
	}
	if((node_to_detach->prev_sibling) == NULL)
		{
		node = node_to_detach->parent;
		node->first_child = node_to_detach->next_sibling;
		}
	else
		{
		node_to_detach->prev_sibling->next_sibling = node_to_detach->next_sibling;
		}
	node_to_detach->parent = NULL;
	return node;
}







