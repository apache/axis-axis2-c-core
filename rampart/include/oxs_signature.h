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

#ifndef OXS_SIGNATURE_H
#define OXS_SIGNATURE_H


/**
  * @file oxs_signature.h
  * @brief Does the XML Signature for OMXMLSecurity  
  */

#include <axis2_defines.h>
#include <oxs_ctx.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_qname.h>
#include <oxs_sign_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * Signs an input buffer @input using the RSA-SHA1 algorithm.
 * Result will be placed in output buffer @output
 * @env pointer to environment struct
 * @sign_ctx the signature context
 * @input input buffer
 * @output output buffer
 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_sig_sign_rsa_sha1(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *output);

/**
 * Signs a content placed in @input buf using the information
 * available in the signature context @sign_ctx.
 * The result will be placed in the buffer @output.
 * Note that the result is base64 encoded.
 * @env pointer to environment struct
 * @sign_ctx the signature context
 * @input input buffer
 * @output output buffer
 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *output);

/**
 * Verifies a @signature with @content using the information
 * available in the signature content @sign_ctx.
 * Note that the signature should be the base64 encoded value of a digital signature.
 * @env pointer to environment struct
 * @sign_ctx the signature context
 * @content the content that's signed
 * @signature the signature value
 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_sig_verify(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axis2_char_t *content,
    axis2_char_t *signature);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_SIGNATURE_H */
