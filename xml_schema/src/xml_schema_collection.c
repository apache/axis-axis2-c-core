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

#include <xml_schema_includes.h>
#include <axis2_hash.h>

typedef struct xml_schema_collection_impl
            xml_schema_collection_impl_t;

struct xml_schema_collection_impl
{
    xml_schema_collection_t collection;

    axis2_hash_t *namespaces;

    axis2_char_t *base_uri;

    axis2_hash_t *in_scope_namespaces;

    axis2_hash_t *systemid2_schemas;

    xml_schema_t *xsd;

    axis2_array_list_t *schemas;

    axis2_hash_t *unresolved_types;

    axis2_array_list_t *builder_list;
};

#define AXIS2_INTF_TO_IMPL(collection) \
        ((xml_schema_collection_impl_t *) collection)


axis2_status_t AXIS2_CALL
xml_schema_collection_free(
    xml_schema_collection_t* collection,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_collection_set_base_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_char_t *base_uri);

axis2_char_t* AXIS2_CALL
xml_schema_collection_get_base_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_collection_init(
    xml_schema_collection_t* collection,
    const axis2_env_t *env);


xml_schema_t* AXIS2_CALL
xml_schema_collection_read_with_reader(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_xml_reader_t *reader);

xml_schema_t * AXIS2_CALL
xml_schema_collection_read_document(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_document_t* document);

xml_schema_t * AXIS2_CALL
xml_schema_collection_read_element(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_node_t *node);

xml_schema_t* AXIS2_CALL
xml_schema_collection_read_document_with_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_document_t *document,
    axis2_char_t *uri);

xml_schema_t * AXIS2_CALL
xml_schema_collection_read_element_with_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *uri);

xml_schema_element_t * AXIS2_CALL
xml_schema_collection_get_element_by_qname(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *qname);

xml_schema_type_t * AXIS2_CALL
xml_schema_collection_get_type_by_qname(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *schema_type_qname);


axis2_status_t AXIS2_CALL
xml_schema_collection_add_unresolved_type(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *qtype,
    void *receiver);

axis2_status_t AXIS2_CALL
xml_schema_collection_resolve_type(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *type_qname,
    xml_schema_type_t *type);

axis2_char_t * AXIS2_CALL
xml_schema_collection_get_namespace_for_prefix(
    xml_schema_collection_t *collection,
    const axis2_env_t *env,
    axis2_char_t *prefix);


axis2_status_t AXIS2_CALL
xml_schema_collection_map_namespace(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_char_t *prefix,
    axis2_char_t *namespc_uri);

static axis2_status_t
add_simple_type(
    const axis2_env_t *env,
    xml_schema_t* schema,
    axis2_char_t *type_name);

axis2_hash_t* AXIS2_CALL
xml_schema_collection_get_systemid2_schemas(
    xml_schema_collection_t *collection,
    const axis2_env_t *env);

