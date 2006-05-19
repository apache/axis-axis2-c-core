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
 
#ifndef AXIS2_SOAP11_BUILDER_HELPER_H
#define AXIS2_SOAP11_BUILDER_HELPER_H

 
/**
 * @file axis2_soap_11_builder_helper.h
 * @brief axis2_soap11_builder_helper
 */
#include <axis2_soap_builder.h> 

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct axis2_soap11_builder_helper axis2_soap11_builder_helper_t;
typedef struct axis2_soap11_builder_helper_ops axis2_soap11_builder_helper_ops_t;
    
/**
 * @defgroup axis2_soap11_builder_helper 
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief axis2_soap11_builder_helper operations ops struct 
 */
 
 AXIS2_DECLARE_DATA   struct axis2_soap11_builder_helper_ops
    {
        axis2_status_t (AXIS2_CALL *free_fn)
                    (axis2_soap11_builder_helper_t *builder_helper,
                     axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *handle_event)
                    (axis2_soap11_builder_helper_t *builder_helper,
                     axis2_env_t **env,
                     axis2_om_node_t *om_element_node,
                     int element_level);
    };                                                      

  /**
    * \brief soap_11_builder_helper struct
    */
struct axis2_soap11_builder_helper
{
    axis2_soap11_builder_helper_ops_t *ops;
};

  /**
    * creates a soap11_builder_helper_create
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap11_builder_helper_t*)
axis2_soap11_builder_helper_create(axis2_env_t **env, 
                                   axis2_soap_builder_t *soap_builder,
                                   axis2_om_stax_builder_t *om_builder);
    
/******************** Macros **************************************************/
    
/** free soap_body */
#define AXIS2_SOAP11_BUILDER_HELPER_FREE(builder_helper, env) \
        ((builder_helper)->ops->free_fn(builder_helper, env))
        
#define AXIS2_SOAP11_BUILDER_HELPER_HANDLE_EVENT(builder_helper, \
            env,  om_element_node, element_level) \
        ((builder_helper)->ops->handle_event(builder_helper, \
            env,  om_element_node,  element_level))
        
/** @} */
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP11_BUILDER_HELPER_H */
