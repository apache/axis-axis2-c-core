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

#ifndef AXIS2C_OM_TEXT_H
#define AXIS2C_OM_TEXT_H

/**
 * @file axis2c_om_attribute.h
 * @brief  represents xml text element
 */


#include <axis2c_om_attribute.h>

typedef struct axis2c_om_text_t
{
	char *value;
	axis2c_om_namespace_t *ns;

	char *mime_type;
	int optimize;
	char *localname;
	int is_binary;
	char *content_id;
	axis2c_om_attribute_t *attribute;
}axis2c_om_text_t;


/**
 * Create a text struct and stores in in a node struct and returns a pointer
 * to the axis2c_node_t struct
 * the data_element field of node struct points to the acctual axis2c_text_t struct
 * The element type of axis2c_node_t struct will be of type AXIS2C_OM_TEXT
 * @param parent This can be null The parent element should be of type AXIS2C_OM_ELEMENT
 * @return pointer to a axis2c_node_t struct containing the text struct
 */

axis2c_node_t *axis2c_om_text_create(axis2c_node_t *parent,const char *value);


/**
 * access the value of the text struct
 * @param textnode node
 * @return char * to the value
 */
char* axis2c_om_text_get_text(axis2c_om_text_t *textnode);

/**
 *	free an axis2c_om_text_t structure
 */

void axis2c_om_text_free(axis2c_om_text_t *text);
/**
 *	axis2c
 *
 */




#endif // AXIS2C_OM_TEXT_H