axis2_array_list_t* AXIS2_CALL
xml_schema_collection_get_schemas(
    xml_schema_collection_t *collection,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_collection_set_schemas(
    xml_schema_collection_t *collection,
    const axis2_env_t *env,
    axis2_array_list_t *schemas);

axis2_hash_t* AXIS2_CALL
xml_schema_collection_get_namespaces(
    xml_schema_collection_t *collection,
    const axis2_env_t *env);

xml_schema_t* AXIS2_CALL
xml_schema_collection_get_schema(
    xml_schema_collection_t *collection,
    const axis2_env_t *env,
    axis2_char_t *system_id);


AXIS2_EXTERN xml_schema_collection_t * AXIS2_CALL
xml_schema_collection_create(const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collection_impl = NULL;

    collection_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_collection_impl_t));
    if (!collection_impl)
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
            sizeof(xml_schema_collection_ops_t));

    if (!collection_impl->collection.ops)
    {
        xml_schema_collection_free(&(collection_impl->collection), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    collection_impl->collection.ops->free =
        xml_schema_collection_free;

    collection_impl->collection.ops->set_base_uri =
        xml_schema_collection_set_base_uri;

    collection_impl->collection.ops->get_base_uri =
        xml_schema_collection_get_base_uri;

    collection_impl->collection.ops->init =
        xml_schema_collection_init;

    collection_impl->collection.ops->read_with_reader =
        xml_schema_collection_read_with_reader;

    collection_impl->collection.ops->read_document =
        xml_schema_collection_read_document;

    collection_impl->collection.ops->read_element =
        xml_schema_collection_read_element;

    collection_impl->collection.ops->read_document_with_uri =
        xml_schema_collection_read_document_with_uri;

    collection_impl->collection.ops->read_element_with_uri =
        xml_schema_collection_read_element_with_uri;

    collection_impl->collection.ops->get_element_by_qname =
        xml_schema_collection_get_element_by_qname;

    collection_impl->collection.ops->get_type_by_qname =
        xml_schema_collection_get_type_by_qname;

    collection_impl->collection.ops->add_unresolved_type =
        xml_schema_collection_add_unresolved_type;

    collection_impl->collection.ops->resolve_type =
        xml_schema_collection_resolve_type;

    collection_impl->collection.ops->get_namespace_for_prefix =
        xml_schema_collection_get_namespace_for_prefix;

    collection_impl->collection.ops->map_namespace =
        xml_schema_collection_map_namespace;

    collection_impl->collection.ops->get_schemas =
        xml_schema_collection_get_schemas;

    collection_impl->collection.ops->get_systemid2_schemas =
        xml_schema_collection_get_systemid2_schemas;

    collection_impl->collection.ops->get_namespaces =
        xml_schema_collection_get_namespaces;

    collection_impl->collection.ops->get_schema =
        xml_schema_collection_get_schema;


    collection_impl->xsd = xml_schema_create(
                env, XML_SCHEMA_NS , &(collection_impl->collection));

    if (!collection_impl->xsd)
    {
        xml_schema_collection_free(&(collection_impl->collection), env);
        return NULL;
    }

    collection_impl->namespaces          = axis2_hash_make(env);
    collection_impl->in_scope_namespaces = axis2_hash_make(env);
    collection_impl->systemid2_schemas   = axis2_hash_make(env);
    collection_impl->schemas             = axis2_array_list_create(env, 10);
    collection_impl->unresolved_types    = axis2_hash_make(env);
    collection_impl->builder_list        = axis2_array_list_create(env, 10);

    if (!collection_impl->namespaces || !collection_impl->in_scope_namespaces ||
            !collection_impl->systemid2_schemas || !collection_impl->schemas ||
            !collection_impl->unresolved_types)
    {
        xml_schema_collection_free(&(collection_impl->collection), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    xml_schema_collection_init(&(collection_impl->collection), env);
    return &(collection_impl->collection);
}


axis2_status_t AXIS2_CALL
xml_schema_collection_free(
    xml_schema_collection_t *collection,
    const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collec_impl = NULL;

    collec_impl = AXIS2_INTF_TO_IMPL(collection);
    if (collec_impl->builder_list)
    {
        int i = 0, count = 0;
        count = axis2_array_list_size(collec_impl->builder_list, env);
        for (i = 0; i < count; i++)
        {
            xml_schema_builder_t *builder = NULL;
            builder = axis2_array_list_get(collec_impl->builder_list, env, i);
            if (builder)
            {
                XML_SCHEMA_BUILDER_FREE(builder, env);
                builder = NULL;
            }
        }
        axis2_array_list_free(collec_impl->builder_list, env);
        collec_impl->builder_list = NULL;
    }


    if (collection->ops)
    {
        AXIS2_FREE(env->allocator, collection->ops);
        collection->ops = NULL;
    }


    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(collection));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
xml_schema_collection_set_base_uri(
    xml_schema_collection_t*collection,
    const axis2_env_t *env,
    axis2_char_t *base_uri)
{
    xml_schema_collection_impl_t *collec_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, base_uri, AXIS2_FAILURE);

    collec_impl = AXIS2_INTF_TO_IMPL(collection);

    if (collec_impl->base_uri)
    {
        AXIS2_FREE(env->allocator, collec_impl->base_uri);
        collec_impl->base_uri = NULL;
    }
    collec_impl->base_uri = AXIS2_STRDUP(base_uri, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
xml_schema_collection_init(
    xml_schema_collection_t* collection,
    const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
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
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_STRING);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_BOOLEAN);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_FLOAT);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_DOUBLE);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_QNAME);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_DECIMAL);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_DURATION);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_DATE);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_TIME);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_DATETIME);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_DAY);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_MONTH);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_MONTHDAY);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_YEAR);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_YEARMONTH);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NOTATION);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_HEXBIN);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_BASE64);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_ANYURI);

    /** derived types from decimal */

    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_LONG);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_SHORT);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_BYTE);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_INTEGER);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_INT);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_POSITIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NEGATIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NONPOSITIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NONNEGATIVEINTEGER);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_UNSIGNEDBYTE);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_UNSIGNEDINT);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_UNSIGNEDLONG);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_UNSIGNEDSHORT);

    /** derived types from string */

    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NAME);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NORMALIZEDSTRING);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NCNAME);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NMTOKEN);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_NMTOKENS);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_ENTITY);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_ENTITIES);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_ID);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_IDREF);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_IDREFS);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_LANGUAGE);
    add_simple_type(env, collection_impl->xsd, XML_SCHEMA_XSD_TOKEN);

    axis2_hash_set(collection_impl->namespaces, XML_SCHEMA_NS,
            AXIS2_HASH_KEY_STRING,  collection_impl->xsd);

    return AXIS2_SUCCESS;
}


