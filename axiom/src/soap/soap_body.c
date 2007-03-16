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

#include "_axiom_soap_envelope.h"
#include "_axiom_soap_body.h"
#include <axis2_hash.h>
#include <axiom_soap_const.h>
#include <axiom_soap_builder.h>

struct axiom_soap_body
{
    axiom_node_t *om_ele_node;

    axis2_bool_t has_fault;

    axiom_soap_fault_t *soap_fault;

    axiom_soap_builder_t *soap_builder;

    int soap_version;
};

AXIS2_EXTERN axiom_soap_body_t* AXIS2_CALL
axiom_soap_body_create(const axis2_env_t *env)
{
    axiom_soap_body_t *soap_body = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    soap_body = (axiom_soap_body_t*)AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_soap_body_t));

    if (!soap_body)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    soap_body->om_ele_node = NULL;
    soap_body->soap_builder = NULL;
    soap_body->has_fault = AXIS2_FALSE;
    soap_body->soap_fault = NULL;

    return soap_body;
}



AXIS2_EXTERN axiom_soap_body_t* AXIS2_CALL
axiom_soap_body_create_with_parent(const axis2_env_t *env,
        axiom_soap_envelope_t *envelope)
{
    axiom_soap_body_t *soap_body = NULL;
    axiom_element_t *ele = NULL;
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele =  NULL;
    axiom_namespace_t *om_ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, envelope, NULL);

    soap_body = axiom_soap_body_create(env);
    if (!soap_body)
    {
        return NULL;
    }

    /*get parent node from SOAP envelope */
    parent_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env);
    if (!parent_node)
    {
        AXIOM_SOAP_BODY_FREE(soap_body, env);
        return NULL;
    }
    parent_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                parent_node, env);

    if (!parent_ele)
    {
        AXIOM_SOAP_BODY_FREE(soap_body, env);
        return NULL;
    }

    om_ns = axiom_element_get_namespace(parent_ele, env, parent_node);

    ele = axiom_element_create(env, parent_node,
            AXIOM_SOAP_BODY_LOCAL_NAME, om_ns,
            &(soap_body->om_ele_node));
    if (!ele)
    {
        axiom_soap_body_free(soap_body, env);
        return NULL;
    }

    axiom_soap_envelope_set_body(envelope, env, soap_body);

    return soap_body;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_body_free(axiom_soap_body_t *soap_body,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_body, AXIS2_FAILURE);

    if (soap_body->soap_fault)
    {
        AXIOM_SOAP_FAULT_FREE(soap_body->soap_fault, env);
        soap_body->soap_fault = NULL;
    }
    AXIS2_FREE(env->allocator, soap_body);
    soap_body = NULL;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL axiom_soap_body_has_fault(axiom_soap_body_t *soap_body,
        const axis2_env_t *env)
{
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (soap_body->soap_fault)
    {
        soap_body->has_fault = AXIS2_TRUE;
        return AXIS2_TRUE;
    }
    else
    {
        if (soap_body->soap_builder)
        {
            while (!(soap_body->soap_fault) && !(AXIOM_NODE_IS_COMPLETE(soap_body->om_ele_node, env)))
            {
                status = AXIOM_SOAP_BUILDER_NEXT(soap_body->soap_builder, env);
                if (status == AXIS2_FAILURE)
                    return AXIS2_FALSE;
            }
            if (soap_body->soap_fault)
            {
                soap_body->has_fault = AXIS2_TRUE;
                return AXIS2_TRUE;
            }
        }
    }

    return AXIS2_FALSE;
}

/**
 * Returns the axiom_soap_fault_t struct in this axiom_soap_bodY_t
 * struct
 *
 * @return the <code>SOAPFault</code> object in this <code>SOAPBody</code>
 *         object
 */
AXIS2_EXTERN axiom_soap_fault_t* AXIS2_CALL
axiom_soap_body_get_fault(axiom_soap_body_t *soap_body,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (soap_body->soap_fault)
    {
        return soap_body->soap_fault;
    }
    else if (soap_body->soap_builder)
    {
        while (!(soap_body->soap_fault) && !(AXIOM_NODE_IS_COMPLETE(soap_body->om_ele_node, env)))
        {
            int status = AXIS2_SUCCESS;
            status = AXIOM_SOAP_BUILDER_NEXT(soap_body->soap_builder, env);
            if (status == AXIS2_FAILURE)
                return NULL;
        }
        if (soap_body->soap_fault)
        {
            soap_body->has_fault = AXIS2_TRUE;
            return soap_body->soap_fault;
        }
    }
    return NULL;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_soap_body_get_base_node(axiom_soap_body_t *soap_body,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return soap_body->om_ele_node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_body_set_base_node(axiom_soap_body_t *soap_body,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    if (AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    soap_body->om_ele_node = node;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_body_set_builder(axiom_soap_body_t *soap_body,
        const axis2_env_t *env,
        axiom_soap_builder_t *builder)
{
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);
    soap_body->soap_builder = builder;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_body_build(axiom_soap_body_t *soap_body,
        const axis2_env_t *env)
{
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (soap_body->om_ele_node && soap_body->soap_builder)
    {
        while (AXIOM_NODE_IS_COMPLETE(soap_body->om_ele_node, env) != AXIS2_TRUE)
        {
            status = AXIOM_SOAP_BUILDER_NEXT(soap_body->soap_builder, env);
            if (status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

/**
    This is an internal function

*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_body_set_fault(axiom_soap_body_t *soap_body,
        const axis2_env_t *env,
        axiom_soap_fault_t *soap_fault)
{
    AXIS2_PARAM_CHECK(env->error, soap_fault, AXIS2_FAILURE);
    if (soap_body->soap_fault)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        soap_body->soap_fault = soap_fault;
        soap_body->has_fault = AXIS2_TRUE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_body_add_child(axiom_soap_body_t *soap_body,
        const axis2_env_t *env,
        axiom_node_t *child)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, child, AXIS2_FAILURE);

    if (soap_body->om_ele_node)
    {
        return AXIOM_NODE_ADD_CHILD(soap_body->om_ele_node, env, child);
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN int AXIS2_CALL
axiom_soap_body_get_soap_version(axiom_soap_body_t *soap_body,
        const axis2_env_t *env)
{
    axiom_element_t *body_ele = NULL;
    axiom_namespace_t *om_ns = NULL;
    axis2_char_t *uri           = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!soap_body->om_ele_node)
        return AXIS2_FAILURE;
    body_ele = AXIOM_NODE_GET_DATA_ELEMENT(soap_body->om_ele_node, env);
    if (!body_ele)
        return AXIS2_FAILURE;
    om_ns = axiom_element_get_namespace(body_ele, env, soap_body->om_ele_node);
    if (!om_ns)
        return AXIS2_FAILURE;
    uri = axiom_namespace_get_uri(om_ns, env);
    if (uri)
    {
        if (AXIS2_STRCMP(uri, AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
            return AXIOM_SOAP11;
        else if (AXIS2_STRCMP(uri, AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
            return AXIOM_SOAP12;
    }
    return AXIS2_FAILURE;
}
