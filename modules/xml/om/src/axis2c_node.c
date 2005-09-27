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

#include <axis2c_node.h>

axis2c_node_t *axis2c_create_node()
{
    axis2c_node_t *node = (axis2c_node_t *) malloc(sizeof(axis2c_node_t));
    if (!node)
    {
	return NULL;
    }
    node->first_child = NULL;
    node->last_child = NULL;
    node->next_sibling = NULL;
    node->prev_sibling = NULL;
    node->parent = NULL;
    node->element_type = 0;
    node->done = FALSE;
    node->builder = NULL;
    node->data_element = NULL;
    return node;
}


void axis2c_free_node(axis2c_node_t * node)
{
    if (!node)
	return;
    //if(node->
}

void axis2c_node_add_child(axis2c_node_t * parent, axis2c_node_t * child)
{
    if (!parent || !child)
	return;
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



axis2c_node_t *axis2c_detach_node(axis2c_node_t * axis2c_node_to_detach)
{
    axis2c_node_t *parent = NULL;
    axis2c_node_t *next_sibling = NULL;
    if (!axis2c_node_to_detach)
    {
	return NULL;
    }

    if (!(axis2c_node_to_detach->parent))
    {
	/* nodes that do not have a parent can't be detached
	 */
	return NULL;
    }
    parent = axis2c_node_to_detach->parent;
    if ((axis2c_node_to_detach->prev_sibling) == NULL)
    {
	parent->first_child = axis2c_node_to_detach->next_sibling;
    }
    else
    {
	axis2c_node_to_detach->prev_sibling->next_sibling =
	    axis2c_node_to_detach->next_sibling;
    }
    if (!(axis2c_node_to_detach->next_sibling))
    {
	axis2c_node_to_detach->next_sibling->prev_sibling =
	    axis2c_node_to_detach->prev_sibling;
    }

    axis2c_node_to_detach->parent = NULL;

    return axis2c_node_to_detach;

}

void axis2c_node_set_parent(axis2c_node_t * node)
{

}

