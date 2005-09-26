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

#include "qname.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


qname_t *create_qname(const char *localname,const char *ns_uri,const char *prefix)
{
	qname_t *qn = (qname_t*)malloc(sizeof(qname_t));
	if(!qn)
	{
		//fprintf(stderr,"Error allocating memory");
		return NULL;
	}
	qn->localpart	= strdup(localname);
	qn->ns_uri		= strdup(ns_uri);
	qn->prefix		= strdup(prefix);
	return qn;
}
void free_qname(qname_t *qn)
{
	if(qn)
		free(qn);
}

