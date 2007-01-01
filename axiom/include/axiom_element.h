/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIOM_ELEMENT_H
#define AXIOM_ELEMENT_H

#include <axiom_namespace.h>
#include <axiom_attribute.h>
#include <axiom_output.h>
#include <axiom_node.h>
#include <axiom_children_iterator.h>
#include <axiom_children_qname_iterator.h>
#include <axiom_child_element_iterator.h>
#include <axis2_hash.h>
#include <axis2_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_element axiom_element_t;
    typedef struct axiom_element_ops axiom_element_ops_t;

/**
 * @defgroup axiom_element element
 * @ingroup axiom_om
 * @{
 */


    /** 
    * @brief element ops struct
    * Encapsulator struct for ops of axiom_element
    */
    struct axiom_element_ops
    {
        int dummy; /* if this is missing, windows compiler goes crazy */
    };

    
  /**
    * \brief element struct
    * Handles the XML element in AXIOM
    */
    struct axiom_element
    {
        /** ops of attribute struct */
        const axiom_element_ops_t *ops;
    };

  /**
    * Creates an AXIOM element with given local name
    * @param env Environment. MUST NOT be NULL.
    * @param parent parent of the element node to be created. can be NULL.
    * @param localname local name of the elment. cannot be NULL.
    * @param ns namespace of the element.  can be NULL.
    * @param node This is an out parameter. cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIOM_ELEMENT
    * @return a pointer to the newly created element struct
    */
    AXIS2_EXTERN axiom_element_t * AXIS2_CALL
    axiom_element_create (const axis2_env_t *env,
                             axiom_node_t *parent,
                             const axis2_char_t *localname,
                             axiom_namespace_t *ns,
                             axiom_node_t **node);

  /**
    * Creates an AXIOM element with given qname
    * @param env Environment. MUST NOT be NULL.
    * @param parent parent of the element node to be created. can be NULL.
    * @param qname qname of the elment.cannot be NULL.
    * @param node This is an out parameter. cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIOM_ELEMENT
    * @return a pointer to the newly created element struct
    */
    AXIS2_EXTERN axiom_element_t * AXIS2_CALL 
    axiom_element_create_with_qname (const axis2_env_t *env,
                                        axiom_node_t *parent,
                                        const axis2_qname_t *qname,
                                        axiom_node_t ** node);
   /*
    * Find a namespace in the scope of the document.
    * Start to find from the given node and go up the hierarchy.
    * @param om_element pointer to om_element_struct contained in
    *        node , 
    * @param env Environment. MUST NOT be NULL.
    * @param node node containing an instance of an AXIOM element,cannot be NULL.
    * @param uri namespace uri..
    * @param prefix namespace prefix. can be NULL.
    * @return pointer to the namespace, if found, else NULL. On error, returns 
    *           NULL and sets error code in environment,s error
    */
    AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL 
    axiom_element_find_namespace(axiom_element_t *om_element,
                    const axis2_env_t *env,
                    axiom_node_t *node,
                    const axis2_char_t *uri,
                    const axis2_char_t *prefix);
  /**
    * Declare a namespace in current element (in the scope of this element ).
    * It checks to see if it is already declared.
    * @param om_element contained in the om node struct
    * @param env Environment. MUST NOT be NULL.
    * @param node node containing an instance of an AXIOM element.
    * @param ns pointer to the namespace struct to be declared
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_declare_namespace(axiom_element_t *om_element,
                       const axis2_env_t *env,
                       axiom_node_t *node,
                       axiom_namespace_t * ns);
   /**
    * Finds a namespace using qname
    * Start to find from the given node and go up the hierarchy.
    * @param om_element om_element contained in node
    * @param env Environment. MUST NOT be NULL.
    * @param node node containing an instance of an AXIOM element, cannot be NULL.                                        
    * @param qname qname of the namespace to be found. cannot be NULL.
    * @return pointer to the namespace, if found, else NULL. On error, returns 
    *           NULL and sets the error code in environment's error struct.
    */
     AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL 
     axiom_element_find_namespace_with_qname(axiom_element_t *om_element,
                                const axis2_env_t *env,
                                axiom_node_t *node,
                                axis2_qname_t *qname);

  /**
    * Adds an attribute to current element
    * @param om_element element to which the attribute is to be added.cannot be NULL.
    * @param env Environment. MUST NOT be NULL.
    * @param attribute attribute to be added.
    * @param node axiom_node_t node that om_element is contained in
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
    */
     AXIS2_EXTERN axis2_status_t AXIS2_CALL 
     axiom_element_add_attribute(axiom_element_t *om_element,
                    const axis2_env_t *env,
                    axiom_attribute_t *attribute,
                    axiom_node_t *node);

  /**
    * Gets (finds) the attribute with the given qname
    * @param element element whose attribute is to be found. 
    * @param env Environment. MUST NOT be NULL.
    * @qname qname qname of the attribute to be found. should not be NULL.
    * @return a pointer to the attribute with given qname if found, else NULL.
    *           On error, returns NULL and sets the error code in environment's error struct.
    */
     AXIS2_EXTERN axiom_attribute_t *AXIS2_CALL 
     axiom_element_get_attribute (axiom_element_t *om_element,
                     const axis2_env_t *env,
                     axis2_qname_t * qname);
                     
    /**
    * Gets (finds) the attribute value with the given qname
    * @param element element whose attribute is to be found. 
    * @param env Environment. MUST NOT be NULL.
    * @qname qname qname of the attribute to be found. should not be NULL.
    * @return the attribute value with given qname if found, else NULL.
    *  On error, returns NULL and sets the error code in environment's error struct.
    */
     AXIS2_EXTERN axis2_char_t * AXIS2_CALL 
     axiom_element_get_attribute_value (axiom_element_t *om_element,
                           const axis2_env_t *env,
                           axis2_qname_t * qname);                         
    
  /**
    * Frees given element 
    * @param element AXIOM element to be freed.
    * @param env Environment. MUST NOT be NULL.
     * @return satus of the op. AXIS2_SUCCESS on success ,AXIS2_FAILURE on error.
    */
     AXIS2_EXTERN axis2_status_t AXIS2_CALL 
     axiom_element_free (axiom_element_t *element,
            const axis2_env_t *env);

  /**
    * Serializes the start part of the given element
    * @param element element to be serialized.
    * @param env Environment. MUST NOT be NULL.
    * @param om_output AXIOM output handler to be used in serializing
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_serialize_start_part(axiom_element_t *om_element,
                          const axis2_env_t *env,
                          axiom_output_t * om_output,
                          axiom_node_t *ele_node);

  /**
    * Serializes the end part of the given element. serialize_start_part must 
    *     have been called before calling this method.
    * @param om_element element to be serialized.
    * @param env Environment must not be null.
    * @param om_output AXIOM output handler to be used in serializing
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_serialize_end_part(axiom_element_t *om_element,
                        const axis2_env_t *env,
                        axiom_output_t * om_output);
   /**
    *  finds a namespace in current elements scope
    * @param om_element
    * @param env environemt must not be null
    * @param uri namespace uri
    * @param prefix prefix 
    * @return axiom_namespace_t if found, else return NULL
    */ 
    
    AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL 
    axiom_element_find_declared_namespace(axiom_element_t *om_element,
                             const axis2_env_t *env,
                             const axis2_char_t *uri,
                             const axis2_char_t *prefix);
                                                         
                                                    
   /**
    * returns the localname of this element
    * @param om_element om_element struct
    * @param env environment struct
    * @returns localname of element, returns NULL on error.
    */        
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axiom_element_get_localname(axiom_element_t *om_element,
                   const axis2_env_t *env);
   /**
    * set the localname of this element
    * @param om_element om_element_struct to with localname is set
    * @param env environment struct
    * @localname text value to be set as localname 
    * @returns status code of op, AXIS2_SUCCESS on success,
    *                   AXIS2_FAILURE on error.
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_set_localname(axiom_element_t *om_element,
                   const axis2_env_t *env,
                   const axis2_char_t *localname);
    /**
     * get the namespace  of om_element 
     * @param om_element om_element struct
     * @param env environemt, MUST NOT be NULL.
     * @returns pointer to axiom_namespace_t struct 
     *          NULL if there is no namespace associated with the element,
     *          NULL on error with error code set to environment's error
     */
    AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL 
    axiom_element_get_namespace(axiom_element_t *om_element,
                   const axis2_env_t *env,
                   axiom_node_t *ele_node);
   /**
    * set the namespace of the element
    * @param om_element Om_element struct
    * @param env environment must not be null
    * @param ns pointer to namespace
    * @returns status code of the op , NULL on error with error code 
    *                  set to environment's error
    */                                         
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_set_namespace(axiom_element_t *om_element,
                   const axis2_env_t *env,
                   axiom_namespace_t *ns,
                   axiom_node_t *node);
    /**
     * get  the attribute list of the element 
     * @param om_element om element
     * @param env environment must not be null
     * @returns axis2_hash poiner to attributes hash
     * This hash table is read only 
     */
    AXIS2_EXTERN axis2_hash_t* AXIS2_CALL 
    axiom_element_get_all_attributes(axiom_element_t *om_element,
                        const axis2_env_t *env);
    /**
     * get the namespace list of the element
     * @param om_element om element
     * @param env environment
     * @returns axis2_hash pointer to namespaces hash
     * this hash table is read only
     */                                             
    AXIS2_EXTERN axis2_hash_t* AXIS2_CALL 
    axiom_element_get_namespaces(axiom_element_t *om_element,
                    const axis2_env_t *env);
    /**
     *@return qname of this element
     * the returned qname should not be externaly freed
     * when om_element struct is freed qname is also
     * freed 
     * @returns axis2_qname_t struct , NULL on failure
     */
    AXIS2_EXTERN axis2_qname_t* AXIS2_CALL 
    axiom_element_get_qname(axiom_element_t *om_element,
               const axis2_env_t *env,
               axiom_node_t *ele_node);
    
   /**
    *   returns a list of children iterator
    *   iterators free function must be called by user
    */        
    AXIS2_EXTERN axiom_children_iterator_t* AXIS2_CALL 
    axiom_element_get_children(axiom_element_t *om_element,
                  const axis2_env_t *env,
                 axiom_node_t *element_node);
                                  
   /**
    *   returns a list of children iterator with qname
    *   returned iterator is freed when om element struct
    *  is freed 
    * @param om_element 
    * @param env 
    * @param element_qname 
    * @param element_node 
    * @returns children qname iterator struct
    */        
                                            
    AXIS2_EXTERN axiom_children_qname_iterator_t * AXIS2_CALL 
    axiom_element_get_children_with_qname(axiom_element_t *om_element,
                             const axis2_env_t *env,
                             axis2_qname_t *element_qname,
                             axiom_node_t* element_node);
    /**
     * returns the om_element corresponding to element_qname
     */                            
    
    AXIS2_EXTERN axiom_element_t* AXIS2_CALL 
    axiom_element_get_first_child_with_qname(axiom_element_t *om_element,
                                 const axis2_env_t *env,
                                 axis2_qname_t *element_qname,
                                 axiom_node_t* element_node,
                                 axiom_node_t **child_node);                       
    /**
     * removes an attribute from the element attribute list
     * user must free this attribute, element free function does not free 
     * attributes that are not is it's attribute list
     * @param om_element
     * @param env
     * @param om_attribute attribute to be removed
     * @return AXIS2_SUCCESS if attribute was found and removed, else 
     *           AXIS2_FAILURE
     */         
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_remove_attribute(axiom_element_t *om_element,
                      const axis2_env_t *env,
                      axiom_attribute_t *om_attribute); 
    /**
     * Sets the text of the given element.
     *caution - This method will wipe out all the text elements (and hence any
     * mixed content) before setting the text
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_element_set_text(axiom_element_t *om_element,
              const axis2_env_t *env,
              const axis2_char_t *text,
              axiom_node_t *element_node); 
    /**
     * select all the text children and concat them to a single string
     * @param element node , the container node of this om element
     * @return the contanated text of all text childrens text values
     *         return null if no text children is avilable or on error
     */                                              
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axiom_element_get_text(axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *element_node);

    /**
     * returns the first child om element of this om element node
     * @param element_node the container node of this om_element
     * @return om_element if one is availble otherwise return NULL
     */
    AXIS2_EXTERN axiom_element_t* AXIS2_CALL 
    axiom_element_get_first_element(
            axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *element_node,
            axiom_node_t **first_element_node);  
    /**
     * returns the serilized text of this element and its children
     * @param element_node the container node this on element is contained 
     * @return a char array of xml , returns NULL on error
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axiom_element_to_string(axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *element_node);            
    /** 
     * returns an iterator with child elements of type AXIOM_ELEMENT
     * iterator is freed when om_element node is freed
     * @param om_element
     * @param element_node
     * @param env enviroment must not be null
     * @returns axiom_child_element_iterator_t , NULL on error                                             
     */
     AXIS2_EXTERN axiom_child_element_iterator_t* AXIS2_CALL 
     axiom_element_get_child_elements(
            axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *element_node);                                                                            
    /**
     * builds this om_element_node completely, This is only possible 
     * if the om_stax_builder is associated with the om_element_node,
     * @param om_element om_element struct
     * @param env environment
     * @param element_node corresponding om element node of this om element
     * struct 
     * @returns AXIS2_SUCCESS if this element node was successfully completed,
     * otherwise returns AXIS2_FAILURE
     */
     AXIS2_EXTERN axis2_status_t AXIS2_CALL 
     axiom_element_build(axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *element_node);
            
    /** 
     * retrieves the default namespace of this element , if available,
     * @param om_element pointer to om element
     * @param env axis2_environment MUST Not be NULL
     * @returns pointer to default namespace if availale , NULL otherwise
     */
     AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL 
     axiom_element_get_default_namespace(
            axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *element_node);
                            
    /**
     * declared a default namespace explicitly 
     * @param om_element pointer to om element
     * @param env environment MUST not be NULL
     * @param uri namespace uri of the default namespace
     * @returns the declared namespace
                                                 */
     AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL 
     axiom_element_declare_default_namespace(axiom_element_t *om_element,
            const axis2_env_t *env,
            axis2_char_t *uri);
            
    /** 
     * checks for the namespace in the context of this element 
     * with the given prefix 
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_element_node pointer to this element node
     * @returns pointer to relevent namespace 
     */
     
     AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL 
     axiom_element_find_namespace_uri(axiom_element_t *om_element,
            const axis2_env_t *env,
            axis2_char_t *prefix,
            axiom_node_t *element_node);  
    /**
     *This will not search the namespace in the scope nor will 
     * declare in the current element, as in set_namespace. This will
     * just assign the given namespace to the element.
     * @param om_ns pointer to namespace to be set
     * @returns 
     */
     AXIS2_EXTERN axis2_status_t AXIS2_CALL 
     axiom_element_set_namespace_with_no_find_in_current_scope(axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_namespace_t *om_ns); 
            
    /**
     *  Extract attributes , returns a clones hash table of attributes,
     *  if attributes are associated with a namespace it is also cloned
     */                                                       
     AXIS2_EXTERN axis2_hash_t* AXIS2_CALL 
     axiom_element_extract_attributes(axiom_element_t *om_element,
            const axis2_env_t *env,
            axiom_node_t *ele_node);
            
     AXIS2_EXTERN axis2_char_t* AXIS2_CALL
     axiom_element_get_attribute_value_by_name(axiom_element_t *om_ele,
            const axis2_env_t *env,
            axis2_char_t *attr_name);

