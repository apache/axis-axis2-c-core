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
 
 #ifndef _AXIS2_SOAP_HEADER_H
 #define _AXIS2_SOAP_HEADER_H
 
 
 /** @defgroup axis2_soap AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */
  
/** @} */

#include <axis2_soap_header.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 * @defgroup axis2_soap_header Soap header
 * @ingroup axis2_soap 
 * @{
 */

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_base_node(axis2_soap_header_t *header,
                                const axis2_env_t *env,
                                axis2_om_node_t *node);

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_soap_version(axis2_soap_header_t *header,
                                   const axis2_env_t *env,
                                   int soap_version);                                                                                                                                                                                                       
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_header_block(axis2_soap_header_t *header,
                                   const axis2_env_t *env,
                                   struct axis2_soap_header_block *header_block);
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_builder(axis2_soap_header_t *header,
                              const axis2_env_t *env,
                              struct axis2_soap_builder *builder); 
    
    
AXIS2_DECLARE(axis2_soap_header_t *)
axis2_soap_header_create(const axis2_env_t *env);    
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif /** _AXIS2_SOAP_HEADER_H  */
