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
#include <axis2.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_xml_schema_obj.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_collection axis2_xml_schema_collection_t;
typedef struct axis2_xml_schema_collection_ops axis2_xml_schema_collection_ops_t;
struct axis2_xml_schema;
struct axis2_xml_schema_validation_event_handler;
struct axis2_om_element;
struct axis2_om_node;
struct axis2_xml_schema_type_receiver;
struct axis2_xml_schema_type;

/** @defgroup axis2_xml_schema_collection Xml Schema Collection
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_collection_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *collection,
            axis2_env_t **env);

    /**
     * Set the base URI. This is used when schemas need to be
     * loaded from relative locations
     * @param base_uri
     */
    axis2_status_t (AXIS2_CALL *
    set_base_uri) (void *collection,
                                                axis2_env_t **env,
                                                axis2_char_t *base_uri);

    /**
     * This section should comply to the XMLSchema specification
     * @see http://www.w3.org/TR/2004/PER-xmlschema-2-20040318/datatypes.html#built-in-datatypes
     *
     * This needs to be inspected by another pair of eyes
     */
    axis2_status_t (AXIS2_CALL * 
    init) (void *collection,
                                        axis2_env_t **env);


    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_reader_and_validation_event_handler) (
                                void *collection,
                                axis2_env_t **env,
                                void *reader, 
                                struct axis2_xml_schema_validation_event_handler *veh); 

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_input_source_and_validation_event_handler) (
                                void *collection,
                                axis2_env_t **env,
                                void *input_source, 
                                struct axis2_xml_schema_validation_event_handler *veh);

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_source_and_validation_event_handler) (
                                void *collection,
                                axis2_env_t **env,
                                void *source, 
                                struct axis2_xml_schema_validation_event_handler *veh); 

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_document_and_validation_event_handler) (
                                void *collection,
                                axis2_env_t **env,
                                void *source, 
                                struct axis2_xml_schema_validation_event_handler *veh);

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_element) (
                        void *collection,
                        axis2_env_t **env,
                        struct axis2_om_element *element, 
                        struct axis2_om_node *node);

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_document_and_uri_and_validation_event_handler) (
                                void *collection,
                                axis2_env_t **env,
                                void *document,
                                axis2_char_t *uri,
                                struct axis2_xml_schema_validation_event_handler *veh); 

    struct axis2_xml_schema *(AXIS2_CALL *
    read_a_element_and_uri) (
                                void *collection,
                                axis2_env_t **env,
                                struct axis2_om_element *element,
                                struct axis2_om_node *node,
                                axis2_char_t *uri);

    struct axis2_xml_schema_element *(AXIS2_CALL *
    get_element_by_qname) (void *collection,
                                     axis2_env_t **env,
                                     axis2_qname_t *qname);

    struct axis2_xml_schema_type *(AXIS2_CALL *
    get_type_by_qname) (void *collection,
                                 axis2_env_t **env,
                                 axis2_qname_t *schema_type_qname);


    axis2_status_t (AXIS2_CALL *
    add_unresolved_type) (void *collection,
                                axis2_env_t **env,
                                axis2_qname_t *qtype, 
                                struct axis2_xml_schema_type_receiver *receiver); 

    axis2_status_t (AXIS2_CALL *
    resolve_type) (void *collection,
                                axis2_env_t **env,
                                axis2_qname_t *type_qame, 
                                struct axis2_xml_schema_type *type); 

    axis2_char_t *(AXIS2_CALL *
    get_namespace_for_prefix) (void *collection,
                                        axis2_env_t **env,
                                        axis2_char_t *prefix); 

    axis2_status_t (AXIS2_CALL *
    map_namespace) (void *collection,
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

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_collection_resolve_methods(
                        axis2_xml_schema_collection_t *collection,
                        axis2_env_t **env,
                        axis2_xml_schema_collection_t *collection_impl,
                        axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_COLLECTION_FREE(collection, env) \
		(((axis2_xml_schema_collection_t *) collection)->ops->free (collection, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_COLLECTION_H */
