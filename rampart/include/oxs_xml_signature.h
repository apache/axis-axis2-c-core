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

#ifndef OXS_XML_SIGNATURE_H
#define OXS_XML_SIGNATURE_H


/**
  * @file oxs_xml_enc.h
  * @brief Does the XML encryption for OMXMLSecurity  
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
/**
 * Sign
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx);

/**
 * Verify
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_sig_verify(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_XML_SIGNATURE_H */
