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
 
 #ifndef AXIS2_OM_NAMESPACE_INTERNAL_H
 #define AXIS2_OM_NAMESPACE_INTERNAL_H
 
 
 /** @defgroup axis2_om AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */
  
/** @} */

#include <axis2_om_namespace.h>

#ifdef __cplusplus
extern "C"
{
#endif
   
AXIS2_DECLARE(axis2_status_t)
axis2_om_namespace_set_uri(axis2_om_namespace_t *ns,
                          axis2_env_t **env,
                          axis2_char_t *ns_uri);
                          
#ifdef __cplusplus
}
#endif

#endif /** AXIS2_OM_NAMESPACE_H */
