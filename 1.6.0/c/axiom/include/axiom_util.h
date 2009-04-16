
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

#ifndef AXIOM_UTIL_H
#define AXIOM_UTIL_H

#include <axutil_array_list.h>
#include <axutil_string.h>
#include <axiom.h>
#include <axutil_uri.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * After calling this method the variable child points to
     * the node of the returning element
     * @param ele_node axiom node 
     * @param env environment, MUST not be NULL   
     * @param uri uri 
     * return the first child element which has the given uri 
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_first_child_element_with_uri(
        axiom_node_t * ele_node,
        const axutil_env_t * env,
        axis2_char_t * uri,
        axiom_node_t ** child);
    /**
     * After calling this method next_node will point to the
     * previous sibling node to the returning node
     * @param ele_node axiom node 
     * @param env environment, MUST not be NULL   
     * @param uri uri
     * @param next_node
     * return the next sibling element to the element which contains
     * the give namespace uri
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_next_sibling_element_with_uri(
        axiom_node_t * ele_node,
        const axutil_env_t * env,
        axis2_char_t * uri,
        axiom_node_t ** next_node);
    /**
     * @param eleaxiom node 
     * @param env environment, MUST not be NULL   
     * @param ele_node
     * @param child_node
     * return the first child element this calls the method
     * axiom_element_get_first_child_element 
     * 
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_first_child_element(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axiom_node_t ** child_node);
    /**
     * @param ele axiom element
     * @param env environment, MUST not be NULL   
     * @param ele_node
     * @param child_node
     * return the last child element of the  given element ele_node
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_last_child_element(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axiom_node_t ** child_node);
    /**
     * @param ele axiom node 
     * @param env environment, MUST not be NULL   
     * @param ele_node
     * @param next_node
     * return the first child element which has the given uri 
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_next_sibling_element(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axiom_node_t ** next_node);
    /**
     * @param ele axiom element
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param localname localname to find the first child element
     * @param child_node
     * 
     * return the first child element from ele_node which contains the
     * given local name
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_first_child_element_with_localname(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param localname to find the last child element
     * @param child_node
     * return the last child element which having the given local name
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_last_child_element_with_localname(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param localname to find the last child element
     * @param next_node
     * return the next sibling element which is having the given local name
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_next_siblng_element_with_localname(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axiom_node_t ** next_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param localname to find the last child element
     * @param uri uri to of the namespace to find the first element
     * @param next_node
     * return the first child element which is having the given local
     * name and the given namespace uri
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_first_child_element_with_uri_localname(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axis2_char_t * uri,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param localname to find the last child element
     * @param uri uri of the namespace to find the last element
     * @param next_node
     * return the last child element which is having the given local name and
     * the given namespace uri
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_last_child_element_with_uri_localname(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axis2_char_t * uri,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param localname to find the last child element
     * @param uri uri of the namespace to find the last element
     * @param next_node
     * return next sibling element which is having the given local name and
     * the given namespace uri
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_next_sibling_element_with_uri_localname(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axis2_char_t * uri,
        axiom_node_t ** next_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param names local names to find the child element 
     * @param child_node
     * return the first child element which is having all the localnames given
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_first_child_element_with_localnames(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axutil_array_list_t * names,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param names local names to find the last child element
     * @param child_node
     * return the last child element which is having all the localnames given
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_last_child_element_with_localnames(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axutil_array_list_t * names,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node axiom node 
     * @param names local names to find the next sibling
     * @param child_node
     * return the next sibling element which is having all the localnames given
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_next_siblng_element_with_localnames(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axutil_array_list_t * names,
        axiom_node_t ** next_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node
     * @param localname local name to find the first child
     * @param attr_name attribute name to find first child
     * @param attr_value attribute value of attr_name attribute
     * @param child_node 
     * return the first child element which is having the given local
     * name and the given attribute (attribute name and attribute value)
     */
    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_first_child_element_with_localname_attr(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axis2_char_t * attr_name,
        axis2_char_t * attr_value,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node
     * @param localname local name to find the last child
     * @param attr_name attribute name to find last child
     * @param attr_value attribute value of attr_name attribute
     * @param child_node 
     * return the last child element which is having the given local
     * name and the given attribute (attribute name and attribute value)
     */
 

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_last_child_element_with_localname_attr(
        axiom_element_t * ele,
        const axutil_env_t * env,
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axis2_char_t * attr_name,
        axis2_char_t * attr_value,
        axiom_node_t ** child_node);
    /**
     * @param ele
     * @param env environment, MUST not be NULL   
     * @param ele_node
     * @param localname local name to find the next sibling child
     * @param attr_name attribute name to find the next sibling child
     * @param attr_value attribute value of attr_name attribute
     * @param child_node 
     * return the next sibling child element which is having the given local
     * name and the given attribute (attribute name and attribute value)
     */

    AXIS2_EXTERN axiom_element_t *AXIS2_CALL
    axiom_util_get_next_siblng_element_with_localname_attr(
        axiom_element_t * ele,
        const axutil_env_t * env, 
        axiom_node_t * ele_node,
        axis2_char_t * localname,
        axis2_char_t * attr_name,
        axis2_char_t * attr_value,
        axiom_node_t ** next_node);
    /**
     * @param node axiom node 
     * @param env environment, MUST not be NULL   
     * 
     * return the element text of axiom_node
     */

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_util_get_child_text(
        axiom_node_t * node,
        const axutil_env_t * env);
    /**
     * @param node axiom node 
     * @param env environment, MUST not be NULL   
     *
     * return the local name of axiom_node
     */

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_util_get_localname(
        axiom_node_t * node,
        const axutil_env_t * env);
    /**
     * @param om_node axiom node 
     * @param env environment, MUST not be NULL   
     * 
     * return the namespace uri of the give node variable om_node if
     * there's no namespace in that particular om_node this method
     * returns NULL
     */

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_util_get_node_namespace_uri(
        axiom_node_t * om_node,
        const axutil_env_t * env);
    /**
     * @param om_ele axiom node 
     * @param env environment, MUST not be NULL   
     * @param om_node
     * return all the child element using the method
     * axiom_child_element_iterator_create
     */

    AXIS2_EXTERN axiom_child_element_iterator_t *AXIS2_CALL
    axiom_util_get_child_elements(
        axiom_element_t * om_ele,
        const axutil_env_t * env,
        axiom_node_t * om_node);

    AXIS2_EXTERN axiom_document_t *AXIS2_CALL
    axiom_util_new_document(
        const axutil_env_t * env,
        const axutil_uri_t * uri);

    AXIS2_EXTERN axiom_node_t* AXIS2_CALL
    axiom_util_get_node_by_local_name(
        const axutil_env_t *env,
        axiom_node_t *node,
        axis2_char_t *local_name);


#ifdef __cplusplus
 }
#endif
#endif                          /* AXIOM_UTIL_H */
