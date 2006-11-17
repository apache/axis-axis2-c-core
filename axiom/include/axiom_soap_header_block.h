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
 
 #ifndef AXIOM_SOAP_HEADER_BLOCK_H
 #define AXIOM_SOAP_HEADER_BLOCK_H

  
   /**
    * @file axiom_soap_header_block.h
    * @brief axiom_soap_header_block struct
    */
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_array_list.h>
#include <axiom_soap_header.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_header_block axiom_soap_header_block_t;
    typedef struct axiom_soap_header_block_ops axiom_soap_header_block_ops_t;
    
/**
 * @defgroup axiom_soap_header_block soap header block
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief soap_header_block operations struct
 *   ops Encapsulator struct of axiom_soap header_block
 */
    struct axiom_soap_header_block_ops
    {
      /**
        * Free an axiom_soap_header_block
        * @param  header_block pointer to soap_header_block struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_soap_header_block_t *header_block,
                 const axis2_env_t *env);
                                             
        axis2_status_t (AXIS2_CALL *
      set_role)(axiom_soap_header_block_t *header_block,
                  const axis2_env_t *env,
                  axis2_char_t *uri);  
        
        axis2_status_t (AXIS2_CALL *
      set_must_understand_with_bool)(axiom_soap_header_block_t *header_block,
                                      const axis2_env_t *env,
                                       axis2_bool_t must_understand);
                             
        axis2_status_t (AXIS2_CALL *
      set_must_understand_with_string)(axiom_soap_header_block_t *header_block,
                                       const axis2_env_t *env,
                                       axis2_char_t *must_understand);
                                             
        axis2_bool_t (AXIS2_CALL *
      get_must_understand)(axiom_soap_header_block_t *header_block,
                             const axis2_env_t *env);
                             
                             
        axis2_bool_t (AXIS2_CALL *
      is_processed)(axiom_soap_header_block_t *header_block,
                      const axis2_env_t *env);
                             
        axis2_status_t (AXIS2_CALL *
      set_processed)(axiom_soap_header_block_t *header_block,
                       const axis2_env_t *env);
                  
        axis2_char_t* (AXIS2_CALL *
      get_role)(axiom_soap_header_block_t *header_block,
                  const axis2_env_t *env);
          
        axis2_status_t (AXIS2_CALL *
      set_attribute)(axiom_soap_header_block_t *header_block,
                       const axis2_env_t *env,
                       axis2_char_t *attr_name,
                       axis2_char_t *attr_value,
                       axis2_char_t *soap_envelope_namespace_uri);
                  
                             
        axis2_char_t* (AXIS2_CALL *
      get_attribute)(axiom_soap_header_block_t *header_block,
                       const axis2_env_t *env,
                       axis2_char_t *attr_name,
                       axis2_char_t *soap_envelope_namespace_uri);                            
                                       
                                              
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axiom_soap_header_block_t *header_block,
                       const axis2_env_t *env);
        
        int (AXIS2_CALL *
      get_soap_version)(axiom_soap_header_block_t *header_block,
                          const axis2_env_t *env);

    };

  /**
    * \brief soap_header_block struct
    * represent a soap_header_block
    */
    struct axiom_soap_header_block
    {
        /** operation of axiom_soap_header_block struct */
        axiom_soap_header_block_ops_t *ops;
       
    };

/**
* creates a soap struct 
* @param env Environment. MUST NOT be NULL
* this is an internal function.    
*/


AXIS2_EXTERN axiom_soap_header_block_t * AXIS2_CALL
axiom_soap_header_block_create_with_parent(const axis2_env_t *env,
                                           const axis2_char_t *localname,
                                           axiom_namespace_t *ns,
                                           struct axiom_soap_header *parent);
                            
/******************** Macros **************************************************/
    
/** free soap_header_block */
#define AXIOM_SOAP_HEADER_BLOCK_FREE(header_block , env) \
        ((header_block)->ops->free_fn(header_block, env))
        
#define AXIOM_SOAP_HEADER_BLOCK_GET_ROLE(header_block , env) \
        ((header_block)->ops->get_role(header_block, env))

#define AXIOM_SOAP_HEADER_BLOCK_IS_PROCESSED(header_block , env) \
        ((header_block)->ops->is_processed(header_block, env))

#define AXIOM_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block , env) \
        ((header_block)->ops->set_processed(header_block, env))
        
#define AXIOM_SOAP_HEADER_BLOCK_GET_MUST_UNDERSTAND(header_block, env) \
        ((header_block)->ops->get_must_understand(header_block, env)) 

#define AXIOM_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_BOOL(header_block,\
             env, must_understand) \
        ((header_block)->ops->set_must_understand_with_bool(header_block, \
             env, must_understand)) 
             
#define AXIOM_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_STRING(header_block,\
             env, must_understand) \
        ((header_block)->ops->set_must_understand_with_string(header_block, \
             env, must_understand))                    

#define AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env) \
        ((header_block)->ops->get_base_node(header_block, env))   
        
#define AXIOM_SOAP_HEADER_BLOCK_GET_SOAP_VERSION(header_block, env) \
        ((header_block)->ops->get_soap_version(header_block, env))
        
#define AXIOM_SOAP_HEADER_BLOCK_GET_ATTRIBUTE(header_block, env,\
            attr_name, soap_envelope_namespace_uri)  \
        ((header_block)->ops->get_attribute(header_block, env, \
            attr_name, soap_envelope_namespace_uri))
            
#define AXIOM_SOAP_HEADER_BLOCK_SET_ATTRIBUTE(header_block, env,\
            attr_name, attr_value, soap_envelope_namespace_uri)  \
        ((header_block)->ops->set_attribute(header_block, env, \
            attr_name, attr_value, soap_envelope_namespace_uri))            

/** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIOM_SOAP_HEADER_BLOCK_H */
