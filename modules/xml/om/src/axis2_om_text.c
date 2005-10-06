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

#include <axis2_om_text.h>
#include <string.h>
#include <axis2_om_node.h>
#include <axis2_errno.h>

axis2_om_text_t *axis2_om_text_create(axis2_om_node_t *parent,const char *value
						,axis2_om_node_t *text_node)
{
    axis2_om_text_t *text;
    axis2_om_node_t *node = axis2_om_node_create();
    if (!node)
    {
		fprintf(stderr,"%d Error",AXIS2_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
    }
    text = (axis2_om_text_t *) malloc(sizeof(axis2_om_text_t));
    if (!text)
    {
		fprintf(stderr,"%d Error",AXIS2_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
    }
    text->value = strdup(value);
    text->attribute = NULL;
    text->content_id = NULL;
    text->mime_type = NULL;
    node->data_element = text;
    node->element_type = AXIS2_OM_TEXT;

	if(parent && parent->element_type == AXIS2_OM_ELEMENT)
	{
		node->done = TRUE;
		node->parent = parent;
		axis2_om_node_add_child(parent,node);
	}
	text_node = node;
    return text;
}



char *axis2_om_text_get_text(axis2_om_text_t * text)
{
    if (!text)
    {
		return NULL;
    }
    if (text->value)
	{
		return text->value;
	}
    else
    {
	//MTOM handling logic should go hear

    }
    return NULL;
}

int axis2_om_text_serialize(axis2_om_text_t *om_text, axis2_om_output_t* om_output)
{
    int status = AXIS2_SUCCESS;
    // TODO : handle null pointer errors
    if (om_text->value)
        status = axis2_om_output_write (om_output, AXIS2_OM_TEXT, 1,
                                        om_text->value);
    return status;
}