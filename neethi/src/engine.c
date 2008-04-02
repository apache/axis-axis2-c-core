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

#include <neethi_engine.h>
#include <neethi_assertion_builder.h>

/*Private functions*/

neethi_all_t *AXIS2_CALL get_operator_all(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

neethi_exactlyone_t *AXIS2_CALL get_operator_exactlyone(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

neethi_reference_t *AXIS2_CALL get_operator_reference(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

neethi_policy_t *AXIS2_CALL get_operator_neethi_policy(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

axis2_status_t AXIS2_CALL process_operation_element(
    const axutil_env_t *env,
    neethi_operator_t *neethi_operator,
    axiom_node_t *node,
    axiom_element_t *element);

axis2_status_t AXIS2_CALL neethi_engine_add_policy_component(
    const axutil_env_t *env,
    neethi_operator_t *container_operator,
    neethi_operator_t *component);

axis2_bool_t AXIS2_CALL operator_is_empty(
    neethi_operator_t *operator,
    const axutil_env_t *env);

neethi_exactlyone_t *AXIS2_CALL compute_resultant_component(
    axutil_array_list_t *normalized_inner_components,
    neethi_operator_type_t type,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL operator_get_components(
    neethi_operator_t *operator,
    const axutil_env_t *env);

neethi_exactlyone_t *AXIS2_CALL normalize_operator(
    neethi_operator_t *operator,
    neethi_registry_t *registry,
    axis2_bool_t deep,
    const axutil_env_t *env);

neethi_exactlyone_t *AXIS2_CALL get_cross_product(
    neethi_exactlyone_t *exactlyone1,
    neethi_exactlyone_t *exactlyone2,
    const axutil_env_t *env);

/*Implementations*/

/*This is the function which is called from outside*/

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_engine_get_policy(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
    {
        return get_operator_neethi_policy(env, node, element);
    }

neethi_all_t *AXIS2_CALL
get_operator_all(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    neethi_all_t *all = NULL;
    neethi_operator_t *neethi_operator = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    all = neethi_all_create(env);

    if (!all)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_operator = neethi_operator_create(env);
    if (!neethi_operator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_operator_set_value(neethi_operator, env, all, OPERATOR_TYPE_ALL);

    status = process_operation_element(env, neethi_operator, node, element);

    neethi_operator_set_value_null(neethi_operator, env);
    neethi_operator_free(neethi_operator, env);
    neethi_operator = NULL;

    if (status != AXIS2_SUCCESS)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NEETHI_ALL_CREATION_FAILED,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[neethi] All creation failed");
        neethi_all_free(all, env);
        all = NULL;
        return NULL;
    }
    return all;
}

neethi_exactlyone_t *AXIS2_CALL
get_operator_exactlyone(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    neethi_exactlyone_t *exactlyone = NULL;
    neethi_operator_t *neethi_operator = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    exactlyone = neethi_exactlyone_create(env);

    if (!exactlyone)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");

        return NULL;
    }
    neethi_operator = neethi_operator_create(env);
    if (!neethi_operator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");

        return NULL;
    }
    neethi_operator_set_value(neethi_operator, env, exactlyone,
                              OPERATOR_TYPE_EXACTLYONE);
    status = process_operation_element(env, neethi_operator, node, element);

    neethi_operator_set_value_null(neethi_operator, env);
    neethi_operator_free(neethi_operator, env);
    neethi_operator = NULL;

    if (status != AXIS2_SUCCESS)
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_NEETHI_EXACTLYONE_CREATION_FAILED,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] Exactlyone creation failed.");
        neethi_exactlyone_free(exactlyone, env);
        exactlyone = NULL;
        return NULL;
    }

    return exactlyone;
}

neethi_reference_t *AXIS2_CALL
get_operator_reference(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    neethi_reference_t *reference = NULL;
    axutil_qname_t *qname = NULL;
    axis2_char_t *attribute_value = NULL;

    reference = neethi_reference_create(env);

    if (!reference)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");

        return NULL;
    }
    qname = axutil_qname_create(env, NEETHI_URI, NULL, NULL);

    if (!qname)
    {
        return NULL;
    }

    attribute_value = axiom_element_get_attribute_value(element, env, qname);
    if (attribute_value)
    {
        neethi_reference_set_uri(reference, env, attribute_value);
    }
    return reference;
}

neethi_policy_t *AXIS2_CALL
get_operator_neethi_policy(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    neethi_policy_t *neethi_policy = NULL;
    neethi_operator_t *neethi_operator = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    neethi_policy = neethi_policy_create(env);

    if (!neethi_policy)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_operator = neethi_operator_create(env);
    if (!neethi_operator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_operator_set_value(neethi_operator, env, neethi_policy,
                              OPERATOR_TYPE_POLICY);
    status = process_operation_element(env, neethi_operator, node, element);

    neethi_operator_set_value_null(neethi_operator, env);
    neethi_operator_free(neethi_operator, env);
    neethi_operator = NULL;

    if (status != AXIS2_SUCCESS)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NEETHI_POLICY_CREATION_FAILED,
                        AXIS2_FAILURE);

        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] Policy creation failed.");
        neethi_policy_free(neethi_policy, env);
        neethi_policy = NULL;
        return NULL;
    }
    return neethi_policy;
}

