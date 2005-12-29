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
 
 #include <axis2_soap_envelope.h>
 #include <axis2_soap_body.h>
 #include <axis2_soap_header.h>
 #include <axis2_hash.h>
 #include <axis2_soap.h>
 
 /******************* impl struct *********************************************/
 
 typedef struct axis2_soap_envelope_impl_t
 {
    axis2_soap_envelope_t soap_envelope;
    
    axis2_om_node_t *base;
    
    int soap_version;
    
    axis2_soap_header_t *header;
     
    axis2_soap_body_t *body;
    
 }axis2_soap_envelope_impl_t;
 
 /****************** Macro ****************************************************/
 
 #define AXIS2_INTF_TO_IMPL(envelope) ((axis2_soap_envelope_impl_t*)envelope)
 
 /****************** Function prototypes **************************************/

axis2_soap_header_t* AXIS2_CALL axis2_soap_envelope_get_header(axis2_soap_envelope_t *envelope,
    axis2_env_t **env);
axis2_soap_header_block_t* AXIS2_CALL axis2_soap_envelope_add_header(axis2_soap_envelope_t *envelope,
    axis2_env_t **env,
    axis2_char_t *namespace_uri, 
    axis2_char_t *name);
axis2_soap_body_t* AXIS2_CALL axis2_soap_envelope_get_body(axis2_soap_envelope_t *envelope,
    axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_soap_envelope_serialize(axis2_soap_envelope_t *envelope,
    axis2_env_t **env,
    axis2_om_output_t *om_output, 
    axis2_bool_t cache);
axis2_status_t AXIS2_CALL
axis2_soap_envelope_free(axis2_soap_envelope_t *envelope,
                       axis2_env_t **env);
axis2_om_node_t* AXIS2_CALL 
axis2_soap_envelope_get_base(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env);
int AXIS2_CALL 
axis2_soap_envelope_get_soap_version(axis2_soap_envelope_t *envelope,
                                   axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_soap_envelope_set_soap_version(axis2_soap_envelope_t *envelope,
                                   axis2_env_t **env,
                                   int soap_version);
axis2_status_t AXIS2_CALL axis2_soap_envelope_set_body(axis2_soap_envelope_t *envelope,
    axis2_env_t **env, axis2_soap_body_t *body);
axis2_status_t AXIS2_CALL axis2_soap_envelope_set_header(axis2_soap_envelope_t *envelope,
    axis2_env_t **env, axis2_soap_header_t *header);
axis2_om_namespace_t* AXIS2_CALL axis2_soap_envelope_get_namespace(axis2_soap_envelope_t *envelope,
            axis2_env_t **env);

                                   
/*************** function implementations *************************************/

axis2_soap_envelope_t* AXIS2_CALL
axis2_soap_envelope_create(axis2_env_t **env, axis2_om_namespace_t *ns)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    axis2_om_element_t *ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    envelope_impl = (axis2_soap_envelope_impl_t*)AXIS2_MALLOC(
                    (*env)->allocator,
                    sizeof(axis2_soap_envelope_impl_t));
    if(!envelope_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    envelope_impl->soap_envelope.ops = NULL;
    envelope_impl->base = NULL;
    envelope_impl->soap_version = AXIS2_SOAP12;    
    envelope_impl->header = NULL;
    envelope_impl->body = NULL;
    
    ele = axis2_om_element_create(env, NULL, AXIS2_SOAP_ENVELOPE_LOCAL_NAME, ns, &(envelope_impl->base));
    if (!ele)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_soap_envelope_free(&(envelope_impl->soap_envelope), env);
        return NULL;
    }
    
    envelope_impl->soap_envelope.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_soap_envelope_ops_t) );
    if (!envelope_impl->soap_envelope.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_soap_envelope_free(&(envelope_impl->soap_envelope), env);
        return NULL;        
    }
    
    envelope_impl->soap_envelope.ops->get_header = axis2_soap_envelope_get_header;
    envelope_impl->soap_envelope.ops->add_header = axis2_soap_envelope_add_header;
    envelope_impl->soap_envelope.ops->get_body = axis2_soap_envelope_get_body;
    envelope_impl->soap_envelope.ops->serialize = axis2_soap_envelope_serialize;    
    envelope_impl->soap_envelope.ops->free = axis2_soap_envelope_free;
    envelope_impl->soap_envelope.ops->get_base = axis2_soap_envelope_get_base;
    envelope_impl->soap_envelope.ops->get_soap_version = axis2_soap_envelope_get_soap_version;
    envelope_impl->soap_envelope.ops->set_soap_version = axis2_soap_envelope_set_soap_version;
    envelope_impl->soap_envelope.ops->get_namespace = axis2_soap_envelope_get_namespace;
    
    return &(envelope_impl->soap_envelope);        
}

axis2_status_t AXIS2_CALL
axis2_soap_envelope_free(axis2_soap_envelope_t *envelope,
                       axis2_env_t **env)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    if(envelope->ops)
    {
        AXIS2_FREE((*env)->allocator, envelope->ops);
        envelope->ops = NULL;
    }
    
    if(envelope_impl->base)
    {
        AXIS2_OM_NODE_FREE_TREE(envelope_impl->base, env);
        envelope_impl->base = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, envelope_impl);
    envelope_impl = NULL;
    return AXIS2_SUCCESS;
}
    
