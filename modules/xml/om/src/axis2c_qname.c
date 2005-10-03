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

#include <axis2c_qname.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <axis2c_errno.h>

axis2c_qname_t *axis2c_qname_create(const char *localname,
				    const char *ns_uri, const char *prefix)
{
    axis2c_qname_t *qn = (axis2c_qname_t *) malloc(sizeof(axis2c_qname_t));
    if (!qn)
    {
		fprintf(stderr," %d Error ",AXIS2C_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
    }
	if(localname)
	{
		qn->localpart = strdup(localname);
	}
	if(ns_uri)
	{
		qn->ns_uri = strdup(ns_uri);
	}
	if(prefix)
	{
		qn->prefix = strdup(prefix);
	}
    return qn;
}

void axis2c_free_qname(axis2c_qname_t * qn)
{
	if (qn)
	{
		if(qn->localpart)
		{
			free(qn->localpart);
		}
		if(qn->ns_uri)
		{
			free(qn->ns_uri);
		}
		if(qn->prefix)
		{
			free(qn->prefix);
		}
		free(qn);
	}
}

boolean_t axis2c_qname_equals(axis2c_qname_t *qn1,axis2c_qname_t *qn2)
{
	int uris_differ = 0;
	int localparts_differ = 0;

	if(!qn1 && !qn2)
	{
		if( qn1 == qn2)
		{
			return true;
		}
	}
	if(qn1->localpart && qn2->localpart)
	{
		localparts_differ = strcmp(qn1->localpart , qn2->localpart);
	}
	else
	{
		localparts_differ = (!(qn1->localpart) || !(qn2->localpart));
	}
	if( qn1->ns_uri && qn2->ns_uri )
	{
		uris_differ = strcmp( qn1->ns_uri , qn2->ns_uri ); 
	}
	else
	{
		uris_differ	= (!(qn1->ns_uri) || !(qn2->ns_uri));
	}
	return (!uris_differ && !localparts_differ);
}