xml_schema_t* AXIS2_CALL
xml_schema_collection_read_with_reader(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_xml_reader_t *reader)
{
    xml_schema_builder_t *sch_builder = NULL;
    axiom_document_t *om_doc = NULL;
    axiom_stax_builder_t *om_builder = NULL;

    AXIS2_PARAM_CHECK(env->error, reader, NULL);

    om_builder = axiom_stax_builder_create(env, reader);
    if (!om_builder)
        return NULL;

    om_doc = axiom_document_create(env, NULL, om_builder);
    if (!om_doc)
    {
        axiom_stax_builder_free(om_builder, env);
        return NULL;
    }
    axiom_document_build_all(om_doc, env);

    sch_builder = xml_schema_builder_create(env, collection);
    if (!sch_builder)
    {
        axiom_stax_builder_free(om_builder, env);
        return NULL;
    }

    return XML_SCHEMA_BUILDER_BUILD(sch_builder, env, om_doc, NULL);
}

xml_schema_t * AXIS2_CALL
xml_schema_collection_read_document(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_document_t* document)
{
    xml_schema_builder_t *sch_builder = NULL;
    xml_schema_collection_impl_t *collec_impl = NULL;
    AXIS2_PARAM_CHECK(env->error , document, NULL);
    collec_impl = AXIS2_INTF_TO_IMPL(collection);
    sch_builder = xml_schema_builder_create(env, collection);
    axis2_array_list_add(collec_impl->builder_list, env, sch_builder);
    return XML_SCHEMA_BUILDER_BUILD(sch_builder, env, document, NULL);
}

xml_schema_t * AXIS2_CALL
xml_schema_collection_read_element(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_node_t *node)
{
    xml_schema_builder_t *sch_builder = NULL;
    xml_schema_collection_impl_t *collec_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, node, NULL);

    sch_builder = xml_schema_builder_create(env, collection);

    if (!sch_builder)
        return NULL;
    collec_impl = AXIS2_INTF_TO_IMPL(collection);
    axis2_array_list_add(collec_impl->builder_list, env, sch_builder);
    return XML_SCHEMA_BUILDER_BUILD_WITH_ROOT_NODE(sch_builder, env, node, NULL);
}

xml_schema_t* AXIS2_CALL
xml_schema_collection_read_document_with_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_document_t *document,
    axis2_char_t *uri)
{
    xml_schema_builder_t *sch_builder = NULL;
    xml_schema_collection_impl_t *sch_collec_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error , document, NULL);
    sch_collec_impl = AXIS2_INTF_TO_IMPL(collection);
    sch_builder = xml_schema_builder_create(env, collection);
    axis2_array_list_add(sch_collec_impl->builder_list, env, sch_builder);
    return XML_SCHEMA_BUILDER_BUILD(sch_builder, env, document, uri);
}

xml_schema_t * AXIS2_CALL
xml_schema_collection_read_element_with_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *uri)
{
    xml_schema_builder_t *sch_builder = NULL;
    xml_schema_collection_impl_t *collection_impl = NULL;

    AXIS2_PARAM_CHECK(env->error, node, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);

    sch_builder = xml_schema_builder_create(env, collection);
    if (!sch_builder)
        return NULL;

    axis2_array_list_add(collection_impl->builder_list, env, sch_builder);

    return XML_SCHEMA_BUILDER_BUILD_WITH_ROOT_NODE(sch_builder, env, node, uri);
}

xml_schema_element_t * AXIS2_CALL
xml_schema_collection_get_element_by_qname(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    axis2_char_t *uri = NULL;
    xml_schema_t *schema = NULL;
    xml_schema_collection_impl_t *collec_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    collec_impl = AXIS2_INTF_TO_IMPL(collection);

    uri = AXIS2_QNAME_GET_URI(qname, env);

    if (collec_impl->namespaces)
        schema = axis2_hash_get(collec_impl->namespaces, uri, AXIS2_HASH_KEY_STRING);

    if (!schema)
        return NULL;

    return XML_SCHEMA_GET_ELEMENT_BY_QNAME(schema, env, qname);
}

xml_schema_type_t * AXIS2_CALL
xml_schema_collection_get_type_by_qname(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *schema_type_qname)
{

    xml_schema_collection_impl_t *collecion_impl = NULL;

    axis2_char_t *uri = NULL;

    xml_schema_t *schema = NULL;

    AXIS2_PARAM_CHECK(env->error, schema_type_qname, NULL);
    collecion_impl = AXIS2_INTF_TO_IMPL(collection);

    uri = AXIS2_QNAME_GET_URI(schema_type_qname, env);

    if (collecion_impl->namespaces && NULL != uri)
        schema = axis2_hash_get(collecion_impl->namespaces, uri, AXIS2_HASH_KEY_STRING);

    if (!schema)
        return NULL;
    return XML_SCHEMA_GET_TYPE_BY_QNAME(schema, env, schema_type_qname);
}


