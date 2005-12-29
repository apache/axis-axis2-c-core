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
#include <axis2_om_attribute.h>
#include <axis2_om_namespace.h>
#include <axis2_xml_writer.h>
#include <string.h>

                                 
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

axis2_status_t AXIS2_CALL
axis2_om_element_free (axis2_om_element_t *element,                                     
                       axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_start_part(axis2_om_element_t * element,
                                      axis2_env_t **env,
                                      axis2_om_output_t *om_output);

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
                               axis2_env_t **env);
                                                          
axis2_status_t AXIS2_CALL 
axis2_om_element_set_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               axis2_om_namespace_t *ns,
                               axis2_om_node_t *node); 

axis2_hash_t * AXIS2_CALL
axis2_om_element_get_all_attributes(axis2_om_element_t *om_element,
                                    axis2_env_t **env);
                                                                                                                                                                                                                               
axis2_hash_t* AXIS2_CALL 
axis2_om_element_get_all_namespaces(axis2_om_element_t *om_element,
                                    axis2_env_t **env);                                                                                                                  
                                     
                                     
axis2_qname_t* AXIS2_CALL
axis2_om_element_get_qname(axis2_om_element_t *om_element,
                            axis2_env_t **env);

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
    
}axis2_om_element_impl_t;

/************************************Macro *****************************/