/** finds given namespace with respect to given node */
#define AXIOM_ELEMENT_FIND_NAMESPACE(om_element,env,om_node, uri, prefix) \
        axiom_element_find_namespace (om_element, env,om_node, uri, prefix)
        
/** declares given namespace with respect to the given node */
#define AXIOM_ELEMENT_DECLARE_NAMESPACE(om_element, env , om_node, ns) \
        axiom_element_declare_namespace (om_element, env, om_node, ns)

/** finds namespace with given qname */
#define AXIOM_ELEMENT_FIND_NAMESPACE_WITH_QNAME (om_element, env, om_node, qname) \
        axiom_element_find_namespace_with_qname (om_element, environment, om_node, qname)
        
/** adds given attribute to given element */
#define AXIOM_ELEMENT_ADD_ATTRIBUTE(om_element, env, attribute, element_node) \
        axiom_element_add_attribute(om_element , env, attribute, element_node)
        
/** gets (finds) given attribute in given element */
#define AXIOM_ELEMENT_GET_ATTRIBUTE(om_element, env, qname) \
        axiom_element_get_attribute(om_element, env, qname)
        
/** free an om element */
#define AXIOM_ELEMENT_FREE(om_element, env) \
        axiom_element_free(om_element, env)
        
/** serialize the start part of given element */
#define AXIOM_ELEMENT_SERIALIZE_START_PART(om_element, env, om_output, ele_node) \
        axiom_element_serialize_start_part(om_element, env, om_output, ele_node)
        