axis2_status_t AXIS2_CALL
process_operation_element(
    const axutil_env_t *env,
    neethi_operator_t *neethi_operator,
    axiom_node_t *node,
    axiom_element_t *element)
{

    neethi_operator_type_t type;
    axiom_element_t *child_element = NULL;
    axiom_node_t *child_node = NULL;
    axiom_children_iterator_t *children_iter = NULL;
    void *value = NULL;

    type = neethi_operator_get_type(neethi_operator, env);
    value = neethi_operator_get_value(neethi_operator, env);
    if (type == OPERATOR_TYPE_POLICY)
    {
        /*Adding attribute values to the hashmap logic
         *comes here.Following is a little hack until we implement attribute
         hash map logic.*/

        axis2_char_t *id = NULL;
        axutil_qname_t *qname = NULL;
        axis2_char_t *name = NULL;

        qname =
            axutil_qname_create(env, NEETHI_ID, NEETHI_WSU_NS,
                                NULL/*NEETHI_WSU_NS_PREFIX*/);
        if (!qname)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
            return AXIS2_FAILURE;
        }
        id = axiom_element_get_attribute_value(element, env, qname);
        axutil_qname_free(qname, env);
        qname = NULL;

        qname = axutil_qname_create(env, NEETHI_NAME, NULL, NULL);
        if (!qname)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
            return AXIS2_FAILURE;
        }
        name = axiom_element_get_attribute_value(element, env, qname);
        axutil_qname_free(qname, env);
        qname = NULL;

        if (id)
        {
            neethi_policy_set_id((neethi_policy_t *) value, env, id);
        }
        if (name)
        {
            neethi_policy_set_name((neethi_policy_t *) value, env, name);
        }
    }

    children_iter = axiom_element_get_children(element, env, node);
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
        {
            child_node = axiom_children_iterator_next(children_iter, env);
            if (child_node)
            {
                if (axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
                {
                    child_element =
                        (axiom_element_t *)
                        axiom_node_get_data_element(child_node, env);
                    if (child_element)
                    {
                        axiom_namespace_t *namespace = NULL;
                        axis2_char_t *uri = NULL;
                        axis2_char_t *local_name = NULL;
                        neethi_operator_t *operator = NULL;
                        local_name =
                            axiom_element_get_localname(child_element, env);

                        namespace =
                            axiom_element_get_namespace(child_element, env,
                                                        child_node);
                        if (!namespace)
                        {
                            AXIS2_ERROR_SET(env->error,
                                            AXIS2_ERROR_NEETHI_ELEMENT_WITH_NO_NAMESPACE,
                                            AXIS2_FAILURE);
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                            "[neethi] Element with no namespace");
                            return AXIS2_FAILURE;
                        }
                        uri = axiom_namespace_get_uri(namespace, env);
                        if (!uri)
                        {
                            AXIS2_ERROR_SET(env->error,
                                            AXIS2_ERROR_INVALID_EMPTY_NAMESPACE_URI,
                                            AXIS2_FAILURE);
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                            "[neethi] Invalid Empty namespace uri.");
                            return AXIS2_FAILURE;
                        }
                        if (axutil_strcmp(uri, NEETHI_NAMESPACE) == 0)
                        {
                            if (axutil_strcmp(local_name, NEETHI_POLICY) == 0)
                            {
                                neethi_policy_t *neethi_policy = NULL;
                                neethi_policy =
                                    get_operator_neethi_policy(env, child_node,
                                                               child_element);
                                if (neethi_policy)
                                {
                                    operator = neethi_operator_create(
    env);
                                    neethi_operator_set_value(operator, env,
                                                              neethi_policy,
                                                              OPERATOR_TYPE_POLICY);
                                    neethi_engine_add_policy_component(env,
                                                                       neethi_operator,
                                                                       operator);
                                }
                                else
                                {
                                    AXIS2_ERROR_SET(env->error,
                                                    AXIS2_ERROR_NEETHI_POLICY_CREATION_FAILED_FROM_ELEMENT,
                                                    AXIS2_FAILURE);
                                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                                    "[neethi] Policy creation failed from element.");
                                    return AXIS2_FAILURE;
                                }
                            }
                            else if (axutil_strcmp(local_name, NEETHI_ALL) == 0)
                            {
                                neethi_all_t *all = NULL;
                                all =
                                    get_operator_all(env, child_node,
                                                     child_element);
                                if (all)
                                {
                                    operator = neethi_operator_create(
    env);
                                    neethi_operator_set_value(operator, env,
                                                              all,
                                                              OPERATOR_TYPE_ALL);
                                    neethi_engine_add_policy_component(env,
                                                                       neethi_operator,
                                                                       operator);
                                }
                                else
                                {
                                    AXIS2_ERROR_SET(env->error,
                                                    AXIS2_ERROR_NEETHI_ALL_CREATION_FAILED_FROM_ELEMENT,
                                                    AXIS2_FAILURE);
                                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                                    "[neethi] All creation failed from element.");
                                    return AXIS2_FAILURE;
                                }
                            }
                            else if (axutil_strcmp
                                     (local_name, NEETHI_EXACTLYONE) == 0)
                            {
                                neethi_exactlyone_t *exactlyone = NULL;
                                exactlyone =
                                    get_operator_exactlyone(env, child_node,
                                                            child_element);
                                if (exactlyone)
                                {
                                    operator = neethi_operator_create(
    env);
                                    neethi_operator_set_value(operator, env,
                                                              exactlyone,
                                                              OPERATOR_TYPE_EXACTLYONE);
                                    neethi_engine_add_policy_component(env,
                                                                       neethi_operator,
                                                                       operator);
                                }
                                else
                                {
                                    AXIS2_ERROR_SET(env->error,
                                                    AXIS2_ERROR_NEETHI_EXACTLYONE_CREATION_FAILED_FROM_ELEMENT,
                                                    AXIS2_FAILURE);
                                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                                    "[neethi] Exactlyone creation failed from element.");
                                    return AXIS2_FAILURE;
                                }
                            }
                            else if (axutil_strcmp(local_name, NEETHI_REFERENCE)
                                     == 0)
                            {
                                neethi_reference_t *reference = NULL;
                                reference =
                                    get_operator_reference(env, child_node,
                                                           child_element);
                                if (reference)
                                {
                                    operator = neethi_operator_create(
    env);
                                    neethi_operator_set_value(operator, env,
                                                              reference,
                                                              OPERATOR_TYPE_REFERENCE);
                                    neethi_engine_add_policy_component(env,
                                                                       neethi_operator,
                                                                       operator);
                                }
                                else
                                {
                                    AXIS2_ERROR_SET(env->error,
                                                    AXIS2_ERROR_NEETHI_REFERENCE_CREATION_FAILED_FROM_ELEMENT,
                                                    AXIS2_FAILURE);
                                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                                    "[neethi] Reference cretion failed from element.");
                                    return AXIS2_FAILURE;
                                }
                            }
                        }
                        else
                        {
                            neethi_assertion_t *assertion = NULL;
                            assertion =
                                neethi_assertion_builder_build(env, child_node,
                                                               child_element);
                            if (assertion)
                            {
                                operator = neethi_operator_create(
    env);
                                neethi_operator_set_value(operator, env,
                                                          assertion,
                                                          OPERATOR_TYPE_ASSERTION);
                                neethi_engine_add_policy_component(env,
                                                                   neethi_operator,
                                                                   operator);
                            }
                            else
                            {
                                AXIS2_ERROR_SET(env->error,
                                                AXIS2_ERROR_NEETHI_ASSERTION_CREATION_FAILED_FROM_ELEMENT,
                                                AXIS2_FAILURE);
                                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                                "[neethi] Assertion creation failed from element.");
                                return AXIS2_FAILURE;
                            }
                        }
                    }
                }
            }
        }
        return AXIS2_SUCCESS;
    }
    else
        return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
