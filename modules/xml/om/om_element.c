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

#include <axis2_om_element.h>
#include "axis2_om_node_internal.h"
#include <axis2_om_attribute.h>
#include <axis2_om_namespace.h>
#include <axis2_xml_writer.h>
#include <axis2_om_stax_builder.h>
#include <string.h>
#include <stdio.h>
                                 
axis2_om_namespace_t *AXIS2_CALL
axis2_om_element_find_namespace(axis2_om_element_t *om_element,
                                axis2_env_t **env,
                                axis2_om_node_t *node,
                                const axis2_char_t * uri,
                                const axis2_char_t * prefix);

axis2_status_t AXIS2_CALL
axis2_om_element_declare_namespace (axis2_om_element_t *om_element,
                                   axis2_env_t **env,
                                   axis2_om_node_t * node,
                                   axis2_om_namespace_t * ns);

axis2_om_namespace_t *AXIS2_CALL
axis2_om_element_find_namespace_with_qname (axis2_om_element_t *om_element,
                                            axis2_env_t **env,
                                            axis2_om_node_t * node,
                                            axis2_qname_t * qname);

axis2_om_namespace_t *AXIS2_CALL
axis2_om_element_find_declared_namespace (axis2_om_element_t *om_element,
                                          axis2_env_t **env,
                                          const axis2_char_t *uri,
                                          const axis2_char_t *prefix);


axis2_status_t AXIS2_CALL
axis2_om_element_add_attribute (axis2_om_element_t *element,
                                axis2_env_t **env,
                                axis2_om_attribute_t *attribute,
                                axis2_om_node_t *element_node);

axis2_om_attribute_t *AXIS2_CALL
axis2_om_element_get_attribute(axis2_om_element_t *element,
                               axis2_env_t **env,
                               axis2_qname_t * qname);
                               
axis2_char_t* AXIS2_CALL
axis2_om_element_get_attribute_value(axis2_om_element_t *element,
                                     axis2_env_t **env,
                                     axis2_qname_t * qname);                               

axis2_status_t AXIS2_CALL
axis2_om_element_free (axis2_om_element_t *element,                                     
                       axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_start_part(axis2_om_element_t * element,
                                      axis2_env_t **env,
                                      axis2_om_output_t *om_output,
                                      axis2_om_node_t *element_node);

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_end_part (axis2_om_element_t * element,
                                     axis2_env_t **env,
                                     axis2_om_output_t * om_output);

axis2_char_t* AXIS2_CALL
axis2_om_element_get_localname(axis2_om_element_t *om_element,
                               axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_element_set_localname(axis2_om_element_t *om_element,
                              axis2_env_t **env,
                               const axis2_char_t *localname);
        
axis2_om_namespace_t *AXIS2_CALL
axis2_om_element_get_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               axis2_om_node_t *ele_node);
                                                          
axis2_status_t AXIS2_CALL 
axis2_om_element_set_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               axis2_om_namespace_t *ns,
                               axis2_om_node_t *node); 

axis2_hash_t * AXIS2_CALL
axis2_om_element_get_all_attributes(axis2_om_element_t *om_element,
                                    axis2_env_t **env);
                                                                                                                                                                                                                               
axis2_hash_t* AXIS2_CALL 
axis2_om_element_get_namespaces(axis2_om_element_t *om_element,
                                axis2_env_t **env);                                                                                                                  
                                     
                                     
axis2_qname_t* AXIS2_CALL
axis2_om_element_get_qname(axis2_om_element_t *om_element,
                            axis2_env_t **env,
                            axis2_om_node_t *ele_node);

axis2_om_children_iterator_t* AXIS2_CALL
axis2_om_element_get_children(axis2_om_element_t *om_element,
                              axis2_env_t **env,
                              axis2_om_node_t *element_node);

axis2_om_children_qname_iterator_t* AXIS2_CALL
axis2_om_element_get_children_with_qname(axis2_om_element_t *om_element,
                                         axis2_env_t **env,
                                         axis2_qname_t *element_qname,
                                         axis2_om_node_t *element_node);

axis2_om_element_t* AXIS2_CALL
axis2_om_element_get_first_child_with_qname(axis2_om_element_t *om_element,
                                            axis2_env_t **env,
                                            axis2_qname_t *element_qname,
                                            axis2_om_node_t *element_node,
                                            axis2_om_node_t **child_node);

axis2_status_t AXIS2_CALL
axis2_om_element_remove_attribute(axis2_om_element_t *om_element, 
                                  axis2_env_t **env,
                                  axis2_om_attribute_t *om_attribute);

axis2_om_element_t* AXIS2_CALL
axis2_om_element_get_first_element(axis2_om_element_t *om_element,
                                    axis2_env_t **env,
                                    axis2_om_node_t *element_node,
                                    axis2_om_node_t **first_ele_node);
axis2_char_t* AXIS2_CALL
axis2_om_element_get_text(axis2_om_element_t *om_element,
                          axis2_env_t **env,
                          axis2_om_node_t *element_node);

axis2_status_t AXIS2_CALL
axis2_om_element_set_text(axis2_om_element_t *om_element,
                          axis2_env_t **env,
                          axis2_char_t *text,
                          axis2_om_node_t *element_node);  

axis2_char_t* AXIS2_CALL
axis2_om_element_to_string(axis2_om_element_t *om_element,
                           axis2_env_t **env,
                           axis2_om_node_t *element_node); 

axis2_om_child_element_iterator_t * AXIS2_CALL
axis2_om_element_get_child_elements(axis2_om_element_t *om_element,
                                    axis2_env_t **env,
                                    axis2_om_node_t *element_node);                                                                                                                
                                                                      
axis2_status_t AXIS2_CALL
axis2_om_element_build(axis2_om_element_t *om_ele,
                       axis2_env_t **env,
                       axis2_om_node_t *om_ele_node);
                       

axis2_om_namespace_t* AXIS2_CALL
axis2_om_element_get_default_namespace(axis2_om_element_t *om_element,
                                        axis2_env_t **env,
                                        axis2_om_node_t *element_node);
                                    
