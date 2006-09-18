/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_wsdl_ext_soap_binding.h>
#include <axis2_wsdl.h>

/**
 * @brief Wsdl Extension Soap Binding struct impl
 *    Wsdl Extention Soap Binding
 */
typedef struct axis2_wsdl_ext_soap_binding_impl
{
    axis2_wsdl_ext_soap_binding_t ext_soap_binding;
    axis2_char_t *style;
    axis2_char_t *transport_uri;

}
axis2_wsdl_ext_soap_binding_impl_t;

#define AXIS2_INTF_TO_IMPL(ext_soap_binding) \
      ((axis2_wsdl_ext_soap_binding_impl_t *)ext_soap_binding)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_binding_free(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_binding_get_style(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_binding_set_style(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env,
        axis2_char_t *style);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_binding_get_transport_uri(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_binding_set_transport_uri(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env,
        axis2_char_t *transport_uri);

/************************** End of function prototypes ************************/

axis2_wsdl_ext_soap_binding_t * AXIS2_CALL
axis2_wsdl_ext_soap_binding_create(const axis2_env_t *env,
        axis2_qname_t *qtype)
{
    axis2_wsdl_ext_soap_binding_impl_t *ext_soap_binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    ext_soap_binding_impl = (axis2_wsdl_ext_soap_binding_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_wsdl_ext_soap_binding_impl_t));


    if (NULL == ext_soap_binding_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ext_soap_binding_impl->style = NULL;
    ext_soap_binding_impl->transport_uri = NULL;
    ext_soap_binding_impl->ext_soap_binding.ops = NULL;
    ext_soap_binding_impl->ext_soap_binding.ext_element = NULL;

    ext_soap_binding_impl->ext_soap_binding.ext_element =
        axis2_wsdl_extensible_element_create(env);
    if (NULL == ext_soap_binding_impl->ext_soap_binding.ext_element)
    {
        axis2_wsdl_ext_soap_binding_free(&(ext_soap_binding_impl->ext_soap_binding), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ext_soap_binding_impl->ext_soap_binding.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_ext_soap_binding_ops_t));
    if (NULL == ext_soap_binding_impl->ext_soap_binding.ops)
    {
        axis2_wsdl_ext_soap_binding_free(&(ext_soap_binding_impl->ext_soap_binding), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (!qtype)
    {
        qtype = axis2_qname_create(env, "binding", AXIOM_SOAP_11_BINDING, NULL);
        if (!qtype) return NULL;
    }
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(
        (&(ext_soap_binding_impl->ext_soap_binding))->ext_element, env, qtype);

    ext_soap_binding_impl->ext_soap_binding.ops->free =  axis2_wsdl_ext_soap_binding_free;
    ext_soap_binding_impl->ext_soap_binding.ops->get_style =
        axis2_wsdl_ext_soap_binding_get_style;
    ext_soap_binding_impl->ext_soap_binding.ops->set_style =
        axis2_wsdl_ext_soap_binding_set_style;
    ext_soap_binding_impl->ext_soap_binding.ops->get_transport_uri =
        axis2_wsdl_ext_soap_binding_get_transport_uri;
    ext_soap_binding_impl->ext_soap_binding.ops->set_transport_uri =
        axis2_wsdl_ext_soap_binding_set_transport_uri;

    return &(ext_soap_binding_impl->ext_soap_binding);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_binding_free(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env)
{
    axis2_wsdl_ext_soap_binding_impl_t *exp_soap_binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    exp_soap_binding_impl = AXIS2_INTF_TO_IMPL(ext_soap_binding);

    if (NULL != ext_soap_binding->ops)
        AXIS2_FREE(env->allocator, ext_soap_binding->ops);

    if (NULL != exp_soap_binding_impl->style)
    {
        AXIS2_FREE(env->allocator, exp_soap_binding_impl->style);
        exp_soap_binding_impl->style = NULL;
    }

    if (NULL != exp_soap_binding_impl->transport_uri)
    {
        AXIS2_FREE(env->allocator, exp_soap_binding_impl->transport_uri);
        exp_soap_binding_impl->transport_uri = NULL;
    }

    if (NULL != ext_soap_binding->ext_element)
    {
        AXIS2_WSDL_EXTENSIBLE_ELEMENT_FREE(ext_soap_binding->ext_element, env);
        ext_soap_binding->ext_element = NULL;
    }

    AXIS2_FREE(env->allocator, exp_soap_binding_impl);
    exp_soap_binding_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_binding_get_style(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(ext_soap_binding)->style;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_binding_set_style(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env,
        axis2_char_t *style)
{
    axis2_wsdl_ext_soap_binding_impl_t *exp_soap_binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, style, AXIS2_FAILURE);

    exp_soap_binding_impl = AXIS2_INTF_TO_IMPL(ext_soap_binding);

    if (exp_soap_binding_impl->style)
    {
        AXIS2_FREE(env->allocator, exp_soap_binding_impl->style);
        exp_soap_binding_impl->style = NULL;
    }
    exp_soap_binding_impl->style = AXIS2_STRDUP(style, env);
    if (!exp_soap_binding_impl->style)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_binding_get_transport_uri(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(ext_soap_binding)->transport_uri;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_binding_set_transport_uri(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
        const axis2_env_t *env,
        axis2_char_t *transport_uri)
{
    axis2_wsdl_ext_soap_binding_impl_t *exp_soap_binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, transport_uri, AXIS2_FAILURE);
    exp_soap_binding_impl = AXIS2_INTF_TO_IMPL(ext_soap_binding);

    if (exp_soap_binding_impl->transport_uri)
    {
        AXIS2_FREE(env->allocator, exp_soap_binding_impl->transport_uri);
        exp_soap_binding_impl->transport_uri = NULL;
    }
    exp_soap_binding_impl->transport_uri = AXIS2_STRDUP(transport_uri, env);
    if (!exp_soap_binding_impl->transport_uri)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