neethi_engine_add_policy_component(
    const axutil_env_t *env,
    neethi_operator_t *container_operator,
    neethi_operator_t *component)
{

    neethi_operator_type_t type;
    void *value = NULL;
    neethi_policy_t *neethi_policy = NULL;
    neethi_exactlyone_t *exactlyone = NULL;
    neethi_all_t *all = NULL;
    neethi_assertion_t *assertion = NULL;

    type = neethi_operator_get_type(container_operator, env);
    value = neethi_operator_get_value(container_operator, env);

    if (value)
    {
        switch (type)
        {
        case OPERATOR_TYPE_POLICY:
            neethi_policy = (neethi_policy_t *) value;
            neethi_policy_add_operator(neethi_policy, env, component);
            break;

        case OPERATOR_TYPE_ALL:
            all = (neethi_all_t *) value;
            neethi_all_add_operator(all, env, component);
            break;

        case OPERATOR_TYPE_EXACTLYONE:
            exactlyone = (neethi_exactlyone_t *) value;
            neethi_exactlyone_add_operator(exactlyone, env, component);
            break;

        case OPERATOR_TYPE_UNKNOWN:
            return AXIS2_FAILURE;
            break;

        case OPERATOR_TYPE_ASSERTION:
            assertion = (neethi_assertion_t *) value;
            neethi_assertion_add_operator(assertion, env, component);
            break;

        case OPERATOR_TYPE_REFERENCE:
            break;
        }
        return AXIS2_SUCCESS;
    }
    else
        return AXIS2_FAILURE;
}