/**
* declared a default namespace explicitly 
*/     
axis2_om_namespace_t* AXIS2_CALL 
axis2_om_element_declare_default_namespace(axis2_om_element_t *om_element,
                                           axis2_env_t **env,
                                           axis2_char_t *uri);
                        
/** 
* checks for the namespace in the context of this element 
* with the given prefix 
*/

axis2_om_namespace_t* AXIS2_CALL 
axis2_om_element_find_namespace_uri(axis2_om_element_t *om_element,
                                    axis2_env_t **env,
                                    axis2_char_t *prefix,
                                    axis2_om_node_t *element_node);                          
                                         
/************************** end function prototypes **********************/
typedef struct axis2_om_element_impl
{   
    axis2_om_element_t om_element;
    /** Element's namespace */
    axis2_om_namespace_t *ns;
    /** Element's local name */
    axis2_char_t *localname;
    /** List of attributes */
    axis2_hash_t *attributes;
    /** List of namespaces */
    axis2_hash_t *namespaces;
    
    axis2_qname_t *qname;
    
    axis2_om_child_element_iterator_t *child_ele_iter;

    axis2_om_children_iterator_t* children_iter;

    axis2_om_children_qname_iterator_t *children_qname_iter;

    axis2_char_t *text_value;
    
    int next_ns_prefix_number;
  
}axis2_om_element_impl_t;

/************************************Macro *****************************/


#define AXIS2_INTF_TO_IMPL(om_element) ((axis2_om_element_impl_t*)om_element)

/**********************************************************************/                                          
AXIS2_DECLARE(axis2_om_element_t *)
axis2_om_element_create (axis2_env_t **env,
                         axis2_om_node_t *parent,
                         const axis2_char_t *localname,
                         axis2_om_namespace_t *ns,
                         axis2_om_node_t **node)
{
    axis2_om_element_impl_t *element;
    AXIS2_ENV_CHECK(env, NULL);
    
    if (!localname || !node)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    (*node) = axis2_om_node_create (env);
    if (!(*node))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    element = (axis2_om_element_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                             sizeof (axis2_om_element_impl_t));

    if (!element)
    {
        AXIS2_FREE ((*env)->allocator, (*node));
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    element->ns = NULL;
    element->localname = NULL;
    element->attributes = NULL;
    element->namespaces = NULL;
    element->qname = NULL;
    element->child_ele_iter = NULL;
    element->children_iter = NULL;
    element->children_qname_iter = NULL;
    element->text_value = NULL;
    element->next_ns_prefix_number = 0;
    
    element->localname = (axis2_char_t *) AXIS2_STRDUP(localname,env);
    if (!element->localname)
    {
        AXIS2_FREE ((*env)->allocator, element);
        AXIS2_FREE ((*env)->allocator, (*node));
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (parent)
    {
        AXIS2_OM_NODE_ADD_CHILD(parent, env, (*node));
         
    }
    axis2_om_node_set_complete((*node), env, AXIS2_FALSE);
    axis2_om_node_set_node_type((*node), env, AXIS2_OM_ELEMENT);
    axis2_om_node_set_data_element((*node), env, element);

    if (ns)
    {
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;
        
        uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
        prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env);
        
        element->ns = axis2_om_element_find_namespace (&(element->om_element), 
                         env, *node, uri, prefix);
        if (!(element->ns))
        {
            if (axis2_om_element_declare_namespace(&(element->om_element), 
                    env, *node, ns) == AXIS2_SUCCESS)
                element->ns = ns;
        }
        if(NULL != prefix && AXIS2_STRCMP(prefix, "") == 0)
        {
            element->ns = NULL;
        }
    }

    element->om_element.ops = NULL;
    element->om_element.ops = (axis2_om_element_ops_t *)AXIS2_MALLOC(
                                        (*env)->allocator,
                                         sizeof(axis2_om_element_ops_t));

    if (!element->om_element.ops)
    {
        AXIS2_FREE ((*env)->allocator, element->localname);
        AXIS2_FREE ((*env)->allocator, element);
        AXIS2_FREE ((*env)->allocator,(*node));
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* set function pointers */
    element->om_element.ops->find_namespace = 
        axis2_om_element_find_namespace;
    element->om_element.ops->declare_namespace =
        axis2_om_element_declare_namespace;
    element->om_element.ops->find_namespace_with_qname =
        axis2_om_element_find_namespace_with_qname;
    element->om_element.ops->add_attribute =
        axis2_om_element_add_attribute;
    element->om_element.ops->get_attribute =
        axis2_om_element_get_attribute;
        
    element->om_element.ops->get_attribute_value = 
        axis2_om_element_get_attribute_value;
                
    element->om_element.ops->free = axis2_om_element_free;
    
    element->om_element.ops->serialize_start_part =
        axis2_om_element_serialize_start_part;
    element->om_element.ops->serialize_end_part =
        axis2_om_element_serialize_end_part;
    
    element->om_element.ops->set_localname = 
        axis2_om_element_set_localname;
    element->om_element.ops->set_namespace =
        axis2_om_element_set_namespace;
        
    element->om_element.ops->get_localname =
        axis2_om_element_get_localname;
    
    element->om_element.ops->get_namespace =
        axis2_om_element_get_namespace;
        
    element->om_element.ops->find_declared_namespace =
        axis2_om_element_find_declared_namespace; 
        
    element->om_element.ops->get_default_namespace = 
        axis2_om_element_get_default_namespace;
        
    element->om_element.ops->declare_default_namespace =
        axis2_om_element_declare_default_namespace;
        
    element->om_element.ops->find_namespace_uri =
        axis2_om_element_find_namespace_uri;                         
        
    element->om_element.ops->get_all_attributes =
        axis2_om_element_get_all_attributes;
           
    element->om_element.ops->get_namespaces =
        axis2_om_element_get_namespaces;
        
    element->om_element.ops->get_qname =
        axis2_om_element_get_qname; 
        
    element->om_element.ops->get_children =
        axis2_om_element_get_children;
        
    element->om_element.ops->get_children_with_qname =
        axis2_om_element_get_children_with_qname;
                        
    element->om_element.ops->get_first_child_with_qname =
        axis2_om_element_get_first_child_with_qname; 
        
    element->om_element.ops->remove_attribute =
        axis2_om_element_remove_attribute;
    
    element->om_element.ops->set_text =
        axis2_om_element_set_text;
    element->om_element.ops->get_text =
        axis2_om_element_get_text;
    element->om_element.ops->get_first_element =
        axis2_om_element_get_first_element;

    element->om_element.ops->to_string =
        axis2_om_element_to_string;
    element->om_element.ops->get_child_elements =
        axis2_om_element_get_child_elements;   

    element->om_element.ops->build =
        axis2_om_element_build;

    return &(element->om_element);
}

AXIS2_DECLARE(axis2_om_element_t *)
axis2_om_element_create_with_qname (axis2_env_t **env,
                                    axis2_om_node_t *parent,
                                    axis2_qname_t *qname,
                                    axis2_om_node_t ** node)
{
    axis2_om_element_t *element = NULL;
    axis2_char_t *localpart = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    if (!qname || !(*node))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;           
    }
    localpart = AXIS2_QNAME_GET_LOCALPART(qname, env);
    if(!localpart)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);        
        return NULL;
    }
    element = axis2_om_element_create (env, parent, localpart, NULL, node);
    if(!element)
        return NULL;
        
    if (*node)
    {
        axis2_om_element_t *ele = NULL;
        axis2_char_t *temp_nsuri = NULL;
        axis2_char_t *temp_prefix = NULL;
        axis2_om_namespace_t *ns  = NULL;
        
        ele =  ((axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT((*node), env));
        temp_nsuri = AXIS2_QNAME_GET_URI(qname, env);
        temp_prefix = AXIS2_QNAME_GET_PREFIX(qname, env);
        if(!ele)
            return NULL;
        
        if((NULL == temp_nsuri) || (AXIS2_STRCMP(temp_nsuri, "") == 0))
        {
               /** no namespace uri is available in given qname
                   no need to bother about it 
                */
            return ele;            
        }            
        
      AXIS2_INTF_TO_IMPL(ele)->ns = axis2_om_element_find_namespace(ele, env,
                (*node), temp_nsuri, temp_prefix);
                                               
       if (!(AXIS2_INTF_TO_IMPL(element)->ns))
       {
           /** could not find a namespace so declare namespace */
           ns = axis2_om_namespace_create(env, temp_nsuri, temp_prefix);
           if (axis2_om_element_declare_namespace(ele , env, *node, ns) == AXIS2_SUCCESS)
           {
                (AXIS2_INTF_TO_IMPL(element)->ns) = ns;
                return ele;
           }
           else
           {
               AXIS2_OM_NAMESPACE_FREE(ns, env);
               AXIS2_OM_ELEMENT_FREE(ele, env);
               AXIS2_FREE((*env)->allocator, *node);
               return NULL;
           }               
       }
    }
    return element;
}

