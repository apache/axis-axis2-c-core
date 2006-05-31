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

#include <axis2_handler.h>
#include <axis2_handler_desc.h>

typedef struct axis2_handler_impl
{
    /** Handler struct */
    axis2_handler_t handler;
    /** Derived struct */
    /*void* derived;*/
    /** Handler description. This is a referance, hence a shallow copy. */
    axis2_handler_desc_t *handler_desc;
} axis2_handler_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(handler) ((axis2_handler_impl_t *)handler)


axis2_status_t AXIS2_CALL 
axis2_handler_free (struct axis2_handler * handler, 
                    const axis2_env_t *env);
                    
axis2_qname_t* AXIS2_CALL 
axis2_handler_get_name(struct axis2_handler * handler, 
                       const axis2_env_t *env) ;

                       
axis2_status_t AXIS2_CALL 
axis2_handler_invoke (struct axis2_handler * handler,
                      const axis2_env_t *env,
                      struct axis2_msg_ctx *msg_ctx);
                      
axis2_param_t* AXIS2_CALL 
axis2_handler_get_param (struct axis2_handler * handler, 
                         const axis2_env_t *env, 
                         axis2_char_t *name);

                         
axis2_status_t AXIS2_CALL 
axis2_handler_init (struct axis2_handler * handler, 
                    const axis2_env_t *env, 
                    struct axis2_handler_desc *handler_desc);

                    
axis2_handler_desc_t* AXIS2_CALL 
axis2_handler_get_handler_desc (struct axis2_handler * handler, 
                                const axis2_env_t *env);
                                
/*void* AXIS2_CALL axis2_handler_get_derived(struct axis2_handler * handler, 
                                                          const axis2_env_t *env);
axis2_status_t AXIS2_CALL axis2_handler_set_derived(struct axis2_handler * handler, 
                                                          const axis2_env_t *env, void* derived);
*/

axis2_handler_t* AXIS2_CALL 
axis2_handler_create(const axis2_env_t *env)
{
    axis2_handler_impl_t *handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    handler_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_handler_impl_t) );
    if (!handler_impl)
    { 
        AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);
        return NULL;        
    }

    /*handler_impl->derived = NULL;*/
    handler_impl->handler_desc = NULL;
    
    /* initialize ops */
    handler_impl->handler.ops = NULL;
    handler_impl->handler.ops  = AXIS2_MALLOC( env->allocator, sizeof(axis2_handler_ops_t) );
    if (!handler_impl->handler.ops)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);
        axis2_handler_free(&(handler_impl->handler), env);
        return NULL;        
    }
    
    handler_impl->handler.ops->free = 
        axis2_handler_free;
        
    handler_impl->handler.ops->init = 
        axis2_handler_init;
        
    handler_impl->handler.ops->invoke = 
        axis2_handler_invoke;
        
    handler_impl->handler.ops->get_name = 
        axis2_handler_get_name;
        
    handler_impl->handler.ops->get_param = 
        axis2_handler_get_param;
        
    handler_impl->handler.ops->get_handler_desc = 
        axis2_handler_get_handler_desc;
        
    /*handler_impl->handler.ops->get_derived= axis2_handler_get_derived;
    handler_impl->handler.ops->set_derived= axis2_handler_set_derived;
    */
    return &(handler_impl->handler);
}

axis2_status_t AXIS2_CALL 
axis2_handler_free (struct axis2_handler * handler, 
                    const axis2_env_t *env)
{
    axis2_handler_impl_t *handler_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    handler_impl = AXIS2_INTF_TO_IMPL(handler);
    if (handler_impl->handler.ops)
    {
        AXIS2_FREE(env->allocator, handler_impl->handler.ops);
        handler_impl->handler.ops = NULL;
    }
    
    AXIS2_FREE(env->allocator, handler_impl);
    handler_impl = NULL;
    
    return AXIS2_SUCCESS;    
}

axis2_qname_t* AXIS2_CALL 
axis2_handler_get_name(struct axis2_handler * handler, 
                       const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    if (!(AXIS2_INTF_TO_IMPL(handler)->handler_desc))
        return NULL;
    
    return AXIS2_HANDLER_DESC_GET_QNAME(AXIS2_INTF_TO_IMPL(handler)->handler_desc, env);
}

axis2_status_t AXIS2_CALL 
axis2_handler_invoke (struct axis2_handler * handler,
                      const axis2_env_t *env,
                      struct axis2_msg_ctx *msg_ctx)
{
    /**TODO invoke has to be implemented by an implementing handler */
    return AXIS2_SUCCESS;
}

axis2_param_t* AXIS2_CALL 
axis2_handler_get_param (struct axis2_handler * handler, 
                         const axis2_env_t *env, 
                         axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    if (!(AXIS2_INTF_TO_IMPL(handler)->handler_desc))
        return NULL;
    
    return AXIS2_HANDLER_DESC_GET_PARAM(AXIS2_INTF_TO_IMPL(handler)->handler_desc, env, name);    
}
 
axis2_status_t AXIS2_CALL 
axis2_handler_init (struct axis2_handler * handler, 
                    const axis2_env_t *env, 
                    struct axis2_handler_desc *handler_desc)
{    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(handler)->handler_desc = handler_desc;
    AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler);
    
    return AXIS2_SUCCESS;    
}

axis2_handler_desc_t* AXIS2_CALL 
axis2_handler_get_handler_desc (struct axis2_handler * handler, 
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(handler)->handler_desc;
}

/*void* AXIS2_CALL axis2_handler_get_derived(struct axis2_handler * handler, 
                                                          const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(handler)->derived;
}
axis2_status_t AXIS2_CALL axis2_handler_set_derived(struct axis2_handler * handler, 
                                                          const axis2_env_t *env, void* derived)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(handler)->derived = derived;
    
    return AXIS2_SUCCESS;    
}
*/
