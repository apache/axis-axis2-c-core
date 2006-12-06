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

#include <axis2_wsdl_ext_soap_op.h>
#include <axis2_wsdl.h>

/**
 * @brief Wsdl Extension Soap Operation struct impl
 *    Wsdl Extention Soap Operation
 */
typedef struct axis2_wsdl_ext_soap_op_impl
{
    axis2_wsdl_ext_soap_op_t ext_soap_op;
    axis2_char_t *soap_action;
    axis2_char_t *style;

}
axis2_wsdl_ext_soap_op_impl_t;

#define AXIS2_INTF_TO_IMPL(ext_soap_op) \
      ((axis2_wsdl_ext_soap_op_impl_t *)ext_soap_op)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_op_free(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_op_get_soap_action(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_op_set_soap_action(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env,
        axis2_char_t *soap_action);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_op_get_style(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_op_set_style(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env,
        axis2_char_t *style);

/************************** End of function prototypes ************************/

axis2_wsdl_ext_soap_op_t * AXIS2_CALL
axis2_wsdl_ext_soap_op_create(const axis2_env_t *env,
        axis2_qname_t *qtype)
{
    axis2_wsdl_ext_soap_op_impl_t *ext_soap_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    ext_soap_op_impl = (axis2_wsdl_ext_soap_op_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_wsdl_ext_soap_op_impl_t));


    if (NULL == ext_soap_op_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ext_soap_op_impl->soap_action = NULL;
    ext_soap_op_impl->style = NULL;
    ext_soap_op_impl->ext_soap_op.ops = NULL;
    ext_soap_op_impl->ext_soap_op.ext_element = NULL;

    ext_soap_op_impl->ext_soap_op.ext_element =
        axis2_wsdl_extensible_element_create(env);
    if (NULL == ext_soap_op_impl->ext_soap_op.ext_element)
    {
        axis2_wsdl_ext_soap_op_free(&(ext_soap_op_impl->ext_soap_op), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ext_soap_op_impl->ext_soap_op.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_ext_soap_op_ops_t));
    if (NULL == ext_soap_op_impl->ext_soap_op.ops)
    {
        axis2_wsdl_ext_soap_op_free(&(ext_soap_op_impl->ext_soap_op), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (!qtype)
    {
        qtype = axis2_qname_create(env, "op", AXIOM_SOAP_11_OP, NULL);
        if (!qtype) return NULL;
    }
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(
        (&(ext_soap_op_impl->ext_soap_op))->ext_element, env, qtype);

    ext_soap_op_impl->ext_soap_op.ops->free =  axis2_wsdl_ext_soap_op_free;
    ext_soap_op_impl->ext_soap_op.ops->get_soap_action =
        axis2_wsdl_ext_soap_op_get_soap_action;
    ext_soap_op_impl->ext_soap_op.ops->set_soap_action =
        axis2_wsdl_ext_soap_op_set_soap_action;
    ext_soap_op_impl->ext_soap_op.ops->get_style =
        axis2_wsdl_ext_soap_op_get_style;
    ext_soap_op_impl->ext_soap_op.ops->set_style =
        axis2_wsdl_ext_soap_op_set_style;

    return &(ext_soap_op_impl->ext_soap_op);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_op_free(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env)
{
    axis2_wsdl_ext_soap_op_impl_t *exp_soap_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    exp_soap_op_impl = AXIS2_INTF_TO_IMPL(ext_soap_op);

    if (ext_soap_op->ops)
        AXIS2_FREE(env->allocator, ext_soap_op->ops);

    if (exp_soap_op_impl->soap_action)
    {
        AXIS2_FREE(env->allocator, exp_soap_op_impl->soap_action);
        exp_soap_op_impl->soap_action = NULL;
    }

    if (exp_soap_op_impl->style)
    {
        AXIS2_FREE(env->allocator, exp_soap_op_impl->style);
        exp_soap_op_impl->style = NULL;
    }

    if (ext_soap_op->ext_element)
    {
        AXIS2_WSDL_EXTENSIBLE_ELEMENT_FREE(ext_soap_op->ext_element, env);
        ext_soap_op->ext_element = NULL;
    }

    AXIS2_FREE(env->allocator, exp_soap_op_impl);
    exp_soap_op_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_op_get_soap_action(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(ext_soap_op)->soap_action;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_op_set_soap_action(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env,
        axis2_char_t *soap_action)
{
    axis2_wsdl_ext_soap_op_impl_t *exp_soap_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_action, AXIS2_FAILURE);

    exp_soap_op_impl = AXIS2_INTF_TO_IMPL(ext_soap_op);

    if (exp_soap_op_impl->soap_action)
    {
        AXIS2_FREE(env->allocator, exp_soap_op_impl->soap_action);
        exp_soap_op_impl->soap_action = NULL;
    }
    exp_soap_op_impl->soap_action = AXIS2_STRDUP(soap_action, env);
    if (!exp_soap_op_impl->soap_action)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_op_get_style(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(ext_soap_op)->style;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_op_set_style(axis2_wsdl_ext_soap_op_t *ext_soap_op,
        const axis2_env_t *env,
        axis2_char_t *style)
{
    axis2_wsdl_ext_soap_op_impl_t *exp_soap_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, style, AXIS2_FAILURE);
    exp_soap_op_impl = AXIS2_INTF_TO_IMPL(ext_soap_op);

    if (exp_soap_op_impl->style)
    {
        AXIS2_FREE(env->allocator, exp_soap_op_impl->style);
        exp_soap_op_impl->style = NULL;
    }
    exp_soap_op_impl->style = AXIS2_STRDUP(style, env);
    if (!exp_soap_op_impl->style)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

