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

#include <axis2_om_doctype.h>

axis2_status_t axis2_om_doctype_impl_free(axis2_environment_t *environment, axis2_om_doctype_t *doctype);

axis2_om_doctype_t *axis2_om_doctype_create(axis2_environment_t *environment, axis2_om_node_t *parent, const axis2_char_t *value, axis2_om_node_t **node)
{
    axis2_om_doctype_t *doctype = NULL;
    
    if (!node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    *node = axis2_om_node_create(environment);
    if (!*node)
    {
		environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
    }
    
    doctype = (axis2_om_doctype_t *) axis2_malloc(environment->allocator, sizeof(axis2_om_doctype_t));
    if (!doctype)
    {
		axis2_om_node_free(environment, *node);
		environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
    }
    
    doctype->value = NULL;
    
    if (value)
    {
        doctype->value = axis2_strdup(environment->string, value);
        if (!doctype->value)
        {
            axis2_om_node_free(environment, *node);
            axis2_free(environment->allocator, doctype);
            environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
            return NULL;
        }
    }

    (*node)->data_element = doctype;
    (*node)->node_type = AXIS2_OM_DOCTYPE;
	
	if (parent)
    {
        (*node)->parent = parent;
        axis2_om_node_add_child (environment, parent, (*node));
    }
	
    /* operations */
    doctype->ops = NULL;
    doctype->ops = (axis2_om_doctype_ops_t*) axis2_malloc(environment->allocator, sizeof(axis2_om_doctype_ops_t));
    if (!doctype->ops)
    {
        axis2_om_node_free(environment, *node);
        axis2_free(environment->allocator, doctype);
        axis2_free(environment->allocator, doctype->value);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    
    doctype->ops->axis2_om_doctype_ops_free = axis2_om_doctype_impl_free;
    
    return doctype;
}


axis2_status_t axis2_om_doctype_impl_free(axis2_environment_t *environment, axis2_om_doctype_t *doctype)
{
    if (doctype)
    {
		if (doctype->value)
		{
	    	axis2_free(environment->allocator, doctype->value);
		}
		axis2_free(environment->allocator, doctype);
    }
}
