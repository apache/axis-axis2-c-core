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

#include <axis2_wsdl_msg_ref.h>

/**
 * @brief Wsdl Binding Message Reference struct impl
 * Referes to the MEP the Message relates to.
 */
typedef struct axis2_wsdl_msg_ref_impl
{
    axis2_wsdl_msg_ref_t msg_ref;
    /**
     * Field messageLabel
     */
    axis2_char_t *msg_label;
    /**
     * Field Direction
     * Can be "in" or "out" depending on the element name being "input" or
     * "output" respectively;
     */
    axis2_char_t *direction;
    /**
     * Field element
     */
    axis2_qname_t *element;

}
axis2_wsdl_msg_ref_impl_t;

#define AXIS2_INTF_TO_IMPL(msg_ref) \
      ((axis2_wsdl_msg_ref_impl_t *)msg_ref)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_msg_ref_free(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env);

axis2_char_t * AXIS2_CALL
axis2_wsdl_msg_ref_get_direction(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_msg_ref_set_direction(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env,
        const axis2_char_t *direction);

axis2_char_t * AXIS2_CALL
axis2_wsdl_msg_ref_get_msg_label(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_msg_ref_set_msg_label(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env,
        const axis2_char_t *msg_label);

axis2_qname_t * AXIS2_CALL
axis2_wsdl_msg_ref_get_element(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wdsl_msg_ref_set_element(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env,
        axis2_qname_t *element);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_wsdl_msg_ref_t * AXIS2_CALL
axis2_wsdl_msg_ref_create(const axis2_env_t *env)
{
    axis2_wsdl_msg_ref_impl_t *msg_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    msg_ref_impl = (axis2_wsdl_msg_ref_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_wsdl_msg_ref_impl_t));

    if (NULL == msg_ref_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    msg_ref_impl->msg_label = NULL;
    msg_ref_impl->direction = NULL;
    msg_ref_impl->element = NULL;

    msg_ref_impl->msg_ref.extensible_component =
        axis2_wsdl_extensible_component_create(env);
    if (NULL == msg_ref_impl->msg_ref.extensible_component)
    {
        AXIS2_WSDL_MSG_REF_FREE(&(msg_ref_impl->msg_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    msg_ref_impl->msg_ref.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_msg_ref_ops_t));
    if (NULL == msg_ref_impl->msg_ref.ops)
    {
        AXIS2_WSDL_MSG_REF_FREE(&(msg_ref_impl->msg_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    msg_ref_impl->msg_ref.ops->free =  axis2_wsdl_msg_ref_free;
    msg_ref_impl->msg_ref.ops->get_direction =
        axis2_wsdl_msg_ref_get_direction;
    msg_ref_impl->msg_ref.ops->set_direction =
        axis2_wsdl_msg_ref_set_direction;
    msg_ref_impl->msg_ref.ops->get_msg_label =
        axis2_wsdl_msg_ref_get_msg_label;
    msg_ref_impl->msg_ref.ops->set_msg_label =
        axis2_wsdl_msg_ref_set_msg_label;
    msg_ref_impl->msg_ref.ops->get_element =  axis2_wsdl_msg_ref_get_element;
    msg_ref_impl->msg_ref.ops->set_element =  axis2_wdsl_msg_ref_set_element;
    return &(msg_ref_impl->msg_ref);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_msg_ref_free(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env)
{
    axis2_wsdl_msg_ref_impl_t *msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_ref_impl = AXIS2_INTF_TO_IMPL(msg_ref);

    if (msg_ref->ops)
    {
        AXIS2_FREE(env->allocator, msg_ref->ops);
    }
    if (msg_ref_impl->msg_label)
    {
        AXIS2_FREE(env->allocator, msg_ref_impl->msg_label);
    }
    if (msg_ref_impl->msg_label)
    {
        AXIS2_FREE(env->allocator, msg_ref_impl->msg_label);
    }
    if (msg_ref_impl->element)
    {
        AXIS2_QNAME_FREE(msg_ref_impl->element, env);
    }
    if (msg_ref->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(msg_ref->extensible_component, env);
    }
    if (msg_ref_impl)
    {
        AXIS2_FREE(env->allocator, msg_ref_impl);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_msg_ref_get_direction(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ref)->direction;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_msg_ref_set_direction(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env,
        const axis2_char_t *direction)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, direction, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(msg_ref)->direction = (axis2_char_t *)direction;
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_msg_ref_get_msg_label(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ref)->msg_label;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_msg_ref_set_msg_label(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env,
        const axis2_char_t *msg_label)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(msg_ref)->msg_label = (axis2_char_t *)msg_label;
    return AXIS2_SUCCESS;
}

axis2_qname_t * AXIS2_CALL
axis2_wsdl_msg_ref_get_element(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ref)->element;
}

axis2_status_t AXIS2_CALL
axis2_wdsl_msg_ref_set_element(axis2_wsdl_msg_ref_t *msg_ref,
        const axis2_env_t *env,
        axis2_qname_t *element)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ref)->element = AXIS2_QNAME_CLONE(element, env);
    return AXIS2_SUCCESS;
}
