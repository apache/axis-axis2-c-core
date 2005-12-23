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
 
 
 #ifndef AXIS2_SOAP_BODY_H
 #define AXIS2_SOAP_BODY_H
 
    /**
    * @file axis2_soap_body.h
    * @brief axis2_soap_body struct
    */
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_namespace.h>    
#include <axis2_soap_envelope.h>    



#ifdef __cplusplus
extern "C"
{
#endif


typedef struct axis2_soap_fault axis2_soap_fault_t;
typedef struct axis2_soap_body_ops axis2_soap_body_ops_t;
    
/**
 * @defgroup axis2_soap_body
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_body operations struct
 *   ops Encapsulator struct of axis2_soap body
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_body_ops
    {
      /**
        * Free an axis2_soap_body
        * @param  body pointer to soap_body struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *free_fn)(axis2_soap_body_t *body,
                                             axis2_env_t **env);
        
        axis2_soap_fault_t* (AXIS2_CALL *add_fault_with_error)
                                            (axis2_soap_body_t *body,
                                             axis2_env_t **env,
                                             axis2_char_t *error);
        
        axis2_bool_t (AXIS2_CALL *has_fault)(axis2_soap_body_t *body,
                                             axis2_env_t **env);
                                              
        axis2_soap_fault_t* (AXIS2_CALL *get_fault)
                                            (axis2_soap_body_t *body,
                                             axis2_env_t **env);
                                                      
        axis2_status_t (AXIS2_CALL *add_fault_with_soap_fault)
                                            (axis2_soap_body_t *body,
                                             axis2_env_t **env,
                                             axis2_soap_fault_t *fault);
                                             
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                                            (axis2_soap_body_t *body,
                                             axis2_env_t **env);
                                             
        axis2_om_node_t* (AXIS2_CALL *set_base_node)
                                            (axis2_soap_body_t *body,
                                             axis2_env_t **env,
                                             axis2_om_node_t *node);
                                                                                          
                                                       
    };                                                      

  /**
    * \brief soap_body struct
    * represent a soap_body
    */
    struct axis2_soap_body
    {
        /** operation of axis2_soap_body struct */
        axis2_soap_body_ops_t *ops;
       
    };

  /**
    * creates a soap body struct 
    * @param env Environment. MUST NOT be NULL
    */
    AXIS2_DECLARE(axis2_soap_body_t *)
    axis2_soap_body_create (axis2_env_t **env);

/******************** Macros **************************************************/
    
    
/** free soap_body */
#define AXIS2_SOAP_BODY_FREE(body , env) \
        ((body)->ops->free_fn(body, env))
        
#define AXIS2_SOAP_BODY_ADD_FAULT_WITH_ERROR(boby, env, error) \
        ((body)->ops->add_fault_with_error(body, env, error))
        
#define AXIS2_SOAP_BODY_HAS_FAULT(body, env) \
        ((body)->ops->has_fault(body, env))
        
#define AXIS2_SOAP_BODY_GET_FAULT(body, env) \
        ((body)->ops->get_fault(body, env))
        
#define AXIS2_SOAP_BODY_ADD_SOAP_FAULT_WITH_SOAP_FAULT(body, env, fault) \
        ((body)->ops->add_fault_with_soap_fault(body, env, fault))
        
#define AXIS2_SOAP_BODY_GET_BASE_NODE(body, env) \
        ((body)->ops->get_base_node(body, env))
        
#define AXIS2_SOAP_BODY_SET_BASE_NODE(body, env, node) \
        ((body)->ops->set_base_node(body, env, node))
                
/** @} */

#ifdef __cplusplus
}
#endif

 
 #endif /* AXIS2_SOAP_BODY_H */