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
 
 #ifndef AXIS2_SOAP_ENVELOPE_H
 #define AXIS2_SOAP_ENVELOPE_H
 
/**
 * @file axis2_soap_envelope.h
 * @brief axis2_soap_envelope struct
 *        corresponds to root element of soap message
 */
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_namespace.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_envelope        axis2_soap_envelope_t;
    typedef struct axis2_soap_envelope_ops    axis2_soap_envelope_ops_t;
    
    typedef struct axis2_soap_body            axis2_soap_body_t;
    typedef struct axis2_soap_header          axis2_soap_header_t;
    
/**
 * @defgroup axis2_soap_envelope
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_envelope operations struct
 *   ops Encapsulator struct of axis2_soap body
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_envelope_ops
    {
      /**
        * Free an axis2_soap_envelope
        * @param  body pointer to soap_envelope struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *free_fn)(axis2_soap_envelope_t *envelope,
                                             axis2_env_t **env);
       /**
        *
        * Returns the <CODE>SOAPHeader</CODE> object for this <CODE>
        * SOAPEnvelope</CODE> object. <P> This SOAPHeader will just be a container
        * for all the headers in the <CODE>OMMessage</CODE> </P> 
        */
        axis2_soap_header_t* (AXIS2_CALL *get_header)
                                            (axis2_soap_envelope_t *envelope,
                                             axis2_env_t **env);
                                             
        axis2_soap_header_t* (AXIS2_CALL *add_header_block)
                                            (axis2_soap_envelope_t *envelope,
                                             axis2_env_t **env,
                                             axis2_char_t *localname,
                                             axis2_om_namespace_t* ns);
                                             
        axis2_soap_body_t* (AXIS2_CALL *get_body)
                                            (axis2_soap_envelope_t *envelope,
                                             axis2_env_t **env);
                                             
        axis2_om_node_t* (AXIS2_CALL *get_base_node)
                                            (axis2_soap_envelope_t *envelope,
                                             axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *set_base_node)
                                            (axis2_soap_envelope_t *envelope,
                                             axis2_env_t **env);
    };

  /**
    * \brief soap_envelope struct
    * represent a soap_envelope
    */
    struct axis2_soap_envelope
    {
        /** operation of axis2_soap_envelope struct */
        axis2_soap_envelope_ops_t *ops;
       
    };

  /**
    * creates a soap body struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_envelope_t *)
axis2_soap_envelope_create (axis2_env_t **env);

AXIS2_DECLARE(axis2_soap_envelope_t *)
axis2_soap_envelope_create_with_namespace(axis2_env_t **env,
                           axis2_om_namespace_t* ns);
                           
/******************** Macros **************************************************/
    
    
/** free soap_envelope */
#define AXIS2_SOAP_ENVELOPE_FREE(envelope , env) \
        ((envelope)->ops->free_fn(envelope, env))

#define AXIS2_SOAP_ENVELOPE_GET_HEADDER(envelope , env) \
        ((envelope)->ops->get_headder(envelope, env))

#define AXIS2_SOAP_ENVELOPE_ADD_HEADDER_BLOCK(envelope , env, localname, ns) \
        ((envelope)->ops->add_headder_block(envelope, env, localname, ns))

#define AXIS2_SOAP_ENVELOPE_GET_BOBY(envelope , env) \
        ((envelope)->ops->get_body(envelope, env))

#define AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(envelope , env) \
        ((envelope)->ops->get_base_node(envelope, env))

#define AXIS2_SOAP_ENVELOPE_SET_BASE_NODE(envelope , env) \
        ((envelope)->ops->set_base_node(envelope, env))                                

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_ENVELOPE_H */