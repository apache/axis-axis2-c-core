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
 
 #ifndef AXIS2_SOAP_HEADER_BLOCK_H
 #define AXIS2_SOAP_HEADER_BLOCK_H

  
   /**
    * @file axis2_soap_header_block.h
    * @brief axis2_soap_header_block struct
    */
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_array_list.h>
#include <axis2_soap_header.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_header_block axis2_soap_header_block_t;
    typedef struct axis2_soap_header_block_ops axis2_soap_header_block_ops_t;
    
/**
 * @defgroup axis2_soap_header_block
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_header_block operations struct
 *   ops Encapsulator struct of axis2_soap header_block
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_header_block_ops
    {
      /**
        * Free an axis2_soap_header_block
        * @param  header_block pointer to soap_header_block struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *free_fn)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
                                             
        axis2_status_t (AXIS2_CALL *set_role)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             axis2_char_t *uri);  
        
        axis2_status_t (AXIS2_CALL *set_must_understand_with_bool)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             axis2_bool_t *must_understand);
                             
        axis2_status_t (AXIS2_CALL *set_must_understand_with_string)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             axis2_char_t *must_understand);
                                             
        axis2_bool_t (AXIS2_CALL *get_must_understand)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
                             
                             
        axis2_bool_t (AXIS2_CALL *is_processed)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
                             
        axis2_status_t (AXIS2_CALL *set_processed)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);                                                                                                                                                                                    
                                                                                                                                    
                                   
        axis2_char_t* (AXIS2_CALL *get_role)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
          
        axis2_status_t (AXIS2_CALL *set_attribute)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             axis2_char_t *attr_name,
                             axis2_char_t *attr_value,
                             axis2_char_t *soap_envelope_namespace_uri);
                             
        axis2_char_t* (AXIS2_CALL *get_attribute)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             axis2_char_t *attr_name,
                             axis2_char_t *soap_envelope_namespace_uri);                            
                                       
                                                                                          
       /**
        * This is only intended to be used by the builder,
        */
        axis2_status_t (AXIS2_CALL *set_base_node)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             axis2_om_node_t *node);
                                              
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
                                                                                                                                                                                       
        int (AXIS2_CALL *get_soap_version)  
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
        int (AXIS2_CALL *set_soap_version)  
                            (axis2_soap_header_block_t *header_block,
                             axis2_env_t **env,
                             int soap_version);                                                                     
    };

  /**
    * \brief soap_header_block struct
    * represent a soap_header_block
    */
    struct axis2_soap_header_block
    {
        /** operation of axis2_soap_header_block struct */
        axis2_soap_header_block_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */

AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap_header_block_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap_header_block_create_with_parent(axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_om_namespace_t *ns,
                                           struct axis2_soap_header *parent);
                            
AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap11_header_block_create_with_parent(axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_om_namespace_t *ns,
                                           struct axis2_soap_header *parent);
AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap12_header_block_create_with_parent(axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_om_namespace_t *ns,
                                           struct axis2_soap_header *parent);                                                                        
/******************** Macros **************************************************/
    
/** free soap_header_block */
#define AXIS2_SOAP_HEADER_BLOCK_FREE(header_block , env) \
        ((header_block)->ops->free_fn(header_block, env))
        
#define AXIS2_SOAP_HEADER_BLOCK_GET_ROLE(header_block , env) \
        ((header_block)->ops->get_role(header_block, env))

#define AXIS2_SOAP_HEADER_BLOCK_IS_PROCESSED(header_block , env) \
        ((header_block)->ops->is_precessed(header_block, env))

#define AXIS2_SOAP_HEADER_BLOCK_SET_PRECESSED(header_block , env) \
        ((header_block)->ops->set_precessed(header_block, env))
        
#define AXIS2_SOAP_HEADER_BLOCK_GET_MUST_UNDERSTAND(header_block, env) \
        ((header_block)->ops->get_must_understand(header_block, env)) 

#define AXIS2_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_BOOL(header_block,\
             env, must_understand) \
        ((header_block)->ops->get_must_understand_with_bool(header_block, \
             env, must_understand)) 
             
#define AXIS2_SOAP_HEADER_BLOCK_SET_MUST_UNDERSTAND_WITH_STRING(header_block,\
             env, must_understand) \
        ((header_block)->ops->get_must_understand_with_string(header_block, \
             env, must_understand))                    

#define AXIS2_SOAP_HEADER_BLOCK_SET_BASE_NODE(header_block, env, node) \
        ((header_block)->ops->set_base_node(header_block, env, node))
        
#define AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env) \
        ((header_block)->ops->get_base_node(header_block, env))   
        
#define AXIS2_SOAP_HEADER_BLOCK_SET_SOAP_VERSION(header_block, env, version) \
        ((header_block)->ops->set_soap_version(header_block, env, version))
        
#define AXIS2_SOAP_HEADER_BLOCK_GET_SOAP_VERSION(header_block, env) \
        ((header_block)->ops->get_soap_version(header_block, env))
        
#define AXIS2_SOAP_HEADER_BLOCK_GET_ATTRIBUTE(header_block, env,\
            attr_name, soap_envelope_namespace_uri)  \
        ((header_block)->ops->get_attribute(header_block, env, \
            attr_name, soap_envelope_namespace_uri))
            
#define AXIS2_SOAP_HEADER_BLOCK_SET_ATTRIBUTE(header_block, env,\
            attr_name, soap_envelope_namespace_uri)  \
        ((header_block)->ops->set_attribute(header_block, env, \
            attr_name, soap_envelope_namespace_uri))            
/** @} */

#ifdef __cplusplus
}
#endif

 
 #endif /* AXIS2_SOAP_HEADER_BLOCK_H */
