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
 
 #ifndef AXIS2_SOAP_FAULT_VALUE_H
 #define AXIS2_SOAP_FAULT_VALUE_H
 
    /**
    * @file axis2_soap_fault_value.h
    * @brief axis2_soap_fault_value 
    */
#include <axis2_env.h>
#include <axis2_soap_fault.h>
#include <axis2_soap_fault_sub_code.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_soap_fault_value_ops axis2_soap_fault_value_ops_t;
typedef struct axis2_soap_fault_value axis2_soap_fault_value_t;
/**
 * @defgroup axis2_soap_fault_value
 * @ingroup axis2_soap_fault_
 * @{
 */

/**
 *   \brief soap_fault_value operations struct
 *   ops Encapsulator struct of axis2_soap_fault_value
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_fault_value_ops
    {
      /**
        * Free an axis2_soap_fault_value
        * @param  fault_value pointer to soap_fault_value struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *free_fn)
                            (axis2_soap_fault_value_t *fault_value,
                             axis2_env_t **env);

        axis2_status_t (AXIS2_CALL *set_base_node)
                             (axis2_soap_fault_value_t *fault_value,
                              axis2_env_t **env,
                              axis2_om_node_t *node);
    
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                             (axis2_soap_fault_value_t *fault_value,
                              axis2_env_t **env);
                              
    };      

  /**
    * \brief soap_fault_value struct
    * represent a soap_fault_value
    */
    struct axis2_soap_fault_value
    {
        /** operation of axis2_soap_fault_value struct */
        axis2_soap_fault_value_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_fault_value_t *)
axis2_soap_fault_value_create_with_subcode(axis2_env_t **env,
                            axis2_soap_fault_sub_code_t *parent);

AXIS2_DECLARE(axis2_soap_fault_value_t *)
axis2_soap_fault_value_create_with_code(axis2_env_t **env,
                            axis2_soap_fault_code_t *parent);  

AXIS2_DECLARE(axis2_soap_fault_value_t *)
axis2_soap_fault_value_create(axis2_env_t **env);

                             

/******************** Macros **************************************************/
    
    
/** free soap_fault_value */
#define AXIS2_SOAP_FAULT_VALUE_FREE(fault_value , env) \
        ((fault_value)->ops->free_fn(fault_value, env))
        
#define AXIS2_SOAP_FAULT_VALUE_GET_BASE_NODE(fault_value, env) \
        ((fault_value)->ops->get_base_node(fault_value, env))         

#define AXIS2_SOAP_FAULT_VALUE_SET_BASE_NODE(fault_value, env, node) \
        ((fault_value)->ops->set_base_node(fault_value, env, node)) 
        
/** @} */

#ifdef __cplusplus
}
#endif 
 
#endif /* AXIS2_SOAP_FAULT_VALUE_H */