/***************************************/

/*This function is only for testing*
 *Remove it later*/
void
check_neethi_policy(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    axutil_array_list_t *list = NULL;
    neethi_operator_t *op = NULL;
    neethi_operator_type_t type;

    list = neethi_policy_get_policy_components(neethi_policy, env);

    if (axutil_array_list_size(list, env) > 1)
    {
        return;
    }
    op = (neethi_operator_t *) axutil_array_list_get(list, env, 0);
    type = neethi_operator_get_type(op, env);
    if (type == OPERATOR_TYPE_EXACTLYONE)
    {
        void *value = neethi_operator_get_value(op, env);
        if (value)
        {
            return;
        }
    }
    else
    {
        return;
    }
}

/************************************************/

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_engine_get_normalize(
    const axutil_env_t *env,
    axis2_bool_t deep,
    neethi_policy_t *neethi_policy)
{
    return neethi_engine_normalize(env, neethi_policy, NULL, deep);
}

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_engine_normalize(
    const axutil_env_t *env,
    neethi_policy_t *neethi_policy,
    neethi_registry_t *registry,
    axis2_bool_t deep)
{
    neethi_policy_t *resultant_neethi_policy = NULL;
    neethi_operator_t *operator = NULL;
    neethi_operator_t *component = NULL;
    neethi_exactlyone_t *exactlyone = NULL;
    axis2_char_t *policy_name = NULL;
    axis2_char_t *policy_id = NULL;

    resultant_neethi_policy = neethi_policy_create(env);
    if (!resultant_neethi_policy)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    policy_name = neethi_policy_get_name(neethi_policy, env);
    if (policy_name)
    {
        neethi_policy_set_name(resultant_neethi_policy, env, policy_name);
    }
    policy_id = neethi_policy_get_id(neethi_policy, env);
    if (policy_id)
    {
        neethi_policy_set_id(resultant_neethi_policy, env, policy_id);
    }

    operator = neethi_operator_create(
    env);
    if (!operator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    neethi_operator_set_value(operator, env, neethi_policy,
                              OPERATOR_TYPE_POLICY);

    exactlyone = normalize_operator(operator, registry, deep, env);

    neethi_operator_set_value_null(operator, env);
    neethi_operator_free(operator, env);
    operator = NULL;

    if (exactlyone)
    {
        component = neethi_operator_create(env);
        neethi_operator_set_value(component, env, exactlyone,
                                  OPERATOR_TYPE_EXACTLYONE);
        neethi_policy_add_operator(resultant_neethi_policy, env, component);

        return resultant_neethi_policy;
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NEETHI_NORMALIZATION_FAILED,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] Normalization failed.");
        return NULL;
    }
}

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_engine_merge(
    const axutil_env_t *env,
    neethi_policy_t *neethi_policy1,
    neethi_policy_t *neethi_policy2)
{

    neethi_exactlyone_t *exactlyone1 = NULL;
    neethi_exactlyone_t *exactlyone2 = NULL;
    neethi_exactlyone_t *exactlyone = NULL;
    neethi_policy_t *neethi_policy = NULL;
    neethi_operator_t *component = NULL;

    exactlyone1 = neethi_policy_get_exactlyone(neethi_policy1, env);
    exactlyone2 = neethi_policy_get_exactlyone(neethi_policy2, env);

    if (!exactlyone1 || !exactlyone2)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NEETHI_WRONG_INPUT_FOR_MERGE,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] Wrong input for merge.");
        return NULL;
    }
    exactlyone = get_cross_product(exactlyone1, exactlyone2, env);
    if (exactlyone)
    {
        neethi_policy = neethi_policy_create(env);
        component = neethi_operator_create(env);
        if (!neethi_policy || !component)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
            return NULL;
        }
        neethi_operator_set_value(component, env, exactlyone,
                                  OPERATOR_TYPE_EXACTLYONE);
        neethi_policy_add_operator(neethi_policy, env, component);
        return neethi_policy;
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NEETHI_CROSS_PRODUCT_FAILED,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] Cross product failed.");
        return NULL;
    }
}

