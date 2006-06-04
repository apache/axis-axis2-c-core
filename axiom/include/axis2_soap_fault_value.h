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

        axis2_status_t (AXIS2_CALL *
      free_fn)(axis2_soap_fault_value_t *fault_value,
                 const axis2_env_t *env);
      
      /**
        * Get the text value of the env:Value element directly under env:Code element
        * @param fault_value pointer to axis2_soap_fault_t
        * @param env Environment. MUST NOT BE NULL
        * @return text value 
        */
      axis2_char_t* (AXIS2_CALL *
      get_text)(axis2_soap_fault_value_t *fault_value,
            const axis2_env_t *env);   
      
      /**
        * Set the text value of the env:Value element directly under env:Code element
        * @param fault_value pointer to axis2_soap_fault_t
        * @param env Environment. MUST NOT BE NULL
        * @param text value to be set
        */
        axis2_om_node_t* (AXIS2_CALL *
      get_base_node)(axis2_soap_fault_value_t *fault_value,
                       const axis2_env_t *env);
        /**
         * set the text value of soap_fault_value element 
         * @param fault_value pointer to soap fault value struct
         * @param env environment MUST not be NULL
         * @param text Text value to be set
         */
                                
        axis2_status_t (AXIS2_CALL *
      set_text)(axis2_soap_fault_value_t *fault_value,
            const axis2_env_t *env, axis2_char_t *text);   
            
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
AXIS2_EXTERN axis2_soap_fault_value_t * AXIS2_CALL
axis2_soap_fault_value_create_with_subcode(const axis2_env_t *env,
                            axis2_soap_fault_sub_code_t *parent);

AXIS2_EXTERN axis2_soap_fault_value_t * AXIS2_CALL
axis2_soap_fault_value_create_with_code(const axis2_env_t *env,
                            axis2_soap_fault_code_t *parent);  


                             

/******************** Macros **************************************************/
    
    
/** free soap_fault_value */
#define AXIS2_SOAP_FAULT_VALUE_FREE(fault_value , env) \
        ((fault_value)->ops->free_fn(fault_value, env))
        
#define AXIS2_SOAP_FAULT_VALUE_GET_BASE_NODE(fault_value, env) \
        ((fault_value)->ops->get_base_node(fault_value, env))         

#define AXIS2_SOAP_FAULT_VALUE_GET_TEXT(fault_value, env) \
      ((fault_value)->ops->get_text(fault_value, env))
      
#define AXIS2_SOAP_FAULT_VALUE_SET_TEXT(fault_value, env, text) \
        ((fault_value)->ops->set_text(fault_value, env, text))      
/** @} */

#ifdef __cplusplus
}
#endif 
 
#endif /* AXIS2_SOAP_FAULT_VALUE_H */
