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

#include "_axiom_soap_fault_code.h"
#include <axiom_soap_fault_sub_code.h>
#include <axiom_soap_fault_value.h>
#include <axiom_soap_builder.h>
#include "_axiom_soap_fault.h"

/***************** impl struct ***********************************************/

typedef struct axiom_soap_fault_code_impl_t
{
    axiom_soap_fault_code_t fault_code;

    axiom_node_t *om_ele_node;

    axiom_soap_fault_sub_code_t *subcode;

    axiom_soap_fault_value_t *value;

    axiom_soap_builder_t *builder;

    int soap_version;

}
axiom_soap_fault_code_impl_t;

/******************** Macro **************************************************/

#define AXIS2_INTF_TO_IMPL(code) ((axiom_soap_fault_code_impl_t*)code)

/******************** function prototypes ************************************/

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_free(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env);

axiom_soap_fault_sub_code_t* AXIS2_CALL
axiom_soap_fault_code_get_sub_code(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env);

axiom_soap_fault_value_t* AXIS2_CALL
axiom_soap_fault_code_get_value(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env);


axiom_node_t* AXIS2_CALL
axiom_soap_fault_code_get_base_node(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env);

/********************* function implementation ********************************/

AXIS2_EXTERN axiom_soap_fault_code_t * AXIS2_CALL
axiom_soap_fault_code_create(const axis2_env_t *env)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_code_impl = (axiom_soap_fault_code_impl_t*) AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_soap_fault_code_impl_t));
    if (!fault_code_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    fault_code_impl->fault_code.ops = NULL;
    fault_code_impl->om_ele_node = NULL;
    fault_code_impl->subcode = NULL;
    fault_code_impl->value = NULL;
    fault_code_impl->builder = NULL;
    fault_code_impl->soap_version = AXIOM_SOAP_VERSION_NOT_SET;
    fault_code_impl->fault_code.ops =
        (axiom_soap_fault_code_ops_t*)AXIS2_MALLOC(env->allocator,
                sizeof(axiom_soap_fault_code_ops_t));

    if (!(fault_code_impl->fault_code.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, fault_code_impl);
        return NULL;
    }

    fault_code_impl->fault_code.ops->free_fn =
        axiom_soap_fault_code_free;

    fault_code_impl->fault_code.ops->get_sub_code =
        axiom_soap_fault_code_get_sub_code;

    fault_code_impl->fault_code.ops->get_value =
        axiom_soap_fault_code_get_value;

    fault_code_impl->fault_code.ops->get_base_node =
        axiom_soap_fault_code_get_base_node;

    return  &(fault_code_impl->fault_code);
}

AXIS2_EXTERN axiom_soap_fault_code_t * AXIS2_CALL
axiom_soap_fault_code_create_with_parent(const axis2_env_t *env,
        axiom_soap_fault_t *fault)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    axiom_soap_fault_code_t *fault_code = NULL;

    axiom_element_t *this_ele = NULL;
    axiom_node_t *this_node = NULL;

    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele = NULL;

    axiom_namespace_t *parent_ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, fault, NULL);

    fault_code = axiom_soap_fault_code_create(env);
    if (!fault_code)
        return NULL;

    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);

    parent_node = AXIOM_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if (!parent_node)
    {
        AXIOM_SOAP_FAULT_CODE_FREE(fault_code, env);
        return NULL;
    }
    parent_ele  = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                parent_node, env);

    if (!parent_ele)
    {
        AXIOM_SOAP_FAULT_CODE_FREE(fault_code, env);
        return NULL;
    }
    fault_code_impl->soap_version =
        axiom_soap_fault_get_soap_version(fault, env);
    if (fault_code_impl->soap_version == AXIOM_SOAP12)
    {
        parent_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    }
    this_ele = axiom_element_create(env,
            parent_node,
            AXIOM_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME,
            parent_ns,
            &this_node);
    if (!this_ele)
    {
        AXIOM_SOAP_FAULT_CODE_FREE(fault_code, env);
        return NULL;
    }

    fault_code_impl->om_ele_node = this_node;

    axiom_soap_fault_set_code(fault, env, fault_code);

    return  &(fault_code_impl->fault_code);
}