axis2_bool_t AXIS2_CALL
operator_is_empty(
    neethi_operator_t *operator,
    const axutil_env_t *env)
{

    neethi_operator_type_t type;
    void *value = NULL;
    neethi_policy_t *neethi_policy = NULL;
    neethi_exactlyone_t *exactlyone = NULL;
    neethi_all_t *all = NULL;
    neethi_assertion_t *assertion = NULL;

    type = neethi_operator_get_type(operator, env);
    value = neethi_operator_get_value(operator, env);

    if (value)
    {
        switch (type)
        {
        case OPERATOR_TYPE_POLICY:
            neethi_policy = (neethi_policy_t *) value;
            return neethi_policy_is_empty(neethi_policy, env);
            break;

        case OPERATOR_TYPE_ALL:
            all = (neethi_all_t *) value;
            return neethi_all_is_empty(all, env);
            break;

        case OPERATOR_TYPE_EXACTLYONE:
            exactlyone = (neethi_exactlyone_t *) value;
            return neethi_exactlyone_is_empty(exactlyone, env);
            break;

        case OPERATOR_TYPE_UNKNOWN:
            return AXIS2_FALSE;
            break;

        case OPERATOR_TYPE_ASSERTION:
            assertion = (neethi_assertion_t *) value;
            return neethi_assertion_is_empty(assertion, env);
            break;

        case OPERATOR_TYPE_REFERENCE:
            break;

        }
        return AXIS2_FALSE;
    }
    else
        return AXIS2_FALSE;
}

axutil_array_list_t *AXIS2_CALL
operator_get_components(
    neethi_operator_t *operator,
    const axutil_env_t *env)
{

    neethi_operator_type_t type;
    void *value = NULL;
    neethi_policy_t *neethi_policy = NULL;
    neethi_exactlyone_t *exactlyone = NULL;
    neethi_all_t *all = NULL;
    neethi_assertion_t *assertion = NULL;

    type = neethi_operator_get_type(operator, env);
    value = neethi_operator_get_value(operator, env);

    if (value)
    {
        switch (type)
        {
        case OPERATOR_TYPE_POLICY:
            neethi_policy = (neethi_policy_t *) value;
            return neethi_policy_get_policy_components(neethi_policy, env);
            break;

        case OPERATOR_TYPE_ALL:
            all = (neethi_all_t *) value;
            return neethi_all_get_policy_components(all, env);
            break;

        case OPERATOR_TYPE_EXACTLYONE:
            exactlyone = (neethi_exactlyone_t *) value;
            return neethi_exactlyone_get_policy_components(exactlyone, env);
            break;

        case OPERATOR_TYPE_UNKNOWN:
            return NULL;
            break;

        case OPERATOR_TYPE_ASSERTION:
            assertion = (neethi_assertion_t *) value;
            return neethi_assertion_get_policy_components(assertion, env);
            break;

        case OPERATOR_TYPE_REFERENCE:
            break;
        }
    }

    return NULL;
}

