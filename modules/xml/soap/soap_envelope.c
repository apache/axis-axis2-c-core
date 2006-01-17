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
 #include <axis2_soap_header_block.h>
 #include <axis2_hash.h>
 #include <axis2_soap.h>
 #include <axis2_soap_builder.h>
 
 /******************* impl struct *********************************************/
 
 typedef struct axis2_soap_envelope_impl_t
 {
    axis2_soap_envelope_t soap_envelope;
    
    axis2_om_node_t *om_ele_node;
    
    int soap_version;
    
    axis2_soap_header_t *header;
     
    axis2_soap_body_t *body;
    
    axis2_soap_builder_t *soap_builder;
    
 }axis2_soap_envelope_impl_t;
 
 /****************** Macro ****************************************************/
 
 #define AXIS2_INTF_TO_IMPL(envelope) ((axis2_soap_envelope_impl_t*)envelope)
 
 /****************** Function prototypes **************************************/

axis2_soap_header_t* AXIS2_CALL
 axis2_soap_envelope_get_header(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env);
                                
                                
axis2_soap_header_block_t* AXIS2_CALL 
axis2_soap_envelope_add_header(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env,
                                axis2_char_t *namespace_uri, 
                                axis2_char_t *name);
                                
axis2_soap_body_t* AXIS2_CALL 
axis2_soap_envelope_get_body(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env);
                                
                                
axis2_status_t AXIS2_CALL 
axis2_soap_envelope_serialize(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env,
                                axis2_om_output_t *om_output, 
                                axis2_bool_t cache);
                                
axis2_status_t AXIS2_CALL
axis2_soap_envelope_free(axis2_soap_envelope_t *envelope,
                            axis2_env_t **env);
axis2_om_node_t* AXIS2_CALL 
axis2_soap_envelope_get_base_node(axis2_soap_envelope_t *envelope,
                             axis2_env_t **env);
                             
axis2_status_t AXIS2_CALL
axis2_soap_envelope_set_base_node(axis2_soap_envelope_t *envelope,
                                  axis2_env_t **env,
                                  axis2_om_node_t *om_node);
                                  
int AXIS2_CALL 
axis2_soap_envelope_get_soap_version(axis2_soap_envelope_t *envelope,
                                   axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_soap_envelope_set_soap_version(axis2_soap_envelope_t *envelope,
                                   axis2_env_t **env,
                                   int soap_version);
                                   
axis2_status_t AXIS2_CALL 
axis2_soap_envelope_set_body(axis2_soap_envelope_t *envelope,
                             axis2_env_t **env, 
                             axis2_soap_body_t *body);

axis2_status_t AXIS2_CALL 
axis2_soap_envelope_set_header(axis2_soap_envelope_t *envelope,
                               axis2_env_t **env, 
                               axis2_soap_header_t *header);
                               
axis2_om_namespace_t* AXIS2_CALL 
axis2_soap_envelope_get_namespace(axis2_soap_envelope_t *envelope,
                                  axis2_env_t **env);
                                  
axis2_status_t AXIS2_CALL
axis2_soap_envelope_set_builder(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env,
                                axis2_soap_builder_t *soap_builder);                                  

                                   
/*************** function implementations *************************************/

AXIS2_DECLARE(axis2_soap_envelope_t*)
axis2_soap_envelope_create_null(axis2_env_t **env)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
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
    envelope_impl->om_ele_node = NULL;
    envelope_impl->soap_version = AXIS2_SOAP12;    
    envelope_impl->header = NULL;
    envelope_impl->body = NULL;
    envelope_impl->soap_builder = NULL;
    
    envelope_impl->soap_envelope.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_soap_envelope_ops_t) );
    if (!envelope_impl->soap_envelope.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_soap_envelope_free(&(envelope_impl->soap_envelope), env);
        return NULL;        
    }
   
    envelope_impl->soap_envelope.ops->free = axis2_soap_envelope_free;
    
    envelope_impl->soap_envelope.ops->get_base_node = 
            axis2_soap_envelope_get_base_node;
    envelope_impl->soap_envelope.ops->set_base_node = 
            axis2_soap_envelope_set_base_node;
    envelope_impl->soap_envelope.ops->get_soap_version = 
            axis2_soap_envelope_get_soap_version;
    envelope_impl->soap_envelope.ops->set_soap_version = 
            axis2_soap_envelope_set_soap_version;
    envelope_impl->soap_envelope.ops->get_body = 
            axis2_soap_envelope_get_body;
    envelope_impl->soap_envelope.ops->set_body = 
            axis2_soap_envelope_set_body;
    
    envelope_impl->soap_envelope.ops->get_header = 
            axis2_soap_envelope_get_header;
   /* envelope_impl->soap_envelope.ops->add_header = axis2_soap_envelope_add_header; */
    envelope_impl->soap_envelope.ops->set_header = 
            axis2_soap_envelope_set_header;
    envelope_impl->soap_envelope.ops->get_namespace = 
            axis2_soap_envelope_get_namespace;      
              
    envelope_impl->soap_envelope.ops->serialize = 
            axis2_soap_envelope_serialize;    
    
    
    
    
    return &(envelope_impl->soap_envelope);        
}


