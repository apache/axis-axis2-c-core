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
 
 #ifndef _AXIOM_SOAP_FAULT_H
 #define _AXIOM_SOAP_FAULT_H
 
 
 /** @defgroup axiom_soap AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */
  
/** @} */

#include <axiom_soap_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 * @defgroup axiom_soap_fault Soap fault
 * @ingroup axiom_soap 
 * @{
 */

axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_code(axiom_soap_fault_t *fault,
                          const axis2_env_t *env,
                          struct axiom_soap_fault_code *code);
axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_reason(axiom_soap_fault_t *fault,
                            const axis2_env_t *env,
                            struct axiom_soap_fault_reason *reason);
axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_node(axiom_soap_fault_t *fault,
                          const axis2_env_t *env,
                          struct axiom_soap_fault_node *node);
axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_role(axiom_soap_fault_t *fault,
                          const axis2_env_t *env,
                          struct axiom_soap_fault_role *role);
axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_detail(axiom_soap_fault_t *fault,
                            const axis2_env_t *env,
                            struct axiom_soap_fault_detail *detail);
axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_base_node(axiom_soap_fault_t *fault,
                               const axis2_env_t *env,
                               axiom_node_t *node);

axis2_status_t AXIS2_CALL 
axiom_soap_fault_set_builder(axiom_soap_fault_t *fault,
                             const axis2_env_t *env,
                             struct axiom_soap_builder *builder);
    
axis2_status_t AXIS2_CALL
axiom_soap_fault_set_soap_version(axiom_soap_fault_t *fault,
                                  const axis2_env_t *env,
                                  int soap_version);
                                  
int AXIS2_CALL
axiom_soap_fault_get_soap_version(axiom_soap_fault_t *fault,
                                  const axis2_env_t *env);
    
    
AXIS2_EXTERN axiom_soap_fault_t * AXIS2_CALL
axiom_soap_fault_create(const axis2_env_t *env);    
    
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif /** _AXIOM_SOAP_FAULT_H  */