/** serialize the end part of given element */
#define AXIOM_ELEMENT_SERIALIZE_END_PART(om_element, env, om_output) \
        axiom_element_serialize_end_part(om_element, env, om_output)
/** get localname */
#define AXIOM_ELEMENT_GET_LOCALNAME(om_element, env)\
        axiom_element_get_localname(om_element,env)
/** get namespace of this element */        
#define AXIOM_ELEMENT_GET_NAMESPACE(om_element, env, ele_node) \
        axiom_element_get_namespace(om_element, env, ele_node)
/** set localname */        
#define AXIOM_ELEMENT_SET_LOCALNAME(om_element, env, localname) \
        axiom_element_set_localname(om_element, env, localname)
/** set namespace */        
#define AXIOM_ELEMENT_SET_NAMESPACE(om_element, env, ns , node) \
        axiom_element_set_namespace(om_element, env, ns, node)

#define AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(om_element, env, uri, prefix) \
        axiom_element_find_declared_namespace(om_element, env, uri, prefix)        
        
#define AXIOM_ELEMENT_GET_QNAME(om_element, env, ele_node) \
        axiom_element_get_qname(om_element, env, ele_node)

#define AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(om_element, env) \
        axiom_element_get_all_attributes(om_element, env)

#define AXIOM_ELEMENT_GET_NAMESPACES(om_element, env) \
        axiom_element_get_namespaces(om_element, env) 
        
