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
 
 #ifndef AXIOM_SOAP_FAULT_TEXT_H
 #define AXIOM_SOAP_FAULT_TEXT_H
 
   /**
    * @file axiom_soap_fault_text.h
    * @brief axiom_soap_fault_text 
    */
#include <axis2_env.h>
#include <axiom_soap_fault_reason.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault_text_ops axiom_soap_fault_text_ops_t;
    typedef struct axiom_soap_fault_text axiom_soap_fault_text_t;
/**
 * @defgroup axiom_soap_fault_text soap fault text
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief soap_fault_text operations struct
 *   ops Encapsulator struct of axiom_soap_fault_text
 */
    struct axiom_soap_fault_text_ops
    {
      /**
        * Free an axiom_soap_fault_text
        * @param  fault_text pointer to soap_fault_text struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_soap_fault_text_t *fault_text,
                 const axis2_env_t *env);

        axis2_status_t (AXIS2_CALL *
      set_lang)(axiom_soap_fault_text_t *fault_text,
                  const axis2_env_t *env,
                  const axis2_char_t* lang);
                                     
        axis2_char_t* (AXIS2_CALL *
      get_lang)(axiom_soap_fault_text_t *fault_text,
                  const axis2_env_t *env);
    
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axiom_soap_fault_text_t *fault_text,
                       const axis2_env_t *env);
                       
        axis2_status_t (AXIS2_CALL *
        set_text)(axiom_soap_fault_text_t *fault_text,
                  const axis2_env_t *env,
                  axis2_char_t *value,
                  axis2_char_t *lang);
                  
        axis2_char_t * (AXIS2_CALL *
        get_text)(axiom_soap_fault_text_t *fault_text,
                  const axis2_env_t *env);
                                                            
    };      

  /**
    * \brief soap_fault_text struct
    * represent a soap_fault_text
    */
    struct axiom_soap_fault_text
    {
        /** operation of axiom_soap_fault_text struct */
        axiom_soap_fault_text_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_EXTERN axiom_soap_fault_text_t * AXIS2_CALL
axiom_soap_fault_text_create_with_parent(const axis2_env_t *env,
                            axiom_soap_fault_reason_t *fault);
                            
/******************** Macros **************************************************/
    
/** free soap_fault_text */
#define AXIOM_SOAP_FAULT_TEXT_FREE(fault_text , env) \
        ((fault_text)->ops->free_fn(fault_text, env))

#define AXIOM_SOAP_FAULT_TEXT_SET_LANG(fault_text , env, lang) \
        ((fault_text)->ops->set_lang(fault_text, env, lang))
        
#define AXIOM_SOAP_FAULT_TEXT_GET_LANG(fault_text , env) \
        ((fault_text)->ops->get_lang(fault_text, env)) 
        
#define AXIOM_SOAP_FAULT_TEXT_GET_BASE_NODE(fault_text, env) \
        ((fault_text)->ops->get_base_node(fault_text, env))         

#define AXIOM_SOAP_FAULT_TEXT_SET_TEXT(fault_text, env, value, lang) \
        ((fault_text)->ops->set_text(fault_text, env, value, lang))

#define AXIOM_SOAP_FAULT_TEXT_GET_TEXT(fault_text, env) \
        ((fault_text)->ops->get_text(fault_text, env))        
/** @} */

#ifdef __cplusplus
}
#endif 
 
#endif /* AXIOM_SOAP_FAULT_TEXT_H */
