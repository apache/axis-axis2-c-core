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

#include <axis2c_om_namespace.h>
#include <string.h>
#include <stdlib.h>
#include <axis2c.h>



axis2c_om_namespace_t *axis2c_om_namespace_create(const char *uri,
						  const char *prefix)
{
    axis2c_om_namespace_t *ns = (axis2c_om_namespace_t *) malloc(
										sizeof(axis2c_om_namespace_t));
    if (!ns)
    {
		return NULL;
    }
    ns->uri = strdup(uri);
    ns->prefix = strdup(prefix);
	if(!(ns->prefix) || !(ns->uri))
	{
		free(ns);
		return NULL;	
	}	
    return ns;
}



void axis2c_om_namespace_free(axis2c_om_namespace_t * ns)
{
    if (ns)
	{ 
		if(ns->prefix)
		{
			free(ns->prefix);
		}
		if(ns->uri)
		{
			free(ns->uri);
		}
		free(ns);
	}
}

boolean_t axis2c_om_namespace_equals(axis2c_om_namespace_t * ns1,
			       axis2c_om_namespace_t * ns2)
{
    int uris_differ = 0;
    int prefixes_differ = 0;

    if (!ns1 || !ns2)
	return 0;

    if (ns1->uri && ns2->uri)
		uris_differ = strcmp(ns1->uri, ns2->uri);
    else
		uris_differ = (ns1->uri || ns2->uri);

    if (ns1->prefix && ns2->prefix)
		prefixes_differ = strcmp(ns1->prefix, ns2->prefix);
    else
		prefixes_differ = (ns1->prefix || ns2->prefix);

    return (!uris_differ && !prefixes_differ);
}
