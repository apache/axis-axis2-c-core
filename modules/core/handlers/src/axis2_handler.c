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
    /** Handler description. This is a referance, hence a shallow copy. */
    axis2_handler_desc_t *handler_desc;
} axis2_handler_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(handler) ((axis2_handler_impl_t *)handler)

axis2_handler_t* AXIS2_CALL axis2_handler_create(axis2_env_t **env);
{
    axis2_handler_impl_t *handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    handler_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_handler_impl_t) );
    if (!handler_impl)
    { 
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;        
    }

    handler_impl->handler_desc = NULL;
    
    /* initialize operations */
    handler_impl->handler.ops = NULL;
    handler_impl->handler.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_handler_ops_t) );
    if (!handler_impl->handler.ops)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        axis2_handler_free(&(handler_impl->handler));
        return NULL;        
    }
    
    handler_impl->handler.ops->get_name = axis2_handler_get_name;
    
    return &(handler_impl->handler);
}

axis2_status_t AXIS2_CALL 
axis2_handler_free (struct axis2_handler * handler, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(handler, env, AXIS2_FAILURE);
    if (handler->ops)
    {
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(handler)->handler.ops);
        AXIS2_INTF_TO_IMPL(handler)->handler.ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(handler));
    AXIS2_INTF_TO_IMPL(handler) = NULL;
    
    return AXIS2_SUCCESS;    
}

axis2_qname_t* AXIS2_CALL 
axis2_handler_get_name(struct axis2_handler * handler, 
                       axis2_env_t **env) 
{
    AXIS2_FUNC_PARAMETER_CHECK(handler, env, NULL);
    
    if (!(AXIS2_INTF_TO_IMPL(handler)->handler_desc))
        return NULL;
    
    return AXIS2_HANDLER_DESC_GET_NAME(AXIS2_INTF_TO_IMPL(handler)->handler_desc);
}

axis2_status_t AXIS2_CALL axis2_handler_invoke (struct axis2_handler * handler, 
                                                axis2_env_t **env,
                                                struct axis2_msg_ctx *msg_ctx)
{
    /**TODO invoke has to be implemented by an implementing handler */
}

axis2_parameter_t* AXIS2_CALL axis2_handler_get_parameter (struct axis2_handler * handler, 
                                                axis2_env_t **env, 
                                                axis2_char_t *name)
{
    AXIS2_FUNC_PARAMETER_CHECK(handler, env, NULL);
    
    if (!(AXIS2_INTF_TO_IMPL(handler)->handler_desc))
        return NULL;
    
    return AXIS2_HANDLER_DESC_GET_PARAMETER(AXIS2_INTF_TO_IMPL(handler)->handler_desc, name);    
}
 
axis2_status_t AXIS2_CALL axis2_handler_init (struct axis2_handler * handler, 
                                           axis2_env_t **env, 
                                           struct axis2_handler_description *handler_desc)
{    
    AXIS2_FUNC_PARAMETER_CHECK(handler, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(handler)->handler_desc = handler_desc;
    
    return AXIS2_SUCCESS;    
}

axis2_handler_description_t* (AXIS2_CALL * get_handler_desc) (struct axis2_handler * handler, 
                                                                           axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(handler, env, NULL);
    return AXIS2_INTF_TO_IMPL(handler)->handler_desc;
}
