/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_OM_DOCUMENT_H
#define AXIS2_OM_DOCUMENT_H

#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_defines.h>
#include <axis2_om_output.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @file axis2_om_document.h
 * @brief om_document represents an XML document
 */

#define CHAR_SET_ENCODING "UTF-8"
#define XML_VERSION	"1.0"

    struct axis2_om_document; 
    struct axis2_om_document_ops;
    struct axis2_om_stax_builder;

/**
 * @defgroup axis2_om_document OM Document
 * @ingroup axis2_om 
 * @{
 */

  /** 
    * @brief OM document ops struct
    * Encapsulator struct for ops of axis2_om_document_t
    */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_document_ops
    {

      /** 
        * Free document struct
        * @param document pointer to axis2_om_document_t struct to be freed
        * @param env Environment. MUST NOT be NULL    
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *free) (struct axis2_om_document *document,
                                           axis2_env_t **env);

      /** Builds the next node if the builder is not finished with input xml stream
        * @param document document whose next node is to be built. cannot be NULL
        * @param env Environment. MUST NOT be NULL.        
        * @return pointer to the next node. NULL on error.
        */
      
      axis2_om_node_t* (AXIS2_CALL *build_next) (struct axis2_om_document *document,
                                                   axis2_env_t **env);

      /**
        * Gets the root element of the document.
        * @param document document to return the root of
        * @param env Environment. MUST NOT be NULL.        
        * @return returns a pointer to the root node. If no root present,
        *            this method tries to build the root. Returns NULL on error. 
        */
        axis2_om_node_t* (AXIS2_CALL *get_root_element)(struct axis2_om_document *document,
                                                        axis2_env_t **env);

      /**
        * set the root element of the document. IF a root node is already exist,it is freed 
        * before setting to root element 
        * @param document document struct to return the root of
        * @param env Environment. MUST NOT be NULL.        
        * @return returns status code AXIS2_SUCCESS on success ,AXIS2_FAILURE on error. 
        */        
                                                        
        axis2_status_t (AXIS2_CALL *set_root_element)(struct axis2_om_document *document,
                                                       axis2_env_t **env,
                                                       axis2_om_node_t *om_node);
       /**
        * This method builds the rest of the xml input stream from current position till
        * the root element is completed .
        *@param document pointer to axis2_om_document_t struct to be built.
        *@param env environment MUST NOT be NULL.
        */
        axis2_om_node_t* (AXIS2_CALL *build_all)(struct axis2_om_document *document,
                                                axis2_env_t **env); 
        /**
         * get builder
         * @return builder , returns NULL if a builder is not associated with 
         *                   document
         */
        struct axis2_om_stax_builder* (AXIS2_CALL *get_builder)
                                                 (struct axis2_om_document *document,
                                                  axis2_env_t **env);                                                         
        /**
         * @param om_document
         * @return status code AXIS2_SUCCESS on success , otherwise AXIS2_FAILURE
         */                                                  
        axis2_status_t (AXIS2_CALL *serialize)(struct axis2_om_document *document,
                                               axis2_env_t **env,
                                               axis2_om_output_t *om_output);
                                                           
    } axis2_om_document_ops_t;

  /**
    * \brief OM document struct
    * Handles the XML document in OM
    */
    typedef struct axis2_om_document
    {
        /** ops of document struct */
        axis2_om_document_ops_t *ops;
      
    } axis2_om_document_t;

  /**
    * creates an axis2_om_document_t struct
    * @param env Environment. MUST NOT be NULL.
    * @param root pointer to document's root node. Optional, can be NULL
    * @param builder pointer to axis2_om_stax_builder 
    * @return pointer to the newly created document.
    */
    AXIS2_DECLARE(axis2_om_document_t *)
    axis2_om_document_create (axis2_env_t **env,
                              axis2_om_node_t * root,
                              struct axis2_om_stax_builder *builder);

/** frees given document */
#define AXIS2_OM_DOCUMENT_FREE(document,env) \
        ((document)->ops->free(document,env))
        
/** builds next node of document */
#define AXIS2_OM_DOCUMENT_BUILD_NEXT(document,env) \
        ((document)->ops->build_next(document,env))
        
/** gets the root eleemnt of given document */
#define AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document,env) \
        ((document)->ops->get_root_element(document,env))
/** sets the root node */
#define AXIS2_OM_DOCUMENT_SET_ROOT_ELEMENT(document,env,om_node) \
        ((document)->ops->set_root_element(document,env,om_node))
               
/** build till the root node is complete */
#define AXIS2_OM_DOCUMENT_BUILD_ALL(document,env) \
        ((document)->ops->build_all(document,env))
        
/** returns the builder */
#define AXIS2_OM_DOCUMENT_GET_BUILDER(document, env) \
        ((document)->ops->get_builder(document, env))
        
/** serialize opertation */
#define AXIS2_OM_DOCUMENT_SERIALIZE(document, env, om_output) \
        ((document)->ops->serialize(document, env, om_output))                

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_OM_DOCUMENT_H */