axis2_om_namespace_t * AXIS2_CALL
axis2_om_element_find_namespace (axis2_om_element_t *om_element,
                                 axis2_env_t **env,
                                 axis2_om_node_t * element_node,
                                 const axis2_char_t * uri,
                                 const axis2_char_t * prefix)
{
    
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_om_node_t *parent = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    /** TODO uri should not be null but when null check added 
        other services fail, fix them and the fix this */
        
    /*AXIS2_PARAM_CHECK((*env)->error, uri, NULL); */
    
    if (!element_node || !om_element)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }
    if (!AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node, env) || 
            AXIS2_OM_NODE_GET_NODE_TYPE(element_node, env) != AXIS2_OM_ELEMENT)
    {
        /* wrong element type or null node */
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }
    
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);

    if (NULL != om_ele_impl->namespaces )
    {   
        void *ns = NULL;
        
        if (!prefix || AXIS2_STRCMP(prefix, "") == 0)
        {   
            /** check for a default namepsace */
            axis2_om_namespace_t *default_ns = NULL;
            axis2_hash_index_t *hashindex;
            
            default_ns = axis2_om_element_get_default_namespace(om_element , 
                            env, element_node);
            if(NULL != default_ns && NULL != uri)
            {
                axis2_char_t *default_uri = NULL;
                default_uri = AXIS2_OM_NAMESPACE_GET_URI(default_ns, env);
                
                if(NULL != default_uri && AXIS2_STRCMP(uri, default_uri) == 0)
                {
                    return default_ns;
                }
            }
            /** prefix is null , so iterate the namespaces hash to find the namespace */            
            for (hashindex = axis2_hash_first (om_ele_impl->namespaces, env);
                 hashindex; hashindex = axis2_hash_next (env, hashindex))
            {
                axis2_hash_this (hashindex, NULL, NULL, &ns);
                if (NULL != ns)
                {   
                    axis2_om_namespace_t *temp_ns = NULL;
                    axis2_char_t *temp_nsuri = NULL;
                    temp_ns = (axis2_om_namespace_t *)ns;
                    temp_nsuri = AXIS2_OM_NAMESPACE_GET_URI(temp_ns, env);
                    
                    if(temp_nsuri && (AXIS2_STRCMP(temp_nsuri, uri) == 0))
                    {
                        /** namespace uri matches , so free hashindex and return ns*/
		                AXIS2_FREE ((*env)->allocator, hashindex);
                        return (axis2_om_namespace_t *) (ns);
                    }
                    temp_ns = NULL;
                    temp_nsuri = NULL;
                    ns = NULL;
                }
            }
            ns = NULL;
        }
        else if(NULL != prefix )
        {   
            /** prefix is not null get namespace directly if exist */
            ns = axis2_hash_get (om_ele_impl->namespaces, prefix,
                             AXIS2_HASH_KEY_STRING);
            if (NULL != ns )
            {
                axis2_om_namespace_t *found_ns  = NULL;
                axis2_char_t *found_uri         = NULL;
                found_ns = (axis2_om_namespace_t*)ns;
               
                found_uri = AXIS2_OM_NAMESPACE_GET_URI(found_ns, env);
               
                /* TODO fix this 
                if( (NULL != found_uri) && AXIS2_STRCMP(found_uri, uri) == 0)
                {
                    printf("\n  furi   %s    real uri %s  %s\n ", found_uri, uri, prefix);
                    return found_ns;
                }
                */
                return found_ns;
            }                             
        }
    }
    /** could not find the namespace in current element scope 
        look in the parent */
    
    parent = AXIS2_OM_NODE_GET_PARENT(element_node, env);                       
    
    if( NULL != parent)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(parent, env) == AXIS2_OM_ELEMENT)
        {
            axis2_om_element_t *om_ele = NULL;
            om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(parent, env);
            if(om_ele)
            {   /** parent exist, parent is om element so find in parent*/             
                return axis2_om_element_find_namespace(om_ele, env,
                        parent, uri, prefix);
            }
        }
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_element_declare_namespace (axis2_om_element_t *ele,
                                    axis2_env_t **env,
                                    axis2_om_node_t *node,
                                    axis2_om_namespace_t *ns)
{
    axis2_om_namespace_t *declared_ns = NULL;
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *uri    = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_ele_impl = AXIS2_INTF_TO_IMPL(ele);
    
    if (!node || !ns || !ele)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
            
        return AXIS2_FAILURE;
    }
    
    uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
    prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env);
    
    declared_ns = axis2_om_element_find_namespace(ele, 
                        env, node, uri, prefix);

    if (NULL != declared_ns)
    {
        if(AXIS2_OM_NAMESPACE_EQUALS(ns, env, declared_ns) == AXIS2_TRUE)
        {/*Namespace already declared, so return */
            return AXIS2_SUCCESS;
        }            
    }
   
    if (!(om_ele_impl->namespaces))
    {
        om_ele_impl->namespaces = axis2_hash_make (env);
            if (!(om_ele_impl->namespaces))
                return AXIS2_FAILURE;
    }
    if(NULL != prefix) 
    {
        axis2_hash_set (om_ele_impl->namespaces,
            prefix, AXIS2_HASH_KEY_STRING, ns);                            
    }
    else
    {
            axis2_char_t *key = NULL;
            key = AXIS2_MALLOC((*env)->allocator, sizeof(char) *10);
            memset(key, 0, sizeof(char)*10);
            om_ele_impl->next_ns_prefix_number++;
            sprintf(key, "axis2ns%d", om_ele_impl->next_ns_prefix_number);
            axis2_hash_set (om_ele_impl->namespaces, key, 
                            AXIS2_HASH_KEY_STRING,  ns);
    }
    
    return AXIS2_SUCCESS;
}

