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

#include <axis2_ntlm.h>
#include <axutil_utils_defines.h>
#include <axutil_utils.h>
#include <string.h>
#include <axutil_string.h>

void AXIS2_CALL
default_ntlm_wrapper_free(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env);

axis2_status_t AXIS2_CALL
default_ntlm_wrapper_create_type1_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *uname,
    const axis2_char_t *passwd,
    const int flags,
    const axis2_char_t *domain);

axis2_status_t AXIS2_CALL
default_ntlm_wrapper_create_type3_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t *header_value,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *uname,
    const axis2_char_t *passwd,
    const axis2_char_t *domain,
    const axis2_char_t *workstation);

typedef struct default_ntlm_wrapper_impl_t
{
    axis2_ntlm_t ntlm;

    /*xmlTextReaderPtr reader;*/

} default_ntlm_wrapper_impl_t;

#define AXIS2_INTF_TO_IMPL(p) ((default_ntlm_wrapper_impl_t*)p)
#define AXIS2_IMPL_TO_INTF(p) &(p->ntlm)

static const axis2_ntlm_ops_t axis2_ntlm_ops_var = {
    default_ntlm_wrapper_free,
    default_ntlm_wrapper_create_type1_message,
    default_ntlm_wrapper_create_type3_message };

AXIS2_EXTERN axis2_ntlm_t *AXIS2_CALL
axis2_ntlm_create(
    const axutil_env_t *env)
{
    default_ntlm_wrapper_impl_t *wrapper_impl = NULL;
    wrapper_impl = (default_ntlm_wrapper_impl_t *)AXIS2_MALLOC(env->allocator,
        sizeof(default_ntlm_wrapper_impl_t));

    if(!wrapper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory. Cannot create default ntlm wrapper");
        return NULL;
    }
    memset(wrapper_impl, 0, sizeof(default_ntlm_wrapper_impl_t));
    /*wrapper_impl->x=NULL;*/
    wrapper_impl->ntlm.ops = &axis2_ntlm_ops_var;
    return &(wrapper_impl->ntlm);
}

/**
 */
void AXIS2_CALL
default_ntlm_wrapper_free(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env)
{
    default_ntlm_wrapper_impl_t *ntlm_impl = NULL;
    ntlm_impl = AXIS2_INTF_TO_IMPL(ntlm);

    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(ntlm));
    return;
}

axis2_status_t AXIS2_CALL
default_ntlm_wrapper_create_type1_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *user,
    const axis2_char_t *password,
    const int flags,
    const axis2_char_t *domain)
{
    axis2_status_t status = AXIS2_FAILURE;
    return status;
}

axis2_status_t AXIS2_CALL
default_ntlm_wrapper_create_type3_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t *header_value,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *user,
    const axis2_char_t *password,
    const axis2_char_t *domain,
    const axis2_char_t *workstation)
{
    axis2_status_t status = AXIS2_FAILURE;
    return status;
}