axis2_om_node_t* AXIS2_CALL 
axis2_soap_envelope_get_base(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(envelope)->base;
}
                                             
int AXIS2_CALL 
axis2_soap_envelope_get_soap_version(axis2_soap_envelope_t *envelope,
                                   axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(envelope)->soap_version;    
}
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_envelope_set_soap_version(axis2_soap_envelope_t *envelope,
                                   axis2_env_t **env,
                                   int soap_version)
{
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(envelope)->soap_version = soap_version;
    return AXIS2_SUCCESS;
}

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
axis2_soap_header_t* AXIS2_CALL axis2_soap_envelope_get_header(axis2_soap_envelope_t *envelope,
    axis2_env_t **env)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    /* TODO axis2_soap_header_t *header =
            (SOAPHeader)getFirstChildWithName(
                    new QName(SOAPConstants.HEADER_LOCAL_NAME));
    if (builder == null && header == null) {
        header = factory.createSOAPHeader(this);
        addChild(header);
    }
    */
    return envelope_impl->header;
}

/**
 * Convenience method to add a SOAP header to this envelope
 *
 * @param namespaceURI
 * @param name
 */
axis2_soap_header_block_t* AXIS2_CALL axis2_soap_envelope_add_header(axis2_soap_envelope_t *envelope,
    axis2_env_t **env,
    axis2_char_t *namespace_uri, 
    axis2_char_t *name)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    axis2_om_namespace_t *ns = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    /* TODO : cache SOAP header and envelope instead of looking them up?*/

    if (namespace_uri)
    {
        ns = axis2_om_namespace_create(env, namespace_uri, NULL);
    }
    
    return axis2_soap_header_block_create_with_parent(env, name, ns, envelope_impl->header);
}

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
axis2_soap_body_t* AXIS2_CALL axis2_soap_envelope_get_body(axis2_soap_envelope_t *envelope,
    axis2_env_t **env)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    /* TODO //check for the first element
    OMElement element = getFirstElement();
    if (element != null) {
        if (SOAPConstants.BODY_LOCAL_NAME.equals(element.getLocalName())) {
            return (SOAPBody) element;
        } else {      // if not second element SHOULD be the envelope
            OMNode node = element.getNextOMSibling();
            while (node != null && node.getType() != OMNode.ELEMENT_NODE) {
                node = node.getNextOMSibling();
            }
            element = (OMElement) node;

            if (node != null &&
                    SOAPConstants.BODY_LOCAL_NAME.equals(element.getLocalName())) {
                return (SOAPBody) element;
            } else {
                throw new OMException("SOAPEnvelope must contain a envelope element which is either first or second child element of the SOAPEnvelope.");
            }
        }
    }*/
    return envelope_impl->body;
}

axis2_status_t AXIS2_CALL axis2_soap_envelope_serialize(axis2_soap_envelope_t *envelope,
    axis2_env_t **env,
    axis2_om_output_t *om_output, 
    axis2_bool_t cache)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);

    /* TODO
    if (!omOutput.isIgnoreXMLDeclaration()) {
        axis2_char_t *charSetEncoding = omOutput.getCharSetEncoding();
        axis2_char_t *xmlVersion = omOutput.getXmlVersion();
        omOutput.getXmlStreamWriter().writeStartDocument(charSetEncoding == null ?
                OMConstants.DEFAULT_CHAR_SET_ENCODING : charSetEncoding,
                xmlVersion == null ? OMConstants.DEFAULT_XML_VERSION : xmlVersion);
    }*/
    return AXIS2_OM_NODE_SERIALIZE(envelope_impl->base, env, om_output);
}

axis2_status_t AXIS2_CALL axis2_soap_envelope_set_body(axis2_soap_envelope_t *envelope,
    axis2_env_t **env, axis2_soap_body_t *body)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    if (!(envelope_impl->body))
    {
        envelope_impl->body = body;
    }
    else
    {
        /* TODO set an error here as there can be only one body */
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_soap_envelope_set_header(axis2_soap_envelope_t *envelope,
    axis2_env_t **env, axis2_soap_header_t *header)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    if (!(envelope_impl->header))
    {
        envelope_impl->header = header;
    }
    else
    {
        /* TODO set an error here as there can be only one header */
    }
    return AXIS2_SUCCESS;
}

axis2_om_namespace_t* AXIS2_CALL axis2_soap_envelope_get_namespace(axis2_soap_envelope_t *envelope,
            axis2_env_t **env)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, NULL);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    if (envelope_impl->base)
    {
        axis2_om_element_t *ele = NULL;
        if (AXIS2_OM_NODE_GET_NODE_TYPE(envelope_impl->base, env) == AXIS2_OM_ELEMENT)
        {
            ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(envelope_impl->base, env);
            if (ele)
            {
                return AXIS2_OM_ELEMENT_GET_NAMESPACE(ele, env);
            }
        }
    }
    return NULL;
}
