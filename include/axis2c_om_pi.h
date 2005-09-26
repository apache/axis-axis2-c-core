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

#ifndef _AXISC_OM_PI_H_
#define _AXISC_OM_PI_H_
#include <axis2c_node.h>

struct om_pi_s;
typedef struct om_pi_s om_pi_t;
struct om_pi_s{
	char *target;
	char *value;
};

node_t *create_om_processing_instruction_with_parent(node_t *parent,const char *target,const char *value);

node_t *create_om_processing_instruction(node_t *parent);

void om_pi_serialize_with_cache(FILE *output_stream);

void om_pi_discard(om_pi_t *om_pi);






#endif // _AXISC_OM_PI_H_

