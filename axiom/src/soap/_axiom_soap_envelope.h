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
 
 #ifndef _AXIOM_SOAP_ENVELOPE_H
 #define _AXIOM_SOAP_ENVELOPE_H
 
 
 /** @defgroup axiom_soap AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */
  
                                   
/** @} */

#include <axiom_soap_envelope.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 * @defgroup axiom_soap_envelope Soap Envelope
 * @ingroup axiom_soap 
 * @{
 */

axis2_status_t AXIS2_CALL
axiom_soap_envelope_set_base_node(axiom_soap_envelope_t *envelope,
                                  const axis2_env_t *env,
                                  axiom_node_t *om_node);  
  
axis2_status_t AXIS2_CALL 
axiom_soap_envelope_set_body(axiom_soap_envelope_t *envelope,
                             const axis2_env_t *env, 
                             struct axiom_soap_body *body);
axis2_status_t AXIS2_CALL 
axiom_soap_envelope_set_header(axiom_soap_envelope_t *envelope,
                               const axis2_env_t *env, 
                               struct axiom_soap_header *header);
    
axis2_status_t AXIS2_CALL
axiom_soap_envelope_set_builder(axiom_soap_envelope_t *envelope,
                                const axis2_env_t *env,
                                struct axiom_soap_builder *soap_builder); 
                                
axis2_status_t AXIS2_CALL 
axiom_soap_envelope_set_soap_version_internal(axiom_soap_envelope_t *envelope,
                                              const axis2_env_t *env,
                                              int soap_version);                                

AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
axiom_soap_envelope_create_null(const axis2_env_t *env);
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif /** _AXIOM_SOAP_ENVELOPE_H  */
