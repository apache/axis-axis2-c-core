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
/*#include <string.h>
#include <axis2_om_node.h>
#include <axis2_om_output.h>
#include <axis2_errno.h>
*/

/* operations */
axis2_status_t axis2_om_text_impl_free(axis2_environment_t *environment, axis2_om_text_t *om_text);
axis2_status_t axis2_om_text_impl_serialize(axis2_environment_t *environment, const axis2_om_text_t *om_text, axis2_om_output_t* om_output);

axis2_om_text_t *axis2_om_text_create(axis2_environment_t *environment, axis2_om_node_t *parent,const axis2_char_t *value
						,axis2_om_node_t **node)
{
    
    axis2_om_text_t *om_text = NULL;
    
    if (!node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    
    *node = axis2_om_node_create(environment);
    
    if (!(*node))
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
    
    om_text = (axis2_om_text_t*) axis2_malloc(environment->allocator, sizeof(axis2_om_text_t));
    
    if (!om_text)
    {
        axis2_free(environment->allocator, *node);
		environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
    
    om_text->value = NULL;
    if (value)
        om_text->value = (axis2_char_t*) axis2_strdup(environment->string, value);
    
    /* om_text->attribute = NULL; */
    om_text->content_id = NULL;
    om_text->mime_type = NULL;
    
    (*node)->data_element = om_text;
    (*node)->node_type = AXIS2_OM_TEXT;

    (*node)->done = AXIS2_FALSE;
    
	if(parent && parent->node_type == AXIS2_OM_ELEMENT)
	{
		(*node)->parent = parent;
		axis2_om_node_add_child(environment, parent,*node);
	}
    
    /* operations */
    om_text->ops = NULL;
    om_text->ops = (axis2_om_text_ops_t*) axis2_malloc(environment->allocator, sizeof(axis2_om_text_ops_t));
    
    if (!om_text->ops)
    {
        axis2_free(environment->allocator, *node);
        axis2_free(environment->allocator, om_text);
		environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
    
    om_text->ops->axis2_om_text_ops_free = axis2_om_text_impl_free;
    om_text->ops->axis2_om_text_ops_serialize = axis2_om_text_impl_serialize;

    return om_text;
}


axis2_status_t axis2_om_text_impl_free(axis2_environment_t *environment, axis2_om_text_t *om_text)
{
    if (!om_text)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    
    if (om_text->value)
        axis2_free(environment->allocator, om_text->value);
    
    if (om_text->ops)
        axis2_free(environment->allocator, om_text->ops);
    
    if (om_text)
        axis2_free(environment->allocator, om_text);
    
    return AXIS2_SUCCESS;
}

axis2_status_t axis2_om_text_impl_serialize(axis2_environment_t *environment, const axis2_om_text_t *om_text, axis2_om_output_t* om_output)
{
    int status = AXIS2_SUCCESS;

    if (!om_text || !om_output)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    
    if (om_text->value)
        status = axis2_om_output_write (environment, om_output, AXIS2_OM_TEXT, 1,
                                        om_text->value);
    return status;
}
