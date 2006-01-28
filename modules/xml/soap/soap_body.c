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
 #include <axis2_hash.h>
 #include <axis2_soap.h>
 #include <axis2_soap_builder.h>
 
 /******************* impl struct *********************************************/
 
 typedef struct axis2_soap_body_impl_t
 {
    axis2_soap_body_t soap_body;
    
    axis2_om_node_t *om_ele_node;
    
    int soap_version;
    
    axis2_bool_t has_fault;
    
    axis2_soap_fault_t *soap_fault;
    
    axis2_soap_builder_t *soap_builder;
    
 }axis2_soap_body_impl_t;
 
 /****************** Macro ****************************************************/
 
 #define AXIS2_INTF_TO_IMPL(body) ((axis2_soap_body_impl_t*)body)
 
 /****************** Function prototypes **************************************/

axis2_status_t AXIS2_CALL
axis2_soap_body_free(axis2_soap_body_t *body,
                       axis2_env_t **env);
                       
axis2_bool_t AXIS2_CALL 
axis2_soap_body_has_fault(axis2_soap_body_t *body,
                              axis2_env_t **env);
                              
axis2_soap_fault_t* AXIS2_CALL 
axis2_soap_body_get_fault(axis2_soap_body_t *body,
                          axis2_env_t **env);
                          
axis2_status_t AXIS2_CALL 
axis2_soap_body_add_fault(axis2_soap_body_t *body,
                          axis2_env_t **env,
                          axis2_soap_fault_t *soap_fault);
                          
axis2_om_node_t* AXIS2_CALL 
axis2_soap_body_get_base_node(axis2_soap_body_t *body,
                              axis2_env_t **env);
                              
axis2_status_t AXIS2_CALL
axis2_soap_body_set_base_node(axis2_soap_body_t *body,
                              axis2_env_t **env,
                              axis2_om_node_t *om_node);                              
                         
int AXIS2_CALL 
axis2_soap_body_get_soap_version(axis2_soap_body_t *body,
                                 axis2_env_t **env);
                                 
                                 
axis2_status_t AXIS2_CALL 
axis2_soap_body_set_soap_version(axis2_soap_body_t *body,
                                 axis2_env_t **env,
                                 int soap_version);
                                 
axis2_status_t AXIS2_CALL 
axis2_soap_body_set_builder(axis2_soap_body_t *body,
                            axis2_env_t **env,
                            axis2_soap_builder_t *builder);   

axis2_status_t AXIS2_CALL
axis2_soap_body_build(axis2_soap_body_t *body,
                      axis2_env_t **env);
                      
axis2_status_t AXIS2_CALL 
axis2_soap_body_set_fault(axis2_soap_body_t *body,
                          axis2_env_t **env,
                          axis2_soap_fault_t *soap_fault);                      

/*************** function implementations *************************************/
axis2_soap_body_t* AXIS2_CALL
axis2_soap_body_create(axis2_env_t **env)
{
    axis2_soap_body_impl_t *body_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    body_impl = (axis2_soap_body_impl_t*)AXIS2_MALLOC(
                (*env)->allocator,
                sizeof(axis2_soap_body_impl_t));
                
    if(!body_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    body_impl->soap_body.ops = NULL;
    body_impl->om_ele_node = NULL;
    body_impl->soap_builder = NULL;
    body_impl->soap_version = AXIS2_SOAP_VERSION_NOT_SET;    
    body_impl->has_fault = AXIS2_FALSE;
    body_impl->soap_fault = NULL; 

    body_impl->soap_body.ops = (axis2_soap_body_ops_t*) AXIS2_MALLOC(
                               (*env)->allocator, sizeof(axis2_soap_body_ops_t));

    if(!(body_impl->soap_body.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, body_impl);
        body_impl = NULL;
        return NULL;
    }

    body_impl->soap_body.ops->free = axis2_soap_body_free;
    body_impl->soap_body.ops->has_fault = axis2_soap_body_has_fault;
    body_impl->soap_body.ops->get_fault = axis2_soap_body_get_fault;
    /*body_impl->soap_body.ops->add_fault = axis2_soap_body_add_fault; */
    body_impl->soap_body.ops->get_base_node = axis2_soap_body_get_base_node;
    body_impl->soap_body.ops->set_base_node = axis2_soap_body_set_base_node;
    body_impl->soap_body.ops->get_soap_version = axis2_soap_body_get_soap_version;
    body_impl->soap_body.ops->set_soap_version = axis2_soap_body_set_soap_version;
    body_impl->soap_body.ops->set_builder = axis2_soap_body_set_builder;
    body_impl->soap_body.ops->build = axis2_soap_body_build;
    body_impl->soap_body.ops->set_fault = axis2_soap_body_set_fault;
    return &(body_impl->soap_body);
return NULL;
}



axis2_soap_body_t* AXIS2_CALL
axis2_soap_body_create_with_parent(axis2_env_t **env, 
                                   axis2_soap_envelope_t *envelope)
{
    axis2_soap_body_impl_t *body_impl = NULL;
    axis2_soap_body_t *body = NULL;
    axis2_om_element_t *ele = NULL;
    axis2_om_node_t *parent = NULL;
    axis2_om_element_t *parent_ele =  NULL;
    axis2_om_namespace_t *om_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, envelope, AXIS2_FAILURE);
    
    body = axis2_soap_body_create(env);
    if(!body)
    {
        return NULL;   
    }
    body_impl = AXIS2_INTF_TO_IMPL(body);
   
    
    /*get parent node from SOAP envelope */
    if (envelope)
    {
        parent = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env);
        if(parent)
        {
            parent_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent, env);
        }            
    }
    if(parent_ele)
        om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    
    ele = axis2_om_element_create(env, parent, 
                                  AXIS2_SOAP_BODY_LOCAL_NAME, om_ns, 
                                  &(body_impl->om_ele_node));
    if (!ele)
    {
        printf(" element null");
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_soap_body_free(&(body_impl->soap_body), env);
        return NULL;
    }
     return &(body_impl->soap_body);           
}

