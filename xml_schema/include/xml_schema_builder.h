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

#ifndef XML_SCHEMA_BUILDER_H
#define XML_SCHEMA_BUILDER_H

/**
 * @file xml_schema_builder.h
 * @brief Axis2 Xml Schema Interface
 *          Contains the definition of a schema. All XML Schema definition 
 *          language (XSD) elements are children of the schema element. 
 *          Represents the World Wide Web Consortium (W3C) schema element
 */

#include <xml_schema_defines.h>
#include <xml_schema_annotated.h>
#include <xml_schema.h>
#include <axiom.h>
#include <axis2_hash.h>
#include <xml_schema_collection.h>
#include <axiom_util.h>
/** @defgroup xml_schema_builder Xml Schema
  * @ingroup xml_schema_builder
  * @{
  */


#ifdef __cplusplus
extern "C"
{
#endif



typedef struct xml_schema_builder 
                    xml_schema_builder_t;
typedef struct xml_schema_builder_ops 
                    xml_schema_builder_ops_t;

#define XML_SCHEMA_NS "http://www.w3.org/2001/XMLSchema"


struct xml_schema_builder_ops
{
    xml_schema_t* (AXIS2_CALL *
    build)(
            xml_schema_builder_t *builder,
            const axutil_env_t *env,
            axiom_document_t *om_doc,
            axis2_char_t *uri);
           
    xml_schema_t* (AXIS2_CALL *
    build_with_root_node)(
            xml_schema_builder_t *builder,
            const axutil_env_t *env,
            axiom_node_t *om_node,
            axis2_char_t *uri);
            
    axis2_status_t (AXIS2_CALL *
    free)(xml_schema_builder_t *builder,
          const axutil_env_t *env);            
};

struct xml_schema_builder
{
    xml_schema_builder_ops_t *ops;
};


AXIS2_EXTERN xml_schema_builder_t * AXIS2_CALL
xml_schema_builder_create(
        const axutil_env_t *env,
        xml_schema_collection_t *sch_collection);

/***************** Macros *****************************************************/

#define XML_SCHEMA_BUILDER_FREE(builder, env) \
        ((builder)->ops->free(builder, env))

#define XML_SCHEMA_BUILDER_BUILD(builder, env, om_doc, uri) \
        ((builder)->ops->build(builder, env, om_doc, uri))
        
#define XML_SCHEMA_BUILDER_BUILD_WITH_ROOT_NODE(builder, env, om_node, uri) \
        ((builder)->ops->build_with_root_node(builder, env, om_node, uri))
        
/**************** end macros **************************************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_BUILDER_H */
