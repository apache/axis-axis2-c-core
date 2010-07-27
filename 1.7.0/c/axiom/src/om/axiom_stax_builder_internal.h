
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

#ifndef AXIOM_STAX_BUILDER_INTERNAL_H
#define AXIOM_STAX_BUILDER_INTERNAL_H

/** @defgroup axiom AXIOM (Axis Object Model)
 * @ingroup axis2
 * @{
 */

#include <axiom_stax_builder.h>
#include <axiom_soap_builder.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_stax builder
     * @ingroup axiom
     * @{
     */

    int AXIS2_CALL
    axiom_stax_builder_get_current_event(
        axiom_stax_builder_t * builder,
        const axutil_env_t * env);

    void AXIS2_CALL
    axiom_stax_builder_set_lastnode(
        axiom_stax_builder_t * builder,
        const axutil_env_t * env,
        axiom_node_t * om_node);

    axiom_node_t *AXIS2_CALL
    axiom_stax_builder_get_lastnode(
        axiom_stax_builder_t * builder,
        const axutil_env_t * env);

    void AXIS2_CALL
    axiom_stax_builder_set_element_level(
        axiom_stax_builder_t * builder,
        const axutil_env_t * env,
        int element_level);

    int AXIS2_CALL
    axiom_stax_builder_get_element_level(
        axiom_stax_builder_t * builder,
        const axutil_env_t * env);

    /* this will be called from root node to free the stax builder */
    void AXIS2_CALL
    axiom_stax_builder_free_internal(
        axiom_stax_builder_t * builder,
        const axutil_env_t * env);


    /**
      * builder is finished building om structure
      * @param builder pointer to stax builder struct to be used
      * @param environment Environment. MUST NOT be NULL.
      *
      * @return AXIS2_TRUE if is complete or AXIS2_FALSE otherwise
      */

    axis2_bool_t AXIS2_CALL
    axiom_stax_builder_is_complete(
        struct axiom_stax_builder *builder,
        const axutil_env_t * env);

    /**
      * moves the reader to next event and returns the token returned by the xml_reader ,
      * @param builder pointer to STAX builder struct to be used
      * @param environment Environment. MUST NOT be NULL.
      * @return next event axiom_xml_reader_event_types. Returns -1 on error
      */
    int AXIS2_CALL
    axiom_stax_builder_next_with_token(
        struct axiom_stax_builder *builder,
        const axutil_env_t * env);

    void AXIS2_CALL
    axiom_stax_builder_set_soap_builder(
        axiom_stax_builder_t *om_builder,
        const axutil_env_t *env,
        axiom_soap_builder_t *soap_builder);

    axiom_node_t *AXIS2_CALL
    axiom_stax_builder_get_root_node(
        axiom_stax_builder_t *om_builder,
        const axutil_env_t * env);



#if 0
    /**
      * Discards the element that is being built currently.
      * @param environment Environment. MUST NOT be NULL, .
      * @param builder pointer to stax builder struct to be used
      * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_stax_builder_discard_current_element(
        struct axiom_stax_builder *builder,
        const axutil_env_t * env);

    AXIS2_EXTERN axiom_xml_reader_t *AXIS2_CALL
    axiom_stax_builder_get_parser(
        axiom_stax_builder_t * om_builder,
        const axutil_env_t * env);

    AXIS2_EXTERN void AXIS2_CALL
    axiom_stax_builder_set_cache(
        axiom_stax_builder_t * om_builder,
        const axutil_env_t * env,
        axis2_bool_t enable_cache);

    /**
     * Builds the next node from stream. Moves pull parser forward and reacts
     * to events.
     * @param builder pointer to stax builder struct to be used
     * @param environment Environment. MUST NOT be NULL.
     * @return a pointer to the next node, or NULL if there are no more nodes.
     *     On erros sets the error and returns NULL.
     */
    axiom_node_t *AXIS2_CALL
    axiom_stax_builder_next(
        struct axiom_stax_builder *builder,
        const axutil_env_t * env);

#endif

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /** AXIOM_STAX_BUILDER_INTERNAL_H */
