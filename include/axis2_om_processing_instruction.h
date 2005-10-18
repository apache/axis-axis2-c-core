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

#include <axis2_om_node.h>

struct axis2_om_processing_instruction;
struct axis2_om_processing_instruction_ops;

typedef struct axis2_om_processing_instruction_ops
{
	axis2_status_t (*axis2_om_processing_instruction_ops_free)(axis2_environment_t *environment, struct axis2_om_processing_instruction *om_pi);
} axis2_om_processing_instruction_ops_t;

typedef struct axis2_om_processing_instruction {
	axis2_om_processing_instruction_ops_t *ops;
    axis2_char_t *target;
    axis2_char_t *value;
} axis2_om_processing_instruction_t;

/**
 *  creates axis2_om_processing_instruction and 
 *	@return a pointer to axis2_om_node_t struct containing PI
 */ 

axis2_om_processing_instruction_t *axis2_om_processing_instruction_create(axis2_environment_t *environment, axis2_om_node_t *parent,const axis2_char_t *target,const axis2_char_t *value,axis2_om_node_t **node);

#endif				/* AXIS2_OM_PI_H */
