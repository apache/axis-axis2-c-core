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

#ifndef OXS_ENCRIPTION_METHOD_H
#define OXS_ENCRIPTION_METHOD_H


/**
  * @file oxs_token_encription_method.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_qname.h>
#ifdef __cplusplus
extern "C"
{
#endif


/**
* Encryption Method element
*/
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_encryption_method_element(const axis2_env_t *env,
                        axiom_node_t *parent,
                        axis2_char_t* algorithm
                    );

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_token_get_encryption_method(const axis2_env_t *env, axiom_node_t *enc_mtd_node);
/*TODO write free method*/
/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_ENCRIPTION_METHOD_H */
