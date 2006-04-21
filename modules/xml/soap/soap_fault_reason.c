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
 
#include <axis2_soap_fault_reason.h>
#include "_axis2_soap_fault.h"
#include <axis2_om_element.h>
#include <axis2_soap_fault_text.h>
#include <axis2_soap_builder.h>
#include <axis2_soap_builder.h>

/*************************** impl struct **************************************/

typedef struct axis2_soap_fault_reason_impl_t
{
    axis2_soap_fault_reason_t fault_reason;
    /* corresponding om element node */    
    axis2_om_node_t *om_ele_node;
    
    axis2_array_list_t *fault_texts;
    /* pointer to soap builder */
    axis2_soap_builder_t *soap_builder;
    
}axis2_soap_fault_reason_impl_t;

/*************************** Macro ********************************************/

#define AXIS2_INTF_TO_IMPL(reason) ((axis2_soap_fault_reason_impl_t*)reason)

/*************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_free(axis2_soap_fault_reason_t *fault_reason,
                                  axis2_env_t **env);

axis2_soap_fault_text_t* AXIS2_CALL 
axis2_soap_fault_reason_get_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_char_t *lang);
                                
axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_reason_get_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                                 
axis2_array_list_t* AXIS2_CALL
axis2_soap_fault_reason_get_all_soap_fault_texts
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                                 
                                  
axis2_soap_fault_text_t* AXIS2_CALL
axis2_soap_fault_reason_get_first_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                                 
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_add_soap_fault_text
                            (axis2_soap_fault_reason_t *fault_reason,
                             axis2_env_t **env,
                             axis2_soap_fault_text_t *fault_text);
                             
static axis2_bool_t 
axis2_soap_fault_reason_lang_exists(axis2_soap_fault_reason_t *fault_reason,
                                    axis2_env_t **env,
                                    axis2_char_t *lang);                                       

                             
                                    

/************************* function implementations ***************************/

AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap_fault_reason_create(axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_reason_impl = (axis2_soap_fault_reason_impl_t*) AXIS2_MALLOC(
                            (*env)->allocator,
                            sizeof(axis2_soap_fault_reason_impl_t));
    if(!fault_reason_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_reason_impl->fault_reason.ops = NULL;
    fault_reason_impl->om_ele_node = NULL;
    fault_reason_impl->fault_texts = NULL;
    fault_reason_impl->fault_reason.ops = 
            (axis2_soap_fault_reason_ops_t*)AXIS2_MALLOC((*env)->allocator,
                sizeof(axis2_soap_fault_reason_ops_t));
                
    if(!(fault_reason_impl->fault_reason.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, fault_reason_impl);
        return NULL;
    }                                                                  
    
    fault_reason_impl->fault_reason.ops->free_fn = 
        axis2_soap_fault_reason_free;
    
    fault_reason_impl->fault_reason.ops->get_soap_fault_text =
        axis2_soap_fault_reason_get_soap_fault_text;        
        
    fault_reason_impl->fault_reason.ops->get_base_node =
        axis2_soap_fault_reason_get_base_node;
        
    fault_reason_impl->fault_reason.ops->get_first_soap_fault_text =
        axis2_soap_fault_reason_get_first_soap_fault_text;  
        
    fault_reason_impl->fault_reason.ops->get_all_soap_fault_texts =
        axis2_soap_fault_reason_get_all_soap_fault_texts;
        
    fault_reason_impl->fault_reason.ops->add_soap_fault_text =
        axis2_soap_fault_reason_add_soap_fault_text;                      
        
  return  &(fault_reason_impl->fault_reason);  
}

AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap_fault_reason_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    axis2_soap_fault_reason_t *fault_reason = NULL;
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    axis2_om_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    
    fault_reason = axis2_soap_fault_reason_create(env);
    if(!fault_reason)
        return NULL;
        
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if(!parent_node)
    {
        return NULL;
    }
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    if(!parent_ele)
    {
        return NULL;
    }
    
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    if(!this_ele)
    {
        return NULL;
    }
    
    fault_reason_impl->om_ele_node = this_node; 
    
    axis2_soap_fault_set_reason (fault, env, fault_reason);                                      
    return  &(fault_reason_impl->fault_reason);            
}                            
                            
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_free(axis2_soap_fault_reason_t *fault_reason,
                                  axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(NULL != fault_reason_impl->fault_texts)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(fault_reason_impl->fault_texts, env);
        
        for( i = 0  ; i < size; i++)
        {
            axis2_soap_fault_text_t *fault_text = NULL;
            void *value = NULL;
            value = AXIS2_ARRAY_LIST_GET(fault_reason_impl->fault_texts, env, i);
            if(NULL != value)
            {
                fault_text = (axis2_soap_fault_text_t *)value;
                AXIS2_SOAP_FAULT_TEXT_FREE(fault_text, env);
                fault_text = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(fault_reason_impl->fault_texts, env);
        fault_reason_impl->fault_texts = NULL;
    }
    if(NULL != fault_reason->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_reason->ops);
        fault_reason->ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, fault_reason_impl);
    
    fault_reason_impl = NULL;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_soap_fault_text_t *soap_text)
{
    /*
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_text, AXIS2_FAILURE);
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    if(!(fault_reason_impl->text))
    {
        fault_reason_impl->text = soap_text;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, 
            " error trying to set soap fault text twice");
        
    }
    */
    return AXIS2_FAILURE;
}
                                
