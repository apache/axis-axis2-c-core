/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <axis2_utils_defines.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_soap.h>
#include <axis2_msg_ctx.h>
#include <rampart_action.h>

/**
  * @file rampart_crypto_engine.h
  * @brief 
  */
#ifndef RAMPART_CRYPTO_ENGINE
#define RAMPART_CRYPTO_ENGINE

#ifdef __cplusplus
extern "C" {
#endif

/*Get the message and encrypt*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_crypto_encrypt_message(const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      axis2_param_t* param_action,
                      axiom_soap_envelope_t *soap_envelope );

/*Get the message and decrypt*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_crypto_decrypt_message(const axis2_env_t *env,
                       axis2_msg_ctx_t *msg_ctx,
                       axis2_param_t* param_action,
                      axiom_soap_envelope_t *soap_envelope );
/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* !RAMPART_CRYPTO_H */