#define AXIS2_INTF_TO_IMPL(om_element) ((axis2_om_element_impl_t*)om_element)
#define AXIS2_IMPL_TO_INTF(element) &(element->om_element)

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
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    (*node) = axis2_om_node_create (env);
    if (!(*node))
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    element = (axis2_om_element_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                             sizeof (axis2_om_element_impl_t));

    if (!element)
    {
        AXIS2_FREE ((*env)->allocator, (*node));
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    element->ns = NULL;
    element->localname = NULL;
    element->attributes = NULL;
    element->namespaces = NULL;
    element->qname = NULL;

    element->localname = (axis2_char_t *) AXIS2_STRDUP(localname,env);
    if (!element->localname)
    {
        AXIS2_FREE ((*env)->allocator, element);
        AXIS2_FREE ((*env)->allocator, (*node));
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    if (parent)
    {
        AXIS2_OM_NODE_SET_PARENT((*node), env, parent);
        AXIS2_OM_NODE_ADD_CHILD((*node), env, parent);
    }
    
    AXIS2_OM_NODE_SET_BUILD_STATUS((*node), env, AXIS2_FALSE);
    AXIS2_OM_NODE_SET_NODE_TYPE((*node), env, AXIS2_OM_ELEMENT);
    AXIS2_OM_NODE_SET_DATA_ELEMENT((*node), env, element);

    if (ns)
    {
        element->ns =
            axis2_om_element_find_namespace (AXIS2_IMPL_TO_INTF(element), env, *node,
                                             AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                             AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env));
        if (!(element->ns))
        {
            if (axis2_om_element_declare_namespace(&(element->om_element), 
                    env, *node, ns) == AXIS2_SUCCESS)
                element->ns = ns;
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
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
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
    element->om_element.ops->get_all_attributes =
        axis2_om_element_get_all_attributes;   
    element->om_element.ops->get_all_namespaces =
        axis2_om_element_get_all_namespaces;
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
                                   
    return &(element->om_element);
}

AXIS2_DECLARE(axis2_om_element_t *)
axis2_om_element_create_with_qname (axis2_env_t **env,
                                    axis2_om_node_t *parent,
                                    axis2_qname_t *qname,
                                    axis2_om_node_t ** node)
{
    axis2_om_element_t *element = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    if (!qname || !(*node))
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;           
    }

    /* TODO:handle namespace in the following */
    element = axis2_om_element_create (env, parent,
              AXIS2_QNAME_GET_LOCALPART(qname, env), NULL, node);
    if (*node)
    {
        axis2_om_element_t *ele = 
            ((axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT((*node), env));
        if ( AXIS2_INTF_TO_IMPL(ele)->ns)
        {
            axis2_om_namespace_t *ns = AXIS2_INTF_TO_IMPL(ele)->ns;
                
            AXIS2_INTF_TO_IMPL(element)->ns = 
                axis2_om_element_find_namespace (element, env, *node,
                                                 AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                                 AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env));
                                                 
            if (!(AXIS2_INTF_TO_IMPL(element)->ns))
            {
                if (axis2_om_element_declare_namespace(element, env, *node, ns) == AXIS2_SUCCESS)
                    (AXIS2_INTF_TO_IMPL(element)->ns) = ns;
            }
        }
    }

    return element;
}

axis2_om_namespace_t * AXIS2_CALL
axis2_om_element_find_namespace (axis2_om_element_t *ele,
                                 axis2_env_t **env,
                                 axis2_om_node_t * node,
                                 const axis2_char_t * uri,
                                 const axis2_char_t * prefix)
{
    void *ns = NULL;
    axis2_hash_index_t *hashindex;
    axis2_om_element_t *element = NULL;
    axis2_om_node_t *parent = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    if (!node)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    if (!AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env) || 
            AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
    {
        /* wrong element type or null node */
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NODE_TYPE);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    element = (axis2_om_element_t *) AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
    
    if (AXIS2_INTF_TO_IMPL(element)->namespaces)
    if (!prefix || axis2_strcmp ( prefix, "") == 0)
    {   
        for (hashindex = axis2_hash_first (AXIS2_INTF_TO_IMPL(element)->namespaces, env);
             hashindex; hashindex = axis2_hash_next (env, hashindex))
        {
            axis2_hash_this (hashindex, NULL, NULL, &ns);
            if (ns && 
                axis2_strcmp (AXIS2_OM_NAMESPACE_GET_URI(
                    (axis2_om_namespace_t *) (ns), env),uri) == 0)
            {
              
		        AXIS2_FREE ((*env)->allocator, hashindex);
                return (axis2_om_namespace_t *) (ns);
            }
        }
        ns = NULL;
    }

    if (AXIS2_INTF_TO_IMPL(element)->namespaces && prefix)
    {    ns = axis2_hash_get (AXIS2_INTF_TO_IMPL(element)->namespaces, prefix,
                             AXIS2_HASH_KEY_STRING);
        if (ns)
        {
            return (axis2_om_namespace_t*)ns;
        }                             
    }
    parent = AXIS2_OM_NODE_GET_PARENT(node, env);                       
    
    if ((parent != NULL) &&
             (AXIS2_OM_NODE_GET_NODE_TYPE(parent, env) == AXIS2_OM_ELEMENT))
    {
        return axis2_om_element_find_namespace (
                (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env),
                    env, parent, uri, prefix);
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
    axis2_om_element_t *element = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!node || !ns)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    declared_ns = axis2_om_element_find_namespace(ele, env, node, 
                                              AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                              AXIS2_OM_NAMESPACE_GET_PREFIX(ns ,env));

    if (declared_ns)
    {
        /*Namespace already declared, so return */
        return AXIS2_SUCCESS;
    }

    element = (axis2_om_element_t *) AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);

    if (!element)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }

    if (!(AXIS2_INTF_TO_IMPL(element)->namespaces))
    {
        AXIS2_INTF_TO_IMPL(element)->namespaces = axis2_hash_make (env);
        if (!(AXIS2_INTF_TO_IMPL(element)->namespaces))
            return AXIS2_FAILURE;
    }

    if (AXIS2_OM_NAMESPACE_GET_PREFIX(ns,env))
    {   axis2_hash_set (AXIS2_INTF_TO_IMPL(element)->namespaces,
                        AXIS2_OM_NAMESPACE_GET_PREFIX(ns,env),
                        AXIS2_HASH_KEY_STRING, ns);
    }
    else
    {
        axis2_hash_set (AXIS2_INTF_TO_IMPL(element)->namespaces,
                        "default",  AXIS2_HASH_KEY_STRING,  ns);
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    
    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    if (!(om_element_impl->namespaces))
    {
        return NULL;
    }
    if (!prefix || AXIS2_STRCMP(prefix, "") == 0)
    {
        for (hash_index = axis2_hash_first (om_element_impl->namespaces, env);
             hash_index; hash_index = axis2_hash_next (env, hash_index))
        {
            axis2_hash_this (hash_index, NULL, NULL, &ns);
            if (AXIS2_STRCMP(AXIS2_OM_NAMESPACE_GET_URI(
                    (axis2_om_namespace_t *)(ns), env), uri) == 0)
            {
		AXIS2_FREE ((*env)->allocator, hash_index);
                return (axis2_om_namespace_t *) (ns);
            }
        }
        ns = NULL;
        return NULL;
    }
    
    ns = axis2_hash_get (om_element_impl->namespaces, prefix, AXIS2_HASH_KEY_STRING);
    if (ns)
        return (axis2_om_namespace_t *) ns;
    else
        return NULL;
}