axis2_om_namespace_t * AXIS2_CALL
axis2_om_element_find_declared_namespace (axis2_om_element_t *om_element,
                                            axis2_env_t **env,
                                            const axis2_char_t * uri,
                                            const axis2_char_t * prefix)
{
    axis2_hash_index_t *hash_index = NULL;
    void *ns = NULL;
    axis2_om_element_impl_t *om_element_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    
    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    if (!(om_element_impl->namespaces))
    {
        return NULL;
    }
    if (!prefix || AXIS2_STRCMP(prefix, "") == 0)
    {
        /** prefix null iterate the namespace hash for matching uri */
        for (hash_index = axis2_hash_first (om_element_impl->namespaces, env);
             hash_index; hash_index = axis2_hash_next (env, hash_index))
        {
            axis2_hash_this (hash_index, NULL, NULL, &ns);
            if(NULL != ns)
            {
                axis2_om_namespace_t *temp_ns = NULL;
                axis2_char_t *temp_nsuri = NULL;
                temp_ns = (axis2_om_namespace_t *)(ns);
                temp_nsuri = AXIS2_OM_NAMESPACE_GET_URI(temp_ns, env);
                
                if (temp_nsuri && AXIS2_STRCMP(temp_nsuri, uri) == 0)
                {
		            AXIS2_FREE ((*env)->allocator, hash_index);
                    return temp_ns;
                }
                temp_ns = NULL;
                temp_nsuri = NULL;
            }
        }
        ns = NULL;
        return NULL;
    }
    else if(NULL != prefix)
    {
        axis2_om_namespace_t *found_ns = NULL;        
        ns = axis2_hash_get(om_element_impl->namespaces, prefix, AXIS2_HASH_KEY_STRING);
        if (NULL != ns)
        {
            axis2_char_t *found_uri = NULL;
            found_ns = (axis2_om_namespace_t *) ns;
            found_uri = AXIS2_OM_NAMESPACE_GET_URI(found_ns, env);
            if(NULL != found_uri && AXIS2_STRCMP(found_uri, uri) == 0)
            {
                return found_ns;
            }
        }                        
    }
    return NULL;
}


axis2_om_namespace_t * AXIS2_CALL
axis2_om_element_find_namespace_with_qname (axis2_om_element_t *element,
                                            axis2_env_t **env,
                                            axis2_om_node_t *node,
                                            axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (!element || !node || !qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    if (AXIS2_QNAME_GET_URI(qname, env))
    {
        return axis2_om_element_find_namespace (element, env, node,
                                                AXIS2_QNAME_GET_URI(qname, env),
                                                AXIS2_QNAME_GET_PREFIX(qname, env));
    }
    else
    {
        return NULL;
    }
}

axis2_status_t AXIS2_CALL
axis2_om_element_add_attribute (axis2_om_element_t *om_element,
                                axis2_env_t **env,
                                axis2_om_attribute_t *attribute,
                                axis2_om_node_t *element_node)
{
    
    axis2_qname_t *qname = NULL;
    axis2_om_element_impl_t *om_element_impl = NULL;
    axis2_om_namespace_t *om_namespace = NULL;
    axis2_om_namespace_t *temp_ns = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attribute, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element_node, AXIS2_FAILURE);
    
    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);    
    om_namespace = AXIS2_OM_ATTRIBUTE_GET_NAMESPACE(attribute, env);
    
    if(om_namespace)
    {
        temp_ns = AXIS2_OM_ELEMENT_FIND_NAMESPACE
                        (om_element, env, 
                         element_node,
                         AXIS2_OM_NAMESPACE_GET_URI(om_namespace, env),
                         AXIS2_OM_NAMESPACE_GET_PREFIX(om_namespace, env));
        if(!temp_ns)
        {
            axis2_status_t status = AXIS2_SUCCESS;
            status = AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(om_element, env, 
                                               element_node, om_namespace); 
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
        }
    } 
   
    if(!(om_element_impl->attributes))
    {
        om_element_impl->attributes = axis2_hash_make (env);
        if (!(om_element_impl->attributes))
            return AXIS2_FAILURE;
    }

    qname = AXIS2_OM_ATTRIBUTE_GET_QNAME(attribute, env);
    if (qname)
    {
        axis2_char_t *name = AXIS2_QNAME_TO_STRING(qname, env);
        axis2_hash_set (om_element_impl->attributes, 
                        name, AXIS2_HASH_KEY_STRING,
                        attribute);
    }
    return ((qname) ? AXIS2_SUCCESS : AXIS2_FAILURE);
}

