
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

#ifndef AXIOM_SOAP_BUILDER_H
#define AXIOM_SOAP_BUILDER_H

#include <axiom_stax_builder.h>
#include <axiom_soap_envelope.h>
#include <axiom_mime_parser.h>

/**
 * @file axiom_soap_builder.h
 * @brief axiom_soap_builder struct
 */

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_builder axiom_soap_builder_t;

    /**
     * @defgroup axiom_soap_builder soap builder
     * @ingroup axiom_soap
     * @{
     */

    /**
     * creates a axiom_soap_builder struct
     * @param env Environment. MUST NOT be NULL
     * @param builder Stax builder
     * @return the created SOAP Builder
     */
    AXIS2_EXTERN axiom_soap_builder_t *AXIS2_CALL
    axiom_soap_builder_create(
        const axutil_env_t * env,
        axiom_stax_builder_t * builder,
        const axis2_char_t * soap_version);
    /**
     * Free the SOAP Builder
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return VOID
     */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_builder_free(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     * Get the SOAP envelope of the SOAP builder
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return the SOAP envelope
     */
    AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
    axiom_soap_builder_get_soap_envelope(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     * Get the OM document of the SOAP Buidler
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return the OM document
     */
    AXIS2_EXTERN axiom_document_t *AXIS2_CALL
    axiom_soap_builder_get_document(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     *
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_SUCCESS if the next element is present else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_next(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     *
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return the axiom_node of the OM document
     */
    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    axiom_soap_builder_get_document_element(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     *
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_set_bool_processing_mandatory_fault_elements(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        axis2_bool_t value);

    /**
     *
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_set_processing_detail_elements(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        axis2_bool_t value);

    /**
     * 
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_TRUE if the the element is present, AXIS2_FALSE otherwise
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axiom_soap_builder_is_processing_detail_elements(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     * Get the SOAP version
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return the SOAP version
     */
    AXIS2_EXTERN int AXIS2_CALL
    axiom_soap_builder_get_soap_version(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     * Process and verifies namespace data of @param om_node
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_process_namespace_data(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        axiom_node_t * om_node,
        axis2_bool_t is_soap_element);

    /**
     * Set the MIME body parts
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_set_mime_body_parts(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        axutil_hash_t * map);

    /**
     * Get the MIME body parts
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     *
     * @return hash of mime body parts
     */
    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_soap_builder_get_mime_body_parts(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env);

    /**
     * Set the mime_parser
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     * @paran mime_parser pointer to a axiom_mime_parser_t instance
     */ 

    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_builder_set_mime_parser(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        axiom_mime_parser_t *mime_parser);

     /**
     * Set the callback function
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     * @param callback to the callback function pointer
     */

    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_builder_set_callback_function(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        AXIS2_READ_INPUT_CALLBACK callback);

     /**
     * Set the callback_ctx 
     * @param builder pointer to the SOAP Builder struct
     * @param env Environment. MUST NOT be NULL
     * @param void pointer to the callback_ctx 
     */ 

    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_builder_set_callback_ctx(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        void *callback_ctx);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_create_attachments(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        void *user_param,
        axis2_char_t *callback_name);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axiom_soap_builder_replace_xop(
        axiom_soap_builder_t * builder,
        const axutil_env_t * env,
        axiom_node_t *om_element_node,
        axiom_element_t *om_element);
        



    /** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIOM_SOAP_BUILDER_H */
