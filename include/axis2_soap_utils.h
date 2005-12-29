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
 
#ifndef AXIS2_SOAP_UTILS_H
#define AXIS2_SOAP_UTILS_H
 
   /**
    * @file axis2_soap_utils.h
    * @brief axis2_soap_utils utility functions
    */

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * @defgroup axis2_soap_utils
 * @ingroup axis2_soap_utils
 * @{
 */

AXIS2_DECLARE(axis2_om_node_t *)
axis2_soap_utils_get_child_with_name(axis2_env_t **env,
                                     axis2_om_node_t *om_node,
                                     axis2_char_t *localname);
                                     
AXIS2_DECLARE(axis2_status_t)
axis2_soap_utils_set_new_node(axis2_env_t **env,
                              axis2_om_node_t *parent,
                              axis2_om_node_t **my_node,
                              axis2_om_node_t  *new_node);
                                                                     
 /** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SOAP_UTILS_H */


