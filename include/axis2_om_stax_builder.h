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

struct axis2_om_stax_builder;
struct axis2_om_stax_builder_ops;

typedef struct axis2_om_stax_builder_ops
{

/**
 *	move parser forward and reacts to events 
 */

axis2_om_node_t *(*axis2_om_stax_builder_ops_next)(axis2_environment_t *environment, axis2_om_stax_builder_t *builder);

axis2_status_t (*axis2_om_stax_builder_ops_discard_current_element)(axis2_environment_t *environment, axis2_om_stax_builder_t *builder);


} axis2_om_stax_builder_ops_t;

struct axis2_om_stax_builder
{
	axis2_om_stax_builder_ops_t *ops;
	void *parser;	
	axis2_om_node_t *lastnode;
	axis2_om_document_t	*document;
	axis2_bool_t done;
	axis2_bool_t parser_accessed;
	axis2_bool_t cache;
};

/**
 *	create an stax_om_builder
 * @param parser   This is the XMLStreamReader of xmlparser
 *				   [ Guththila ]	
 */

axis2_om_stax_builder_t *axis2_om_stax_builder_create(axis2_environment_t *environment, void *parser);

#define axis2_om_stax_builder_next(environment, builder) ((builder)->ops->axis2_om_stax_builder_ops_next(environment, builder))
#define axis2_om_stax_builder_discard_current_element(environment, builder) ((builder)->ops->axis2_om_stax_builder_ops_discard_current_element(environment, builder))


#endif /* AXIS2_OM_STAX_BUILDER_H */
