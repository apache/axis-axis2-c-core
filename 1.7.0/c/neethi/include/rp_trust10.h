
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

#ifndef RP_TRUST10_H
#define RP_TRUST10_H

/** @defgroup trust10
 * @ingroup trust10
 * @{
 */

#include <rp_includes.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct rp_trust10_t rp_trust10_t;

    AXIS2_EXTERN rp_trust10_t *AXIS2_CALL
    rp_trust10_create(
        const axutil_env_t * env);

    AXIS2_EXTERN void AXIS2_CALL
    rp_trust10_free(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
        rp_trust10_get_must_support_client_challenge(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
        rp_trust10_set_must_support_client_challenge(
        rp_trust10_t * trust10,
        const axutil_env_t * env,
        axis2_bool_t must_support_client_challenge);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_trust10_get_must_support_server_challenge(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_trust10_set_must_support_server_challenge(
        rp_trust10_t * trust10,
        const axutil_env_t * env,
        axis2_bool_t must_support_server_challenge);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_trust10_get_require_client_entropy(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_trust10_set_require_client_entropy(
        rp_trust10_t * trust10,
        const axutil_env_t * env,
        axis2_bool_t require_client_entropy);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_trust10_get_require_server_entropy(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_trust10_set_require_server_entropy(
        rp_trust10_t * trust10,
        const axutil_env_t * env,
        axis2_bool_t require_server_entropy);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_trust10_get_must_support_issued_token(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_trust10_set_must_support_issued_token(
        rp_trust10_t * trust10,
        const axutil_env_t * env,
        axis2_bool_t must_support_issued_token);


    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_trust10_increment_ref(
        rp_trust10_t * trust10,
        const axutil_env_t * env);

#ifdef __cplusplus
}
#endif
#endif