axis2_status_t AXIS2_CALL
xml_schema_collection_add_unresolved_type(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *qtype,
    void *obj)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    axis2_char_t *qn_string = NULL;
    axis2_array_list_t *receivers = NULL;


    AXIS2_PARAM_CHECK(env->error, qtype, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, obj, AXIS2_FAILURE);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);

    qn_string = AXIS2_QNAME_TO_STRING(qtype, env);
    if (collection_impl->unresolved_types &&
            qn_string)
    {
        receivers = axis2_hash_get(collection_impl->unresolved_types,
                qn_string, AXIS2_HASH_KEY_STRING);

        if (!receivers)
        {
            receivers = axis2_array_list_create(env, 10);
            axis2_hash_set(collection_impl->unresolved_types, qn_string,
                    AXIS2_HASH_KEY_STRING, receivers);
        }
    }

    axis2_array_list_add(receivers, env, obj);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
xml_schema_collection_resolve_type(
    xml_schema_collection_t* collection,
    const axis2_env_t *env,
    axis2_qname_t *type_qname,
    xml_schema_type_t *type)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    axis2_char_t *qn_string = NULL;
    axis2_array_list_t *receivers = NULL;
    AXIS2_PARAM_CHECK(env->error, type_qname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type, AXIS2_FAILURE);

    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    qn_string = AXIS2_QNAME_TO_STRING(type_qname, env);
    if (qn_string && NULL != collection_impl->unresolved_types)
    {
        receivers = axis2_hash_get(collection_impl->unresolved_types,
                qn_string, AXIS2_HASH_KEY_STRING);

        if (receivers)
        {
            int i = 0;

            for (i = 0; i < axis2_array_list_size(receivers, env); i++)
            {
                void *obj = NULL;
                obj = axis2_array_list_get(receivers, env, i);
                if (obj)
                {
                    XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(obj, env, type);
                }
            }
        }
        else
        {
            return AXIS2_FAILURE;
        }
        if (collection_impl->unresolved_types)
            axis2_hash_set(collection_impl->unresolved_types, qn_string,
                    AXIS2_HASH_KEY_STRING, NULL);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
xml_schema_collection_get_namespace_for_prefix(
    xml_schema_collection_t  *collection,
    const axis2_env_t *env,
    axis2_char_t *prefix)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, prefix, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if (collection_impl->in_scope_namespaces)
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
xml_schema_collection_map_namespace(
    xml_schema_collection_t  *collection,
    const axis2_env_t *env,
    axis2_char_t *prefix,
    axis2_char_t *namespc_uri)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if (collection_impl->in_scope_namespaces)
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
    xml_schema_t* schema,
    axis2_char_t *type_name)
{
    void* type = NULL;
    type = xml_schema_simple_type_create(env, schema);
    XML_SCHEMA_TYPE_SET_NAME(type, env, type_name);
    XML_SCHEMA_ADD_TYPE(schema, env, type);
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
xml_schema_collection_get_systemid2_schemas(
    xml_schema_collection_t *collection,
    const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->systemid2_schemas;
}

axis2_array_list_t* AXIS2_CALL
xml_schema_collection_get_schemas(
    xml_schema_collection_t *collection,
    const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->schemas;
}

axis2_status_t AXIS2_CALL
xml_schema_collection_set_schemas(
    xml_schema_collection_t *collection,
    const axis2_env_t *env,
    axis2_array_list_t *schemas)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    if (collection_impl->schemas)
    {
        /** TODO */
    }
    collection_impl->schemas = schemas;
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
xml_schema_collection_get_namespaces(
    xml_schema_collection_t *collection,
    const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->namespaces;
}

xml_schema_t* AXIS2_CALL
xml_schema_collection_get_schema(
    xml_schema_collection_t *collection,
    const axis2_env_t *env,
    axis2_char_t *system_id)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, system_id, NULL);
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return axis2_hash_get(collection_impl->systemid2_schemas,
            system_id, AXIS2_HASH_KEY_STRING);
}

axis2_char_t* AXIS2_CALL
xml_schema_collection_get_base_uri(
    xml_schema_collection_t* collection,
    const axis2_env_t *env)
{
    xml_schema_collection_impl_t *collection_impl = NULL;
    collection_impl = AXIS2_INTF_TO_IMPL(collection);
    return collection_impl->base_uri;
}