AXIS2_DECLARE(axis2_soap_envelope_t*)
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
    envelope_impl->om_ele_node = NULL;
    envelope_impl->soap_version = AXIS2_SOAP12;    
    envelope_impl->header = NULL;
    envelope_impl->body = NULL;
    envelope_impl->soap_builder =  NULL;
    
    ele = axis2_om_element_create(env, NULL, AXIS2_SOAP_ENVELOPE_LOCAL_NAME, ns, &(envelope_impl->om_ele_node));
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
    
    envelope_impl->soap_envelope.ops->get_header = 
        axis2_soap_envelope_get_header;
  /*  envelope_impl->soap_envelope.ops->add_header = axis2_soap_envelope_add_header; */
    envelope_impl->soap_envelope.ops->get_body = 
        axis2_soap_envelope_get_body;
    envelope_impl->soap_envelope.ops->serialize = 
        axis2_soap_envelope_serialize;    
    envelope_impl->soap_envelope.ops->free = 
        axis2_soap_envelope_free;
    envelope_impl->soap_envelope.ops->get_base_node = 
        axis2_soap_envelope_get_base_node;
    envelope_impl->soap_envelope.ops->get_soap_version = 
        axis2_soap_envelope_get_soap_version;
    envelope_impl->soap_envelope.ops->set_soap_version = 
        axis2_soap_envelope_set_soap_version;
    envelope_impl->soap_envelope.ops->get_namespace = 
        axis2_soap_envelope_get_namespace;
    envelope_impl->soap_envelope.ops->set_builder =
        axis2_soap_envelope_set_builder;
        
        
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
    
    if(envelope_impl->om_ele_node)
    {
        AXIS2_OM_NODE_FREE_TREE(envelope_impl->om_ele_node, env);
        envelope_impl->om_ele_node = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, envelope_impl);
    envelope_impl = NULL;
    return AXIS2_SUCCESS;
}
    
axis2_om_node_t* AXIS2_CALL 
axis2_soap_envelope_get_base_node(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(envelope, env, NULL);
    return AXIS2_INTF_TO_IMPL(envelope)->om_ele_node;
}

