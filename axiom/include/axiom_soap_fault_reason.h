
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

#ifndef AXIOM_SOAP_FAULT_REASON_H
#define AXIOM_SOAP_FAULT_REASON_H

/**
* @file axiom_soap_fault_reason.h
* @brief axiom_soap_fault_reason
*/
#include <axutil_env.h>
#include <axiom_soap_fault.h>
#include <axutil_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault_reason axiom_soap_fault_reason_t;
    struct axiom_soap_fault_text;
    struct axiom_soap_builder;

    /**
     * @defgroup axiom_soap_fault_reason soap fault reason
     * @ingroup axiom_soap
     * @{
     */

    /**
      * creates a SOAP fault reason struct 
      * @param env Environment. MUST NOT be NULL
      * @param fault the SOAP fault
      *
      * @return the created SOAP fault reason struct
      */
    AXIS2_EXTERN axiom_soap_fault_reason_t *AXIS2_CALL
    axiom_soap_fault_reason_create_with_parent(
        const axutil_env_t * env,
        axiom_soap_fault_t * fault);

    /**
      * Free an axiom_soap_fault_reason
      * @param  fault_reason pointer to soap_fault_reason struct
      * @param  env Environment. MUST NOT be NULL
      *
      * @return VOID 
      */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_fault_reason_free(
        axiom_soap_fault_reason_t * fault_reason,
        const axutil_env_t * env);

    /**
      * Get the SOAP fault text by comparing the given string
      * @param  fault_reason pointer to soap_fault_reason struct
      * @param  env Environment. MUST NOT be NULL
      * @param lang string to be compares
      *
      * @return the SOAP fault text of the SOAP fault string 
      */
    AXIS2_EXTERN struct axiom_soap_fault_text *AXIS2_CALL
    axiom_soap_fault_reason_get_soap_fault_text(
        axiom_soap_fault_reason_t * fault_reason,
        const axutil_env_t * env,
        axis2_char_t * lang);

    /**
      * Returns all the  SOAP fault reason strings as an array list
      * @param  fault_reason pointer to soap_fault_reason struct
      * @param  env Environment. MUST NOT be NULL
      *
      * @return  all the  SOAP fault reason strings as an array list
      */
    AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
    axiom_soap_fault_reason_get_all_soap_fault_texts(
        axiom_soap_fault_reason_t * fault_reason,
        const axutil_env_t * env);

    /**
      * Retuens the first SOAP fault reason string
      * @param  fault_reason pointer to soap_fault_reason struct
      * @param  env Environment. MUST NOT be NULL
      *
      * @return The first SOAP fault reason string
      */
    AXIS2_EXTERN struct axiom_soap_fault_text *AXIS2_CALL
        axiom_soap_fault_reason_get_first_soap_fault_text(
        axiom_soap_fault_reason_t * fault_reason,
        const axutil_env_t * env);

    /**
      * Add a string as a SOAP fault reason
      * @param  fault_reason pointer to soap_fault_reason struct
      * @param  env Environment. MUST NOT be NULL
      * @param fault_text The text to be added as the SOAP fault reason
      *
      * @return satus of the op. AXIS2_SUCCESS on success 
      *         else AXIS2_FAILURE
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_fault_reason_add_soap_fault_text(
        axiom_soap_fault_reason_t * fault_reason,
        const axutil_env_t * env,
        struct axiom_soap_fault_text *fault_text);

    /**
      * Get the  base node of the SOAP fault reason
      * @param  fault_reason pointer to soap_fault_reason struct
      * @param  env Environment. MUST NOT be NULL
      * @return the base node of the SOAP fault reason 
      */
    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    axiom_soap_fault_reason_get_base_node(
        axiom_soap_fault_reason_t * fault_reason,
        const axutil_env_t * env);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_SOAP_FAULT_REASON_H */
