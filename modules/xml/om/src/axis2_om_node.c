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
#include <axis2_om_text.h>

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
    node->done = AXIS2_FALSE;
    node->builder = NULL;
    node->data_element = NULL;
	node->current_child = NULL;
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

axis2_om_node_t *axis2_om_node_get_first_child(axis2_om_node_t *parent_node)
{
	/**  */
	if(!parent_node)
	{
		return NULL;
	}
	if(parent_node->first_child)
	{
		parent_node->current_child = parent_node->first_child;
		return parent_node->first_child;		
	}
	return NULL;
}

axis2_om_node_t *axis2_om_node_get_next_child(axis2_om_node_t *parent_node)
{
	axis2_om_node_t *node=NULL;
	if(parent_node && !(parent_node->first_child))
	{
		/*fprintf(stderr,"Error ");*/
		return NULL;	
	}
	
	if(parent_node && parent_node->first_child  && !(parent_node->current_child))	
	{
		/*fprintf(stderr,"Error first call get_first_child");*/
		return NULL;		
	}
	if(parent_node->current_child->next_sibling)
	{
		node= parent_node->current_child->next_sibling;
		parent_node->current_child = node;
		return node;		
	}
	return NULL;
}

int *axis2_om_node_serialize(axis2_om_node_t *om_node, axis2_om_output_t * om_output)
{
    if (!om_node || !om_output)
        return AXIS2_ERROR_INVALID_POINTER_PARAMATERS;
    
    int status = AXIS2_SUCCESS;
    
    switch (om_node->element_type)
    {
        case AXIS2_OM_ELEMENT:
            status = axis2_om_element_serialize_start_part( (axis2_om_element_t*)om_node->data_element, om_output );
            if (status != AXIS2_SUCCESS) 
                return status;
            break;
        case AXIS2_OM_TEXT:
            status = axis2_om_text_serialize((axis2_om_text_t*)om_node->data_element, om_output );
            return status;
            break;
    }

    /* handle children*/
    axis2_om_node_t *child_node = axis2_om_node_get_first_child(om_node);

    while (child_node)
    {
        status = axis2_om_node_serialize(child_node, om_output);
            if (status != AXIS2_SUCCESS) 
                return status;
        child_node = axis2_om_node_get_next_child(om_node);
    }
        
    switch (om_node->element_type)
    {
        case AXIS2_OM_ELEMENT:
            status = axis2_om_element_serialize_end_part( (axis2_om_element_t*)om_node->data_element, om_output );
            if (status != AXIS2_SUCCESS) 
                return status;
            break;
    }
    
    return status;

}
