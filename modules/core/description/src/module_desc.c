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
 
#include <axis2_module_desc.h>

/** 
 * @brief Module Descripton struct impl
 *	Module Descripton 
 */ 
typedef struct axis2_module_desc_impl
{
	axis2_module_desc_t module_desc;
    
    axis2_qname_t *qname;
    
    axis2_conf_t *parent;
    
    /** 
     * To store module opeartions , which are suppose to be added to a service 
     * if it is engaged to a service
     */
    axis2_hash_t *ops;

} axis2_module_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(module_desc) ((axis2_module_desc_impl_t *)module_desc)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_module_desc_free(axis2_module_desc_t *module_desc,
                            axis2_env_t **env);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_inflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_inflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *inflow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_outflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_outflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *outflow);

axis2_flow_t * AXIS2_CALL
axis2_module_desc_get_fault_inflow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_inflow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_flow_t *falut_inflow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_outflow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_outflow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env,
                                            axis2_flow_t *fault_outflow);

axis2_qname_t * AXIS2_CALL
axis2_module_desc_get_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_module_desc_add_op (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_op_t *op);

axis2_hash_t * AXIS2_CALL
axis2_module_desc_get_ops (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_conf_t * AXIS2_CALL
axis2_module_desc_get_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_conf_t *parent);

axis2_status_t AXIS2_CALL
axis2_module_desc_add_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_module_desc_get_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_module_desc_get_params (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_char_t *param_name);

/************************** End of function prototypes ************************/

axis2_module_desc_t * AXIS2_CALL 
axis2_module_desc_create (axis2_env_t **env)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	module_desc_impl = (axis2_module_desc_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_module_desc_impl_t));
		
	if(NULL == module_desc_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
        return NULL;
    }
    
    module_desc_impl->qname = NULL;
    module_desc_impl->parent = NULL;	
    module_desc_impl->module_desc.params = NULL;
    module_desc_impl->module_desc.flow_container = NULL;
    module_desc_impl->ops = NULL;
    
    module_desc_impl->module_desc.params = axis2_param_container_create(env);
    if(NULL == module_desc_impl->module_desc.params)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
    module_desc_impl->module_desc.flow_container = axis2_flow_container_create(env);
    if(NULL == module_desc_impl->module_desc.flow_container)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
    module_desc_impl->ops = axis2_hash_make(env);
    if(NULL == module_desc_impl->ops)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
        return NULL;        
    }
   
	module_desc_impl->module_desc.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_module_desc_ops_t));
	if(NULL == module_desc_impl->module_desc.ops)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
	module_desc_impl->module_desc.ops->free = axis2_module_desc_free;
    module_desc_impl->module_desc.ops->get_inflow = axis2_module_desc_get_inflow;
    module_desc_impl->module_desc.ops->set_inflow = axis2_module_desc_set_inflow;
    module_desc_impl->module_desc.ops->get_outflow = axis2_module_desc_get_outflow;
    module_desc_impl->module_desc.ops->set_outflow = axis2_module_desc_set_outflow;
    
    module_desc_impl->module_desc.ops->get_fault_inflow = 
            axis2_module_desc_get_fault_inflow;
    
    module_desc_impl->module_desc.ops->set_fault_inflow = 
            axis2_module_desc_set_fault_inflow;
    
    module_desc_impl->module_desc.ops->get_fault_outflow = 
        axis2_module_desc_get_fault_outflow;
    
    module_desc_impl->module_desc.ops->set_fault_outflow = 
            axis2_module_desc_set_fault_outflow;
    
    module_desc_impl->module_desc.ops->get_name = axis2_module_desc_get_name;
    module_desc_impl->module_desc.ops->set_name = axis2_module_desc_set_name;
    
    module_desc_impl->module_desc.ops->add_op = 
            axis2_module_desc_add_op;
    
    module_desc_impl->module_desc.ops->get_ops = 
            axis2_module_desc_get_ops;
    
    module_desc_impl->module_desc.ops->get_parent = axis2_module_desc_get_parent;
    module_desc_impl->module_desc.ops->set_parent = axis2_module_desc_set_parent;
    module_desc_impl->module_desc.ops->add_param = axis2_module_desc_add_param;
    module_desc_impl->module_desc.ops->get_param = axis2_module_desc_get_param;
    module_desc_impl->module_desc.ops->get_params = axis2_module_desc_get_params;
    
    module_desc_impl->module_desc.ops->is_param_locked = 
            axis2_module_desc_is_param_locked;
    
	return &(module_desc_impl->module_desc);
}

