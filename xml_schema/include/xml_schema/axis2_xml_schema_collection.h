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

#ifndef AXIS2_XML_SCHEMA_COLLECTION_H
#define AXIS2_XML_SCHEMA_COLLECTION_H

/**
 * @file axis2_xml_schema_collection.h
 * @brief Axis2 Xml Schema Collection  interface
 *          Contains a cache of XML Schema definition language (XSD).
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>

#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <xml_schema/axis2_xml_schema_obj.h>
#include <axis2_xml_reader.h>
#include <axis2_om_document.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_collection axis2_xml_schema_collection_t;
typedef struct axis2_xml_schema_collection_ops axis2_xml_schema_collection_ops_t;
struct axis2_xml_schema;
struct axis2_om_node;
struct axis2_xml_schema_type_receiver;
struct axis2_xml_schema_type;

/** @defgroup axis2_xml_schema_collection Xml Schema Collection
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_collection_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_base_uri)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_char_t *base_uri);
                   
    axis2_status_t (AXIS2_CALL * 
    init)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env);


    struct axis2_xml_schema* (AXIS2_CALL *
    read_with_reader)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_xml_reader_t *reader); 

    
    struct axis2_xml_schema *(AXIS2_CALL *
    read_document) (
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_om_document_t *document);

    struct axis2_xml_schema *(AXIS2_CALL *
    read_element)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_om_node_t *ele_node);

    struct axis2_xml_schema* (AXIS2_CALL *
    read_document_with_uri) (
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_om_document_t *document,
            axis2_char_t *uri); 

    struct axis2_xml_schema *(AXIS2_CALL *
    read_element_with_uri)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_om_node_t *ele_node,
            axis2_char_t *uri);
            
    const axis2_hash_t* (AXIS2_CALL *
    get_xml_schema)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_char_t *system_id);
            
    struct axis2_xml_schema_element *(AXIS2_CALL *
    get_element_by_qname)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_qname_t *qname);

    struct axis2_xml_schema_type *(AXIS2_CALL *
    get_type_by_qname)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_qname_t *schema_type_qname);


    axis2_status_t (AXIS2_CALL *
    add_unresolved_type)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_qname_t *qtype, 
            void *receiver); 
            

    axis2_status_t (AXIS2_CALL *
    resolve_type)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_qname_t *type_qname, 
            struct axis2_xml_schema_type *type); 
            

    axis2_char_t *(AXIS2_CALL *
    get_namespace_for_prefix)(
            axis2_xml_schema_collection_t *collection,
            axis2_env_t **env,
            axis2_char_t *prefix); 
                     

    axis2_status_t (AXIS2_CALL *
    map_namespace)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_char_t *prefix, 
            axis2_char_t *namespc_uri);
            
    axis2_hash_t* (AXIS2_CALL *
    get_systemid2_schemas)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env);
            
    axis2_array_list_t* (AXIS2_CALL *
    get_schemas)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env);
            
    axis2_status_t* (AXIS2_CALL *
    set_schemas)(
            axis2_xml_schema_collection_t *collection,
            axis2_env_t **env,
            axis2_array_list_t *schemas);            
                      
    axis2_hash_t* (AXIS2_CALL *
    get_namespaces)(
            axis2_xml_schema_collection_t *collection,
            axis2_env_t **env);
            
    axis2_status_t (AXIS2_CALL *
    set_namespaces)(
            axis2_xml_schema_collection_t *collection,
            axis2_env_t **env);
            
                            
};            

struct axis2_xml_schema_collection
{
    axis2_xml_schema_collection_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_collection_t *)
axis2_xml_schema_collection_create(axis2_env_t **env);

/***************** MACROS *****************************************************/

#define AXIS2_XML_SCHEMA_COLLECTION_FREE(collection, env) \
		((collection)->ops->free (collection, env))
		
#define AXIS2_XML_SCHEMA_COLLECTION_SET_BASE_URI(collection, env, base_uri) \
		((collection)->ops->set_base_uri (collection, env, base_uri))
		
#define AXIS2_XML_SCHEMA_COLLECTION_INIT(collection, env) \
		((collection)->ops->init(collection, env))
		
#define AXIS2_XML_SCHEMA_COLLECTION_READ_WITH_READER(collection, env, reader) \
        ((collection)->ops->read_with_reader( \
        collection, env, reader))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_DOCUMENT(collection, env, doc) \
        ((collection)->ops->read_document(collection, env, doc))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_DOCUMENT_WITH_URI(collection , env, document, uri) \
        ((collection)->ops->read_document_with_uri( \
        collection , env, document, uri))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_ELEMENT(collection, env, uri) \
        ((collection)->ops->read_element(collection, env, uri))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_ELEMENT_WITH_URI(collection, env, uri ) \
        ((collection)->ops->read_element_with_uri(collection, env, uri ))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_ELEMENT_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_element_by_qname(collection, env, qname))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_TYPE_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_type_by_qname(collection, env, qname))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_ELEMENT_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_element_by_qname(collection, env, qname))
        
#define AXIS2_XML_SCHEMA_COLLECTION_ADD_UNRESOLVED_TYPE(collection, env, qtype, receiver) \
        ((collection)->ops->add_unresolved_type(collection, env, qtype, receiver))
        
#define AXIS2_XML_SCHEMA_COLLECTION_RESOLVE_TYPE(collection, env, type_qname, type) \
        ((collection)->ops->resolve_type(collection, env, type_qname, type))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_NAMESPACE_FOR_PREFIX(collection, env, prefix) \
        ((collection)->ops->get_namespace_for_prefix(collection, env, prefix))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_SYSTEMID2_SCHEMAS(collection, env) \
        ((collection)->ops->get_systemid2_schemas(collection, env))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_SCHEMAS(collection, env) \
        ((collection)->ops->get_schemas(collection, env))
        
#define AXIS2_XML_SCHEMA_COLLECTION_SET_SCHEMAS(collection, env) \
        ((collection)->ops->set_schemas(collection, env))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_NAMESPACES(collection, env) \
        ((collection)->ops->get_namespaces(collection, env))
        
#define AXIS2_XML_SCHEMA_COLLECTION_SET_NAMESPACES(collection, env, ht_ns) \
        ((collection)->ops->set_namespaces(collection, env, ht_ns))                
                
        
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_COLLECTION_H */
