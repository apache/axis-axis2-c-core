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
 
 #ifndef AXIS2_SOAP_FAULT_DETAIL_H
 #define AXIS2_SOAP_FAULT_DETAIL_H
  
/**
 * @file axis2_soap_fault_detail.h
 * @brief axis2_soap_fault_detail struct
 */
 
#include <axis2_env.h>
#include <axis2_soap_fault.h>
#include <axis2_om_children_iterator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_fault_detail_ops 
                        axis2_soap_fault_detail_ops_t;

/**
 * @defgroup axis2_soap_fault_detail
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_fault_detail operations struct
 *   ops Encapsulator struct of axis2_soap_fault_detail
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_fault_detail_ops
    {
      /**
        * Free an axis2_soap_fault_detail
        * @param  fault_detail pointer to soap_fault_detail struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *free_fn)
                                (axis2_soap_fault_detail_t *fault_detail,
                                 axis2_env_t **env);
                                     
        axis2_status_t (AXIS2_CALL *add_detail_entry)
                                (axis2_soap_fault_detail_t *fault_detail,
                                 axis2_env_t **env,
                                 axis2_om_node_t *detail_node);
                                 
        axis2_om_children_iterator_t* (AXIS2_CALL *get_all_detail_entries)
                                (axis2_soap_fault_detail_t *fault_detail,
                                 axis2_env_t **env); 

        axis2_status_t (AXIS2_CALL *set_base_node)
                                (axis2_soap_fault_code_t *fault_code,
                                 axis2_env_t **env,
                                 axis2_om_node_t *node);
    
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                                (axis2_soap_fault_code_t *fault_code,
                                 axis2_env_t **env);
                                                                                                                
    };      

  /**
    * \brief soap_fault_code struct
    * represent a soap_fault_code
    */
    struct axis2_soap_fault_detail
    {
        /** operation of axis2_soap_fault_detail struct */
        axis2_soap_fault_detail_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_fault_detail_t *)
axis2_soap_fault_detail_create_with_parent(axis2_env_t **env,
                                             axis2_soap_fault_t *fault);
                            

AXIS2_DECLARE(axis2_soap_fault_detail_t *)
axis2_soap_fault_detail_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_soap_fault_detail_t *)
axis2_soap_fault_detail_create_with_parent_extract
                (axis2_env_t **env,
                 axis2_soap_fault_t *fault,
                 axis2_bool_t *extract_namespace_from_parent);

/******************** Macros **************************************************/
    
    
/** free soap_fault_detail */
#define AXIS2_SOAP_FAULT_DETAIL_FREE(fault_detail , env) \
        ((fault_detail)->ops->free_fn(fault_detail, env))

#define AXIS2_SOAP_FAULT_DETAIL_ADD_DETAIL_ENTRY(fault_detail, env, detail) \
        ((fault_detail)->ops->add_detail_entry(fault_detail, env, detail))

#define AXIS2_SOAP_FAULT_DETAIL_GET_ALL_DETAIL_ENTRIES(fault_detail, env) \
        ((fault_detail)->ops->get_all_detail_entries(fault_detail, env))                 
        
#define AXIS2_SOAP_FAULT_DETAIL_GET_BASE_NODE(fault_detail, env) \
        ((fault_detail)->ops->get_base_node(fault_detail, env))         

#define AXIS2_SOAP_FAULT_DETAIL_SET_BASE(fault_detail, env, node) \
        ((fault_detail)->ops->set_base_node(fault_detail, env, node))  
/** @} */

#ifdef __cplusplus
}
#endif
 
 #endif /* AXIS2_SOAP_FAULT_DETAIL_H */