axis2_om_namespace_t * AXIS2_CALL
axis2_om_element_find_namespace_with_qname (axis2_om_element_t *element,
                                            axis2_env_t **env,
                                            axis2_om_node_t *node,
                                            axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, NULL);
   
    if (!node || !qname)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, 
                                     AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attribute, AXIS2_FAILURE);

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
            AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(om_element, env, 
                                               element_node, om_namespace); 
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
        axis2_hash_set (om_element_impl->attributes, 
                        qname, sizeof (axis2_qname_t),
                        attribute);

    return ((qname) ? AXIS2_SUCCESS : AXIS2_FAILURE);
}

axis2_om_attribute_t * AXIS2_CALL
axis2_om_element_get_attribute (axis2_om_element_t *om_element,
                                axis2_env_t **env,
                                axis2_qname_t *qname)
{
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    
    if (!qname)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, 
                                     AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    return (axis2_om_attribute_t*)
                (axis2_hash_get(AXIS2_INTF_TO_IMPL(om_element)->attributes, qname,
                                sizeof (axis2_qname_t)));
}

axis2_status_t AXIS2_CALL
axis2_om_element_free (axis2_om_element_t *om_element,
                       axis2_env_t **env)
{
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_om_element_impl_t *element_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    if (element_impl->localname)
    {
        AXIS2_FREE ((*env)->allocator,element_impl->localname);
    }
    if (element_impl->ns)
    {
            /* it is the responsibility of the element where the namespace is declared to free it */
    }
    if (element_impl->attributes)
    {
        axis2_hash_index_t *hi;
        void *val = NULL;
            
        for (hi = axis2_hash_first (element_impl->attributes, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
               axis2_hash_this (hi, NULL, NULL, &val);

                if (val)
                   status = AXIS2_OM_ATTRIBUTE_FREE ((axis2_om_attribute_t *)val, env);
                else
                {
                   status = AXIS2_FAILURE;
                }
                 val = NULL;
                   
            }
           axis2_hash_free (AXIS2_INTF_TO_IMPL(om_element)->attributes, env);
        }
        if (element_impl->namespaces)
        {
            axis2_hash_index_t *hi;
            void *val = NULL;
            
            for (hi = axis2_hash_first (element_impl->namespaces, env); hi;
                 hi = axis2_hash_next ( env, hi))
            {
               axis2_hash_this (hi, NULL, NULL, &val);

                if (val)
                AXIS2_OM_NAMESPACE_FREE ((axis2_om_namespace_t *)val, env);
                val = NULL;
                   
            }
         
            axis2_hash_free (element_impl->namespaces, env);
            /*TODO: free namespaces */
           /* need to iterate and free individual namespaces */
        }
        if(element_impl->qname)
            AXIS2_QNAME_FREE(element_impl->qname, env);
        AXIS2_FREE ((*env)->allocator, om_element->ops);
        AXIS2_FREE ((*env)->allocator, element_impl);
  
    return status;
}

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_start_part (axis2_om_element_t *om_element,
                                       axis2_env_t **env,
                                       axis2_om_output_t *om_output)
{
    int status = AXIS2_SUCCESS;
   
    axis2_om_element_impl_t *ele_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
     
    if (!om_output)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,
                                     AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);                                     
        return AXIS2_FAILURE;
    }
    ele_impl = AXIS2_INTF_TO_IMPL(om_element);

    if (ele_impl->ns &&
        AXIS2_OM_NAMESPACE_GET_URI(ele_impl->ns, env) &&
        AXIS2_OM_NAMESPACE_GET_PREFIX(ele_impl->ns, env))
    {
        status = axis2_om_output_write (om_output, env,
                              AXIS2_OM_ELEMENT, 3,
                              ele_impl->localname, 
                              AXIS2_OM_NAMESPACE_GET_URI(ele_impl->ns, env),
                              AXIS2_OM_NAMESPACE_GET_PREFIX(ele_impl->ns, env));
    }                                   
    else if (ele_impl->ns && 
        AXIS2_OM_NAMESPACE_GET_URI(ele_impl->ns, env) &&
            !AXIS2_OM_NAMESPACE_GET_PREFIX(ele_impl->ns, env))
    {  
        status = 
            axis2_om_output_write (om_output, env,
                                AXIS2_OM_ELEMENT, 2,
                                ele_impl->localname,
                                AXIS2_OM_NAMESPACE_GET_URI(ele_impl->ns, env));
    }                                   
    else
    {
        status = axis2_om_output_write (om_output, env,
                                    AXIS2_OM_ELEMENT, 1, ele_impl->localname);

    }
    if (ele_impl->attributes)
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

    
    if (ele_impl->namespaces)
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

    return status;
}

