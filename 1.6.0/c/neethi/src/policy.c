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

#include <neethi_policy.h>
#include <neethi_engine.h>
#include <axiom_attribute.h>


struct neethi_policy_t
{
    /* Contains the child components */
    axutil_array_list_t *policy_components;

    /* A wsp:Policy element can have any number of attributes
     * This has will store those */
    axutil_hash_t *attributes;

    /* This is the node containing the policy */
    axiom_node_t *root_node;
};


static void neethi_policy_clear_attributes(
    axutil_hash_t *attributes,
    const axutil_env_t *env);


/* Creates a neethi_policy object */

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
neethi_policy_create(
    const axutil_env_t * env)
{
    neethi_policy_t *neethi_policy = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    neethi_policy = (neethi_policy_t *) AXIS2_MALLOC(env->allocator,
                                                     sizeof(neethi_policy_t));

    if (neethi_policy == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_policy->policy_components = NULL;

    neethi_policy->policy_components = axutil_array_list_create(env, 0);
    if (!(neethi_policy->policy_components))
    {
        neethi_policy_free(neethi_policy, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_policy->attributes = axutil_hash_make(env);
    neethi_policy->root_node = NULL;

    return neethi_policy;
}

/* Deallocate the memory for the neethi_policy object */


AXIS2_EXTERN void AXIS2_CALL
neethi_policy_free(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    if (neethi_policy)
    {
        /* We first remove the component list */

        if (neethi_policy->policy_components)
        {
            int i = 0;
            int size = 0;

            size = axutil_array_list_size(
                neethi_policy->policy_components, env);

            for (i = 0; i < size; i++)
            {
                neethi_operator_t *operator = NULL;
                operator =(neethi_operator_t *)axutil_array_list_get(
                    neethi_policy->policy_components, env, i);
                if (operator)
                {    
                    neethi_operator_free(operator, env);
                }
                operator = NULL;
            }
            axutil_array_list_free(neethi_policy->policy_components, env);
            neethi_policy->policy_components = NULL;
        }
        if(neethi_policy->attributes)
        {
            neethi_policy_clear_attributes(neethi_policy->attributes, 
                env);
            neethi_policy->attributes = NULL;
        }
        if (neethi_policy->root_node)
        {
            axiom_node_free_tree(neethi_policy->root_node, env);
            neethi_policy->root_node = NULL;
        }
        AXIS2_FREE(env->allocator, neethi_policy);
        neethi_policy = NULL;
    }
    return;
}

/* This will free the attribute hash and its content.*/

static void neethi_policy_clear_attributes(
    axutil_hash_t *attributes,
    const axutil_env_t *env)
{
    if(attributes)
    {
        axutil_hash_index_t *hi = NULL;
        void *val = NULL;
        const void *key = NULL;

        for (hi = axutil_hash_first(attributes, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, &key, NULL, &val);

            if(key)
            {
                AXIS2_FREE(env->allocator, (axis2_char_t *)key);
                key = NULL;
            }

            if (val)
            {
                AXIS2_FREE(env->allocator, (axis2_char_t *)val);
                val = NULL;
            }
        }
        axutil_hash_free(attributes, env);
        attributes = NULL;
    }
}
/* Implementations */

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
neethi_policy_get_policy_components(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    return neethi_policy->policy_components;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_policy_add_policy_components(
    neethi_policy_t *neethi_policy,
    axutil_array_list_t *arraylist,
    const axutil_env_t *env)
{
    int size = axutil_array_list_size(arraylist, env);
    int i = 0;

    if (axutil_array_list_ensure_capacity
        (neethi_policy->policy_components, env, size + 1) != AXIS2_SUCCESS)
    {    
        return AXIS2_FAILURE;
    }

    for (i = 0; i < size; i++)
    {
        void *value = NULL;
        value = axutil_array_list_get(arraylist, env, i);
        /* The ref count is incremented in order to prvent double frees.*/
        neethi_operator_increment_ref((neethi_operator_t *) value, env);
        axutil_array_list_add(neethi_policy->policy_components, env, value);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_policy_add_operator(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env,
    neethi_operator_t *operator)
{
    neethi_operator_increment_ref(operator, env);
    axutil_array_list_add(neethi_policy->policy_components, env, operator);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
neethi_policy_is_empty(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    return axutil_array_list_is_empty(neethi_policy->policy_components, env);
}

/* A normalized policy always has just one child and it is an exactlyone
 * first child. So this method */

AXIS2_EXTERN neethi_exactlyone_t *AXIS2_CALL
neethi_policy_get_exactlyone(
    neethi_policy_t *normalized_neethi_policy,
    const axutil_env_t *env)
{
    neethi_exactlyone_t *exactlyone = NULL;
    axutil_array_list_t *list = NULL;

    list = neethi_policy_get_policy_components(normalized_neethi_policy, env);
    if (list)
    {
        if (axutil_array_list_size(list, env) == 1)
        {
            neethi_operator_t *op = NULL;
            op = (neethi_operator_t *) axutil_array_list_get(list, env, 0);
            if (!op)
            {
                return NULL;
            }
            if (neethi_operator_get_type(op, env) != OPERATOR_TYPE_EXACTLYONE)
            {
                return NULL;
            }
            exactlyone =
                (neethi_exactlyone_t *) neethi_operator_get_value(op, env);
            return exactlyone;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

/* This function is called for a normalized policy
 * So it will return the components of the only
 * child. That should be an exactlyone The children
 * of that exactlyone are alls.*/

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
neethi_policy_get_alternatives(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    neethi_exactlyone_t *exactlyone = NULL;
    exactlyone = neethi_policy_get_exactlyone(neethi_policy, env);
    if (!exactlyone)
        return NULL;

    return neethi_exactlyone_get_policy_components(exactlyone, env);

}

/* This will return any attribute which has 
 * the local name of Name */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
neethi_policy_get_name(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    if(neethi_policy->attributes)
    {
        axutil_qname_t *qname = NULL;
        axis2_char_t *name = NULL;
        qname = axutil_qname_create(env, NEETHI_NAME, NULL, NULL);

        if(qname)
        {
            axis2_char_t *key = axutil_qname_to_string(qname, env);
            if(key)
            {
                name = (axis2_char_t *)axutil_hash_get(neethi_policy->attributes, key, 
                    AXIS2_HASH_KEY_STRING);
            }
            axutil_qname_free(qname, env);
            qname = NULL;
            return name;
        }        
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}


/* This method will return the attribute value of 
 * wsu:Id if there are any such attributes */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
neethi_policy_get_id(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    if(neethi_policy->attributes)
    {
        axis2_char_t *id = NULL;
        axutil_qname_t *qname = NULL;
        qname = axutil_qname_create(env, NEETHI_ID, NEETHI_WSU_NS, NULL);

        if(qname)
        {
            axis2_char_t *key = axutil_qname_to_string(qname, env);
            if(key)
            {
                id =  (axis2_char_t *)axutil_hash_get(neethi_policy->attributes, key, 
                    AXIS2_HASH_KEY_STRING);
            }
            axutil_qname_free(qname, env);
            qname = NULL;
            return id;
        }        
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}


/* When we encounter an attribute with wsu:Id 
 * we will store it in the hash. We are not 
 * considering the prefix. Just the namespace and
 * the local_name. */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
    neethi_policy_set_id(
    neethi_policy_t * neethi_policy,
    const axutil_env_t * env,
    axis2_char_t * id)
{
    axutil_qname_t *qname = NULL;
    axis2_char_t *key = NULL;

    qname = axutil_qname_create(env, NEETHI_ID, NEETHI_WSU_NS, NULL);

    if(qname)
    {
        key = axutil_qname_to_string(qname, env);
        if(key)
        {
            axutil_hash_set(neethi_policy->attributes, axutil_strdup(env, key), 
                AXIS2_HASH_KEY_STRING, axutil_strdup(env, id));
        }
        axutil_qname_free(qname, env);
        return AXIS2_SUCCESS;
    }
    else
    {
        return AXIS2_FAILURE;
    }
}

/* When we encounter an attribute with Name 
 * we will store it in the hash. This has no
 * Namespace.*/


AXIS2_EXTERN axis2_status_t AXIS2_CALL
    neethi_policy_set_name(
    neethi_policy_t * neethi_policy,
    const axutil_env_t * env,
    axis2_char_t * name)
{
    axutil_qname_t *qname = NULL;
    axis2_char_t *key = NULL;

    qname = axutil_qname_create(env, NEETHI_NAME, NULL, NULL);

    if(qname)
    {
        key = axutil_qname_to_string(qname, env);
        if(key)
        {
            axutil_hash_set(neethi_policy->attributes, axutil_strdup(env, key), 
                AXIS2_HASH_KEY_STRING, axutil_strdup(env, name));
        }
        axutil_qname_free(qname, env);
        return AXIS2_SUCCESS;
    }
    else
    {
        return AXIS2_FAILURE;
    }
}


AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
neethi_policy_get_attributes(
    neethi_policy_t *neethi_policy,
    const axutil_env_t *env)
{
    return neethi_policy->attributes;
}

/*This function is for serializing*/
AXIS2_EXTERN axiom_node_t *AXIS2_CALL
neethi_policy_serialize(
    neethi_policy_t *neethi_policy,
    axiom_node_t *parent,
    const axutil_env_t *env)
{

    axiom_node_t *policy_node = NULL;
    axiom_element_t *policy_ele = NULL;
    axiom_namespace_t *policy_ns = NULL;
    axutil_array_list_t *components = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    policy_ns = axiom_namespace_create(env, NEETHI_NAMESPACE, NEETHI_PREFIX);

    policy_ele =
        axiom_element_create(env, parent, NEETHI_POLICY, policy_ns,
                             &policy_node);
    if (!policy_ele)
    {
        return NULL;
    }

    components = neethi_policy_get_policy_components(neethi_policy, env);

    if (components)
    {
        int i = 0;
        for (i = 0; i < axutil_array_list_size(components, env); i++)
        {
            neethi_operator_t *operator = NULL;
            operator =(neethi_operator_t *) axutil_array_list_get(components,
                                                                  env, i);
            if (operator)
            {
                status = neethi_operator_serialize(operator, env, policy_node);
                if (status != AXIS2_SUCCESS)
                {
                    return NULL;
                }
            }
        }
    }
    return policy_node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_policy_set_root_node(
    neethi_policy_t *policy,
    const axutil_env_t *env,
    axiom_node_t *root_node)
{
    policy->root_node = root_node;
    return AXIS2_SUCCESS;
}