axis2_status_t AXIS2_CALL 
axis2_soap_envelope_set_base_node(axis2_soap_envelope_t *envelope,
                                  axis2_env_t **env,
                                  axis2_om_node_t *node)
{
   axis2_soap_envelope_impl_t *envelope_impl = NULL;
   AXIS2_FUNC_PARAM_CHECK( envelope, env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   
   envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   envelope_impl->om_ele_node = node;
   return AXIS2_SUCCESS;
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
    axis2_qname_t *header_qn = NULL;
    axis2_om_node_t *header_node = NULL;
    axis2_om_element_t *header_ele = NULL;
    axis2_om_element_t *envelope_ele = NULL;
    
    
    AXIS2_FUNC_PARAM_CHECK(envelope, env, NULL);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    if(envelope_impl->header)
    {
        return envelope_impl->header;
    }        
    else
    {
        envelope_impl->header = axis2_soap_header_create(env);
        envelope_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                            envelope_impl->om_ele_node, env);
        header_qn = axis2_qname_create(env, AXIS2_SOAP_HEADER_LOCAL_NAME, NULL, NULL);
        if(!header_qn)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }                            
        header_ele = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(envelope_ele,
                         env, header_qn, envelope_impl->om_ele_node, &header_node); 
                         
        AXIS2_SOAP_HEADER_SET_BASE_NODE(envelope_impl->header, env, header_node);
        if(envelope_impl->soap_version == AXIS2_SOAP11)
        {
            AXIS2_SOAP_HEADER_SET_SOAP_VERSION(envelope_impl->header, env, AXIS2_SOAP11);
        }
        else if(envelope_impl->soap_version == AXIS2_SOAP12)
        {
            AXIS2_SOAP_HEADER_SET_SOAP_VERSION(envelope_impl->header, env, AXIS2_SOAP11);
        }
        if(envelope_impl->header)
            return envelope_impl->header;                         
    }            
    if(!(envelope_impl->header))
    {
        envelope_impl->header = axis2_soap_header_create_with_parent(env, envelope);
        if(envelope_impl->soap_version == AXIS2_SOAP12)
        {
            AXIS2_SOAP_HEADER_SET_SOAP_VERSION(envelope_impl->header, env, AXIS2_SOAP12);
        }
        else if(envelope_impl->soap_version == AXIS2_SOAP11)
        {
            AXIS2_SOAP_HEADER_SET_SOAP_VERSION(envelope_impl->header, env, AXIS2_SOAP12);
        }
    }
    return envelope_impl->header;
}

/**
 * Convenience method to add a SOAP header to this envelope
 *
 * @param namespaceURI
 * @param name
 */
