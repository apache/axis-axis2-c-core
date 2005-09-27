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

#include <axis2c_om_doctype.h>
#include <stdlib.h>

node_t *axis2c_create_om_doctype(node_t * parent, const char *value)
{
    om_doctype_t *doctype = NULL;
    node_t *node = axis2c_create_node();
    if (!node)
    {
	//fprintf(stderr,"Error");
	return NULL;
    }
    doctype = (om_doctype_t *) malloc(sizeof(om_doctype_t));
    if (!doctype)
    {
	free(node);
	return NULL;
    }
    doctype->value = strdup(value);
    node->data_element = doctype;
    node->element_type = OM_DOCTYPE;
    if (parent)
    {
	node->parent = parent;
	axis2c_node_add_child(parent, node);
    }
    return node;
}

node_t *axis2c_create_empty_om_doctype(node_t * parent)
{
    node_t *node = NULL;
    om_doctype_t *doctype = NULL;
    if (!node)
    {				// error handling       
	return NULL;
    }

    doctype = (om_doctype_t *) malloc(sizeof(om_doctype_t));

    if (!doctype)
    {
	free(node);
	return NULL;
    }
    doctype->value = NULL;
    node->data_element = doctype;
    node->element_type = OM_DOCTYPE;
    if (parent)
    {
	node->parent = parent;
	axis2c_node_add_child(parent, node);
    }
    return node;
}

void axis2c_free_om_doctype(om_doctype_t * om_doc)
{
    if (om_doc)
    {
	if (om_doc->value)
	    free(om_doc->value);
	free(om_doc);
    }
}

char *axis2c_om_doctype_get_value(node_t * doctype_node)
{
    if (!doctype_node || doctype_node->element_type != OM_DOCTYPE)
    {
	return NULL;
    }
    return strdup(((om_doctype_t *) (doctype_node->data_element))->value);
}
void axis2c_om_doctype_set_value(node_t * doctype_node, const char *value)
{
    om_doctype_t *doctype = NULL;
    if (!doctype_node || doctype_node->element_type != OM_DOCTYPE)
    {
	return;
    }
    doctype = (om_doctype_t *) (doctype_node->data_element);
    if (doctype->value)
    {
	free(doctype->value);
    }
    doctype->value = strdup(value);
}