axis2_soap_fault_text_t* AXIS2_CALL 
axis2_soap_fault_reason_get_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_char_t *lang)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    int status = AXIS2_SUCCESS;
    int size = 0;
    int i = 0;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault_reason, NULL);
    if(!lang || (AXIS2_STRCMP(lang, "") == 0))
        return NULL;
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    /** Here we have to build the soap fault reason element completly */
    if(!fault_reason_impl->fault_texts)
    {
    
        if(NULL != fault_reason_impl->soap_builder &&
            !(AXIS2_OM_NODE_IS_COMPLETE(fault_reason_impl->om_ele_node, env)))
        {
            while(!(AXIS2_OM_NODE_IS_COMPLETE(fault_reason_impl->om_ele_node, env)))
            {
                status = AXIS2_SOAP_BUILDER_NEXT(fault_reason_impl->soap_builder, env);
                if(status == AXIS2_FAILURE)
                    return NULL;
            }
        }
    }        
    
    if(!fault_reason_impl->fault_texts)
        return NULL;
    
    /** iterate the array list */            
    size = AXIS2_ARRAY_LIST_SIZE(fault_reason_impl->fault_texts, env);
    for( i = 0; i < size; i++)
    {
        axis2_soap_fault_text_t *fault_text = NULL;
        void *value = NULL;
        
        value = AXIS2_ARRAY_LIST_GET(fault_reason_impl->fault_texts, env, i);
        if(NULL != value)
        {
            axis2_char_t *fault_lang = NULL;
            fault_text = (axis2_soap_fault_text_t *)value;
            fault_lang = AXIS2_SOAP_FAULT_TEXT_GET_LANG(fault_text, env);
            if(NULL != fault_lang && AXIS2_STRCMP(lang, fault_lang) == 0)
            {
                return fault_text;
            }
        }
    }
    return NULL;    
}
                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_om_node_t *node)
{
   axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
   
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   
   fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   fault_reason_impl->om_ele_node = node;
   
   return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_reason_get_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
     AXIS2_ENV_CHECK(env, NULL);
     return AXIS2_INTF_TO_IMPL(fault_reason)->om_ele_node;
}
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_builder(axis2_soap_fault_reason_t *fault_reason,
                                    axis2_env_t **env,
                                    axis2_soap_builder_t *builder)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    fault_reason_impl->soap_builder = builder;
    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
