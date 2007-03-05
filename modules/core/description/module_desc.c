/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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

struct axis2_module_desc
{
    axis2_module_t *module;
    axis2_qname_t *qname;
    axis2_conf_t *parent;
    /**
     * To store module operations , which are supposed to be added to a service
     * the module is engaged to a service
     */
    axis2_hash_t *ops;
    /** 
     * flow container that encapsulates the flows associated with the 
     * module 
     */
    axis2_flow_container_t *flow_container;
    /** 
     * parameter container that stores all the parameters associated with 
     * the module 
     */
    axis2_param_container_t *params;
};

AXIS2_EXTERN axis2_module_desc_t *AXIS2_CALL
axis2_module_desc_create(
    const axis2_env_t *env)
{
    axis2_module_desc_t *module_desc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    module_desc = (axis2_module_desc_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_module_desc_t));

    if (NULL == module_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc->qname = NULL;
    module_desc->module = NULL;
    module_desc->parent = NULL;
    module_desc->params = NULL;
    module_desc->flow_container = NULL;
    module_desc->ops = NULL;

    module_desc->params = axis2_param_container_create(env);
    if (NULL == module_desc->params)
    {
        axis2_module_desc_free(module_desc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc->flow_container = axis2_flow_container_create(env);
    if (NULL == module_desc->flow_container)
    {
        axis2_module_desc_free(module_desc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return NULL;
    }

    module_desc->ops = axis2_hash_make(env);
    if (NULL == module_desc->ops)
    {
        axis2_module_desc_free(module_desc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    return module_desc;
}

AXIS2_EXTERN axis2_module_desc_t *AXIS2_CALL
axis2_module_desc_create_with_qname(
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_module_desc_t *module_desc = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    module_desc = axis2_module_desc_create(env);
    if (NULL == module_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    module_desc->qname = (axis2_qname_t *)qname;

    return module_desc;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_desc_free(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (module_desc->module)
    {
        AXIS2_MODULE_SHUTDOWN(module_desc->module, env);
        module_desc->module = NULL;
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

    module_desc->parent = NULL;

    if (module_desc->qname)
    {
        AXIS2_QNAME_FREE(module_desc->qname, env);
        module_desc->qname = NULL;
    }

    if (module_desc->ops)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(module_desc->ops, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            struct axis2_op * op = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            op = (struct axis2_op *) val;
            if (op)
                AXIS2_OP_FREE(op, env);
            val = NULL;
            op = NULL;

        }
        axis2_hash_free(module_desc->ops, env);
        module_desc->ops = NULL;
    }

    if (module_desc)
    {
        AXIS2_FREE(env->allocator, module_desc);
        module_desc = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_desc_free_void_arg(
    void *module_desc,
    const axis2_env_t *env)
{
    axis2_module_desc_t *module_desc_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_desc_l = (axis2_module_desc_t *) module_desc;
    return axis2_module_desc_free(module_desc_l, env);
}

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_in_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_FLOW_CONTAINER_GET_IN_FLOW(module_desc->flow_container, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
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

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_out_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_FLOW_CONTAINER_GET_OUT_FLOW(module_desc->flow_container, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
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

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_in_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_FLOW_CONTAINER_GET_FAULT_IN_FLOW(module_desc->flow_container, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
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

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_out_flow(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_FLOW_CONTAINER_GET_FAULT_OUT_FLOW(module_desc->flow_container, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
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

AXIS2_EXTERN const axis2_qname_t *AXIS2_CALL
axis2_module_desc_get_qname(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return module_desc->qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_desc_set_qname(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);

    if (module_desc->qname)
    {
        AXIS2_QNAME_FREE(module_desc->qname, env);
        module_desc->qname = NULL;
    }

    module_desc->qname = AXIS2_QNAME_CLONE((axis2_qname_t *)qname, env);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_desc_add_op(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    const axis2_qname_t *op_qname = NULL;
    axis2_char_t *op_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);

    if (NULL == (module_desc->ops))
    {
        module_desc->ops = axis2_hash_make(env);
        if (!module_desc->ops)
            return AXIS2_FAILURE;
    }

    op_qname = AXIS2_OP_GET_QNAME(op, env);

    if (NULL == op_qname)
    {
        return AXIS2_FAILURE;
    }
    op_name = AXIS2_QNAME_TO_STRING((axis2_qname_t *)op_qname, env);
    axis2_hash_set(module_desc->ops, op_name, AXIS2_HASH_KEY_STRING, op);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_module_desc_get_all_ops(
    const  axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return module_desc->ops;
}

AXIS2_EXTERN axis2_conf_t *AXIS2_CALL
axis2_module_desc_get_parent(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return module_desc->parent;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_conf_t *parent)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, parent, AXIS2_FAILURE);

    module_desc->parent = parent;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
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

AXIS2_EXTERN axis2_param_t *AXIS2_CALL
axis2_module_desc_get_param(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAM(module_desc->params, env, name);
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_module_desc_get_all_params(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(module_desc->params, env);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_bool_t ret_state = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FAILURE);

    /* checking the locked value of parent*/
    if (NULL == module_desc->parent)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_MODULE_DESC, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    locked = AXIS2_CONF_IS_PARAM_LOCKED(module_desc->parent, env, param_name);

    if (AXIS2_TRUE == locked)
    {
        ret_state = AXIS2_TRUE;
    }
    else
    {
        param_l = axis2_module_desc_get_param(module_desc, env,
                param_name);
        if (param_l && AXIS2_TRUE == AXIS2_PARAM_IS_LOCKED(param_l, env))
            ret_state = AXIS2_TRUE;
        else
            ret_state = AXIS2_FALSE;

    }
    return ret_state;
}

AXIS2_EXTERN axis2_module_t *AXIS2_CALL
axis2_module_desc_get_module(
    const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return module_desc->module;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_desc_set_module(
    axis2_module_desc_t *module_desc,
    const axis2_env_t *env,
    axis2_module_t *module)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module, AXIS2_FAILURE);
    module_desc->module = module;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_param_container_t *AXIS2_CALL
axis2_module_desc_get_param_container(const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    return module_desc->params;
}

AXIS2_EXTERN axis2_flow_container_t *AXIS2_CALL
axis2_module_desc_get_flow_container(const axis2_module_desc_t *module_desc,
    const axis2_env_t *env)
{
    return module_desc->flow_container;
}