axis2_module_desc_t * AXIS2_CALL 
axis2_module_desc_create_with_qname (axis2_env_t **env, axis2_qname_t *qname)
{
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
	
	axis2_module_desc_impl_t *module_desc_impl = 
        AXIS2_INTF_TO_IMPL(axis2_module_desc_create(env));
    if(NULL == module_desc_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    module_desc_impl->qname = qname;
		
	return &(module_desc_impl->module_desc);
}


/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_module_desc_free(axis2_module_desc_t *module_desc,
                            axis2_env_t **env)
{
    axis2_module_desc_impl_t * module_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    
    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    
    if(NULL != module_desc->ops)
        AXIS2_FREE((*env)->allocator, module_desc->ops);
    
    if(NULL == module_desc->params)
    {
        AXIS2_PARAM_CONTAINER_FREE(module_desc->params, env);
        module_desc->params = NULL;
    }
    
    if(NULL == module_desc->flow_container)
    {
        AXIS2_FLOW_CONTAINER_FREE(module_desc->flow_container, env);
        module_desc->flow_container = NULL;
    }
    
    module_desc_impl->parent = NULL;
    
    if(NULL != module_desc_impl->qname)
    {
        AXIS2_QNAME_FREE(module_desc_impl->qname, env);
        module_desc_impl->qname = NULL;
    }
    
    if(NULL != module_desc_impl->ops)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (module_desc_impl->ops, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_op *op = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            op = (struct axis2_op *) val;
            if (op)
                AXIS2_OP_FREE (op, env);
            
            val = NULL;
            op = NULL;
               
        }
        axis2_hash_free(module_desc_impl->ops, env);
        module_desc_impl->ops = NULL;
    }
    
    if(module_desc_impl)
        AXIS2_FREE((*env)->allocator, module_desc_impl);
    module_desc_impl = NULL;
    
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_inflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_FLOW_CONTAINER_GET_INFLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_inflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *inflow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, inflow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_INFLOW(module_desc->flow_container, env, 
        inflow);    
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_outflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_FLOW_CONTAINER_GET_OUTFLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_outflow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *outflow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outflow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_OUTFLOW(module_desc->flow_container, env, 
        outflow);
}

axis2_flow_t * AXIS2_CALL
axis2_module_desc_get_fault_inflow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_FLOW_CONTAINER_GET_FAULT_INFLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_inflow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_flow_t *falut_inflow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, falut_inflow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_FAULT_INFLOW(module_desc->flow_container,
        env, falut_inflow);
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_outflow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    return AXIS2_FLOW_CONTAINER_GET_FAULT_OUTFLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_outflow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env,
                                            axis2_flow_t *fault_outflow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_outflow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_FAULT_OUTFLOW(module_desc->flow_container, 
        env, fault_outflow);
}

axis2_qname_t * AXIS2_CALL
axis2_module_desc_get_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(module_desc)->qname;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    if(module_desc_impl->qname)
    {
        AXIS2_QNAME_FREE(module_desc_impl->qname, env);
        module_desc_impl->qname = NULL;
    }
    
    module_desc_impl->qname = qname;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_add_op (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_op_t *op)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    axis2_qname_t *optr_name = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE);
    
    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    if (NULL == (module_desc_impl->ops))
	{                    
		module_desc_impl->ops = axis2_hash_make (env);
	}	
    
    optr_name = AXIS2_OP_GET_NAME(op, env);
    if(NULL == optr_name)
    {
        return AXIS2_FAILURE;
    }
    
    axis2_hash_set(module_desc_impl->ops, optr_name, sizeof(axis2_qname_t), 
        op);
    
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_module_desc_get_ops (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->ops;
}

axis2_conf_t * AXIS2_CALL
axis2_module_desc_get_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_conf_t *parent)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, parent, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(module_desc)->parent = parent;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_add_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_param_t *param)
{
    axis2_char_t *param_name_l = NULL;
    axis2_status_t ret_status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    
    param_name_l = AXIS2_PARAM_GET_NAME(param, env);
    if(NULL == param_name_l)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_PARAM, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if(AXIS2_TRUE == axis2_module_desc_is_param_locked(module_desc, env, param_name_l))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {        
        ret_status = AXIS2_PARAM_CONTAINER_ADD_PARAM(module_desc->params, env, param);
    } 
    return ret_status;    
}

axis2_param_t * AXIS2_CALL
axis2_module_desc_get_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAM(module_desc->params, env, name);
}

axis2_array_list_t * AXIS2_CALL
axis2_module_desc_get_params (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(module_desc->params, env);
}

axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_char_t *param_name)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_bool_t ret_state = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    /* checking the locked value of parent*/
    if(NULL == module_desc_impl->parent)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_MODULE_DESC, AXIS2_FALSE);
        return AXIS2_FALSE;
    }
    locked = AXIS2_CONF_IS_PARAM_LOCKED(module_desc_impl->parent, env, param_name);
    
    if(AXIS2_TRUE == locked)
    {
        ret_state = AXIS2_TRUE;
    }
    else
    {
        param_l = axis2_module_desc_get_param(module_desc, env, 
            param_name);
        if(NULL != param_l && AXIS2_TRUE == AXIS2_PARAM_IS_LOCKED(param_l, env))
            ret_state = AXIS2_TRUE;
        else
            ret_state = AXIS2_FALSE;
        
    }  
    return ret_state;    
}
