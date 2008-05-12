
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

#ifndef AXIOM_SOAP_ENVELOPE_H
#define AXIOM_SOAP_ENVELOPE_H

/**
 * @file axiom_soap_envelope.h
 * @brief axiom_soap_envelope struct
 *        corresponds to root element of soap message
 */
#include <axutil_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_namespace.h>
#include <axutil_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_envelope axiom_soap_envelope_t;

    struct axiom_soap_body;
    struct axiom_soap_header;
    struct axiom_soap_header_block;
    struct axiom_soap_builder;

    /**
     * @defgroup axiom_soap_envelope soap envelope
     * @ingroup axiom_soap
     * @{
     */

    /**
     * create a soap_envelope with the given namespace prefix and uri
     * as the prefix and uri, The uri of ns should be valid soap uri
     * @param env Environment. MUST NOT be NULL
     * @param ns The OM namespace
     *
     * @return Created SOAP envelope
     */
    AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
    axiom_soap_envelope_create(
        const axutil_env_t * env,
        axiom_namespace_t * ns);

    /**
     * create a soap_envelope with the given namespace prefix and uri is selected 
     * according to soap_version, soap version should be one of AXIOM_SOAP11
     * or AXIOM_SOAP12
     * @param env Environment. MUST NOT be NULL
     * @param prefix soap envelope prefix
     * if prefix is NULL default prefix is used
     *
     * @return a pointer to soap envelope struct
     */
    AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
    axiom_soap_envelope_create_with_soap_version_prefix(
        const axutil_env_t * env,
        int soap_version,
        const axis2_char_t * prefix);

    /**
     * Create the default SOAP envelope
     * @param envelope OM SOAP Envelope
     * @param env Environment. MUST NOT be NULL
     *
     * @return Created SOAP envelope
     */
    AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
    axiom_soap_envelope_create_default_soap_envelope(
        const axutil_env_t * env,
        int soap_version);

    /**
     * Create the default SOAP fault envelope
     * @param envelope OM SOAP Envelope
     * @param env Environment. MUST NOT be NULL
     *
     * @return Created SOAP fault envelope
     */
    AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
    axiom_soap_envelope_create_default_soap_fault_envelope(
        const axutil_env_t * env,
        const axis2_char_t * code_value,
        const axis2_char_t * reason_text,
        const int soap_version,
        axutil_array_list_t * sub_codes,
        axiom_node_t * detail_node);

    /**
     * gets the soap header of this soap envelope
     * @param envelope soap envelope
     * @param env environment must not be null
     * @return soap header null it no header is present
     */
    AXIS2_EXTERN struct axiom_soap_header *AXIS2_CALL
    axiom_soap_envelope_get_header(
         axiom_soap_envelope_t * envelope,
         const axutil_env_t * env);

    /**
     * Returns the soap body associated with this soap envelope
     * @param envelope soap_envelope
     * @param env environment
     * @return soap_body
     */
    AXIS2_EXTERN struct axiom_soap_body *AXIS2_CALL
    axiom_soap_envelope_get_body(
         axiom_soap_envelope_t * envelope,
         const axutil_env_t * env);

    /**
     * serialize function , serialize the soap envelope 
     * IF the soap version it set to soap11 the soap fault part is converted 
     * to soap11 fault even is the underlying soap fault is of soap12 type
     * @param envelope soap envelope
     * @param env environment must not be null
     * @param om_output 
     * @param cache whether caching is enabled or not
     * @return status code , AXIS2_SUCCESS if success ,
     *                 AXIS2_FAILURE otherwise
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_envelope_serialize(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env,
        axiom_output_t * om_output,
        axis2_bool_t cache);

    /**
     * Free function, This function deallocate all the resources associated 
     * with the soap_envelope
     * IT frees it's soap body and soap headers as well as the underlying
     * om node tree by calling axiom_node_free_tree function
     * @param envelope soap_envelope
     * @param env environment
     * @return VOID
     */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_envelope_free(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env);

    /**
     * returns the om_node associated with this soap envelope
     * @param envelope soap_envelope
     * @param env environment
     * @return axiom_node_t pointer
     */
    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    axiom_soap_envelope_get_base_node(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env);

    /** returns the soap version of this soap envelope
     * @param envelope soap_envelope
     * @param env environment must not be null
     * @return soap_version AXIOM_SOAP12 or AXIOM_SOAP11
     */
    AXIS2_EXTERN int AXIS2_CALL
    axiom_soap_envelope_get_soap_version(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env);

    /** 
     * Return the soap envelope namespace
     * @param envelope 
     * @param env 
     * @return axiom_namespace_t 
     */
    AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
    axiom_soap_envelope_get_namespace(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env);

    /**
     * Set the SOAP version 
     * @param envelope OM SOAP Envelope
     * @param env Environment. MUST NOT be NULL
     * @param soap_version, the SOAP version number. 
     *      Must be either AXIOM_SOAP11 or AXIOM_SOAP12
     *
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_envelope_set_soap_version(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env,
        int soap_version);

    /**
     * Increment the reference number for the created instance 
     * @param envelope OM SOAP Envelope
     * @param env Environment. MUST NOT be NULL
     *
     * @return AXIS2_SUCCESS on success else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_envelope_increment_ref(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env);

    /**
     * get the soap builder of the envelope 
     * @param envelope OM SOAP Envelope
     * @param env Environment. MUST NOT be NULL
     *
     * @return soap_builder struct related to the envelope
     */
    AXIS2_EXTERN struct axiom_soap_builder *AXIS2_CALL
    axiom_soap_envelope_get_soap_builder(
        axiom_soap_envelope_t * envelope,
        const axutil_env_t * env);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_SOAP_ENVELOPE_H */
