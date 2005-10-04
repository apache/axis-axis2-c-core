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

#include <axis2_om_node.h>

axis2_om_node_t *axis2_om_node_create()
{
    axis2_om_node_t *node = (axis2_om_node_t *) malloc(sizeof(axis2_om_node_t));
   
	if (!node)
    {
    	return NULL;
    }
    
    node->first_child = NULL;
    node->last_child = NULL;
    node->next_sibling = NULL;
    node->prev_sibling = NULL;
    node->parent = NULL;
    node->element_type = AXIS2_OM_INVALID;
    node->done = FALSE;
    node->builder = NULL;
    node->data_element = NULL;
    return node;
}


void axis2_om_node_free(axis2_om_node_t * node)
{
    if (!node)
	return;
}

void axis2_om_node_add_child(axis2_om_node_t * parent, axis2_om_node_t * child)
{
    if (!parent || !child)
	{
		return;
    }
    if (parent->first_child == NULL)
    {
	    parent->first_child = child;
    }
    else
    {
	    parent->last_child->next_sibling = child;
	    child->prev_sibling = parent->last_child;
    }
    
    child->parent = parent;
    parent->last_child = child;

}



axis2_om_node_t *axis2_om_node_detach(axis2_om_node_t * node_to_detach)
{
    axis2_om_node_t *parent = NULL;

    if (!node_to_detach)
    {
	    return NULL;
    }

    if (!(node_to_detach->parent))
    {
	    /* nodes that do not have a parent can't be detached  */
    	return NULL;
    }
    
    parent = node_to_detach->parent;
    
    if ((node_to_detach->prev_sibling) == NULL)
    {
	    parent->first_child = node_to_detach->next_sibling;
    } 
    else
    {
	    node_to_detach->prev_sibling->next_sibling =
	    node_to_detach->next_sibling;
    }
    if (!(node_to_detach->next_sibling))
    {
	    node_to_detach->next_sibling->prev_sibling =
	    node_to_detach->prev_sibling;
    }

    node_to_detach->parent = NULL;

    return node_to_detach;
}

void axis2_om_node_set_parent(axis2_om_node_t * node,axis2_om_node_t *parent)
{
	if(!parent || !node)
	{
		return ;
	}

	if(parent == node->parent )
	{/* same parent already exist */
		return ;
	}
	/* if a new parent is assigned in  place of existing */
	/*	one first the node should  be detached  */

	if(!(node->parent))
	{
		axis2_om_node_detach(node);
	}
	node->parent = parent;
}

/**
 * This will insert a sibling just after the current information item
 * @param node the node in consideration
 * @param nodeto_insert the node that will be inserted
 */
 
void axis2_om_node_insert_sibling_after(axis2_om_node_t *node,
			axis2_om_node_t *node_to_insert)
{
	if(!node || !node_to_insert )
	{
		return ;
	}
	node_to_insert->parent = node->parent;
	node_to_insert->prev_sibling = node;
	
	if(node->next_sibling)
	{
		node->next_sibling->prev_sibling = node_to_insert;
	}

    node_to_insert->next_sibling = node->next_sibling;
	node->next_sibling = node_to_insert;
}


void axis2_om_node_insert_sibling_before(axis2_om_node_t *node,
		axis2_om_node_t *node_to_insert)
{
	if(!node || !node_to_insert )
	{
		return;
	}

	node_to_insert->parent = node->parent;

	node_to_insert->prev_sibling = node->prev_sibling;
	node_to_insert->next_sibling = node;

	if(!(node->prev_sibling))
	{
		node->parent->first_child = node_to_insert;
	}
	else
	{
		node->prev_sibling->next_sibling = node_to_insert;
	
	}
	node->prev_sibling = node_to_insert;
}
