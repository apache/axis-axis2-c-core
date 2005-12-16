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

#include <axis2_om_children_qname_iterator.h>
#include <axis2_om_element.h>

 /************** function prototypes *************************************/
 
axis2_status_t AXIS2_CALL 
axis2_om_children_qname_iterator_free(axis2_om_children_qname_iterator_t *iterator,
                                axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL
axis2_om_children_qname_iterator_remove(axis2_om_children_qname_iterator_t *iterator,
                                  axis2_env_t **env);  

axis2_bool_t AXIS2_CALL 
axis2_om_children_qname_iterator_has_next(axis2_om_children_qname_iterator_t *iterator,
                                     axis2_env_t **env); 

axis2_om_node_t* AXIS2_CALL 
axis2_om_children_qname_iterator_next(axis2_om_children_qname_iterator_t *iterator,
                                axis2_env_t **env); 

static axis2_bool_t AXIS2_CALL
axis2_om_children_qname_iterator_qname_matches(axis2_env_t **env,
                                               axis2_qname_t *element_qname,
                                               axis2_qname_t *qname_to_match);
                                
/****************** impl struct ***********************************************/

typedef struct axis2_om_children_qname_iterator_impl_t 
{
    axis2_om_children_qname_iterator_t iterator;
    axis2_om_node_t *current_child;
    axis2_om_node_t *last_child;
    axis2_bool_t next_called;
    axis2_bool_t remove_called;    
    axis2_qname_t *given_qname;
    axis2_bool_t need_to_move_forward;
    axis2_bool_t matching_node_found;
    
}axis2_om_children_qname_iterator_impl_t;

/*************************** Macro *********************************************/

#define AXIS2_INTF_TO_IMPL(iterator) ((axis2_om_children_qname_iterator_impl_t*)iterator)

/*******************************************************************************/

AXIS2_DECLARE(axis2_om_children_qname_iterator_t *)
axis2_om_children_qname_iterator_create(axis2_env_t **env,
                                        axis2_om_node_t *current_child,
                                        axis2_qname_t *given_qname)
{
    axis2_om_children_qname_iterator_impl_t *iterator_impl = NULL;    
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, current_child, NULL);
    AXIS2_PARAM_CHECK((*env)->error, given_qname, NULL);
    
    iterator_impl = (axis2_om_children_qname_iterator_impl_t*)AXIS2_MALLOC((*env)->allocator,
                        sizeof(axis2_om_children_qname_iterator_impl_t));
    
    if(!iterator_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;        
    }
    
    iterator_impl->iterator.ops = NULL;
    iterator_impl->current_child = NULL;
    iterator_impl->last_child = NULL;
    iterator_impl->need_to_move_forward = AXIS2_TRUE;
    iterator_impl->matching_node_found = AXIS2_FALSE;
    iterator_impl->next_called = AXIS2_FALSE;
    iterator_impl->remove_called = AXIS2_FALSE;
    
    
    iterator_impl->iterator.ops = 
            (axis2_om_children_qname_iterator_ops_t*)AXIS2_MALLOC((*env)->allocator,
                sizeof(axis2_om_children_qname_iterator_ops_t));
    
    if(!(iterator_impl->iterator.ops))
    {
        AXIS2_OM_CHILDREN_QNAME_ITERATOR_FREE(&(iterator_impl->iterator), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
                                    
    iterator_impl->current_child = current_child;
    iterator_impl->given_qname = given_qname;  
    
    iterator_impl->iterator.ops->free_fn = 
            axis2_om_children_qname_iterator_free;
    iterator_impl->iterator.ops->remove = 
            axis2_om_children_qname_iterator_remove;
    iterator_impl->iterator.ops->has_next =
            axis2_om_children_qname_iterator_has_next;
    iterator_impl->iterator.ops->next =
            axis2_om_children_qname_iterator_next;
   return &(iterator_impl->iterator);    
}

axis2_status_t AXIS2_CALL 
axis2_om_children_qname_iterator_free(axis2_om_children_qname_iterator_t *iterator,
                                axis2_env_t **env)
{
    axis2_om_children_qname_iterator_impl_t *iterator_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(iterator, env, AXIS2_FAILURE);
    
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    
    if(iterator->ops)
        AXIS2_FREE((*env)->allocator, iterator->ops);
    AXIS2_FREE((*env)->allocator, iterator_impl);
    return AXIS2_SUCCESS;
}
                                
axis2_status_t AXIS2_CALL
axis2_om_children_qname_iterator_remove(axis2_om_children_qname_iterator_t *iterator,
                                  axis2_env_t **env)
{
    axis2_om_children_qname_iterator_impl_t *iterator_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(iterator, env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    if(!(iterator_impl->next_called))
        return AXIS2_FAILURE;
     if(iterator_impl->remove_called)
        return AXIS2_FAILURE;

    iterator_impl->remove_called = AXIS2_TRUE;

    if(!(iterator_impl->last_child))
        return AXIS2_FAILURE;
    AXIS2_OM_NODE_DETACH(iterator_impl->last_child, env);
    return AXIS2_SUCCESS;      
}    

axis2_bool_t AXIS2_CALL 
axis2_om_children_qname_iterator_has_next(axis2_om_children_qname_iterator_t *iterator,
                                     axis2_env_t **env)
{   
    axis2_om_children_qname_iterator_impl_t *iterator_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(iterator, env, AXIS2_FALSE);

    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    
    while(iterator_impl->need_to_move_forward)
    {
        if(iterator_impl->current_child)
        {
            axis2_om_element_t *om_element = NULL;
            if(AXIS2_OM_NODE_GET_NODE_TYPE(iterator_impl->current_child, env) == AXIS2_OM_ELEMENT)
            {
                om_element = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                                    iterator_impl->current_child, env);    
            }
            
            if(om_element && axis2_om_children_qname_iterator_qname_matches(env,
                    AXIS2_OM_ELEMENT_GET_QNAME(om_element, env), iterator_impl->given_qname))
            {
                iterator_impl->matching_node_found = AXIS2_TRUE;
                iterator_impl->need_to_move_forward = AXIS2_FALSE;                
                
            }
            else
            {
                iterator_impl->current_child = AXIS2_OM_NODE_GET_NEXT_SIBLING(iterator_impl->current_child, env);
                if(iterator_impl->current_child)
                {
                    iterator_impl->need_to_move_forward = AXIS2_TRUE;
                    iterator_impl->matching_node_found = AXIS2_TRUE;                    
                }                    
                else
                {
                    iterator_impl->need_to_move_forward = AXIS2_FALSE;
                    iterator_impl->matching_node_found = AXIS2_FALSE;       
                }
            }
        }
        else
        {
            iterator_impl->need_to_move_forward = AXIS2_FALSE;    
        }            
     }
    return iterator_impl->matching_node_found;     
    
}

axis2_om_node_t* AXIS2_CALL 
axis2_om_children_qname_iterator_next(axis2_om_children_qname_iterator_t *iterator,
                                axis2_env_t **env)
{
    axis2_om_children_qname_iterator_impl_t *iterator_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(iterator, env, NULL);

    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    
    iterator_impl->need_to_move_forward = AXIS2_TRUE;
    iterator_impl->matching_node_found = AXIS2_FALSE;
    iterator_impl->next_called = AXIS2_TRUE;
    iterator_impl->remove_called = AXIS2_FALSE;

    iterator_impl->last_child = iterator_impl->current_child;
    iterator_impl->current_child = AXIS2_OM_NODE_GET_NEXT_SIBLING(iterator_impl->current_child, env);
    return iterator_impl->last_child;
}        

static axis2_bool_t AXIS2_CALL
axis2_om_children_qname_iterator_qname_matches(axis2_env_t **env,
                                               axis2_qname_t *element_qname,
                                               axis2_qname_t *qname_to_match)
{
    int lparts_match =  0;
    int uris_match = 0;
    axis2_char_t *ele_lpart = NULL;
    axis2_char_t *match_lpart = NULL;
    axis2_char_t *ele_nsuri = NULL;
    axis2_char_t *match_nsuri = NULL;
    
    if(!(qname_to_match))
        return AXIS2_TRUE;    
    if(qname_to_match)
    {
        match_lpart = AXIS2_QNAME_GET_LOCALPART(qname_to_match, env);
        match_nsuri = AXIS2_QNAME_GET_URI(qname_to_match, env);
    }
    if(element_qname)
    {
        ele_lpart = AXIS2_QNAME_GET_LOCALPART(element_qname, env);
        ele_nsuri = AXIS2_QNAME_GET_URI(element_qname, env);
    }
    
    lparts_match = !match_lpart || 
                    (AXIS2_STRCMP(match_lpart,"") == 0) ||
                    element_qname && (AXIS2_STRCMP(ele_lpart, match_lpart) == 0) ;

    
    uris_match = !match_nsuri || (AXIS2_STRCMP(match_nsuri,"") == 0) ||
                       element_qname && (AXIS2_STRCMP(ele_nsuri, match_nsuri) == 0);
    
    return lparts_match && uris_match;   
}