axis2_om_attribute_t * AXIS2_CALL
axis2_om_element_get_attribute (axis2_om_element_t *om_element,
                                axis2_env_t **env,
                                axis2_qname_t *qname)
{
    axis2_om_element_impl_t *element_impl = NULL;
    axis2_char_t *name = NULL;
    axis2_om_attribute_t *attr = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    
    element_impl = AXIS2_INTF_TO_IMPL(om_element);
    name = AXIS2_QNAME_TO_STRING(qname, env);

    if ((element_impl->attributes) && name )
    {
        attr = (axis2_om_attribute_t*) (axis2_hash_get(element_impl->attributes, name, AXIS2_HASH_KEY_STRING));
    }
    return attr;
}

axis2_status_t AXIS2_CALL
axis2_om_element_free (axis2_om_element_t *om_element,
                       axis2_env_t **env)
{
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_om_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!om_element)
        return AXIS2_FAILURE;
    
    element_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    if (NULL != element_impl->localname)
    {
        AXIS2_FREE ((*env)->allocator,element_impl->localname);
        element_impl->localname = NULL;
    }
    if (element_impl->ns)
    {
            /* it is the responsibility of the element where the namespace is declared to free it */
    }
    if (NULL != element_impl->attributes)
    {
        axis2_hash_index_t *hi;
        void *val = NULL;
            
        for (hi = axis2_hash_first (element_impl->attributes, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
               axis2_hash_this (hi, NULL, NULL, &val);

               if (val)
               {
                   AXIS2_OM_ATTRIBUTE_FREE ((axis2_om_attribute_t *)val, env);
               }
               val = NULL;
        }
        axis2_hash_free (element_impl->attributes, env);
        element_impl->attributes = NULL;
    }
        
    if (NULL != element_impl->namespaces)
    {
        axis2_hash_index_t *hi;
        void *val = NULL;
        for (hi = axis2_hash_first (element_impl->namespaces, env); hi;
                hi = axis2_hash_next ( env, hi))
        {
            axis2_hash_this (hi, NULL, NULL, &val);
            if (NULL != val)
            {
                 AXIS2_OM_NAMESPACE_FREE ((axis2_om_namespace_t *)val, env);
                 val = NULL;
            }
        }
        axis2_hash_free (element_impl->namespaces, env);
        element_impl->namespaces = NULL;  
    }
    if(NULL != element_impl->qname)
    {
        AXIS2_QNAME_FREE(element_impl->qname, env);
        element_impl->qname = NULL;
    }
    if(NULL != element_impl->children_iter)
    {
        AXIS2_OM_CHILDREN_ITERATOR_FREE(element_impl->children_iter, env);
        element_impl->children_iter = NULL;
    }
    if(NULL != element_impl->child_ele_iter)
    {
        AXIS2_OM_CHILD_ELEMENT_ITERATOR_FREE(element_impl->child_ele_iter, env);
        element_impl->child_ele_iter = NULL;
    }
    if(NULL != element_impl->children_qname_iter)
    {
        AXIS2_OM_CHILDREN_QNAME_ITERATOR_FREE(element_impl->children_qname_iter, env);
        element_impl->children_qname_iter = NULL;
    }
    if(NULL != element_impl->text_value)
    {
        AXIS2_FREE((*env)->allocator, element_impl->text_value);
        element_impl->text_value = NULL;
    }
    
    if(NULL != om_element->ops)
    {
        AXIS2_FREE ((*env)->allocator, om_element->ops);
        om_element->ops = NULL;
    }
    
   
    AXIS2_FREE ((*env)->allocator, element_impl);
    
    return status;
}

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_start_part (axis2_om_element_t *om_element,
                                       axis2_env_t **env,
                                       axis2_om_output_t *om_output,
                                       axis2_om_node_t *ele_node)
{
    int status = AXIS2_SUCCESS;
    axis2_om_element_impl_t *ele_impl = NULL;
    
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);
    
    ele_impl = AXIS2_INTF_TO_IMPL(om_element);

    if (NULL != ele_impl->ns )
    {
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;
        
        uri = AXIS2_OM_NAMESPACE_GET_URI(ele_impl->ns, env);
        prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(ele_impl->ns, env);
        
        if((NULL != uri) && (NULL != prefix) && (AXIS2_STRCMP(prefix, "") != 0))
        {
            status = axis2_om_output_write (om_output, env,
                        AXIS2_OM_ELEMENT, 3, ele_impl->localname, 
                        uri, prefix);
        
        }
        else if(NULL != uri)
        {
            status = axis2_om_output_write (om_output, env,
                        AXIS2_OM_ELEMENT, 2, ele_impl->localname, uri);
        }                        
    }                                   
    else
    {
        status = axis2_om_output_write (om_output, env,
                                    AXIS2_OM_ELEMENT, 1, ele_impl->localname);

    }
    if (NULL != ele_impl->attributes)
    {
        axis2_hash_index_t *hi;
        void *val;
        for (hi = axis2_hash_first (ele_impl->attributes, env); hi;
             hi = axis2_hash_next (env, hi))
        {
            axis2_hash_this (hi, NULL, NULL, &val);

            if (val)
                status =
                    AXIS2_OM_ATTRIBUTE_SERIALIZE ((axis2_om_attribute_t *)val,
                                                  env, om_output);
            else
            {
                status = AXIS2_FAILURE;
            }
        }
    }

    
    if (NULL != ele_impl->namespaces)
    {
        axis2_hash_index_t *hi;
        void *val;
        for (hi = axis2_hash_first ( ele_impl->namespaces, env); hi;
             hi = axis2_hash_next (env, hi))
        {
            axis2_hash_this (hi, NULL, NULL, &val);

            if (val)
                status = 
                    AXIS2_OM_NAMESPACE_SERIALIZE((axis2_om_namespace_t *)val,
                                                  env, om_output);
            else
            {
                status = AXIS2_FAILURE;
            }
        }
    }
    if((NULL == ele_impl->namespaces) && (NULL == ele_impl->ns))
    {
        axis2_om_namespace_t *default_ns = NULL;
        default_ns = AXIS2_OM_ELEMENT_GET_DEFAULT_NAMESPACE(om_element, 
                        env, ele_node); 
        if(NULL != default_ns)
        {
            AXIS2_OM_NAMESPACE_SERIALIZE(default_ns, env, om_output);        
        }                           
    }
    return status;
}

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_end_part (axis2_om_element_t *om_element,
                                     axis2_env_t **env,
                                     axis2_om_output_t * om_output)
{
    int status = AXIS2_SUCCESS;
    AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);
    status = axis2_om_output_write (om_output, env, AXIS2_OM_ELEMENT, 0);
    return status;
}


