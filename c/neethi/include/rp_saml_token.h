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

#ifndef RP_SAML_TOKEN_H
#define RP_SAML_TOKEN_H

#include <rp_includes.h>
#include <axutil_utils.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
    typedef struct rp_saml_token rp_saml_token_t;
    
    AXIS2_EXTERN rp_saml_token_t * AXIS2_CALL
    rp_saml_token_create(
        const axutil_env_t *env);

    AXIS2_EXTERN void AXIS2_CALL
    rp_saml_token_free(
        rp_saml_token_t *saml_token,
        const axutil_env_t *env);
    
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    rp_saml_token_get_inclusion(
            rp_saml_token_t *saml_token,
            const axutil_env_t *env);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_saml_token_set_inclusion(
            rp_saml_token_t *saml_token,
            const axutil_env_t *env,
            axis2_char_t * inclusion);
    
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_saml_token_get_derivedkeys(
                    rp_saml_token_t *saml_token,
                    const axutil_env_t *env);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_saml_token_set_derivedkeys(
                    rp_saml_token_t *saml_token,
                    const axutil_env_t *env,
                    axis2_bool_t derivedkeys);
    
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_saml_token_get_require_key_identifier_reference(
        rp_saml_token_t * saml_token,
        const axutil_env_t * env);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_saml_token_set_require_key_identifier_reference(
        rp_saml_token_t * saml_token,
        const axutil_env_t * env,
        axis2_bool_t require_key_identifier_reference);
    
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_saml_token_get_token_version_and_type(
        rp_saml_token_t * saml_token,
        const axutil_env_t * env);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_saml_token_set_token_version_and_type(
        rp_saml_token_t * saml_token,
        const axutil_env_t * env,
        axis2_char_t * token_version_and_type);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_saml_token_increment_ref(
        rp_saml_token_t * saml_token,
        const axutil_env_t * env);
    
    
#ifdef __cplusplus
}
#endif
#endif
