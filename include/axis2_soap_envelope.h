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
    
    struct axis2_soap_body;
    struct axis2_soap_header;
    struct axis2_soap_header_block;
    
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
         * Returns the <CODE>SOAPHeader</CODE> object for this <CODE>
         * SOAPEnvelope</CODE> object. <P> This SOAPHeader will just be a container
         * for all the headers in the <CODE>OMMessage</CODE> </P>
         *
         * @return the <CODE>SOAPHeader</CODE> object or <CODE> null</CODE> if there
         *         is none
         * @throws org.apache.axis2.om.OMException
         *                     if there is a problem obtaining
         *                     the <CODE>SOAPHeader</CODE>
         *                     object
         * @throws OMException
         */
        struct axis2_soap_header* (AXIS2_CALL *get_header)(axis2_soap_envelope_t *envelope,
            axis2_env_t **env);
        /**
         * Convenience method to add a SOAP header to this envelope
         *
         * @param namespaceURI
         * @param name
         */
        struct axis2_soap_header_block* (AXIS2_CALL *add_header)(axis2_soap_envelope_t *envelope,
            axis2_env_t **env,
            axis2_char_t *namespace_uri, 
            axis2_char_t *name);
        /**
         * Returns the <CODE>SOAPBody</CODE> object associated with this
         * <CODE>SOAPEnvelope</CODE> object. <P> This SOAPBody will just be a
         * container for all the BodyElements in the <CODE>OMMessage</CODE> </P>
         *
         * @return the <CODE>SOAPBody</CODE> object for this <CODE>
         *         SOAPEnvelope</CODE> object or <CODE>null</CODE> if there is none
         * @throws org.apache.axis2.om.OMException
         *                     if there is a problem obtaining
         *                     the <CODE>SOAPBody</CODE> object
         * @throws OMException
         */
        struct axis2_soap_body* (AXIS2_CALL *get_body)(axis2_soap_envelope_t *envelope,
            axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *serialize)(axis2_soap_envelope_t *envelope,
            axis2_env_t **env,
            axis2_om_output_t *om_output, 
            axis2_bool_t cache);
        axis2_status_t (AXIS2_CALL *free)(axis2_soap_envelope_t *envelope,
                               axis2_env_t **env);
        axis2_om_node_t* (AXIS2_CALL *get_base)(axis2_soap_envelope_t *envelope,
                                        axis2_env_t **env);
        int (AXIS2_CALL *get_soap_version)(axis2_soap_envelope_t *envelope,
                                           axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *set_soap_version)(axis2_soap_envelope_t *envelope,
                                           axis2_env_t **env,
                                           int soap_version);
        axis2_status_t (AXIS2_CALL *set_body)(axis2_soap_envelope_t *envelope,
            axis2_env_t **env, struct axis2_soap_body *body);
        axis2_status_t (AXIS2_CALL *set_header)(axis2_soap_envelope_t *envelope,
            axis2_env_t **env, struct axis2_soap_header *header);
        axis2_om_namespace_t* (AXIS2_CALL *get_namespace)(axis2_soap_envelope_t *envelope,
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
AXIS2_DECLARE(axis2_soap_envelope_t*)
axis2_soap_envelope_create(axis2_env_t **env, axis2_om_namespace_t *ns);
                           
/******************** Macros **************************************************/

#define AXIS2_SOAP_ENVELOPE_GET_HEADDER(envelope, env) ((envelope)->ops->get_header(envelope, env))
#define AXIS2_SOAP_ENVELOPE_ADD_HEADER(envelope, env, namespace_uri, name) ((envelope)->ops->add_header(envelope, env, namespace_uri, name))
#define AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env) ((envelope)->ops->get_body(envelope, env))
#define AXIS2_SOAP_ENVELOPE_SERIALIZE(envelope, env, om_output, cache) ((envelope)->ops->serialize(envelope, env, om_output, cache))
#define AXIS2_SOAP_ENVELOPE_FREE(envelope, env) ((envelope)->ops->free(envelope, env))
#define AXIS2_SOAP_ENVELOPE_GET_BASE(envelope, env) ((envelope)->ops->get_base(envelope, env))
#define AXIS2_SOAP_ENVELOPE_GET_SOAP_VERSION(envelope, env) ((envelope)->ops->get_soap_version(envelope, env))
#define AXIS2_SOAP_ENVELOPE_SET_SOAP_VERSION(envelope, env, soap_version) ((envelope)->ops->set_soap_version(envelope, env, soap_version))
#define AXIS2_SOAP_ENVELOPE_SET_BODY(envelope, env, body) ((envelope)->ops->set_body(envelope, env, body))
#define AXIS2_SOAP_ENVELOPE_SET_HEADER(envelope, env, header) ((envelope)->ops->set_header(envelope, env, header))
#define AXIS2_SOAP_ENVELOPE_GET_NAMESPACE(envelope, env) ((envelope)->ops->get_namespace(envelope, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_ENVELOPE_H */
