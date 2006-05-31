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
 
#include <axis2_soap_fault.h>
#include <axis2_om_element.h>
#include "_axis2_soap_fault_text.h"
#include "_axis2_soap_fault_reason.h"
#include <axis2_om_namespace.h>
#include <axis2_qname.h>

 
/****************** impl struct *********************************************/
 
 typedef struct axis2_soap_fault_text_impl_t
 {
    axis2_soap_fault_text_t fault_text;
    
    axis2_om_attribute_t *lang_attribute;
    
    axis2_om_namespace_t *lang_namespace;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_bool_t lang_ns_used;
    
}axis2_soap_fault_text_impl_t;

/********************** Macro ************************************************/

#define AXIS2_INTF_TO_IMPL(fault_txt) \
        ((axis2_soap_fault_text_impl_t*)fault_txt)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL
axis2_soap_fault_text_free(axis2_soap_fault_text_t *fault_text,
                                  const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_soap_fault_text_set_lang(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env,
                                      axis2_char_t* lang);
                                     
axis2_char_t* AXIS2_CALL
axis2_soap_fault_text_get_lang(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env);
                                     
axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_text_get_base_node(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env);
                                      
axis2_status_t AXIS2_CALL
axis2_soap_fault_text_set_text(axis2_soap_fault_text_t *fault_text,
                               const axis2_env_t *env,
                               axis2_char_t *value,
                               axis2_char_t *lang);
                                                                    
axis2_char_t* AXIS2_CALL
axis2_soap_fault_text_get_text(axis2_soap_fault_text_t *fault_text,
                               const axis2_env_t *env);                                      
/***************************** functions **************************************/                                      
                                                       

AXIS2_DECLARE(axis2_soap_fault_text_t *)
axis2_soap_fault_text_create(const axis2_env_t *env)
{
    axis2_soap_fault_text_impl_t *fault_text_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    fault_text_impl = (axis2_soap_fault_text_impl_t*)AXIS2_MALLOC(
                        env->allocator,
                        sizeof(axis2_soap_fault_text_impl_t));
    if(!fault_text_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                                  
    
    fault_text_impl->fault_text.ops = NULL;
    fault_text_impl->om_ele_node = NULL;
    fault_text_impl->lang_attribute = NULL;
    fault_text_impl->lang_namespace = NULL;
    fault_text_impl->lang_ns_used = AXIS2_FALSE;
    
    fault_text_impl->lang_namespace = axis2_om_namespace_create(env,
                        AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_URI,
                        AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_PREFIX);
                                        
    if(!(fault_text_impl->lang_namespace))
            return NULL;
            
    fault_text_impl->fault_text.ops = (axis2_soap_fault_text_ops_t*)AXIS2_MALLOC(
                                       env->allocator,
                                       sizeof(axis2_soap_fault_text_ops_t));
    if(!(fault_text_impl->fault_text.ops))
    {
        AXIS2_FREE(env->allocator, fault_text_impl);
        fault_text_impl = NULL;
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                                       
    fault_text_impl->fault_text.ops->free_fn = 
        axis2_soap_fault_text_free;
        
    fault_text_impl->fault_text.ops->set_lang =
        axis2_soap_fault_text_set_lang;        
    
    fault_text_impl->fault_text.ops->get_lang =
        axis2_soap_fault_text_get_lang;
    
    fault_text_impl->fault_text.ops->get_base_node =
        axis2_soap_fault_text_get_base_node;
        
    fault_text_impl->fault_text.ops->set_text =
        axis2_soap_fault_text_set_text;   
        
    fault_text_impl->fault_text.ops->get_text =
        axis2_soap_fault_text_get_text;
                 

    return &(fault_text_impl->fault_text);    
}



AXIS2_DECLARE(axis2_soap_fault_text_t *)
axis2_soap_fault_text_create_with_parent(const axis2_env_t *env,
                            axis2_soap_fault_reason_t *parent)
{
    axis2_soap_fault_text_impl_t *fault_text_impl = NULL;
    axis2_soap_fault_text_t *fault_text = NULL;
    int soap_version = -1;
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_namespace_t *parent_ns = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, parent, NULL);
    
    fault_text = axis2_soap_fault_text_create(env);
    
    if(!fault_text)
    {
        return NULL;
    }
    
    fault_text_impl = AXIS2_INTF_TO_IMPL(fault_text);
    
    parent_node = AXIS2_SOAP_FAULT_REASON_GET_BASE_NODE(parent, env);
    if(!parent_node)
    {
        AXIS2_SOAP_FAULT_TEXT_FREE(fault_text, env);
        return NULL;
    }
    
    parent_ele  = (axis2_om_element_t*)
                    AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
    if(!parent_ele)
    {
        AXIS2_SOAP_FAULT_TEXT_FREE(fault_text, env);
        return NULL;
    }
    soap_version = axis2_soap_fault_reason_get_soap_version(parent, env);
    if(soap_version == AXIS2_SOAP12)
    {
        parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    }
    this_ele = axis2_om_element_create(env, 
               parent_node, AXIS2_SOAP12_SOAP_FAULT_TEXT_LOCAL_NAME, 
               parent_ns, &this_node);
               
    if(!this_ele)
    {
        AXIS2_SOAP_FAULT_TEXT_FREE(fault_text, env);
        return NULL;
    }
    
    fault_text_impl->om_ele_node = this_node;
    
    AXIS2_SOAP_FAULT_REASON_ADD_SOAP_FAULT_TEXT(parent, env, fault_text);
    
    return &(fault_text_impl->fault_text);
}
                            
                                                       
axis2_status_t AXIS2_CALL
axis2_soap_fault_text_free(axis2_soap_fault_text_t *fault_text,
                                  const axis2_env_t *env)
{
    axis2_soap_fault_text_impl_t* fault_text_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_text_impl = AXIS2_INTF_TO_IMPL(fault_text);
    
    if(NULL != fault_text->ops)
    {
        AXIS2_FREE(env->allocator, fault_text->ops);
        fault_text->ops = NULL; 
    }    
    if(fault_text_impl->lang_ns_used == AXIS2_FALSE && fault_text_impl->lang_namespace)
    {
        AXIS2_OM_NAMESPACE_FREE(fault_text_impl->lang_namespace, env);
        fault_text_impl->lang_namespace = NULL;
    }
    AXIS2_FREE(env->allocator, fault_text_impl);
    fault_text_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_soap_fault_text_set_lang(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env,
                                      axis2_char_t* lang)
{
    axis2_soap_fault_text_impl_t* fault_text_impl = NULL;
    int status = AXIS2_SUCCESS;
    axis2_om_element_t *om_ele = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, lang, AXIS2_FAILURE);
    
    fault_text_impl = AXIS2_INTF_TO_IMPL(fault_text);
    if(NULL != fault_text_impl->lang_attribute)
    {
        axis2_char_t *attr_lang = NULL;
        attr_lang = AXIS2_OM_ATTRIBUTE_GET_VALUE( fault_text_impl->lang_attribute, env);
        if(NULL != attr_lang)
        {
            if(AXIS2_STRCMP(attr_lang, lang) == 0)
            {
                /** this attribute already exists */
                return AXIS2_SUCCESS;
            }
        } 
        AXIS2_OM_ATTRIBUTE_SET_VALUE(fault_text_impl->lang_attribute, env, lang);
        return AXIS2_SUCCESS;
    }
    
    fault_text_impl->lang_attribute = axis2_om_attribute_create(env, 
                                        AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_LOCAL_NAME,
                                        lang, fault_text_impl->lang_namespace);
    if(!fault_text_impl->lang_attribute)
        return AXIS2_FAILURE;
        
    
    if(!fault_text_impl->om_ele_node)
        return AXIS2_FAILURE;
    
    om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(fault_text_impl->om_ele_node, env);
    if(!om_ele)
        return AXIS2_FAILURE;
        
    status = AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(om_ele, env, fault_text_impl->lang_attribute,
                                       fault_text_impl->om_ele_node);
   
    if(status == AXIS2_SUCCESS)
        fault_text_impl->lang_ns_used = AXIS2_TRUE;
    else
    {
        AXIS2_OM_ATTRIBUTE_FREE(fault_text_impl->lang_attribute, env);
        fault_text_impl->lang_attribute = NULL;
    }
    return status;
}
                                     
axis2_char_t* AXIS2_CALL
axis2_soap_fault_text_get_lang(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env)
{
    axis2_soap_fault_text_impl_t* fault_text_impl = NULL;
    axis2_om_element_t* om_ele = NULL;
    axis2_qname_t* tmp_qname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    fault_text_impl = AXIS2_INTF_TO_IMPL(fault_text);   
    if(!fault_text_impl->om_ele_node)
        return NULL;

    om_ele = (axis2_om_element_t*)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(fault_text_impl->om_ele_node, env);
    if(!om_ele)
        return NULL;
    
    if(!(fault_text_impl->lang_attribute))
    {

    /* this logic need to be rechecked */
        tmp_qname = axis2_qname_create(env, 
                        AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_LOCAL_NAME,
                        AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_URI,
                        AXIS2_SOAP12_SOAP_FAULT_TEXT_LANG_ATTR_NS_PREFIX);

        fault_text_impl->lang_attribute = 
            AXIS2_OM_ELEMENT_GET_ATTRIBUTE(om_ele, env, tmp_qname);
        AXIS2_QNAME_FREE(tmp_qname, env);                                           
    }
    if(fault_text_impl->lang_attribute)
    {
        return AXIS2_OM_ATTRIBUTE_GET_VALUE(fault_text_impl->lang_attribute, env);
    }
    else
        return NULL;
}
                                     
axis2_status_t AXIS2_CALL 
axis2_soap_fault_text_set_base_node(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env,
                                      axis2_om_node_t *node)
{
    axis2_soap_fault_text_impl_t* fault_text_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    
    fault_text_impl = AXIS2_INTF_TO_IMPL(fault_text);
    
    if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error, 
                AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    fault_text_impl->om_ele_node = node;
    return AXIS2_SUCCESS;
}
    
axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_text_get_base_node(axis2_soap_fault_text_t *fault_text,
                                      const axis2_env_t *env)
{
    axis2_soap_fault_text_impl_t* fault_text_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_text_impl = AXIS2_INTF_TO_IMPL(fault_text); 
    return fault_text_impl->om_ele_node;
}

axis2_status_t AXIS2_CALL
axis2_soap_fault_text_set_text(axis2_soap_fault_text_t *fault_text,
                               const axis2_env_t *env,
                               axis2_char_t *value,
                               axis2_char_t *lang)
{
    axis2_soap_fault_text_impl_t *text_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);
    
    text_impl = AXIS2_INTF_TO_IMPL(fault_text);
    
    if(NULL != text_impl->om_ele_node)
    {
        axis2_om_element_t *text_ele = NULL;
        text_ele = (axis2_om_element_t *) 
                AXIS2_OM_NODE_GET_DATA_ELEMENT(text_impl->om_ele_node, env);
        if(NULL != text_ele)
        {
            AXIS2_OM_ELEMENT_SET_TEXT(text_ele, env, value, text_impl->om_ele_node);
            if(NULL != lang)
            {
                axis2_soap_fault_text_set_lang(fault_text, env, lang);
            }
            return AXIS2_SUCCESS;
        }
    } 
    return AXIS2_FAILURE;     
}                               
                               
axis2_char_t* AXIS2_CALL
axis2_soap_fault_text_get_text(axis2_soap_fault_text_t *fault_text,
                               const axis2_env_t *env)
{
    axis2_soap_fault_text_impl_t *text_impl = NULL;
    axis2_char_t *text = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    text_impl = AXIS2_INTF_TO_IMPL(fault_text);
    
    if(NULL != text_impl->om_ele_node)
    {
        axis2_om_element_t *text_ele = NULL;
        text_ele = (axis2_om_element_t *) 
                AXIS2_OM_NODE_GET_DATA_ELEMENT(text_impl->om_ele_node, env);
        if(NULL != text_ele)
        {
            text = AXIS2_OM_ELEMENT_GET_TEXT(text_ele, env, 
                        text_impl->om_ele_node);
            return text;
        }
    } 
    return NULL;
}


