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

#include <axis2_om_namespace.h>

axis2_status_t axis2_om_namespace_ops_free(axis2_environment_t *environment, axis2_om_namespace_t *om_namespace);

axis2_bool_t axis2_om_namespace_ops_equals(axis2_environment_t *environment, axis2_om_namespace_t *ns1,	axis2_om_namespace_t *ns2);

axis2_status_t axis2_om_namespace_ops_serialize(axis2_environment_t *environment, axis2_om_namespace_t *om_namespace, axis2_om_output_t* om_output);

axis2_om_namespace_t *axis2_om_namespace_create(axis2_environment_t *environment, 
        const axis2_char_t *uri,  const axis2_char_t *prefix)

{
    axis2_om_namespace_t *ns = NULL;
    
    if (!uri) /* there must be a URI */
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    
    ns = (axis2_om_namespace_t*) axis2_malloc(environment->allocator, 
										sizeof(axis2_om_namespace_t));
    
    if (!ns)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
    
    ns->uri = (axis2_char_t*)axis2_strdup(environment->string, uri);
    if (!ns->uri)
    {
        axis2_free(environment->allocator, ns);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
    
    ns->prefix = NULL;
    if (prefix)
    {
        ns->prefix = (axis2_char_t*)axis2_strdup(environment->string, prefix);
        if (!ns->prefix)
        {
            axis2_free(environment->allocator, ns);
            axis2_free(environment->allocator, ns->uri);
            environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
            return NULL;
        }    
	}
    
    /* operations */
    ns->ops = NULL;
    ns->ops = (axis2_om_namespace_ops_t*) axis2_malloc(environment->allocator, sizeof(axis2_om_namespace_ops_t));
    
    if (!ns->ops)
    {
        axis2_free(environment->allocator, ns);
        axis2_free(environment->allocator, ns->uri);
        axis2_free(environment->allocator, ns->prefix);
		environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
    
    ns->ops->free = axis2_om_namespace_ops_free;
    ns->ops->equals = axis2_om_namespace_ops_equals;
    ns->ops->serialize = axis2_om_namespace_ops_serialize;
    
    return ns;
}



axis2_status_t axis2_om_namespace_ops_free(axis2_environment_t *environment, struct axis2_om_namespace *om_namespace)
{
    if (om_namespace)
	{ 
		if(om_namespace->prefix)
		{
			axis2_free(environment->allocator, om_namespace->prefix);
            om_namespace->prefix = NULL;
		}
        
		if(om_namespace->uri)
		{
			axis2_free(environment->allocator, om_namespace->uri);
            om_namespace->uri = NULL;
		}
        
        if(om_namespace->ops)
		{
			axis2_free(environment->allocator, om_namespace->ops);
            om_namespace->ops = NULL;
		}
        
		axis2_free(environment->allocator, om_namespace);
	}
}

axis2_bool_t axis2_om_namespace_ops_equals(axis2_environment_t *environment, axis2_om_namespace_t * ns1,
			       axis2_om_namespace_t * ns2)
{
    int uris_differ = 0;
    int prefixes_differ = 0;

    if (!ns1 || !ns2)
	return AXIS2_FALSE;

    if (ns1->uri && ns2->uri)
		uris_differ = axis2_strcmp(environment->string, ns1->uri, ns2->uri);
    else
		uris_differ = (ns1->uri || ns2->uri);

    if (ns1->prefix && ns2->prefix)
		prefixes_differ = axis2_strcmp(environment->string, ns1->prefix, ns2->prefix);
    else
		prefixes_differ = (ns1->prefix || ns2->prefix);

    return (!uris_differ && !prefixes_differ);
}

axis2_status_t axis2_om_namespace_ops_serialize(axis2_environment_t *environment, axis2_om_namespace_t *om_namespace, axis2_om_output_t* om_output)
{
    int status = AXIS2_SUCCESS;
    if (!om_namespace || !om_output)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    
    if (om_namespace->uri && om_namespace->prefix)
        status = axis2_om_output_write (om_output, AXIS2_OM_NAMESPACE, 2,
                                        om_namespace->prefix, om_namespace->uri);
    return status;

}