neethi_exactlyone_t *AXIS2_CALL
normalize_operator(
    neethi_operator_t *operator,
    neethi_registry_t *registry,
    axis2_bool_t deep,
    const axutil_env_t *env)
{
    axutil_array_list_t *child_component_list = NULL;
    neethi_operator_t *child_component = NULL;
    axutil_array_list_t *arraylist = NULL;
    int i = 0;

    neethi_operator_type_t type = neethi_operator_get_type(operator, env);

    if (operator_is_empty(operator, env))
    {
        neethi_exactlyone_t *exactlyone = NULL;
        exactlyone = neethi_exactlyone_create(env);
        if (!exactlyone)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
            return NULL;
        }
        if (type != OPERATOR_TYPE_EXACTLYONE)
        {
            neethi_all_t *all = NULL;
            neethi_operator_t *component = NULL;
            all = neethi_all_create(env);
            component = neethi_operator_create(env);
            if (!all || !component)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                return NULL;
            }
            neethi_operator_set_value(component, env, all, OPERATOR_TYPE_ALL);
            neethi_exactlyone_add_operator(exactlyone, env, component);
        }
        return exactlyone;
    }

    child_component_list = axutil_array_list_create(env, 0);
    arraylist = operator_get_components(operator, env);

    for (i = 0; i < axutil_array_list_size(arraylist, env); i++)
    {
        neethi_operator_type_t component_type;
        child_component =
            (neethi_operator_t *) axutil_array_list_get(arraylist, env, i);
        component_type = neethi_operator_get_type(child_component, env);

        if (component_type == OPERATOR_TYPE_ASSERTION)
        {
            /*Assertion normalization part comes here */
            if (deep)
            {
                return NULL;
            }
            else
            {
                neethi_exactlyone_t *exactlyone = NULL;
                neethi_all_t *all = NULL;
                neethi_operator_t *op = NULL;

                exactlyone = neethi_exactlyone_create(env);
                all = neethi_all_create(env);
                op = neethi_operator_create(env);

                if (!all || !op || !exactlyone)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                    AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                    return NULL;
                }
                neethi_all_add_operator(all, env, child_component);
                neethi_operator_set_value(op, env, all, OPERATOR_TYPE_ALL);
                neethi_exactlyone_add_operator(exactlyone, env, op);
                axutil_array_list_add(child_component_list, env, exactlyone);
            }
        }
        else if (component_type == OPERATOR_TYPE_POLICY)
        {
            neethi_policy_t *neethi_policy = NULL;
            neethi_all_t *all = NULL;
            axutil_array_list_t *children = NULL;
            neethi_operator_t *to_normalize = NULL;
            neethi_exactlyone_t *exactlyone = NULL;

            all = neethi_all_create(env);
            if (!all)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                return NULL;
            }
            neethi_policy =
                (neethi_policy_t *) neethi_operator_get_value(child_component,
                                                              env);
            if (neethi_policy)
            {
                children =
                    neethi_policy_get_policy_components(neethi_policy, env);
                if (children)
                {
                    neethi_all_add_policy_components(all, children, env);
                    to_normalize = neethi_operator_create(env);
                    if (!to_normalize)
                    {
                        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                        AXIS2_FAILURE);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                        return NULL;
                    }
                    neethi_operator_set_value(to_normalize, env, all,
                                              OPERATOR_TYPE_ALL);
                    exactlyone =
                        normalize_operator(to_normalize, registry, deep, env);
                    if (exactlyone)
                    {
                        axutil_array_list_add(child_component_list, env,
                                              exactlyone);
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                                    AXIS2_ERROR_NEETHI_NO_CHILDREN_POLICY_COMPONENTS,
                                    AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "[neethi] No children policy components");
                    return NULL;
                }
            }
        }

        else if (component_type == OPERATOR_TYPE_REFERENCE)
        {
            neethi_reference_t *policy_ref = NULL;
            axis2_char_t *uri = NULL;
            neethi_policy_t *policy = NULL;
            neethi_all_t *all = NULL;
            axutil_array_list_t *children = NULL;
            neethi_operator_t *to_normalize = NULL;
            neethi_exactlyone_t *exactlyone = NULL;

            policy_ref =
                (neethi_reference_t *)
                neethi_operator_get_value(child_component, env);
            uri = neethi_reference_get_uri(policy_ref, env);
            if (!uri)
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_NEETHI_URI_NOT_SPECIFIED,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Uri not specified");
                return NULL;
            }
            policy = neethi_registry_lookup(registry, env, uri);
            if (!policy)
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_NEETHI_NO_ENTRY_FOR_THE_GIVEN_URI,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] No entry for the given uri");
                return NULL;
            }
            neethi_operator_set_value(child_component, env, policy,
                                      OPERATOR_TYPE_POLICY);

            all = neethi_all_create(env);
            if (!all)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                return NULL;
            }

            policy =
                (neethi_policy_t *) neethi_operator_get_value(child_component,
                                                              env);
            if (policy)
            {
                children = neethi_policy_get_policy_components(policy, env);
                if (children)
                {
                    neethi_all_add_policy_components(all, children, env);
                    to_normalize = neethi_operator_create(env);
                    if (!to_normalize)
                    {
                        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                        AXIS2_FAILURE);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                        return NULL;
                    }
                    neethi_operator_set_value(to_normalize, env, all,
                                              OPERATOR_TYPE_ALL);
                    exactlyone =
                        normalize_operator(to_normalize, registry, deep, env);
                    if (exactlyone)
                    {
                        axutil_array_list_add(child_component_list, env,
                                              exactlyone);
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                                    AXIS2_ERROR_NEETHI_NO_CHILDREN_POLICY_COMPONENTS,
                                    AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "[neethi] No children policy components");
                    return NULL;
                }
            }
        }
        else
        {
            neethi_exactlyone_t *exactlyone = NULL;
            exactlyone =
                normalize_operator(child_component, registry, deep, env);
            if (exactlyone)
            {
                axutil_array_list_add(child_component_list, env, exactlyone);
            }
        }
    }

    return compute_resultant_component(child_component_list, type, env);
}