axis2_char_t* AXIS2_CALL
axis2_om_element_get_localname(axis2_om_element_t *om_element,
                               axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->localname;
}                               

axis2_status_t AXIS2_CALL
axis2_om_element_set_localname(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               const axis2_char_t *localname)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    if(om_ele_impl->localname)
    {
        AXIS2_FREE((*env)->allocator, om_ele_impl->localname);
        om_ele_impl->localname = NULL;
    }
    om_ele_impl->localname = (axis2_char_t*)AXIS2_STRDUP(localname,env);
    if(!(om_ele_impl->localname))
    {
        return AXIS2_FAILURE;    
    }
    return AXIS2_SUCCESS;        
}                               
        
axis2_om_namespace_t *AXIS2_CALL
axis2_om_element_get_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               axis2_om_node_t *ele_node)
                               
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_om_namespace_t *ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    if(NULL != om_ele_impl->ns)
    {
        ns = om_ele_impl->ns;
    }
    else
    {   /* TODO need to change get_namespace and get_qname methods so
            that default namespace is handled properly */
        ns = axis2_om_element_get_default_namespace(
                om_element, env, ele_node);
                       
    }
    return ns;
}                               
                                                          
axis2_status_t AXIS2_CALL 
axis2_om_element_set_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               axis2_om_namespace_t *ns,
                               axis2_om_node_t *node)
{
    axis2_om_namespace_t *om_ns = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,ns , AXIS2_FAILURE);
    om_ns = axis2_om_element_find_namespace(om_element, env, node,
                                AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                AXIS2_OM_NAMESPACE_GET_PREFIX(ns , env));
    if(!om_ns)
    {
        status = axis2_om_element_declare_namespace(om_element, env, node, ns);
        if(status == AXIS2_FAILURE)
            return AXIS2_FAILURE;
        AXIS2_INTF_TO_IMPL(om_element)->ns = ns;
    }
    else
    {
        AXIS2_INTF_TO_IMPL(om_element)->ns = om_ns;
    }
    return AXIS2_SUCCESS;                                
}


axis2_hash_t * AXIS2_CALL
axis2_om_element_get_all_attributes(axis2_om_element_t *om_element,
                                    axis2_env_t **env)
{

    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->attributes;
} 

axis2_hash_t* AXIS2_CALL
axis2_om_element_get_namespaces
                     (axis2_om_element_t *om_element,
                      axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->namespaces;
} 

axis2_qname_t* AXIS2_CALL
axis2_om_element_get_qname(axis2_om_element_t *om_element,
                            axis2_env_t **env,
                            axis2_om_node_t *ele_node)
{
    axis2_om_namespace_t *ns =  NULL;
    axis2_om_element_impl_t *om_element_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);    
    
    if(om_element_impl->qname)
    {
            return om_element_impl->qname;
    }
    else
    {
        ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_element, env, ele_node);
        if(ns)
        {            
            if(AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env))
            {
                om_element_impl->qname = axis2_qname_create(env ,
                                            om_element_impl->localname,
                                            AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                            AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env));
            }
            else
            {
                om_element_impl->qname = axis2_qname_create(env,
                                            om_element_impl->localname,
                                            AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                            NULL);            
            }
        
        }
        else
        {
            om_element_impl->qname = axis2_qname_create(env, 
                                                om_element_impl->localname,
                                                NULL,  NULL);
        }
    }
    return om_element_impl->qname;
}

axis2_om_children_iterator_t* AXIS2_CALL
axis2_om_element_get_children(axis2_om_element_t *om_element,
                              axis2_env_t **env,
                              axis2_om_node_t *element_node)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    if(om_ele_impl->children_iter)
    {
        return om_ele_impl->children_iter;
    }
    else
    {
        om_ele_impl->children_iter = axis2_om_children_iterator_create(env,
                    AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env));
        return om_ele_impl->children_iter;                     
    }
    return NULL;
}

axis2_om_children_qname_iterator_t* AXIS2_CALL
axis2_om_element_get_children_with_qname(axis2_om_element_t *om_element,
                                         axis2_env_t **env,
                                         axis2_qname_t *element_qname,
                                         axis2_om_node_t *element_node)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    if(NULL != om_ele_impl->children_qname_iter)
    {
        AXIS2_OM_CHILDREN_QNAME_ITERATOR_FREE(om_ele_impl->children_qname_iter, env);
        om_ele_impl->children_qname_iter = NULL;
    }
    om_ele_impl->children_qname_iter =  axis2_om_children_qname_iterator_create(env,
                AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env),
                element_qname);
    return om_ele_impl->children_qname_iter;                
}

