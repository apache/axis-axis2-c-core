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
 * An object that represents the contents of the SOAP body
 * element in a SOAP message. B SOAP body element consists of XML data
 * that affects the way the application-specific content is processed.
 * <P>
 * B <code>SOAPBody</code> object contains <code>OMBodyBlock</code>
 * objects, which have the content for the SOAP body.
 * B <code>SOAPFault</code> object, which carries status and/or
 * error information, is an example of a <code>OMBodyBlock</code> object.
 */

/**
 *   \brief soap_body operations struct
 *   ops Encapsulator struct of axis2_soap body
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_body_ops
    {
        axis2_status_t (AXIS2_CALL *free)(axis2_soap_body_t *body,
                                          axis2_env_t **env);
        /**
         * Indicates whether a <code>SOAPFault</code> object exists in
         * this <code>SOAPBody</code> object.
         *
         * @return <code>true</code> if a <code>SOAPFault</code> object exists in
         *         this <code>SOAPBody</code> object; <code>false</code>
         *         otherwise
         */
        axis2_bool_t (AXIS2_CALL *has_fault)(axis2_soap_body_t *body,
                                             axis2_env_t **env);
        /**
         * Returns the <code>SOAPFault</code> object in this <code>SOAPBody</code>
         * object.
         *
         * @return the <code>SOAPFault</code> object in this <code>SOAPBody</code>
         *         object
         */
        axis2_soap_fault_t* (AXIS2_CALL *get_fault)(axis2_soap_body_t *body,
                                                    axis2_env_t **env);
        /**
         * @param soapFault
         * @throws org.apache.axis2.om.OMException
         *
         * @throws OMException
         */
        axis2_status_t (AXIS2_CALL *add_fault)(axis2_soap_body_t *body,
                                               axis2_env_t **env,
                                               axis2_soap_fault_t *soap_fault);
                                                
        axis2_om_node_t* (AXIS2_CALL *get_base_node)(axis2_soap_body_t *body,
                                                     axis2_env_t **env);
                                                     
        axis2_status_t (AXIS2_CALL *set_base_node)(axis2_soap_body_t *body,
                                                    axis2_env_t **env,
                                                    axis2_om_node_t *om_node);
        
        int (AXIS2_CALL *get_soap_version)(axis2_soap_body_t *body,
                                           axis2_env_t **env);
         
        axis2_status_t (AXIS2_CALL *set_soap_version)(axis2_soap_body_t *body,
                                                      axis2_env_t **env,
                                                      int soap_version);
                                                      
        axis2_status_t (AXIS2_CALL *set_builder)(axis2_soap_body_t *body,
                                                 axis2_env_t **env,
                                                 struct axis2_soap_builder *builder);
                                                     
        axis2_status_t (AXIS2_CALL *build)(axis2_soap_body_t *body, 
                                           axis2_env_t **env);                                                 
                                                              
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
        
#define AXIS2_SOAP_BODY_HAS_FAULT(body, env) \
        ((body)->ops->has_fault(body, env))
        
#define AXIS2_SOAP_BODY_GET_FAULT(body, env) \
        ((body)->ops->get_fault(body, env))
        
#define AXIS2_SOAP_BODY_ADD_FAULT(body, env, soap_fault) \
        ((body)->ops->add_fault(body, env, soap_fault))
        
#define AXIS2_SOAP_BODY_GET_BASE_NODE(body, env) \
        ((body)->ops->get_base_node(body, env))
                                   
#define AXIS2_SOAP_BODY_SET_BASE_NODE(body, env, om_node) \
        ((body)->ops->set_base_node(body, env, om_node))
        
#define AXIS2_SOAP_BODY_GET_SOAP_VERSION(body, env) \
        ((body)->ops->get_soap_version(body, env))
        
#define AXIS2_SOAP_BODY_SET_SOAP_VERSION(body, env, soap_version) \
        ((body)->ops->set_soap_version(body, env, soap_version)) 
        
#define AXIS2_SOAP_BODY_SET_BUILDER(body, env, builder) \
        ((body)->ops->set_builder(body, env, builder))               

#define AXIS2_SOAP_BODY_BUILD(body, env) \
        ((body)->ops->build(body, env))

/** @} */
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_BODY_H */
