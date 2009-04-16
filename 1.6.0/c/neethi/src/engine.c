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
#include <axiom_attribute.h>

/*Private functions*/

static neethi_all_t *neethi_engine_get_operator_all(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

static neethi_exactlyone_t *neethi_engine_get_operator_exactlyone(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

static neethi_reference_t *neethi_engine_get_operator_reference(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

static neethi_policy_t *neethi_engine_get_operator_neethi_policy(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element);

static axis2_status_t neethi_engine_process_operation_element(
    const axutil_env_t *env,
    neethi_operator_t *neethi_operator,
    axiom_node_t *node,
    axiom_element_t *element);

static axis2_status_t neethi_engine_add_policy_component(
    const axutil_env_t *env,
    neethi_operator_t *container_operator,
    neethi_operator_t *component);

static axis2_bool_t neethi_engine_operator_is_empty(
    neethi_operator_t *operator,
    const axutil_env_t *env);

static neethi_exactlyone_t *neethi_engine_compute_resultant_component(
    axutil_array_list_t *normalized_inner_components,
    neethi_operator_type_t type,
    const axutil_env_t *env);

static axutil_array_list_t *neethi_engine_operator_get_components(
    neethi_operator_t *operator,
    const axutil_env_t *env);

static neethi_exactlyone_t *neethi_engine_normalize_operator(
    neethi_operator_t *operator,
    neethi_registry_t *registry,
    axis2_bool_t deep,
    const axutil_env_t *env);

static neethi_exactlyone_t *neethi_engine_get_cross_product(
    neethi_exactlyone_t *exactlyone1,
    neethi_exactlyone_t *exactlyone2,
    const axutil_env_t *env);

static void neethi_engine_clear_element_attributes(
    axutil_hash_t *attr_hash,
    const axutil_env_t *env);


/*Implementations*/

/*This is the function which is called from outside*/

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_engine_get_policy(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    /* This function will be called recursively */

    return neethi_engine_get_operator_neethi_policy(env, node, element);
}

static neethi_all_t *neethi_engine_get_operator_all(
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

    status = neethi_engine_process_operation_element(env, neethi_operator, node, element);

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

static neethi_exactlyone_t *neethi_engine_get_operator_exactlyone(
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
    status = neethi_engine_process_operation_element(env, neethi_operator, node, element);

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

neethi_reference_t *neethi_engine_get_operator_reference(
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

/* This function will be called when we encounter a wsp:Policy
 * element */


static neethi_policy_t *neethi_engine_get_operator_neethi_policy(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    neethi_policy_t *neethi_policy = NULL;
    neethi_operator_t *neethi_operator = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    /* Creates a policy struct */

    neethi_policy = neethi_policy_create(env);

    if (!neethi_policy)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    /* Then we wrap it in a neethi_operator */

    neethi_operator = neethi_operator_create(env);
    if (!neethi_operator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_operator_set_value(neethi_operator, env, neethi_policy,
                              OPERATOR_TYPE_POLICY);

    /* This function will do all the processing and build the 
     * policy object model */

    status = neethi_engine_process_operation_element(env, neethi_operator, node, element);

    /* To prevent freeing the policy object from the operator
     * we set it to null. This object will be freed from a parent 
     * or from an outsider who creates a policy object */

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

/* This function will construct the policy objecy model by 
 * filling the component array_list inside the passing 
 * policy operator */


static axis2_status_t neethi_engine_process_operation_element(
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
        /* wsp:Policy element can have any number of attributes
         * we will store them in a hash from the uri and localname */

        axutil_hash_t *attributes = axiom_element_extract_attributes(
            element, env, node);

        if(attributes)
        {
            axutil_hash_index_t *hi = NULL;            
        
            /* When creating the policy object we created the hash */        
    
            axutil_hash_t *ht = neethi_policy_get_attributes(
                (neethi_policy_t *)value, env);

            if(!ht)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[neethi] Policy hash map creation failed.");
                return AXIS2_FAILURE;
            }
            

            for (hi = axutil_hash_first(attributes, env);
                hi; hi = axutil_hash_next(env, hi))
            {
                axis2_char_t *key = NULL;
                void *val = NULL;
                axutil_qname_t *qname = NULL;
                axis2_char_t *attr_val = NULL;    
                axiom_namespace_t *ns = NULL;
                axis2_char_t *ns_uri = NULL;
                axiom_attribute_t *om_attr = NULL;                
    
                axutil_hash_this(hi, NULL, NULL, &val);
                if(val)
                {
                    om_attr = (axiom_attribute_t *) val;
                    ns = axiom_attribute_get_namespace(om_attr, env);
                    if(ns)
                    {
                        ns_uri = axiom_namespace_get_uri(ns, env);
                    }

                    qname = axutil_qname_create(env, 
                        axiom_attribute_get_localname(om_attr, env),
                        ns_uri, NULL);
                    if(qname)
                    {
                        key = axutil_qname_to_string(qname, env);
                        if(key)
                        {
                            attr_val = axiom_attribute_get_value(om_attr, env);
                            if(attr_val)
                            {
                                /* axutil_qname_free will free the returned key 
                                 * of the qname so will duplicate it when storing */                                

                                axutil_hash_set(ht, axutil_strdup(env,key), AXIS2_HASH_KEY_STRING, 
                                    axutil_strdup(env, attr_val));
                            }                                
                        }   
                        axutil_qname_free(qname, env);  
                    }
                }    
            }
            /* axiom_element_extract_attributes method will always returns 
             * a cloned copy, so we need to free it after we have done with it */

            neethi_engine_clear_element_attributes(attributes, env);
            attributes = NULL;
        }
    }

    children_iter = axiom_element_get_children(element, env, node);
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
        {
            /* Extract the element and check the namespace. If the namespace 
             * is in ws_policy then we call the relevent operator builder 
             * otherwise we will call the assertion_builder */


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
                        if ((axutil_strcmp(uri, NEETHI_NAMESPACE) == 0) || 
                            (axutil_strcmp(uri, NEETHI_POLICY_15_NAMESPACE) == 0))
                        {
                            /* Looking at the localname we will call the relevent 
                             * operator function. After that the newly created 
                             * object is wrapped in a neethi_operator and stored in 
                             * the parent's component list */

                            if (axutil_strcmp(local_name, NEETHI_POLICY) == 0)
                            {
                                neethi_policy_t *neethi_policy = NULL;
                                neethi_policy =
                                    neethi_engine_get_operator_neethi_policy(env, child_node,
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
                                    neethi_engine_get_operator_all(env, child_node,
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
                                    neethi_engine_get_operator_exactlyone(env, child_node,
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
                                    neethi_engine_get_operator_reference(env, child_node,
                                                           child_element);
                                if (reference)
                                {
                                    operator = neethi_operator_create(env);
                                    neethi_operator_set_value(operator, env, reference, 
                                        OPERATOR_TYPE_REFERENCE);
                                    neethi_engine_add_policy_component(env, neethi_operator,
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
                            /* This is an assertion in a different domain. Assertion builder
                             * should be called and that will call the relevent assertion builder 
                             * after looking at the localname and the namespace */
                
                            neethi_assertion_t *assertion = NULL;
                            assertion =
                                neethi_assertion_builder_build(env, child_node, child_element);
                            if (assertion)
                            {
                                operator = neethi_operator_create(env);
                                neethi_operator_set_value(operator, env, assertion,
                                                          OPERATOR_TYPE_ASSERTION);
                                neethi_engine_add_policy_component(env, neethi_operator, operator);
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


/* After looking at the operator_type this function will 
 * call the relevent neethi operator's add operator 
 * function */

static axis2_status_t neethi_engine_add_policy_component(
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
/*

Following function will normalize accorading to the 
WS-Policy spec. Normalize policy is in the following 
format.

<wsp:Policy>
    <wsp:ExactlyOne>
        ( <wsp:All> ( <Assertion …> … </Assertion> )* </wsp:All> )*
   </wsp:ExactlyOne>
</wsp:Policy> 

*/


AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_engine_get_normalize(
    const axutil_env_t *env,
    axis2_bool_t deep,
    neethi_policy_t *neethi_policy)
{
    /* In the first call we pass the registry as null.*/

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

    /* Normalize policy will be contained in the new policy
     * created below */

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

    operator = neethi_operator_create(env);
    if (!operator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    neethi_operator_set_value(operator, env, neethi_policy, OPERATOR_TYPE_POLICY);

    /* When we call the normalization it should always return an exactlyone as the 
     * out put. */

    exactlyone = neethi_engine_normalize_operator(operator, registry, deep, env);

    /* We are frreing the operator used to wrap the object */

    neethi_operator_set_value_null(operator, env);
    neethi_operator_free(operator, env);
    operator = NULL;

    /* This exactlyone is set as the first component of the 
     * normalized policy */

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
    exactlyone = neethi_engine_get_cross_product(exactlyone1, exactlyone2, env);
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

static axis2_bool_t neethi_engine_operator_is_empty(
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

static axutil_array_list_t *neethi_engine_operator_get_components(
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

static neethi_exactlyone_t *neethi_engine_normalize_operator(
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

    if (neethi_engine_operator_is_empty(operator, env))
    {
        /* If this is an empty operator we just add 
         * an exactlyone and all */

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
    arraylist = neethi_engine_operator_get_components(operator, env);

    /* Here we are recursively normalize each and every component */

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

                /* We wrap everything inside an exactlyone */

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
                        neethi_engine_normalize_operator(to_normalize, registry, deep, env);
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

            /* If the operator is a policy reference we will 
             * extract the relevent policy from the uri and 
             * normalize as we are doing for a neethi_policy 
             * object */

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

            if(!registry)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Cannot resolve the reference.Registry Not provided");
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
                        neethi_engine_normalize_operator(to_normalize, registry, deep, env);
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
                neethi_engine_normalize_operator(child_component, registry, deep, env);
            if (exactlyone)
            {
                axutil_array_list_add(child_component_list, env, exactlyone);
            }
        }
    }

    /* So at this point we have set of exactlyones in the array_list, So we will 
     * compute one exactlyone out of those after the following call */

    return neethi_engine_compute_resultant_component(child_component_list, type, env);
}

/* This function will return a single exactlyone from all the 
 * components in the list */


static neethi_exactlyone_t *neethi_engine_compute_resultant_component(
    axutil_array_list_t * normalized_inner_components,
    neethi_operator_type_t type,
    const axutil_env_t * env)
{
    neethi_exactlyone_t *exactlyone = NULL;
    int size = 0;

    if(normalized_inner_components)
    {
        size = axutil_array_list_size(normalized_inner_components, env);
    }

    if (type == OPERATOR_TYPE_EXACTLYONE)
    {
        /* If the operator is an exactlyone then we get all the components
         * in the exatlyones and add them to a newly created exactlyone */

        int i = 0;
        neethi_exactlyone_t *inner_exactlyone = NULL;
        exactlyone = neethi_exactlyone_create(env);

        for (i = 0; i < size; i++)
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
        /* Here arry_list contains one exactlyone means this operator 
         * is already normalized. So we will return that. Otherwise we 
         * will get the crossproduct. */

        if (size > 1)
        {
            /* Get the first one and do the cross product with other 
             * components */

            int i = 0;
            exactlyone = (neethi_exactlyone_t *)axutil_array_list_get
                (normalized_inner_components, env, 0);
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
                for (i = 1; i < size; i++)
                {
                    current_exactlyone = (neethi_exactlyone_t *)
                        axutil_array_list_get(normalized_inner_components, env, i);
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
                        exactlyone = neethi_engine_get_cross_product(
                            exactlyone, current_exactlyone, env);
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
            exactlyone = (neethi_exactlyone_t *)axutil_array_list_get(
                normalized_inner_components, env, 0);
        }
    }
    axutil_array_list_free(normalized_inner_components, env);
    normalized_inner_components = NULL;

    return exactlyone;
}

/* The cross product will return all the different combinations 
 * of alternatives and put them into one exactlyone */


static neethi_exactlyone_t *neethi_engine_get_cross_product(
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
        current_all1 = (neethi_all_t *) neethi_operator_get_value(
            (neethi_operator_t *) axutil_array_list_get(array_list1, env, i), env);

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
            current_all2 = (neethi_all_t *) neethi_operator_get_value(
                (neethi_operator_t *) axutil_array_list_get(array_list2, env, j), env);

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

static void neethi_engine_clear_element_attributes(
    axutil_hash_t *attr_hash,
    const axutil_env_t *env)
{
    axutil_hash_index_t *hi = NULL;

    for(hi = axutil_hash_first(attr_hash, env); hi; hi = axutil_hash_next(env, hi))
    {
        void *val = NULL;
        axutil_hash_this(hi, NULL, NULL, &val);
        if (val)
        {
            axiom_attribute_free((axiom_attribute_t *)val, env);
            val = NULL;
        }
    }
    axutil_hash_free(attr_hash, env);
    attr_hash = NULL;

    return;
}
