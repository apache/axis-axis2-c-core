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
 
#include <axis2_xml_schema_collection.h>
#include <axis2_xml_schema_element.h>
#include <axis2_xml_schema_type.h>
#include <axis2_xml_schema.h>
#include <axis2_validation_event_handler.h> 
#include <axis2_hash.h>
#include <axis2_xml_schema_type.h>
#include <axis2_xml_schema_type_receiver.h>
#include <axis2_xml_schema_element.h>

typedef struct axis2_xml_schema_collection_impl 
                axis2_xml_schema_collection_impl_t;

/** 
 * @brief Xml Schema Obj Struct Impl
 *	Axis2 Xml Schema Obj  
 */ 
struct axis2_xml_schema_collection_impl
{
    axis2_xml_schema_collection_t collection;
    /**
     */
    axis2_hash_t *namespaces;
    /**
     * base URI is used as the base for loading the
     * imports
     */
    axis2_char_t *base_uri;
    /**
     * In-scope namespaces for XML processing
     */
    axis2_hash_t *in_scope_namespaces;
    
    axis2_xml_schema_t *xml_schema;
    
    axis2_hash_t *unresolved_types;
};

#define AXIS2_INTF_TO_IMPL(collection) \
        ((axis2_xml_schema_collection_impl_t *) collection)


