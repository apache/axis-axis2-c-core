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
 
#include <xml_schema/axis2_xml_schema_collection.h>
#include <xml_schema/axis2_xml_schema_element.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <xml_schema/axis2_xml_schema.h>
#include <xml_schema/axis2_validation_event_handler.h> 
#include <axis2_hash.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <xml_schema/axis2_xml_schema_type_receiver.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_xml_schema_collection_impl axis2_xml_schema_collection_impl_t;

/** 
 * @brief Xml Schema Obj Struct Impl
 *	Axis2 Xml Schema Obj  
 */ 
struct axis2_xml_schema_collection_impl
{
    axis2_xml_schema_collection_t collection;
    /**
     * Namespaces we know about.  Each one has an equivalent XmlSchema.
     */
    axis2_hash_t *namespcs;
    /**
     * base URI is used as the base for loading the
     * imports
     */
    axis2_char_t *base_uri;
    /**
     * In-scope namespaces for XML processing
     */
    axis2_hash_t *in_scope_namespcs;
    axis2_xml_schema_t *xml_schema;
    axis2_hash_t *unresolved_types;
};

#define INTF_TO_IMPL(collection) ((axis2_xml_schema_collection_impl_t *) collection)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_free(void *collection,
                axis2_env_t **env);

/**
 * Set the base URI. This is used when schemas need to be
 * loaded from relative locations
 * @param base_uri
 */
axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_set_base_uri(void *collection,
                                            axis2_env_t **env,
                                            axis2_char_t *base_uri);

/**
 * This section should comply to the XMLSchema specification
 * @see http://www.w3.org/TR/2004/PER-xmlschema-2-20040318/datatypes.html#built-in-datatypes
 *
 * This needs to be inspected by another pair of eyes
 */
axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_init(void *collection,
                                    axis2_env_t **env);

static axis2_status_t add_simple_type(void *collection,
                                        axis2_env_t **env,
                                        axis2_xml_schema_t *schema,
                                        axis2_char_t *type_name);

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_reader_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *reader, 
                                            axis2_xml_schema_validation_event_handler_t *veh); 

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_input_source_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *input_source, 
                                            axis2_xml_schema_validation_event_handler_t *veh);

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_source_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *source, 
                                            axis2_xml_schema_validation_event_handler_t *veh); 

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_document_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *source, 
                                            axis2_xml_schema_validation_event_handler_t *veh);

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_element(
                                            void *collection,
                                            axis2_env_t **env,
                                            axis2_om_element_t *element, 
                                            axis2_om_node_t *node);

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_document_and_uri_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *document,
                                            axis2_char_t *uri,
                                            axis2_xml_schema_validation_event_handler_t *veh); 

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_element_and_uri(
                                            void *collection,
                                            axis2_env_t **env,
                                            axis2_om_element_t *element,
                                            axis2_om_node_t *node,
                                            axis2_char_t *uri);

axis2_xml_schema_element_t *AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(void *collection,
                                                 axis2_env_t **env,
                                                 axis2_qname_t *qname);

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(void *collection,
                                             axis2_env_t **env,
                                             axis2_qname_t *schema_type_qname);


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(void *collection,
                                                axis2_env_t **env,
                                                axis2_qname_t *qtype, 
                                                axis2_xml_schema_type_receiver_t *receiver); 

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_resolve_type(void *collection,
                                            axis2_env_t **env,
                                            axis2_qname_t *type_qame, 
                                         struct axis2_xml_schema_type *type); 

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_collection_get_namespace_for_prefix(void *collection,
                                                    axis2_env_t **env,
                                                    axis2_char_t *prefix); 

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(void *collection,
                                            axis2_env_t **env,
                                            axis2_char_t *prefix, 
                                            axis2_char_t *namespc_uri);


