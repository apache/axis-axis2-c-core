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

#ifndef AXIOM_STAX_BUILDER_H
#define AXIOM_STAX_BUILDER_H

/**
 * @file axiom_stax_builder.h
 * @brief om model stax builder
 */

#include <axiom_node.h>
#include <axiom_xml_reader.h>
#include <axiom_document.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_stax_builder stax builder
     * @ingroup axiom_om
     * @{
     */

    typedef struct axiom_stax_builder axiom_stax_builder_t;


    /**
      * creates an stax builder
      * @param environment Environment. MUST NOT be NULL, .
      * @return a pointer to the newly created builder struct. 
      */
    AXIS2_EXTERN axiom_stax_builder_t * AXIS2_CALL
    axiom_stax_builder_create(const axis2_env_t *env,
            axiom_xml_reader_t *parser);

    /**
      * Builds the next node from stream. Moves pull parser forward and reacts 
      * to events.
      * @param environment Environment. MUST NOT be NULL.
      * @param builder pointer to stax builder struct to be used
      * @return a pointer to the next node, or NULL if there are no more nodes.
      *           On erros sets the error and returns NULL.
      */
    AXIS2_EXTERN axiom_node_t* AXIS2_CALL
    axiom_stax_builder_next(struct axiom_stax_builder *builder,
            const axis2_env_t *env);

    /**
      * Discards the element that is being built currently.
      * @param environment Environment. MUST NOT be NULL, .
      * @param builder pointer to stax builder struct to be used
      * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_stax_builder_discard_current_element(struct axiom_stax_builder *builder,
            const axis2_env_t *env);
    /**
     * Free op
     * @param builder pointer to builder struct
     * @param env environment, MUST NOT be NULL
     * @return status of the op AXIS2_SUCCESS on success,
     *         AXIS2_FAILURE on error.
     */

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_stax_builder_free(struct axiom_stax_builder *builder,
            const axis2_env_t *env);

    /** get the document associated with the builder
     * @param builder axiom_stax_builder 
     * @param env environment 
     * @return pointer to document struct associated with builder
     *         NULL if there is no document associated with the builder,
     *         NULL if an error occured.
     */
    AXIS2_EXTERN axiom_document_t* AXIS2_CALL
    axiom_stax_builder_get_document(struct axiom_stax_builder *builder,
            const axis2_env_t *env);
    /**
     * builder is finished building om structure
     */

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axiom_stax_builder_is_complete(struct axiom_stax_builder* builder,
            const axis2_env_t *env);
    /**
     * moves the reader to next event and returns the token returned
    * by the xml_reader , returns -1 on error 
     */
    AXIS2_EXTERN int AXIS2_CALL
    axiom_stax_builder_next_with_token(struct axiom_stax_builder *builder,
            const axis2_env_t *env);

    /** builds next node */
#define AXIOM_STAX_BUILDER_NEXT(builder,env) \
        axiom_stax_builder_next(builder, env)
    /** discards current node */
#define AXIOM_STAX_BUILDER_DISCARD_CURRENT_ELEMENT(builder,env) \
        axiom_stax_builder_discard_current_element(builder, env)
    /** free op of the builder */
#define AXIOM_STAX_BUILDER_FREE(builder,env) \
        axiom_stax_builder_free(builder,env)

    /** get the document associated with the builder */
#define AXIOM_STAX_BUILDER_GET_DOCUMENT(builder,env) \
        axiom_stax_builder_get_document(builder,env)
    /** builder is finished building */
#define AXIOM_STAX_BUILDER_IS_COMPLETE(builder, env) \
        axiom_stax_builder_is_complete(builder, env)
    /** moves the builder to next event */
#define AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(builder, env) \
        axiom_stax_builder_next_with_token(builder, env)

    /** @} */


#ifdef __cplusplus
}
#endif


#endif                          /* AXIOM_STAX_BUILDER_H */