axis2_status_t AXIS2_CALL
axis2_soap_body_free(axis2_soap_body_t *body,
                       axis2_env_t **env)
{
    axis2_soap_body_impl_t *body_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(body, env, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if(body_impl->soap_fault)
    {
        AXIS2_SOAP_FAULT_FREE(body_impl->soap_fault, env);
        body_impl->soap_fault = NULL;        
    }
    if(body->ops)
    {
        AXIS2_FREE((*env)->allocator, body->ops);
        body->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, body_impl);
    body_impl = NULL;
    return AXIS2_SUCCESS;
}
    
/**
 * Indicates whether a <code>SOAPFault</code> object exists in
 * this <code>SOAPBody</code> object.
 *
 * @return <code>true</code> if a <code>SOAPFault</code> object exists in
 *         this <code>SOAPBody</code> object; <code>false</code>
 *         otherwise
 */
axis2_bool_t AXIS2_CALL axis2_soap_body_has_fault(axis2_soap_body_t *body,
    axis2_env_t **env) 
{
    axis2_soap_body_impl_t *body_impl = NULL;
    axis2_om_node_t *fault_node = NULL;
    axis2_om_element_t *fault_ele = NULL;
    axis2_om_element_t *soap_body_ele = NULL;
    AXIS2_FUNC_PARAM_CHECK(body, env, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if(body_impl->has_fault)
        return body_impl->has_fault;
    else
    {
        soap_body_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(body_impl->om_ele_node, env);
        if(soap_body_ele)
        {
            axis2_char_t* localname = NULL;
            axis2_om_namespace_t *om_ns = NULL;
            axis2_char_t* namespace_uri = NULL;
            fault_ele = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT( soap_body_ele, env, body_impl->om_ele_node, &fault_node);
            if (fault_ele)
            {
                localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(fault_ele, env);
                om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(fault_ele, env);
                if (om_ns)
                    namespace_uri = AXIS2_OM_NAMESPACE_GET_URI(om_ns, env);
            
                if(localname && namespace_uri && fault_ele && AXIS2_STRCMP(AXIS2_SOAP_FAULT_LOCAL_NAME, localname) == 0 &&
                       (AXIS2_STRCMP(AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, namespace_uri) == 0 ||   
                         AXIS2_STRCMP(AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, namespace_uri) == 0))
                 {
                    body_impl->has_fault = AXIS2_TRUE;
                    return body_impl->has_fault;
                 }   
             }
        }
    }
    return AXIS2_FALSE;
}

/**
 * Returns the <code>SOAPFault</code> object in this <code>SOAPBody</code>
 * object.
 *
 * @return the <code>SOAPFault</code> object in this <code>SOAPBody</code>
 *         object
 */
axis2_soap_fault_t* AXIS2_CALL axis2_soap_body_get_fault(axis2_soap_body_t *body,
    axis2_env_t **env) 
{
    axis2_soap_body_impl_t *body_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(body, env, NULL);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if(body_impl->has_fault)
    {
        return body_impl->soap_fault;
    }
    else if(body_impl->soap_builder != NULL)
    {
        while(!(body_impl->soap_fault) && !(AXIS2_OM_NODE_GET_BUILD_STATUS(body_impl->om_ele_node, env)))
        {
            int status = AXIS2_SUCCESS;
            status = AXIS2_SOAP_BUILDER_NEXT(body_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                return NULL;
        }
    }
    
    /*
    else
    {
        axis2_om_node_t *first_node = NULL;
        axis2_om_element_t *first_ele = NULL;
        axis2_om_namespace_t *om_ns = NULL;
        axis2_char_t *ns_uri = NULL;
        axis2_char_t *localname = NULL;        
        first_node = AXIS2_OM_NODE_GET_FIRST_CHILD(body_impl->om_ele_node, env);
        if(first_node)
        {
                
            first_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(first_node, env);
            localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(first_ele, env);
            om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(first_ele, env);
            ns_uri = AXIS2_OM_NAMESPACE_GET_URI(om_ns, env);
            if(first_ele && AXIS2_STRCMP(AXIS2_SOAP_FAULT_LOCAL_NAME, localname) == 0 &&
                (AXIS2_STRCMP(ns_uri, AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0 ||
                 AXIS2_STRCMP(ns_uri, AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) == 0))
            {
                body_impl->has_fault = AXIS2_TRUE;
                body_impl->soap_fault = axis2_soap_fault_create(env);
                AXIS2_SOAP_FAULT_SET_SOAP_VERSION(body_impl->soap_fault, 
                        env, body_impl->soap_version);
                AXIS2_SOAP_FAULT_SET_BASE_NODE(body_impl->soap_fault, env, first_node);
                return body_impl->soap_fault;                            
            }                 
        }
    }
    */
    return NULL;
}

/**
 * @param soapFault
 * @throws org.apache.axis2.om.OMException
 *
 * @throws OMException
 */
axis2_status_t AXIS2_CALL 
axis2_soap_body_add_fault(axis2_soap_body_t *body,
                          axis2_env_t **env,
                          axis2_soap_fault_t *soap_fault)
{
    axis2_soap_body_impl_t *body_impl = NULL;
    axis2_om_node_t *fault_node = NULL;
    AXIS2_FUNC_PARAM_CHECK(body, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_fault, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    
    if (body_impl->has_fault) 
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    fault_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(soap_fault, env);
    if(fault_node)
    {
        body_impl->soap_fault = soap_fault;
        AXIS2_OM_NODE_ADD_CHILD(fault_node , env, (body_impl->om_ele_node));
        body_impl->has_fault = AXIS2_TRUE;
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;            
}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_body_get_base_node(axis2_soap_body_t *body,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(body, env, NULL);
    return AXIS2_INTF_TO_IMPL(body)->om_ele_node;
}

axis2_status_t AXIS2_CALL
axis2_soap_body_set_base_node(axis2_soap_body_t *body,
                              axis2_env_t **env,
                              axis2_om_node_t *node)
{
   axis2_soap_body_impl_t *body_impl = NULL;
   AXIS2_FUNC_PARAM_CHECK( body, env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   body_impl = AXIS2_INTF_TO_IMPL(body);
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   body_impl->om_ele_node = node;
   return AXIS2_SUCCESS;
}                              
                              
                                             
int AXIS2_CALL 
axis2_soap_body_get_soap_version(axis2_soap_body_t *body,
                                   axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(body, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(body)->soap_version;    
}
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_body_set_soap_version(axis2_soap_body_t *body,
                                   axis2_env_t **env,
                                   int soap_version)
{
    AXIS2_FUNC_PARAM_CHECK(body, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(body)->soap_version = soap_version;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_body_set_builder(axis2_soap_body_t *body,
                            axis2_env_t **env,
                            axis2_soap_builder_t *builder)
{
    axis2_soap_body_impl_t *body_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    body_impl->soap_builder = builder;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_soap_body_build(axis2_soap_body_t *body,
                      axis2_env_t **env)
{
    axis2_soap_body_impl_t *soap_body_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(body, env, AXIS2_FAILURE);
    soap_body_impl = AXIS2_INTF_TO_IMPL(body);
    if(soap_body_impl->om_ele_node && soap_body_impl->soap_builder)
    {
        while(!AXIS2_OM_NODE_GET_BUILD_STATUS(soap_body_impl->om_ele_node, env))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(soap_body_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;                
        }
    }            
    return AXIS2_SUCCESS;
}
axis2_status_t AXIS2_CALL 
axis2_soap_body_set_fault(axis2_soap_body_t *body,
                          axis2_env_t **env,
                          axis2_soap_fault_t *soap_fault)
{
    axis2_soap_body_impl_t *body_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, soap_fault, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (body_impl->has_fault) 
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY, AXIS2_FAILURE);
        AXIS2_LOG_WRITE((*env)->log, 
            "AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY", AXIS2_LOG_DEBUG);
        return AXIS2_FAILURE;
    }
    body_impl->soap_fault = soap_fault;
    return AXIS2_SUCCESS;
}