axis2_status_t AXIS2_CALL
axis2_om_element_serialize_end_part (axis2_om_element_t *om_element,
                                     axis2_env_t **env,
                                     axis2_om_output_t * om_output)
{
    int status = AXIS2_SUCCESS;
    if (!om_output)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error ,
                                     AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error , AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    status =
        axis2_om_output_write (om_output, env, AXIS2_OM_ELEMENT, 0);
    return status;
}


axis2_char_t* AXIS2_CALL
axis2_om_element_get_localname(axis2_om_element_t *om_element,
                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->localname;
}                               

axis2_status_t AXIS2_CALL
axis2_om_element_set_localname(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               const axis2_char_t *localname)
{
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_element)->localname = 
        (axis2_char_t*)AXIS2_STRDUP(localname,env);
    return AXIS2_SUCCESS;        
}                               
        
axis2_om_namespace_t *AXIS2_CALL
axis2_om_element_get_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->ns;
}                               
                                                          
axis2_status_t AXIS2_CALL 
axis2_om_element_set_namespace(axis2_om_element_t *om_element,
                               axis2_env_t **env,
                               axis2_om_namespace_t *ns,
                               axis2_om_node_t *node)
{
    axis2_om_namespace_t *om_ns = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,ns , AXIS2_FAILURE);
    om_ns = axis2_om_element_find_namespace(om_element, env, node,
                                AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                                AXIS2_OM_NAMESPACE_GET_PREFIX(ns , env));
    if(!om_ns)
    {
        status = axis2_om_element_declare_namespace(om_element, env, node, ns);
        AXIS2_INTF_TO_IMPL(om_element)->ns = ns;
    }
    else
        AXIS2_INTF_TO_IMPL(om_element)->ns = om_ns;
    
    return AXIS2_SUCCESS;                                
}