neethi_exactlyone_t *AXIS2_CALL
compute_resultant_component(
    axutil_array_list_t * normalized_inner_components,
    neethi_operator_type_t type,
    const axutil_env_t * env)
{
    neethi_exactlyone_t *exactlyone = NULL;

    if (type == OPERATOR_TYPE_EXACTLYONE)
    {
        int i = 0;
        neethi_exactlyone_t *inner_exactlyone = NULL;
        exactlyone = neethi_exactlyone_create(env);

        for (i = 0;
             i < axutil_array_list_size(normalized_inner_components, env); i++)
        {
            inner_exactlyone =
                (neethi_exactlyone_t *)
                axutil_array_list_get(normalized_inner_components, env, i);
            if (inner_exactlyone)
            {
                neethi_exactlyone_add_policy_components(exactlyone,
                                                        neethi_exactlyone_get_policy_components
                                                        (inner_exactlyone, env),
                                                        env);
            }
            else
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_NEETHI_EXACTLYONE_NOT_FOUND_IN_NORMALIZED_POLICY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Exactlyone not found in normalized policy");
                return NULL;
            }
        }
    }
    else if (type == OPERATOR_TYPE_POLICY || type == OPERATOR_TYPE_ALL)
    {
        if (axutil_array_list_size(normalized_inner_components, env) > 1)
        {
            int i = 0;
            exactlyone =
                (neethi_exactlyone_t *)
                axutil_array_list_get(normalized_inner_components, env, 0);
            if (!exactlyone)
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_NEETHI_EXACTLYONE_NOT_FOUND_IN_NORMALIZED_POLICY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Exactlyone not found in normalized policy");
                return NULL;
            }
            if (!neethi_exactlyone_is_empty(exactlyone, env))
            {
                neethi_exactlyone_t *current_exactlyone = NULL;
                i = 1;
                for (i = 1;
                     i < axutil_array_list_size(normalized_inner_components,
                                                env); i++)
                {
                    current_exactlyone =
                        (neethi_exactlyone_t *)
                        axutil_array_list_get(normalized_inner_components, env,
                                              i);
                    if (!current_exactlyone)
                    {
                        AXIS2_ERROR_SET(env->error,
                                        AXIS2_ERROR_NEETHI_EXACTLYONE_NOT_FOUND_IN_NORMALIZED_POLICY,
                                        AXIS2_FAILURE);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                        "[neethi] Exactlyone not found in normalized policy");
                        return NULL;
                    }
                    if (neethi_exactlyone_is_empty(current_exactlyone, env))
                    {
                        exactlyone = current_exactlyone;
                        break;
                    }
                    else
                    {
                        neethi_exactlyone_t *temp = NULL;
                        neethi_exactlyone_t *temp1 = NULL;
                        temp = exactlyone;
                        temp1 = current_exactlyone;
                        exactlyone =
                            get_cross_product(exactlyone, current_exactlyone,
                                              env);
                        neethi_exactlyone_free(temp, env);
                        neethi_exactlyone_free(temp1, env);
                        temp = NULL;
                        temp1 = NULL;
                    }
                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_NEETHI_EXACTLYONE_IS_EMPTY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Exactlyone is Empty");
                return NULL;
            }
        }
        else
        {
            exactlyone =
                (neethi_exactlyone_t *)
                axutil_array_list_get(normalized_inner_components, env, 0);
        }
    }
    axutil_array_list_free(normalized_inner_components, env);
    normalized_inner_components = NULL;

    return exactlyone;
}

