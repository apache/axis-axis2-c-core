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

#ifndef AXIS2C_OM_TEXT_H
#define AXIS2C_OM_TEXT_H
#include <axis2c_om_attribute.h>

typedef struct axis2c_om_text_t
{
	char *value;
	// mtom stuff should go here
	char *mime_type;
	int optimize;
	int is_binary;
	char *content_id;
	axis2c_om_attribute_t *attribute;

};

node_t *axis2c_create_om_text(const char *value);

char* axis2c_om_text_get_text(om_text_t *textnode);
node_t *axis2c_create_om_text_with_parent(axis2c_node_t *parent,const char *value);







#endif // AXIS2C_OM_TEXT_H


