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

#ifndef AXIS2C_STAX_OMBUILDER_H
#define AXIS2C_STAX_OMBUILDER_H

#define XMLNS_URI "http://www.w3.org/XML/1998/namespace"
#define XMLNS_PREFIX "xml"

#include <xmlpullparser.h>
#include <axis2c_node.h>
#include <axis2c_om_document.h>

struct axis2c_stax_om_builder_s
{
	XML_PullParser	*parser;	
	axis2c_node_t	*lastnode;
	axis2c_om_document_t   *document;
	int	done;
	int	parser_accessed;
	int	cache;

};


axis2c_stax_om_builder_t *axis2c_create_stax_om_builder(XML_PullParser *parser);

axis2c_stax_om_builder_t *axis2c_create_stax_om_builder(XML_PullParser *parser);

axis2c_node_t *axis2c_stax_om_builder_create_om_element(axis2c_stax_om_builder_t *builder);

axis2c_node_t *axis2c_stax_om_builder_create_om_comment(axis2c_stax_om_builder_t *builder);

axis2c_node_t *axis2c_stax_om_builder_create_om_doctype(axis2c_stax_om_builder_t *builder_t);

axis2c_node_t *axis2c_stax_om_builder_create_om_pi(axis2c_stax_om_builder_t *builder);

void axis2c_stax_om_builder_end_element(axis2c_stax_om_builder_t *builder);

void axis2c_stax_om_builder_process_attributes(axis2c_stax_om_builder_t *builder,axis2c_node_t *element_node);

axis2c_node_t *axis2c_stax_om_builder_create_om_text(axis2c_stax_om_builder_t *builder);

void axis2c_stax_om_builder_discard(axis2c_stax_om_builder_t *builder,axis2c_node_t *element_node);

void axis2c_stax_om_builder_end_element(axis2c_stax_om_builder_t *builder);

int axis2c_stax_om_builder_next(axis2c_stax_om_builder_t *builder);

axis2c_node_t *axis2c_stax_om_builder_get_document_element(axis2c_stax_om_builder_t *builder);

axis2c_node_t *axis2c_stax_om_builder_process_namespace_data(axis2c_stax_om_builder_t *builder,axis2c_node_t *element,int is_soap_element);
















#endif // AXIS2C_STAX_OMBUILDER_H