#define AXIOM_ELEMENT_GET_CHILDREN(om_element, env, element_node) \
        axiom_element_get_children(om_element, env, element_node)

#define AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(om_element, env, \
            element_qname, element_node) \
        axiom_element_get_children_with_qname(om_element,\
            env, element_qname, element_node)

#define AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(om_element, env,\
            element_qname, element_node , child_node) \
        axiom_element_get_first_child_with_qname(om_element, env, \
            element_qname, element_node, child_node)

#define AXIOM_ELEMENT_REMOVE_ATTRIBUTE(om_element, env, om_attribute) \
        axiom_element_remove_attribute(om_element, env, om_attribute)

#define AXIOM_ELEMENT_GET_FIRST_ELEMENT(om_element, env, element_node, first_node) \
        axiom_element_get_first_element(om_element, env, element_node, first_node)

#define AXIOM_ELEMENT_GET_TEXT(om_element, env, element_node) \
        axiom_element_get_text(om_element, env, element_node)

#define AXIOM_ELEMENT_SET_TEXT(om_element, env, text, element_node) \
        axiom_element_set_text(om_element, env, text, element_node)

#define AXIOM_ELEMENT_TO_STRING(om_element, env, element_node) \
        axiom_element_to_string(om_element, env, element_node) 
               
