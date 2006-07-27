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

#ifndef OXS_ENC_ENGINE_H
#define OXS_ENC_ENGINE_H


/**
  * @file oxs_enc.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_ctx.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_qname.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl_constants.h>

#ifdef __cplusplus
extern "C"
{
#endif



AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encrypt(const axis2_env_t *env,
                enc_ctx_ptr enc_ctx,
                oxs_buffer_ptr input,
                axis2_char_t* key,
                oxs_buffer_ptr result);

/*We expect user to provide a template as below 
TODO: Write the template*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encrypt_template(const axis2_env_t *env,
                        axiom_node_t* template,
                        axis2_char_t* data,
                        enc_ctx_ptr enc_ctx
                        );


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_cipher_data_node_read(const axis2_env_t *env,
                         enc_ctx_ptr enc_ctx, axiom_node_t* node);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encryption_data_node_read(const axis2_env_t *env,
                        enc_ctx_ptr enc_ctx,
                        axiom_node_t* node);





/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_ENC_ENGINE_H */