/************* function prototypes *******************************************/


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_free(axis2_xml_schema_collection_t* collection,
                                 axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_set_base_uri
                  (axis2_xml_schema_collection_t* collection,
                   axis2_env_t **env,
                   axis2_char_t *base_uri);
                   
axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_init(axis2_xml_schema_collection_t* collection,
                                 axis2_env_t **env);


struct axis2_xml_schema* AXIS2_CALL
axis2_xml_schema_collection_read_a_reader_and_validation_event_handler(
                            axis2_xml_schema_collection_t* collection,
                            axis2_env_t **env,
                            void *reader, 
                            axis2_validation_event_handler_t *veh); 
                            
struct axis2_xml_schema* AXIS2_CALL
axis2_xml_schema_collection_read_a_input_source_and_validation_event_handler(
                                axis2_xml_schema_collection_t* collection,
                                axis2_env_t **env,
                                void *input_source, 
                                axis2_validation_event_handler_t *veh);

struct axis2_xml_schema *   AXIS2_CALL
axis2_xml_schema_collection_read_a_source_and_validation_event_handler(
                                axis2_xml_schema_collection_t* collection,
                                axis2_env_t **env,
                                void *source, 
                                axis2_validation_event_handler_t *veh); 

struct axis2_xml_schema * AXIS2_CALL
axis2_xml_schema_collection_read_a_document_and_validation_event_handler (
                                axis2_xml_schema_collection_t* collection,
                                axis2_env_t **env,
                                void *document, 
                                axis2_validation_event_handler_t *veh);

struct axis2_xml_schema * AXIS2_CALL
axis2_xml_schema_collection_read_a_element
                    (axis2_xml_schema_collection_t* collection,
                     axis2_env_t **env,
                     struct axis2_om_node *node);

struct axis2_xml_schema* AXIS2_CALL
axis2_xml_schema_collection_read_a_document_uri_and_validation_event_handler
                         (axis2_xml_schema_collection_t* collection,
                        axis2_env_t **env,
                        void *document,
                        axis2_char_t *uri,
                        axis2_validation_event_handler_t *veh); 

struct axis2_xml_schema * AXIS2_CALL
axis2_xml_schema_collection_read_a_element_and_uri
                           (axis2_xml_schema_collection_t* collection,
                             axis2_env_t **env,
                             struct axis2_om_node *node,
                             axis2_char_t *uri);

struct axis2_xml_schema_element * AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname
                       (axis2_xml_schema_collection_t* collection,
                        axis2_env_t **env,
                        axis2_qname_t *qname);

struct axis2_xml_schema_type * AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname
                   (axis2_xml_schema_collection_t* collection,
                    axis2_env_t **env,
                    axis2_qname_t *schema_type_qname);


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type
                       (axis2_xml_schema_collection_t* collection,
                        axis2_env_t **env,
                        axis2_qname_t *qtype, 
                        struct axis2_xml_schema_type_receiver *receiver); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_resolve_type
               (axis2_xml_schema_collection_t* collection,
                axis2_env_t **env,
                axis2_qname_t *type_qame, 
                struct axis2_xml_schema_type *type); 

axis2_char_t * AXIS2_CALL
axis2_xml_schema_collection_get_namespace_for_prefix
                (axis2_xml_schema_collection_t *collection,
                 axis2_env_t **env,
                 axis2_char_t *prefix); 
                     

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace
                (axis2_xml_schema_collection_t* collection,
                 axis2_env_t **env,
                 axis2_char_t *prefix, 
                 axis2_char_t *namespc_uri);
                    
                    

AXIS2_DECLARE(axis2_xml_schema_collection_t *)
axis2_xml_schema_collection_create(axis2_env_t **env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    
    collection_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_collection_impl_t));
    if(!collection_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    collection_impl->base_uri  = NULL;
    collection_impl->collection.ops = NULL;
    collection_impl->in_scope_namespaces = NULL;
    collection_impl->namespaces = NULL;
    collection_impl->unresolved_types = NULL;
    collection_impl->xml_schema = NULL;
    
    collection_impl->collection.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_collection_ops_t)); 
    
    if(!collection_impl->collection.ops)
    {
        axis2_xml_schema_collection_free(&(collection_impl->collection), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_xml_schema_collection_init(&(collection_impl->collection), env);

    collection_impl->collection.ops->free = 
            axis2_xml_schema_collection_free;
    collection_impl->collection.ops->set_base_uri = 
            axis2_xml_schema_collection_set_base_uri;
    collection_impl->collection.ops->init = 
            axis2_xml_schema_collection_init;
            
    collection_impl->collection.ops->read_a_reader_and_validation_event_handler = 
            axis2_xml_schema_collection_read_a_reader_and_validation_event_handler;
            
    collection_impl->collection.ops->read_a_input_source_and_validation_event_handler = 
            axis2_xml_schema_collection_read_a_input_source_and_validation_event_handler;
            
    collection_impl->collection.ops->read_a_source_and_validation_event_handler = 
            axis2_xml_schema_collection_read_a_source_and_validation_event_handler;
            
    collection_impl->collection.ops->read_a_document_and_validation_event_handler = 
            axis2_xml_schema_collection_read_a_document_and_validation_event_handler;
            
    collection_impl->collection.ops->read_a_element = 
            axis2_xml_schema_collection_read_a_element;
            
    collection_impl->collection.ops->read_a_document_uri_and_validation_event_handler= 
        axis2_xml_schema_collection_read_a_document_uri_and_validation_event_handler;
    collection_impl->collection.ops->read_a_element_and_uri = 
            axis2_xml_schema_collection_read_a_element_and_uri;
    collection_impl->collection.ops->get_element_by_qname = 
            axis2_xml_schema_collection_get_element_by_qname;
    collection_impl->collection.ops->get_type_by_qname = 
            axis2_xml_schema_collection_get_type_by_qname;
    collection_impl->collection.ops->add_unresolved_type = 
            axis2_xml_schema_collection_add_unresolved_type;
    collection_impl->collection.ops->resolve_type = 
            axis2_xml_schema_collection_resolve_type;
    collection_impl->collection.ops->get_namespace_for_prefix = 
            axis2_xml_schema_collection_get_namespace_for_prefix;
    collection_impl->collection.ops->map_namespace = 
            axis2_xml_schema_collection_map_namespace;

    return &(collection_impl->collection);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_free(axis2_xml_schema_collection_t *collection,
                                 axis2_env_t **env)
{
    if(NULL != collection->ops)
    {
        AXIS2_FREE((*env)->allocator, collection->ops);
        collection->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(collection));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_set_base_uri(axis2_xml_schema_collection_t*collection,
                                            axis2_env_t **env,
                                            axis2_char_t *base_uri)
{
    return AXIS2_SUCCESS;
}

/**
 * This section should comply to the XMLSchema specification
 * @see http://www.w3.org/TR/2004/PER-xmlschema-2-20040318/datatypes.html#built-in-datatypes
 *
 * This needs to be inspected by another pair of eyes
 */
axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_init(axis2_xml_schema_collection_t   *collection,
                                    axis2_env_t **env) 
{
    /*
    Primitive types

    3.2.1 string
    3.2.2 boolean
    3.2.3 decimal
    3.2.4 float
    3.2.5 double
    3.2.6 duration
    3.2.7 dateTime
    3.2.8 time
    3.2.9 date
    3.2.10 gYearMonth
    3.2.11 gYear
    3.2.12 gMonthDay
    3.2.13 gDay
    3.2.14 gMonth
    3.2.15 hexBinary
    3.2.16 base64Binary
    3.2.17 anyURI
    3.2.18 QName
    3.2.19 NOTATION
    */


    /*
     3.3.1 normalizedString
    3.3.2 token
    3.3.3 language
    3.3.4 NMTOKEN
    3.3.5 NMTOKENS
    3.3.6 Name
    3.3.7 NCName
    3.3.8 ID
    3.3.9 IDREF
    3.3.10 IDREFS
    3.3.11 ENTITY
    3.3.12 ENTITIES
    3.3.13 integer
    3.3.14 nonPositiveInteger
    3.3.15 negativeInteger
    3.3.16 long
    3.3.17 int
    3.3.18 short
    3.3.19 byte
    3.3.20 nonNegativeInteger
    3.3.21 unsignedLong
    3.3.22 unsignedInt
    3.3.23 unsignedShort
    3.3.24 unsignedByte
    3.3.25 positiveInteger
    */

     /*derived types from decimal */

    /* derived types from string */
    return AXIS2_SUCCESS;
}

static axis2_status_t add_simple_type(axis2_xml_schema_collection_t  *collection,
                                        axis2_env_t **env,
                                        axis2_xml_schema_t *schema,
                                        axis2_char_t *type_name)
{
    return AXIS2_SUCCESS;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_reader_and_validation_event_handler(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            void *reader, 
                                            axis2_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_input_source_and_validation_event_handler(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            void *input_source, 
                                            axis2_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_source_and_validation_event_handler(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            void  *source, 
                                            axis2_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_document_and_validation_event_handler(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            void *source, 
                                            axis2_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_element(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            axis2_om_node_t *node)
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_document_uri_and_validation_event_handler(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            void *document,
                                            axis2_char_t *uri,
                                            axis2_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_element_and_uri(
                                            axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            axis2_om_node_t *node,
                                            axis2_char_t *uri) 
{
    return NULL;
}

axis2_xml_schema_element_t *AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(axis2_xml_schema_collection_t  *collection,
                                                 axis2_env_t **env,
                                                 axis2_qname_t *qname) 
{
    return NULL;
}

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(axis2_xml_schema_collection_t  *collection,
                                             axis2_env_t **env,
                                             axis2_qname_t *schema_type_qname) 
{
    return NULL;
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(axis2_xml_schema_collection_t  *collection,
                                                axis2_env_t **env,
                                                axis2_qname_t *qtype, 
                                                axis2_xml_schema_type_receiver_t *receiver) 
{
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_resolve_type(axis2_xml_schema_collection_t  *collection,
                                         axis2_env_t **env,
                                         axis2_qname_t *type_qame, 
                                          struct axis2_xml_schema_type *type) 
{
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_collection_get_namespace_for_prefix(axis2_xml_schema_collection_t  *collection,
                                                    axis2_env_t **env,
                                                    axis2_char_t *prefix) 
{
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(axis2_xml_schema_collection_t  *collection,
                                            axis2_env_t **env,
                                            axis2_char_t *prefix, 
                                            axis2_char_t *namespc_uri)
{
    return AXIS2_SUCCESS;
}