axis2_om_element_t* AXIS2_CALL
axis2_om_element_get_first_child_with_qname(axis2_om_element_t *om_element,
                                            axis2_env_t **env,
                                            axis2_qname_t *element_qname,
                                            axis2_om_node_t *element_node,
                                            axis2_om_node_t **child_node)
{
    axis2_om_node_t *om_node = NULL;
    axis2_om_children_qname_iterator_t *children_iterator = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_qname, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    
    children_iterator = axis2_om_children_qname_iterator_create(env,
                        AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env),
                        element_qname);
    if (!children_iterator)
        return NULL;

    if(AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(children_iterator, env))
    {
        om_node =   AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(children_iterator, env); 
                
    }
    if(om_node && (AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) == AXIS2_OM_ELEMENT))
    {
        AXIS2_OM_CHILDREN_QNAME_ITERATOR_FREE(children_iterator, env);
        
        if(child_node){ *child_node = om_node; }
        return (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
    }
    else
    {
        AXIS2_OM_CHILDREN_QNAME_ITERATOR_FREE(children_iterator, env);
        return NULL;    
    }        
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_element_remove_attribute(axis2_om_element_t *om_element, 
                                  axis2_env_t **env,
                                  axis2_om_attribute_t *om_attribute)
{
    axis2_om_element_impl_t *om_element_impl = NULL;
    axis2_qname_t *qname = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_attribute, AXIS2_FAILURE);
    
    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    qname = AXIS2_OM_ATTRIBUTE_GET_QNAME(om_attribute, env);
    if (qname && (om_element_impl->attributes))
    {    
        axis2_char_t *name = NULL;
        name = AXIS2_QNAME_TO_STRING(qname, env);
        if(name)
        {
            axis2_hash_set (om_element_impl->attributes, name, 
                AXIS2_HASH_KEY_STRING, NULL);
            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_FAILURE;
}

axis2_om_element_t* AXIS2_CALL
axis2_om_element_get_first_element(axis2_om_element_t *om_element,
                                    axis2_env_t **env,
                                    axis2_om_node_t *element_node,
                                    axis2_om_node_t **first_ele_node)
{
    axis2_om_node_t *temp_node = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
   
    temp_node = AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env);
    while(NULL != temp_node )
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(temp_node, env) == AXIS2_OM_ELEMENT)
        {
            if(first_ele_node){            *first_ele_node = temp_node;}
            return (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(temp_node, env);
        }
        else
        {
            temp_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(temp_node, env);
        }
    }     
    return NULL;
}                                    
axis2_char_t* AXIS2_CALL
axis2_om_element_get_text(axis2_om_element_t *om_element,
                          axis2_env_t **env,
                          axis2_om_node_t *element_node)
{
    axis2_om_element_impl_t *element_impl = NULL;
    axis2_char_t *dest = NULL;
    axis2_char_t* temp_text = NULL;
    axis2_om_text_t* text_node = NULL;
    axis2_om_node_t* temp_node = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    element_impl = AXIS2_INTF_TO_IMPL(om_element);

    if(NULL != element_impl->text_value)
    {
        AXIS2_FREE((*env)->allocator, element_impl->text_value);
        element_impl->text_value = NULL;
    }

    temp_node = AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env);
    
    while(NULL != temp_node)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(temp_node, env) == AXIS2_OM_TEXT)
        {
            int dest_len = 0;
            int curr_len = 0;
            axis2_char_t *temp_dest = NULL;
            
            text_node = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(temp_node, env); 
            if(text_node)
            {
                temp_text = AXIS2_OM_TEXT_GET_VALUE(text_node, env);
                if(dest && temp_text && AXIS2_STRCMP(temp_text, "") != 0)
                {
                    dest_len = AXIS2_STRLEN(dest);
                    curr_len = dest_len + AXIS2_STRLEN(temp_text);
                    temp_dest = AXIS2_MALLOC((*env)->allocator, 
                                    (curr_len +1 )*sizeof(axis2_char_t));
                    
                    memcpy(temp_dest, dest, dest_len*sizeof(axis2_char_t));
                    memcpy((temp_dest + dest_len * sizeof(axis2_char_t)), 
                                temp_text, curr_len - dest_len);
                    
                    temp_dest[curr_len] = '\0';
                
                    AXIS2_FREE((*env)->allocator, dest);
                    dest = NULL;
                    dest = temp_dest;
                }
                else if(!dest && temp_text && AXIS2_STRCMP(temp_text, "") != 0)
                {
                    dest = AXIS2_STRDUP(temp_text, env); 
                }
            }
        }
        temp_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(temp_node, env);
    }
    
    element_impl->text_value = dest;
    return element_impl->text_value;
}                          

axis2_status_t AXIS2_CALL
axis2_om_element_set_text(axis2_om_element_t *om_element,
                          axis2_env_t **env,
                          axis2_char_t *text,
                          axis2_om_node_t *element_node)
{
    axis2_om_node_t* temp_node = NULL;
    axis2_om_text_t* om_text = NULL;
    axis2_om_node_t* node_to_free = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element_node, AXIS2_FAILURE);
    
    temp_node = AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env);
    while( temp_node != NULL)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(temp_node, env) == AXIS2_OM_TEXT)
        {
            node_to_free = AXIS2_OM_NODE_DETACH(temp_node, env);
            AXIS2_OM_NODE_FREE_TREE(node_to_free, env);
        }
        temp_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(temp_node, env);    
    } 
    temp_node = NULL;
       
    om_text = axis2_om_text_create(env, NULL, text, &temp_node);
    AXIS2_OM_NODE_ADD_CHILD(element_node, env, temp_node);
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
axis2_om_element_to_string(axis2_om_element_t *om_element,
                           axis2_env_t **env,
                           axis2_om_node_t *element_node)
{
    int status = AXIS2_SUCCESS;
    axis2_om_output_t *om_output = NULL;
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_char_t *xml = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0);
    if(!xml_writer)
        return NULL;
    
    om_output = axis2_om_output_create(env, xml_writer);
    if(!om_output)
        return NULL;
    
    status = AXIS2_OM_NODE_SERIALIZE(element_node, env, om_output);
    if(status == AXIS2_SUCCESS)
    {
        xml = AXIS2_XML_WRITER_GET_XML(xml_writer, env);
    }
    AXIS2_OM_OUTPUT_FREE(om_output, env);
    return xml;                
}

