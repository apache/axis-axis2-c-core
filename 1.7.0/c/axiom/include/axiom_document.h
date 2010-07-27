
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

#ifndef AXIOM_DOCUMENT_H
#define AXIOM_DOCUMENT_H

#include <axutil_env.h>
#include <axiom_node.h>
#include <axutil_utils_defines.h>
#include <axiom_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @file axiom_document.h
     * @brief om_document represents an XML document
     */

#define CHAR_SET_ENCODING "UTF-8"
#define XML_VERSION   "1.0"

    struct axiom_stax_builder;

    /**
     * @defgroup axiom_document document
     * @ingroup axiom_om
     * @{
     */

    typedef struct axiom_document axiom_document_t;

    /**
      * Gets the root element of the document.
      * @param document document to return the root of
      * @param env Environment. MUST NOT be NULL.  
      * @return returns a pointer to the root node. If no root present,
      *            this method tries to build the root. Returns NULL on error. 
      */
    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    axiom_document_get_root_element(
        axiom_document_t *document,
        const axutil_env_t * env);

    /**
     * This method builds the rest of the xml input stream from current position till
     * the root element is completed .
     * @param document pointer to axiom_document_t struct to be built.
     * @param env environment MUST NOT be NULL.
     */
    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    axiom_document_build_all(
        axiom_document_t *document,
        const axutil_env_t * env);

    /** Builds the next node if the builder is not finished with input xml stream
     * @param document document whose next node is to be built. cannot be NULL
     * @param env Environment. MUST NOT be NULL.
     * @return pointer to the next node. NULL on error.
     */
    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    axiom_document_build_next(
        struct axiom_document *document,
        const axutil_env_t * env);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_DOCUMENT_H */
