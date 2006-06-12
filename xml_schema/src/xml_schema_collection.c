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
 
#include <axis2_xml_schema_includes.h>
#include <axis2_hash.h>

typedef struct axis2_xml_schema_collection_impl 
                axis2_xml_schema_collection_impl_t;

struct axis2_xml_schema_collection_impl
{
    axis2_xml_schema_collection_t collection;

    axis2_hash_t *namespaces;

    axis2_char_t *base_uri;

    axis2_hash_t *in_scope_namespaces;
    
    axis2_hash_t *systemid2_schemas;
    
    axis2_xml_schema_t *xsd;
    
    axis2_array_list_t *schemas;
    
    axis2_hash_t *unresolved_types;
    
    axis2_array_list_t *builder_list;
    
};

#define AXIS2_INTF_TO_IMPL(collection) \
        ((axis2_xml_schema_collection_impl_t *) collection)


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_free(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_set_base_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_char_t *base_uri);
        
axis2_char_t* AXIS2_CALL
axis2_xml_schema_collection_get_base_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env);        
                   
axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_init(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env);


axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_with_reader(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_xml_reader_t *reader);
                            
axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_document(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_document_t* document);

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_node_t *node);

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_document_with_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_document_t *document,
        axis2_char_t *uri);

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element_with_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_char_t *uri);

axis2_xml_schema_element_t * AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *qname);

axis2_xml_schema_type_t * AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *schema_type_qname);


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *qtype, 
        void *receiver); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_resolve_type(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *type_qname, 
        axis2_xml_schema_type_t *type); 

axis2_char_t * AXIS2_CALL
axis2_xml_schema_collection_get_namespace_for_prefix(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env,
        axis2_char_t *prefix); 
                     

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_char_t *prefix, 
        axis2_char_t *namespc_uri);
        
static axis2_status_t 
add_simple_type(
        const axis2_env_t *env,
        axis2_xml_schema_t* schema,
        axis2_char_t *type_name);
        
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_collection_get_systemid2_schemas(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env);
        
