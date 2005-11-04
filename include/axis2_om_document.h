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
    * @brief OM document operations struct
    * Encapsulator struct for operations of axis2_om_document_t
    */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_document_ops
    {

      /** 
        * Frees document struct
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document pointer to document struct to be freed
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *axis2_om_document_ops_free) (axis2_env_t *
                                                      environment,
                                                      struct axis2_om_document
                                                      * document);

      /**
        * Builds the next node if the builder is not finished with input xml stream
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document whose next node is to be built. Mandatory, cannot be NULL
        * @return pointer to the next node. NULL on error.
        */
        axis2_om_node_t
            *(AXIS2_CALL *axis2_om_document_ops_build_next) (axis2_env_t *
                                                  environment,
                                                  struct axis2_om_document *
                                                  document);

      /**
        * adds the child node to the document. To the back of the children list.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to add the child. Mandatory, cannot be NULL.
        * @param child child node to be added. Mandatory, cannot be NULL.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *axis2_om_document_ops_add_child)
            (axis2_env_t * environment,
             struct axis2_om_document * document, axis2_om_node_t * child);

      /**
        * Gets the root element of the document.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to return the root of
        * @return returns a pointer to the root node. If no root present, this method tries to 
        *             build the root. Returns NULL on error. 
        */
        axis2_om_node_t
            * (AXIS2_CALL *axis2_om_document_ops_get_root_element) (axis2_env_t *
                                                         environment,
                                                         struct
                                                         axis2_om_document *
                                                         document);

      /**
        * Gets the next sibling of the current element of document's builder
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to be used
        * @return returns a pointer to the next sibling. Tries to build the document
        *           till the next sibling is found. Returns NULL if no sibling is present.
        *           On error, returns NULL and set the error. 
        */
        axis2_om_node_t
            * (AXIS2_CALL *axis2_om_document_ops_get_next_sibling) (axis2_env_t *
                                                         environment,
                                                         struct
                                                         axis2_om_document *
                                                         document);

      /**
        * Gets the first child of the document's root node. get_root_element must have been 
        *       caled before this method is called.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to be used
        * @return returns a pointer to the first child. Tries to build the document
        *           till the first child is found. Returns NULL if no child is present.
        *           On error, returns NULL and sets the error. 
        */
        axis2_om_node_t
            * (AXIS2_CALL *axis2_om_document_ops_get_first_child) (axis2_env_t *
                                                        environment,
                                                        struct
                                                        axis2_om_document *
                                                        document);

      /**
        * Gets the next child of the document's current node. get_first_child must have been 
        *       caled before this method is called.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.        
        * @param document document to be used
        * @return returns a pointer to the next child. Tries to build the document
        *           till the next child is found. Returns NULL if no child is present.
        *           On error, returns NULL and sets the error. 
        */
        axis2_om_node_t
            * (AXIS2_CALL *axis2_om_document_ops_get_next_child) (axis2_env_t *
                                                       environment,
                                                       struct
                                                       axis2_om_document *
                                                       document);
    } axis2_om_document_ops_t;

  /**
    * \brief OM document struct
    * Handles the XML document in OM
    */
    typedef struct axis2_om_document
    {
        /** operations of document struct */
        axis2_om_document_ops_t *ops;
        /** root element */
        axis2_om_node_t *root_element;
        /** last child */
        axis2_om_node_t *last_child;
        /** first child */
        axis2_om_node_t *first_child;
        /** done building the document */
        axis2_bool_t done;
        /** builder of the document */
        struct axis2_om_stax_builder *builder;
        /** char set encoding */        
        axis2_char_t *char_set_encoding;
        /** XML version */
        axis2_char_t *xml_version;
    } axis2_om_document_t;

  /**
    * creates a document
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param root pointer to document's root node. Optional, can be NULL
    * @param builder pointer to xml builder 
    * @return pointer to the newly created document.
    */
    AXIS2_DECLARE(axis2_om_document_t *) axis2_om_document_create (axis2_env_t *
                                                       environment,
                                                       axis2_om_node_t * root,
                                                       struct
                                                       axis2_om_stax_builder
                                                       *builder);

/** frees given document */
#define axis2_om_document_free(environment, document) ((document)->ops->axis2_om_document_ops_free(environment, document))
/** adds a child to document */
#define axis2_om_document_add_child(environment, document, child) ((document)->ops->axis2_om_document_ops_add_child(environment, document, child))
/** builds next node of document */
#define axis2_om_document_build_next(environment, document) ((document)->ops->axis2_om_document_ops_build_next(environment, document))
/** gets the root eleemnt of given document */
#define axis2_om_document_get_root_element(environment, document) ((document)->ops->axis2_om_document_ops_get_root_element(environment, document))
/** gets the next sibling of given document's current node */
#define axis2_om_document_get_next_sibling(environment, document) ((document)->ops->axis2_om_document_ops_get_next_sibling(environment, document))
/** gets the first child of given document's root node */
#define axis2_om_document_get_first_child(environment, document) ((document)->ops->axis2_om_document_ops_get_first_child(environment, document))
/** gets the next child of given document's current node */
#define axis2_om_document_get_next_child(environment, document) ((document)->ops->axis2_om_document_ops_get_next_child(environment, document))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_DOCUMENT_H */
