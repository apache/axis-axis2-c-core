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

#ifdef AXIS2_NTLM_ENABLED

#include <axis2_ntlm.h>

AXIS2_EXTERN void AXIS2_CALL
axis2_ntlm_free(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env)
{
    (ntlm)->ops->free(ntlm, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ntlm_auth_create_type1_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *uname,
    const axis2_char_t *passwd,
    const int flags,
    const axis2_char_t *domain)
{
    return (ntlm)->ops->create_type1_message(ntlm, env, encoded_message, encoded_len, uname, 
            passwd, flags, domain);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ntlm_auth_create_type3_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t *header_value,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *uname,
    const axis2_char_t *passwd,
    const axis2_char_t *domain,
    const axis2_char_t *workstation)
{
    return (ntlm)->ops->create_type3_message(ntlm, env, header_value, encoded_message, encoded_len, 
            uname, passwd, domain, workstation);
}

#endif