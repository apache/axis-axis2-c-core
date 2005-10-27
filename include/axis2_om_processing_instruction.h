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
extern "C"
{
#endif

    struct axis2_om_processing_instruction;
    struct axis2_om_processing_instruction_ops;
    
    
/**
 * @defgroup axis2_om_processing_instruction OM Processing Instruction
 * @ingroup axis2_om 
 * @{
 */


  /** 
    * @brief OM text operations struct
    * Encapsulator struct for operations of axis2_om_processing_instruction
    */
   AXIS2_DECLARE_DATA  typedef struct axis2_om_processing_instruction_ops
    {
        /**
         * Frees an instance of axis2_om_processing_instruction
         * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
         * @param om_pi processing instruction to be freed.
         * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *axis2_om_processing_instruction_ops_free)
            (axis2_environment_t * environment,
             struct axis2_om_processing_instruction * om_pi);
    } axis2_om_processing_instruction_ops_t;

  /** 
    * \brief OM processing instruction
    * Handles the XML processing instructions in OM
    */
    typedef struct axis2_om_processing_instruction
    {
        /** operations struct  */
        axis2_om_processing_instruction_ops_t *ops;
        /** processing instruction  target */
        axis2_char_t *target;
        /** processing instruction  value */
        axis2_char_t *value;
    } axis2_om_processing_instruction_t;

  /**
    * Creates a processing instruction 
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param parent parent of the element node to be created. Optional, can be NULL.
    * @param target target of the processing instruction. Mandatory, cannot be NULL.
    * @param value value of the processing instruction. Mandatory, cannot be NULL.
    * @param node This is an out parameter. Mandatory, cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIS2_OM_ELEMENT
    * @return a pointer tonewly created processing instruction struct 
    */
    AXIS2_DECLARE(axis2_om_processing_instruction_t *) 
        axis2_om_processing_instruction_create (axis2_environment_t *
                                                 environment,
                                                 axis2_om_node_t * parent,
                                                 const axis2_char_t * target,
                                                 const axis2_char_t * value,
                                                 axis2_om_node_t ** node);

/** frees given processing instruction */
#define axis2_om_processing_instruction_free(environment,pi) ((pi)->ops->axis2_om_processing_instruction_impl_free(environment,pi))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_PI_H */
