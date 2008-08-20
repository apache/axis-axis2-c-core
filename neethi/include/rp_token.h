
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

#ifndef RP_GENERIC_TOKEN_H
#define RP_GENERIC_TOKEN_H

/** @defgroup rp_token
 * @ingroup rp_token
 * @{
 */

#include <rp_includes.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        DERIVEKEY_NONE =0,
        DERIVEKEY_NEEDED, 
        DERIVEKEY_IMPLIED,
        DERIVEKEY_EXPLICIT
    } derive_key_type_t;

    typedef enum
    {
        DERIVEKEY_VERSION_SC10 =0,
        DERIVEKEY_VERSION_SC13
    } derive_key_version_t;

    typedef struct rp_token_t rp_token_t;

    AXIS2_EXTERN rp_token_t *AXIS2_CALL
    rp_token_create(
        const axutil_env_t * env);

    AXIS2_EXTERN void AXIS2_CALL
    rp_token_free(
        rp_token_t * token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_token_get_issuer(
        rp_token_t * token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_set_issuer(
        rp_token_t * token,
        const axutil_env_t * env,
        axis2_char_t * issuer);

    AXIS2_EXTERN derive_key_type_t AXIS2_CALL
    rp_token_get_derivedkey_type(
        rp_token_t * token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_set_derivedkey_type(
        rp_token_t * token,
        const axutil_env_t * env,
        derive_key_type_t derivedkey);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_token_get_is_issuer_name(
        rp_token_t * token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_set_is_issuer_name(
        rp_token_t * token,
        const axutil_env_t * env,
        axis2_bool_t is_issuer_name);

    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    rp_token_get_claim(
        rp_token_t * token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_set_claim(
        rp_token_t * token,
        const axutil_env_t * env,
        axiom_node_t *claim);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_increment_ref(
        rp_token_t * token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_set_derive_key_version(
        rp_token_t *token, 
        const axutil_env_t *env, 
        derive_key_version_t version);

    AXIS2_EXTERN derive_key_version_t AXIS2_CALL
    rp_token_get_derive_key_version(
        rp_token_t *token, 
        const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_token_set_inclusion(
        rp_token_t *token, 
        const axutil_env_t *env, 
        axis2_char_t *inclusion);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_token_get_inclusion(
        rp_token_t *token, 
        const axutil_env_t *env);


#ifdef __cplusplus
}
#endif
#endif
