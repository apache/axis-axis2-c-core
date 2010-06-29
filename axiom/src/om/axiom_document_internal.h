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

#ifndef AXIOM_DOCUMENT_INTERNAL_H_
#define AXIOM_DOCUMENT_INTERNAL_H_

#include <axiom_document.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
      * creates an axiom_document_t struct
      * @param env Environment. MUST NOT be NULL.
      * @param root pointer to document's root node. Optional, can be NULL
      * @param builder pointer to axiom_stax_builder
      * @return pointer to the newly created document.
      */
    axiom_document_t *AXIS2_CALL
    axiom_document_create(
        const axutil_env_t * env,
        axiom_node_t * root,
        struct axiom_stax_builder *builder);

    /**
      * Free document struct
      * @param document pointer to axiom_document_t struct to be freed
      * @param env Environment. MUST NOT be NULL
      * @return status of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
      */
    void AXIS2_CALL
    axiom_document_free(
        struct axiom_document *document,
        const axutil_env_t * env);

    /**
     * Free document struct only, Does not free the associated axiom structure.
     * @param document pointer to axiom_document_t struct to be freed
     * @param env Environment. MUST NOT be NULL
     * @return status of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
     */
    void AXIS2_CALL
    axiom_document_free_self(
        struct axiom_document *document,
        const axutil_env_t * env);

    /**
      * set the root element of the document. IF a root node is already exist,it is freed
      * before setting to root element
      * @param document document struct to return the root of
      * @param env Environment. MUST NOT be NULL.
      * @return returns status code AXIS2_SUCCESS on success ,AXIS2_FAILURE on error.
      */
    axis2_status_t AXIS2_CALL
    axiom_document_set_root_element(
        struct axiom_document *document,
        const axutil_env_t * env,
        axiom_node_t * om_node);

#if 0
    /* these methods are commented, because it is not used anymore (1.6.0)*/

    /**
     * get builder
     * @param document pointer to axiom_document_t struct to be built.
     * @param env environment MUST NOT be NULL.
     * @return builder, returns NULL if a builder is not associated with
     * document
     */
    AXIS2_EXTERN struct axiom_stax_builder *AXIS2_CALL
    axiom_document_get_builder(
        struct axiom_document *document,
        const axutil_env_t * env);

    /**
     * sets builder for document.
     * @param document pointer to axiom_document_t struct to be built.
     * @param env environment MUST NOT be NULL.
     * @param builder pointer to builder to associate with document
     */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_document_set_builder(
        axiom_document_t * document,
        const axutil_env_t * env,
        struct axiom_stax_builder * builder);

    /**
     * @param om_document
     * @return status code AXIS2_SUCCESS on success , otherwise AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_document_serialize(
        struct axiom_document *document,
        const axutil_env_t * env,
        axiom_output_t * om_output);


#endif

#ifdef __cplusplus
}
#endif

#endif /* AXIOM_DOCUMENT_INTERNAL_H_ */
