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

#ifndef _AXISC_OM_COMMENT_H_
#define _AXISC_OM_COMMENT_H_
#include "node.h"


struct om_comment_s;
typedef struct om_comment_s om_comment_t;
struct om_comment_s
{
	char *value;
};

node_t *create_om_comment(const char *value);
void om_comment_free(node_t *comment_node);

#endif // _AXISC_OM_COMMENT_H_






