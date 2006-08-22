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
#include <axis2_module.h>

typedef struct axis2_module_desc_impl
{
    axis2_module_desc_t module_desc;
    axis2_module_t *module;
    axis2_qname_t *qname;
    axis2_conf_t *parent;
    /**
     * To store module operations , which are supposed to be added to a service
     * the module is engaged to a service
     */
    axis2_hash_t *ops;

}
axis2_module_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(module_desc) ((axis2_module_desc_impl_t *)module_desc)

axis2_status_t AXIS2_CALL
axis2_module_desc_free(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_in_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_in_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *in_flow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_out_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_out_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *out_flow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_in_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_in_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *falut_in_flow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_out_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_out_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *fault_out_flow);

const axis2_qname_t *AXIS2_CALL
axis2_module_desc_get_qname(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_qname(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_module_desc_add_op(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_op_t *op);

axis2_hash_t *AXIS2_CALL
axis2_module_desc_get_all_ops(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_conf_t *AXIS2_CALL
axis2_module_desc_get_parent(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_conf_t *parent);

axis2_status_t AXIS2_CALL
axis2_module_desc_add_param(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_param_t *param);

axis2_param_t *AXIS2_CALL
axis2_module_desc_get_param(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_array_list_t *AXIS2_CALL
axis2_module_desc_get_all_params(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name);

axis2_module_t *AXIS2_CALL
axis2_module_desc_get_module(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_module(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_module_t *module);

axis2_module_desc_t *AXIS2_CALL
axis2_module_desc_create(
    const axis2_env_t *env)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    module_desc_impl = (axis2_module_desc_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_module_desc_impl_t));

    if (NULL == module_desc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc_impl->qname = NULL;
    module_desc_impl->module = NULL;
    module_desc_impl->parent = NULL;
    module_desc_impl->module_desc.params = NULL;
    module_desc_impl->module_desc.flow_container = NULL;
    module_desc_impl->ops = NULL;

    module_desc_impl->module_desc.params = axis2_param_container_create(env);
    if (NULL == module_desc_impl->module_desc.params)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc_impl->module_desc.flow_container = axis2_flow_container_create(env);
    if (NULL == module_desc_impl->module_desc.flow_container)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return NULL;
    }

    module_desc_impl->ops = axis2_hash_make(env);
    if (NULL == module_desc_impl->ops)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc_impl->module_desc.ops =
        AXIS2_MALLOC (env->allocator, sizeof(axis2_module_desc_ops_t));
    if (NULL == module_desc_impl->module_desc.ops)
    {
        axis2_module_desc_free(&(module_desc_impl->module_desc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc_impl->module_desc.ops->free = axis2_module_desc_free;
    module_desc_impl->module_desc.ops->get_in_flow = axis2_module_desc_get_in_flow;
    module_desc_impl->module_desc.ops->set_in_flow = axis2_module_desc_set_in_flow;
    module_desc_impl->module_desc.ops->get_out_flow = axis2_module_desc_get_out_flow;
    module_desc_impl->module_desc.ops->set_out_flow = axis2_module_desc_set_out_flow;
    module_desc_impl->module_desc.ops->get_module = axis2_module_desc_get_module;
    module_desc_impl->module_desc.ops->set_module = axis2_module_desc_set_module;
    module_desc_impl->module_desc.ops->get_fault_in_flow =
        axis2_module_desc_get_fault_in_flow;
    module_desc_impl->module_desc.ops->set_fault_in_flow =
        axis2_module_desc_set_fault_in_flow;
    module_desc_impl->module_desc.ops->get_fault_out_flow =
        axis2_module_desc_get_fault_out_flow;
    module_desc_impl->module_desc.ops->set_fault_out_flow =
        axis2_module_desc_set_fault_out_flow;
    module_desc_impl->module_desc.ops->get_qname = axis2_module_desc_get_qname;
    module_desc_impl->module_desc.ops->set_qname = axis2_module_desc_set_qname;
    module_desc_impl->module_desc.ops->add_op =
        axis2_module_desc_add_op;
    module_desc_impl->module_desc.ops->get_all_ops =
        axis2_module_desc_get_all_ops;
    module_desc_impl->module_desc.ops->get_parent = axis2_module_desc_get_parent;
    module_desc_impl->module_desc.ops->set_parent = axis2_module_desc_set_parent;
    module_desc_impl->module_desc.ops->add_param = axis2_module_desc_add_param;
    module_desc_impl->module_desc.ops->get_param = axis2_module_desc_get_param;
    module_desc_impl->module_desc.ops->get_all_params = axis2_module_desc_get_all_params;
    module_desc_impl->module_desc.ops->is_param_locked =
        axis2_module_desc_is_param_locked;

    return &(module_desc_impl->module_desc);
}

AXIS2_EXTERN axis2_module_desc_t *AXIS2_CALL
axis2_module_desc_create_with_qname(
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    module_desc_impl = AXIS2_INTF_TO_IMPL(axis2_module_desc_create(env));
    if (NULL == module_desc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc_impl->qname = (axis2_qname_t *)qname;

    return &(module_desc_impl->module_desc);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_free(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);

    if (module_desc_impl->module)
    {
        AXIS2_MODULE_SHUTDOWN(module_desc_impl->module, env);
        module_desc_impl->module = NULL;
    }

    if (module_desc->params)
    {
        AXIS2_PARAM_CONTAINER_FREE(module_desc->params, env);
        module_desc->params = NULL;
    }

    if (module_desc->flow_container)
    {
        AXIS2_FLOW_CONTAINER_FREE(module_desc->flow_container, env);
        module_desc->flow_container = NULL;
    }

    module_desc_impl->parent = NULL;

    if (module_desc_impl->qname)
    {
        AXIS2_QNAME_FREE(module_desc_impl->qname, env);
        module_desc_impl->qname = NULL;
    }

    if (module_desc_impl->ops)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (module_desc_impl->ops, env); hi;
                hi = axis2_hash_next ( env, hi))
        {
            struct axis2_op * op = NULL;
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

    if (module_desc->ops)
    {
        AXIS2_FREE(env->allocator, module_desc->ops);
        module_desc->ops = NULL;
    }

    if (module_desc_impl)
    {
        AXIS2_FREE(env->allocator, module_desc_impl);
        module_desc_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_free_void_arg(
    void *module_desc,
    const axis2_env_t *env)
{
    axis2_module_desc_t *module_desc_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_desc_l = (axis2_module_desc_t *) module_desc;
    return axis2_module_desc_free(module_desc_l, env);
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_in_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_FLOW_CONTAINER_GET_IN_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_in_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *in_flow)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_flow, AXIS2_FAILURE);

    return AXIS2_FLOW_CONTAINER_SET_IN_FLOW(module_desc->flow_container, env,
            in_flow);
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_out_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_FLOW_CONTAINER_GET_OUT_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_out_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *out_flow)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_flow, AXIS2_FAILURE);

    return AXIS2_FLOW_CONTAINER_SET_OUT_FLOW(module_desc->flow_container, env,
            out_flow);
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_in_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_FLOW_CONTAINER_GET_FAULT_IN_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_in_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *falut_in_flow)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, falut_in_flow, AXIS2_FAILURE);

    return AXIS2_FLOW_CONTAINER_SET_FAULT_IN_FLOW(module_desc->flow_container,
            env, falut_in_flow);
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_out_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_FLOW_CONTAINER_GET_FAULT_OUT_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_out_flow(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_flow_t *fault_out_flow)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_out_flow, AXIS2_FAILURE);

    return AXIS2_FLOW_CONTAINER_SET_FAULT_OUT_FLOW(module_desc->flow_container,
            env, fault_out_flow);
}

const axis2_qname_t *AXIS2_CALL
axis2_module_desc_get_qname(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(module_desc)->qname;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_qname(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);

    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    if (module_desc_impl->qname)
    {
        AXIS2_QNAME_FREE(module_desc_impl->qname, env);
        module_desc_impl->qname = NULL;
    }

    module_desc_impl->qname = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_add_op(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_char_t *op_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);

    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    if (NULL == (module_desc_impl->ops))
    {
        module_desc_impl->ops = axis2_hash_make (env);
        if (!module_desc_impl->ops)
            return AXIS2_FAILURE;
    }

    op_qname = AXIS2_OP_GET_QNAME(op, env);

    if (NULL == op_qname)
    {
        return AXIS2_FAILURE;
    }
    op_name = AXIS2_QNAME_TO_STRING(op_qname, env);
    axis2_hash_set(module_desc_impl->ops, op_name, AXIS2_HASH_KEY_STRING, op);

    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_module_desc_get_all_ops(
    const  axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->ops;
}

axis2_conf_t *AXIS2_CALL
axis2_module_desc_get_parent(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_conf_t *parent)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, parent, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(module_desc)->parent = parent;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_add_param(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    axis2_char_t *param_name_l = NULL;
    axis2_status_t ret_status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    param_name_l = AXIS2_PARAM_GET_NAME(param, env);
    if (NULL == param_name_l)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_PARAM,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (AXIS2_TRUE == axis2_module_desc_is_param_locked(module_desc, env, param_name_l))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        ret_status = AXIS2_PARAM_CONTAINER_ADD_PARAM(module_desc->params, env, param);
    }
    return ret_status;
}

axis2_param_t *AXIS2_CALL
axis2_module_desc_get_param(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAM(module_desc->params, env, name);
}

axis2_array_list_t *AXIS2_CALL
axis2_module_desc_get_all_params(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(module_desc->params, env);
}

axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_module_desc_impl_t *module_desc_impl = NULL;
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_bool_t ret_state = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FAILURE);

    module_desc_impl = AXIS2_INTF_TO_IMPL(module_desc);
    /* checking the locked value of parent*/
    if (NULL == module_desc_impl->parent)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_MODULE_DESC, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    locked = AXIS2_CONF_IS_PARAM_LOCKED(module_desc_impl->parent, env, param_name);

    if (AXIS2_TRUE == locked)
    {
        ret_state = AXIS2_TRUE;
    }
    else
    {
        param_l = axis2_module_desc_get_param(module_desc, env,
                param_name);
        if (NULL != param_l && AXIS2_TRUE == AXIS2_PARAM_IS_LOCKED(param_l, env))
            ret_state = AXIS2_TRUE;
        else
            ret_state = AXIS2_FALSE;

    }
    return ret_state;
}

axis2_module_t *AXIS2_CALL
axis2_module_desc_get_module(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->module;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_module(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_module_t *module)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(module_desc)->module = module;
    return AXIS2_SUCCESS;
}
