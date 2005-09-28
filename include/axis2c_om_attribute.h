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

#ifndef AXIS2C_OM_ATTRIBUTE_H
#define AXIS2C_OM_ATTRIBUTE_H

#include <axis2c_qname.h>
#include <axis2c_node.h>
#include <axis2c_om_namespace.h>

typedef struct axis2c_om_attribute_t{
	char *localname;
	char *value;
	axis2c_om_namespace_t *ns;
}axis2c_om_attribute_t;

axis2c_om_attribute_t *axis2c_create_om_attribute(const char *localname,const char *value, axis2c_om_namespace_t *ns);
axis2c_qname_t *axis2c_om_attribute_get_qname(axis2c_om_attribute_t *attribute);
void axis2c_om_attribute_free(axis2c_om_attribute_t *attr);






#endif   // _AXISC_ATTRIBUTE_H_

