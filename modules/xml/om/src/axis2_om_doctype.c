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

#include <axis2_om_doctype.h>
#include <stdlib.h>
#include <axis2_errno.h>

axis2_node_t *axis2_create_om_doctype(axis2_node_t * parent,const char *value)
{
    axis2_om_doctype_t *doctype = NULL;
    axis2_node_t *node = axis2_node_create();
    if (!node)
    {
		return NULL;
    }
    doctype = (axis2_om_doctype_t *) malloc(sizeof(axis2_om_doctype_t));
    if (!doctype)
    {
		fprintf(stderr,"%d Error",AXIS2_ERROR_OM_MEMORY_ALLOCATION);
		free(node);
		return NULL;
    }
    doctype->value     = strdup(value);

    node->data_element = doctype;
    
	node->element_type = AXIS2_OM_DOCTYPE;
    
	if (parent)
    {
		node->parent = parent;
		axis2_node_add_child(parent, node);
    }
    return node;
}

axis2_node_t *axis2_om_doctype_create_empty_doctype(axis2_node_t * parent)
{
    axis2_node_t *node = NULL;
    axis2_om_doctype_t *doctype = NULL;
    if (!node)
    {	       
		return NULL;
    }

    doctype = (axis2_om_doctype_t *) malloc(sizeof(axis2_om_doctype_t));

    if (!doctype)
    {
		free(node);
		return NULL;
    }
    
	doctype->value = NULL;
    node->data_element = doctype;
    node->element_type =AXIS2_OM_DOCTYPE;
    
	if (parent)
    {
		node->parent = parent;
		axis2_node_add_child(parent, node);
    }
    return node;
}

void axis2_free_om_doctype(axis2_om_doctype_t * om_doc)
{
    if (om_doc)
    {
		if (om_doc->value)
		{
	    	free(om_doc->value);
		}
		free(om_doc);
    }
}

char *axis2_om_doctype_get_value(axis2_node_t * doctype_node)
{
    if (!doctype_node || doctype_node->element_type !=AXIS2_OM_DOCTYPE)
    {
		return NULL;
    }
    return ((axis2_om_doctype_t *) (doctype_node->data_element))->value;
}

void axis2_om_doctype_set_value(axis2_node_t * doctype_node,const char *value)
{
    axis2_om_doctype_t *doctype = NULL;
    if (!doctype_node || doctype_node->element_type !=AXIS2_OM_DOCTYPE)
    {
		return;
    }
    doctype = (axis2_om_doctype_t *) (doctype_node->data_element);
    if (doctype->value)
    {
		free(doctype->value);
    }
    doctype->value = strdup(value);
}
