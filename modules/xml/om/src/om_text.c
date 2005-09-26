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

#include <axis2c_om_text.h>
#include <string.h>
#include <axis2c_node.h>

node_t *create_om_text(const char *value)
{
	om_text_t *text;
	node_t *node= create_node();
	if(!node)
	{
			// error handling
		return NULL;
	}
	text =(om_text_t*)malloc(sizeof(om_text_t));
	if(!text)
	{
		// error handling
		return NULL;
	}
	text->value =strdup(value);
	text->attribute=NULL;
	text->content_id=NULL;
	text->mime_type = NULL;
	node->data_element = text;
	node->element_type = OM_TEXT;
	return node;
}
node_t *create_om_text_with_parent(node_t *parent,const char *value)
{	
	node_t *node;
	if(!parent)
		return NULL;
	node = create_om_text(value);
	if(!node)
	{
		// error handling
		return NULL;
	}
	node->done = TRUE;
	// set parent as the parent of this text node
	node->parent=parent;
	node_add_child(parent,node);
	return node;
}

char *om_text_get_text(om_text_t *text)
{
	if(!text)
	{
		// error 
		return NULL;
	}
	if(text->value)
		return text->value;

	else
	{
		//MTOM handling logic should go hear
	
	}
	return NULL;
}







