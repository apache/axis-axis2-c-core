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

#ifndef OXS_ASYM_CTX_H
#define OXS_ASYM_CTX_H


/**
  * @file oxs_asym_ctx.h
  * @brief 
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

/** @defgroup oxs_asym_ctx Message Context 
 * @ingroup 
 * @{
 */

    typedef enum  {
        OXS_ASYM_CTX_FORMAT_PEM=0,
        OXS_ASYM_CTX_FORMAT_PKCS12
    }oxs_asym_ctx_format_t;

    typedef enum  {
        OXS_ASYM_CTX_OPERATION_PUB_ENCRYPT=0,
        OXS_ASYM_CTX_OPERATION_PRV_DECRYPT,
        OXS_ASYM_CTX_OPERATION_PUB_DECRYPT,
        OXS_ASYM_CTX_OPERATION_PRV_ENCRYPT
    }oxs_asym_ctx_operation_t;

    typedef struct oxs_asym_ctx_t oxs_asym_ctx_t;

    /*Create function*/
    AXIS2_EXTERN oxs_asym_ctx_t *AXIS2_CALL
    oxs_asym_ctx_create(const axis2_env_t *env);

    /*Free*/
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    oxs_asym_ctx_free(oxs_asym_ctx_t *ctx,
        const axis2_env_t *env);


/**********************Getter functions******************************************/
   
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_free(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_asym_ctx_get_file_name(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN oxs_asym_ctx_format_t AXIS2_CALL
oxs_asym_ctx_get_format(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_asym_ctx_get_algorithm(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN oxs_asym_ctx_operation_t AXIS2_CALL
oxs_asym_ctx_get_operation(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN openssl_pkey_t* AXIS2_CALL
oxs_asym_ctx_private_key(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN oxs_x509_cert_t* AXIS2_CALL
oxs_asym_ctx_get_certificate(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_file_name(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *file_name);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_format(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_asym_ctx_format_t format);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_algorithm(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *algorithm);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_operation(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_asym_ctx_operation_t operation);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_certificate(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_x509_cert_t *certificate);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_private_key(oxs_asym_ctx_t *asym_ctx,
                    const axis2_env_t *env,
                    openssl_pkey_t *private_key);
/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_ASYM_CTX_H */
