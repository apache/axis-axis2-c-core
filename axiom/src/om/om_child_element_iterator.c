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
 
 #include <axis2_om_child_element_iterator.h>
 
 /************** function prototypes *************************************/
 
axis2_status_t AXIS2_CALL 
axis2_om_child_element_iterator_free
                    (void *iterator,
                     const axis2_env_t *env);
                                
axis2_status_t AXIS2_CALL
axis2_om_child_element_iterator_remove
                    (axis2_om_child_element_iterator_t *iterator,
                     const axis2_env_t *env);  

axis2_bool_t AXIS2_CALL 
axis2_om_child_element_iterator_has_next
                    (axis2_om_child_element_iterator_t *iterator,
                     const axis2_env_t *env); 

axis2_om_node_t* AXIS2_CALL 
axis2_om_child_element_iterator_next
                    (axis2_om_child_element_iterator_t *iterator,
                     const axis2_env_t *env);  
                                
/*************** end function prototypes **********************************/                                

typedef struct axis2_om_child_element_iterator_impl_t
{
    axis2_om_child_element_iterator_t iterator;
    axis2_om_node_t *current_child;
    axis2_om_node_t *last_child;
    axis2_bool_t next_called;
    axis2_bool_t remove_called;
}axis2_om_child_element_iterator_impl_t; 

/***************** end impl struct ****************************************/

#define AXIS2_INTF_TO_IMPL(iterator) \
        ((axis2_om_child_element_iterator_impl_t *)iterator)

/**************** end macro ***********************************************/



AXIS2_EXTERN  axis2_om_child_element_iterator_t * AXIS2_CALL
axis2_om_child_element_iterator_create(const axis2_env_t *env, 
                                  axis2_om_node_t *current_child)
{
    axis2_om_child_element_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    iterator_impl = (axis2_om_child_element_iterator_impl_t *)AXIS2_MALLOC(
                        env->allocator,
                        sizeof(axis2_om_child_element_iterator_impl_t));

    if(!iterator_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;    
    }
    
    iterator_impl->current_child = NULL;
    iterator_impl->last_child    = NULL;
    
    iterator_impl->next_called = AXIS2_FALSE;
    iterator_impl->remove_called = AXIS2_FALSE;
    
    iterator_impl->iterator.ops = NULL;
    iterator_impl->iterator.ops = 
        (axis2_om_child_element_iterator_ops_t*)AXIS2_MALLOC(env->allocator,
            sizeof(axis2_om_child_element_iterator_ops_t));
    
    if(!(iterator_impl->iterator.ops))
    {
        AXIS2_OM_CHILD_ELEMENT_ITERATOR_FREE(&(iterator_impl->iterator), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    iterator_impl->current_child = current_child;
    iterator_impl->iterator.ops->free_fn = 
            axis2_om_child_element_iterator_free;
    iterator_impl->iterator.ops->remove = 
            axis2_om_child_element_iterator_remove;
    iterator_impl->iterator.ops->has_next = 
            axis2_om_child_element_iterator_has_next;
    iterator_impl->iterator.ops->next = 
            axis2_om_child_element_iterator_next;
    return &(iterator_impl->iterator);
}  


axis2_status_t AXIS2_CALL 
axis2_om_child_element_iterator_free
            (void *iterator,
             const axis2_env_t *env)
{
    axis2_om_child_element_iterator_impl_t *iterator_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    if(iterator_impl->iterator.ops)
        AXIS2_FREE(env->allocator, iterator_impl->iterator.ops);
        
    AXIS2_FREE(env->allocator, iterator_impl);        
    return AXIS2_SUCCESS;
}                                
                                
axis2_status_t AXIS2_CALL
axis2_om_child_element_iterator_remove
                (axis2_om_child_element_iterator_t *iterator,
                 const axis2_env_t *env)
{
    axis2_om_node_t *last_child = NULL;
    axis2_om_child_element_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    if(!(iterator_impl->next_called))
        return AXIS2_FAILURE;
     if(iterator_impl->remove_called)
        return AXIS2_FAILURE;

    iterator_impl->remove_called = AXIS2_TRUE;

    if(!(iterator_impl->last_child))
        return AXIS2_FAILURE;
    last_child = AXIS2_OM_NODE_DETACH(iterator_impl->last_child, env);
    AXIS2_OM_NODE_FREE_TREE(last_child, env);
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
axis2_om_child_element_iterator_has_next
                (axis2_om_child_element_iterator_t *iterator,
                 const axis2_env_t *env)
{
    axis2_om_child_element_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    
    return (iterator_impl->current_child) ? AXIS2_TRUE : AXIS2_FALSE;
}                                      

axis2_om_node_t* AXIS2_CALL 
axis2_om_child_element_iterator_next
                (axis2_om_child_element_iterator_t *iterator,
                 const axis2_env_t *env)
{
    axis2_om_child_element_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    
    iterator_impl->next_called = AXIS2_TRUE;
    iterator_impl->remove_called = AXIS2_FALSE;
    if(iterator_impl->current_child)
    {
        iterator_impl->last_child = iterator_impl->current_child;
        do{
            iterator_impl->current_child = AXIS2_OM_NODE_GET_NEXT_SIBLING(
                                            iterator_impl->current_child, env);

          }while(iterator_impl->current_child && 
                (AXIS2_OM_NODE_GET_NODE_TYPE(iterator_impl->current_child, env) 
                != AXIS2_OM_ELEMENT));   
        
                                                    
        return iterator_impl->last_child;                                            
    }
    return NULL;
}
