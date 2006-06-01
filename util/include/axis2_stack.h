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
 
 #ifndef AXIS2_STACK_H
 #define AXIS2_STACK_H
 
 
 /**
 * @file axis2_stack.h
 * @brief represents a stack
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_stack_ops axis2_stack_ops_t;
typedef struct axis2_stack axis2_stack_t;

/**
 * @defgroup axis2_util represents a data structure stack
 * @ingroup axis2_util
 * @{
 */

/**
 * \brief Axis2 stack ops struct
 * Encapsulator struct for ops of axis2_qname
 */

AXIS2_DECLARE_DATA struct axis2_stack_ops
{
    /**
     * Free function of the stack
     * @param stack pointer to stack
     * @param env environemnt
     */
     
    axis2_status_t (AXIS2_CALL*
    free)(axis2_stack_t *stack,
          const axis2_env_t *env);      
       
    void* (AXIS2_CALL*
    pop)(axis2_stack_t *stack,
         const axis2_env_t *env); 
    
    axis2_status_t (AXIS2_CALL *
    push)(axis2_stack_t *stack,
          const axis2_env_t *env,
          void* value);
          
    int (AXIS2_CALL *
    size)(axis2_stack_t *stack,
          const axis2_env_t *env);
    /** 
     * returns the last put value from the stack
     * without removing it from stack
     */      
          
    void* (AXIS2_CALL *
    get)(axis2_stack_t *stack,
         const axis2_env_t *env);  
         
    void* (AXIS2_CALL *
    get_at)(axis2_stack_t *stack,
            const axis2_env_t *env,
            int i);
                                       
          
};
 
struct axis2_stack
{
    axis2_stack_ops_t *ops;
};

AXIS2_EXTERN axis2_stack_t * AXIS2_CALL
axis2_stack_create(const axis2_env_t *env);
 
 
/*************MACROS ********************************************************/
 
#define AXIS2_STACK_FREE( stack, env) \
        ((stack)->ops->free( stack, env))
        
#define AXIS2_STACK_POP( stack, env) \
        ((stack)->ops->pop( stack, env))
        
#define AXIS2_STACK_PUSH( stack, env, value) \
        ((stack)->ops->push( stack, env, value))
        
#define AXIS2_STACK_SIZE( stack, env) \
        ((stack)->ops->size( stack, env)) 
        
#define AXIS2_STACK_GET( stack, env) \
        ((stack)->ops->get( stack, env))
        
#define AXIS2_STACK_GET_AT( stack, env, i) \
        ((stack)->ops->get_at( stack, env, i))        
 
/*************END MACROS ****************************************************/
 /** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_STACK_H */

