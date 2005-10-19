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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup axis2_om_text OM Text
 * @ingroup axis2_om 
 * @{
 */

/** @cond */
struct axis2_om_processing_instruction;
struct axis2_om_processing_instruction_ops;
/** @endcond */
    
/** @struct axis2_om_processing_instruction_ops
    @brief OM text operations struct

    Encapsulator struct for operations of axis2_om_processing_instruction
*/
typedef struct axis2_om_processing_instruction_ops
{
    /**
     * Free an instance of axis2_om_processing_instruction
     * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
     * @param om_pi 
     * @returns Status code
     */
	axis2_status_t (*axis2_om_processing_instruction_ops_free)(axis2_environment_t *environment, struct axis2_om_processing_instruction *om_pi);
} axis2_om_processing_instruction_ops_t;

/** \struct axis2_om_processing_instruction
    \brief OM processing_instruction

    Handles the XML processing instructions in OM
*/
typedef struct axis2_om_processing_instruction {
    /** operations struct  */
	axis2_om_processing_instruction_ops_t *ops;
    /* PI target */
    axis2_char_t *target;
    /** PI value */
    axis2_char_t *value;
} axis2_om_processing_instruction_t;

/**
 *  creates axis2_om_processing_instruction 
 * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
 * @param target 
 * @param value
 * @parent parent node
 * @param node This is an outparameter , the created struct will be set as node's data_element
 *	@return a pointer to axis2_om_processing_instruction_t struct 
 */ 

axis2_om_processing_instruction_t *axis2_om_processing_instruction_create(axis2_environment_t *environment, axis2_om_node_t *parent,const axis2_char_t *target,const axis2_char_t *value,axis2_om_node_t **node);

#define axis2_om_processing_instruction_free(environment,pi) ((pi)->ops->axis2_om_processing_instruction_impl_free(environment,pi))

/** @} */
#ifdef __cplusplus
}
#endif

#endif				/* AXIS2_OM_PI_H */
