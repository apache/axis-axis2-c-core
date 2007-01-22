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

#ifndef RP_ENCRYPTION_CRYPTO_H
#define RP_ENCRYPTION_CRYPTO_H

/** @defgroup rp_encryption_crypto
 * @ingroup rp_encryption_crypto
 * @{
 */

#include <rp_includes.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
    typedef struct rp_encryption_crypto_t rp_encryption_crypto_t;

    AXIS2_EXTERN rp_encryption_crypto_t *AXIS2_CALL
    rp_encryption_crypto_create(const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_encryption_crypto_free(
        rp_encryption_crypto_t *encryption_crypto,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_encryption_crypto_get_encryption_prop_file(
        rp_encryption_crypto_t *encryption_crypto,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_encryption_crypto_set_encryption_prop_file(
        rp_encryption_crypto_t *encryption_crypto,
        const axis2_env_t *env,
        axis2_char_t *encryption_prop_file);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_encryption_crypto_get_decryption_prop_file(
        rp_encryption_crypto_t *encryption_crypto,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_encryption_crypto_set_decryption_prop_file(
        rp_encryption_crypto_t *encryption_crypto,
        const axis2_env_t *env,
        axis2_char_t *decryption_prop_file);



#ifdef __cplusplus
}
#endif
#endif  
