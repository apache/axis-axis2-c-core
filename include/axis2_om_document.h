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

#ifndef AXIS2_OM_DOCUMENT_H
#define AXIS2_OM_DOCUMENT_H
#include <axis2_om_node.h>

//typedef struct axis2_stax_om_builder_t axis2_stax_om_builder_t;

/**
 * @file axis2_om_node.h
 * @brief om_document represents a xml document
 *	
 */



#define CHAR_SET_ENCODING "UTF-8"
#define XML_VERSION	"1.0"


typedef struct axis2_om_document_t
{
	axis2_om_node_t *root_element;
	axis2_om_node_t *last_child;
	axis2_om_node_t *first_child;
	int done;
	axis2_stax_om_builder_t *builder;
	char *char_set_encoding;
	char *xml_version;

}axis2_om_document_t;


/**
 *	creates and returns axis2_om_document returns null if there isn't enough memory
 *  @param root_ele pointer to document's root node IF NUll is parsed a document root
 *         will be set to NULL
 *  @param builder pointer to xml builder 
 */

axis2_om_document_t *axis2_om_document_create(axis2_om_node_t *root_ele,axis2_stax_om_builder_t *builder);

/** 
 *  Free axis2_om_document_t struct
 *	@param document 
 */


void axis2_free_om_document(axis2_om_document_t *document);

/**
 *	causes the parser to proceed if the xml input is not finised yet
 *	@param document 
 */


void axis2_om_document_build_next(axis2_om_document_t *document);

/**
 *	adds the child node as a child to the back of the list
 */
void axis2_om_document_add_child(axis2_om_document_t *document,axis2_om_node_t *child);

/**
 *	This cause the parser to proceed 	
 *	@param document
 *	@ returns The first 
 */


axis2_om_node_t *axis2_om_document_get_root_element(axis2_om_document_t *document);

/**
 * set the CHAR SET ENCODING of this document
 */
void axis2_document_set_char_set_encoding(axis2_om_document_t *char_set_encoding);

/**
 * set the xml version of this document
 */
void axis2_document_set_xmlversion(axis2_om_document_t *document);


axis2_om_node_t *axis2_om_document_get_next_sibling(axis2_om_document_t *document);


axis2_om_node_t *axis2_om_document_get_first_child(axis2_om_document_t *document);

axis2_om_node_t *axis2_om_document_get_current_child(axis2_om_document_t *document);

/*	to implement
 *	get first child with qname
 *	get children iterator
 *	get children with name
 */




#endif // AXIS2_OM_DOCUMENT_H
