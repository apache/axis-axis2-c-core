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
 
#include <axis2_soap.h> 
#include <axis2_soap_header_block.h>
#include <axis2_soap_header.h>
#include <axis2_soap_envelope.h>
#include <axis2_soap_builder.h>

/******************* impl struct **********************************************/

typedef struct axis2_soap_header_block_impl_t
{
    axis2_soap_header_block_t header_block;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_om_element_t *om_ele;
    
    int soap_version;
    
    axis2_soap_header_t *parent;

    axis2_bool_t processed;
    
    
}axis2_soap_header_block_impl_t;

/***************** Macro ******************************************************/

#define AXIS2_INTF_TO_IMPL(hblock) ((axis2_soap_header_block_impl_t*)hblock) 

/***************** function prototypes ****************************************/

axis2_status_t AXIS2_CALL 
axis2_soap_header_block_free(axis2_soap_header_block_t *header_block,
                             axis2_env_t **env);
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_role(axis2_soap_header_block_t *header_block,
                                 axis2_env_t **env,
                                 axis2_char_t *uri);  

axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_must_understand_with_bool
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         axis2_bool_t *must_understand);
                        
axis2_status_t AXIS2_CALL
axis2_soap_header_block_set_must_understand_with_string
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         axis2_char_t *must_understand);
                                        
axis2_bool_t AXIS2_CALL 
axis2_soap_header_block_get_must_understand
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env);
                        
                        
axis2_bool_t AXIS2_CALL 
axis2_soap_header_block_is_processed
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env);
                        
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_processed
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env);                                                                                                                                                                                    
                                                                                                                            
                            
axis2_char_t* AXIS2_CALL 
axis2_soap_header_block_get_role
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env);
                                                                                    
axis2_status_t AXIS2_CALL axis2_soap_header_block_set_base_node
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         axis2_om_node_t *node);
                                        
axis2_om_node_t* AXIS2_CALL 
axis2_soap_header_block_get_base_node
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env);
                                                                                                                                                                                
int AXIS2_CALL 
axis2_soap_header_block_get_soap_version  
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_soap_version  
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         int soap_version); 
                         
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_attribute
                       (axis2_soap_header_block_t *header_block,
                        axis2_env_t **env,
                        axis2_char_t *attr_name,
                        axis2_char_t *attr_value,
                        axis2_char_t *soap_envelope_namespace_uri);
                        
axis2_char_t* AXIS2_CALL 
axis2_soap_header_block_get_attribute
                       (axis2_soap_header_block_t *header_block,
                        axis2_env_t **env,
                        axis2_char_t *attr_name,
                        axis2_char_t *soap_envelope_namespace_uri);  
                        
                         
/*************** function implementations *************************************/                                                                                             

AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap_header_block_create(axis2_env_t **env)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    header_block_impl = (axis2_soap_header_block_impl_t*)AXIS2_MALLOC(
                         (*env)->allocator, 
                         sizeof(axis2_soap_header_block_impl_t));
    if(!header_block_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);    
        return NULL;
    }                         
    
    header_block_impl->header_block.ops = NULL;
    header_block_impl->om_ele           = NULL;
    header_block_impl->om_ele_node      = NULL;
    header_block_impl->parent           = NULL;
    header_block_impl->soap_version     = AXIS2_SOAP_VERSION_NOT_SET;
    
    header_block_impl->header_block.ops = (axis2_soap_header_block_ops_t *)AXIS2_MALLOC(
                                            (*env)->allocator,
                                            sizeof(axis2_soap_header_block_ops_t));
    if(!(header_block_impl->header_block.ops))
    {
        AXIS2_FREE((*env)->allocator, header_block_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    
    header_block_impl->header_block.ops->free_fn =
        axis2_soap_header_block_free;
    header_block_impl->header_block.ops->set_role =
        axis2_soap_header_block_set_role;
    header_block_impl->header_block.ops->set_must_understand_with_bool =
        axis2_soap_header_block_set_must_understand_with_bool;
    header_block_impl->header_block.ops->set_must_understand_with_string =
        axis2_soap_header_block_set_must_understand_with_string;                                                                   
    header_block_impl->header_block.ops->get_must_understand =
        axis2_soap_header_block_get_must_understand;
    header_block_impl->header_block.ops->is_processed =
        axis2_soap_header_block_is_processed;
    header_block_impl->header_block.ops->set_processed =
        axis2_soap_header_block_set_processed;
    header_block_impl->header_block.ops->get_role =
        axis2_soap_header_block_get_role;
    header_block_impl->header_block.ops->set_soap_version =
        axis2_soap_header_block_set_soap_version;
    header_block_impl->header_block.ops->get_soap_version =
        axis2_soap_header_block_get_soap_version;
    header_block_impl->header_block.ops->set_base_node =
        axis2_soap_header_block_set_base_node;
    header_block_impl->header_block.ops->get_base_node =
        axis2_soap_header_block_get_base_node;                                                        

    return &(header_block_impl->header_block);
}


AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap_header_block_create_with_parent(axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_om_namespace_t *ns,
                                           axis2_soap_header_t *header)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_soap_header_block_t *header_block = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *parent_node = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, localname, NULL);
    
    header_block = axis2_soap_header_block_create(env);
    if(!header_block)
        return NULL;   
    
    parent_node = AXIS2_SOAP_HEADER_GET_BASE_NODE(header, env);    
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    header_block_impl->om_ele = axis2_om_element_create( env,
                                    parent_node, localname, ns , &this_node);
                                    
    header_block_impl->om_ele_node = this_node; 
    header_block_impl->parent = header;               
    return &(header_block_impl->header_block);                                
}

axis2_status_t AXIS2_CALL 
axis2_soap_header_block_free(axis2_soap_header_block_t *header_block,
                             axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    if(header_block->ops)
    {
        AXIS2_FREE((*env)->allocator, header_block->ops);
        header_block->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(header_block));
    header_block = NULL;
    return AXIS2_SUCCESS;
}
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_role(axis2_soap_header_block_t *header_block,
                                 axis2_env_t **env,
                                 axis2_char_t *role_uri)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_char_t *attr_localname = NULL;
    axis2_char_t *attr_nsuri     = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    
    if(header_block_impl->soap_version == AXIS2_SOAP_VERSION_NOT_SET)
        return AXIS2_FAILURE;
    if(header_block_impl->soap_version == AXIS2_SOAP11)
    {
        attr_localname = AXIS2_SOAP11_ATTR_ACTOR;
        attr_nsuri     = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(header_block_impl->soap_version == AXIS2_SOAP12)
    {
        attr_localname = AXIS2_SOAP12_SOAP_ROLE;
        attr_nsuri     = AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }        
    axis2_soap_header_block_set_attribute(header_block, 
                        env, attr_localname, role_uri, attr_nsuri);    
    return AXIS2_SUCCESS;    
}

axis2_char_t* AXIS2_CALL 
axis2_soap_header_block_get_role
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_char_t *attr_localname = NULL;
    axis2_char_t *attr_nsuri     = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, NULL);
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    
    if(header_block_impl->soap_version == AXIS2_SOAP_VERSION_NOT_SET)
        return NULL;
    if(header_block_impl->soap_version == AXIS2_SOAP11)
    {
        attr_localname = AXIS2_SOAP11_ATTR_ACTOR;
        attr_nsuri     = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(header_block_impl->soap_version == AXIS2_SOAP12)
    {
        attr_localname = AXIS2_SOAP12_SOAP_ROLE;
        attr_nsuri     = AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }        
    return axis2_soap_header_block_get_attribute(header_block, env,
                attr_localname, attr_nsuri);
}  

axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_must_understand_with_bool
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         axis2_bool_t *must_understand)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_char_t *attr_nsuri     = NULL;
    axis2_char_t* attr_value = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    if(header_block_impl->soap_version == AXIS2_SOAP_VERSION_NOT_SET)
        return AXIS2_FAILURE;
    if(header_block_impl->soap_version == AXIS2_SOAP11)
    {
        attr_nsuri     = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(header_block_impl->soap_version == AXIS2_SOAP12)
    {
        attr_nsuri     = AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(must_understand)
    {
         attr_value = "1";
    }
    else
    {
        attr_value = "0";
    }
    return axis2_soap_header_block_set_attribute(header_block, env, 
                AXIS2_SOAP_ATTR_MUST_UNDERSTAND, attr_value, attr_nsuri);
            
}
                        
axis2_status_t AXIS2_CALL
axis2_soap_header_block_set_must_understand_with_string
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         axis2_char_t *must_understand)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_char_t *attr_nsuri     = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, must_understand, AXIS2_FAILURE);
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    if(header_block_impl->soap_version == AXIS2_SOAP_VERSION_NOT_SET)
        return AXIS2_FAILURE;
    if(header_block_impl->soap_version == AXIS2_SOAP11)
    {
        attr_nsuri     = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(header_block_impl->soap_version == AXIS2_SOAP12)
    {
        attr_nsuri     = AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(AXIS2_STRCMP(AXIS2_SOAP_ATTR_MUST_UNDERSTAND_TRUE, must_understand) == 0 ||
       AXIS2_STRCMP(AXIS2_SOAP_ATTR_MUST_UNDERSTAND_FALSE, must_understand) == 0 ||
       AXIS2_STRCMP(AXIS2_SOAP_ATTR_MUST_UNDERSTAND_0, must_understand) == 0 ||
       AXIS2_STRCMP(AXIS2_SOAP_ATTR_MUST_UNDERSTAND_1, must_understand) == 0)
    {        

        axis2_soap_header_block_set_attribute(header_block, env, 
                AXIS2_SOAP_ATTR_MUST_UNDERSTAND, must_understand, attr_nsuri);
        return AXIS2_SUCCESS;                
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_MUST_UNDERSTAND_SHOULD_BE_1_0_TRUE_FALSE, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }
}
                                        
axis2_bool_t AXIS2_CALL 
axis2_soap_header_block_get_must_understand
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env)
{
    axis2_char_t *must_understand = NULL;
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_char_t *attr_nsuri     = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);

    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    if(header_block_impl->soap_version == AXIS2_SOAP_VERSION_NOT_SET)
        return AXIS2_FAILURE;
    if(header_block_impl->soap_version == AXIS2_SOAP11)
    {
        attr_nsuri     = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    if(header_block_impl->soap_version == AXIS2_SOAP12)
    {
        attr_nsuri     = AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    must_understand = axis2_soap_header_block_get_attribute(header_block,
            env, AXIS2_SOAP_ATTR_MUST_UNDERSTAND, attr_nsuri);
    if(AXIS2_STRCMP(must_understand, AXIS2_SOAP_ATTR_MUST_UNDERSTAND_1) == 0 ||            
       AXIS2_STRCMP(must_understand, AXIS2_SOAP_ATTR_MUST_UNDERSTAND_TRUE) == 0)
    {
        return AXIS2_TRUE;
    }       
    else if(AXIS2_STRCMP(must_understand, AXIS2_SOAP_ATTR_MUST_UNDERSTAND_0) == 0 ||            
       AXIS2_STRCMP(must_understand, AXIS2_SOAP_ATTR_MUST_UNDERSTAND_FALSE) == 0)
    {
        return AXIS2_FALSE;
    }
    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_VALUE_FOUND_IN_MUST_UNDERSTAND,
        AXIS2_FAILURE);
    return AXIS2_FALSE;        
}
                        
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_attribute
                       (axis2_soap_header_block_t *header_block,
                        axis2_env_t **env,
                        axis2_char_t *attr_name,
                        axis2_char_t *attr_value,
                        axis2_char_t *soap_envelope_namespace_uri)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_om_attribute_t* om_attr = NULL;
    axis2_qname_t *qn = NULL;
    axis2_om_namespace_t *om_ns = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_name, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_envelope_namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_value, AXIS2_FAILURE);
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    
    qn = axis2_qname_create(env, attr_name, soap_envelope_namespace_uri, NULL);
    if(!qn)
        return AXIS2_FAILURE;
    om_attr = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(header_block_impl->om_ele, env, qn);
    if(om_attr)
    {
       AXIS2_OM_ATTRIBUTE_SET_VALUE(om_attr, env, attr_value);
    }
    else
    {
        om_ns = axis2_om_namespace_create(env, 
                                          soap_envelope_namespace_uri,
                                          AXIS2_SOAP_DEFAULT_NAMESPACE_PREFIX);
                                            
        om_attr = axis2_om_attribute_create(env, attr_name, attr_value, om_ns);
        AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(header_block_impl->om_ele, 
                                env, om_attr, header_block_impl->om_ele_node);
    }
    AXIS2_QNAME_FREE(qn, env);
    return AXIS2_SUCCESS;    
}
                        
