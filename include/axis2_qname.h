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

#ifndef AXIS2_QNAME_H
#define AXIS2_QNAME_H


/**
 * @file axis2_qname.h
 * @brief represents a qualified name
 */

#include <axis2.h>

static const char XML_NAMESPACE_URI[] ="http://www.w3c.org/XML/1998/namespace";

typedef struct axis2_qname_t{
	char *localpart;
	char *ns_uri;
	char *prefix;
	
}axis2_qname_t;

/**
 *	creates a qname struct
 *
 */

axis2_qname_t *axis2_qname_create(const char *localname,const char *ns_uri,const char *prefix);
/**
 * Free a qname struct
 */

void axis2_qname_free(axis2_qname_t *qn);
/**
 *	compare tow qname structs and returns true (1) if equals and false (0) otherwise
 *
 */

boolean_t axis2_qname_equals(axis2_qname_t *qn1,axis2_qname_t *qn2);

#endif // AXIS2_QNAME_H
