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

#include <axis2_parameter.h>
#include <axis2.h>

typedef struct axis2_param_impl_s
{
    /** Parameter struct */
    axis2_param_t parameter;
    /** Parameter name */
    axis2_char_t *name;
    /** Parameter value */
    void* value;
    /** Parameter locked? */
    axis2_bool_t locked;
    /** Parameter type */
    int type; /*default is AXIS2_TEXT_PARAMETER */
    /**
     * to store the parameter element
     * <parameter name="ServiceClass1" locked="false">
     * org.apache.axis2.sample.echo.EchoImpl</parameter>
     */
    axis2_om_node_t *parameter_element ;
} axis2_param_impl_t;

#define AXIS2_INTF_TO_IMPL(parameter) ((axis2_param_impl_t *)parameter)

axis2_char_t* AXIS2_CALL 
axis2_parameter_get_name(axis2_param_t *parameter, 
							axis2_env_t **env);
	
void* AXIS2_CALL 
axis2_parameter_get_value(axis2_param_t *parameter, 
							axis2_env_t **env);
	
axis2_status_t AXIS2_CALL 
axis2_parameter_set_name(axis2_param_t *parameter, 
							axis2_env_t **env, 
							axis2_char_t *name);

axis2_status_t AXIS2_CALL 
axis2_parameter_set_value(axis2_param_t *parameter, 
							axis2_env_t **env, 
							void *value);

axis2_bool_t AXIS2_CALL 
axis2_parameter_is_locked(axis2_param_t *parameter, 
							axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_parameter_set_locked(axis2_param_t *parameter, 
							axis2_env_t **env, 
							axis2_bool_t value);

int AXIS2_CALL 
axis2_parameter_get_parameter_type(axis2_param_t *parameter, 
									axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_parameter_set_parameter_type(axis2_param_t *parameter, 
									axis2_env_t **env, 
									int type);

axis2_status_t AXIS2_CALL 
axis2_parameter_set_parameter_element(axis2_param_t *parameter, 
										axis2_env_t **env, 
										axis2_om_node_t *element);

axis2_om_node_t* AXIS2_CALL 
axis2_parameter_get_parameter_element(axis2_param_t *parameter, 
										axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_parameter_free(axis2_param_t *parameter, 
						axis2_env_t **env);

axis2_param_t* AXIS2_CALL 
axis2_parameter_create(axis2_env_t **env, 
						axis2_char_t *name, void *value)
{
    axis2_param_impl_t *parameter_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    parameter_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_param_impl_t) );
    if (!parameter_impl)
    { 
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;        
    }
    
    parameter_impl->name = name; /* shallow copy */
    parameter_impl->value = value; /* shallow copy */
    parameter_impl->locked = AXIS2_FALSE;
    parameter_impl->type = AXIS2_TEXT_PARAMETER;
    parameter_impl->parameter_element = NULL;
    
    /* initialize operations */
    parameter_impl->parameter.ops->get_name = axis2_parameter_get_name;
    parameter_impl->parameter.ops->get_value = axis2_parameter_get_value;
    parameter_impl->parameter.ops->set_name = axis2_parameter_set_name; 
    parameter_impl->parameter.ops->set_value = axis2_parameter_set_value; 
    parameter_impl->parameter.ops->is_locked = axis2_parameter_is_locked;
    parameter_impl->parameter.ops->set_locked = axis2_parameter_set_locked;
    parameter_impl->parameter.ops->get_parameter_type = 
		axis2_parameter_get_parameter_type;
    parameter_impl->parameter.ops->set_parameter_type = 
		axis2_parameter_set_parameter_type;
    parameter_impl->parameter.ops->set_parameter_element = 
		axis2_parameter_set_parameter_element;
    parameter_impl->parameter.ops->get_parameter_element = 
		axis2_parameter_get_parameter_element;
    parameter_impl->parameter.ops->free = axis2_parameter_free;
    
    return &(parameter_impl->parameter);
}

axis2_char_t* AXIS2_CALL axis2_parameter_get_name(axis2_param_t *parameter, axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(parameter)->name;
}

void* AXIS2_CALL axis2_parameter_get_value(axis2_param_t *parameter, axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(parameter)->value;
}

axis2_status_t AXIS2_CALL axis2_parameter_set_name(axis2_param_t *parameter, axis2_env_t **env, axis2_char_t *name)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(parameter)->name = name; /* shallow copy */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_parameter_set_value(axis2_param_t *parameter, axis2_env_t **env, void *value)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(parameter)->value = value; /* shallow copy */
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL axis2_parameter_is_locked(axis2_param_t *parameter, axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    return AXIS2_INTF_TO_IMPL(parameter)->locked;
}

axis2_status_t AXIS2_CALL axis2_parameter_set_locked(axis2_param_t *parameter, axis2_env_t **env, axis2_bool_t value)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(parameter)->locked = value;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL axis2_parameter_get_parameter_type(axis2_param_t *parameter, axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    return AXIS2_INTF_TO_IMPL(parameter)->type;
}

axis2_status_t AXIS2_CALL axis2_parameter_set_parameter_type(axis2_param_t *parameter, axis2_env_t **env, int type)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(parameter)->type = type;
    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL axis2_parameter_set_parameter_element(axis2_param_t *parameter, axis2_env_t **env, axis2_om_node_t *element)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(parameter)->parameter_element = element; /* shallow copy */
    return AXIS2_SUCCESS;

}

axis2_om_node_t* AXIS2_CALL axis2_parameter_get_parameter_element(axis2_param_t *parameter, axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
    
    return AXIS2_INTF_TO_IMPL(parameter)->parameter_element;
}

axis2_status_t AXIS2_CALL axis2_parameter_free(axis2_param_t *parameter, axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(parameter, env, AXIS2_FAILURE);
	if(NULL != parameter->ops)
		AXIS2_FREE((*env)->allocator, parameter->ops);
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(parameter));    
    return AXIS2_SUCCESS;
}
