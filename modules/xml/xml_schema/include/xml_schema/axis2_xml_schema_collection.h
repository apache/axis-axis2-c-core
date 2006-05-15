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
#include <axis2_const.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <xml_schema/axis2_xml_schema_obj.h>
#include <xml_schema/axis2_validation_event_handler.h>

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
    free) (axis2_xml_schema_collection_t* collection,
           axis2_env_t **env);

    /**
     * Set the base URI. This is used when schemas need to be
     * loaded from relative locations
     * @param base_uri
     */
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
    read_a_reader_and_validation_event_handler) (
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            void *reader, 
            axis2_validation_event_handler_t *veh); 

    struct axis2_xml_schema* (AXIS2_CALL *
    read_a_input_source_and_validation_event_handler)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            void *input_source, 
            axis2_validation_event_handler_t *veh);
            

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_source_and_validation_event_handler) (
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            void *source, 
            axis2_validation_event_handler_t *veh); 

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_document_and_validation_event_handler) (
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            void *document, 
            axis2_validation_event_handler_t *veh);

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_element)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            struct axis2_om_node *node);

    struct axis2_xml_schema* (AXIS2_CALL *
    read_a_document_uri_and_validation_event_handler) (
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            void *document,
            axis2_char_t *uri,
            axis2_validation_event_handler_t *veh); 

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_element_and_uri)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            struct axis2_om_node *node,
            axis2_char_t *uri);

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
            struct axis2_xml_schema_type_receiver *receiver); 
            

    axis2_status_t (AXIS2_CALL *
    resolve_type)(
            axis2_xml_schema_collection_t* collection,
            axis2_env_t **env,
            axis2_qname_t *type_qame, 
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
		
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_READER_AND_VALIDATION_EVENT_HANDLER( \
        collection, env, reader, veh ) \
        ((collection)->ops->read_a_reader_and_validation_event_handler( \
        collection, env, reader, veh ))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_INPUT_SOURCE_AND_VALIDATION_EVENT_HANDLER(\
        collection, env, input_source, veh) \
        ((collection)->ops->read_a_input_source_and_validation_event_handler( \
        collection, env, input_source, veh))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_SOURCE_AND_VALIDATION_EVENT_HANDLER(\
        collection, env, source, veh) \
        ((collection)->ops->read_a_source_and_validation_event_handler(\
        collection, env, source, veh))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_DOCUMENT_AND_VALIDATION_EVENT_HANDLER( \
        collection, env, doc, veh ) \
        ((collection)->ops->read_a_document_and_validation_event_handler( \
        collection, env, doc, veh))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_DOCUMENT_URI_AND_VALIDATION_EVENT_HANDLER( \
        collection , env, document, uri , veh) \
        ((collection)->ops->read_a_document_uri_and_validation_event_handler( \
        collection , env, document, uri , veh))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_DOCUMENT_URI_AND_VALIDATION_EVENT_HANDLER( \
        collection , env, document, uri , veh) \
        ((collection)->ops->read_a_document_uri_and_validation_event_handler( \
        collection , env, document, uri , veh))
        
#define AXIS2_XML_SCHEMA_COLLECTION_READ_A_ELEMENT_AND_URI(collection, env, uri ) \
        ((collection)->ops->read_a_element_and_uri(collection, env, uri ))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_ELEMENTS_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_elements_by_qname(collection, env, qname))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_TYPE_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_type_by_qname(collection, env, qname))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_ELEMENT_BY_QNAME(collection, env, qname) \
        ((collection)->ops->get_element_by_qname(collection, env, qname))
        
#define AXIS2_XML_SCHEMA_COLLECTION_GET_NAMESPACE_FOR_PREFIX(collection, env, prefix) \
        ((collection)->ops->get_namespace_for_prefix(collection, env, prefix))
        
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_COLLECTION_H */
