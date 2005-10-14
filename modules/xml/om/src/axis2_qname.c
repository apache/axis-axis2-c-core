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

#include <axis2_qname.h>
#include <axis2_environment.h>

axis2_status_t axis2_qname_impl_free(axis2_environment_t *environment,axis2_qname_t *qname);
axis2_bool_t axis2_qname_impl_equals(axis2_environment_t *environment,axis2_qname_t *qn1,axis2_qname_t *qn2);
axis2_qname_t *axis2_qname_create(axis2_environment_t *environment,const axis2_char_t *localpart,
				    const axis2_char_t *namespace_uri, const axis2_char_t *prefix);


axis2_status_t axis2_qname_impl_free(axis2_environment_t *environment,axis2_qname_t *qname)
{
	if (!qname)
	{
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
	if(qname->localpart)
	{
		axis2_free(environment->allocator,qname->localpart);
	}
	if(qname->namespace_uri)
	{
		axis2_free(environment->allocator,qname->namespace_uri);
	}
	if(qname->prefix)
	{
		axis2_free(environment->allocator,qname->prefix);
	}
	if(qname->ops)
	{
	    axis2_free(environment->allocator,qname->ops);
	}
	axis2_free(environment->allocator,qname);
	return AXIS2_SUCCESS;
	
}

axis2_bool_t axis2_qname_impl_equals(axis2_environment_t *environment,axis2_qname_t *qn1,axis2_qname_t *qn2)
{

	int uris_differ = 0;
	int localparts_differ = 0;
	
	if(!qn1 || !qn2)
	{
	    environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FALSE;
	}
		
	if(qn1 && qn2)
	{
		if( qn1 == qn2)
		{
			return AXIS2_TRUE;
		}
	}
	if(qn1->localpart && qn2->localpart)
	{
		localparts_differ = axis2_strcmp(environment->string  ,qn1->localpart , qn2->localpart);
	}
	else
	{
		localparts_differ = ((qn1->localpart) || (qn2->localpart));
	}
	if( qn1->namespace_uri && qn2->namespace_uri )
	{
		uris_differ = axis2_strcmp(environment->string, qn1->namespace_uri , qn2->namespace_uri ); 
	}
	else
	{
		uris_differ	= ((qn1->namespace_uri) || (qn2->namespace_uri));
	}
	return (!uris_differ && !localparts_differ) ? AXIS2_TRUE : AXIS2_FALSE ;

}


axis2_qname_t *axis2_qname_create(axis2_environment_t *environment,const axis2_char_t *localpart,
				    const axis2_char_t *namespace_uri, const axis2_char_t *prefix)
{
    
    axis2_qname_t *qn=NULL;
    /* localpart or prefix can't be null */
    if(!localpart)   
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    
    qn = (axis2_qname_t *)axis2_malloc(environment->allocator,sizeof(axis2_qname_t));
    if (!qn)
    {
		environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
    }
    /* set properties */ 
    
    qn->localpart = axis2_strdup(environment->string,localpart);
	if(!(qn->localpart))
	{
	   axis2_free(environment->allocator,qn);
	   environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
	   return NULL;
	}
	
	if(!prefix)
	{
	    qn->prefix = axis2_strdup(environment->string,"");
	}
	else
	{
	qn->prefix = axis2_strdup(environment->string,prefix);
	}
	if(!(qn->prefix))
	{
	    axis2_free(environment->allocator,qn->localpart);
	    axis2_free(environment->allocator,qn);
	    environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
	    return NULL;
	}
	if(!namespace_uri)
	{
	    qn->namespace_uri = axis2_strdup(environment->string,"");
	}
	else
	{
	    qn->namespace_uri = axis2_strdup(environment->string,namespace_uri);
	}
	if(!(qn->namespace_uri))
	{
	    axis2_free(environment->allocator,qn->localpart);
	    axis2_free(environment->allocator,qn->prefix);
	    axis2_free(environment->allocator,qn);
	    environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
	    return NULL;   
	}
	

    qn->ops = NULL;
    qn->ops = axis2_malloc(environment->allocator,sizeof(axis2_qname_ops_t));
    /* operations */
    if(!qn->ops)
    {
        axis2_free(environment->allocator,qn->localpart);
        if(qn->namespace_uri)
            axis2_free(environment->allocator,qn->namespace_uri);
        axis2_free(environment->allocator,qn->prefix);
        axis2_free(environment->allocator,qn);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }    
    
    qn->ops->axis2_qname_ops_free = axis2_qname_impl_free;
	qn->ops->axis2_qname_ops_equals = axis2_qname_impl_equals;
    return qn;
}
