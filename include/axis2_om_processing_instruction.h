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

#ifndef AXIS2_OM_PI_H
#define AXIS2_OM_PI_H

/**
 * @file axis2_om_processing_instruction.h
 * @brief represents a xml processing instruction also known as PI
 */

#include <axis2_node.h>

typedef struct axis2_om_processing_instruction_t {
    char *target;
    char *value;
} axis2_om_processing_instruction_t;

/**
 *  creates axis2_om_processing_instruction and 
 *	@return a pointer to axis2_node_t struct containing PI
 */ 

axis2_node_t *axis2_om_processing_instruction_create(axis2_node_t *parent
					,const char *target,const char *value);
/**
 *  creates empty axis2_om_processing_instruction and 
 *	@return a pointer to axis2_node_t struct containing PI
 */

axis2_node_t *axis2_create_om_processing_instruction_create_empty(axis2_node_t *parent);



void axis2_om_processing_instruction_serialize_with_cache(FILE *output_stream);
/**
 *
 *
 */

void axis2_om_processing_instruction_discard(axis2_om_processing_instruction_t *om_pi);

#endif				// AXIS2_OM_PI_H
