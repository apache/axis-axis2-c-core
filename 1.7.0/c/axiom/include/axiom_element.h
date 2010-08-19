
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
#include <axutil_hash.h>
#include <axutil_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_element axiom_element_t;

    /**
     * @defgroup axiom_element element
     * @ingroup axiom_om
     * @{
     */

    /**
     * Creates an AXIOM element with given local name
     * @param env Environment. MUST NOT be NULL.
     * @param parent parent of the element node to be created. can be NULL.
     * @param localname local name of the element. cannot be NULL.
     * @param ns namespace of the element.  can be NULL.
     *                       If the value of the namespace has not already been declared
     *                       then the namespace structure ns will be cloned and declared and will be
     *                       freed when the tree is freed.
     *                       Caller has to delete the original ns object passed to the method.
     * @param node This is an out parameter. cannot be NULL.
     *                       Returns the node corresponding to the comment created.
     *                       Node type will be set to AXIOM_ELEMENT
     * @return a pointer to the newly created element struct
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_element_create(
        const axutil_env_t * env,
        axiom_node_t * parent,
        const axis2_char_t * localname,
        axiom_namespace_t * ns,
        axiom_node_t ** node);

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
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_element_create_with_qname(
        const axutil_env_t * env,
        axiom_node_t * parent,
        const axutil_qname_t * qname,
        axiom_node_t ** node);

    /**
     * Frees given element
     * @param element AXIOM element to be freed.
     * @param env Environment. MUST NOT be NULL.
     * @return status of the operation. AXIS2_SUCCESS on success ,AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_element_free(
        axiom_element_t * element,
        const axutil_env_t * env);

    /**
     * finds a namespace in current element's scope, by uri or prefix or both
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param uri namespace uri, may be null
     * @param prefix prefix
     * @return axiom_namespace_t if found, else return NULL
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_element_find_declared_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        const axis2_char_t * uri,
        const axis2_char_t * prefix);

    /**
     * Find a namespace in the scope of the document.
     * Start to find from the given node and go up the hierarchy.
     * @param om_element pointer to om_element_struct contained in node ,
     * @param env Environment. MUST NOT be NULL.
     * @param node node containing an instance of an AXIOM element,cannot be NULL.
     * @param uri namespace uri..
     * @param prefix namespace prefix. can be NULL.
     * @return pointer to the namespace, if found, else NULL. On error, returns 
     *           NULL and sets error code in environment,s error
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_element_find_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * node,
        const axis2_char_t * uri,
        const axis2_char_t * prefix);

    /**
     * Finds a namespace using qname. Start to find from the given node and go up the hierarchy.
     * @param om_element om_element contained in node
     * @param env Environment. MUST NOT be NULL.
     * @param node node containing an instance of an AXIOM element, cannot be NULL.
     * @param qname qname of the namespace to be found. cannot be NULL.
     * @return pointer to the namespace, if found, else NULL. On error, returns
     *           NULL and sets the error code in environment's error struct.
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_element_find_namespace_with_qname(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * node,
        axutil_qname_t * qname);

    /**
      * Declare a namespace in current element (in the scope of this element ).
      * It checks to see if it is already declared at this level or in its ancestors
      * @param om_element contained in the om node struct
      * @param env Environment. MUST NOT be NULL.
      * @param node node containing an instance of an AXIOM element.
      * @param ns pointer to the namespace struct to be declared. Should not be null
      * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_declare_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * node,
        axiom_namespace_t * ns);

    /**
     * get the namespace  of om_element
     * @param om_element om_element struct
     * @param env environment, MUST NOT be NULL.
     * @returns pointer to axiom_namespace_t struct
     *          NULL if there is no namespace associated with the element,
     *          NULL on error with error code set to environment's error
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_element_get_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * ele_node);

    /**
     * set the namespace of the element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param ns pointer to namespace. Must not be NULL
     *                       If the value of the namespace has not already been declared
     *                       then the namespace structure ns will be declared and will be
     *                       freed when the tree is freed.
     * @returns status code of the op, with error code
     *                  set to environment's error
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_set_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_namespace_t * ns,
        axiom_node_t * node);

    /**
     * get the namespace list of the element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @returns axutil_hash pointer to namespaces hash
     * this hash table is read only
     */
    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_element_get_namespaces(
        axiom_element_t * om_element,
        const axutil_env_t * env);

    /**
     * Adds an attribute to current element. The current element takes responsibility of the
     * assigned attribute
     * @param om_element element to which the attribute is to be added.cannot be NULL.
     * @param env Environment. MUST NOT be NULL.
     * @param attribute attribute to be added.
     * @param node axiom_node_t node that om_element is contained in
     * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_add_attribute(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_attribute_t * attribute,
        axiom_node_t * node);

    /**
     * Gets (finds) the attribute with the given qname
     * @param element element whose attribute is to be found.
     * @param env Environment. MUST NOT be NULL.
     * @qname qname qname of the attribute to be found. should not be NULL.
     * @return a pointer to the attribute with given qname if found, else NULL.
     *           On error, returns NULL and sets the error code in environment's error struct.
     */
    AXIS2_EXTERN axiom_attribute_t *AXIS2_CALL
    axiom_element_get_attribute(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axutil_qname_t * qname);

    /**
     * get  the attribute list of the element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @returns axutil_hash pointer to attributes hash
     * This hash table is read only
     */
    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_element_get_all_attributes(
        axiom_element_t * om_element,
        const axutil_env_t * env);

    /**
     * Gets (finds) the attribute value with the given qname
     * @param element element whose attribute is to be found.
     * @param env Environment. MUST NOT be NULL.
     * @qname qname qname of the attribute to be found. should not be NULL.
     * @return the attribute value with given qname if found, else NULL.
     *  On error, returns NULL and sets the error code in environment's error struct.
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_element_get_attribute_value(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axutil_qname_t * qname);

    /**
     *  Extract attributes , returns a clones hash table of attributes,
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     */
    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_element_extract_attributes(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * ele_node);

    /**
     * Returns the attribute value as a string for the given element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param attr_name the attribute name
     * @return the attribute value as a string
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_element_get_attribute_value_by_name(
        axiom_element_t * om_ele,
        const axutil_env_t * env,
        axis2_char_t * attr_name);

    /**
     * Select all the text children and concatenate them to a single string. The string
     * returned by this method call will be free by axiom when this method is called again.
     * So it is recommended to have a copy of the return value if this method is going to
     * be called more that once and the return values of the earlier calls are important.
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param element node , the container node of this om element
     * @return the concatenated text of all text children text values
     *         return null if no text children is available or on error
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_element_get_text(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node);

    /**
     * Sets the text of the given element.
     * caution - This method will wipe out all the text elements (and hence any mixed content)
     * before setting the text
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param text text to set.
     * @param element_node node of element.
     * @return AXIS2_SUCCESS if attribute was found and removed, else
     *           AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_set_text(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        const axis2_char_t * text,
        axiom_node_t * element_node);

    /**
     * returns the localname of this element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @returns localname of element, returns NULL on error.
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_element_get_localname(
        axiom_element_t * om_element,
        const axutil_env_t * env);

    /**
     * set the localname of this element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @localname text value to be set as localname
     * @returns status code of operation, AXIS2_SUCCESS on success,
     *                   AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_set_localname(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        const axis2_char_t * localname);

    /**
     * return qname of this element. The returned qname should not be freed by the caller.
     * It will be freed when om_element struct is freed
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param ele_node pointer to this element node
     * @returns axutil_qname_t struct , NULL on failure
     */
    AXIS2_EXTERN axutil_qname_t *AXIS2_CALL
    axiom_element_get_qname(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * ele_node);

    /**
     * returns a list of children iterator. Returned iterator is freed when om_element struct
     * is freed.
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param element_node pointer to this element node
     */
    AXIS2_EXTERN axiom_children_iterator_t *AXIS2_CALL
    axiom_element_get_children(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node);

    /**
     * returns a list of children iterator with qname. Returned iterator is freed when om element
     * struct is freed
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param element_node pointer to this element node
     * @returns children qname iterator struct
     */
    AXIS2_EXTERN axiom_children_qname_iterator_t *AXIS2_CALL
    axiom_element_get_children_with_qname(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axutil_qname_t * element_qname,
        axiom_node_t * element_node);

    /**
     * Returns the first om_element corresponding to element_qname
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param element_qname qname of the element
     * @param om_node pointer to this element node
     * @param element_node
     * @param child_node
     * @returns children qname iterator struct
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_element_get_first_child_with_qname(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axutil_qname_t * element_qname,
        axiom_node_t * element_node,
        axiom_node_t ** child_node);

    /**
     * returns the first child om element of this om element node
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @return om_element if one is available otherwise return NULL
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_element_get_first_element(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node,
        axiom_node_t ** first_element_node);

    /**
     * returns an iterator with child elements of type AXIOM_ELEMENT
     * iterator is freed when om_element node is freed
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param element_node
     * @returns axiom_child_element_iterator_t , NULL on error
     */
    AXIS2_EXTERN axiom_child_element_iterator_t *AXIS2_CALL
    axiom_element_get_child_elements(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node);

    /**
	 * This method will declare the namespace without checking whether it is already declared. 
	 * (This method is only used by codegen. We have to remove this method in future)
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
     *
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_declare_namespace_assume_param_ownership(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_namespace_t * ns);

#if 0
    /**
     * builds this om_element_node completely, This is only possible
     * if the om_stax_builder is associated with the om_element_node,
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @param element_node corresponding om element node of this om element
     * struct
     * @returns AXIS2_SUCCESS if this element node was successfully completed,
     * otherwise returns AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_build(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node);

    /**
     * checks for the namespace in the context of this element
     * with the given prefix
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_element_node pointer to this element node
     * @returns pointer to relevent namespace
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_element_find_namespace_uri(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        const axis2_char_t * prefix,
        axiom_node_t * element_node);

    /**
     * Returns the Local name of the element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     *
     * @return the Local name of the element
     */
    AXIS2_EXTERN axutil_string_t *AXIS2_CALL
    axiom_element_get_localname_str(
        axiom_element_t * om_element,
        const axutil_env_t * env);

    /**
     * Set the Local name of the element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param localname  the Local name of the element
     *
     * @return
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_set_localname_str(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axutil_string_t * localname);

    /**
     *This will not search the namespace in the scope nor will
     * declare in the current element, as in set_namespace. This will
     * just assign the given namespace to the element.
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @param om_ns pointer to namespace to be set
     * @returns
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_set_namespace_with_no_find_in_current_scope(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_namespace_t * om_ns);


    /**
     * unconditionally set the namespace of the element
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param ns pointer to namespace
     *                       The namespace ns is assumed to have been declared already.
     * @returns status code of the op, with error code
     *                  set to environment's error
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_set_namespace_assume_param_ownership(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_namespace_t * ns);

    /**
     * declared a default namespace explicitly
     * @param om_element pointer to om element
     * @param env environment MUST not be NULL
     * @param uri namespace uri of the default namespace
     * @returns the declared namespace
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_element_declare_default_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axis2_char_t * uri);

    /**
     * Create an OM Element and an OM node from given string params
     * @param env environment MUST not be NULL
     * @param parent pointer to this parent element node
     * @param localname the locanmae of the element
     * @param ns the namespace of the element
     * @param node the reference ot the created node
     * @return
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_element_create_str(
        const axutil_env_t * env,
        axiom_node_t * parent,
        axutil_string_t * localname,
        axiom_namespace_t * ns,
        axiom_node_t ** node);

    /**
     * Return whether the element is empty or not
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     *
     * @return AXIS2_TRUE if empty AXIS2_FALSE if not empty
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axiom_element_get_is_empty(
        axiom_element_t * om_element,
        const axutil_env_t * env);

    /**
     * removes an attribute from the element attribute list
     * user must free this attribute, element free function does not free
     * attributes that are not is it's attribute list
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_attribute attribute to be removed
     * @return AXIS2_SUCCESS if attribute was found and removed, else
     *           AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_element_remove_attribute(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_attribute_t * om_attribute);

    /**
     * returns the serilized text of this element and its children
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param element_node the container node this on element is contained
     * @return a char array of xml , returns NULL on error
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_element_to_string(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node);
#endif
    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_ELEMENT_H */
