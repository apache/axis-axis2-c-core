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

#ifndef AXIS2_OM_STAX_BUILDER_H
#define AXIS2_OM_STAX_BUILDER_H

/**
 * @file axis2_om_stax_builder.h
 * @brief om model stax builder 
 */

#include <axis2_om_node.h>
#include <axis2_xml_reader.h>
#include <axis2_om_document.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif


    struct axis2_om_stax_builder;
    struct axis2_om_stax_builder_ops;

/**
 * @defgroup axis2_om_stax_builder OM Stax Builder
 * @ingroup axis2_om 
 * @{
 */

  /** 
    * @brief OM stax builder ops struct
    * Encapsulator struct for ops of axis2_om_stax_builder
    */
 AXIS2_DECLARE_DATA    typedef struct axis2_om_stax_builder_ops
    {
      /**
        * Builds the next node from stream. Moves pull parser forward and reacts to events.
        * @param environment Environment. MUST NOT be NULL.
        * @param builder pointer to stax builder struct to be used
        * @return a pointer to the next node, or NULL if there are no more nodes.
        *           On erros sets the error and returns NULL.
        */
        axis2_om_node_t*(AXIS2_CALL *next) (struct axis2_om_stax_builder *builder,
                                            axis2_env_t **env);

      /**
        * Discards the element that is being built currently.
        * @param environment Environment. MUST NOT be NULL, .
        * @param builder pointer to stax builder struct to be used
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *discard_current_element)
                                    (struct axis2_om_stax_builder *builder,
                                     axis2_env_t **env);
        /**
         * Free op
         * @param builder pointer to builder struct
         * @param env environment, MUST NOT be NULL
         * @return status of the op AXIS2_SUCCESS on success,
         *         AXIS2_FAILURE on error.
         */
			 
		axis2_status_t (AXIS2_CALL *free)(struct axis2_om_stax_builder *builder,
		                                  axis2_env_t **env);
        
       /** get the document associated with the builder  
        * @param builder axis2_om_stax_builder 
        * @param env environment 
        * @return pointer to document struct associated with builder
        *         NULL if there is no document associated with the builder,
        *         NULL if an error occured.
        */                                             
        axis2_om_document_t* (AXIS2_CALL *get_document)
                                         (struct axis2_om_stax_builder *builder,
                                          axis2_env_t **env);
        /**
         *  set the document associated with the builder
         * @param builder pointer to builder struct
         * @param env environment , MUST NOT be NULL.
         * @param document pointer to document struct that needs to be associated 
         *                  with builder
         * @return status of the op. AXIS2_SUCCESS on success and AXIS2_FAILURE 
         *                  on error.
         */                                          

        axis2_status_t  (AXIS2_CALL *set_document)(struct axis2_om_stax_builder *builder,
                                                   axis2_env_t **env,
                                                   axis2_om_document_t *document);                                                    
                                                           
    } axis2_om_stax_builder_ops_t;

  /** 
    * \brief OM stax builder struct
    * Bulds OM document using a pull parser interface
    */
    typedef struct axis2_om_stax_builder
    {
        /** ops struct */
        axis2_om_stax_builder_ops_t *ops;
       
    }axis2_om_stax_builder_t;

  /**
    * creates an stax builder
    * @param environment Environment. MUST NOT be NULL, .
    * @return a pointer to the newly created builder struct. 
    */
    AXIS2_DECLARE(axis2_om_stax_builder_t *)
    axis2_om_stax_builder_create(axis2_env_t **env,
                                           axis2_xml_reader_t *parser);


/** builds next node */
#define AXIS2_OM_STAX_BUILDER_NEXT(builder,env) \
        ((builder)->ops->next(builder, env))
/** discards current node */
#define AXIS2_OM_STAX_BUILDER_DISCARD_CURRENT_ELEMENT(builder,env) \
        ((builder)->ops->discard_current_element(builder, env))
/** free op of the builder */
#define AXIS2_OM_STAX_BUILDER_FREE(builder,env) \
        ((builder)->ops->free(builder,env))
/** associate a document with a builder */       
#define AXIS2_OM_STAX_BUILDER_SET_DOCUMENT(builder,env,document) \
        ((builder)->ops->set_document(builder,env,document))
/** get the document associated with the builder */  
#define AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder,env) \
        ((builder)->ops->get_document(builder,env))
	
/** @} */


#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_OM_STAX_BUILDER_H */
