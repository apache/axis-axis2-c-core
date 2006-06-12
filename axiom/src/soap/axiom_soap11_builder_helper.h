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
 
#ifndef AXIOM_SOAP11_BUILDER_HELPER_H
#define AXIOM_SOAP11_BUILDER_HELPER_H

 
/**
 * @file axiom_soap_11_builder_helper.h
 * @brief axiom_soap11_builder_helper
 */
#include <axiom_soap_builder.h> 

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct axiom_soap11_builder_helper axiom_soap11_builder_helper_t;
typedef struct axiom_soap11_builder_helper_ops axiom_soap11_builder_helper_ops_t;
    
/**
 * @defgroup axiom_soap11_builder_helper 
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief axiom_soap11_builder_helper operations ops struct 
 */
 
 AXIS2_DECLARE_DATA   struct axiom_soap11_builder_helper_ops
    {
        axis2_status_t (AXIS2_CALL *free_fn)
                    (axiom_soap11_builder_helper_t *builder_helper,
                     const axis2_env_t *env);
        
        axis2_status_t (AXIS2_CALL *handle_event)
                    (axiom_soap11_builder_helper_t *builder_helper,
                     const axis2_env_t *env,
                     axiom_node_t *om_element_node,
                     int element_level);
    };                                                      

  /**
    * \brief soap_11_builder_helper struct
    */
struct axiom_soap11_builder_helper
{
    axiom_soap11_builder_helper_ops_t *ops;
};

  /**
    * creates a soap11_builder_helper_create
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_EXTERN axiom_soap11_builder_helper_t* AXIS2_CALL
axiom_soap11_builder_helper_create(const axis2_env_t *env, 
                                   axiom_soap_builder_t *soap_builder,
                                   axiom_stax_builder_t *om_builder);
    
/******************** Macros **************************************************/
    
/** free soap_body */
#define AXIOM_SOAP11_BUILDER_HELPER_FREE(builder_helper, env) \
        ((builder_helper)->ops->free_fn(builder_helper, env))
        
#define AXIOM_SOAP11_BUILDER_HELPER_HANDLE_EVENT(builder_helper, \
            env,  om_element_node, element_level) \
        ((builder_helper)->ops->handle_event(builder_helper, \
            env,  om_element_node,  element_level))
        
/** @} */
#ifdef __cplusplus
}
#endif

#endif /* AXIOM_SOAP11_BUILDER_HELPER_H */
