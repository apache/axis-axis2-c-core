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

#ifndef XML_SCHEMA_COLLECTION_H
#define XML_SCHEMA_COLLECTION_H

/**
 * @file xml_schema_collection.h
 * @brief Axis2 Xml Schema Collection  interface
 *          Contains a cache of XML Schema definition language (XSD).
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>

#include <axutil_array_list.h>
#include <axutil_hash.h>
#include <axis2_qname.h>
#include <xml_schema_obj.h>
#include <axiom_xml_reader.h>
#include <axiom_document.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_collection 
                xml_schema_collection_t;
typedef struct xml_schema_collection_ops 
                xml_schema_collection_ops_t;
struct xml_schema;
struct axiom_node;
struct xml_schema_type;

/** @defgroup xml_schema_collection Xml Schema Collection
  * @ingroup xml_schema
  * @{
  */

struct xml_schema_collection_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_base_uri)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_char_t *base_uri);
            
    axis2_char_t* (AXIS2_CALL*
    get_base_uri)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env);
                        
                   
    axis2_status_t (AXIS2_CALL * 
    init)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env);


    struct xml_schema* (AXIS2_CALL *
    read_with_reader)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axiom_xml_reader_t *reader); 

    
    struct xml_schema *(AXIS2_CALL *
    read_document) (
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axiom_document_t *document);

    struct xml_schema *(AXIS2_CALL *
    read_element)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axiom_node_t *ele_node);

    struct xml_schema* (AXIS2_CALL *
    read_document_with_uri) (
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axiom_document_t *document,
            axis2_char_t *uri); 

    struct xml_schema *(AXIS2_CALL *
    read_element_with_uri)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axiom_node_t *ele_node,
            axis2_char_t *uri);
            
    struct xml_schema* (AXIS2_CALL *
    get_schema)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_char_t *system_id);
            
    struct xml_schema_element *(AXIS2_CALL *
    get_element_by_qname)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    struct xml_schema_type *(AXIS2_CALL *
    get_type_by_qname)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_qname_t *schema_type_qname);


    axis2_status_t (AXIS2_CALL *
    add_unresolved_type)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_qname_t *qtype, 
            void *receiver); 
            

    axis2_status_t (AXIS2_CALL *
    resolve_type)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_qname_t *type_qname, 
            struct xml_schema_type *type); 
            

    axis2_char_t *(AXIS2_CALL *
    get_namespace_for_prefix)(
            xml_schema_collection_t *collection,
            const axutil_env_t *env,
            axis2_char_t *prefix); 
                     

    axis2_status_t (AXIS2_CALL *
    map_namespace)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env,
            axis2_char_t *prefix, 
            axis2_char_t *namespc_uri);
            
    axutil_hash_t* (AXIS2_CALL *
    get_systemid2_schemas)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env);
            
    axutil_array_list_t* (AXIS2_CALL *
    get_schemas)(
            xml_schema_collection_t* collection,
            const axutil_env_t *env);
            
    axis2_status_t* (AXIS2_CALL *
    set_schemas)(
            xml_schema_collection_t *collection,
            const axutil_env_t *env,
            axutil_array_list_t *schemas);            
                      
    axutil_hash_t* (AXIS2_CALL *
    get_namespaces)(
            xml_schema_collection_t *collection,
            const axutil_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_namespaces)(
            xml_schema_collection_t *collection,
            const axutil_env_t *env);
            
                            
};            

struct xml_schema_collection
{
    xml_schema_collection_ops_t *ops;
};

AXIS2_EXTERN xml_schema_collection_t * AXIS2_CALL
xml_schema_collection_create(const axutil_env_t *env);

/***************** MACROS *****************************************************/

#define XML_SCHEMA_COLLECTION_FREE(collection, env) \
      ((collection)->ops->free (collection, env))
      
#define XML_SCHEMA_COLLECTION_INIT(collection, env) \
      ((collection)->ops->init(collection, env))
      
#define XML_SCHEMA_COLLECTION_READ_WITH_READER(collection, env, reader) \
        ((collection)->ops->read_with_reader( \
        collection, env, reader))
        
#define XML_SCHEMA_COLLECTION_READ_DOCUMENT(collection, env, doc) \
        ((collection)->ops->read_document(collection, env, doc))
        
#define XML_SCHEMA_COLLECTION_READ_DOCUMENT_WITH_URI(collection , env, document, uri) \
        ((collection)->ops->read_document_with_uri( \
        collection , env, document, uri))
        
#define XML_SCHEMA_COLLECTION_READ_ELEMENT(collection, env, uri) \
        ((collection)->ops->read_element(collection, env, uri))
        
#define XML_SCHEMA_COLLECTION_READ_ELEMENT_WITH_URI(collection, env, ele_node, uri ) \
        ((collection)->ops->read_element_with_uri(collection, env, ele_node, uri ))
        
#define XML_SCHEMA_COLLECTION_GET_ELEMENT_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_element_by_qname(collection, env, qname))
        
#define XML_SCHEMA_COLLECTION_GET_TYPE_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_type_by_qname(collection, env, qname))
        
#define XML_SCHEMA_COLLECTION_GET_ELEMENT_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_element_by_qname(collection, env, qname))
        
#define XML_SCHEMA_COLLECTION_ADD_UNRESOLVED_TYPE(collection, env, qtype, receiver) \
        ((collection)->ops->add_unresolved_type(collection, env, qtype, receiver))
        
#define XML_SCHEMA_COLLECTION_RESOLVE_TYPE(collection, env, type_qname, type) \
        ((collection)->ops->resolve_type(collection, env, type_qname, type))
        
#define XML_SCHEMA_COLLECTION_GET_NAMESPACE_FOR_PREFIX(collection, env, prefix) \
        ((collection)->ops->get_namespace_for_prefix(collection, env, prefix))
        
#define XML_SCHEMA_COLLECTION_GET_SYSTEMID2_SCHEMAS(collection, env) \
        ((collection)->ops->get_systemid2_schemas(collection, env))
        
#define XML_SCHEMA_COLLECTION_GET_SCHEMAS(collection, env) \
        ((collection)->ops->get_schemas(collection, env))
        
#define XML_SCHEMA_COLLECTION_SET_SCHEMAS(collection, env) \
        ((collection)->ops->set_schemas(collection, env))
        
#define XML_SCHEMA_COLLECTION_GET_NAMESPACES(collection, env) \
        ((collection)->ops->get_namespaces(collection, env))
        
#define XML_SCHEMA_COLLECTION_SET_NAMESPACES(collection, env, ht_ns) \
        ((collection)->ops->set_namespaces(collection, env, ht_ns))                
        
#define XML_SCHEMA_COLLECTION_GET_SCHEMA(collection, env, system_id) \
        ((collection)->ops->get_schema(collection, env, system_id))
        
#define XML_SCHEMA_COLLECTION_SET_BASE_URI(collection, env, base_uri) \
      ((collection)->ops->set_base_uri (collection, env, base_uri))
      
#define XML_SCHEMA_COLLECTION_GET_BASE_URI(collection, env) \
        ((collection)->ops->get_base_uri(collection, env))
        
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_COLLECTION_H */
