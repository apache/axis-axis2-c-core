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


#include <axis2c_om_document.h>
#include <stdlib.h>


om_document_t *axis2c_create_om_document_with_root(axis2c_node_t * root_ele,
						   stax_om_builder_t *
						   builder)
{

    axis2c_om_document_t *doc = (axis2c_om_document_t *) malloc(sizeof(axis2c_om_document_t));
    if (!doc)
    {

    }
    doc->builder = builder;
    doc->root_element = root_ele;
    doc->first_child = NULL;
    doc->last_child = NULL;
    doc->char_set_encoding = CHAR_SET_ENCODING;
    doc->xml_version = XML_VERSION;
    return doc;
}

axis2c_om_document_t *axis2c_create_om_document(stax_om_builder_t * builder)
{
    axis2c_om_document_t *doc = (axis2c_om_document_t *) malloc(sizeof(axis2c_om_document_t));
    if (!doc)
    {
	return NULL;
    }
    //doc->builder = builder;
    doc->char_set_encoding = strdup(CHAR_SET_ENCODING);
    doc->xml_version = strdup(XML_VERSION);
    doc->done = FALSE;
    doc->first_child = NULL;
    doc->last_child = NULL;
}

void axis2c_free_om_document(axis2c_om_document_t * doc)
{



}

axis2c_node_t *axis2c_om_document_get_document_element(axis2c_om_document_t * document)
{
    //while(document->root_element == NULL)
    //{}
    return document->root_element;
}

void axis2c_om_document_set_charset_encoding(axis2c_om_document_t * document,
					     char *charset_encoding)
{
    if (document)
    {
	if (document->char_set_encoding)
	    free(document->char_set_encoding);

	document->char_set_encoding = strdup(charset_encoding);
    }
}

char *axis2c_om_document_get_charset_encoding(axis2c_om_document_t * document)
{
    if (document)
    {
	return strdup(document->char_set_encoding);
    }
}


void axis2c_om_document_add_child(axis2c_om_document_t * document, axis2c_node_t * child)
{
    if (!document || !child || child->element_type != OM_ELEMENT)
    {
	//error
	return;

    }
    if (document->first_child == NULL)
    {
	document->first_child = child;
	child->prev_sibling = NULL;
    }
    else
    {
	child->prev_sibling = document->last_child;
	document->last_child->next_sibling = child;
    }
    child->next_sibling = NULL;
}


