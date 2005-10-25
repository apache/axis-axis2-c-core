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
#include <axis2_om_document.h>
#include <axis2_environment.h>

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
    * @brief OM stax builder operations struct
    * Encapsulator struct for operations of axis2_om_stax_builder
    */
    typedef struct axis2_om_stax_builder_ops
    {
      /**
        * Builds the next node from stream. Moves pull parser forward and reacts to events.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param builder pointer to stax builder struct to be used
        * @return a pointer to the next node, or NULL if there are no more nodes.
        *           On erros sets the error and returns NULL.
        */
        AXIS2_DECLARE_DATA axis2_om_node_t * 
            (AXIS2_CALL *axis2_om_stax_builder_ops_next) (axis2_environment_t *
                                                environment,
                                                struct axis2_om_stax_builder *
                                                builder);

      /**
        * Discards the element that is being built currently.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param builder pointer to stax builder struct to be used
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        AXIS2_DECLARE_DATA axis2_status_t (AXIS2_CALL *axis2_om_stax_builder_ops_discard_current_element)
            (axis2_environment_t * environment,
             struct axis2_om_stax_builder * builder);


    } axis2_om_stax_builder_ops_t;

  /** 
    * \brief OM stax builder struct
    * Bulds OM document using a pull parser interface
    */
    typedef struct axis2_om_stax_builder
    {
        /** operations struct */
        axis2_om_stax_builder_ops_t *ops;
        /** pull parser instance used by the builder */
        void *parser;
        /** last node the builder found */
        axis2_om_node_t *lastnode;
        /** document associated with the builder */
        axis2_om_document_t *document;
        /** done building the document? */
        axis2_bool_t done;
        /** parser was accessed? */
        axis2_bool_t parser_accessed;
        /** caching enabled? */
        axis2_bool_t cache;
    } axis2_om_stax_builder_t;

  /**
    * creates an stax builder
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param parser pull parser to be used by the builder. Parser should implement the
    *           Guththila interface. If NULL, default xml parser
    *           (guththila pull parser) will be created with stdin as the input stream.
	* @param parser_env parser envioronment. Optional, can be NULL.
    * @return a pointer to the newly created builder struct. 
    */
    AXIS2_DECLARE( axis2_om_stax_builder_t *) axis2_om_stax_builder_create (axis2_environment_t
                                                           * environment,
                                                           void *parser, void* parser_env);

/** builds next node */
#define axis2_om_stax_builder_next(environment, builder) ((builder)->ops->axis2_om_stax_builder_ops_next(environment, builder))
/** discards current node */
#define axis2_om_stax_builder_discard_current_element(environment, builder) ((builder)->ops->axis2_om_stax_builder_ops_discard_current_element(environment, builder))

/** @} */


#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_OM_STAX_BUILDER_H */
