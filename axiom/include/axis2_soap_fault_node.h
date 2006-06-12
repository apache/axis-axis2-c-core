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
 
 #ifndef AXIS2_SOAP_FAULT_NODE_H
 #define AXIS2_SOAP_FAULT_NODE_H
 
   /**
    * @file axis2_soap_fault_node.h
    * @brief axis2_soap_fault_node struct
    */
#include <axis2_env.h>
#include <axis2_soap_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_fault_node_ops axis2_soap_fault_node_ops_t;
    typedef struct axis2_soap_fault_node axis2_soap_fault_node_t;
/**
 * @defgroup axis2_soap_fault_node
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_fault_node operations struct
 *   ops Encapsulator struct of axis2_soap_fault_node
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_fault_node_ops
    {
      /**
        * Free an axis2_soap_fault_node
        * @param  fault_node pointer to soap_fault_node struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
      free_fn) (axis2_soap_fault_node_t *fault_node,
                  const axis2_env_t *env);

        axis2_status_t (AXIS2_CALL *
      set_value)(axis2_soap_fault_node_t *fault_node,
                   const axis2_env_t *env,
                   axis2_char_t *fault_val);
                                     
        axis2_char_t* (AXIS2_CALL *
      get_value)(axis2_soap_fault_node_t *fault_node,
                   const axis2_env_t *env);
    
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axis2_soap_fault_node_t *fault_node,
                       const axis2_env_t *env);
    };      

  /**
    * \brief soap_fault_node struct
    * represent a soap_fault_node
    */
    struct axis2_soap_fault_node
    {
        /** operation of axis2_soap_fault_node struct */
        axis2_soap_fault_node_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_EXTERN axis2_soap_fault_node_t * AXIS2_CALL
axis2_soap_fault_node_create_with_parent(const axis2_env_t *env,
                                         axis2_soap_fault_t *fault);
                            

/******************** Macros **************************************************/
    
    
/** free soap_fault_node */
#define AXIS2_SOAP_FAULT_NODE_FREE(fault_node , env) \
        ((fault_node)->ops->free_fn(fault_node, env))

#define AXIS2_SOAP_FAULT_NODE_GET_VALUE(fault_node , env) \
        ((fault_node)->ops->get_value(fault_node, env)) 
        
#define AXIS2_SOAP_FAULT_NODE_GET_BASE_NODE(fault_node, env) \
        ((fault_node)->ops->get_base_node(fault_node, env))         

#define AXIS2_SOAP_FAULT_NODE_SET_VALUE(fault_node , env, uri) \
        ((fault_node)->ops->set_value(fault_node, env, uri))        
        
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_FAULT_NODE_H */
