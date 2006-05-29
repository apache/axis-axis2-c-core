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

#ifndef AXIS2_XML_SCHEMA_BUILDER_H
#define AXIS2_XML_SCHEMA_BUILDER_H

/**
 * @file axis2_xml_schema_builder.h
 * @brief Axis2 Xml Schema Interface
 *          Contains the definition of a schema. All XML Schema definition 
 *          language (XSD) elements are children of the schema element. 
 *          Represents the World Wide Web Consortium (W3C) schema element
 */

#include <xml_schema/axis2_xml_schema_defines.h>
#include <xml_schema/axis2_xml_schema_annotated.h>
#include <xml_schema/axis2_xml_schema.h>
#include <axis2_om.h>
#include <axis2_hash.h>
#include <xml_schema/axis2_xml_schema_collection.h>
/** @defgroup axis2_xml_schema_builder Xml Schema
  * @ingroup axis2_xml_schema_builder
  * @{
  */

typedef struct axis2_xml_schema_builder 
                    axis2_xml_schema_builder_t;
typedef struct axis2_xml_schema_builder_ops 
                    axis2_xml_schema_builder_ops_t;

#define AXIS2_XML_SCHEMA_NS "http://www.w3.org/2001/XMLSchema"

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_builder_ops
{
    axis2_xml_schema_t* (AXIS2_CALL *
    build)(
            axis2_xml_schema_builder_t *builder,
            axis2_env_t **env,
            axis2_om_document_t *om_doc,
            axis2_char_t *uri);
           
    axis2_xml_schema_t* (AXIS2_CALL *
    build_with_root_node)(
            axis2_xml_schema_builder_t *builder,
            axis2_env_t **env,
            axis2_om_node_t *om_node,
            axis2_char_t *uri);
            
    axis2_status_t (AXIS2_CALL *
    free)(axis2_xml_schema_builder_t *builder,
          axis2_env_t **env);            
};

struct axis2_xml_schema_builder
{
    axis2_xml_schema_builder_ops_t *ops;
};


AXIS2_DECLARE(axis2_xml_schema_builder_t *)
axis2_xml_schema_builder_create(
        axis2_env_t **env,
        axis2_xml_schema_collection_t *sch_collection);

/***************** Macros *****************************************************/

#define AXIS2_XML_SCHEMA_BUILDER_FREE(builder, env) \
        ((builder)->ops->free(builder, env))

#define AXIS2_XML_SCHEMA_BUILDER_BUILD(builder, env, om_doc, uri) \
        ((builder)->ops->build(builder, env, om_doc, uri))
        
#define AXIS2_XML_SCHEMA_BUILDER_BUILD_WITH_ROOT_NODE(builder, env, om_node, uri) \
        ((builder)->ops->build_with_root_node(builder, env, om_node, uri))
        
/**************** end macros **************************************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_BUILDER_H */
