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

#ifndef AXIS2_OM_TEXT_H
#define AXIS2_OM_TEXT_H

/**
 * @file axis2_om_attribute.h
 * @brief  represents xml text element
 */


#include <axis2_om_attribute.h>

typedef struct axis2_om_text_t
{
	char *value;
	axis2_om_namespace_t *ns;

	char *mime_type;
	int optimize;
	char *localname;
	int is_binary;
	char *content_id;
	axis2_om_attribute_t *attribute;
}axis2_om_text_t;


/**
 * Create a text struct and stores in in a node struct and returns a pointer
 * to the axis2_om_text_t struct
 * the data_element field of node struct points to the acctual axis2_text_t struct
 * The element type of axis2_node_t struct will be of type AXIS2_OM_TEXT
 * @param parent This can be null The parent element should be of type AXIS2_OM_ELEMENT
 * @return pointer to a axis2_om_text_t struct 
 */

axis2_om_text_t *axis2_om_text_create(axis2_om_node_t *parent,const char *value
						,axis2_om_node_t *node);


/**
 * access the value of the text struct
 * @param textnode node
 * @return char * to the value
 */
char* axis2_om_text_get_text(axis2_om_text_t *textnode);

/**
 *	free an axis2_om_text_t structure
 */

void axis2_om_text_free(axis2_om_text_t *text);
/**
 *	axis2
 *
 */




#endif // AXIS2_OM_TEXT_H
