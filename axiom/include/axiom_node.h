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

#ifndef AXIOM_NODE_H
#define AXIOM_NODE_H

/** @defgroup axiom AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @file axiom_node.h
 * @brief defines axiom_node struct and its ops
 */
#include <axis2_env.h>
#include <axis2_utils.h>


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_node axiom_node_t;
    struct axiom_node_ops;
    struct axiom_output;
    struct axiom_document;
    struct axiom_stax_builder;
/**
 * @defgroup axiom_node  OM Node
 * @ingroup axiom 
 * @{
 */

  /** 
    * @brief OM types
    */
    typedef enum axiom_types_t
    {
        /** Invalid node type */
        AXIOM_INVALID = 0,
        /** OM document type */
        AXIOM_DOCUMENT,
        /** OM element type */
        AXIOM_ELEMENT,
        /** OM doctype type */
        AXIOM_DOCTYPE,
        /** OM comment type */
        AXIOM_COMMENT,
        /** OM attribute type */
        AXIOM_ATTRIBUTE,
        /** OM namespace type */
        AXIOM_NAMESPACE,
        /** OM processing instruction type */
        AXIOM_PROCESSING_INSTRUCTION,
        /** OM text type */
        AXIOM_TEXT
    } axiom_types_t;

  /** 
    * @brief OM Node ops struct
    * Encapsulator struct for ops of axiom_node
    */
AXIS2_DECLARE_DATA   typedef struct axiom_node_ops
{
    /**
    * Frees an om node and all of its children
    * @param om_node node to be freed.
    * @param env Environment. MUST NOT be NULL, .
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free) (axiom_node_t *om_node,
           const axis2_env_t *env);
    /**
    * Adds given node as child to parent. child should not have a parent
    * if child has a parent it will be detached from existing parent
    * @param om_node parent node. cannot be NULL.
    * @param env Environment. MUST NOT be NULL, .
    * @param child child node.
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    add_child) (axiom_node_t* om_node,
                const axis2_env_t *env,
                axiom_node_t *child);

    /**
    * Detaches given node from the parent and reset the links
    * @param om_node node to be detached, cannot be NULL.
    * @param env Environment. MUST NOT be NULL, .
    * @return a pointer to detached node,returns NULL on error with error
    *           code set to environment's error struct
    */
    axiom_node_t  *(AXIS2_CALL *
    detach) (axiom_node_t  *om_node,
             const axis2_env_t *env);

    /**
    * Inserts a sibling node after the given node
    * @param om_node node to whom the sibling to be inserted. , cannot be NULL.
    * @param env Environment. MUST NOT be NULL, .
    * @param node_to_insert the node to be inserted. , cannot be NULL.
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    insert_sibling_after)(axiom_node_t  *om_node,
                          const axis2_env_t *env,
                          axiom_node_t  * node_to_insert);

    /**
    * Inserts a sibling node before the given current node
    * @param om_node node to whom the sibling to be inserted. , cannot be NULL.
    * @param env Environment. MUST NOT be NULL, .
    * @param node_to_insert the node to be inserted. , cannot be NULL.
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    insert_sibling_before)(axiom_node_t  *om_node,
                           const axis2_env_t *env,
                           axiom_node_t  * node_to_insert);

    /**
    * Serializes the given node. This op makes the node go
    * through its children and  serialize them in order.
    * @param om_node node to be serialized.  cannot be NULL.
    * @param env Environment .MUST NOT be NULL.    
    * @param om_output OM output handler to be used in serializing
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    serialize)(axiom_node_t  * om_node,
               const axis2_env_t *env,
              struct axiom_output *om_output);
              
    /** get parent of om_node
    *@om_node node 
    *@param env environment
    *@return pointer to parent node of om_node, return NULL if no parent exists or
    *        when an error occured.
    */                                    

    axiom_node_t * (AXIS2_CALL *
    get_parent)(axiom_node_t  *om_node,
                const axis2_env_t *env);

    /**
    * get the first child of om_node
    * @param om_node node
    * @param env environment must not be null.
    * @returns pointer to first child node , NULL is returned on error with 
    *                     error code set in environments error
    */
    axiom_node_t * (AXIS2_CALL *
    get_first_child)(axiom_node_t  *om_node,
                     const axis2_env_t *env);
    /**
    * get the last child
    * @param om_node node
    * @param env environment, MUST NOT be NULL
    * @return pointer to last child of this node , return NULL on error.
    */                                                   
    axiom_node_t * (AXIS2_CALL *
    get_last_child)(axiom_node_t  *om_node,
                    const axis2_env_t *env);
    /**
    * get the previous sibling 
    * @param om_node om_node struct 
    * @param env environment , must node be null
    * @returns a pointer to previous sibling , NULL if a previous sibling does not exits
    *                (happens when this node is the first child of a node )
    */                                                            
    axiom_node_t * (AXIS2_CALL *
    get_previous_sibling)(axiom_node_t  *om_node,
                          const axis2_env_t *env);
    /**
    * get next sibling
    * @param om_node om_node struct
    * @param env environment, MUST NOT be NULL.
    * @return next sibling of this node.
    */ 
    axiom_node_t * (AXIS2_CALL *
    get_next_sibling)(axiom_node_t  *om_node,
                      const axis2_env_t *env);
    /**
    * get the node type of this element
    * Node type can be one of AXIOM_ELEMENT, AXIOM_COMMENT, AXIOM_TEXT
    *      AXIOM_DOCTYPE, AXIOM_PROCESSING_INSTRUCTION
    * @param om_node  node of which node type is required
    * @param env environment
    * @return node type
    */
    axiom_types_t (AXIS2_CALL *
    get_node_type)(axiom_node_t  *om_node,
                   const axis2_env_t *env);
    /**
    * get the struct contained in the node
    * IF the node is on type AXIOM_ELEMENT , this method returns
    * a pointer to  axiom_element_t struct contained
    * @param om_node node
    * @param env environment, MUST NOT be NULL.
    * @returns pointer to struct contained in the node
    *          returns NULL if no struct is contained 
    */                                                
    void* (AXIS2_CALL *
    get_data_element)(axiom_node_t  *om_node,
                      const axis2_env_t *env);    
    /**
    * Indicates whether parser has parsed this information item completely or not 
    * @param om_node om_node struct
    * @param env environment, MUST NOT be NULL.
    * @returns AXIS2_TRUE if node is completly build, 
    *          AXIS2_FALSE if node is not completed
    */                                                
    axis2_bool_t (AXIS2_CALL *
    is_complete)(axiom_node_t  *om_node,
                      const axis2_env_t *env); 
   /**
    * returns the associated document,
    * only valid if built using builder and for a node of type 
    * AXIOM_ELEMENT
    * returns null if no document is available
    * @param om_node 
    * @param env environment, MUST NOT be NULL.
    */   
    struct axiom_document* (AXIS2_CALL *
    get_document)(axiom_node_t *om_node,
                  const axis2_env_t *env);
                  
    axis2_char_t* (AXIS2_CALL *
    to_string)(axiom_node_t *om_node,
               const axis2_env_t *env);                  
                                                                        
} axiom_node_ops_t;


