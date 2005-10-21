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

#include <axis2_environment.h>
#include <axis2_om_node.h>
#include <axis2_defines.h>

/*typedef struct axis2_om_stax_builder_t axis2_om_stax_builder_t;*/

/**
 * @file axis2_om_node.h
 * @brief om_document represents a xml document
 *	
 */

typedef struct axis2_om_stax_builder axis2_om_stax_builder_t;

#define CHAR_SET_ENCODING "UTF-8"
#define XML_VERSION	"1.0"

/** @cond */
struct axis2_om_document;
struct axis2_om_document_ops;
/** @endcond */

/** @struct axis2_om_document_ops
    @brief OM document operations struct

    Encapsulator struct for operations of axis2_om_document_t
*/
typedef struct axis2_om_document_ops
{

    /** 
     *  Free axis2_om_document_t struct
     *	@param document 
     */
    axis2_status_t (*axis2_om_document_ops_free) (axis2_environment_t *
                                                  environment,
                                                  struct axis2_om_document *
                                                  document);

    /**
     *	causes the parser to proceed if the xml input is not finised yet
     *	@param document 
     */
    axis2_om_node_t *(*axis2_om_document_ops_build_next) (axis2_environment_t
                                                          * environment,
                                                          struct
                                                          axis2_om_document *
                                                          document);

    /**
     *	adds the child node as a child to the back of the list
     */
      axis2_status_t (*axis2_om_document_ops_add_child) (axis2_environment_t *
                                                         environment,
                                                         struct
                                                         axis2_om_document *
                                                         document,
                                                         axis2_om_node_t *
                                                         child);

    /**
     *	This cause the parser to proceed 	
     *	@param document
     *	@ returns The first 
     */
    axis2_om_node_t
        *(*axis2_om_document_ops_get_root_element) (axis2_environment_t *
                                                    environment,
                                                    struct axis2_om_document *
                                                    document);

    axis2_om_node_t
        *(*axis2_om_document_ops_get_next_sibling) (axis2_environment_t *
                                                    environment,
                                                    struct axis2_om_document *
                                                    document);


    axis2_om_node_t
        *(*axis2_om_document_ops_get_first_child) (axis2_environment_t *
                                                   environment,
                                                   struct axis2_om_document *
                                                   document);

    axis2_om_node_t
        *(*axis2_om_document_ops_get_next_child) (axis2_environment_t *
                                                  environment,
                                                  struct axis2_om_document *
                                                  document);
} axis2_om_document_ops_t;


typedef struct axis2_om_document
{
    axis2_om_document_ops_t *ops;
    axis2_om_node_t *root_element;
    axis2_om_node_t *last_child;
    axis2_om_node_t *first_child;
    axis2_bool_t done;
    axis2_om_stax_builder_t *builder;
    axis2_char_t *char_set_encoding;
    axis2_char_t *xml_version;

} axis2_om_document_t;

/**
    *	creates and returns axis2_om_document returns null if there isn't enough memory
    *  @param root pointer to document's root node IF NUll is parsed a document root
    *         will be set to NULL
    *  @param builder pointer to xml builder 
    */
axis2_om_document_t *axis2_om_document_ops_create (axis2_environment_t *
                                                   environment,
                                                   axis2_om_node_t * root,
                                                   axis2_om_stax_builder_t *
                                                   builder);

#define axis2_om_document_free(environment, document) ((document)->ops->axis2_om_document_ops_free(environment, document))
#define axis2_om_document_add_child(environment, document, child) ((document)->ops->axis2_om_document_ops_add_child(environment, document, child))
#define axis2_om_document_build_next(environment, document) ((document)->ops->axis2_om_document_ops_build_next(environment, document))
#define axis2_om_document_get_root_element(environment, document) ((document)->ops->axis2_om_document_ops_get_root_element(environment, document))
#define axis2_om_document_get_next_sibling(environment, document) ((document)->ops->axis2_om_document_ops_get_next_sibling(environment, document))
#define axis2_om_document_get_first_child(environment, document) ((document)->ops->axis2_om_document_ops_get_first_child(environment, document))
#define axis2_om_document_get_next_child(environment, document) ((document)->ops->axis2_om_document_ops_get_next_child(environment, document))

/*	to implement
 *	get first child with qname
 *	get children iterator
 *	get children with name
 */

#endif /* AXIS2_OM_DOCUMENT_H */
