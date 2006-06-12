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
#include <axis2_soap_const.h> 
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_soap_fault axis2_soap_fault_t;
typedef struct axis2_soap_fault_ops axis2_soap_fault_ops_t;
    
struct axis2_soap_fault_reason;
struct axis2_soap_fault_detail;
struct axis2_soap_fault_sub_code;
struct axis2_soap_fault_code;
struct axis2_soap_fault_node;
struct axis2_soap_fault_role;
struct axis2_soap_fault_text;
struct axis2_soap_fault_value;
struct axis2_soap_body;
struct axis2_soap_builder;
    
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
        * @return status of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
      free_fn)(axis2_soap_fault_t *fault,
                 const axis2_env_t *env);
      /**
        * this function returns a axis2_soap_fault_code struct
        * if a fault code is associated with this soap fault
        * only valid when called after building the soap fault
        * @param fault soap fault struct
        * @param env environment must not be NULL
        * @returns pointer to  soap_fault_code struct if one is associated 
        *  with this soap_fault struct , NULL is returned otherwise
        */      
        struct axis2_soap_fault_code * (AXIS2_CALL *
      get_code)(axis2_soap_fault_t *fault,
                  const axis2_env_t *env);
       /** 
        * @param fault soap fault struct
        * @param env environment must not be NULL
        * @returns pointer to soap_fault_reason struct if one is associated 
        * with this soap_fault struct , NULL is returned otherwise
        */                                   
                                              
        struct axis2_soap_fault_reason *(AXIS2_CALL *
      get_reason)(axis2_soap_fault_t *fault,
                    const axis2_env_t *env);
                                             
       /** 
        * @param fault soap fault struct
        * @param env environment must not be NULL
        * @returns pointer to soap_fault_node struct if one is associated 
        *  with this soap_fault struct , NULL is returned otherwise
        */                                        
        struct axis2_soap_fault_node* (AXIS2_CALL *
      get_node)(axis2_soap_fault_t *fault,
                  const axis2_env_t *env);
                                             
       /** 
        * @param fault soap fault struct
        * @param env environment must not be NULL
        * @returns pointer to soap_fault_code struct if one is associated 
        * with this soap_fault struct , NULL is returned otherwise
        */                                         
        struct axis2_soap_fault_role* (AXIS2_CALL *
      get_role)(axis2_soap_fault_t *fault,
                  const axis2_env_t *env);
      /** 
        * @param fault soap fault struct
        * @param env environment must not be NULL
        * @returns a pointer to  soap_fault_code struct if one is 
        * associated with this soap_fault struct , NULL is returned otherwise
        */   

        struct axis2_soap_fault_detail* (AXIS2_CALL *
      get_detail)(axis2_soap_fault_t *fault,
                    const axis2_env_t *env); 
      /** 
        * @param fault soap fault struct 
        * @param env enviroment must not be NULL
        * @returns a pointer to  soap_fault_code struct if one is 
        * associated with this soap_fault struct , NULL is returned otherwise
        */   
        axis2_char_t * (AXIS2_CALL *
      get_exception)(axis2_soap_fault_t *fault,
                       const axis2_env_t *env);
      /**
        * set an error string 
        * @param fualt soap fault struct
        * @param env enviroment must not be NULL
        * @param exception error message to be stored on soap fault
        */      
        axis2_status_t (AXIS2_CALL *
      set_exception) (axis2_soap_fault_t *fault,
                        const axis2_env_t *env,
                        axis2_char_t *exception);                                                                                             
     /**
       * returns the axiom_node_t struct which is wrapped by
       * this soap fault struct
       * @param fault soap fault struct
       * @param env environment must not be NULL
       * @returns a pointer to  axiom_node_t struct if an om node is associated 
       * with this soap fault struct, otherwise return NULL
       */
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axis2_soap_fault_t *fault,
                       const axis2_env_t *env);
                       
                                 
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
 * creates a soap fault struct 
 * @param env environment must not be NULL
 * @param parent soap body struct to which this soap
 * fault is the child
 * @param env Environment. MUST NOT be NULL
 * @returns pointer to axis2_soap_fault_t struct on success
 *  otherwise return NULL with error code set in environments error
 */

    
AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create_with_parent(const axis2_env_t *env,
                                    struct axis2_soap_body *parent);

/** create an returns a axis2_soap_fault_t struct with a soap fault detail 
 * element and have this exceptio string as a text of a child of soap fault
 * detail
 * @param env environment must not be NULL
 * @param parent soap body struct must not be NULL
 * @param exceptio an error string must not be NULL
 * @returns pointer to axis2_soap_fault_t on success ,
 * otherwise return NULL
 */

AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create_with_exception(const axis2_env_t *env,
                                        struct axis2_soap_body *parent,  
                                        axis2_char_t* exception);
                                        
AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create_default_fault(const axis2_env_t *env,
                                      struct axis2_soap_body *parent,
                                      const axis2_char_t *code_value,
                                      const axis2_char_t *reason_text,
                                      const int soap_version);                                         
                                        
/******************** Macros **************************************************/
    
    
/** free soap_fault */
#define AXIS2_SOAP_FAULT_FREE(fault , env) \
        ((fault)->ops->free_fn(fault, env))

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
        
        
#define AXIS2_SOAP_FAULT_GET_EXCEPTION(fault, env) \
        ((fault)->ops->get_exception(fault, env))
        
#define AXIS2_SOAP_FAULT_SET_EXCEPTION(fault, env, exception) \
        ((fault)->ops->set_exception(fault, env, exception)) 
        
/** @} */
#ifdef __cplusplus
}
#endif

 
#endif /* AXIS2_SOAP_FAULT_H */
