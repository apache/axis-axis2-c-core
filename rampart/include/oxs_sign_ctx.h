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

#ifndef OXS_SIGN_CTX_H
#define OXS_SIGN_CTX_H


/**
  * @file oxs_sign_ctx.h
  * @brief Keeps information relavent for a single node of signing. 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <oxs_x509_cert.h>
#include <openssl_pkey.h>

#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct oxs_sign_ctx_t oxs_sign_ctx_t;

    /*Create function*/
    AXIS2_EXTERN oxs_sign_ctx_t *AXIS2_CALL
    oxs_sign_ctx_create(const axis2_env_t *env);

    /*Free*/
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    oxs_sign_ctx_free(oxs_sign_ctx_t *ctx,
        const axis2_env_t *env);


/**********************Getter functions******************************************/
axis2_char_t *AXIS2_CALL
oxs_sign_ctx_get_sign_mtd_algo(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
oxs_sign_ctx_get_c14n_mtd(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
oxs_sign_ctx_get_sign_parts(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env);

oxs_x509_cert_t *AXIS2_CALL
oxs_sign_ctx_get_certificate(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env);

openssl_pkey_t *AXIS2_CALL
oxs_sign_ctx_get_private_key(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env);

openssl_pkey_t *AXIS2_CALL
oxs_sign_ctx_get_public_key(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env);

/**********************Setter functions******************************************/
axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_sign_mtd_algo(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    axis2_char_t *sign_mtd_algo);

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_c14n_mtd(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    axis2_char_t *c14n_mtd);

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_sign_parts(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    axis2_array_list_t *sign_parts);

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_certificate(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    oxs_x509_cert_t *certificate);

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_private_key(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    openssl_pkey_t *prv_key);

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_public_key(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    openssl_pkey_t *pub_key);


/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_SIGN_CTX_H */
