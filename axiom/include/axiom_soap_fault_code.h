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
 
 #ifndef AXIOM_SOAP_FAULT_CODE_H
 #define AXIOM_SOAP_FAULT_CODE_H
 
 
   /**
    * @file axiom_soap_fault_code.h
    * @brief axiom_soap_fault_code struct
    */
#include <axis2_env.h>
#include <axiom_soap_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault_code_ops axiom_soap_fault_code_ops_t;
    typedef struct axiom_soap_fault_code axiom_soap_fault_code_t;
    
    struct axiom_soap_fault_value;
    struct axiom_soap_fault_sub_code;
    struct axiom_soap_builder;
/**
 * @defgroup axiom_soap_fault_code soap fault code
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief soap_fault_code operations struct
 *   ops Encapsulator struct of axiom_soap_fault_code
 */
 AXIS2_DECLARE_DATA   struct axiom_soap_fault_code_ops
    {
      /**
        * Free an axiom_soap_fault_code
        * @param  fault_code pointer to soap_fault_code struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_soap_fault_code_t *fault_code,
                 const axis2_env_t *env);
      /**
      * @returns axiom_soap_fault_sub_code struct if one is associated with
      * this fault_code struct , otherwise teturns NULL
      */
        
        struct axiom_soap_fault_sub_code* (AXIS2_CALL *
      get_sub_code)(axiom_soap_fault_code_t *fault_code,
                      const axis2_env_t *env);
                                     
      /**
        * @returns soap_fault_value if available
      */      
        struct axiom_soap_fault_value* (AXIS2_CALL *get_value)                                         
                                    (axiom_soap_fault_code_t *fault_code,
                                     const axis2_env_t *env);
                                     
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axiom_soap_fault_code_t *fault_code,
                       const axis2_env_t *env);
                                 
};      

  /**
    * \brief soap_fault_code struct
    * represent a soap_fault_code
    */
    struct axiom_soap_fault_code
    {
        /** operation of axiom_soap_fault_code struct */
        axiom_soap_fault_code_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
    
AXIS2_EXTERN axiom_soap_fault_code_t * AXIS2_CALL
axiom_soap_fault_code_create_with_parent(const axis2_env_t *env,
                            axiom_soap_fault_t *fault);
                            
AXIS2_EXTERN axiom_soap_fault_code_t * AXIS2_CALL
axiom_soap_fault_code_create_with_parent_value(const axis2_env_t *env,
                            axiom_soap_fault_t *fault,
                            axis2_char_t *value);
                            
/******************** Macros **************************************************/
    
    
/** free soap_fault_code */
#define AXIOM_SOAP_FAULT_CODE_FREE(fault_code , env) \
        ((fault_code)->ops->free_fn(fault_code, env))

#define AXIOM_SOAP_FAULT_CODE_GET_SUB_CODE(fault_code , env) \
        ((fault_code)->ops->get_sub_code(fault_code, env))        

#define AXIOM_SOAP_FAULT_CODE_GET_VALUE(fault_code , env) \
        ((fault_code)->ops->get_value(fault_code, env)) 
        
#define AXIOM_SOAP_FAULT_CODE_GET_BASE_NODE(fault_code, env) \
        ((fault_code)->ops->get_base_node(fault_code, env))         

/** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIOM_SOAP_FAULT_CODE_H */