#define AXIOM_ELEMENT_GET_CHILD_ELEMENTS(om_element, env, element_node) \
        axiom_element_get_child_elements(om_element, env, element_node)

#define AXIOM_ELEMENT_BUILD(om_element, env, element_node) \
        axiom_element_build(om_element, env, element_node)
        
#define AXIOM_ELEMENT_GET_DEFAULT_NAMESPACE(om_element, env, element_node) \
        axiom_element_get_default_namespace(om_element, env, element_node)
        
#define AXIOM_ELEMENT_DECLARE_DEFAULT_NAMESPACE(om_element, env, uri) \
        axiom_element_declare_default_namespace(om_element, env, uri)
        
#define AXIOM_ELEMENT_FIND_NAMESPACE_URI(om_element, env, prefix, element_node) \
        axiom_element_find_namespace_uri(om_element, env, prefix, element_node)
        
#define AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE(om_element, env, qname) \
        axiom_element_get_attribute_value(om_element, env, qname)

#define AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_element, env, attr_name) \
        axiom_element_get_attribute_value_by_name(om_element, env, attr_name)
        
                
#define AXIOM_ELEMENT_SET_NAMESPACE_WITH_NO_FIND_IN_CURRENT_SCOPE(om_element, env, om_ns) \
        axiom_element_set_namespace_with_no_find_in_current_scope(om_element, env, om_ns)
               
#define AXIOM_ELEMENT_EXTRACT_ATTRIBUTES(om_element, env, ele_node) \
        axiom_element_extract_attributes(om_element, env, ele_node)
        

                                                      
/** @} */


#ifdef __cplusplus
}
#endif

#endif    /* AXIOM_ELEMENT_H */
