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
 
 #ifndef AXIS2_SOAP_FAULT_ROLE_H
 #define AXIS2_SOAP_FAULT_ROLE_H
 
 
   /**
    * @file axis2_soap_fault_role.h
    * @brief axis2_soap_fault_role 
    */
#include <axis2_env.h>
#include <axis2_soap_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_fault_role_ops axis2_soap_fault_role_ops_t;

/**
 * @defgroup axis2_soap_fault_role
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_fault_role operations struct
 *   ops Encapsulator struct of axis2_soap_fault_role
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_fault_role_ops
    {
      /**
        * Free an axis2_soap_fault_role
        * @param  fault_role pointer to soap_fault_role struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *free_fn)
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env);

        axis2_status_t (AXIS2_CALL *set_role_value)
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env,
                                     axis2_char_t* uri);
                                     
        axis2_char_t* (AXIS2_CALL *get_role_value)                                         
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env);
                                     
        axis2_status_t (AXIS2_CALL *set_base_node)
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env,
                                     axis2_om_node_t *node);
    
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env);
                                 
        int (AXIS2_CALL *get_soap_version)
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env);
                                     
        int (AXIS2_CALL *set_soap_version)
                                    (axis2_soap_fault_role_t *fault_role,
                                     axis2_env_t **env,
                                     int soap_version);                                                                      
                                                                                                                
    };      

  /**
    * \brief soap_fault_role struct
    * represent a soap_fault_role
    */
    struct axis2_soap_fault_role
    {
        /** operation of axis2_soap_fault_role struct */
        axis2_soap_fault_role_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_fault_role_t *)
axis2_soap_fault_role_create(axis2_env_t **env);
    
AXIS2_DECLARE(axis2_soap_fault_role_t *)
axis2_soap_fault_role_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault,
                            axis2_bool_t extract_ns_from_parent);
                            

AXIS2_DECLARE(axis2_soap_fault_role_t *)
axis2_soap11_fault_role_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault);

AXIS2_DECLARE(axis2_soap_fault_role_t *)
axis2_soap12_fault_role_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault);

/******************** Macros **************************************************/
    
    
/** free soap_fault_role */
#define AXIS2_SOAP_FAULT_ROLE_FREE(fault_role , env) \
        ((fault_role)->ops->free_fn(fault_role, env))

#define AXIS2_SOAP_FAULT_ROLE_SET_VALUE(fault_role , env, value) \
        ((fault_role)->ops->set_role_value(fault_role, env, value))
        
#define AXIS2_SOAP_FAULT_ROLE_GET_VALUE(fault_role , env) \
        ((fault_role)->ops->get_role_value(fault_role, env)) 
        
#define AXIS2_SOAP_FAULT_ROLE_GET_BASE_NODE(fault_role, env) \
        ((fault_role)->ops->get_base_node(fault_role, env))         

#define AXIS2_SOAP_FAULT_ROLE_SET_BASE_NODE(fault_role, env, node) \
        ((fault_role)->ops->set_base_node(fault_role, env, node)) 

#define AXIS2_SOAP_FAULT_ROLE_GET_SOAP_VRESION(fault_role, env) \
        ((fault_role)->ops->get_soap_version(fault_role, env)) 

#define AXIS2_SOAP_FAULT_ROLE_SET_SOAP_VRESION(fault_role, env, version) \
        ((fault_role)->ops->set_soap_version(fault_role, env, version))        
                
/** @} */

#ifdef __cplusplus
}
#endif
 
 #endif /* AXIS2_SOAP_FAULT_ROLE_H */