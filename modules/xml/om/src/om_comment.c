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

#include "om_comment.h"
#include "string.h"


node_t *create_om_comment(const char *value)
{
	node_t *node =NULL;
	om_comment_t *comment=NULL;
		
	node= create_node();
	if(!node)
	{
	//	fprintf(stderr,"Error");
		return NULL;
	}
	comment = (om_comment_t*)malloc(sizeof(om_comment_t));
	if(!comment)
	{
		free(node);
		//fprintf(stderr,"Error");
		return NULL;
	
	}
	comment->value = strdup(value);
	node->data_element = comment;
	node->element_type = OM_COMMENT;
	return node;
}


void om_comment_free(node_t *comment_node)
{
	if(!comment_node)
	{
		return;
	}
	if(comment_node->data_element)
	{
		free(((om_comment_t*)(comment_node->data_element))->value);
	}


}

