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

#include <woden_soap_fault_code.h>

typedef struct woden_soap_fault_code_impl woden_soap_fault_code_impl_t;

/** 
 * @brief Soap Fault Code Struct Impl
 *   Axis2 Soap Fault Code  
 */ 
struct woden_soap_fault_code_impl
{
    woden_soap_fault_code_t soap_fault_code;
    axis2_char_t *f_token;
    axis2_qname_t *f_code_qn;
};

#define INTF_TO_IMPL(soap_fault_code) ((woden_soap_fault_code_impl_t *) soap_fault_code)

static void *WODEN_SOAP_FAULT_CODE_ANY = NULL;

axis2_status_t AXIS2_CALL 
woden_soap_fault_code_free(
        void *soap_fault_code,
        const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
woden_soap_fault_code_is_qname(
        void *soap_fault_code,
        const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
woden_soap_fault_code_is_token(
        void *soap_fault_code,
        const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
woden_soap_fault_code_get_qname(
        void *soap_fault_code,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_soap_fault_code_get_token(
        void *soap_fault_code,
        const axis2_env_t *env);


static woden_soap_fault_code_t *
create(
        const axis2_env_t *env)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_soap_fault_code_impl_t));

    soap_fault_code_impl->f_token = NULL;
    soap_fault_code_impl->f_code_qn = NULL;
   
    soap_fault_code_impl->soap_fault_code.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_soap_fault_code_ops_t));
    
    soap_fault_code_impl->soap_fault_code.ops->free = woden_soap_fault_code_free;

    soap_fault_code_impl->soap_fault_code.ops->is_qname = 
        woden_soap_fault_code_is_qname;
    soap_fault_code_impl->soap_fault_code.ops->is_token = 
        woden_soap_fault_code_is_token;
    soap_fault_code_impl->soap_fault_code.ops->get_qname = 
        woden_soap_fault_code_get_qname;
    soap_fault_code_impl->soap_fault_code.ops->get_token = 
        woden_soap_fault_code_get_token;
    

    return &(soap_fault_code_impl->soap_fault_code);
}

AXIS2_EXTERN woden_soap_fault_code_t * AXIS2_CALL
woden_soap_fault_code_create(
        const axis2_env_t *env,
        axis2_char_t *token,
        axis2_qname_t *code_qn)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = (woden_soap_fault_code_impl_t *) create(env);

    soap_fault_code_impl->f_token = AXIS2_STRDUP(token, env);
    soap_fault_code_impl->f_code_qn = AXIS2_QNAME_CLONE(code_qn, env);

    return &(soap_fault_code_impl->soap_fault_code);
}

axis2_status_t AXIS2_CALL
woden_soap_fault_code_free(
        void *soap_fault_code,
        const axis2_env_t *env)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = INTF_TO_IMPL(soap_fault_code);

    if(soap_fault_code_impl->f_token)
    {
        AXIS2_FREE(env->allocator, soap_fault_code_impl->f_token);
        soap_fault_code_impl->f_token = NULL;
    }

    if(soap_fault_code_impl->f_code_qn)
    {
        AXIS2_QNAME_FREE(soap_fault_code_impl->f_code_qn, env);
        soap_fault_code_impl->f_code_qn = NULL;
    }

    if((&(soap_fault_code_impl->soap_fault_code))->ops)
    {
        AXIS2_FREE(env->allocator, (&(soap_fault_code_impl->soap_fault_code))->ops);
        (&(soap_fault_code_impl->soap_fault_code))->ops = NULL;
    }

    if(soap_fault_code_impl)
    {
        AXIS2_FREE(env->allocator, soap_fault_code_impl);
        soap_fault_code_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_soap_fault_code_get_soap_fault_code_any(
        const axis2_env_t *env)
{

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!WODEN_SOAP_FAULT_CODE_ANY)
        WODEN_SOAP_FAULT_CODE_ANY = woden_soap_fault_code_create(env, 
                "#any", NULL);

    return WODEN_SOAP_FAULT_CODE_ANY;
}

axis2_bool_t AXIS2_CALL
woden_soap_fault_code_get_soap_fault_code_is_qname(
        void *soap_fault_code,
        const axis2_env_t *env)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = INTF_TO_IMPL(soap_fault_code);

    return (NULL != soap_fault_code_impl->f_code_qn);
}

axis2_bool_t AXIS2_CALL
woden_soap_fault_code_is_token(
        void *soap_fault_code,
        const axis2_env_t *env)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = INTF_TO_IMPL(soap_fault_code);

    return (NULL != soap_fault_code_impl->f_token);
}

axis2_qname_t *AXIS2_CALL
woden_soap_fault_code_get_qname(
        void *soap_fault_code,
        const axis2_env_t *env)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = INTF_TO_IMPL(soap_fault_code);

    return soap_fault_code_impl->f_code_qn;
}

axis2_char_t *AXIS2_CALL
woden_soap_fault_code_get_token(
        void *soap_fault_code,
        const axis2_env_t *env)
{
    woden_soap_fault_code_impl_t *soap_fault_code_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_fault_code_impl = INTF_TO_IMPL(soap_fault_code);

    return soap_fault_code_impl->f_token;
}

