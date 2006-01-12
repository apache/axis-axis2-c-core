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

#include <axis2_handler_desc.h>
#include <axis2_param_container.h>
#include <axis2_string.h>
#include <axis2_handler.h>

typedef struct axis2_handler_desc_impl
{
	/** handler description */
	axis2_handler_desc_t handler_desc;
    
    /** name */
    axis2_qname_t *qname;
    /** phase rules */
    axis2_phase_rule_t *rules;
    /** handler repsesented by meta information*/
    axis2_handler_t *handler;
    /** class qname */
    axis2_char_t *class_name;
	/** parent param container */
    axis2_param_container_t *parent;
    
} axis2_handler_desc_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(handler_desc) \
        ((axis2_handler_desc_impl_t *)handler_desc)

axis2_qname_t* AXIS2_CALL 
axis2_handler_desc_get_qname (struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_set_qname (struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env, 
                                                axis2_qname_t *qname);
axis2_phase_rule_t* AXIS2_CALL 
axis2_handler_desc_get_rules(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_set_rules(struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env, 
                                                axis2_phase_rule_t *phase_rule);
axis2_param_t* AXIS2_CALL 
axis2_handler_desc_get_param(struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env, 
                                                axis2_char_t *name);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_add_param(struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env, 
                                                axis2_param_t *param);

axis2_array_list_t* AXIS2_CALL 
axis2_handler_desc_get_params(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);
axis2_bool_t AXIS2_CALL 
axis2_handler_desc_is_param_locked(struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env, 
                                                axis2_char_t * param_name);
axis2_handler_t* AXIS2_CALL 
axis2_handler_desc_get_handler(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_set_handler(struct axis2_handler_desc *handler_desc, 
                                                axis2_env_t **env, 
                                                axis2_handler_t * handler);
axis2_char_t* AXIS2_CALL 
axis2_handler_desc_get_class_name(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_set_class_name(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, 
                                                axis2_char_t *class_name);
axis2_param_container_t* AXIS2_CALL 
axis2_handler_desc_get_parent(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_set_parent(struct axis2_handler_desc *handler_desc, 
                                            axis2_env_t **env,
                                            axis2_param_container_t *parent);
axis2_status_t AXIS2_CALL 
axis2_handler_desc_free (struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);

axis2_handler_desc_t* AXIS2_CALL 
axis2_handler_desc_create_with_qname(axis2_env_t **env, 
                                        axis2_qname_t *qname)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    handler_desc_impl = AXIS2_MALLOC( (*env)->allocator, 
        sizeof(axis2_handler_desc_impl_t) );
    if (!handler_desc_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    handler_desc_impl->handler_desc.param_container = NULL;
    handler_desc_impl->qname = NULL;
    handler_desc_impl->rules = NULL;
    handler_desc_impl->handler = NULL;
    handler_desc_impl->class_name = NULL;
	handler_desc_impl->parent = NULL;
    handler_desc_impl->handler_desc.ops = NULL;
	
	handler_desc_impl->handler_desc.param_container  = 
        axis2_param_container_create(env);
    if (!handler_desc_impl->handler_desc.param_container)
    {
        /** error code is already set by last param container create */
        axis2_handler_desc_free(&(handler_desc_impl->handler_desc), env);
        return NULL;        
    }
    
    handler_desc_impl->rules = axis2_phase_rule_create(env, NULL);
    if (!handler_desc_impl->rules)
    {
        /** error code is already set by last param container create */
        axis2_handler_desc_free(&(handler_desc_impl->handler_desc), env);
        return NULL;        
    }
    
    if (qname)
    {
        handler_desc_impl->qname = AXIS2_QNAME_CLONE(qname, env);
    }

    /* initialize ops */
    
    handler_desc_impl->handler_desc.ops  = AXIS2_MALLOC( (*env)->allocator, 
            sizeof(axis2_handler_desc_ops_t) );
    if (!handler_desc_impl->handler_desc.ops)
    {
        axis2_handler_desc_free(&(handler_desc_impl->handler_desc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    handler_desc_impl->handler_desc.ops->get_qname = axis2_handler_desc_get_qname;
    handler_desc_impl->handler_desc.ops->set_qname = axis2_handler_desc_set_qname;
    handler_desc_impl->handler_desc.ops->get_rules = axis2_handler_desc_get_rules;
    handler_desc_impl->handler_desc.ops->set_rules = axis2_handler_desc_set_rules;
    handler_desc_impl->handler_desc.ops->get_param = axis2_handler_desc_get_param;
    handler_desc_impl->handler_desc.ops->add_param = axis2_handler_desc_add_param;
    handler_desc_impl->handler_desc.ops->get_params = axis2_handler_desc_get_params;
    handler_desc_impl->handler_desc.ops->is_param_locked = axis2_handler_desc_is_param_locked;
    handler_desc_impl->handler_desc.ops->get_handler = axis2_handler_desc_get_handler;
    handler_desc_impl->handler_desc.ops->set_handler = axis2_handler_desc_set_handler;
    handler_desc_impl->handler_desc.ops->get_class_name = axis2_handler_desc_get_class_name;
    handler_desc_impl->handler_desc.ops->set_class_name = axis2_handler_desc_set_class_name;
    handler_desc_impl->handler_desc.ops->get_parent = axis2_handler_desc_get_parent;
    handler_desc_impl->handler_desc.ops->set_parent = axis2_handler_desc_set_parent;
    handler_desc_impl->handler_desc.ops->free = axis2_handler_desc_free;
    
    return &(handler_desc_impl->handler_desc);
}

axis2_qname_t* AXIS2_CALL axis2_handler_desc_get_qname (struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(handler_desc)->qname;
}

axis2_status_t AXIS2_CALL axis2_handler_desc_set_qname (struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_qname_t *qname)
{
    /**TODO: need to have the qname copy constructor here */
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(handler_desc)->qname = qname;
    return AXIS2_SUCCESS;
    
}

axis2_phase_rule_t* AXIS2_CALL axis2_handler_desc_get_rules(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(handler_desc)->rules;
}

axis2_status_t AXIS2_CALL axis2_handler_desc_set_rules(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_phase_rule_t *phase_rule)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    if (handler_desc_impl->rules)
    {
        AXIS2_PHASE_RULE_FREE(handler_desc_impl->rules, env);
        handler_desc_impl->rules = NULL;
    }
    
    if (phase_rule)
    {
        handler_desc_impl->rules = AXIS2_PHASE_RULE_CLONE(phase_rule, env);
        if (!(handler_desc_impl->rules))
            return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_param_t* AXIS2_CALL axis2_handler_desc_get_param(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_char_t *name)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAM(handler_desc->param_container, env, name);
}

axis2_status_t AXIS2_CALL axis2_handler_desc_add_param(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_param_t * param)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    if (AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(handler_desc_impl->parent, env, AXIS2_PARAM_GET_NAME(param, env)) )
    {
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE , AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    return AXIS2_PARAM_CONTAINER_ADD_PARAM(handler_desc->param_container, env, param);
}

axis2_array_list_t* AXIS2_CALL axis2_handler_desc_get_params(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(handler_desc->param_container, env);
}

axis2_bool_t AXIS2_CALL axis2_handler_desc_is_param_locked(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_char_t * param_name)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    /* see if it is locked in parent */
    if( AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(handler_desc_impl->parent, env, param_name) )
        return AXIS2_TRUE;
    
    return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(handler_desc->param_container, env, param_name);    
}

axis2_handler_t* AXIS2_CALL axis2_handler_desc_get_handler(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(handler_desc)->handler;
}

axis2_status_t AXIS2_CALL axis2_handler_desc_set_handler(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_handler_t * handler)
{
    /* handler dec is the place wehre the handler really lives. Hence this is a deep copy and 
    should be freed by the free mechanism. There is a coupling here in trems of freeing */
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    if (handler_desc_impl->handler)
    {
        AXIS2_HANDLER_FREE(handler_desc_impl->handler, env);
        handler_desc_impl->handler = NULL;
    }
    
    if (handler)
        handler_desc_impl->handler = handler; /* Shallow copy, but free method should free this */
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL axis2_handler_desc_get_class_name(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(handler_desc)->class_name;
}

axis2_status_t AXIS2_CALL axis2_handler_desc_set_class_name(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env, axis2_char_t *class_name)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    if (handler_desc_impl->class_name)
    {
        AXIS2_FREE((*env)->allocator, handler_desc_impl->class_name);
        handler_desc_impl->class_name = NULL;
    }
    
    if (class_name)
    {
        handler_desc_impl->class_name = AXIS2_STRDUP(class_name, env);
        if (!handler_desc_impl->class_name)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_param_container_t* AXIS2_CALL axis2_handler_desc_get_parent(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(handler_desc)->parent;
}

axis2_status_t AXIS2_CALL axis2_handler_desc_set_parent(struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env,
                                                axis2_param_container_t * parent)
{
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(handler_desc)->parent = parent; /* shallow copy, because the parent lives somewhere else*/
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_handler_desc_free (struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env)
{
    axis2_handler_desc_impl_t *handler_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(handler_desc, env, AXIS2_FAILURE);
    
    handler_desc_impl = AXIS2_INTF_TO_IMPL(handler_desc);
    
    if (handler_desc_impl->handler_desc.ops)
    {
        AXIS2_FREE((*env)->allocator, handler_desc_impl->handler_desc.ops);
        handler_desc_impl->handler_desc.ops = NULL;
    }
    
    if (handler_desc_impl->handler_desc.param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(handler_desc_impl->handler_desc.
            param_container, env);
        handler_desc_impl->handler_desc.param_container = NULL;
    }    
    
	if (handler_desc_impl->qname)
    {
        AXIS2_QNAME_FREE(handler_desc_impl->qname, env);
        handler_desc_impl->qname = NULL;
    }
	
    if (handler_desc_impl->rules)
    {
        AXIS2_PHASE_RULE_FREE(handler_desc_impl->rules, env);
        handler_desc_impl->rules = NULL;
    }
    
    if (handler_desc_impl->handler)
    {
        AXIS2_HANDLER_FREE(handler_desc_impl->handler, env);
        handler_desc_impl->handler = NULL;
    }
    
    if (handler_desc_impl->class_name)
    {
        AXIS2_FREE((*env)->allocator, handler_desc_impl->class_name);
        handler_desc_impl->class_name = NULL;
    }
    if(handler_desc_impl)
    {
        AXIS2_FREE((*env)->allocator, handler_desc_impl);
        handler_desc_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}
