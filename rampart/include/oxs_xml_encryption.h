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

#ifndef OXS_XML_ENCRYPTION_H
#define OXS_XML_ENCRYPTION_H


/**
  * @file oxs_xml_enc.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_ctx.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_enc_encrypt_node(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *node,
                            axiom_node_t **enc_type_node);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_node(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *enc_type_node,
                            axiom_node_t **decrypted_node);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_enc_encrypt_data(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            oxs_buffer_t *content_buf,
                            axiom_node_t **enc_type_node);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_data(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *enc_type_node,
                            oxs_buffer_t *result_buf);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_enc_encrypt_key(const axis2_env_t *env,
                            oxs_asym_ctx_t * asym_ctx,
                            axiom_node_t *parent,
                            oxs_key_t *sym_key,
                            axis2_array_list_t *id_list);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_key(const axis2_env_t *env,
                            oxs_asym_ctx_t * asym_ctx,
                            axiom_node_t *parent,
                            axiom_node_t *encrypted_key_node,
                            oxs_key_t *key);                            
/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_XML_ENCRYPTION_H */
