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

#ifndef AXIS2C_OM_DOCUMENT_H
#define AXIS2C_OM_DOCUMENT_H
#include <axis2c_node.h>


typedef struct axis2c_om_document_t
{
	axis2c_node_t *root_element;
	axis2c_node_t *first_child;
	axis2c_node_t *last_child;
};

axis2c_om_document_t *axis2c_create_om_document();
void axis2c_free_om_document(axis2c_om_document_t *document);




#endif // AXIS2C_OM_DOCUMENT_H

