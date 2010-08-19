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

#ifndef AXIOM_ELEMENT_INTERNAL_H_
#define AXIOM_ELEMENT_INTERNAL_H_

/** @defgroup axiom AXIOM (Axis Object Model)
 * @ingroup axis2
 * @{
 */

/** @} */

#include <axiom_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_element element
     * @ingroup axiom
     * @{
     */

    /**
     * Collect all the namespaces with distinct prefixes in the parents of the given element.
     * Effectively this is the set of namespaces declared above this element that are inscope at
     * this element and might be used by it or its children.
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @returns pointer to hash of relevant namespaces
     */
    axutil_hash_t * AXIS2_CALL
    axiom_element_gather_parent_namespaces(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * om_node);

    /**
     * Examines the subtree beginning at the provided element for each element or attribute,
     * if it refers to a namespace declared in a parent of the subtree root element, if not already
     * declared, redeclares that namespace at the level of the subtree root and removes
     * it from the set of parent inscope_namespaces. inscope_namespaces contains all the parent
     * namespaces which should be redeclared at some point.
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @param inscope_namespaces pointer to hash of parent namespaces
     */
    void AXIS2_CALL
    axiom_element_redeclare_parent_namespaces(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * om_node,
        axutil_hash_t *inscope_namespaces);

    /**
     * If the provided namespace used by the provided element is one of the namespaces from the
     * parent of the root element, redeclares that namespace at the root element and removes it
     * from the hash of parent namespaces
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @param ns pointer to namespace to redeclare
     * @param inscope_namespaces pointer to hash of parent namespaces
     */
    void AXIS2_CALL
    axiom_element_use_parent_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * om_node,
        axiom_namespace_t *ns,
        axutil_hash_t *inscope_namespaces);

    /**
     * retrieves the default namespace of this element
     * @param om_element pointer to om element
     * @param env axutil_environment MUST Not be NULL
     * @param element_node corresponding om element node of this om element
     * @returns pointer to default namespace if available , NULL otherwise
     */
    axiom_namespace_t *AXIS2_CALL
    axiom_element_get_default_namespace(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_node_t * element_node);

    /**
     * Serializes the start part of the given element
     * @param element element to be serialized.
     * @param env Environment. MUST NOT be NULL.
     * @param om_output AXIOM output handler to be used in serializing
     * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    axis2_status_t AXIS2_CALL
    axiom_element_serialize_start_part(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_output_t * om_output,
        axiom_node_t * ele_node);

    /**
     * Serializes the end part of the given element. serialize_start_part must
     *     have been called before calling this method.
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param om_node pointer to this element node
     * @param om_output AXIOM output handler to be used in serializing
     * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    axis2_status_t AXIS2_CALL
    axiom_element_serialize_end_part(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axiom_output_t * om_output);

    /**
     * Set whether the element is empty or not
     * @param om_element pointer to om_element
     * @param env environment MUST not be NULL
     * @param is_empty AXIS2_TRUE if empty AXIS2_FALSE if not empty
     * @return VOID
     */
    void AXIS2_CALL
    axiom_element_set_is_empty(
        axiom_element_t * om_element,
        const axutil_env_t * env,
        axis2_bool_t is_empty);

#endif /* AXIOM_ELEMENT_INTERNAL_H_ */