axis2_array_list_t* AXIS2_CALL 
axis2_xml_schema_collection_get_schemas(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_set_schemas(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env,
        axis2_array_list_t *schemas);
        
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_collection_get_namespaces(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env);  
        
axis2_xml_schema_t* AXIS2_CALL 
axis2_xml_schema_collection_get_schema(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env,
        axis2_char_t *system_id);                                  
        

AXIS2_EXTERN axis2_xml_schema_collection_t * AXIS2_CALL
axis2_xml_schema_collection_create(const axis2_env_t *env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    
    collection_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_collection_impl_t));
    if(!collection_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    collection_impl->base_uri  = NULL;
    collection_impl->collection.ops = NULL;
    collection_impl->in_scope_namespaces = NULL;
    collection_impl->namespaces = NULL;
    collection_impl->unresolved_types = NULL;
    collection_impl->xsd = NULL;
    collection_impl->systemid2_schemas = NULL;
    collection_impl->schemas = NULL;
    collection_impl->builder_list = NULL;
    
    collection_impl->collection.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_collection_ops_t)); 
    
    if(!collection_impl->collection.ops)
    {
        axis2_xml_schema_collection_free(&(collection_impl->collection), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    collection_impl->collection.ops->free = 
            axis2_xml_schema_collection_free;
    
    collection_impl->collection.ops->set_base_uri = 
            axis2_xml_schema_collection_set_base_uri;
            
    collection_impl->collection.ops->get_base_uri =
            axis2_xml_schema_collection_get_base_uri;
                        
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
            
    collection_impl->collection.ops->get_schemas =
            axis2_xml_schema_collection_get_schemas;

    collection_impl->collection.ops->get_systemid2_schemas =
            axis2_xml_schema_collection_get_systemid2_schemas; 
            
    collection_impl->collection.ops->get_namespaces = 
            axis2_xml_schema_collection_get_namespaces;
            
    collection_impl->collection.ops->get_schema =
            axis2_xml_schema_collection_get_schema;            
            

    collection_impl->xsd = axis2_xml_schema_create(
        env, AXIS2_XML_SCHEMA_NS ,&(collection_impl->collection));
    
    if(!collection_impl->xsd)
    {
        axis2_xml_schema_collection_free(&(collection_impl->collection), env);
        return NULL;
    }        
    
    collection_impl->namespaces          = axis2_hash_make(env);
    collection_impl->in_scope_namespaces = axis2_hash_make(env);
    collection_impl->systemid2_schemas   = axis2_hash_make(env);
    collection_impl->schemas             = axis2_array_list_create(env, 10);
    collection_impl->unresolved_types    = axis2_hash_make(env);
    collection_impl->builder_list        = axis2_array_list_create(env, 2);
    
    if(!collection_impl->namespaces || !collection_impl->in_scope_namespaces ||
            !collection_impl->systemid2_schemas || !collection_impl->schemas ||
            !collection_impl->unresolved_types )
    {
        axis2_xml_schema_collection_free(&(collection_impl->collection), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }            
    
    axis2_xml_schema_collection_init(&(collection_impl->collection), env);
    return &(collection_impl->collection);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_free(
    axis2_xml_schema_collection_t *collection,
    const axis2_env_t *env)
{
    if(NULL != collection->ops)
    {
        AXIS2_FREE(env->allocator, collection->ops);
        collection->ops = NULL;
    }
    /** TODO */
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(collection));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_set_base_uri(
        axis2_xml_schema_collection_t*collection,
        const axis2_env_t *env,
        axis2_char_t *base_uri)
{
    axis2_xml_schema_collection_impl_t *collec_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, base_uri, AXIS2_FAILURE);
    
    collec_impl = AXIS2_INTF_TO_IMPL(collection);
    
    if(NULL != collec_impl->base_uri)
    {
        AXIS2_FREE(env->allocator, collec_impl->base_uri);
        collec_impl->base_uri = NULL;
    }
    collec_impl->base_uri = AXIS2_STRDUP(base_uri, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_init(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
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
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_STRING);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_BOOLEAN);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_FLOAT);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_DOUBLE);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_QNAME);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_DECIMAL);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_DURATION);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_DATE);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_TIME);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_DATETIME);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_DAY);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_MONTH);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_MONTHDAY);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_YEAR);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_YEARMONTH);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NOTATION);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_HEXBIN);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_BASE64);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_ANYURI);

    /** derived types from decimal */

    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_LONG);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_SHORT);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_BYTE);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_INTEGER);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_INT);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_POSITIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NEGATIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NONPOSITIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NONNEGATIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_UNSIGNEDBYTE);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_UNSIGNEDINT);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_UNSIGNEDLONG);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_UNSIGNEDSHORT);
    
    /** derived types from string */
    
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NAME);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NORMALIZEDSTRING);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NCNAME);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NMTOKEN);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_NMTOKENS);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_ENTITY);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_ENTITIES);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_ID);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_IDREF);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_IDREFS);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_LANGUAGE);
    add_simple_type(env, collection_impl->xsd, AXIS2_XML_SCHEMA_XSD_TOKEN);
    
    axis2_hash_set(collection_impl->namespaces, AXIS2_XML_SCHEMA_NS,
            AXIS2_HASH_KEY_STRING,  collection_impl->xsd);
        
    return AXIS2_SUCCESS;
}


axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_with_reader(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_xml_reader_t *reader)
{
    axis2_xml_schema_builder_t *sch_builder = NULL;
    axiom_document_t *om_doc = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    
    AXIS2_PARAM_CHECK(env->error, reader, NULL);
    
    om_builder = axiom_stax_builder_create(env, reader);
    if(!om_builder)
        return NULL;
        
    om_doc = axiom_document_create(env, NULL, om_builder);
    if(!om_doc)
    {
        AXIOM_STAX_BUILDER_FREE(om_builder, env);
        return NULL;
    }    
    AXIOM_DOCUMENT_BUILD_ALL(om_doc, env);
    
    sch_builder = axis2_xml_schema_builder_create(env, collection);
    if(!sch_builder)
    {
        AXIOM_STAX_BUILDER_FREE(om_builder, env);
        return NULL;
    }
    
    return AXIS2_XML_SCHEMA_BUILDER_BUILD(sch_builder, env, om_doc, NULL);
}
                            
axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_document(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_document_t* document)
{
    axis2_xml_schema_builder_t *sch_builder = NULL;
    AXIS2_PARAM_CHECK(env->error , document, NULL);
    sch_builder = axis2_xml_schema_builder_create(env, collection);
    return AXIS2_XML_SCHEMA_BUILDER_BUILD(sch_builder, env, document, NULL);    
}

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    axis2_xml_schema_builder_t *sch_builder = NULL;
    
    AXIS2_PARAM_CHECK(env->error, node, NULL);
    
    sch_builder = axis2_xml_schema_builder_create(env, collection);
    
    if(!sch_builder)
        return NULL;
    return AXIS2_XML_SCHEMA_BUILDER_BUILD_WITH_ROOT_NODE(sch_builder, env, node, NULL);
}

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_collection_read_document_with_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_document_t *document,
        axis2_char_t *uri)
{
    axis2_xml_schema_builder_t *sch_builder = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error , document, NULL);
    sch_builder = axis2_xml_schema_builder_create(env, collection);
    return AXIS2_XML_SCHEMA_BUILDER_BUILD(sch_builder, env, document, uri); 
}

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_collection_read_element_with_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_char_t *uri)
{
    axis2_xml_schema_builder_t *sch_builder = NULL;
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    
    AXIS2_PARAM_CHECK(env->error, node, NULL);
    sch_builder = axis2_xml_schema_builder_create(env, collection);
    if(!sch_builder)
        return NULL;

    AXIS2_ARRAY_LIST_ADD(collection_impl->builder_list, env, sch_builder);

    return AXIS2_XML_SCHEMA_BUILDER_BUILD_WITH_ROOT_NODE(sch_builder, env, node, uri);
}

axis2_xml_schema_element_t * AXIS2_CALL
axis2_xml_schema_collection_get_element_by_qname(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_char_t *uri = NULL;
    axis2_xml_schema_t *schema = NULL;
    axis2_xml_schema_collection_impl_t *collec_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    
    collec_impl = AXIS2_INTF_TO_IMPL(collection);
    
    uri = AXIS2_QNAME_GET_URI(qname, env);

    if(NULL != collec_impl->namespaces)
        schema = axis2_hash_get(collec_impl->namespaces, uri, AXIS2_HASH_KEY_STRING);   

    if(!schema)
        return NULL;
        
    return AXIS2_XML_SCHEMA_GET_ELEMENT_BY_QNAME(schema, env, qname);
}