AXIS2_EXTERN axiom_soap_fault_code_t * AXIS2_CALL
axiom_soap_fault_code_create_with_parent_value(const axis2_env_t *env,
        axiom_soap_fault_t *fault,
        axis2_char_t *value)
{
    axiom_soap_fault_code_t *fault_code = NULL;
    axiom_soap_fault_value_t *fault_value = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, value, NULL);

    fault_code = axiom_soap_fault_code_create_with_parent(env, fault);
    if (!fault_code)
        return NULL;

    fault_value = axiom_soap_fault_value_create_with_code(env, fault_code);
    if (!fault_value)
    {
        axiom_soap_fault_code_free(fault_code, env);
        return NULL;
    }
    AXIOM_SOAP_FAULT_VALUE_SET_TEXT(fault_value, env, value);
    return fault_code;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_free(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);

    if (fault_code_impl->subcode)
    {
        AXIOM_SOAP_FAULT_SUB_CODE_FREE(fault_code_impl->subcode, env);
        fault_code_impl->subcode = NULL;
    }
    if (fault_code_impl->value)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_code_impl->value, env);
        fault_code_impl->value = NULL;
    }
    if (fault_code->ops)
    {
        AXIS2_FREE(env->allocator, fault_code->ops);
        fault_code->ops = NULL;
    }

    AXIS2_FREE(env->allocator, fault_code_impl);
    fault_code_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_set_value(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env,
        axiom_soap_fault_value_t *fault_val)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_PARAM_CHECK(env->error, fault_val, AXIS2_FAILURE);

    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    if (!(fault_code_impl->value))
    {
        fault_code_impl->value = fault_val;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log , AXIS2_LOG_SI,
                "trying to set fault value to fault code more than once");
    }
    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_set_sub_code(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env,
        axiom_soap_fault_sub_code_t *fault_subcode)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_subcode, AXIS2_FAILURE);

    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);

    if (!(fault_code_impl->subcode))
    {
        fault_code_impl->subcode = fault_subcode;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log , AXIS2_LOG_SI,
                "trying to set fault subcode to fault code more than once ");
    }
    return AXIS2_FAILURE;
}

axiom_soap_fault_sub_code_t* AXIS2_CALL
axiom_soap_fault_code_get_sub_code(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);

    if (fault_code_impl->subcode)
    {
        return fault_code_impl->subcode;
    }
    else if (fault_code_impl->builder)
    {
        while (!(fault_code_impl->subcode) &&
                !(AXIOM_NODE_IS_COMPLETE(fault_code_impl->om_ele_node, env)))
        {
            status = AXIOM_SOAP_BUILDER_NEXT(fault_code_impl->builder, env);
            if (status == AXIS2_FAILURE)
            {
                break;
            }
        }
    }
    return fault_code_impl->subcode;
}

axiom_soap_fault_value_t* AXIS2_CALL
axiom_soap_fault_code_get_value(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;

    int status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, NULL);

    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);

    if (fault_code_impl->value)
    {
        return fault_code_impl->value;
    }
    else if (fault_code_impl->builder)
    {
        while (!(fault_code_impl->value) &&
                !(AXIOM_NODE_IS_COMPLETE(fault_code_impl->om_ele_node, env)))
        {
            status = AXIOM_SOAP_BUILDER_NEXT(fault_code_impl->builder, env);
            if (status == AXIS2_FAILURE)
            {
                break;
            }
        }
    }
    return fault_code_impl->value;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_set_base_node(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);

    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);

    if (AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    fault_code_impl->om_ele_node = node;
    return AXIS2_SUCCESS;

}

axiom_node_t* AXIS2_CALL
axiom_soap_fault_code_get_base_node(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(fault_code)->om_ele_node;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_set_builder(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env,
        axiom_soap_builder_t *soap_builder)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_builder, AXIS2_FAILURE);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    fault_code_impl->builder = soap_builder;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axiom_soap_fault_code_get_soap_version(axiom_soap_fault_code_t  *fault_code,
        const axis2_env_t *env)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    return fault_code_impl->soap_version;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_code_set_soap_version(axiom_soap_fault_code_t *fault_code,
        const axis2_env_t *env,
        int soap_version)
{
    axiom_soap_fault_code_impl_t *fault_code_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_code_impl = AXIS2_INTF_TO_IMPL(fault_code);
    fault_code_impl->soap_version = soap_version;
    return AXIS2_SUCCESS;
}

