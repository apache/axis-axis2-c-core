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

#ifndef _AXISC_OM_DOCTYPE_H_
#define _AXISC_OM_DOCTYPE_H_
#include <axis2c_node.h>


struct om_doctype_s;
typedef struct om_doctype_s om_doctype_t;

struct om_doctype_s {
    char *value;
};

axis2c_node_t *create_om_doctype(const char *value);
char *om_text_get_text(node_t * text_node);

#endif				//  _AXISC_OM_DOCTYPE_H_
