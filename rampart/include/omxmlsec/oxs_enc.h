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

#ifndef OXS_ENC_H
#define OXS_ENC_H


/**
  * @file oxs_enc.h
  * @brief 
  */

#include <axis2_defines.h>
#include <omxmlsec/oxs_constants.h>
#include <omxmlsec/oxs_ctx.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_qname.h>
#ifdef __cplusplus
extern "C"
{
#endif

/**
*@node node to be encrypted
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_xml_encrypt(const axis2_env_t *env,
                        enc_ctx_t* ctx,
                        axiom_node_t* tmpl,
                        axiom_node_t* node                        
                    );

/**
*@data data to be encrypted
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_binary_encrypt(const axis2_env_t *env,
                        enc_ctx_t* ctx,
                        axiom_node_t* tmpl,
                        axis2_char_t* data
                   );


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_populate_ctx(const axis2_env_t *env,
                    enc_ctx_t* ctx,
                    axiom_node_t* tmpl
                    );

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_ENC_H */