neethi_exactlyone_t *AXIS2_CALL
get_cross_product(
    neethi_exactlyone_t *exactlyone1,
    neethi_exactlyone_t *exactlyone2,
    const axutil_env_t *env)
{
    neethi_exactlyone_t *cross_product = NULL;
    neethi_all_t *cross_product_all = NULL;
    neethi_all_t *current_all1 = NULL;
    neethi_all_t *current_all2 = NULL;
    axutil_array_list_t *array_list1 = NULL;
    axutil_array_list_t *array_list2 = NULL;
    neethi_operator_t *component = NULL;
    int i = 0;
    int j = 0;

    cross_product = neethi_exactlyone_create(env);
    if (!cross_product)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    array_list1 = neethi_exactlyone_get_policy_components(exactlyone1, env);
    array_list2 = neethi_exactlyone_get_policy_components(exactlyone2, env);

    if (!array_list1 || !array_list2)
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_NEETHI_NO_CHILDREN_POLICY_COMPONENTS,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] No children policy components");
        return NULL;
    }

    for (i = 0; i < axutil_array_list_size(array_list1, env); i++)
    {
        current_all1 = (neethi_all_t *) neethi_operator_get_value((neethi_operator_t *) axutil_array_list_get(array_list1, env, i), env);

        if (!current_all1)
        {
            AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_NEETHI_ALL_NOT_FOUND_WHILE_GETTING_CROSS_PRODUCT,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[neethi] All not found while getting cross product");
            return NULL;
        }

        for (j = 0; j < axutil_array_list_size(array_list2, env); j++)
        {
            current_all2 = (neethi_all_t *) neethi_operator_get_value((neethi_operator_t *) axutil_array_list_get(array_list2, env, j), env);

            if (!current_all2)
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_NEETHI_ALL_NOT_FOUND_WHILE_GETTING_CROSS_PRODUCT,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] All not found while getting cross product");
                return NULL;
            }

            cross_product_all = neethi_all_create(env);
            if (!cross_product_all)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                return NULL;
            }
            neethi_all_add_policy_components(cross_product_all,
                                             neethi_all_get_policy_components
                                             (current_all1, env), env);

            neethi_all_add_policy_components(cross_product_all,
                                             neethi_all_get_policy_components
                                             (current_all2, env), env);

            component = neethi_operator_create(env);
            if (!component)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
                return NULL;
            }
            neethi_operator_set_value(component, env, cross_product_all,
                                      OPERATOR_TYPE_ALL);
            neethi_exactlyone_add_operator(cross_product, env, component);
        }
    }
    return cross_product;
}

/*These functions are for serializing a policy object*/

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
neethi_engine_serialize(
    neethi_policy_t *policy,
    const axutil_env_t *env)
{

    return neethi_policy_serialize(policy, NULL, env);
}