axis2_soap_header_block_t* AXIS2_CALL 
axis2_soap_envelope_add_header(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env,
                                axis2_char_t *namespace_uri, 
                                axis2_char_t *name)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    axis2_om_namespace_t *ns = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, NULL);
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
axis2_soap_body_t* AXIS2_CALL 
axis2_soap_envelope_get_body(axis2_soap_envelope_t *envelope,
                            axis2_env_t **env)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    axis2_om_element_t *envelope_ele = NULL;
    axis2_om_node_t *first_node = NULL;
    axis2_om_element_t *first_ele = NULL;
    
    axis2_om_node_t *next_node = NULL;
    axis2_om_element_t *next_ele = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(envelope, env, NULL);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    
    
    if(envelope_impl->body)
    {
        return envelope_impl->body;
    }
    
    
    envelope_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        envelope_impl->om_ele_node, env);
                            
    
    first_ele = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(envelope_ele, 
                        env, envelope_impl->om_ele_node, &first_node);
    if(first_ele)
    {
        if(AXIS2_STRCMP(AXIS2_SOAP_BODY_LOCAL_NAME, 
                AXIS2_OM_ELEMENT_GET_LOCALNAME(first_ele, env)) == 0)
        {
            envelope_impl->body = axis2_soap_body_create(env);
            AXIS2_SOAP_BODY_SET_BASE_NODE(envelope_impl->body, env, first_node);
            AXIS2_SOAP_BODY_SET_SOAP_VERSION(envelope_impl->body, env, envelope_impl->soap_version);
            return envelope_impl->body;
        }
        else
        {
            next_node = AXIS2_OM_NODE_GET_NEXT_SIBLING( first_node, env);
         
            while(next_node && AXIS2_OM_NODE_GET_NODE_TYPE(next_node , env) != AXIS2_OM_ELEMENT)
            {
                next_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_node , env);
            }
            next_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(next_node, env);
           
            if(next_ele && AXIS2_STRCMP(AXIS2_SOAP_BODY_LOCAL_NAME, 
                    AXIS2_OM_ELEMENT_GET_LOCALNAME(next_ele, env)) == 0)
            {
                envelope_impl->body = axis2_soap_body_create(env);
                AXIS2_SOAP_BODY_SET_BASE_NODE(envelope_impl->body, env, next_node);
                AXIS2_SOAP_BODY_SET_SOAP_VERSION(envelope_impl->body, env, envelope_impl->soap_version);
                return envelope_impl->body;
            }                    
            else
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOAP_ENVELOPE_MUST_HAVE_BODY_ELEMENT, AXIS2_FAILURE);
                return NULL;            
            }
        }               
    
    }
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
    return AXIS2_OM_NODE_SERIALIZE(envelope_impl->om_ele_node, env, om_output);
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
    
    if (envelope_impl->om_ele_node)
    {
        axis2_om_element_t *ele = NULL;
        if (AXIS2_OM_NODE_GET_NODE_TYPE(envelope_impl->om_ele_node, env) == AXIS2_OM_ELEMENT)
        {
            ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(envelope_impl->om_ele_node, env);
            if (ele)
            {
                return AXIS2_OM_ELEMENT_GET_NAMESPACE(ele, env);
            }
        }
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_soap_envelope_set_builder(axis2_soap_envelope_t *envelope,
                                axis2_env_t **env,
                                axis2_soap_builder_t *soap_builder)
{
    axis2_soap_envelope_impl_t *envelope_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(envelope, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_builder, AXIS2_FAILURE);
    envelope_impl = AXIS2_INTF_TO_IMPL(envelope);
    envelope_impl->soap_builder = soap_builder;
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_soap_envelope_t *)
axis2_soap_envelope_create_default_soap_envelope(axis2_env_t **env,
                                                 int soap_version)
{
    axis2_soap_envelope_t *soap_env = NULL;
    axis2_soap_header_t *soap_header = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_om_namespace_t *om_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    if(soap_version == AXIS2_SOAP11)
    {
        om_ns = axis2_om_namespace_create(env,
            AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, 
            AXIS2_SOAP_DEFAULT_NAMESPACE_PREFIX);
        if(!om_ns)
            return NULL;    
        soap_env = axis2_soap_envelope_create(env, om_ns);
        AXIS2_SOAP_ENVELOPE_SET_SOAP_VERSION(soap_env, env, AXIS2_SOAP11);
        soap_header = axis2_soap11_header_create_with_parent(env, soap_env);
        soap_body   = axis2_soap_body_create_with_parent(env, soap_env);
        AXIS2_SOAP_BODY_SET_SOAP_VERSION(soap_body, env, AXIS2_SOAP11);
        return soap_env;    
    }
    else if(soap_version == AXIS2_SOAP12)
    {
        om_ns = axis2_om_namespace_create(env,
            AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, 
            AXIS2_SOAP_DEFAULT_NAMESPACE_PREFIX);
        if(!om_ns)
            return NULL;    
        soap_env = axis2_soap_envelope_create(env, om_ns);
        AXIS2_SOAP_ENVELOPE_SET_SOAP_VERSION(soap_env, env, AXIS2_SOAP12);
        soap_header = axis2_soap12_header_create_with_parent(env, soap_env);
        soap_body   = axis2_soap_body_create_with_parent(env, soap_env);
        AXIS2_SOAP_BODY_SET_SOAP_VERSION(soap_body, env, AXIS2_SOAP12);
        return soap_env;    
    }
    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SOAP_VERSION, AXIS2_FAILURE);
    return NULL;
}                                                 