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
#include <axis2_soap_fault.h>
#include <axis2_soap_envelope.h>    


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct axis2_soap_body      axis2_soap_body_t;
typedef struct axis2_soap_body_ops  axis2_soap_body_ops_t;
struct axis2_soap_builder;
    
/**
 * @defgroup axis2_soap_body
 * @ingroup axis2_soap
 * @{
 */

/**
 * a struct that represents the contents of the SOAP body
 * element in a SOAP message. SOAP body element consists of XML data
 * that affects the way the application-specific content is processed.
 * soap_body_struct contains soap_header and 
 * which have the content for the SOAP body.
 * B <code>SOAPFault</code> object, which carries status and/or
 * error information, is an example of a <code>OMBodyBlock</code> object.
 */

/**
 *   \brief soap_body operations struct
 *   ops Encapsulator struct of axis2_soap body
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_body_ops
    {
        /**
         * Deallocate all the resources associated to soap_body
         * But it does not deallocate the underlying om structure
         * @param body soap_body struct
         * @param env must not be null
         * @return status code AXIS2_SUCCESS 
         */
        axis2_status_t (AXIS2_CALL *free)(axis2_soap_body_t *body,
                                          axis2_env_t **env);
        /**
         * Indicates whether a soap fault is available with this 
         * soap body 
         * @param body soap_body struct
         * @param env environment must not be null
         * @return AXIS2_TRUE if fault is available, AXIS2_FALSE otherwise
         */
        axis2_bool_t (AXIS2_CALL *has_fault)(axis2_soap_body_t *body,
                                             axis2_env_t **env);
        /**
         * returns the soap fault in this soap_body 
         * IF a soap_builder is associated with the soap_body
         * Pulling will  take place 
         * @param body soap_body 
         * @param env environment must not be null
         * @return axis2_soap_fault_t if available, NULL otherwise
        */
        axis2_soap_fault_t* (AXIS2_CALL *get_fault)(axis2_soap_body_t *body,
                                                    axis2_env_t **env);
        /**
         * get the underlying om_node 
         * @param body soap_body
         * @param env environment must not be null
         * @returns axis2_om_node_t
         */
        axis2_om_node_t* (AXIS2_CALL *get_base_node)(axis2_soap_body_t *body,
                                                     axis2_env_t **env);
        /**
         * associate an om node with this soap body
         * This om node should contain a om element with localname as Body
         * @param body soap_body
         * @param env environment
         * @param status code AXIS2_SUCCESS on success , AXIS2_FAILURE on error
         */
        axis2_status_t (AXIS2_CALL *set_base_node)(axis2_soap_body_t *body,
                                                    axis2_env_t **env,
                                                    axis2_om_node_t *om_node);
        /**
         *  return the soap version 
         * @param body soap_body
         * @param env environment must not be null
         * @return one of AXIS2_SOAP11 or AXIS2_SOAP12 
         */
        int (AXIS2_CALL *get_soap_version)(axis2_soap_body_t *body,
                                           axis2_env_t **env);
                                                      
        /**
         *  associate a soap_builder with this soap_body
         * This function is only to be used by soap builder
         * @param body soap_body
         * @param env environment must not be null
         * @param builder pointer to soap_builder
         * @return return status code
         */
        axis2_status_t (AXIS2_CALL *set_builder)(axis2_soap_body_t *body,
                                                 axis2_env_t **env,
                                                 struct axis2_soap_builder *builder);
                                                     
        /**
         * build the soap body completely 
         */                                                 
        axis2_status_t (AXIS2_CALL *build)(axis2_soap_body_t *body, 
                                           axis2_env_t **env);                                                 
        /**
         * This function is should be used by the soap builder only,
         * just set internal pointer in soap body struct to its fault
         * Does not create the om tree . If om tree is needed, use add_fault 
         * function 
         */                                                 
        axis2_status_t (AXIS2_CALL *set_fault)(axis2_soap_body_t *body,
                                               axis2_env_t **env,
                                               struct axis2_soap_fault *soap_fault);                                                 
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
axis2_soap_body_create(axis2_env_t **env);    
    
    
AXIS2_DECLARE(axis2_soap_body_t *)
axis2_soap_body_create_with_parent(axis2_env_t **env, 
                                   struct axis2_soap_envelope *envelope);
    
/******************** Macros **************************************************/
    
/** free soap_body */
#define AXIS2_SOAP_BODY_FREE(body , env) \
        ((body)->ops->free(body, env))
        
/** indecate whether soap_body has a fault or not*/        
#define AXIS2_SOAP_BODY_HAS_FAULT(body, env) \
        ((body)->ops->has_fault(body, env))
        
#define AXIS2_SOAP_BODY_GET_FAULT(body, env) \
        ((body)->ops->get_fault(body, env))
        
#define AXIS2_SOAP_BODY_GET_BASE_NODE(body, env) \
        ((body)->ops->get_base_node(body, env))
                                   
#define AXIS2_SOAP_BODY_SET_BASE_NODE(body, env, om_node) \
        ((body)->ops->set_base_node(body, env, om_node))
        
#define AXIS2_SOAP_BODY_GET_SOAP_VERSION(body, env) \
        ((body)->ops->get_soap_version(body, env))
        
#define AXIS2_SOAP_BODY_SET_BUILDER(body, env, builder) \
        ((body)->ops->set_builder(body, env, builder))               

#define AXIS2_SOAP_BODY_BUILD(body, env) \
        ((body)->ops->build(body, env))

#define AXIS2_SOAP_BODY_SET_FAULT(body, env, fault) \
        ((body)->ops->set_fault(body, env, fault))
/** @} */
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_BODY_H */