axis2_soap_fault_reason_get_all_soap_fault_texts
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    int status = AXIS2_SUCCESS;
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(!(fault_reason_impl->fault_texts) && (NULL != fault_reason_impl->soap_builder))
    {
       if(!(AXIS2_OM_NODE_IS_COMPLETE(fault_reason_impl->om_ele_node, env)))
        {
            while(!(AXIS2_OM_NODE_IS_COMPLETE(fault_reason_impl->om_ele_node, env)))
            {
                status = AXIS2_SOAP_BUILDER_NEXT(fault_reason_impl->soap_builder, env);
                if(status == AXIS2_FAILURE)
                    return NULL;
            }
        }
    }   
    return fault_reason_impl->fault_texts;
}                                 
                                 
                                  
axis2_soap_fault_text_t* AXIS2_CALL
axis2_soap_fault_reason_get_first_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    int status = AXIS2_SUCCESS;
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(!(fault_reason_impl->fault_texts) && (NULL != fault_reason_impl->soap_builder))
    {
       if(!(AXIS2_OM_NODE_IS_COMPLETE(fault_reason_impl->om_ele_node, env)))
        {
            while(!(AXIS2_OM_NODE_IS_COMPLETE(fault_reason_impl->om_ele_node, env)))
            {
                status = AXIS2_SOAP_BUILDER_NEXT(fault_reason_impl->soap_builder, env);
                if(status == AXIS2_FAILURE)
                    return NULL;
            }
        }
    }   
    if(NULL != fault_reason_impl->fault_texts)
    {
        axis2_soap_fault_text_t *fault_text = NULL;
        void *value = NULL;
        value = AXIS2_ARRAY_LIST_GET(fault_reason_impl->fault_texts, env, 0);
        if(NULL != value)
            return (axis2_soap_fault_text_t*)value;
    }
    return NULL;
}                                 
                                 
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_add_soap_fault_text
                            (axis2_soap_fault_reason_t *fault_reason,
                             axis2_env_t **env,
                             axis2_soap_fault_text_t *fault_text)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!fault_text)
        return AXIS2_FAILURE;
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    if(!(fault_reason_impl->fault_texts))
    {
        fault_reason_impl->fault_texts = axis2_array_list_create(env, 1);
        if(!fault_reason_impl->fault_texts)
            return AXIS2_FAILURE;
        AXIS2_ARRAY_LIST_ADD( fault_reason_impl->fault_texts, env, fault_text);            
    }        
    else
    {
        axis2_char_t *lang = NULL;
        axis2_bool_t is_exists = AXIS2_FALSE;
        lang = AXIS2_SOAP_FAULT_TEXT_GET_LANG(fault_text, env);
        if(NULL != lang)
        {
            is_exists = axis2_soap_fault_reason_lang_exists(fault_reason, env, lang);
            if(is_exists == AXIS2_TRUE)
                return AXIS2_FAILURE;
            /** this soap_fault text already exists */                
        }
        AXIS2_ARRAY_LIST_ADD( fault_reason_impl->fault_texts, env, fault_text);
    }
    return AXIS2_SUCCESS;
}

static axis2_bool_t 
axis2_soap_fault_reason_lang_exists(axis2_soap_fault_reason_t *fault_reason,
                                    axis2_env_t **env,
                                    axis2_char_t *lang)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    int size = 0;
    int i    = 0;
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(!lang || (AXIS2_STRCMP(lang,"") == 0) || !fault_reason_impl->fault_texts)
        return AXIS2_FALSE;    
    
    size = AXIS2_ARRAY_LIST_SIZE(fault_reason_impl->fault_texts, env);
    for(i = 0; i< size; i++)
    {
        axis2_soap_fault_text_t *fault_text = NULL;
        void *value = NULL;
        value = AXIS2_ARRAY_LIST_GET(fault_reason_impl->fault_texts, env, i);
        if(NULL != value)
        {
            axis2_char_t *text_lang = NULL;
            fault_text = (axis2_soap_fault_text_t *)value;
            
            text_lang = AXIS2_SOAP_FAULT_TEXT_GET_LANG(fault_text, env);
            if(NULL != text_lang && (AXIS2_STRCMP(lang, text_lang) == 0))
            {
                return AXIS2_TRUE;
            }
        }
    }                    
    return AXIS2_FALSE;
}                                                                 
