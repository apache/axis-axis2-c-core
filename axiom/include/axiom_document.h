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

#include <axis2_env.h>
#include <axiom_node.h>
#include <axis2_utils_defines.h>
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

    struct axiom_document; 
    struct axiom_document_ops;
    struct axiom_stax_builder;

/**
 * @defgroup axiom_document document
 * @ingroup axiom_om
 * @{
 */

  /** 
    * @brief document ops struct
    * Encapsulator struct for ops of axiom_document_t
    */
    typedef struct axiom_document_ops
    {
        int dummy; /* if this is missing, windows compiler goes crazy */
    } axiom_document_ops_t;

  /**
    * \brief document struct
    * Handles the XML document in AXIOM
    */
    typedef struct axiom_document
    {
        /** ops of document struct */
        const axiom_document_ops_t *ops;
    } axiom_document_t;

  /**
    * creates an axiom_document_t struct
    * @param env Environment. MUST NOT be NULL.
    * @param root pointer to document's root node. Optional, can be NULL
    * @param builder pointer to axiom_stax_builder 
    * @return pointer to the newly created document.
    */
    AXIS2_EXTERN axiom_document_t * AXIS2_CALL
    axiom_document_create (const axis2_env_t *env,
                              axiom_node_t * root,
                              struct axiom_stax_builder *builder);
  /** 
    * Free document struct
    * @param document pointer to axiom_document_t struct to be freed
    * @param env Environment. MUST NOT be NULL    
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_document_free (struct axiom_document *document,
           const axis2_env_t *env);

  /** Builds the next node if the builder is not finished with input xml stream
    * @param document document whose next node is to be built. cannot be NULL
    * @param env Environment. MUST NOT be NULL.        
    * @return pointer to the next node. NULL on error.
    */
  
    AXIS2_EXTERN axiom_node_t* AXIS2_CALL 
    axiom_document_build_next (struct axiom_document *document,
               const axis2_env_t *env);

  /**
    * Gets the root element of the document.
    * @param document document to return the root of
    * @param env Environment. MUST NOT be NULL.        
    * @return returns a pointer to the root node. If no root present,
    *            this method tries to build the root. Returns NULL on error. 
    */
    AXIS2_EXTERN axiom_node_t* AXIS2_CALL 
    axiom_document_get_root_element(struct axiom_document *document,
                      const axis2_env_t *env);

  /**
    * set the root element of the document. IF a root node is already exist,it is freed 
    * before setting to root element 
    * @param document document struct to return the root of
    * @param env Environment. MUST NOT be NULL.        
    * @return returns status code AXIS2_SUCCESS on success ,AXIS2_FAILURE on error. 
    */        
                                                    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_document_set_root_element(struct axiom_document *document,
                      const axis2_env_t *env,
                      axiom_node_t *om_node);
   /**
    * This method builds the rest of the xml input stream from current position till
    * the root element is completed .
    *@param document pointer to axiom_document_t struct to be built.
    *@param env environment MUST NOT be NULL.
    */
    AXIS2_EXTERN axiom_node_t* AXIS2_CALL 
    axiom_document_build_all(struct axiom_document *document,
               const axis2_env_t *env); 
    /**
     * get builder
     * @return builder , returns NULL if a builder is not associated with 
     *                   document
     */
    AXIS2_EXTERN struct axiom_stax_builder* AXIS2_CALL 
    axiom_document_get_builder(struct axiom_document *document,
                 const axis2_env_t *env);                                                         
    /**
     * @param om_document
     * @return status code AXIS2_SUCCESS on success , otherwise AXIS2_FAILURE
     */                                                  
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_document_serialize(struct axiom_document *document,
               const axis2_env_t *env,
               axiom_output_t *om_output);

/** frees given document */
#define AXIOM_DOCUMENT_FREE(document,env) \
        axiom_document_free(document,env)
        
/** builds next node of document */
#define AXIOM_DOCUMENT_BUILD_NEXT(document,env) \
        axiom_document_build_next(document,env)
        
/** gets the root eleemnt of given document */
#define AXIOM_DOCUMENT_GET_ROOT_ELEMENT(document,env) \
        axiom_document_get_root_element(document,env)
/** sets the root node */
#define AXIOM_DOCUMENT_SET_ROOT_ELEMENT(document,env,om_node) \
        axiom_document_set_root_element(document,env,om_node)
               
/** build till the root node is complete */
#define AXIOM_DOCUMENT_BUILD_ALL(document,env) \
        axiom_document_build_all(document,env)
        
/** returns the builder */
#define AXIOM_DOCUMENT_GET_BUILDER(document, env) \
        axiom_document_get_builder(document, env)
        
/** serialize opertation */
#define AXIOM_DOCUMENT_SERIALIZE(document, env, om_output) \
        axiom_document_serialize(document, env, om_output)

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIOM_DOCUMENT_H */
