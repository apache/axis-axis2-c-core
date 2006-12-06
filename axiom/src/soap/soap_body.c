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

/******************* impl struct ****************************************/
typedef struct axiom_soap_body_impl_t
{
    axiom_soap_body_t soap_body;

    axiom_node_t *om_ele_node;

    axis2_bool_t has_fault;

    axiom_soap_fault_t *soap_fault;

    axiom_soap_builder_t *soap_builder;

    int soap_version;

}
axiom_soap_body_impl_t;

/****************** Macro ****************************************************/

#define AXIS2_INTF_TO_IMPL(body) ((axiom_soap_body_impl_t*)body)

/****************** Function prototypes **************************************/

axis2_status_t AXIS2_CALL
axiom_soap_body_free(axiom_soap_body_t *body,
        const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axiom_soap_body_has_fault(axiom_soap_body_t *body,
        const axis2_env_t *env);

axiom_soap_fault_t* AXIS2_CALL
axiom_soap_body_get_fault(axiom_soap_body_t *body,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axiom_soap_body_add_fault(axiom_soap_body_t *body,
        const axis2_env_t *env,
        axiom_soap_fault_t *soap_fault);

axiom_node_t* AXIS2_CALL
axiom_soap_body_get_base_node(axiom_soap_body_t *body,
        const axis2_env_t *env);


axis2_status_t AXIS2_CALL
axiom_soap_body_build(axiom_soap_body_t *body,
        const axis2_env_t *env);



axis2_status_t AXIS2_CALL
axiom_soap_body_add_child(axiom_soap_body_t *body,
        const axis2_env_t *env,
        axiom_node_t *child);

int AXIS2_CALL
axiom_soap_body_get_soap_version(axiom_soap_body_t *body,
        const axis2_env_t *env);



/*************** function implementations *************************************/
axiom_soap_body_t* AXIS2_CALL
axiom_soap_body_create(const axis2_env_t *env)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    body_impl = (axiom_soap_body_impl_t*)AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_soap_body_impl_t));

    if (!body_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    body_impl->soap_body.ops = NULL;
    body_impl->om_ele_node = NULL;
    body_impl->soap_builder = NULL;
    body_impl->has_fault = AXIS2_FALSE;
    body_impl->soap_fault = NULL;

    body_impl->soap_body.ops = (axiom_soap_body_ops_t*) AXIS2_MALLOC(
                env->allocator, sizeof(axiom_soap_body_ops_t));

    if (!(body_impl->soap_body.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, body_impl);
        body_impl = NULL;
        return NULL;
    }

    body_impl->soap_body.ops->free =
        axiom_soap_body_free;

    body_impl->soap_body.ops->has_fault =
        axiom_soap_body_has_fault;

    body_impl->soap_body.ops->get_fault =
        axiom_soap_body_get_fault;

    body_impl->soap_body.ops->get_base_node =
        axiom_soap_body_get_base_node;

    body_impl->soap_body.ops->build =
        axiom_soap_body_build;

    body_impl->soap_body.ops->add_child =
        axiom_soap_body_add_child;

    body_impl->soap_body.ops->get_soap_version =
        axiom_soap_body_get_soap_version;

    return &(body_impl->soap_body);

    return NULL;
}



axiom_soap_body_t* AXIS2_CALL
axiom_soap_body_create_with_parent(const axis2_env_t *env,
        axiom_soap_envelope_t *envelope)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    axiom_soap_body_t *body = NULL;
    axiom_element_t *ele = NULL;
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele =  NULL;
    axiom_namespace_t *om_ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, envelope, NULL);

    body = axiom_soap_body_create(env);
    if (!body)
    {
        return NULL;
    }
    body_impl = AXIS2_INTF_TO_IMPL(body);


    /*get parent node from SOAP envelope */
    parent_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env);
    if (!parent_node)
    {
        AXIOM_SOAP_BODY_FREE(body, env);
        return NULL;
    }
    parent_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                parent_node, env);

    if (!parent_ele)
    {
        AXIOM_SOAP_BODY_FREE(body, env);
        return NULL;
    }

    om_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);

    ele = axiom_element_create(env, parent_node,
            AXIOM_SOAP_BODY_LOCAL_NAME, om_ns,
            &(body_impl->om_ele_node));
    if (!ele)
    {
        axiom_soap_body_free(body, env);
        return NULL;
    }

    axiom_soap_envelope_set_body(envelope, env, &(body_impl->soap_body));

    return &(body_impl->soap_body);
}

axis2_status_t AXIS2_CALL
axiom_soap_body_free(axiom_soap_body_t *body,
        const axis2_env_t *env)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, body, AXIS2_FAILURE);

    body_impl = AXIS2_INTF_TO_IMPL(body);

    if (body_impl->soap_fault)
    {
        AXIOM_SOAP_FAULT_FREE(body_impl->soap_fault, env);
        body_impl->soap_fault = NULL;
    }
    if (body->ops)
    {
        AXIS2_FREE(env->allocator, body->ops);
        body->ops = NULL;
    }
    AXIS2_FREE(env->allocator, body_impl);
    body_impl = NULL;
    return AXIS2_SUCCESS;
}

/**
 * Indicates whether a object exists in
 * this SOAPBody object.
 *
 * @return true</code> if a <code>SOAPFault</code> object exists in
 *         this <code>SOAPBody</code> object; <code>false</code>
 *         otherwise
 */
