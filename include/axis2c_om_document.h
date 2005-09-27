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

#ifndef _AXISC_OM_DOCUMENT_H_
#define _AXISC_OM_DOCUMENT_H_
#include "axis2c_node.h"

struct om_document_s;
typedef struct om_document_s om_document_t;

struct om_document_s
{
	axis2c_node_t *root_element;
	axis2c_node_t *first_child;
	axis2c_node_t *last_child;
};

om_document_t *create_om_document();
void free_om_document(om_document_t *document);




#endif // _AXISC_OM_DOCUMENT_H_

