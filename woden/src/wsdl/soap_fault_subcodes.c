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

#include <woden_soap_fault_subcodes.h>

typedef struct woden_soap_fault_subcodes_impl woden_soap_fault_subcodes_impl_t;

/**
 * @brief Soap Fault Subcodes Struct Impl
 *   Axis2 Soap Fault Subcodes
 */
struct woden_soap_fault_subcodes_impl
{
    woden_soap_fault_subcodes_t soap_fault_subcodes;
    axis2_char_t *f_token;
    axis2_array_list_t *f_subcode_qns;
};

#define INTF_TO_IMPL(soap_fault_subcodes) ((woden_soap_fault_subcodes_impl_t *) soap_fault_subcodes)

static void *WODEN_SOAP_FAULT_SUBCODES_ANY = NULL;

axis2_status_t AXIS2_CALL
woden_soap_fault_subcodes_free(
    void *soap_fault_subcodes,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
woden_soap_fault_subcodes_is_qnames(
    void *soap_fault_subcodes,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
woden_soap_fault_subcodes_is_token(
    void *soap_fault_subcodes,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_soap_fault_subcodes_get_qnames(
    void *soap_fault_subcodes,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_soap_fault_subcodes_get_token(
    void *soap_fault_subcodes,
    const axis2_env_t *env);


static woden_soap_fault_subcodes_t *
create(
    const axis2_env_t *env)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    soap_fault_subcodes_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_soap_fault_subcodes_impl_t));

    soap_fault_subcodes_impl->f_token = NULL;
    soap_fault_subcodes_impl->f_subcode_qns = NULL;

    soap_fault_subcodes_impl->soap_fault_subcodes.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_soap_fault_subcodes_ops_t));

    soap_fault_subcodes_impl->soap_fault_subcodes.ops->free = woden_soap_fault_subcodes_free;

    soap_fault_subcodes_impl->soap_fault_subcodes.ops->is_qnames =
        woden_soap_fault_subcodes_is_qnames;
    soap_fault_subcodes_impl->soap_fault_subcodes.ops->is_token =
        woden_soap_fault_subcodes_is_token;
    soap_fault_subcodes_impl->soap_fault_subcodes.ops->get_qnames =
        woden_soap_fault_subcodes_get_qnames;
    soap_fault_subcodes_impl->soap_fault_subcodes.ops->get_token =
        woden_soap_fault_subcodes_get_token;


    return &(soap_fault_subcodes_impl->soap_fault_subcodes);
}

AXIS2_EXTERN woden_soap_fault_subcodes_t * AXIS2_CALL
woden_soap_fault_subcodes_create(
    const axis2_env_t *env,
    axis2_char_t *token,
    axis2_array_list_t *subcode_qns)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    soap_fault_subcodes_impl = (woden_soap_fault_subcodes_impl_t *) create(env);

    soap_fault_subcodes_impl->f_token = AXIS2_STRDUP(token, env);
    soap_fault_subcodes_impl->f_subcode_qns = subcode_qns;

    return &(soap_fault_subcodes_impl->soap_fault_subcodes);
}

axis2_status_t AXIS2_CALL
woden_soap_fault_subcodes_free(
    void *soap_fault_subcodes,
    const axis2_env_t *env)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_subcodes_impl = INTF_TO_IMPL(soap_fault_subcodes);

    if (soap_fault_subcodes_impl->f_token)
    {
        AXIS2_FREE(env->allocator, soap_fault_subcodes_impl->f_token);
        soap_fault_subcodes_impl->f_token = NULL;
    }

    if (soap_fault_subcodes_impl->f_subcode_qns)
    {
        int i = 0, size = 0;

        size = AXIS2_ARRAY_LIST_SIZE(soap_fault_subcodes_impl->f_subcode_qns,
                env);
        for (i = 0; i < size; i++)
        {
            axis2_qname_t *qname = AXIS2_ARRAY_LIST_GET(soap_fault_subcodes_impl->
                    f_subcode_qns, env, i);
            AXIS2_QNAME_FREE(qname, env);
        }
        AXIS2_ARRAY_LIST_FREE(soap_fault_subcodes_impl->f_subcode_qns, env);
        soap_fault_subcodes_impl->f_subcode_qns = NULL;
    }

    if ((&(soap_fault_subcodes_impl->soap_fault_subcodes))->ops)
    {
        AXIS2_FREE(env->allocator, (&(soap_fault_subcodes_impl->soap_fault_subcodes))->ops);
        (&(soap_fault_subcodes_impl->soap_fault_subcodes))->ops = NULL;
    }

    if (soap_fault_subcodes_impl)
    {
        AXIS2_FREE(env->allocator, soap_fault_subcodes_impl);
        soap_fault_subcodes_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_soap_fault_subcodes_get_soap_fault_subcodes_any(
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    if (!WODEN_SOAP_FAULT_SUBCODES_ANY)
        WODEN_SOAP_FAULT_SUBCODES_ANY = woden_soap_fault_subcodes_create(env,
                "#any", NULL);

    return WODEN_SOAP_FAULT_SUBCODES_ANY;
}

axis2_bool_t AXIS2_CALL
woden_soap_fault_subcodes_is_qnames(
    void *soap_fault_subcodes,
    const axis2_env_t *env)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;
    int size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_subcodes_impl = INTF_TO_IMPL(soap_fault_subcodes);

    size = AXIS2_ARRAY_LIST_SIZE(soap_fault_subcodes_impl->f_subcode_qns, env);
    return (size > 0);
}

axis2_bool_t AXIS2_CALL
woden_soap_fault_subcodes_is_token(
    void *soap_fault_subcodes,
    const axis2_env_t *env)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_subcodes_impl = INTF_TO_IMPL(soap_fault_subcodes);

    return (NULL != soap_fault_subcodes_impl->f_token);
}

axis2_array_list_t *AXIS2_CALL
woden_soap_fault_subcodes_get_qnames(
    void *soap_fault_subcodes,
    const axis2_env_t *env)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    soap_fault_subcodes_impl = INTF_TO_IMPL(soap_fault_subcodes);

    return soap_fault_subcodes_impl->f_subcode_qns;
}

axis2_char_t *AXIS2_CALL
woden_soap_fault_subcodes_get_token(
    void *soap_fault_subcodes,
    const axis2_env_t *env)
{
    woden_soap_fault_subcodes_impl_t *soap_fault_subcodes_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    soap_fault_subcodes_impl = INTF_TO_IMPL(soap_fault_subcodes);

    return soap_fault_subcodes_impl->f_token;
}

