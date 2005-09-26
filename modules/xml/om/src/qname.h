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

#ifndef _AXISC_QNAME_H_
#define _AXISC_QNAME_H_

static const char XML_NAMESPACE_URI[] ="http://www.w3c.org/XML/1998/namespace";

struct qname_s;
typedef struct qname_s qname_t;

struct qname_s{
	char *localpart;
	char *ns_uri;
	char *prefix;
	
};

qname_t *create_qname(const char *localname,const char *ns_uri,const char *prefix);
void free_qname(qname_t *qn);

#endif // _AXISC_QNAME_H_

