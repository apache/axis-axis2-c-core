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
#include <xml_schema/axis2_xml_schema_simple_type.h>

typedef struct axis2_xml_schema_collection_impl 
                axis2_xml_schema_collection_impl_t;

/** 
 * @brief Xml Schema Obj Struct Impl
 *	Axis2 Xml Schema Obj  
 */ 
struct axis2_xml_schema_collection_impl
{
    axis2_xml_schema_collection_t collection;

    axis2_hash_t *namespaces;

    axis2_char_t *base_uri;

    axis2_hash_t *in_scope_namespaces;
    
    axis2_hash_t *schema_id2_schemas;
    
    axis2_xml_schema_t *xml_schema;
    
    axis2_hash_t *schemas;
    
    axis2_hash_t *unresolved_types;
};

#define AXIS2_INTF_TO_IMPL(collection) \
        ((axis2_xml_schema_collection_impl_t *) collection)


/************* function prototypes *******************************************/


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_free(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_set_base_uri(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_char_t *base_uri);
                   
axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_init(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env);


axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_with_reader(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_xml_reader_t *reader);
                            
axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_document(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_document_t* document);

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_node_t *node);

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_document_with_uri(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_document_t *document,
        axis2_char_t *uri);

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element_with_uri(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_node_t *node,
        axis2_char_t *uri);

axis2_xml_schema_element_t * AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_xml_schema_type_t * AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *schema_type_qname);


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *qtype, 
        void *receiver); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_resolve_type(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *type_qame, 
        axis2_xml_schema_type_t *type); 

axis2_char_t * AXIS2_CALL
axis2_xml_schema_collection_get_namespace_for_prefix(
        axis2_xml_schema_collection_t *collection,
        axis2_env_t **env,
        axis2_char_t *prefix); 
                     

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_char_t *prefix, 
        axis2_char_t *namespc_uri);
        
static axis2_status_t 
add_simple_type(
        axis2_env_t **env,
        axis2_xml_schema_t* schema,
        axis2_xml_schema_simple_type_t *simple_type,
        axis2_char_t *type_name);

/*************** end function prototypes ****************************/                    

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
    collection_impl->schema_id2_schemas = NULL;
    collection_impl->schemas = NULL;
    
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
            
    collection_impl->collection.ops->read_with_reader = 
            axis2_xml_schema_collection_read_with_reader;
            
    collection_impl->collection.ops->read_document = 
            axis2_xml_schema_collection_read_document;
            
    collection_impl->collection.ops->read_element = 
            axis2_xml_schema_collection_read_element;
            
    collection_impl->collection.ops->read_document_with_uri = 
        axis2_xml_schema_collection_read_document_with_uri;
        
    collection_impl->collection.ops->read_element_with_uri = 
            axis2_xml_schema_collection_read_element_with_uri;
            
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
axis2_xml_schema_collection_init(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env)
{

    return AXIS2_SUCCESS;
}


axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_with_reader(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_xml_reader_t *reader)
{

    return NULL;
}
                            
axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_document(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_document_t* document)
{

    return NULL;
}

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_node_t *node)
{
    return NULL;
}

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_document_with_uri(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_document_t *document,
        axis2_char_t *uri)
{
    return NULL;
}

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element_with_uri(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_om_node_t *node,
        axis2_char_t *uri)
{
    return NULL;
}

axis2_xml_schema_element_t * AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    return NULL;
}

axis2_xml_schema_type_t * AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *schema_type_qname)
{
    return NULL;
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *qtype, 
        void *receiver)
{
    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_resolve_type(
        axis2_xml_schema_collection_t* collection,
        axis2_env_t **env,
        axis2_qname_t *type_qame, 
        axis2_xml_schema_type_t *type)
{
    return AXIS2_SUCCESS;
} 

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_collection_get_namespace_for_prefix(
        axis2_xml_schema_collection_t  *collection,
        axis2_env_t **env,
        axis2_char_t *prefix) 
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, prefix, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if(NULL != collection_impl->in_scope_namespaces)
    {
        axis2_char_t *ns = NULL;
        ns = (axis2_char_t *)axis2_hash_get(
                collection_impl->in_scope_namespaces, prefix, AXIS2_HASH_KEY_STRING);
        return ns;                
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(
        axis2_xml_schema_collection_t  *collection,
        axis2_env_t **env,
        axis2_char_t *prefix, 
        axis2_char_t *namespc_uri)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if(NULL != collection_impl->in_scope_namespaces)
    {
        axis2_hash_set(collection_impl->in_scope_namespaces, prefix,
            AXIS2_HASH_KEY_STRING, namespc_uri);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

static axis2_status_t 
add_simple_type(
        axis2_env_t **env,
        axis2_xml_schema_t* schema,
        axis2_xml_schema_simple_type_t *simple_type,
        axis2_char_t *type_name)
{
    axis2_xml_schema_simple_type_t* type = NULL;
    type = axis2_xml_schema_simple_type_create(env, schema);
    AXIS2_XML_SCHEMA_TYPE_SET_NAME(type, env, type_name);
    AXIS2_XML_SCHEMA_ADD_TYPE(schema, env, type);
    return AXIS2_SUCCESS;
}        