axis2_bool_t AXIS2_CALL axiom_soap_body_has_fault(axiom_soap_body_t *body,
        const axis2_env_t *env)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (body_impl->soap_fault)
    {
        body_impl->has_fault = AXIS2_TRUE;
        return AXIS2_TRUE;
    }
    else
    {
        if (body_impl->soap_builder)
        {
            while (!(body_impl->soap_fault) && !(AXIOM_NODE_IS_COMPLETE(body_impl->om_ele_node, env)))
            {
                status = AXIOM_SOAP_BUILDER_NEXT(body_impl->soap_builder, env);
                if (status == AXIS2_FAILURE)
                    return AXIS2_FALSE;
            }
            if (body_impl->soap_fault)
            {
                body_impl->has_fault = AXIS2_TRUE;
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
axiom_soap_fault_t* AXIS2_CALL
axiom_soap_body_get_fault(axiom_soap_body_t *body,
        const axis2_env_t *env)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (body_impl->soap_fault)
    {
        return body_impl->soap_fault;
    }
    else if (body_impl->soap_builder)
    {
        while (!(body_impl->soap_fault) && !(AXIOM_NODE_IS_COMPLETE(body_impl->om_ele_node, env)))
        {
            int status = AXIS2_SUCCESS;
            status = AXIOM_SOAP_BUILDER_NEXT(body_impl->soap_builder, env);
            if (status == AXIS2_FAILURE)
                return NULL;
        }
        if (body_impl->soap_fault)
        {
            body_impl->has_fault = AXIS2_TRUE;
            return body_impl->soap_fault;
        }
    }
    return NULL;
}

/**
 * @param soapFault
 *
axis2_status_t AXIS2_CALL
axiom_soap_body_add_fault(axiom_soap_body_t *body,
                          const axis2_env_t *env,
                          axiom_soap_fault_t *soap_fault)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    axiom_node_t *fault_node = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_fault, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);

    if (body_impl->has_fault)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    fault_node = AXIOM_SOAP_FAULT_GET_BASE_NODE(soap_fault, env);
    if(fault_node)
    {
        body_impl->soap_fault = soap_fault;
        AXIOM_NODE_ADD_CHILD(fault_node , env, (body_impl->om_ele_node));
        body_impl->has_fault = AXIS2_TRUE;
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
*/

axiom_node_t* AXIS2_CALL
axiom_soap_body_get_base_node(axiom_soap_body_t *body,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(body)->om_ele_node;
}

axis2_status_t AXIS2_CALL
axiom_soap_body_set_base_node(axiom_soap_body_t *body,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    body_impl->om_ele_node = node;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axiom_soap_body_set_builder(axiom_soap_body_t *body,
        const axis2_env_t *env,
        axiom_soap_builder_t *builder)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    body_impl->soap_builder = builder;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axiom_soap_body_build(axiom_soap_body_t *body,
        const axis2_env_t *env)
{
    axiom_soap_body_impl_t *soap_body_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_body_impl = AXIS2_INTF_TO_IMPL(body);
    if (soap_body_impl->om_ele_node && soap_body_impl->soap_builder)
    {
        while (AXIOM_NODE_IS_COMPLETE(soap_body_impl->om_ele_node, env) != AXIS2_TRUE)
        {
            status = AXIOM_SOAP_BUILDER_NEXT(soap_body_impl->soap_builder, env);
            if (status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

/**
    This is an internal function

*/
axis2_status_t AXIS2_CALL
axiom_soap_body_set_fault(axiom_soap_body_t *body,
        const axis2_env_t *env,
        axiom_soap_fault_t *soap_fault)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, soap_fault, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (body_impl->soap_fault)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_ONLY_ONE_SOAP_FAULT_ALLOWED_IN_BODY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        body_impl->soap_fault = soap_fault;
        body_impl->has_fault = AXIS2_TRUE;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_soap_body_add_child(axiom_soap_body_t *body,
        const axis2_env_t *env,
        axiom_node_t *child)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, child, AXIS2_FAILURE);

    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (body_impl->om_ele_node)
    {
        return AXIOM_NODE_ADD_CHILD(body_impl->om_ele_node, env, child);
    }
    return AXIS2_FAILURE;
}
int AXIS2_CALL
axiom_soap_body_get_soap_version(axiom_soap_body_t *body,
        const axis2_env_t *env)
{
    axiom_soap_body_impl_t *body_impl = NULL;
    axiom_element_t *body_ele = NULL;
    axiom_namespace_t *om_ns = NULL;
    axis2_char_t *uri           = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    body_impl = AXIS2_INTF_TO_IMPL(body);
    if (!body_impl->om_ele_node)
        return AXIS2_FAILURE;
    body_ele = AXIOM_NODE_GET_DATA_ELEMENT(body_impl->om_ele_node, env);
    if (!body_ele)
        return AXIS2_FAILURE;
    om_ns = AXIOM_ELEMENT_GET_NAMESPACE(body_ele, env, body_impl->om_ele_node);
    if (!om_ns)
        return AXIS2_FAILURE;
    uri = AXIOM_NAMESPACE_GET_URI(om_ns, env);
    if (uri)
    {
        if (AXIS2_STRCMP(uri, AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
            return AXIOM_SOAP11;
        else if (AXIS2_STRCMP(uri, AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
            return AXIOM_SOAP12;
    }
    return AXIS2_FAILURE;
}
