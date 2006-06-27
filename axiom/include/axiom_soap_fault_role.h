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
 
 #ifndef AXIOM_SOAP_FAULT_ROLE_H
 #define AXIOM_SOAP_FAULT_ROLE_H
 
 
   /**
    * @file axiom_soap_fault_role.h
    * @brief axiom_soap_fault_role 
    */
#include <axis2_env.h>
#include <axiom_soap_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault_role_ops axiom_soap_fault_role_ops_t;
    typedef struct axiom_soap_fault_role axiom_soap_fault_role_t;
/**
 * @defgroup axiom_soap_fault_role soap fault role
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief soap_fault_role operations struct
 *   ops Encapsulator struct of axiom_soap_fault_role
 */
 AXIS2_DECLARE_DATA   struct axiom_soap_fault_role_ops
    {
      /**
        * Free an axiom_soap_fault_role
        * @param  fault_role pointer to soap_fault_role struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_soap_fault_role_t *fault_role,
                 const axis2_env_t *env);

        axis2_status_t (AXIS2_CALL *
      set_role_value)(axiom_soap_fault_role_t *fault_role,
                        const axis2_env_t *env,
                        axis2_char_t* uri);
                                     
        axis2_char_t* (AXIS2_CALL *
      get_role_value)(axiom_soap_fault_role_t *fault_role,
                        const axis2_env_t *env);
                                     
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axiom_soap_fault_role_t *fault_role,
                       const axis2_env_t *env);
                                                                                                                   
    };      

  /**
    * \brief soap_fault_role struct
    * represent a soap_fault_role
    */
    struct axiom_soap_fault_role
    {
        /** operation of axiom_soap_fault_role struct */
        axiom_soap_fault_role_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
    
AXIS2_EXTERN axiom_soap_fault_role_t * AXIS2_CALL
axiom_soap_fault_role_create_with_parent(const axis2_env_t *env,
                            axiom_soap_fault_t *fault);
                            
/******************** Macros **************************************************/
    
    
/** free soap_fault_role */
#define AXIOM_SOAP_FAULT_ROLE_FREE(fault_role , env) \
        ((fault_role)->ops->free_fn(fault_role, env))

#define AXIOM_SOAP_FAULT_ROLE_GET_VALUE(fault_role , env) \
        ((fault_role)->ops->get_role_value(fault_role, env)) 
        
#define AXIOM_SOAP_FAULT_ROLE_GET_BASE_NODE(fault_role, env) \
        ((fault_role)->ops->get_base_node(fault_role, env))         

#define AXIOM_SOAP_FAULT_ROLE_SET_VALUE(fault_role , env, value) \
        ((fault_role)->ops->set_role_value(fault_role, env, value))
/** @} */

#ifdef __cplusplus
}
#endif
 
#endif /* AXIOM_SOAP_FAULT_ROLE_H */
