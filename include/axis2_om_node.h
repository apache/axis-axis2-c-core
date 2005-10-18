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

/**
 * @file axis2_om_node.h
 * @brief defines axis2_om_node_t struct and its operations
 */
#include <axis2_environment.h>

struct axis2_om_node;
struct axis2_om_node_ops;
typedef struct axis2_om_output axis2_om_output_t;


/** 
 *   Types used in OM 
 */


typedef enum axis2_om_types_t {
    AXIS2_OM_INVALID = 0,
	AXIS2_OM_DOCUMENT,
	AXIS2_OM_ELEMENT,
	AXIS2_OM_DOCTYPE,
	AXIS2_OM_COMMENT,
	AXIS2_OM_ATTRIBUTE,
	AXIS2_OM_NAMESPACE,
	AXIS2_OM_PROCESSING_INSTRUCTION,
	AXIS2_OM_TEXT
} axis2_om_types_t;


/**
 *  Encapsulator struct of axis2_om_node operations
 */    
 
typedef struct axis2_om_node_ops
{
   /**
    *   Free an om node and its all children
    *   @return status code
    */
    axis2_status_t (*axis2_om_node_ops_free)(axis2_environment_t *environment,
        struct axis2_om_node *om_node);
   /**
    *   Add child node as child to parent
    *   @param parent
    *   @param child  
    *   @return status code   
    */
    axis2_status_t (*axis2_om_node_ops_add_child)(axis2_environment_t *environment,
        struct axis2_om_node *parent,struct axis2_om_node *child);
    
    /**
     *  detach this node from the node and reset the links
     *  @return a pointer to detached node 
     */
    struct axis2_om_node *(*axis2_om_node_ops_detach)(axis2_environment_t *environment,
        struct axis2_om_node *om_node);
    
   /**
    *  Inserts a sibling node after the current node
    *  @param current_node  
    *  @param node_to_insert the node that will be inserted 
    *  @return return status code 
    */
    axis2_status_t (*axis2_om_node_ops_insert_sibling_after)(axis2_environment_t *environment,
        struct axis2_om_node *current_node,struct axis2_om_node *node_to_insert);
    
   /**
    *	Inserts a sibling node after the current node
    *	@param current_node   
    *  @param node_to_insert the node that will be inserted 
    *  @return status code
    */

    axis2_status_t (*axis2_om_node_ops_insert_sibling_before)(axis2_environment_t *environment,
        struct axis2_om_node *current_node,struct axis2_om_node *node_to_insert);
        
   /**
    * set a parent node to a given node
    * @param child_node  
    * @param parent the node that will be set as parent
    * @return status code
    */

    axis2_status_t (*axis2_om_node_ops_set_parent)(axis2_environment_t *environment,
        struct axis2_om_node *child_node,struct axis2_om_node *parent_node);
   
   /** get the first child of a node
    *  returns the first child node of this node
    * @param 
    *  @return returns a pointer to first child if there is no child returns null
    */
   struct axis2_om_node *(*axis2_om_node_ops_get_first_child)(axis2_environment_t *environment,
        struct axis2_om_node *parent_node);
   /**
    * get the next child of this node
    * This function should only be called after a call to get_first_child function
    *  @param parent_node
    *  @return pointer to next child , if there isn't next child returns null
    */
    struct axis2_om_node *(*axis2_om_node_ops_get_next_child)(axis2_environment_t *environment,
        struct axis2_om_node *parent_node);
        
   /**
    *   serialize operation of node
    *   @returns status code
    */
    
    axis2_status_t (*axis2_om_node_ops_serialize)(axis2_environment_t *environment,
        struct axis2_om_node *om_node, axis2_om_output_t * om_output);


}axis2_om_node_ops_t;


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

typedef struct axis2_om_node{
    /** operations of this struct */
    axis2_om_node_ops_t *ops;
    /** links that maintain the tree */
	struct axis2_om_node *parent;
	struct axis2_om_node *prev_sibling;
	struct axis2_om_node *next_sibling;
	struct axis2_om_node *first_child;
	struct axis2_om_node *current_child;
	struct axis2_om_node *last_child;
	
	/** indicate the type stored in data_element */
	axis2_om_types_t node_type;
	/** done true means that this node is completely built , false otherwise */
	int done;
	
	/** instances of om struct types will be stored here                    */
	void* data_element;
} axis2_om_node_t;

/**
 * Create a node struct.
 * @return a pointer to node struct instance null otherwise
 */

axis2_om_node_t *axis2_om_node_create(axis2_environment_t *environment);

#define axis2_om_node_free(environment,om_node) ((om_node)->ops->axis2_om_node_ops_free(environment,om_node))
#define axis2_om_node_add_child(environment,parent,child) ((parent)->ops->axis2_om_node_ops_add_child(environment,parent,child))
#define axis2_om_node_detach(environment,om_node) ((om_node)->ops->axis2_om_node_ops_detach(environment,om_node))
#define axis2_om_node_insert_sibling_after(environment,current_node,node_to_insert) \
        ((current_node)->ops->axis2_om_node_ops_insert_sibling_after(environment,current_node,node_to_insert))

#define axis2_om_node_insert_sibling_before(environment,current_node,node_to_insert) ((current_node)->ops->axis2_om_node_ops_insert_sibling_before(environment,current_node,node_to_insert))
#define axis2_om_node_set_parent(environment,child_node,parent_node) ((child_node)->ops->axis2_om_node_ops_set_parent(environment,child_node,parent_node))
#define axis2_om_node_get_first_child(environment,om_node) ((om_node)->ops->axis2_om_node_get_ops_first_child(environment,om_node))
#define axis2_om_node_get_next_child(environment,om_node) ((om_node)->ops->axis2_om_node_get_ops_next_child(environemt,om_node))
#define axis2_om_node_serialize(environment,om_node,om_output) ((om_node)->ops->axis2_om_node_ops_serialize(environment,om_node,om_output))




#endif /* AXIS2_OM_NODE_H */
