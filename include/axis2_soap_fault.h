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
 
 #ifndef AXIS2_SOAP_FAULT_H
 #define AXIS2_SOAP_FAULT_H
 
 
   /**
    * @file axis2_soap_fault.h
    * @brief axis2_soap_fault struct
    */
#include <axis2_soap_envelope.h>
#include <axis2_soap_body.h>
#include <axis2_soap.h> 
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_soap_fault_reason    axis2_soap_fault_reason_t;
typedef struct axis2_soap_fault_detail    axis2_soap_fault_detail_t;
typedef struct axis2_soap_fault_sub_code  axis2_soap_fault_sub_code_t;
typedef struct axis2_soap_fault_code      axis2_soap_fault_code_t;
typedef struct axis2_soap_fault_node      axis2_soap_fault_node_t;
typedef struct axis2_soap_fault_role      axis2_soap_fault_role_t;
typedef struct axis2_soap_fault_text      axis2_soap_fault_text_t;
typedef struct axis2_soap_fault_value     axis2_soap_fault_value_t;


typedef struct axis2_soap_fault_ops axis2_soap_fault_ops_t;
    
    typedef enum axis2_soap_fault_types
    {
        AXIS2_SOAP_TYPE_NONE = 0,
        AXIS2_SOAP_FAULT,
        AXIS2_SOAP_FAULT_DETAIL,
        AXIS2_SOAP_FAULT_SUB_CODE,
        AXIS2_SOAP_FAULT_CODE,
        AXIS2_SOAP_FAULT_NODE,
        AXIS2_SOAP_FAULT_ROLE,
        AXIS2_SOAP_FAULT_TEXT,
        AXIS2_SOAP_FAULT_VALUE
    };
    
    
/**
 * @defgroup axis2_soap_fault
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_fault operations struct
 *   ops Encapsulator struct of axis2_soap fault
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_fault_ops
    {
      /**
        * Free an axis2_soap_fault
        * @param  fault pointer to soap_fault struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *free_fn)(axis2_soap_fault_t *fault,
                                             axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *set_code)(axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_soap_fault_code_t *code);
                                              
        axis2_soap_fault_code_t * (AXIS2_CALL *get_code)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);
                                         
        axis2_status_t (AXIS2_CALL *set_reason)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_soap_fault_reason_t *reason);
                                              
        axis2_soap_fault_reason_t* (AXIS2_CALL *get_reason)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);
                                             
        axis2_status_t (AXIS2_CALL *set_node)(axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_soap_fault_node_t *node);
                                              
        axis2_soap_fault_node_t* (AXIS2_CALL *get_node)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);
                                             
        axis2_status_t (AXIS2_CALL *set_role)(axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_soap_fault_role_t *role);
                                              
        axis2_soap_fault_role_t* (AXIS2_CALL *get_role)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);

        axis2_status_t (AXIS2_CALL *set_detail)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_soap_fault_detail_t *detail);
                                        
        axis2_soap_fault_detail_t* (AXIS2_CALL *get_detail)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);  
                                              
        axis2_char_t * (AXIS2_CALL *get_exception)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);
                                              
        axis2_status_t (AXIS2_CALL *set_exception)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_char_t *exception);                                                                                             
        
       /**
        * This is only intended to be used by the builder,
        * do not use this function in other places
        */
        axis2_status_t (AXIS2_CALL *set_base_node)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              axis2_om_node_t *node);
                                              
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env);  
                                                     
        axis2_status_t (AXIS2_CALL *set_soap_version)
                                             (axis2_soap_fault_t *fault,
                                              axis2_env_t **env,
                                              int soap_version);
                                           
        int (AXIS2_CALL *get_soap_version)(axis2_soap_fault_t *fault,
                                           axis2_env_t **env);                                                                                                                                                                                                                                                           
                                         
    };

  /**
    * \brief soap_fault struct
    * represent a soap_fault
    */
    struct axis2_soap_fault
    {
        /** operation of axis2_soap_fault struct */
        axis2_soap_fault_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_fault_t *)
axis2_soap_fault_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_soap_fault_t *)
axis2_soap_fault_create_with_parent(axis2_env_t **env,
                                    axis2_soap_body_t *parent);

AXIS2_DECLARE(axis2_soap_fault_t *)
axis2_soap_fault_create_with_exception(axis2_env_t **env,
                                        axis2_soap_body_t *parent,  
                                        axis2_char_t* exception);
/******************** Macros **************************************************/
    
    
/** free soap_fault */
#define AXIS2_SOAP_FAULT_FREE(fault , env) \
        ((fault)->ops->free_fn(fault, env))

#define AXIS2_SOAP_FAULT_SET_CODE(fault , env, code) \
        ((fault)->ops->set_code(fault, env, code))

#define AXIS2_SOAP_FAULT_SET_REASON(fault , env, reason) \
        ((fault)->ops->set_reason(fault, env, reason))

#define AXIS2_SOAP_FAULT_SET_DETAIL(fault , env, detail) \
        ((fault)->ops->set_detail(fault, env, detail))

#define AXIS2_SOAP_FAULT_SET_ROLE(fault , env, role) \
        ((fault)->ops->set_role(fault, env, role))
        
#define AXIS2_SOAP_FAULT_SET_NODE(fault , env, node) \
        ((fault)->ops->set_node(fault, env, node)) 
        
#define AXIS2_SOAP_FAULT_SET_BASE_NODE(fault, env, node) \
        ((fault)->ops->set_base_node(fault, env, node))
        
#define AXIS2_SOAP_FAULT_GET_CODE(fault , env) \
        ((fault)->ops->get_code(fault, env))

#define AXIS2_SOAP_FAULT_GET_REASON(fault , env) \
        ((fault)->ops->get_reason(fault, env))

#define AXIS2_SOAP_FAULT_GET_DETAIL(fault , env) \
        ((fault)->ops->get_detail(fault, env))

#define AXIS2_SOAP_FAULT_GET_ROLE(fault , env) \
        ((fault)->ops->get_role(fault, env))
        
#define AXIS2_SOAP_FAULT_GET_NODE(fault , env) \
        ((fault)->ops->get_node(fault, env)) 
        
#define AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env) \
        ((fault)->ops->get_base_node(fault, env)) 
        
#define AXIS2_SOAP_FAULT_GET_SOAP_VERSION(fault, env) \
        ((fault)->ops->get_soap_version(fault, env))
        
#define AXIS2_SOAP_FAULT_SET_SOAP_VERSION(fault, env) \
        ((fault)->ops->set_soap_version(fault, env))
        
#define AXIS2_SOAP_FAULT_GET_EXCEPTION(fault, env) \
        ((fault)->ops->get_exception(fault, env))
        
#define AXIS2_SOAP_FAULT_SET_EXCEPTION(fault, env, exception) \
        ((fault)->ops->set_exception(fault, env, exception))        
                                              
/** @} */
#ifdef __cplusplus
}
#endif

 
#endif /* AXIS2_SOAP_FAULT_H */

