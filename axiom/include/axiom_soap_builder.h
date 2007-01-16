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
     */
    AXIS2_EXTERN axiom_soap_builder_t * AXIS2_CALL
    axiom_soap_builder_create(const axis2_env_t *env,
            axiom_stax_builder_t *builder,
            const axis2_char_t *soap_version);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_soap_builder_free(axiom_soap_builder_t *builder,
            const axis2_env_t *env);

    AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
    axiom_soap_builder_get_soap_envelope(axiom_soap_builder_t *builder,
            const axis2_env_t *env);

    AXIS2_EXTERN axiom_document_t* AXIS2_CALL
    axiom_soap_builder_get_document(axiom_soap_builder_t *builder,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_builder_next(axiom_soap_builder_t *builder,
            const axis2_env_t *env);

    AXIS2_EXTERN axiom_node_t* AXIS2_CALL 
    axiom_soap_builder_get_document_element(axiom_soap_builder_t *builder,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_soap_builder_set_bool_processing_mandatory_fault_elements(axiom_soap_builder_t *builder,
            const axis2_env_t *env,
            axis2_bool_t value);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_soap_builder_set_processing_detail_elements(axiom_soap_builder_t *builder,
            const axis2_env_t *env,
            axis2_bool_t value);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
    axiom_soap_builder_is_processing_detail_elements(axiom_soap_builder_t *builder,
            const axis2_env_t *env);


    AXIS2_EXTERN int AXIS2_CALL 
    axiom_soap_builder_get_soap_version(axiom_soap_builder_t *builder,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_soap_builder_process_namespace_data(axiom_soap_builder_t *builder,
            const axis2_env_t *env,
            axiom_node_t *om_node,
            axis2_bool_t is_soap_element);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_soap_builder_set_mime_body_parts(axiom_soap_builder_t *builder,
            const axis2_env_t *env,
            axis2_hash_t *map);

/******************** Macros **************************************************/

#define AXIOM_SOAP_BUILDER_FREE(builder, env) \
        axiom_soap_builder_free(builder, env)

#define AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(builder, env) \
        axiom_soap_builder_get_soap_envelope(builder, env)

#define AXIOM_SOAP_BUILDER_GET_DOCUMENT_ELEMENT(builder, env) \
        axiom_soap_builder_get_document_element(builder, env)

#define AXIOM_SOAP_BUILDER_NEXT(builder, env) \
        axiom_soap_builder_next(builder, env)

#define AXIOM_SOAP_BUILDER_GET_DOCUMENT(builder, env) \
        axiom_soap_builder_get_document(builder, env)

#define AXIOM_SOAP_BUILDER_NEXT(builder, env) \
        axiom_soap_builder_next(builder, env)

#define AXIOM_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(builder, env, value) \
        axiom_soap_builder_set_bool_processing_mandatory_fault_elements(builder, env, value)

#define AXIOM_SOAP_BUILDER_SET_PROCESSING_DETAIL_ELEMENTS(builder, env, value) \
        axiom_soap_builder_set_processing_detail_elements(builder, env, value)

#define AXIOM_SOAP_BUILDER_IS_PROCESSING_DETAIL_ELEMENTS(builder, env) \
        axiom_soap_builder_is_processing_detail_elements(builder, env)


#define AXIOM_SOAP_BUILDER_GET_SOAP_VERSION(builder, env) \
        axiom_soap_builder_get_soap_version(builder, env)

#define AXIOM_SOAP_BUILDER_PROCESS_NAMESPACE_DATA(builder, env, om_node, is_soap_element) \
        axiom_soap_builder_process_namespace_data(builder, env, om_node, is_soap_element)

#define AXIOM_SOAP_BUILDER_SET_MIME_BODY_PARTS(builder, env, map) \
        axiom_soap_builder_set_mime_body_parts(builder, env, map)

    /** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIOM_SOAP_BUILDER_H */