axis2_hash_t * AXIS2_CALL
axis2_om_element_get_all_attributes(axis2_om_element_t *om_element,
                                    axis2_env_t **env)
{

    AXIS2_FUNC_PARAM_CHECK(om_element,env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->attributes;
} 

axis2_hash_t* AXIS2_CALL
axis2_om_element_get_all_namespaces
                                (axis2_om_element_t *om_element,
                                 axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_element,env, NULL);
    return AXIS2_INTF_TO_IMPL(om_element)->namespaces;
} 

axis2_qname_t* AXIS2_CALL
axis2_om_element_get_qname(axis2_om_element_t *om_element,
                            axis2_env_t **env)
{
    axis2_om_namespace_t *ns =  NULL;
    axis2_om_element_impl_t *om_element_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);

    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);    
    
    if(om_element_impl->qname)
    {
            return om_element_impl->qname;
    }
    else
    {
        ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_element, env);
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    return axis2_om_children_iterator_create(env,
                AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env));
    
}

axis2_om_children_qname_iterator_t* AXIS2_CALL
axis2_om_element_get_children_with_qname(axis2_om_element_t *om_element,
                                         axis2_env_t **env,
                                         axis2_qname_t *element_qname,
                                         axis2_om_node_t *element_node)
{
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_qname, NULL);
    return axis2_om_children_qname_iterator_create(env,
                AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env),
                element_qname);
     
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_qname, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    
    children_iterator = axis2_om_children_qname_iterator_create(env,
                        AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env),
                        element_qname);
     
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_attribute, AXIS2_FAILURE);
    
    om_element_impl = AXIS2_INTF_TO_IMPL(om_element);
    
    qname = AXIS2_OM_ATTRIBUTE_GET_QNAME(om_attribute, env);
    if (qname && (om_element_impl->attributes))
    {    axis2_hash_set (om_element_impl->attributes, 
                        qname, sizeof (axis2_qname_t), NULL);
    
        return AXIS2_SUCCESS;
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
   
    temp_node = AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env);
    while(temp_node != NULL)
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
    axis2_char_t* dest = NULL;
    axis2_char_t* temp_text = NULL;
    axis2_om_text_t* text_node = NULL;
    axis2_om_node_t* temp_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
    
    temp_node = AXIS2_OM_NODE_GET_FIRST_CHILD(element_node, env);
    
    while(temp_node)
    {
        
        if(AXIS2_OM_NODE_GET_NODE_TYPE(temp_node, env) == AXIS2_OM_TEXT)
        {
            int dest_len = 0;
            int curr_len = 0;
            axis2_char_t *temp_dest = NULL;
            
            text_node = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(temp_node, env); 
            temp_text = AXIS2_OM_TEXT_GET_VALUE(text_node, env);
            if(dest && temp_text && AXIS2_STRCMP(temp_text, "") != 0)
            {
                dest_len = AXIS2_STRLEN(dest);
                curr_len = dest_len + AXIS2_STRLEN(temp_text);
                temp_dest = AXIS2_MALLOC((*env)->allocator, (curr_len +1 )*sizeof(axis2_char_t));
                memcpy(temp_dest, dest, dest_len*sizeof(axis2_char_t));
                memcpy((temp_dest + dest_len * sizeof(axis2_char_t)), temp_text, curr_len - dest_len);
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
        temp_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(temp_node, env);
    }  
    return dest;
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, AXIS2_FAILURE);
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
    AXIS2_OM_NODE_ADD_CHILD(temp_node, env, element_node);
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
    AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0);
    om_output = axis2_om_output_create(env, xml_writer);
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
   axis2_om_node_t *first_node = NULL;
   axis2_om_element_t *ele = NULL;
   AXIS2_FUNC_PARAM_CHECK(om_element, env, NULL);
   AXIS2_PARAM_CHECK((*env)->error, element_node, NULL);
   ele = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(om_element, env, element_node, &first_node);
   if(ele && first_node)
   {
        return axis2_om_child_element_iterator_create(env, first_node);
   }
   else return NULL;
}


