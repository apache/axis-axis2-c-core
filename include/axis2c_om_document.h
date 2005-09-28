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

#ifndef AXIS2C_OM_DOCUMENT_H
#define AXIS2C_OM_DOCUMENT_H
#include <axis2c_node.h>

//typedef struct axis2c_stax_om_builder_t axis2c_stax_om_builder_t;


#define CHAR_SET_ENCODING "UTF-8"
#define XML_VERSION	"1.0"


typedef struct axis2c_om_document_t
{
	axis2c_node_t *root_element;
	axis2c_node_t *first_child;
	axis2c_node_t *last_child;
	int done;
	axis2c_stax_om_builder_t *builder;
	char *char_set_encoding;
	char *xml_version;

}axis2c_om_document_t;


axis2c_om_document_t *axis2c_create_om_document_with_root(axis2c_node_t *root_ele,axis2c_stax_om_builder_t *builder);

axis2c_om_document_t *axis2c_create_om_document(axis2c_stax_om_builder_t *builder);

void axis2c_free_om_document(axis2c_om_document_t *document);

axis2c_node_t *axis2c_om_document_get_document_element();

void axis2c_om_document_set_charset_encoding(axis2c_om_document_t *document,char *charset_encoding);

void axis2c_om_document_add_child(axis2c_om_document_t *document,axis2c_node_t *child);

#endif // AXIS2C_OM_DOCUMENT_H