/**
* This is the structure that defines a node in om tree 
*/

    struct axiom_node
    {
        /** ops of node struct */
        axiom_node_ops_t *ops;
               
    };

  /**
    * Creates a node struct.
    * @param env Environment. MUST NOT be NULL, .
    * @return a pointer to newly created node struct. NULL on error. 
    */
AXIS2_EXTERN axiom_node_t * AXIS2_CALL 
axiom_node_create (const axis2_env_t *env);

/** frees given node */
#define AXIOM_NODE_FREE_TREE(om_node,env) \
        ((om_node)->ops->free(om_node,env))
/** adds given child to the given parent */
#define AXIOM_NODE_ADD_CHILD(om_node, env, child) \
        ((om_node)->ops->add_child(om_node, env, child))
/** detaches the given node from its parent */
#define AXIOM_NODE_DETACH(om_node,env) \
        ((om_node)->ops->detach(om_node,env))
/** inserts a sibling after the given node */
#define AXIOM_NODE_INSERT_SIBLING_AFTER(om_node,env,node_to_insert) \
        ((om_node)->ops->insert_sibling_after(om_node,env,node_to_insert))
/** inserts a sibling before the given node */
#define AXIOM_NODE_INSERT_SIBLING_BEFORE(om_node,env,node_to_insert) \
        ((om_node)->ops->insert_sibling_before(om_node,env,node_to_insert))
/** serialize an om_node and all its children */
#define AXIOM_NODE_SERIALIZE(om_node,env, om_output) \
        ((om_node)->ops->serialize(om_node,env,om_output))
/** returns parent of this om_node */   
#define AXIOM_NODE_GET_PARENT(om_node,env) \
        ((om_node)->ops->get_parent(om_node,env))
/** get firet child of this om_node  */
#define AXIOM_NODE_GET_FIRST_CHILD(om_node,env) \
        ((om_node)->ops->get_first_child(om_node,env))
/**get last child  */  
#define AXIOM_NODE_GET_LAST_CHILD(om_node,env) \
        ((om_node)->ops->get_last_child(om_node,env))
/** get next sibling */  
#define AXIOM_NODE_GET_NEXT_SIBLING(om_node,env) \
        ((om_node)->ops->get_next_sibling(om_node,env))
/** get previous sibling */
#define AXIOM_NODE_GET_PREVIOUS_SIBLING(om_node,env) \
        ((om_node)->ops->get_previous_sibling(om_node,env))
/** get build status */

#define AXIOM_NODE_IS_COMPLETE(om_node,env) \
        ((om_node)->ops->is_complete(om_node,env))
/** get data element of this node can be om_element om_text etc */
#define AXIOM_NODE_GET_DATA_ELEMENT(om_node,env) \
        ((om_node)->ops->get_data_element(om_node,env))
/** node type one of om_node_types */
#define AXIOM_NODE_GET_NODE_TYPE(om_node,env) \
        ((om_node)->ops->get_node_type(om_node,env))       
/** get document */        
#define AXIOM_NODE_GET_DOCUMENT(om_node, env) \
        ((om_node)->ops->get_document(om_node, env))

#define AXIOM_NODE_TO_STRING(om_node, env) \
        ((om_node)->ops->to_string(om_node, env))        
/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_NODE_H */
