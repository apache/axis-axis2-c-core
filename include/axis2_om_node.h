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
#include <axis2_environment.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_node;
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
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_node node to be freed.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_node_ops_free) (axis2_environment_t *
                                                  environment,
                                                  struct axis2_om_node *
                                                  om_node);
       /**
        * Adds given node as child to parent
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param parent parent node. Mandatory, cannot be NULL.
        * @param child child node. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_node_ops_add_child) (axis2_environment_t *
                                                       environment,
                                                       struct axis2_om_node *
                                                       parent,
                                                       struct axis2_om_node *
                                                       child);

      /**
        * Detaches given node from the parent and reset the links
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_node node to be detached. Mandatory, cannot be NULL.
        * @return a pointer to detached node, NULL on error.
        */
        struct axis2_om_node *(AXIS2_CALL *axis2_om_node_ops_detach) (axis2_environment_t
                                                           * environment,
                                                           struct
                                                           axis2_om_node *
                                                           om_node);

      /**
        * Inserts a sibling node after the given current node
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param current_node node to whom the sibling to be inserted. Mandatory, cannot be NULL.
        * @param node_to_insert the node to be inserted. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_node_ops_insert_sibling_after)
            (axis2_environment_t * environment,
             struct axis2_om_node * current_node,
             struct axis2_om_node * node_to_insert);

      /**
        * Inserts a sibling node before the given current node
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param current_node node to whom the sibling to be inserted. Mandatory, cannot be NULL.
        * @param node_to_insert the node to be inserted. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_node_ops_insert_sibling_before)
            (axis2_environment_t * environment,
             struct axis2_om_node * current_node,
             struct axis2_om_node * node_to_insert);

      /**
        * Sets a parent node to a given node
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param child_node child node to whom a parent to be added. Mandatory, cannot be NULL.
        * @param parent the node that will be set as parent. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_node_ops_set_parent) (axis2_environment_t
                                                          * environment,
                                                          struct axis2_om_node
                                                          * child_node,
                                                          struct axis2_om_node
                                                          * parent_node);

       /** 
        * gets the first child of a given node
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param parent_node node whose first child is to be returnd.
        * @return a pointer to first child if there is one, else returns NULL.
        *           On error sets the error and returns NULL
        */
        struct axis2_om_node
            *(AXIS2_CALL *axis2_om_node_ops_get_first_child) (axis2_environment_t *
                                                   environment,
                                                   struct axis2_om_node *
                                                   parent_node);
                                                   
       /**
        * Gets the next child of the given node.
        * This function should only be called after a call to get_first_child function
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param parent_node node whose next child is to be returned.
        *  @return a pointer to next child if there is one, else returns NULL.
        *           On error sets the error and returns NULL
        */
        struct axis2_om_node
            *(AXIS2_CALL *axis2_om_node_ops_get_next_child) (axis2_environment_t *
                                                  environment,
                                                  struct axis2_om_node *
                                                  parent_node);

       /**
        * Serializes the given node. This operation makes the node go through its children and 
        * serialize them in order.
        * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.    
        * @param node node to be serialized. Mandatory cannot be NULL.
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_node_ops_serialize) (axis2_environment_t *
                                                         environment,
                                                         struct axis2_om_node
                                                         * om_node,
                                                         struct axis2_om_output *
                                                         om_output);


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
        /* links that maintain the tree */
        /** parent node */
        struct axis2_om_node *parent;
        /** previous sibling */
        struct axis2_om_node *prev_sibling;
        /** next sibling */
        struct axis2_om_node *next_sibling;
        /** first child */
        struct axis2_om_node *first_child;
        /** current child */
        struct axis2_om_node *current_child;
        /** last child */
        struct axis2_om_node *last_child;
        /** node type, indicates the type stored in data_element */
        axis2_om_types_t node_type;
        /** done true means that this node is completely built , false otherwise */
        int done;
        /** instances of an om struct, whose type is defined by node type */
        void *data_element;
    } axis2_om_node_t;

  /**
    * Creates a node struct.
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @return a pointer to newly created node struct. NULL on error. 
    */
    AXIS2_DECLARE(axis2_om_node_t *) axis2_om_node_create (axis2_environment_t * environment);

/** frees given node */
#define axis2_om_node_free(environment,om_node) ((om_node)->ops->axis2_om_node_ops_free(environment,om_node))
/** adds given child to the given parent */
#define axis2_om_node_add_child(environment,parent,child) ((parent)->ops->axis2_om_node_ops_add_child(environment,parent,child))
/** detaches the given node from its parent */
#define axis2_om_node_detach(environment,om_node) ((om_node)->ops->axis2_om_node_ops_detach(environment,om_node))
/** inserts a sibling after the given node */
#define axis2_om_node_insert_sibling_after(environment,current_node,node_to_insert) \
        ((current_node)->ops->axis2_om_node_ops_insert_sibling_after(environment,current_node,node_to_insert))
/** inserts a sibling before the given node */
#define axis2_om_node_insert_sibling_before(environment,current_node,node_to_insert) ((current_node)->ops->axis2_om_node_ops_insert_sibling_before(environment,current_node,node_to_insert))
/** sets the parent of the given node */
#define axis2_om_node_set_parent(environment,child_node,parent_node) ((child_node)->ops->axis2_om_node_ops_set_parent(environment, child_node,parent_node))
/** gets the first child of the given node */
#define axis2_om_node_get_first_child(environment, om_node) ((om_node)->ops->axis2_om_node_ops_get_first_child(environment, om_node))
/** gets the next child of the given node */
#define axis2_om_node_get_next_child(environment, om_node) ((om_node)->ops->axis2_om_node_ops_get_next_child(environment, om_node))
/** serializes the given node */
#define axis2_om_node_serialize(environment, om_node, om_output) ((om_node)->ops->axis2_om_node_ops_serialize(environment, om_node, om_output))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_NODE_H */