axis2_xml_schema_type_t * AXIS2_CALL
axis2_xml_schema_collection_get_type_by_qname(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *schema_type_qname)
{
    
    axis2_xml_schema_collection_impl_t *collecion_impl = NULL;
    
    axis2_char_t *uri = NULL;
    
    axis2_xml_schema_t *schema = NULL;
    
    AXIS2_PARAM_CHECK(env->error, schema_type_qname, NULL);
    collecion_impl = AXIS2_INTF_TO_IMPL(collection);

    uri = AXIS2_QNAME_GET_URI(schema_type_qname, env);

    if(NULL != collecion_impl->namespaces && NULL != uri)
        schema = axis2_hash_get(collecion_impl->namespaces, uri, AXIS2_HASH_KEY_STRING);
    
    if(!schema)
        return NULL;         
    return AXIS2_XML_SCHEMA_GET_TYPE_BY_QNAME(schema, env, schema_type_qname);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_add_unresolved_type(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *qtype, 
        void *obj)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    axis2_char_t *qn_string = NULL;
    axis2_array_list_t *receivers = NULL;
    
        
    AXIS2_PARAM_CHECK(env->error, qtype, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, obj, AXIS2_FAILURE);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
        
    qn_string = AXIS2_QNAME_TO_STRING(qtype, env);
    if(NULL != collection_impl->unresolved_types && 
        NULL != qn_string)
    {
        receivers = axis2_hash_get(collection_impl->unresolved_types, 
            qn_string, AXIS2_HASH_KEY_STRING);
            
        if(!receivers)
        {
            receivers = axis2_array_list_create(env, 10);            
            axis2_hash_set(collection_impl->unresolved_types, qn_string,
                AXIS2_HASH_KEY_STRING, receivers);
        }    
    }
        
    AXIS2_ARRAY_LIST_ADD(receivers, env, obj);

    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_resolve_type(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env,
        axis2_qname_t *type_qname, 
        axis2_xml_schema_type_t *type)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    axis2_char_t *qn_string = NULL;
    axis2_array_list_t *receivers = NULL;
    AXIS2_PARAM_CHECK(env->error, type_qname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type, AXIS2_FAILURE);
    
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    qn_string = AXIS2_QNAME_TO_STRING(type_qname, env);
    if(NULL != qn_string && NULL != collection_impl->unresolved_types)
    {
        receivers = axis2_hash_get(collection_impl->unresolved_types, 
            qn_string, AXIS2_HASH_KEY_STRING);
            
        if(NULL != receivers)
        {
            int i = 0;
            
            for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(receivers, env); i++)
            {
                void *obj = NULL;
                obj = AXIS2_ARRAY_LIST_GET(receivers, env, i);
                if(NULL != obj)
                {
                    AXIS2_XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(obj, env, type);
                }
            }
        }
        else
        {
            return AXIS2_FAILURE;
        }            
        if(NULL != collection_impl->unresolved_types)
            axis2_hash_set(collection_impl->unresolved_types, qn_string, 
                AXIS2_HASH_KEY_STRING, NULL);
    }
    return AXIS2_SUCCESS;
} 

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_collection_get_namespace_for_prefix(
        axis2_xml_schema_collection_t  *collection,
        const axis2_env_t *env,
        axis2_char_t *prefix) 
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, prefix, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if(NULL != collection_impl->in_scope_namespaces)
    {
        axis2_char_t *ns = NULL;
        ns = (axis2_char_t *)axis2_hash_get(
                collection_impl->in_scope_namespaces, 
                prefix, AXIS2_HASH_KEY_STRING);
        return ns;                
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_collection_map_namespace(
        axis2_xml_schema_collection_t  *collection,
        const axis2_env_t *env,
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
        const axis2_env_t *env,
        axis2_xml_schema_t* schema,
        axis2_char_t *type_name)
{
    void* type = NULL;
    type = axis2_xml_schema_simple_type_create(env, schema);
    AXIS2_XML_SCHEMA_TYPE_SET_NAME(type, env, type_name);
    AXIS2_XML_SCHEMA_ADD_TYPE(schema, env, type);
    return AXIS2_SUCCESS;
}        

axis2_hash_t* AXIS2_CALL 
axis2_xml_schema_collection_get_systemid2_schemas(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->systemid2_schemas; 
}        

axis2_array_list_t* AXIS2_CALL 
axis2_xml_schema_collection_get_schemas(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->schemas;
} 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_collection_set_schemas(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env,
        axis2_array_list_t *schemas)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if(NULL != collection_impl->schemas)
    {
        /** TODO */
    }
    collection_impl->schemas = schemas;
    return AXIS2_SUCCESS;
}     

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_collection_get_namespaces(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->namespaces;
}                  

axis2_xml_schema_t* AXIS2_CALL 
axis2_xml_schema_collection_get_schema(
        axis2_xml_schema_collection_t *collection,
        const axis2_env_t *env,
        axis2_char_t *system_id)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, system_id, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return axis2_hash_get(collection_impl->systemid2_schemas,
        system_id, AXIS2_HASH_KEY_STRING);
}        

axis2_char_t* AXIS2_CALL
axis2_xml_schema_collection_get_base_uri(
        axis2_xml_schema_collection_t* collection,
        const axis2_env_t *env)
{
    axis2_xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->base_uri;
}        
