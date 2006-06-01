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

#ifndef AXIS2_OM_UTIL_H
#define AXIS2_OM_UTIL_H

#include <axis2_array_list.h>
#include <axis2_string.h>
#include <axis2_om.h>

#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_uri(
        axis2_om_node_t *ele_node,
        const axis2_env_t *env,
        axis2_char_t *uri,
        axis2_om_node_t **child);
        
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_sibling_element_with_uri(
        axis2_om_node_t *ele_node,
        const axis2_env_t *env,
        axis2_char_t *uri,
        axis2_om_node_t **next_node);        



AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element(axis2_om_element_t *ele,
                                      const axis2_env_t *env,
                                      axis2_om_node_t *ele_node,
                                      axis2_om_node_t **child_node);
                                      
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element(axis2_om_element_t *ele,
                                     const axis2_env_t *env,
                                     axis2_om_node_t *ele_node,
                                     axis2_om_node_t **child_node);
                                     
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_sibling_element(axis2_om_element_t *ele,
                                       const axis2_env_t *env,
                                       axis2_om_node_t *ele_node,
                                       axis2_om_node_t **next_node);
                                       
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_om_node_t **child_node);
                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_om_node_t **child_node);
                                                                                         
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_siblng_element_with_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_om_node_t **next_node);
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_uri_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *uri,
                                             axis2_om_node_t **child_node);                                                                                                                
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_uri_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *uri,
                                             axis2_om_node_t **child_node);                        AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_sibling_element_with_uri_localname(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *uri,
                                             axis2_om_node_t **next_node); 
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_localnames(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_array_list_t *names,
                                             axis2_om_node_t **child_node);
                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_localnames(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_array_list_t *names,
                                             axis2_om_node_t **child_node);
                                                                                         
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_siblng_element_with_localnames(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_array_list_t *names,
                                             axis2_om_node_t **next_node); 
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_first_child_element_with_localname_attr(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *attr_name,
                                             axis2_char_t *attr_value,
                                             axis2_om_node_t **child_node);
                                             
                                             
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_last_child_element_with_localname_attr(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *attr_name,
                                             axis2_char_t *attr_value,
                                             axis2_om_node_t **child_node);
                                                                                         
AXIS2_EXTERN axis2_om_element_t * AXIS2_CALL
axis2_om_util_get_next_siblng_element_with_localname_attr(axis2_om_element_t *ele,
                                             const axis2_env_t  *env,
                                             axis2_om_node_t *ele_node,
                                             axis2_char_t *localname,
                                             axis2_char_t *attr_name,
                                             axis2_char_t *attr_value,
                                             axis2_om_node_t **next_node); 
                                                                            
AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_om_util_get_child_text(axis2_om_node_t *node,
                             const axis2_env_t *env);
                             
AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_om_util_get_localname(axis2_om_node_t *node,
                            const axis2_env_t *env);
                            
AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_om_util_get_node_namespace_uri(axis2_om_node_t *om_node,
                                     const axis2_env_t *env);                            
                                                                                             
                         
AXIS2_EXTERN axis2_om_child_element_iterator_t* AXIS2_CALL
axis2_om_util_get_child_elements(axis2_om_element_t *om_ele, 
                                 const axis2_env_t *env,
                                 axis2_om_node_t *om_node);
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_OM_UTIL_H */
