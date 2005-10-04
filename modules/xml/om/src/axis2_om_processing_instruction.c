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
 
#include <axis2_om_processing_instruction.h>
#include <axis2_om_node.h>
#include <stdlib.h>
#include <axis2_errno.h>

axis2_om_processing_instruction_t *axis2_om_processing_instruction_create(axis2_om_node_t *parent,const char *target,const char *value,axis2_om_node_t *pinode)
{
    axis2_om_processing_instruction_t *pi = NULL;
    axis2_om_node_t *pi_node = axis2_om_node_create();
    
	if (pi_node)
    {
		pi = (axis2_om_processing_instruction_t *)malloc(
				sizeof(axis2_om_processing_instruction_t));
		if (!pi)
		{
	    	free(pi_node);
	    	fprintf(stderr, "%d error", AXIS2_ERROR_OM_MEMORY_ALLOCATION);
	    	return NULL;
		}
		if(target)
		{
			pi->target = strdup(target);
		}
		if(value)
		{
			pi->value = strdup(value);
		}
	}
    
	pi_node->data_element = pi;
    pi_node->element_type =AXIS2_OM_PROCESSING_INSTRUCTION;

    if (parent)
    {
		pi_node->parent = parent;
		axis2_om_node_add_child(parent, pi_node);
    }
	pinode = pi_node;
	
    return pi;
}

axis2_om_processing_instruction_t *axis2_om_processing_instruction_create_empty(axis2_om_node_t *parent
								,axis2_om_node_t *node)
{
    axis2_om_node_t *pi_node = NULL;
    axis2_om_processing_instruction_t *pi = NULL;
    pi_node = axis2_om_node_create();
    if (pi_node)
    {
		pi = (axis2_om_processing_instruction_t *)malloc(
				sizeof(axis2_om_processing_instruction_t));
		if (!pi)
		{
	    	free(pi_node);
	    	return NULL;
		}
		pi->target = NULL;
		pi->value = NULL;
    }
    
	pi_node->data_element = pi;
    pi_node->element_type =AXIS2_OM_PROCESSING_INSTRUCTION;
    if (parent)
    {
		pi_node->parent = parent;
		axis2_om_node_add_child(parent, pi_node);
    }
	node = pi_node;
    return pi;
}


char *axis2_om_processing_instruction_get_value(axis2_om_node_t * pi_node)
{
    if (!pi_node || pi_node->element_type !=AXIS2_OM_PROCESSING_INSTRUCTION)
    {
		return NULL;
    }
    return ((axis2_om_processing_instruction_t *)(pi_node->data_element))->value;
}

char *axis2_om_processing_instruction_get_target(axis2_om_node_t * pi_node)
{
    if (!pi_node || pi_node->element_type !=AXIS2_OM_PROCESSING_INSTRUCTION)
    {
	return NULL;
    }
    return ((axis2_om_processing_instruction_t *)(pi_node->data_element))->target;
}

void axis2_om_processing_instruction_set_value(axis2_om_node_t * pi_node,
						const char *value)
{
    axis2_om_processing_instruction_t *pi = NULL;
    if (!pi_node || pi_node->element_type !=AXIS2_OM_PROCESSING_INSTRUCTION)
    {
		return;
    }
    pi = ((axis2_om_processing_instruction_t *)(pi_node->data_element));
    if (pi->value)
    {
		free(pi->value);
    }
    pi->value = strdup(value);
    pi = NULL;
}

void axis2_om_processing_instruction_set_target(axis2_om_node_t * pi_node,
						 const char *value)
{
    axis2_om_processing_instruction_t *pi = NULL;
    if (!pi_node || pi_node->element_type != AXIS2_OM_PROCESSING_INSTRUCTION)
    {
		return;
    }
    pi = (axis2_om_processing_instruction_t *) (pi_node->data_element);
    if (pi->target)
    {
		free(pi->target);
    }
    pi->target = strdup(value);
    pi = NULL;
    return;
}