axis2_char_t* AXIS2_CALL 
axis2_soap_header_block_get_attribute
                       (axis2_soap_header_block_t *header_block,
                        axis2_env_t **env,
                        axis2_char_t *attr_name,
                        axis2_char_t *soap_envelope_namespace_uri)
{
    axis2_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_om_attribute_t* om_attr = NULL;
    axis2_char_t *attr_value = NULL;
    axis2_qname_t *qn = NULL;
    AXIS2_FUNC_PARAM_CHECK(header_block, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, attr_name, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_envelope_namespace_uri, NULL);
    header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
    
    qn = axis2_qname_create(env, attr_name, soap_envelope_namespace_uri, NULL);
    if(!qn)
        return NULL;
    om_attr = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(header_block_impl->om_ele, env, qn);
    if(om_attr)
       attr_value = AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attr, env);
    AXIS2_QNAME_FREE(qn, env);
    return attr_value;       
}
                       
axis2_bool_t AXIS2_CALL 
axis2_soap_header_block_is_processed
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(header_block)->processed;
}
                        
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_processed
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(header_block)->processed = AXIS2_TRUE;
    return AXIS2_SUCCESS;
}                                                                                                                                                                                    
                                                                                                                            
axis2_status_t AXIS2_CALL axis2_soap_header_block_set_base_node
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         axis2_om_node_t *node)
{
   axis2_soap_header_block_impl_t *header_block_impl = NULL;
   AXIS2_FUNC_PARAM_CHECK( header_block, env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   header_block_impl = AXIS2_INTF_TO_IMPL(header_block);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   header_block_impl->om_ele_node = node;
   header_block_impl->om_ele = (axis2_om_element_t *)
            AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
   return AXIS2_SUCCESS;

}
                                        
axis2_om_node_t* AXIS2_CALL 
axis2_soap_header_block_get_base_node
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header_block, env, NULL);
    return AXIS2_INTF_TO_IMPL(header_block)->om_ele_node;
}
                                                                                                                                                                                
int AXIS2_CALL 
axis2_soap_header_block_get_soap_version  
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(header_block)->soap_version;
}
axis2_status_t AXIS2_CALL 
axis2_soap_header_block_set_soap_version  
                        (axis2_soap_header_block_t *header_block,
                         axis2_env_t **env,
                         int soap_version)
{
    AXIS2_FUNC_PARAM_CHECK(header_block, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_version, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(header_block)->soap_version = soap_version;
    return AXIS2_SUCCESS;
}
/******************** soap11 **************************************************/
AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap11_header_block_create_with_parent(axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_om_namespace_t *ns,
                                           axis2_soap_header_t *header)
{
    axis2_soap_header_block_t *header_block = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    header_block = axis2_soap_header_block_create_with_parent(env, localname, ns, header);
    if(!header_block)
        return NULL;
    axis2_soap_header_block_set_soap_version(header_block, env, AXIS2_SOAP11);
    return header_block;
}                                           
/******************** soap12 **************************************************/                                           
AXIS2_DECLARE(axis2_soap_header_block_t *)
axis2_soap12_header_block_create_with_parent(axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_om_namespace_t *ns,
                                           axis2_soap_header_t *header)
{
    axis2_soap_header_block_t *header_block = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    header_block = axis2_soap_header_block_create_with_parent(env, localname, ns, header);
    if(!header_block)
        return NULL;
    axis2_soap_header_block_set_soap_version(header_block, env, AXIS2_SOAP12);
    return header_block;
}                                           
