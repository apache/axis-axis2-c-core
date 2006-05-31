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
 
 #ifndef AXIS2_OM_NODE_INTERNAL_H
 #define AXIS2_OM_NODE_INTERNAL_H
 
 
 /** @defgroup axis2_om AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */
  
/** @} */

#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 * @defgroup axis2_om_node  OM Node
 * @ingroup axis2_om 
 * @{
 */
struct axis2_om_document;
struct axis2_om_stax_builder;    
    
/**
 * Sets a parent node to a given node, if a parent already exist for this node
 * then it is detached before seting the parent internal function;
 * @param om_node child node to whom a parent to be added. , cannot be NULL.
 * @param env Environment. MUST NOT be NULL, .
 * @param parent_node the node that will be set as parent. , cannot be NULL.
 * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
 */

AXIS2_DECLARE(axis2_status_t)
axis2_om_node_set_parent (axis2_om_node_t * om_node,
                          const axis2_env_t *env,
                          axis2_om_node_t * parent);

/**
*   sets a node as first child of om_node
* @param om_node om_node
* @param env environment, MUST NOT be NULL.
* @param first_child child to be set as first child
*/  
AXIS2_DECLARE(axis2_status_t)
axis2_om_node_set_first_child(axis2_om_node_t *om_node,
                              const axis2_env_t *env,
                              axis2_om_node_t *first_child); 
/**
* set previous sibling
* @param om_node 
* @param env environment, MUST NOT be NULL.
* @param prev_sibling 
* @return status of the op, AXIS2_SUCCESS on success
*               AXIS2_FAILURE on error
*/
    
     
AXIS2_DECLARE(axis2_status_t)  
axis2_om_node_set_previous_sibling(axis2_om_node_t *om_node,                                  
                                   const axis2_env_t *env,
                                   axis2_om_node_t *prev_sibling); 
/**
* set next sibling
* @param om_node 
* @param env environment, MUST NOT be NULL.
* @param last_sibling 
* @return status of the op, AXIS2_SUCCESS on success,
*                   AXIS2_FAILURE on error
*/                                                  
 
AXIS2_DECLARE(axis2_status_t)
axis2_om_node_set_next_sibling(axis2_om_node_t *om_node,
                               const axis2_env_t *env,
                               axis2_om_node_t *next_sibling);
/**
*   set node type
* @param om_node    
* @param env environment, MUST NOT be NULL.
* @param type type of the node
* @return status code of the op AXIS2_SUCCESS on success,
*                       AXIS2_FAILURE on error
*/
AXIS2_DECLARE(axis2_status_t) 
axis2_om_node_set_node_type(axis2_om_node_t *om_node,
                            const axis2_env_t *env,
                            axis2_om_types_t type);

/**
* set data element
* @param om_node  node struct  
* @param env environment, MUST NOT be NULL.
* @param data_element 
* @return status code of the op AXIS2_SUCCESS on success,
*                       AXIS2_FAILURE on error
*/
AXIS2_DECLARE(axis2_status_t) 
axis2_om_node_set_data_element(axis2_om_node_t *om_node,
                               const axis2_env_t *env,
                               void* data_element);
/**
* sets the build status , if the node if completety build, this attribute  is
* set to AXIS2_TRUE , otherwise AXIS2_FALSE  
* @param om_node 
* @param env environment, MUST NOT be NULL.
* @param done                   
*/                                    
AXIS2_DECLARE(axis2_status_t)
axis2_om_node_set_complete(axis2_om_node_t *om_node,
                               const axis2_env_t *env,
                               axis2_bool_t done);  
  
/**
* This functions is only to be used by builder
* do not use this function
*/  
AXIS2_DECLARE(axis2_status_t)
axis2_om_node_set_document(axis2_om_node_t *om_node,
                           const axis2_env_t *env,
                           struct axis2_om_document *om_doc);
/**
*    sets the builder 
*
*/                           
AXIS2_DECLARE(axis2_status_t)
axis2_om_node_set_builder(axis2_om_node_t *om_node,
                          const axis2_env_t *env,
                          struct axis2_om_stax_builder *builder);                           

AXIS2_DECLARE(struct axis2_om_stax_builder *)
axis2_om_node_get_builder(axis2_om_node_t *om_node,
                          const axis2_env_t *env);                                
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif /** AXIS2_OM_NODE_INTERNAL_H */
