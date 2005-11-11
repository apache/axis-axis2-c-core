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

#ifndef AXIS2_OM_NODE_H
#define AXIS2_OM_NODE_H

/** @defgroup axis2_om AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @file axis2_om_node.h
 * @brief defines axis2_om_node struct and its operations
 */
#include <axis2_env.h>
#include <axis2.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_node ;
    struct axis2_om_node_ops;
    struct axis2_om_output;

/**
 * @defgroup axis2_om_node  OM Node
 * @ingroup axis2_om 
 * @{
 */

  /** 
    * @brief OM types
    */
    typedef enum axis2_om_types_t
    {
        /** Invalid node type */
        AXIS2_OM_INVALID = 0,
        /** OM document type */
        AXIS2_OM_DOCUMENT,
        /** OM element type */
        AXIS2_OM_ELEMENT,
        /** OM doctype type */
        AXIS2_OM_DOCTYPE,
        /** OM comment type */
        AXIS2_OM_COMMENT,
        /** OM attribute type */
        AXIS2_OM_ATTRIBUTE,
        /** OM namespace type */
        AXIS2_OM_NAMESPACE,
        /** OM processing instruction type */
        AXIS2_OM_PROCESSING_INSTRUCTION,
        /** OM text type */
        AXIS2_OM_TEXT
    } axis2_om_types_t;

  /** 
    * @brief OM Node operations struct
    * Encapsulator struct for operations of axis2_om_node
    */
  AXIS2_DECLARE_DATA   typedef struct axis2_om_node_ops
    {
       /**
        * Frees an om node and all of its children
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_node node to be freed.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *free) (struct axis2_om_node *om_node,
                                           axis2_env_t **env);
       /**
        * Adds given node as child to parent
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param parent parent node. Mandatory, cannot be NULL.
        * @param child child node. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *add_child) (struct axis2_om_node *om_node,
                                                axis2_env_t **env,
                                                struct axis2_om_node *parent);

      /**
        * Detaches given node from the parent and reset the links
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_node node to be detached. Mandatory, cannot be NULL.
        * @return a pointer to detached node, NULL on error.
        */
        struct axis2_om_node *(AXIS2_CALL *detach) (struct axis2_om_node *om_node,
                                                    axis2_env_t **env);

      /**
        * Inserts a sibling node after the given current node
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param current_node node to whom the sibling to be inserted. Mandatory, cannot be NULL.
        * @param node_to_insert the node to be inserted. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *insert_sibling_after)(struct axis2_om_node *om_node,
                                                          axis2_env_t **env,
                                                          struct axis2_om_node * node_to_insert);

      /**
        * Inserts a sibling node before the given current node
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param current_node node to whom the sibling to be inserted. Mandatory, cannot be NULL.
        * @param node_to_insert the node to be inserted. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *insert_sibling_before)(struct axis2_om_node *om_node,
                                                           axis2_env_t **env,
                                                           struct axis2_om_node * node_to_insert);

      /**
        * Sets a parent node to a given node
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param child_node child node to whom a parent to be added. Mandatory, cannot be NULL.
        * @param parent the node that will be set as parent. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *set_parent) (struct axis2_om_node *om_node,
                                                 axis2_env_t **env,
                                                 struct axis2_om_node *parent_node);

       
       /**
        * Serializes the given node. This operation makes the node go through its children and 
        * serialize them in order.
        * @param env Environment .MUST NOT be NULL, if NULL behaviour is undefined.    
        * @param node node to be serialized. Mandatory cannot be NULL.
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *serialize) (struct axis2_om_node * om_node, axis2_env_t **env,
                                    struct axis2_om_output *om_output);
                                                
        struct axis2_om_node* (AXIS2_CALL *get_parent)(struct axis2_om_node *om_node,
                                                    axis2_env_t **env);

        struct axis2_om_node* (AXIS2_CALL *get_first_child)(struct axis2_om_node *om_node,
                                                        axis2_env_t **env);
                    
        struct axis2_om_node* (AXIS2_CALL *get_last_child)(struct axis2_om_node *om_node,
                                                        axis2_env_t **env);

        struct axis2_om_node* (AXIS2_CALL *get_previous_sibling)(struct axis2_om_node *om_node,
                                                            axis2_env_t **env);

        struct axis2_om_node* (AXIS2_CALL *get_next_sibling)(struct axis2_om_node *om_node,
                                                        axis2_env_t **env);

        axis2_om_types_t (AXIS2_CALL *get_node_type)(struct axis2_om_node *om_node,
                                                        axis2_env_t **env);
                                                
        void* (AXIS2_CALL *get_data_element)(struct axis2_om_node *om_node,
                                                axis2_env_t **env);    
                                                
        axis2_bool_t (AXIS2_CALL *get_build_status)(struct axis2_om_node *om_node,
                                                    axis2_env_t **env);                            

        axis2_status_t (AXIS2_CALL *set_first_child)(struct axis2_om_node *om_node,
                                                        axis2_env_t **env,
                                                        struct axis2_om_node *first_child);
                    
        axis2_status_t (AXIS2_CALL *set_last_child)(struct axis2_om_node *om_node,
                                                    axis2_env_t **env,
                                                    struct axis2_om_node *last_child);
        axis2_status_t (AXIS2_CALL *set_next_sibling)(struct axis2_om_node *om_node,
                                                        axis2_env_t **env,
                                                        struct axis2_om_node *last_sibling);
        
        axis2_status_t (AXIS2_CALL *set_previous_sibling)(struct axis2_om_node *om_node,
                                                            axis2_env_t **env,
                                                            struct axis2_om_node *prev_sibling);
        
        axis2_status_t (AXIS2_CALL *set_node_type)(struct axis2_om_node *om_node,
                                                axis2_env_t **env,
                                                axis2_om_types_t type);
        
        axis2_status_t (AXIS2_CALL *set_data_element)(struct axis2_om_node *om_node,
                                                    axis2_env_t **env,
                                                    void* data_element);
        axis2_status_t (AXIS2_CALL *set_build_status)(struct axis2_om_node *om_node,
                                                    axis2_env_t **env,
                                                    axis2_bool_t done);
    } axis2_om_node_ops_t;


/**
* This is the structure that defines a node in om tree 
* @param parent   - parent node if one is available
* @param parser   - carries a pointer to the XML_PullParser 
* @param element_type - the type of the element one of omtypes
* @param data_element  - stores the structs created for storing xml elements
*						e.g axis2_om_element_t axis2_om_text_t  
*
* we keep pointers parent , previous sibling , next sibling , 
* first child and last child for constructing and navigating the tree
*
*/

    typedef struct axis2_om_node
    {
        /** operations of node struct */
        axis2_om_node_ops_t *ops;
               
    } axis2_om_node_t;

  /**
    * Creates a node struct.
    * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @return a pointer to newly created node struct. NULL on error. 
    */
AXIS2_DECLARE(axis2_om_node_t *) axis2_om_node_create (axis2_env_t **env);

/** frees given node */
#define AXIS2_OM_NODE_FREE(om_node,env) ((om_node)->ops->free(om_node,env))
/** adds given child to the given parent */

#define AXIS2_OM_NODE_ADD_CHILD(om_node,env,parent) \
        ((parent)->ops->add_child(om_node,env,parent))
/** detaches the given node from its parent */

#define AXIS2_OM_NODE_DETACH(om_node,env) ((om_node)->ops->detach(om_node,env))
/** inserts a sibling after the given node */

#define AXIS2_OM_NODE_INSERT_SIBLING_AFTER(om_node,env,node_to_insert) \
        ((current_node)->ops->insert_sibling_after(om_node,env,node_to_insert))
/** inserts a sibling before the given node */
#define AXIS2_OM_NODE_INSERT_SIBLING_BEFORE(om_node,env,node_to_insert) \
        ((current_node)->ops->insert_sibling_before(om_node,env,node_to_insert))
   
/** sets the parent of the given node */
#define AXIS2_OM_NODE_SET_PARENT(om_node,env,parent_node) \
        ((om_node)->ops->set_parent(om_node,env,parent_node))

#define AXIS2_OM_NODE_SERIALIZE(om_node,env, om_output) \
        ((om_node)->ops->serialize(om_node,env,om_output))
   
#define AXIS2_OM_NODE_GET_PARENT(om_node,env) \
        ((om_node)->ops->get_parent(om_node,env))

#define AXIS2_OM_NODE_GET_FIRST_CHILD(om_node,env) \
        ((om_node)->ops->get_first_child(om_node,env))
   
#define AXIS2_OM_NODE_GET_LAST_CHILD(om_node,env) \
        ((om_node)->ops->get_last_child(om_node,env))
   
#define AXIS2_OM_NODE_GET_NEXT_SIBLING(om_node,env) \
        ((om_node)->ops->get_next_sibling(om_node,env))
  
#define AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(om_node,env) \
        ((om_node)->ops->get_previous_sibling(om_node,env))

#define AXIS2_OM_NODE_GET_BUILD_STATUS(om_node,env) \
        ((om_node)->ops->get_build_status(om_node,env))

#define AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node,env) \
        ((om_node)->ops->get_data_element(om_node,env))
        

#define AXIS2_OM_NODE_GET_NODE_TYPE(om_node,env) \
        ((om_node)->ops->get_node_type(om_node,env))       

#define AXIS2_OM_NODE_SET_LAST_CHILD(om_node,env,last_child) \
        ((om_node)->ops->set_last_child(om_node,env,last_child))

#define AXIS2_OM_NODE_SET_FIRST_CHILD(om_node,env,first_child)\
        ((om_node)->ops->set_first_child(om_node,env,first_child))

#define AXIS2_OM_NODE_SET_PREVIOUS_SIBLING(om_node,env,prev_sibling) \
        ((om_node)->ops->set_previous_sibling(om_node,env,prev_sibling))
  
#define AXIS2_OM_NODE_SET_NEXT_SIBLING(om_node,env,next_sibling) \
        ((om_node)->ops->set_next_sibling(om_node,env,next_sibling))

#define AXIS2_OM_NODE_SET_BUILD_STATUS(om_node,env,done) \
        ((om_node)->ops->set_build_status(om_node,env,done))

#define AXIS2_OM_NODE_SET_DATA_ELEMENT(om_node,env,data_element) \
        ((om_node)->ops->set_data_element(om_node,env,data_element))
   
#define AXIS2_OM_NODE_SET_NODE_TYPE(om_node,env,node_type) \
        ((om_node)->ops->set_node_type(om_node,env,node_type))  


/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_NODE_H */
