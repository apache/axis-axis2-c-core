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

#include "_axiom_soap_fault.h"
#include <axiom_element.h>
#include <axiom_node.h>
#include "_axiom_soap_fault_detail.h"

/******************** impl struct ********************************************/

typedef struct axiom_soap_fault_detail_impl_t
{
    axiom_soap_fault_detail_t fault_detail;

    axiom_node_t *om_ele_node;

}
axiom_soap_fault_detail_impl_t;

/***************** Macro *****************************************************/

#define AXIS2_INTF_TO_IMPL(detail) ((axiom_soap_fault_detail_impl_t*)detail)

/****************** function prototypes ***************************************/

axis2_status_t AXIS2_CALL
axiom_soap_fault_detail_free(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axiom_soap_fault_detail_add_detail_entry
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env,
        axiom_node_t *ele_node);

axiom_children_iterator_t* AXIS2_CALL
axiom_soap_fault_detail_get_all_detail_entries
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env);



axiom_node_t* AXIS2_CALL
axiom_soap_fault_detail_get_base_node
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env);

/******************** function implementations ********************************/

AXIS2_EXTERN axiom_soap_fault_detail_t * AXIS2_CALL
axiom_soap_fault_detail_create(const axis2_env_t *env)
{
    axiom_soap_fault_detail_impl_t *fault_detail_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    fault_detail_impl = (axiom_soap_fault_detail_impl_t*)AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_soap_fault_detail_impl_t));
    if (!fault_detail_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    fault_detail_impl->fault_detail.ops = NULL;
    fault_detail_impl->om_ele_node = NULL;

    fault_detail_impl->fault_detail.ops = (axiom_soap_fault_detail_ops_t*)AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_soap_fault_detail_ops_t));
    if (!(fault_detail_impl->fault_detail.ops))
    {
        AXIS2_FREE(env->allocator, fault_detail_impl);
        fault_detail_impl = NULL;
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    fault_detail_impl->fault_detail.ops->free_fn =
        axiom_soap_fault_detail_free;

    fault_detail_impl->fault_detail.ops->add_detail_entry =
        axiom_soap_fault_detail_add_detail_entry;

    fault_detail_impl->fault_detail.ops->get_all_detail_entries =
        axiom_soap_fault_detail_get_all_detail_entries;

    fault_detail_impl->fault_detail.ops->get_base_node =
        axiom_soap_fault_detail_get_base_node;

    return &(fault_detail_impl->fault_detail);
}

AXIS2_EXTERN axiom_soap_fault_detail_t * AXIS2_CALL
axiom_soap_fault_detail_create_with_parent
(const axis2_env_t *env,
        axiom_soap_fault_t *fault)
{
    axiom_soap_fault_detail_impl_t *fault_detail_impl = NULL;
    axiom_soap_fault_detail_t *fault_detail = NULL;
    axiom_element_t *this_ele = NULL;
    axiom_node_t *this_node = NULL;
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele = NULL;
    axiom_namespace_t *parent_ns = NULL;
    int soap_version = -1;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, fault, NULL);

    fault_detail = axiom_soap_fault_detail_create(env);
    if (!fault_detail)
        return NULL;

    fault_detail_impl = AXIS2_INTF_TO_IMPL(fault_detail);

    parent_node = AXIOM_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if (!parent_node)
        return NULL;

    parent_ele  = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                parent_node, env);
    if (!parent_ele)
        return NULL;
    soap_version = axiom_soap_fault_get_soap_version(fault, env);
    if (soap_version == AXIOM_SOAP12)
    {
        parent_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    }
    this_ele = axiom_element_create(env,
            parent_node,
            AXIOM_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME,
            parent_ns,
            &this_node);
    if (!this_ele)
    {
        AXIOM_SOAP_FAULT_DETAIL_FREE(fault_detail, env);
        return NULL;
    }
    fault_detail_impl->om_ele_node = this_node;
    axiom_soap_fault_set_detail(fault, env, fault_detail);
    return  &(fault_detail_impl->fault_detail);
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_detail_free(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (fault_detail->ops)
    {
        AXIS2_FREE(env->allocator, fault_detail->ops);
        fault_detail->ops = NULL;
    }
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(fault_detail));
    fault_detail = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_detail_add_detail_entry
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    axiom_soap_fault_detail_impl_t *fault_detail_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    fault_detail_impl = AXIS2_INTF_TO_IMPL(fault_detail);

    if (AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_OM_ELEMENT_EXPECTED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    AXIOM_NODE_ADD_CHILD(fault_detail_impl->om_ele_node, env, node);
    return AXIS2_SUCCESS;
}

axiom_children_iterator_t* AXIS2_CALL
axiom_soap_fault_detail_get_all_detail_entries
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env)
{
    axiom_soap_fault_detail_impl_t *fault_detail_impl = NULL;
    axiom_element_t *om_ele = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_detail_impl = AXIS2_INTF_TO_IMPL(fault_detail);
    if (fault_detail_impl->om_ele_node)
    {
        om_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(fault_detail_impl->om_ele_node, env);
        return AXIOM_ELEMENT_GET_CHILDREN(om_ele,  env,
                fault_detail_impl->om_ele_node);
    }
    return NULL;
}


axis2_status_t AXIS2_CALL
axiom_soap_fault_detail_set_base_node
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    axiom_soap_fault_detail_impl_t *fault_detail_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    fault_detail_impl = AXIS2_INTF_TO_IMPL(fault_detail);

    if (AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    fault_detail_impl->om_ele_node = node;
    return AXIS2_SUCCESS;
}

axiom_node_t* AXIS2_CALL
axiom_soap_fault_detail_get_base_node
(axiom_soap_fault_detail_t *fault_detail,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(fault_detail)->om_ele_node;
}
