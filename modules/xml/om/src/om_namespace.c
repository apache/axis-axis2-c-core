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

#include "om_namespace.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



om_namespace_t *create_om_namespace(const char *uri,const char *prefix)
{
	om_namespace_t *ns=(om_namespace_t*)malloc(sizeof(om_namespace_t));
	if(!ns)
	{
		//fprintf(stderr,"Couldnot allocate momery");
		return NULL;
	}
	ns->uri		= strdup(uri);
	ns->prefix	= strdup(prefix);
	return ns;
}



void free_om_namespace(om_namespace_t *ns)
{
	if(ns)
		free(ns);
}

int om_namespace_equals(om_namespace_t *ns1,om_namespace_t *ns2);

