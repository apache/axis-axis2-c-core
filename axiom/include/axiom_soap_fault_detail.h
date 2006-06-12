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
 
 #ifndef AXIOM_SOAP_FAULT_DETAIL_H
 #define AXIOM_SOAP_FAULT_DETAIL_H
  
/**
 * @file axiom_soap_fault_detail.h
 * @brief axiom_soap_fault_detail struct
 */
 
#include <axis2_env.h>
#include <axiom_soap_fault.h>
#include <axiom_children_iterator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault_detail_ops axiom_soap_fault_detail_ops_t;
    typedef struct axiom_soap_fault_detail axiom_soap_fault_detail_t;

/**
 * @defgroup axiom_soap_fault_detail
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief soap_fault_detail operations struct
 *   ops Encapsulator struct of axiom_soap_fault_detail
 */
 AXIS2_DECLARE_DATA   struct axiom_soap_fault_detail_ops
    {
      /**
        * Free an axiom_soap_fault_detail
        * @param  fault_detail pointer to soap_fault_detail struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_soap_fault_detail_t *fault_detail,
                 const axis2_env_t *env);
                                     
        axis2_status_t (AXIS2_CALL *
      add_detail_entry)(axiom_soap_fault_detail_t *fault_detail,
                          const axis2_env_t *env,
                          axiom_node_t *ele_node);
                                 
        axiom_children_iterator_t* (AXIS2_CALL *
      get_all_detail_entries) (axiom_soap_fault_detail_t *fault_detail,
                                 const axis2_env_t *env); 

        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axiom_soap_fault_detail_t *fault_code,
                       const axis2_env_t *env);
        
    };      

  /**
    * \brief soap_fault_code struct
    * represent a soap_fault_code
    */
    struct axiom_soap_fault_detail
    {
        /** operation of axiom_soap_fault_detail struct */
        axiom_soap_fault_detail_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */



AXIS2_EXTERN axiom_soap_fault_detail_t * AXIS2_CALL
axiom_soap_fault_detail_create_with_parent
                        (const axis2_env_t *env,
                         axiom_soap_fault_t *fault);
                         
/******************** Macros **************************************************/
    
/** free soap_fault_detail */
#define AXIOM_SOAP_FAULT_DETAIL_FREE(fault_detail , env) \
        ((fault_detail)->ops->free_fn(fault_detail, env))

#define AXIOM_SOAP_FAULT_DETAIL_ADD_DETAIL_ENTRY(fault_detail, env, ele_node) \
        ((fault_detail)->ops->add_detail_entry(fault_detail, env, ele_node))

#define AXIOM_SOAP_FAULT_DETAIL_GET_ALL_DETAIL_ENTRIES(fault_detail, env) \
        ((fault_detail)->ops->get_all_detail_entries(fault_detail, env))                 
        
#define AXIOM_SOAP_FAULT_DETAIL_GET_BASE_NODE(fault_detail, env) \
        ((fault_detail)->ops->get_base_node(fault_detail, env))         


        
/** @} */

#ifdef __cplusplus
}
#endif
 
#endif /* AXIOM_SOAP_FAULT_DETAIL_H */
