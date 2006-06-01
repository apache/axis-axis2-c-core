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
 
#include <axis2_stack.h>
#include <axis2_utils.h>
#include <axis2_env.h>
#include <stdlib.h>
#include <string.h>

#define AXIS2_STACK_DEFAULT_CAPACITY 10
 
typedef struct axis2_stack_impl 
{
    axis2_stack_t stack;

    void **data;
    /** current number of elements */
    int size;
    /** total capacity */
    int capacity;

    axis2_bool_t is_empty_stack;

}axis2_stack_impl_t;

#define AXIS2_INTF_TO_IMPL(stack) ((axis2_stack_impl_t *)stack)

 
axis2_status_t AXIS2_CALL
axis2_stack_free(axis2_stack_t *stack,
                 const axis2_env_t *env);      
    
void* AXIS2_CALL
axis2_stack_pop(axis2_stack_t *stack,
                const axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
axis2_stack_push(axis2_stack_t *stack,
                 const axis2_env_t *env,
                 void* value);
        
int AXIS2_CALL
axis2_stack_size(axis2_stack_t *stack,
                 const axis2_env_t *env);
                 
void* AXIS2_CALL
axis2_stack_get(axis2_stack_t *stack,
                const axis2_env_t *env);    
                
void* AXIS2_CALL
axis2_stack_get_at(axis2_stack_t *stack,
                   const axis2_env_t *env,
                   int i);
                                                

AXIS2_EXTERN axis2_stack_t * AXIS2_CALL
axis2_stack_create(const axis2_env_t *env)
{
    axis2_stack_impl_t *stack_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);

    stack_impl = (axis2_stack_impl_t*)AXIS2_MALLOC(env->allocator, 
                        sizeof(axis2_stack_impl_t));
                        
    if(!stack_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    stack_impl->data = NULL;
    stack_impl->size = 0;
    stack_impl->capacity = AXIS2_STACK_DEFAULT_CAPACITY;
    stack_impl->is_empty_stack = AXIS2_TRUE;
    stack_impl->stack.ops = NULL;
    
    stack_impl->data = AXIS2_MALLOC(env->allocator, sizeof(void*)*                 
                                       AXIS2_STACK_DEFAULT_CAPACITY);
    if(NULL == stack_impl->data)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_stack_free(&(stack_impl->stack) , env);
        return NULL;
    }

    stack_impl->stack.ops = (axis2_stack_ops_t *)AXIS2_MALLOC(env->allocator,
                                sizeof(axis2_stack_ops_t));
                                
    if(NULL ==  stack_impl->stack.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_stack_free(&(stack_impl->stack) , env);
        return NULL;
    }                                   
    
    
    stack_impl->stack.ops->free = 
        axis2_stack_free;
        
    stack_impl->stack.ops->pop = 
        axis2_stack_pop;
        
    stack_impl->stack.ops->push =
        axis2_stack_push;
        
    stack_impl->stack.ops->size =
        axis2_stack_size;    
        
    stack_impl->stack.ops->get =
        axis2_stack_get;       
        
    stack_impl->stack.ops->get_at =
        axis2_stack_get_at;             
    
    return &(stack_impl->stack);                   
} 

axis2_status_t AXIS2_CALL
axis2_stack_free(axis2_stack_t *stack,
                 const axis2_env_t *env)
{
    axis2_stack_impl_t *stack_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    stack_impl = AXIS2_INTF_TO_IMPL(stack);
    
    if(NULL != stack_impl->data)
    {
        AXIS2_FREE(env->allocator, stack_impl->data);
        stack_impl->data = NULL;
    }
    if(NULL != stack->ops)
    {
        AXIS2_FREE(env->allocator, stack->ops);
        stack->ops = NULL;
    }
    AXIS2_FREE(env->allocator, stack_impl);
    stack_impl = NULL;
    return AXIS2_SUCCESS;
}  

void* AXIS2_CALL
axis2_stack_pop(axis2_stack_t *stack,
                const axis2_env_t *env)
{
    axis2_stack_impl_t *stack_impl = NULL;
    void *value = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    stack_impl = AXIS2_INTF_TO_IMPL(stack);
    if(stack_impl->is_empty_stack == AXIS2_TRUE || 
            stack_impl->size == 0)
    {            
        return NULL;
    }
    if(stack_impl->size > 0)
    {
        value = stack_impl->data[stack_impl->size -1 ];
        stack_impl->data[stack_impl->size-1] = NULL;
        stack_impl->size--;
        if(stack_impl->size == 0)
        {
            stack_impl->is_empty_stack = AXIS2_TRUE;
        }
    }
    return value;
}                 

axis2_status_t AXIS2_CALL 
axis2_stack_push(axis2_stack_t *stack,
                 const axis2_env_t *env,
                 void* value)
{
    axis2_stack_impl_t *stack_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);

    stack_impl = AXIS2_INTF_TO_IMPL(stack);
    if((stack_impl->size  < stack_impl->capacity) && (stack_impl->capacity >0 ))
    {
        stack_impl->data[stack_impl->size++] = value;
    }
    else
    {
        void **new_data = NULL;
        
        int new_capacity = stack_impl->capacity + AXIS2_STACK_DEFAULT_CAPACITY; 
        
        new_data = AXIS2_MALLOC(env->allocator, sizeof(void*)*new_capacity);
        if(!new_data)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        memset(new_data, 0, sizeof(void*)*new_capacity);
        memcpy(new_data, stack_impl->data, sizeof(void*) *(stack_impl->capacity));
        stack_impl->capacity = new_capacity;
        
        AXIS2_FREE(env->allocator, stack_impl->data);
        stack_impl->data = NULL;
        stack_impl->data = new_data;
        
        stack_impl->data[stack_impl->size++] = value;
    }
    stack_impl->is_empty_stack = AXIS2_FALSE;
    return AXIS2_SUCCESS;    
}                 
        
int AXIS2_CALL
axis2_stack_size(axis2_stack_t *stack,
                 const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(stack)->size;
}   

void * AXIS2_CALL
axis2_stack_get(axis2_stack_t *stack,
                const axis2_env_t *env)
{
    axis2_stack_impl_t *stack_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    stack_impl = AXIS2_INTF_TO_IMPL(stack);
    if(stack_impl->size > 0)
    {
        return stack_impl->data[stack_impl->size-1];
    }
    return NULL;
} 

void* AXIS2_CALL
axis2_stack_get_at(axis2_stack_t *stack,
                   const axis2_env_t *env,
                   int i)
{
    axis2_stack_impl_t *stack_impl = NULL;
    stack_impl = AXIS2_INTF_TO_IMPL(stack);
    if((stack_impl->size == 0) || ( i < 0) || (i >= stack_impl->size))
    {
        return NULL;
    }
    return stack_impl->data[i];
}                                                
