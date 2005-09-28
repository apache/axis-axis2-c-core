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

#ifndef AXIS2C_OM_DOCTYPE_H
#define AXIS2C_OM_DOCTYPE_H
#include <axis2c_node.h>


typedef struct axis2c_om_doctype_t
{
	char *value;
}axis2c_om_doctype_t;

axis2c_node_t *create_om_doctype(axis2c_node_t *parent,const char *value);

axis2c_node_t *create_empty_om_doctype(axis2c_node_t *parent);

void free_om_doctype(axis2c_om_doctype_t *om_doc);

char *om_doctype_get_value(axis2c_node_t *doctype_node);

void om_doctype_set_value(axis2c_node_t *doctype_node,const char *value);

#endif				//  AXIS2C_OM_DOCTYPE_H
