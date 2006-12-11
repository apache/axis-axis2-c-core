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
#include <oxs_asym_ctx.h>
#include <oxs_xml_encryption.h>
/**
  * @file rampart_encryption.h
  * @brief 
  */
#ifndef RAMPART_SEC_HEADER_PROCESSOR_H
#define RAMPART_SEC_HEADER_PROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* Processes a message depending on it's security related claims.
* This is th emain module in the infow of a message if rampart is enabled.
* Processing is depending on the order of tokens apear in the @sec_node
* Also the module will check for the InflowSecurity Settings	
* @param env pointer to environment struct
* @param msg_ctx message context
* @param actions actions defined in the descriptor file
* @param soap_envelope the SOAP envelope
* @param sec_node The security element
* @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shp_process_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_actions_t *actions,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node);


/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* !RAMPART_SEC_HEADER_PROCESSOR_H */
