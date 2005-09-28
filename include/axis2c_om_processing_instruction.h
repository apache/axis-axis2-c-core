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

#ifndef AXIS2C_OM_PI_H
#define AXIS2C_OM_PI_H
#include <axis2c_node.h>

typedef struct axis2c_om_processing_instruction_t {
    char *target;
    char *value;
} axis2c_om_processing_instruction_t;

axis2c_node_t
    *axis2c_create_om_processing_instruction_with_parent(axis2c_node_t *
							 parent,
							 const char
							 *target,
							 const char
							 *value);

axis2c_node_t *axis2c_create_om_processing_instruction(axis2c_node_t *
						       parent);

void axis2c_om_processing_instruction_serialize_with_cache(FILE *
							   output_stream);

void
axis2c_om_processing_instruction_discard(axis2c_om_processing_instruction_t
					 * om_pi);






#endif				// AXIS2C_OM_PI_H
