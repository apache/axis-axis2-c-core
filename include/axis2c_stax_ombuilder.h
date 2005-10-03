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

/**
 * @file axis2c_stax_om_builder.h
 * @brief om model stax builder 
 */



#define XMLNS_URI "http://www.w3.org/XML/1998/namespace"
#define XMLNS_PREFIX "xml"

#include <xmlpullparser.h>
#include <axis2c_node.h>
#include <axis2c_om_document.h>

struct axis2c_stax_om_builder_s
{
	XML_PullParser			*parser;	
	axis2c_node_t			*lastnode;
	axis2c_om_document_t	*document;
	boolean_t				done;
	boolean_t				parser_accessed;
	boolean_t				cache;
};

/**
 *	create an stax_om_builder
 * @param parser   This is the XMLStreamReader of xmlparser
 *				   [ Guththila ]	
 */

axis2c_stax_om_builder_t *axis2c_stax_om_builder_create(XML_PullParser *parser);

/**
 *	Create an om element and adds to the document
 *	@param builder pointer to the builder
 *  @param returns a pointer to axis2c_node_t containing the element or NULL if there
 *  isn't enough memory
 */

axis2c_node_t *axis2c_stax_om_builder_create_om_element(
						axis2c_stax_om_builder_t *builder);

/**
 *	creates an om comment
 *	@param builder
 *  @returns 
 */


axis2c_node_t *axis2c_stax_om_builder_create_om_comment(
						axis2c_stax_om_builder_t *builder);

/**
 *	create an om doctype
 *
 */
axis2c_node_t *axis2c_stax_om_builder_create_om_doctype(
						axis2c_stax_om_builder_t *builder_t);

/**
 *	create om_processing_instruction
 */

axis2c_node_t *axis2c_stax_om_builder_create_om_processing_instruction(
						axis2c_stax_om_builder_t *builder);

/**
 *	End element processing
 */
void axis2c_stax_om_builder_end_element(axis2c_stax_om_builder_t *builder);
/**
 *	move parser forward and reacts to events 
 */

int axis2c_stax_om_builder_next(axis2c_stax_om_builder_t *builder);

/**
 *	process attrbites 
 *
 */

void axis2c_stax_om_builder_process_attributes(axis2c_stax_om_builder_t *builder,axis2c_node_t *element_node);

/**
 *	create om text
 *
 */
axis2c_node_t *axis2c_stax_om_builder_create_om_text(axis2c_stax_om_builder_t *builder);

/**
 *	discard building an element
 */


void axis2c_stax_om_builder_discard(axis2c_stax_om_builder_t *builder,axis2c_node_t *element_node);

/**
 *	process namespaces 
 *
 */

axis2c_node_t *axis2c_stax_om_builder_process_namespace_data(axis2c_stax_om_builder_t *builder,axis2c_node_t *element,int is_soap_element);




#endif // AXIS2C_STAX_OMBUILDER_H
