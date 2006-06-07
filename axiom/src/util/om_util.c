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
 
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_array_list.h>


AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_uri(
        axis2_om_node_t *ele_node,
        const axis2_env_t *env,
        axis2_char_t *uri,
        axis2_om_node_t **child)
{
    axis2_om_node_t *child_node = NULL;
    
    if(!ele_node || !uri)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    child_node = AXIS2_OM_NODE_GET_FIRST_CHILD(ele_node, env);
    while(NULL != child_node)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child_node, env) == AXIS2_OM_ELEMENT)
        {
            axis2_om_element_t *child_ele = NULL;
            axis2_om_namespace_t *ns = NULL;
            
            child_ele = (axis2_om_element_t*)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(child_node, env);
            ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(child_ele, env, child_node);
            if(NULL != ns)
            {
                axis2_char_t *child_uri = NULL;
                child_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                if(NULL != child_uri && AXIS2_STRCMP(child_uri, uri) == 0)
                {
                    (*child) = child_node;
                    return child_ele;
                }
            }
        }    
        child_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(child_node, env);
    }
    return NULL;
}        
        
        
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_sibling_element_with_uri(
        axis2_om_node_t *ele_node,
        const axis2_env_t *env,
        axis2_char_t *uri,
        axis2_om_node_t **next_node)
{
    axis2_om_node_t *next_sib_node = NULL;
    if(!ele_node || !uri)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    next_sib_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while(NULL != next_sib_node)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sib_node, env) == AXIS2_OM_ELEMENT)
        {
            axis2_om_element_t *sib_ele = NULL;
            axis2_om_namespace_t *ns = NULL;
            
            sib_ele = (axis2_om_element_t*)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sib_node, env);
            ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(sib_ele, env, next_sib_node);
            if(NULL != ns)
            {
                axis2_char_t *sib_uri = NULL;
                sib_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                if(NULL != sib_uri && AXIS2_STRCMP(sib_uri, uri) == 0)
                {
                    (*next_node) = next_sib_node;
                    return sib_ele;
                }
            }
        }    
        next_sib_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sib_node, env);
    }
    return NULL;
}        
        
        

AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element(axis2_om_element_t *ele,
                                      const axis2_env_t *env,
                                      axis2_om_node_t *ele_node,
                                      axis2_om_node_t **child_node)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    
    return AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(ele, env, ele_node, child_node);
}                                      
                                      
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element(axis2_om_element_t *ele,
                                     const axis2_env_t *env,
                                     axis2_om_node_t *ele_node,
                                     axis2_om_node_t **child_node)
{
    axis2_om_node_t *last_node = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    
    last_node = AXIS2_OM_NODE_GET_LAST_CHILD(ele_node, env);
    while(NULL != last_node)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(last_node, env) == AXIS2_OM_ELEMENT)
        {
            *child_node = last_node;
            return (axis2_om_element_t*)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(last_node, env);
        
        }
        last_node = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(ele_node, env);
    }
    return NULL;    
}                                     
                                     
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_sibling_element(axis2_om_element_t *ele,
                                       const axis2_env_t *env,
                                       axis2_om_node_t *ele_node,
                                       axis2_om_node_t **next_node)
{
    axis2_om_node_t *next_sibling = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            *next_node = next_sibling;
            return (axis2_om_element_t *)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}                                       
                                       
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_localname(axis2_om_element_t *ele,
                                             const axis2_env_t *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child = NULL;
    axis2_om_node_t *next_sibling = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_om_element_t *om_ele  = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    
    child = AXIS2_OM_NODE_GET_FIRST_CHILD(ele_node, env);
    if(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(child, env);
            if(NULL != om_ele)
            {
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    *child_node = child;
                    return om_ele;
                }
            }                        
        }
        om_ele = NULL;
        child_localname = NULL;
    }
    
    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    *child_node = next_sibling;
                    return om_ele;
                }
            
            }
            om_ele = NULL;
            child_localname = NULL;
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);  
    }  
    return NULL;
}                                             
                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_localname(axis2_om_element_t *ele,
                                             const axis2_env_t *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *child_localname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    
    child = AXIS2_OM_NODE_GET_LAST_CHILD(ele_node, env);
    while(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)  
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if(NULL != om_ele)
            {
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && 
                    AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    *child_node = child;
                    return om_ele;
                }
            
            }                      
            om_ele = NULL;
            child_localname = NULL;
        }
        child = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(child, env);                
    }
    return NULL;
}                                             
                                                                                         
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_siblng_element_with_localname(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_om_node_t **next_node)
{
    axis2_om_node_t *next_sibling = NULL;
    axis2_om_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != ele_localname && AXIS2_STRCMP(localname, ele_localname) == 0)
                {
                    *next_node = next_sibling;
                    return om_ele;
                }
            }
            om_ele = NULL;
            ele_localname = NULL;                        
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_uri_localname(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *uri,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child = NULL;
    axis2_om_node_t *next_sibling = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_om_element_t *om_ele  = NULL;
    axis2_om_namespace_t *ns    = NULL;
    axis2_char_t *ns_uri        = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);
     
    child = AXIS2_OM_NODE_GET_FIRST_CHILD(ele_node, env);
    if(!child)
    {
        return NULL;
        
    }
    if(AXIS2_OM_NODE_GET_NODE_TYPE(ele_node, env) == AXIS2_OM_ELEMENT)
    {
        om_ele = (axis2_om_element_t *)
                    AXIS2_OM_NODE_GET_DATA_ELEMENT(child , env);
        if(NULL != om_ele)
        {
            child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
            ns              = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env, child);
            if(NULL != ns)
                ns_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                
            if((NULL != child_localname) && 
                (AXIS2_STRCMP(child_localname, localname) == 0) && (NULL != ns_uri) 
                    && (AXIS2_STRCMP(ns_uri, uri) == 0))
            {
                *child_node = child;
                return om_ele;
            }
        }                        
    }
    om_ele = NULL;
    child_localname = NULL;
    ns = NULL;
    ns_uri = NULL;
    
    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                ns              = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env, next_sibling);
                if(NULL != ns)
                    ns_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                    
                if((NULL != child_localname) && 
                    (AXIS2_STRCMP(child_localname, localname) == 0) && (NULL != ns_uri) &&
                    (AXIS2_STRCMP(ns_uri, uri) == 0))
                {
                    *child_node = next_sibling;
                    return om_ele;
                }
            
            }
            om_ele = NULL;
            child_localname = NULL;
            ns = NULL;
            ns_uri = NULL;
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);  
    }  
    return NULL;
}                                                                                                                                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_uri_localname(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *uri,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child          = NULL;
    axis2_om_element_t *om_ele      = NULL;
    axis2_char_t *child_localname   = NULL;
    axis2_om_namespace_t *ns        = NULL;
    axis2_char_t *ns_uri            = NULL;
               
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);        
    
    child = AXIS2_OM_NODE_GET_LAST_CHILD(ele_node, env);
    while(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)  
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if(NULL != om_ele)
            {
                ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env, ele_node);
                if(NULL != ns)
                    ns_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && 
                    (AXIS2_STRCMP(child_localname, localname) == 0) && (NULL != ns_uri)
                    && (AXIS2_STRCMP(ns_uri, uri) == 0))
                {
                    *child_node = child;
                    return om_ele;
                }
            }                      
            om_ele = NULL;
            child_localname = NULL;
            ns = NULL;
            ns_uri = NULL;
        }
        child = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(child, env);                
    }
    return NULL;
}
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_sibling_element_with_uri_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *uri,
                                             axis2_om_node_t **next_node)
{
    axis2_om_node_t *next_sibling = NULL;
    axis2_om_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    axis2_om_namespace_t *ns      = NULL;
    axis2_char_t *ns_uri          = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env, next_sibling);
                if(NULL != ns)
                    ns_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != ele_localname && 
                    (AXIS2_STRCMP(localname, ele_localname) == 0) && (NULL != ns) 
                    && (AXIS2_STRCMP(ns_uri, uri) == 0))
                {
                    *next_node = next_sibling;
                    return om_ele;
                }
            }
            om_ele = NULL;
            ele_localname = NULL;
            ns_uri = NULL;
            ns = NULL; 
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_localnames(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_array_list_t *names,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child = NULL;
    axis2_om_node_t *next_sibling = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_char_t *given_localname = NULL;
    axis2_om_element_t *om_ele  = NULL;
    int size = 0;
    int i    = 0;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, names, NULL);
    
    child = AXIS2_OM_NODE_GET_FIRST_CHILD(ele_node, env);
    if(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(child, env);
            if(NULL != om_ele)
            {
                size = AXIS2_ARRAY_LIST_SIZE(names, env);
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);

                for(i =0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(names, env, i);
                    
                    if((NULL != child_localname) && (NULL != given_localname) && 
                            AXIS2_STRCMP(child_localname, given_localname) == 0)
                    {
                        *child_node = child;
                        return om_ele;
                    }
                    given_localname = NULL;
                }                    
            }                        
        }
        om_ele = NULL;
        child_localname = NULL;
        given_localname = NULL;
        size = 0;
    }
    
    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                size = AXIS2_ARRAY_LIST_SIZE(names, env);
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                for(i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(names, env, i);
                    if((NULL != child_localname) && (NULL != given_localname) && 
                        (AXIS2_STRCMP(child_localname, given_localname) == 0))
                    {
                        *child_node = next_sibling;
                        return om_ele;
                    }
                    given_localname = NULL;
                }
            }
            om_ele = NULL;
            child_localname = NULL;
            given_localname = NULL;
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);  
    }  
    return NULL;
   
}                                             
                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_localnames(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_array_list_t *names,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_char_t *given_localname = NULL;
    int size = 0;
    int i = 0;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, names, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    
    child = AXIS2_OM_NODE_GET_LAST_CHILD(ele_node, env);
    while(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)  
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if(NULL != om_ele)
            {
                size = AXIS2_ARRAY_LIST_SIZE(names, env);
                for(i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(names, env, i);
                    child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                    if(NULL != child_localname && (NULL != given_localname) &&
                        (AXIS2_STRCMP(child_localname, given_localname) == 0))
                    {
                        *child_node = child;    
                        return om_ele;
                    }
                }
            }                      
            om_ele = NULL;
            child_localname = NULL;
            given_localname = NULL;
        }
        child = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(child, env);                
    }
    return NULL;
}                                             
                                                                                         
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_siblng_element_with_localnames(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_array_list_t *names,
                                             axis2_om_node_t **next_node)
{
    axis2_om_node_t *next_sibling = NULL;
    axis2_om_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    axis2_char_t *given_localname = NULL;
    int size = 0;
    int i    = 0;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, names, NULL);
    AXIS2_PARAM_CHECK(env->error, next_node, NULL);
    

    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                    AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                size = AXIS2_ARRAY_LIST_SIZE(names, env);
                for(i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(names, env, i);
                    ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                    if((NULL != ele_localname) && (NULL != given_localname) && 
                        (AXIS2_STRCMP(given_localname, ele_localname) == 0))
                    {
                        *next_node = next_sibling;
                        return om_ele;
                    }
                
                }
            }
            om_ele = NULL;
            ele_localname = NULL;                        
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;    
}                                              
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_localname_attr(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *attr_name,
                                             axis2_char_t *attr_value,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child          = NULL;
    axis2_om_node_t *next_sibling   = NULL;
    axis2_char_t *child_localname   = NULL;
    axis2_om_element_t *om_ele      = NULL;
    axis2_om_attribute_t *om_attr   = NULL;
    axis2_hash_t *attr_ht           = NULL;
    axis2_hash_index_t *hi          = NULL; 
    axis2_char_t *om_attr_value     = NULL;
    axis2_char_t *om_attr_name      = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_name, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_value, NULL);
    
    child = AXIS2_OM_NODE_GET_FIRST_CHILD(ele_node, env);

    if(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(child, env);
            if(NULL != om_ele)
            {
                
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    attr_ht = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if(NULL != attr_ht)
                    {
                        
                        for( hi = axis2_hash_first(attr_ht, env);  hi;
                                hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if(NULL != val)
                            {
                                om_attr = (axis2_om_attribute_t *)val;
                                om_attr_name = 
                                    AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attr, env);
                                    
                                om_attr_value =
                                    AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attr, env); 
                                if(NULL != om_attr_name && NULL != om_attr_value &&
                                AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    AXIS2_FREE(env->allocator, hi);
                                    *child_node = child;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;                                                                             }
                        }                                
                    }
                }
            }                        
        }
        om_ele = NULL;
        child_localname = NULL;
        hi = NULL;
    }

    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    attr_ht = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if(NULL != attr_ht)
                    {
                        
                        for( hi = axis2_hash_first(attr_ht, env);  hi;
                                hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if(NULL != val)
                            {
                                om_attr = (axis2_om_attribute_t *)val;
                                om_attr_name = 
                                    AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attr, env);
                                    
                                om_attr_value =
                                    AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attr, env); 
                    
                                if(NULL != om_attr_name && NULL != om_attr_value &&
                                AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    *child_node = child;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;   
                            }
                        }                                
                    }
                }
            
            }
            om_ele = NULL;
            child_localname = NULL;
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);  
    }  
    return NULL;
}                                             
                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_localname_attr(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *attr_name,
                                             axis2_char_t *attr_value,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *child = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_om_attribute_t *om_attr = NULL;
    axis2_char_t *om_attr_name = NULL;
    axis2_char_t *om_attr_value = NULL;
    axis2_hash_index_t *hi      = NULL;
    axis2_hash_t *attr_ht       = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    
    child = AXIS2_OM_NODE_GET_LAST_CHILD(ele_node, env);
    while(NULL != child)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(child, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t*)  
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if(NULL != om_ele)
            {
                child_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != child_localname && 
                    AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    attr_ht = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if(NULL != attr_ht)
                    {
                        
                        for( hi = axis2_hash_first(attr_ht, env);  hi;
                                hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if(NULL != val)
                            {
                                om_attr = (axis2_om_attribute_t *)val;
                                om_attr_name = 
                                    AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attr, env);
                                    
                                om_attr_value =
                                    AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attr, env); 
                    
                                if(NULL != om_attr_name && NULL != om_attr_value &&
                                AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    AXIS2_FREE(env->allocator, hi);
                                    *child_node = child;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;   
                            }
                        }                                
                    }
                }
            
            }                      
            om_ele = NULL;
            child_localname = NULL;
        }
        child = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(child, env);                
    }
    return NULL;
}                                             
                                                                                         
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_siblng_element_with_localname_attr(axis2_om_element_t *ele,
                                             axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *attr_name,
                                             axis2_char_t *attr_value,
                                             axis2_om_node_t **child_node)
{
    axis2_om_node_t *next_sibling = NULL;
    axis2_om_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    axis2_om_attribute_t *om_attr = NULL;
    axis2_char_t *om_attr_value   = NULL;
    axis2_char_t *om_attr_name    = NULL;
    axis2_hash_t *attr_ht         = NULL;
    axis2_hash_index_t *hi        = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    
    
    next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while(NULL != next_sibling)
    {
        if(AXIS2_OM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIS2_OM_ELEMENT)
        {
            om_ele = (axis2_om_element_t *)
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if(NULL != om_ele)
            {
                ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if(NULL != ele_localname && AXIS2_STRCMP(localname, ele_localname) == 0)
                {
                    attr_ht = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if(NULL != attr_ht)
                    {
                        
                        for( hi = axis2_hash_first(attr_ht, env);  hi;
                                hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if(NULL != val)
                            {
                                om_attr = (axis2_om_attribute_t *)val;
                                om_attr_name = 
                                    AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attr, env);
                                    
                                om_attr_value =
                                    AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attr, env); 
                    
                                if(NULL != om_attr_name && NULL != om_attr_value &&
                                AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    AXIS2_FREE(env->allocator, hi);
                                    *child_node = next_sibling;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;   
                            }
                        }                                
                    }
                }
            }
            om_ele = NULL;
            ele_localname = NULL;                        
        }
        next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}                                             
                                                                            
AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_om_util_get_child_node_text(axis2_om_node_t *om_node,
                             const axis2_env_t *env)
{
    axis2_om_element_t *om_ele = NULL;
    AXIS2_ENV_CHECK(env , NULL);
    if(NULL == om_node)
        return NULL;
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) != AXIS2_OM_ELEMENT)
        return NULL;
    
    om_ele = (axis2_om_element_t *)
             AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
    
    if(NULL != om_ele)
    {
        return AXIS2_OM_ELEMENT_GET_TEXT(om_ele, env, om_node);
    }
    return NULL;
}                             
                             
AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_om_util_get_localname(axis2_om_node_t *node,
                            const axis2_env_t *env)
{
    axis2_om_element_t *om_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    if(NULL == node)
        return NULL;
    if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
        return NULL;
    om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_NODE_TYPE(node, env);
    if(NULL != om_ele)
        return AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
    return NULL;
}                            
    
AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_om_util_get_node_namespace_uri(axis2_om_node_t *om_node,
                                     const axis2_env_t *env)
{
    axis2_om_element_t *om_ele = NULL;
    axis2_om_namespace_t *om_ns   = NULL;
    if(!om_node)
        return NULL;
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) == AXIS2_OM_ELEMENT)
    {
        om_ele = AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
        if(NULL == om_ele)
            return NULL;
        om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env, om_node);
        if(NULL != om_ns)
            return AXIS2_OM_NAMESPACE_GET_URI(om_ns, env);                    
    }        
    return NULL;
} 

AXIS2_EXTERN axis2_om_child_element_iterator_t* AXIS2_CALL
axis2_om_util_get_child_elements(axis2_om_element_t *om_ele, 
                                 const axis2_env_t *env,
                                 axis2_om_node_t *om_node)
{
    axis2_om_element_t *first_ele = NULL;
    axis2_om_node_t *first_node   = NULL;
    AXIS2_PARAM_CHECK(env->error, om_node, NULL);
    AXIS2_PARAM_CHECK(env->error, om_ele, NULL);
    first_ele = 
        AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(om_ele, env, om_node, &first_node);
    if(NULL != first_ele)
    {
        return axis2_om_child_element_iterator_create(env, first_node);
    }        
    return NULL;
}