axis2_om_child_element_iterator_t * AXIS2_CALL
axis2_om_element_get_child_elements(axis2_om_element_t *om_element,
                                    axis2_env_t **env,
                                    axis2_om_node_t *element_node)
{
   axis2_om_element_impl_t *om_ele_impl = NULL;
   axis2_om_node_t *first_node = NULL;
   axis2_om_element_t *ele = NULL;
   AXIS2_ENV_CHECK(env, NULL);
   AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
   om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
   ele = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(om_element, env, element_node, &first_node);
   if(om_ele_impl->child_ele_iter)
   {
        return om_ele_impl->child_ele_iter;
   }
   else if(ele && first_node)
   {
        om_ele_impl->child_ele_iter = axis2_om_child_element_iterator_create(env, first_node);
        return om_ele_impl->child_ele_iter;
   }
   return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_element_build(axis2_om_element_t *om_ele,
                       axis2_env_t **env,
                       axis2_om_node_t *om_ele_node)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_om_stax_builder_t *builder = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_PARAM_CHECK((*env)->error, om_ele_node, AXIS2_FAILURE);
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_ele_node, env) != AXIS2_OM_ELEMENT)
        return AXIS2_FAILURE;
        
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_ele);
    
    builder = axis2_om_node_get_builder(om_ele_node, env);
    if(!builder)
        return AXIS2_FAILURE;
    while(!AXIS2_OM_NODE_IS_COMPLETE(om_ele_node, env) && 
            !AXIS2_OM_STAX_BUILDER_IS_COMPLETE(builder, env))
    {
        void *value  = NULL;
        value   = AXIS2_OM_STAX_BUILDER_NEXT(builder, env);
        if(!value)
            return AXIS2_FAILURE;
    }            
    return AXIS2_SUCCESS;
}                       

axis2_om_namespace_t* AXIS2_CALL
axis2_om_element_get_default_namespace(axis2_om_element_t *om_element,
                                        axis2_env_t **env,
                                        axis2_om_node_t *element_node)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_namespace_t *default_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    if(NULL != om_ele_impl->namespaces)
    {
        default_ns = axis2_hash_get(om_ele_impl->namespaces, "",
            AXIS2_HASH_KEY_STRING);
        if(NULL != default_ns)
        {
            return default_ns;
        }             
    }
    
    parent_node = AXIS2_OM_NODE_GET_PARENT(element_node, env);
    if((NULL != parent_node) && 
        (AXIS2_OM_NODE_GET_NODE_TYPE(parent_node, env) == AXIS2_OM_ELEMENT))
    {
        axis2_om_element_t *parent_ele = NULL;
        parent_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
        if(NULL != parent_ele)
        {
            return axis2_om_element_get_default_namespace(parent_ele, env, parent_node);
        }            
    } 
    return NULL;
}                                        
                                    
/**
* declared a default namespace explicitly 
*/     
axis2_om_namespace_t* AXIS2_CALL 
axis2_om_element_declare_default_namespace(axis2_om_element_t *om_element,
                                           axis2_env_t **env,
                                           axis2_char_t *uri)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_om_namespace_t *default_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    default_ns = axis2_om_namespace_create(env, uri, "");
    if(!default_ns)
    {
        return NULL;
    }
    if(NULL != om_ele_impl->namespaces)
    {
        om_ele_impl->namespaces = axis2_hash_make(env);
        if(!(om_ele_impl->namespaces))
        {
            return NULL;
        }
    }
    
    axis2_hash_set(om_ele_impl->namespaces, "", 
        AXIS2_HASH_KEY_STRING, default_ns);
    return default_ns;        
}                                           
                        
/** 
* checks for the namespace in the context of this element 
* with the given prefix 
*/

axis2_om_namespace_t* AXIS2_CALL 
axis2_om_element_find_namespace_uri(axis2_om_element_t *om_element,
                                    axis2_env_t **env,
                                    axis2_char_t *prefix,
                                    axis2_om_node_t *element_node)
{
    axis2_om_element_impl_t *om_ele_impl = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_namespace_t *ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    AXIS2_PARAM_CHECK((*env)->error, prefix, NULL);
    
    om_ele_impl = AXIS2_INTF_TO_IMPL(om_element);
    if(NULL != om_ele_impl->namespaces)
    {
        ns = axis2_hash_get(om_ele_impl->namespaces, prefix,
            AXIS2_HASH_KEY_STRING);
        if(NULL != ns)
        {
            return ns;
        }             
    }
    
    parent_node = AXIS2_OM_NODE_GET_PARENT(element_node, env);
    if((NULL != parent_node) && 
        (AXIS2_OM_NODE_GET_NODE_TYPE(parent_node, env) == AXIS2_OM_ELEMENT))
    {
        axis2_om_element_t *parent_ele = NULL;
        parent_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
        if(NULL != parent_ele)
        {
            return axis2_om_element_find_namespace_uri(parent_ele, env, prefix, parent_node);
        }            
    } 
    return NULL;
}                                    

axis2_char_t* AXIS2_CALL
axis2_om_element_get_attribute_value (axis2_om_element_t *om_element,
                                axis2_env_t **env,
                                axis2_qname_t *qname)
{
    axis2_om_element_impl_t *element_impl = NULL;
    axis2_char_t *name = NULL;
    axis2_om_attribute_t *attr = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    
    element_impl = AXIS2_INTF_TO_IMPL(om_element);
    name = AXIS2_QNAME_TO_STRING(qname, env);

    if ((NULL != element_impl->attributes) && (NULL != name) )
    {
        attr = (axis2_om_attribute_t*) axis2_hash_get(element_impl->attributes,
                    name, AXIS2_HASH_KEY_STRING);
        if(NULL != attr)
        {
            return AXIS2_OM_ATTRIBUTE_GET_VALUE(attr, env);
        }
    }
    return NULL;
}
