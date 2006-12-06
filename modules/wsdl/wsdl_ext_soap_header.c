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

#include <axis2_wsdl_ext_soap_header.h>
#include <axis2_wsdl_ext_soap_body.h>
#include <axis2_wsdl.h>

/**
 * @brief Wsdl Extension Soap Header struct impl
 *    Wsdl Extention Soap Header
 */
typedef struct axis2_wsdl_ext_soap_header_impl
{
    axis2_wsdl_ext_soap_header_t ext_soap_header;

    axis2_qname_t *msg_qname;
    axis2_char_t *part;
    axis2_qname_t *qelement;
}
axis2_wsdl_ext_soap_header_impl_t;

#define AXIS2_INTF_TO_IMPL(ext_soap_header) \
      ((axis2_wsdl_ext_soap_header_impl_t *)ext_soap_header)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_free(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_header_get_part(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_set_part(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env,
        axis2_char_t *part);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_ext_soap_header_get_msg_qname(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_set_msg_qname(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env,
        axis2_qname_t *msg_qname);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_ext_soap_header_get_qelement(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_set_qelement(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env,
        axis2_qname_t *qelement);

/************************** End of function prototypes ************************/

axis2_wsdl_ext_soap_header_t * AXIS2_CALL
axis2_wsdl_ext_soap_header_create(const axis2_env_t *env,
        axis2_qname_t *qtype)
{
    axis2_wsdl_ext_soap_header_impl_t *ext_soap_header_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    ext_soap_header_impl = (axis2_wsdl_ext_soap_header_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_wsdl_ext_soap_header_impl_t));


    if (NULL == ext_soap_header_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ext_soap_header_impl->part = NULL;
    ext_soap_header_impl->msg_qname = NULL;
    ext_soap_header_impl->qelement = NULL;
    ext_soap_header_impl->ext_soap_header.ops = NULL;
    ext_soap_header_impl->ext_soap_header.ext_soap_body = NULL;

    ext_soap_header_impl->ext_soap_header.ext_soap_body =
        axis2_wsdl_ext_soap_body_create(env, NULL);
    if (NULL == ext_soap_header_impl->ext_soap_header.ext_soap_body)
    {
        axis2_wsdl_ext_soap_header_free(&(ext_soap_header_impl->ext_soap_header), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ext_soap_header_impl->ext_soap_header.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_ext_soap_header_ops_t));
    if (NULL == ext_soap_header_impl->ext_soap_header.ops)
    {
        axis2_wsdl_ext_soap_header_free(&(ext_soap_header_impl->ext_soap_header), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (!qtype)
    {
        qtype = axis2_qname_create(env, "header", AXIOM_SOAP_11_HEADER, NULL);
        if (!qtype) return NULL;
    }
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(
        (&(ext_soap_header_impl->ext_soap_header))->ext_soap_body->ext_element,
        env, qtype);

    ext_soap_header_impl->ext_soap_header.ops->free =  axis2_wsdl_ext_soap_header_free;
    ext_soap_header_impl->ext_soap_header.ops->get_part =
        axis2_wsdl_ext_soap_header_get_part;
    ext_soap_header_impl->ext_soap_header.ops->set_part =
        axis2_wsdl_ext_soap_header_set_part;
    ext_soap_header_impl->ext_soap_header.ops->get_msg_qname =
        axis2_wsdl_ext_soap_header_get_msg_qname;
    ext_soap_header_impl->ext_soap_header.ops->set_msg_qname =
        axis2_wsdl_ext_soap_header_set_msg_qname;
    ext_soap_header_impl->ext_soap_header.ops->get_qelement =
        axis2_wsdl_ext_soap_header_get_qelement;
    ext_soap_header_impl->ext_soap_header.ops->set_qelement =
        axis2_wsdl_ext_soap_header_set_qelement;

    return &(ext_soap_header_impl->ext_soap_header);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_free(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env)
{
    axis2_wsdl_ext_soap_header_impl_t *exp_soap_header_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    exp_soap_header_impl = AXIS2_INTF_TO_IMPL(ext_soap_header);

    if (ext_soap_header->ops)
        AXIS2_FREE(env->allocator, ext_soap_header->ops);

    if (exp_soap_header_impl->part)
    {
        AXIS2_FREE(env->allocator, exp_soap_header_impl->part);
        exp_soap_header_impl->part = NULL;
    }

    if (exp_soap_header_impl->msg_qname)
    {
        AXIS2_QNAME_FREE(exp_soap_header_impl->msg_qname, env);
        exp_soap_header_impl->msg_qname = NULL;
    }

    if (exp_soap_header_impl->qelement)
    {
        AXIS2_QNAME_FREE(exp_soap_header_impl->qelement, env);
        exp_soap_header_impl->qelement = NULL;
    }

    if (ext_soap_header->ext_soap_body)
    {
        AXIS2_WSDL_EXT_SOAP_BODY_FREE(ext_soap_header->ext_soap_body, env);
        ext_soap_header->ext_soap_body = NULL;
    }

    AXIS2_FREE(env->allocator, exp_soap_header_impl);
    exp_soap_header_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_header_get_part(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(ext_soap_header)->part;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_set_part(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env,
        axis2_char_t *part)
{
    axis2_wsdl_ext_soap_header_impl_t *exp_soap_header_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, part, AXIS2_FAILURE);

    exp_soap_header_impl = AXIS2_INTF_TO_IMPL(ext_soap_header);

    if (exp_soap_header_impl->part)
    {
        AXIS2_FREE(env->allocator, exp_soap_header_impl->part);
        exp_soap_header_impl->part = NULL;
    }
    exp_soap_header_impl->part = AXIS2_STRDUP(part, env);
    if (!exp_soap_header_impl->part)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_ext_soap_header_get_msg_qname(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(ext_soap_header)->msg_qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_set_msg_qname(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env,
        axis2_qname_t *msg_qname)
{
    axis2_wsdl_ext_soap_header_impl_t *exp_soap_header_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_qname, AXIS2_FAILURE);
    exp_soap_header_impl = AXIS2_INTF_TO_IMPL(ext_soap_header);

    if (exp_soap_header_impl->msg_qname)
    {
        AXIS2_QNAME_FREE(exp_soap_header_impl->msg_qname, env);
        exp_soap_header_impl->msg_qname = NULL;
    }
    exp_soap_header_impl->msg_qname = AXIS2_QNAME_CLONE(msg_qname, env);
    if (!exp_soap_header_impl->msg_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_ext_soap_header_get_qelement(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(ext_soap_header)->qelement;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_header_set_qelement(axis2_wsdl_ext_soap_header_t *ext_soap_header,
        const axis2_env_t *env,
        axis2_qname_t *qelement)
{
    axis2_wsdl_ext_soap_header_impl_t *exp_soap_header_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qelement, AXIS2_FAILURE);
    exp_soap_header_impl = AXIS2_INTF_TO_IMPL(ext_soap_header);

    if (exp_soap_header_impl->qelement)
    {
        AXIS2_QNAME_FREE(exp_soap_header_impl->qelement, env);
        exp_soap_header_impl->qelement = NULL;
    }
    exp_soap_header_impl->qelement = AXIS2_QNAME_CLONE(qelement, env);
    if (!exp_soap_header_impl->qelement)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

