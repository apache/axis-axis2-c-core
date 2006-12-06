/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIOM_PI_H
#define AXIOM_PI_H

/**
 * @file axiom_processing_instruction.h
 * @brief represents a xml processing instruction also known as PI
 */

#include <axiom_node.h>
#include <axiom_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axiom_processing_instruction;
    struct axiom_processing_instruction_ops;
    
    
/**
 * @defgroup axiom_processing_instruction pocessing instruction
 * @ingroup axiom_om 
 * @{
 */


  /** 
    * @brief 
    * Encapsulator struct for ops of axiom_processing_instruction
    */
     typedef struct axiom_processing_instruction_ops
    {
        /**
         * Frees an instance of axiom_processing_instruction
         * @param om_pi processing instruction to be freed.
         * @param env Environment. MUST NOT be NULL, .
         * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        free)(struct axiom_processing_instruction * om_pi,
              const axis2_env_t *env);
        /**
         * set processing instruction data
         *@param om_pi
         *@param env        
         *@param value 
         */
         
        axis2_status_t (AXIS2_CALL *
        set_value)(struct axiom_processing_instruction *om_pi,
                   const axis2_env_t *env,
                   const axis2_char_t* value);
        /**
         * set processing instruction target
         *@param om_pi processing_instruction struct
         *@param env environment, MUST NOT be NULL.
         *@param target 
         *@return status of the op AXIS2_SUCCESS on success,
         *                      AXIS2_FAILURE on error.
         */
        
        axis2_status_t (AXIS2_CALL *
        set_target)(struct axiom_processing_instruction *om_pi,
                    const axis2_env_t *env,
                    const axis2_char_t* target);
        /**
         * get PI target
         * @param om_pi processing_instruction struct
         * @param env environment 
         * @return target text , NULL on error or if target is null
         */
        axis2_char_t* (AXIS2_CALL *
        get_target)(struct axiom_processing_instruction *om_pi,
                    const axis2_env_t *env);
        /**
         *  get data part of processing_instruction
         * @param om_pi processing instruction 
         * @param env environment , MUST NOT be NULL.
         * @return data text , NULL if there is no data,
         */
        axis2_char_t* (AXIS2_CALL *
        get_value)(struct axiom_processing_instruction *om_pi,
                   const axis2_env_t *env);                                                                                                                           
                           
        /**
         *  serialize function
         * @param om_pi processing_instruction struct
         * @param env environment, MUST NOT be NULL.
         * @param om_output om_output handler struct
         * @return status of the op, AXIS2_SUCCESS on success,
         *         AXIS2_FAILURE on error 
         */
        axis2_status_t (AXIS2_CALL *
        serialize)(struct axiom_processing_instruction *om_pi,
                  const axis2_env_t *env, 
                  axiom_output_t *om_output);
                                    
                                          
    } axiom_processing_instruction_ops_t;

  /** 
    * \brief processing instruction
    * Handles the XML processing instructions in AXIOM
    */
    typedef struct axiom_processing_instruction
    {
        /** ops struct  */
        axiom_processing_instruction_ops_t *ops;

    } axiom_processing_instruction_t;

  /**
    * Creates a processing instruction 
    * @param environment Environment. MUST NOT be NULL.
    * @param parent parent of the element node to be created. Optional, can be NULL.
    * @param target target of the processing instruction.cannot be NULL.
    * @param value value of the processing instruction.cannot be NULL.
    * @param node This is an out parameter. cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIOM_PROCESSING_INSTRUCTION
    * @return a pointer tonewly created processing instruction struct 
    */
    AXIS2_EXTERN axiom_processing_instruction_t * AXIS2_CALL 
        axiom_processing_instruction_create (const axis2_env_t *env,
                                                axiom_node_t * parent,
                                                const axis2_char_t * target,
                                                const axis2_char_t * value,
                                                axiom_node_t ** node);


/** frees given processing instruction */
#define AXIOM_PROCESSING_INSTRUCTION_FREE(pi, env) \
        ((pi)->ops->free(pi, env))
/** set data text of processing_instruction */        
#define AXIOM_PROCESSING_INSTRUCION_SET_VALUE(pi, env, value) \
        ((pi)->ops->set_value(pi,env,value))
/** get data text of processing_instruction */        
#define AXIOM_PROCESSING_INSTRUCTION_GET_VALUE(pi, env) \
        ((pi)->ops->get_value(pi, env))        
/** set target of processing instruction */        
#define AXIOM_PROCESSING_INSTRUCION_SET_TARGET(pi, env, value) \
        ((pi)->ops->set_target(pi, env, value))
/** get target text */        
#define AXIOM_PROCESSING_INSTRUCTION_GET_TARGET(pi, env) \
        ((pi)->ops->get_target(pi, env))
/** serialize */
#define AXIOM_PROCESSING_INSTRUCTION_SERIALIZE(pi, env, om_output) \
        ((pi)->ops->serialize(pi, env, om_output))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_PI_H */