AXIS2_DECLARE(axis2_xml_schema_collection_t *)
axis2_xml_schema_collection_create(axis2_env_t **env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    
    collection_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_collection_impl_t));

    collection_impl->collection.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_collection_ops_t)); 

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
    collection_impl->collection.ops->read_a_document_and_uri_and_validation_event_handler= 
            axis2_xml_schema_collection_read_a_document_and_uri_and_validation_event_handler;
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
axis2_xml_schema_collection_free(void *collection,
                                        axis2_env_t **env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;

    collection_impl = INTF_TO_IMPL(collection);

    if((&(collection_impl->collection))->ops)
    {
        free((&(collection_impl->collection))->ops);
        (&(collection_impl->collection))->ops = NULL;
    }

    if(collection_impl)
    {
        free(collection_impl);
        collection_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_collection_resolve_methods(
                        axis2_xml_schema_collection_t *collection,
                        axis2_env_t **env,
                        axis2_xml_schema_collection_t *collection_impl,
                        axis2_hash_t *methods)
{    
    axis2_xml_schema_collection_impl_t *collection_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, collection_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    collection_impl_l = (axis2_xml_schema_collection_impl_t *) collection_impl;
    
    collection->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_collection_ops_t));
    collection->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    collection->ops->set_base_uri = 
            collection_impl_l->collection.ops->set_base_uri;
    collection->ops->init = 
            collection_impl_l->collection.ops->init; 
    collection->ops->read_a_reader_and_validation_event_handler = 
            collection_impl_l->collection.ops->read_a_reader_and_validation_event_handler; 
    collection->ops->read_a_input_source_and_validation_event_handler = 
            collection_impl_l->collection.ops->read_a_input_source_and_validation_event_handler; 
    collection->ops->read_a_source_and_validation_event_handler = 
            collection_impl_l->collection.ops->read_a_source_and_validation_event_handler; 
    collection->ops->read_a_document_and_validation_event_handler = 
            collection_impl_l->collection.ops->read_a_document_and_validation_event_handler; 
    collection->ops->read_a_element = 
            collection_impl_l->collection.ops->read_a_element; 
    collection->ops->read_a_document_and_uri_and_validation_event_handler = 
            collection_impl_l->collection.ops->read_a_document_and_uri_and_validation_event_handler; 
    collection->ops->read_a_element_and_uri = 
            collection_impl_l->collection.ops->read_a_element_and_uri; 
    collection->ops->get_element_by_qname = 
            collection_impl_l->collection.ops->get_element_by_qname; 
    collection->ops->get_type_by_qname = 
            collection_impl_l->collection.ops->get_type_by_qname; 
    collection->ops->add_unresolved_type = 
            collection_impl_l->collection.ops->add_unresolved_type; 
    collection->ops->resolve_type = 
            collection_impl_l->collection.ops->resolve_type; 
    collection->ops->get_namespace_for_prefix = 
            collection_impl_l->collection.ops->get_namespace_for_prefix; 
    collection->ops->map_namespace = 
            collection_impl_l->collection.ops->map_namespace; 
    return AXIS2_SUCCESS;    

}

/**
 * Set the base URI. This is used when schemas need to be
 * loaded from relative locations
 * @param base_uri
 */
axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_set_base_uri(void *collection,
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
axis2_xml_schema_collection_init(void *collection,
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

static axis2_status_t add_simple_type(void *collection,
                                        axis2_env_t **env,
                                        axis2_xml_schema_t *schema,
                                        axis2_char_t *type_name)
{
    return AXIS2_SUCCESS;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_reader_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *reader, 
                                            axis2_xml_schema_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_input_source_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *input_source, 
                                            axis2_xml_schema_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_source_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *source, 
                                            axis2_xml_schema_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_document_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *source, 
                                            axis2_xml_schema_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_element(
                                            void *collection,
                                            axis2_env_t **env,
                                            axis2_om_element_t *element, 
                                            axis2_om_node_t *node)
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_document_and_uri_and_validation_event_handler(
                                            void *collection,
                                            axis2_env_t **env,
                                            void *document,
                                            axis2_char_t *uri,
                                            axis2_xml_schema_validation_event_handler_t *veh) 
{
    return NULL;
}

axis2_xml_schema_t *AXIS2_CALL
axis2_xml_schema_collection_read_a_element_and_uri(
                                            void *collection,
                                            axis2_env_t **env,
                                            axis2_om_element_t *element,
                                            axis2_om_node_t *node,
                                            axis2_char_t *uri) 
{
    return NULL;
}

axis2_xml_schema_element_t *AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(void *collection,
                                                 axis2_env_t **env,
                                                 axis2_qname_t *qname) 
{
    return NULL;
}

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(void *collection,
                                             axis2_env_t **env,
                                             axis2_qname_t *schema_type_qname) 
{
    return NULL;
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(void *collection,
                                                axis2_env_t **env,
                                                axis2_qname_t *qtype, 
                                                axis2_xml_schema_type_receiver_t *receiver) 
{
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_resolve_type(void *collection,
                                         axis2_env_t **env,
                                         axis2_qname_t *type_qame, 
                                          struct axis2_xml_schema_type *type) 
{
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_collection_get_namespace_for_prefix(void *collection,
                                                    axis2_env_t **env,
                                                    axis2_char_t *prefix) 
{
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(void *collection,
                                            axis2_env_t **env,
                                            axis2_char_t *prefix, 
                                            axis2_char_t *namespc_uri)
{
    return AXIS2_SUCCESS;
}


