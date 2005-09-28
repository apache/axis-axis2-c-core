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


axis2c_qname_t *axis2c_create_qname(const char *localname,
				    const char *ns_uri, const char *prefix)
{
    axis2c_qname_t *qn = (axis2c_qname_t *) malloc(sizeof(axis2c_qname_t));
    if (!qn)
    {
	//fprintf(stderr,"Error allocating memory");
	return NULL;
    }
    qn->localpart = strdup(localname);
    qn->ns_uri = strdup(ns_uri);
    qn->prefix = strdup(prefix);
    return qn;
}

void axis2c_free_qname(axis2c_qname_t * qn)
{
    if (qn)
	free(qn);
}
