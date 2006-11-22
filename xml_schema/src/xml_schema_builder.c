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

#include <xml_schema_includes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <axis2_string_util.h>
#include <axis2_allocator.h>
#include <limits.h>
#include <axis2_string.h>
#include <axiom_util.h>
#include <xml_schema_complex_content_restriction.h>

typedef struct xml_schema_builder_impl
{
    xml_schema_builder_t builder;

    axiom_node_t *root_node;

    void *schema;

    xml_schema_collection_t *collection;

    int gen_no;

    axis2_array_list_t *document_list;

    axiom_node_t *schema_element;

}
xml_schema_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(builder) \
        ((xml_schema_builder_impl_t*)builder)


xml_schema_t* AXIS2_CALL
xml_schema_builder_build(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_document_t *om_doc,
    axis2_char_t *uri);

xml_schema_t* AXIS2_CALL
xml_schema_builder_build_with_root_node(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *root,
    axis2_char_t *uri);

axis2_status_t AXIS2_CALL
xml_schema_builder_free(
    xml_schema_builder_t *builder,
    const axis2_env_t *env);


static xml_schema_t*
handle_xml_schema_element(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axis2_char_t  *uri);

static xml_schema_annotation_t*
handle_annotation(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ann_node,
    axiom_node_t *sch_node);

static xml_schema_redefine_t*
handle_redefine(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *redefine_node,
    axiom_node_t *sch_node);

static axis2_status_t
set_namespace_attributes(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axiom_node_t *sch_node);

static axis2_status_t
put_namespace(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axis2_char_t *prefix,
    axis2_char_t *ns);

static xml_schema_simple_type_t*
handle_simple_type(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *simple_node,
    axiom_node_t *sch_node);

static xml_schema_complex_type_t*
handle_complex_type(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *complex_node,
    axiom_node_t *schema_node);

static xml_schema_simple_content_t*
handle_simple_content(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *simple_node,
    axiom_node_t *schema_node);

static xml_schema_complex_content_t*
handle_complex_content(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *complex_node,
    axiom_node_t *schema_node);

static xml_schema_simple_content_restriction_t*
handle_simple_content_restriction(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *res_node,
    axiom_node_t *schema_node);


static xml_schema_simple_content_extension_t*
handle_simple_content_extension(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *sim_ext_node,
    axiom_node_t *schema_node);

static xml_schema_complex_content_restriction_t*
handle_complex_content_restriction(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *res_node,
    axiom_node_t *schema_node);

static xml_schema_complex_content_extension_t*
handle_complex_content_extension(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *cmp_ext_node,
    axiom_node_t *schema_node);

static xml_schema_attribute_group_ref_t*
handle_attribute_group_ref(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *attr_grp_node,
    axiom_node_t *schema_node);

static xml_schema_group_base_t*
handle_sequence(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *seq_node,
    axiom_node_t *schema_node);

static xml_schema_any_t*
handle_any(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *any_node,
    axiom_node_t *schema_node);

static xml_schema_group_base_t*
handle_choice(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *choice_node,
    axiom_node_t *schema_node);

static xml_schema_group_base_t*
handle_all(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *all_node,
    axiom_node_t *schema_node);

static xml_schema_group_t*
handle_group(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *group_node,
    axiom_node_t *schema_node);

static xml_schema_attribute_group_t*
handle_attribute_group(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *attr_grp_node,
    axiom_node_t *schema_node);

static xml_schema_any_attribute_t*
handle_any_attribute(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *any_attr_node,
    axiom_node_t *schema_node);

static xml_schema_group_ref_t*
handle_group_ref(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *grp_ref_node,
    axiom_node_t *schema_node);

static xml_schema_attribute_t*
handle_attribute(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *attr_node,
    axiom_node_t *schema_node);

static xml_schema_element_t*
handle_element(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axiom_node_t *schema_node,
    axis2_bool_t is_global);
/*
static axis2_status_t
populate_element_namespaces(
        xml_schema_builder_t *builder,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axis2_hash_t* ele_to_ns_map);
*/
static xml_schema_identity_constraint_t*
handle_constraint(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *attr_node,
    axiom_node_t *schema_node,
    int type);

static xml_schema_import_t*
handle_import(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *import_node,
    axiom_node_t *schema_node);

static xml_schema_external_t *
handle_include(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *include_node,
    axiom_node_t *schema_node);


static xml_schema_app_info_t*
handle_app_info(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *content);

static xml_schema_documentation_t*
handle_documentation(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *content);

static long
get_min_occurs(
    const axis2_env_t *env,
    axiom_node_t *content);

static long
get_max_occurs(
    const axis2_env_t *env,
    axiom_node_t *content);

static xml_schema_derivation_method_t*
get_derivation(
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axis2_char_t *attr_name);

static axis2_char_t *
get_enum_string(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_element_t *om_ele,
    axis2_char_t *attr_name);

xml_schema_t* AXIS2_CALL
xml_schema_builder_resolve_xml_schema(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axis2_char_t *target_namespace,
    axis2_char_t *schema_location,
    axis2_char_t *base_uri);

static axis2_bool_t
value_exist_in_hash(
    const axis2_env_t *env,
    axis2_hash_t *ht,
    axis2_char_t *value);

static xml_schema_form_t*
get_from_default(
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axis2_char_t *attr_name);

static xml_schema_t *
resolve_xml_schema(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axis2_char_t *target_namespace,
    axis2_char_t *schema_location);

static xml_schema_t *
resolve_xml_schema_with_uri(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axis2_char_t *target_namespace,
    axis2_char_t *schema_location,
    axis2_char_t *base_uri);

static xml_schema_annotation_t*
handle_annotation_with_element(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ann_node);

AXIS2_EXTERN xml_schema_builder_t * AXIS2_CALL
xml_schema_builder_create(
    const axis2_env_t *env,
    xml_schema_collection_t *sch_collection)
{
    xml_schema_builder_impl_t *builder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, sch_collection, NULL);

    builder_impl = (xml_schema_builder_impl_t *)
            AXIS2_MALLOC(env->allocator, sizeof(xml_schema_builder_impl_t));

    if (!builder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    builder_impl->builder.ops = NULL;

    builder_impl->collection = NULL;

    builder_impl->root_node = NULL;

    builder_impl->schema = NULL;

    builder_impl->gen_no = 0;

    builder_impl->document_list = NULL;


    builder_impl->schema = xml_schema_create(env, NULL, sch_collection);
    if (!builder_impl->schema)
    {
        xml_schema_builder_free(&(builder_impl->builder), env);
        return NULL;
    }

    builder_impl->collection = sch_collection;

    builder_impl->builder.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_builder_ops_t));
    if (!builder_impl->builder.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        xml_schema_builder_free(&(builder_impl->builder), env);
        return NULL;
    }

    builder_impl->document_list = axis2_array_list_create(env, 2);

    builder_impl->builder.ops->free =
        xml_schema_builder_free;

    builder_impl->builder.ops->build =
        xml_schema_builder_build;

    builder_impl->builder.ops->build_with_root_node =
        xml_schema_builder_build_with_root_node;

    return &(builder_impl->builder);
}

xml_schema_t* AXIS2_CALL
xml_schema_builder_build(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_document_t *om_doc,
    axis2_char_t *uri)
{
    xml_schema_builder_impl_t *builder_impl = NULL;
    axiom_node_t *root_node = NULL;

    AXIS2_PARAM_CHECK(env->error, om_doc, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    AXIS2_ARRAY_LIST_ADD(builder_impl->document_list, env, om_doc);

    root_node = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(om_doc, env);

    if (!root_node)
        return NULL;

    return handle_xml_schema_element(builder, env, root_node, uri);
}

xml_schema_t* AXIS2_CALL
xml_schema_builder_build_with_root_node(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *root,
    axis2_char_t *uri)
{
    return handle_xml_schema_element(builder, env, root, uri);
}

axis2_status_t AXIS2_CALL
xml_schema_builder_free(
    xml_schema_builder_t *builder,
    const axis2_env_t *env)
{
    xml_schema_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if (builder_impl->schema)
    {
        XML_SCHEMA_FREE(builder_impl->schema, env);
        builder_impl->schema = NULL;
    }

    if (builder_impl->builder.ops)
    {
        AXIS2_FREE(env->allocator, builder_impl->builder.ops);
        builder_impl->builder.ops = NULL;
    }
    AXIS2_FREE(env->allocator, builder_impl);
    builder_impl = NULL;

    return AXIS2_SUCCESS;
}


static xml_schema_t*
handle_xml_schema_element(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axis2_char_t  *uri)
{
    xml_schema_builder_impl_t *builder_impl = NULL;

    axis2_hash_t *ht_sch2schemas = NULL;
    axis2_array_list_t *schemas        = NULL;
    axis2_hash_t *namespaces     = NULL;
    axis2_char_t *target_ns = NULL;

    /** element form default */
    void *ele_form = NULL;
    void *attr_form = NULL;

    void *drv = NULL;

    /** ele and node */
    axiom_element_t *child_ele = NULL;
    axiom_node_t    *child_ele_node = NULL;


    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    set_namespace_attributes(builder, env, builder_impl->schema, ele_node);
    XML_SCHEMA_SET_ROOT_NODE(builder_impl->schema, env, ele_node);

    if (uri)
    {
        ht_sch2schemas = XML_SCHEMA_COLLECTION_GET_SYSTEMID2_SCHEMAS(
                    builder_impl->collection, env);

        if (ht_sch2schemas)
        {
            axis2_hash_set(ht_sch2schemas, uri,
                    AXIS2_HASH_KEY_STRING, builder_impl->schema);
        }
    }
    schemas =
        XML_SCHEMA_COLLECTION_GET_SCHEMAS(builder_impl->collection, env);
    if (schemas)
    {
        AXIS2_ARRAY_LIST_ADD(schemas, env, builder_impl->schema);
    }


    namespaces =
        XML_SCHEMA_COLLECTION_GET_NAMESPACES(builder_impl->collection, env);
    target_ns = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);

    if (namespaces && NULL != target_ns)
    {
        void *val = NULL;
        val = axis2_hash_get(namespaces, target_ns, AXIS2_HASH_KEY_STRING);
        if (!val)
        {
            /** value does not exist */
            axis2_hash_set(namespaces, target_ns,
                    AXIS2_HASH_KEY_STRING, builder_impl->schema);

        }
    }

    ele_form = get_from_default(env, ele_node, "elementFormDefault");
    XML_SCHEMA_SET_ELEMENT_FORM_DEFAULT(builder_impl->schema, env, ele_form);

    ele_form = NULL;

    attr_form = get_from_default(env, ele_node, "attributeFormDefault");
    XML_SCHEMA_SET_ATTR_FORM_DEFAULT(builder_impl->schema, env, attr_form);
    attr_form = NULL;

    drv = get_derivation(env, ele_node, "blockDefault");
    XML_SCHEMA_SET_BLOCK_DEFAULT(builder_impl->schema, env, drv);

    drv = NULL;

    drv = get_derivation(env, ele_node, "finalDefault");
    XML_SCHEMA_SET_FINAL_DEFAULT(builder_impl->schema, env, drv);

    if (uri)
        XML_SCHEMA_OBJ_SET_SOURCE_URI(builder_impl->schema, env, uri);
    /***********
         * for ( each childElement)
         *      if( simpleTypeElement)
         *         handleSimpleType
         *      else if( complexType)
         *         handleComplexType
         *      else if( element)
         *         handleElement
         *      else if( include)
         *         handleInclude
         *      else if( import)
         *         handleImport
         *      else if (group)
         *         handleGroup
         *      else if (attributeGroup)
         *         handleattributeGroup
         *      else if( attribute)
         *         handleattribute
         *      else if (redefine)
         *         handleRedefine
         *      else if(notation)
         *         handleNotation
         */
    child_ele = axiom_util_get_first_child_element_with_uri(ele_node, env,
            XML_SCHEMA_NS, &child_ele_node);
    while (child_ele)
    {
        axis2_char_t *localname = NULL;
        xml_schema_obj_collection_t *items = NULL;
        xml_schema_obj_collection_t *includes = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(child_ele, env);
        items = XML_SCHEMA_GET_ITEMS(builder_impl->schema, env);
        includes = XML_SCHEMA_GET_INCLUDES(builder_impl->schema, env);

        if (!localname)
            continue;

        if (AXIS2_STRCMP(localname, "simpleType") == 0)
        {
            void *type = NULL;
            void *t = NULL;
            axis2_qname_t *qn = NULL;


            type = handle_simple_type(builder, env, child_ele_node, ele_node);

            XML_SCHEMA_ADD_TYPE(builder_impl->schema, env, type);
            if (items)
            {
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
            }
            qn = XML_SCHEMA_TYPE_GET_QNAME(type, env);
            t =  XML_SCHEMA_GET_TYPE_BY_QNAME(builder_impl->schema, env, qn);
            if (qn)
            {
                axis2_char_t *u = NULL;
                axis2_char_t *p = NULL;

                u = AXIS2_QNAME_GET_URI(qn, env);
                p = AXIS2_QNAME_GET_LOCALPART(qn, env);
                XML_SCHEMA_COLLECTION_RESOLVE_TYPE(builder_impl->collection,
                        env, qn, type);
            }


        }
        else if (AXIS2_STRCMP(localname, "complexType") == 0)
        {
            void *type = NULL;
            axis2_qname_t *qn = NULL;
            type = handle_complex_type(builder, env, child_ele_node, ele_node);

            XML_SCHEMA_ADD_TYPE(builder_impl->schema, env, type);
            if (items)
            {
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
            }

            qn = XML_SCHEMA_TYPE_GET_QNAME(type, env);
            if (qn)
            {
                XML_SCHEMA_COLLECTION_RESOLVE_TYPE(
                    builder_impl->collection, env, qn, type);
            }
        }
        else if (AXIS2_STRCMP(localname, "element") == 0)
        {
            void *sch_ele = NULL;
            sch_ele = handle_element(builder, env, child_ele_node, ele_node, AXIS2_TRUE);
            if (sch_ele)
            {
                axis2_qname_t *qualified_qname = NULL;
                axis2_char_t *qname_uri = NULL; /* for debuging */
                axis2_qname_t *ref_qname = NULL;
                xml_schema_obj_table_t *elements = NULL;

                elements = XML_SCHEMA_GET_ELEMENTS(builder_impl->schema, env);

                qualified_qname = XML_SCHEMA_ELEMENT_GET_QNAME(sch_ele, env);
                ref_qname = XML_SCHEMA_ELEMENT_GET_REF_NAME(sch_ele, env);

                if (qualified_qname && NULL != elements)
                {
                    qname_uri = AXIS2_QNAME_GET_URI(qualified_qname, env);
                    XML_SCHEMA_OBJ_TABLE_ADD(elements, env,
                            qualified_qname, sch_ele);
                }
                else if (ref_qname && NULL != elements)
                {
                    XML_SCHEMA_OBJ_TABLE_ADD(elements, env, ref_qname, sch_ele);
                }
                if (items)
                    XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, sch_ele);
            }
        }
        else if (AXIS2_STRCMP(localname, "include") == 0)
        {
            void *inc_ele = NULL;

            inc_ele = handle_include(builder, env, child_ele_node, ele_node);
            if (includes)
                XML_SCHEMA_OBJ_COLLECTION_ADD(includes, env, inc_ele);
            if (items)
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, inc_ele);
        }
        else if (AXIS2_STRCMP(localname, "import") == 0)
        {
            void *import_ele = NULL;
            import_ele = handle_import(builder, env, child_ele_node, ele_node);
            if (includes)
                XML_SCHEMA_OBJ_COLLECTION_ADD(includes, env, import_ele);
            if (items)
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, import_ele);
        }
        else if (AXIS2_STRCMP(localname, "group"))
        {
            void *group = NULL;
            xml_schema_obj_table_t *groups = NULL;
            axis2_char_t *name  = NULL;
            group = handle_group(builder, env, child_ele_node, ele_node);
            groups = XML_SCHEMA_GET_GROUPS(builder_impl->schema, env);
            if (group)
            {
                name = XML_SCHEMA_GROUP_GET_NAME(group, env);
                XML_SCHEMA_OBJ_TABLE_PUT(groups, env, name, group);
            }
            if (items)
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, group);
        }
        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp = NULL;
            axis2_char_t *name = NULL;
            xml_schema_obj_table_t *attr_grps = NULL;
            attr_grp = handle_attribute_group(builder, env, child_ele_node,
                    ele_node);
            attr_grps = XML_SCHEMA_GET_ATTR_GROUPS(builder_impl->schema, env);
            if (attr_grp)
            {
                name = XML_SCHEMA_ATTRIBUTE_GROUP_GET_NAME(attr_grp, env);
                XML_SCHEMA_OBJ_TABLE_PUT(attr_grps,
                        env, name, attr_grp);
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, attr_grp);
            }
        }
        else if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attr = NULL;
            xml_schema_obj_table_t *attr_tbl = NULL;
            attr = handle_attribute(builder, env, child_ele_node, ele_node);
            attr_tbl = XML_SCHEMA_GET_ATTRS(builder_impl->schema, env);
            if (attr)
            {
                axis2_qname_t *qn = NULL;
                qn = XML_SCHEMA_ATTRIBUTE_GET_QNAME(attr, env);
                XML_SCHEMA_OBJ_TABLE_ADD(attr_tbl, env, qn, attr);
                XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, attr);
            }
        }
        else if (AXIS2_STRCMP(localname, "redefine") == 0)
        {
            void *redefine = NULL;
            redefine = handle_redefine(builder, env, child_ele_node, ele_node);
            XML_SCHEMA_OBJ_COLLECTION_ADD(includes, env, redefine);
        }
        else if (AXIS2_STRCMP(localname, "notation") == 0)
        {
            /**TODO implement notation */
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation(builder, env, child_ele_node, ele_node);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(
                builder_impl->schema, env, annotation);
        }
        child_ele = NULL;
        child_ele = axiom_util_get_next_sibling_element_with_uri(child_ele_node, env,
                XML_SCHEMA_NS, &child_ele_node);
    }
    return builder_impl->schema;
}

static xml_schema_annotation_t*
handle_annotation(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ann_node,
    axiom_node_t *sch_node)
{
    void *annotation = NULL;

    axiom_element_t *child_ele = NULL;

    axiom_node_t *child_node   = NULL;

    axiom_element_t *ann_ele = NULL;

    xml_schema_obj_collection_t *collection = NULL;

    AXIS2_PARAM_CHECK(env->error, builder, NULL);
    AXIS2_PARAM_CHECK(env->error, ann_node, NULL);
    AXIS2_PARAM_CHECK(env->error, sch_node, NULL);

    annotation = xml_schema_annotation_create(env);

    collection = XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env);

    ann_ele = AXIOM_NODE_GET_DATA_ELEMENT(ann_node, env);

    child_ele =
        axiom_util_get_first_child_element(ann_ele, env, ann_node, &child_node);

    while (child_ele)
    {
        axis2_char_t *localname  = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(child_ele, env);

        if (localname && AXIS2_STRCMP(localname, "documentation") == 0)
        {
            void *documentation = NULL;
            documentation = xml_schema_documentation_create(env);
            if (documentation)
            {
                axiom_child_element_iterator_t *ele_iter = NULL;
                ele_iter = axiom_util_get_child_elements(child_ele, env, child_node);
                if (ele_iter)
                {
                    XML_SCHEMA_DOCUMENTATION_SET_MARKUP(documentation, env, ele_iter);
                }
            }
            XML_SCHEMA_OBJ_COLLECTION_ADD(collection, env, documentation);
        }
        child_ele = axiom_util_get_first_child_element(child_ele , env, child_node, &child_node);
    }
    return annotation;
}

static xml_schema_redefine_t*
handle_redefine(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *redefine_node,
    axiom_node_t *sch_node)
{
    void *redefine = NULL;
    void *schema = NULL;
    axiom_element_t *redefine_ele = NULL;

    axiom_element_t *child_ele = NULL;
    axiom_node_t    *child_ele_node = NULL;
    axis2_char_t *schema_location = NULL;
    xml_schema_obj_table_t *schema_types = NULL;
    xml_schema_obj_collection_t *items = NULL;

    AXIS2_PARAM_CHECK(env->error, redefine_node, NULL);
    AXIS2_PARAM_CHECK(env->error, sch_node, NULL);

    redefine_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(redefine_node, env);
    if (!redefine_ele)
        return NULL;

    redefine = xml_schema_redefine_create(env);
    schema_location = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(
                redefine_ele, env, "schemaLocation");



    XML_SCHEMA_EXTERNAL_SET_SCHEMA_LOCATION(redefine, env, schema_location);

    schema = resolve_xml_schema(builder, env, NULL, schema_location);

    XML_SCHEMA_EXTERNAL_SET_SCHEMA(redefine, env, schema);

    schema_types = XML_SCHEMA_REDEFINE_GET_SCHEMA_TYPES(redefine, env);

    items =  XML_SCHEMA_REDEFINE_GET_ITEMS(redefine, env);

    child_ele = axiom_util_get_first_child_element_with_uri(
                redefine_node, env, XML_SCHEMA_NS, &child_ele_node);

    while (child_ele)
    {
        axis2_char_t *localname = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(child_ele, env);
        if (AXIS2_STRCMP(localname, "simpleType") == 0)
        {
            void *type = NULL;
            axis2_qname_t *qn = NULL;

            type = handle_simple_type(builder, env, child_ele_node, sch_node);

            qn = XML_SCHEMA_TYPE_GET_QNAME(type, env);

            XML_SCHEMA_OBJ_TABLE_ADD(schema_types, env, qn, type);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);

        }
        else if (AXIS2_STRCMP(localname, "complexType") == 0)
        {
            void *type = NULL;
            axis2_qname_t *qn = NULL;

            type = handle_complex_type(builder, env, child_ele_node, sch_node);

            qn = XML_SCHEMA_TYPE_GET_QNAME(type, env);

            XML_SCHEMA_OBJ_TABLE_ADD(schema_types, env, qn, type);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
        }
        else if (AXIS2_STRCMP(localname, "group") == 0)
        {
            void *grp = NULL;
            axis2_char_t *name = NULL;
            xml_schema_obj_table_t *groups = NULL;

            groups = XML_SCHEMA_REDEFINE_GET_GROUP(redefine, env);

            grp = handle_group(builder, env, child_ele_node, sch_node);

            name = XML_SCHEMA_GROUP_GET_NAME(grp, env);

            XML_SCHEMA_OBJ_TABLE_PUT(groups, env, name, grp);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, grp);
        }
        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp = NULL;
            xml_schema_obj_table_t *groups = NULL;
            axis2_char_t *name = NULL;

            attr_grp = handle_attribute_group(builder, env, child_ele_node, sch_node);

            groups = XML_SCHEMA_REDEFINE_GET_ATTRIBUTE_GROUP(redefine, env);

            name = XML_SCHEMA_ATTRIBUTE_GROUP_GET_NAME(attr_grp, env);

            XML_SCHEMA_OBJ_TABLE_PUT(groups, env, name, attr_grp);

        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, child_ele_node);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(redefine, env, annotation);
        }
    }
    return redefine;
}

static axis2_status_t
set_namespace_attributes(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axiom_node_t *sch_node)
{
    xml_schema_builder_impl_t *builder_impl = NULL;
    axiom_node_t* parent_node = NULL;
    axis2_hash_t *ht_ns = NULL;
    axis2_hash_index_t *hi_ns = NULL;
    axiom_element_t *om_ele = NULL;

    axis2_hash_t *ht_sch_ns = NULL;
    axis2_qname_t *target_ns_qn = NULL;
    axis2_char_t *contain = NULL;

    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, sch_node, AXIS2_FAILURE);

    if (AXIOM_NODE_GET_NODE_TYPE(sch_node, env) != AXIOM_ELEMENT)
        return AXIS2_FAILURE;

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    parent_node = AXIOM_NODE_GET_PARENT(sch_node, env);

    if (parent_node &&
            AXIOM_NODE_GET_NODE_TYPE(sch_node, env) == AXIOM_ELEMENT)
    {
        set_namespace_attributes(builder, env, schema, parent_node);
    }

    om_ele = AXIOM_NODE_GET_DATA_ELEMENT(sch_node, env);
    if (!om_ele)
        return AXIS2_FAILURE;

    ht_ns = AXIOM_ELEMENT_GET_NAMESPACES(om_ele, env);
    if (ht_ns)
    {
        ht_sch_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(schema, env);
        if (!ht_sch_ns)
            return AXIS2_FAILURE;
    
        for (hi_ns = axis2_hash_first(ht_ns, env); hi_ns;
                hi_ns = axis2_hash_next(env, hi_ns))
        {
            void *val = NULL;
            axis2_hash_this(hi_ns, NULL, NULL, &val);
            if (val)
            {
                axis2_char_t *uri = NULL;
                axis2_char_t *prefix = NULL;
                uri = AXIOM_NAMESPACE_GET_URI((axiom_namespace_t*)val, env);
                prefix =
                    AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t*)val, env);
    
                if (prefix && AXIS2_STRCMP(prefix, "") != 0)
                {
                    axis2_hash_set(ht_sch_ns, prefix,
                            AXIS2_HASH_KEY_STRING, uri);
    
                    if (uri && AXIS2_STRCMP(uri, XML_SCHEMA_NS) == 0)
                    {
                        XML_SCHEMA_SET_SCHEMA_NS_PREFIX(schema, env, prefix);
                    }
                }
                else
                {
                    /** default namespace [xmlns=""] */
                    axis2_hash_set(ht_sch_ns, "", AXIS2_HASH_KEY_STRING, uri);
                }
            }
        }
    }
    /** note: targetNamespace may availabe even though namespace declarations not available */ 
    target_ns_qn = axis2_qname_create(env, "targetNamespace", NULL, NULL);

    contain = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE(om_ele, env, target_ns_qn);

    if (target_ns_qn)
        AXIS2_QNAME_FREE(target_ns_qn, env);

    if (contain &&
            (value_exist_in_hash(env, ht_sch_ns, contain)) == AXIS2_FALSE)
    {
        put_namespace(builder, env, "", contain);
        if (AXIS2_STRCMP(contain, "") != 0)
        {
            XML_SCHEMA_SET_SCHEMA_NS_PREFIX(schema, env, contain);
        }
    }
    if (contain && (AXIS2_STRCMP(contain, "") != 0) &&
            (builder_impl->schema))
    {
        XML_SCHEMA_SET_TARGET_NAMESPACE(builder_impl->schema, env, contain);
    }
    return AXIS2_SUCCESS;
}

static axis2_status_t
put_namespace(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axis2_char_t *prefix,
    axis2_char_t *ns)
{
    xml_schema_builder_impl_t *builder_impl = NULL;
    axis2_hash_t *ht_sch_ns = NULL;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    AXIS2_PARAM_CHECK(env->error, prefix, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns , AXIS2_FAILURE);

    ht_sch_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

    if (axis2_hash_get(ht_sch_ns, prefix, AXIS2_HASH_KEY_STRING))
    {
        /** key already exist */
        axis2_char_t *gen_prefix = AXIS2_MALLOC(env->allocator, sizeof(10));
        sprintf(gen_prefix, "gen%d", builder_impl->gen_no++);
        axis2_hash_set(ht_sch_ns, gen_prefix, AXIS2_HASH_KEY_STRING, ns);
    }

    axis2_hash_set(ht_sch_ns, prefix, AXIS2_HASH_KEY_STRING, ns);

    return AXIS2_SUCCESS;
}

static xml_schema_simple_type_t*
handle_simple_type(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *simple_node,
    axiom_node_t *sch_node)
{
    void *sim_type = NULL;
    xml_schema_builder_impl_t *builder_impl = NULL;
    axis2_char_t *attr_value = NULL;
    axiom_element_t *sim_ele = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    axiom_element_t *union_ele = NULL;
    axiom_node_t *union_node = NULL;

    axiom_element_t *list_ele = NULL;
    axiom_node_t    *list_node = NULL;

    axiom_element_t *restriction_ele = NULL;
    axiom_node_t *restriction_node = NULL;


    AXIS2_PARAM_CHECK(env->error, simple_node, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    sim_type = xml_schema_simple_type_create(env, builder_impl->schema);

    sim_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(simple_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(sim_ele, env, "name");

    if (attr_value)
    {
        XML_SCHEMA_TYPE_SET_NAME(sim_type, env, attr_value);
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(sim_ele, env, "final");

    if (attr_value)
    {
        void *drv_method = NULL;
        if (AXIS2_STRCASECMP(attr_value, "all") == 0 ||
                AXIS2_STRCASECMP(attr_value, "#all") == 0)
        {
            drv_method = xml_schema_derivation_method_create(env, XML_SCHEMA_CONST_ALL);
        }
        else
        {
            drv_method = xml_schema_derivation_method_create(env, attr_value);
        }
        attr_value = NULL;
    }

    ele1 = axiom_util_get_first_child_element_with_uri_localname(sim_ele, env,
            simple_node, "annotation", XML_SCHEMA_NS, &node1);

    if (ele1)
    {
        void *annotation = NULL;
        annotation = handle_annotation_with_element(builder, env, node1);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sim_type, env, annotation);
    }

    restriction_ele = axiom_util_get_first_child_element_with_uri_localname(sim_ele,
            env, simple_node, "restriction", XML_SCHEMA_NS, &restriction_node);

    if (restriction_ele)
    {
        void *restriction = NULL;
        axiom_element_t *rest_ann_ele = NULL;
        axiom_node_t *rest_ann_node   = NULL;

        axiom_element_t *inline_sim_ele = NULL;
        axiom_node_t   *inline_sim_node = NULL;

        axiom_element_t *ele1         = NULL;
        axiom_node_t    *node1        = NULL;

        axis2_char_t *attribute_value   = NULL;

        restriction = xml_schema_simple_type_restriction_create(env);

        rest_ann_ele = axiom_util_get_first_child_element_with_uri_localname(restriction_ele,
                env, restriction_node, "annotation", XML_SCHEMA_NS, &rest_ann_node);

        if (rest_ann_ele)
        {
            void *rest_annotation = NULL;
            rest_annotation = handle_annotation_with_element(builder, env, rest_ann_node);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(rest_annotation, env, rest_annotation);
        }

        inline_sim_ele = axiom_util_get_first_child_element_with_uri_localname(restriction_ele,
                env, restriction_node, "simpleType", XML_SCHEMA_NS, &inline_sim_node);

        attribute_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(restriction_ele, env,
                "base");

        if (attribute_value)
        {
            axis2_array_list_t *temp_list = NULL;
            axis2_hash_t *ht_ns           = NULL;
            axis2_char_t *namesp          = NULL;
            axis2_array_list_t *last_list = NULL;
            axis2_char_t *name            = NULL;
            axis2_qname_t *qn              = NULL;

            temp_list = axis2_tokenize(env, attribute_value, ':');

            if (temp_list && AXIS2_ARRAY_LIST_SIZE(temp_list, env) >= 1)
            {
                namesp = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(temp_list, env, 0);
            }

            ht_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);
            namesp = axis2_hash_get(ht_ns, namesp, AXIS2_HASH_KEY_STRING);

            last_list = axis2_last_token(env, attribute_value, ':');

            name = AXIS2_ARRAY_LIST_GET(last_list, env, ':');

            qn = axis2_qname_create(env, name, namesp, NULL);

            XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_SET_BASE_TYPE_NAME(restriction, env, qn);
        }
        else if (inline_sim_ele)
        {
            void *base_type = NULL;
            base_type = handle_simple_type(builder , env, inline_sim_node, builder_impl->schema);
            XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_SET_BASE_TYPE(restriction, env, base_type);
        }


        ele1 = axiom_util_get_first_child_element_with_uri(restriction_node, env,
                XML_SCHEMA_NS, &node1);

        while (ele1)
        {
            axis2_char_t *localname = NULL;
            void        *annotation = NULL;
            xml_schema_obj_collection_t *facets = NULL;

            localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

            if (localname &&
                    AXIS2_STRCMP(localname, "annotation") != 0 &&
                    AXIS2_STRCMP(localname, "simpleType") != 0)
            {
                void *facet = NULL;

                facet = xml_schema_facet_construct(env, node1);

                annotation = axiom_util_get_first_child_element_with_uri_localname(ele1,
                        env, node1, "annotation", XML_SCHEMA_NS, &node1);


                if (annotation)
                {
                    void *facet_annotation = NULL;
                    facet_annotation = handle_annotation_with_element(builder, env, node1);
                    XML_SCHEMA_ANNOTATED_SET_ANNOTATION(facet, env, facet_annotation);
                }
                facets = XML_SCHEMA_SIMPLE_TYPE_RESTRICTION_GET_FACETS(restriction, env);
                XML_SCHEMA_OBJ_COLLECTION_ADD(facets, env, facet);
            }
            ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                    XML_SCHEMA_NS, &node1);
        }
        XML_SCHEMA_SIMPLE_TYPE_SET_CONTENT(sim_type, env, restriction);
    }
    else if ((list_ele = axiom_util_get_first_child_element_with_uri_localname(sim_ele,
            env, simple_node, "list", XML_SCHEMA_NS, &list_node)))
    {
        void *list = NULL;

        axiom_element_t *inline_list_ele = NULL;
        axiom_element_t *list_ann_ele = NULL;
        axiom_node_t *list_ann_node   = NULL;

        list = xml_schema_simple_type_list_create(env);

        attr_value = NULL;

        attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(list_ele, env, "itemType");

        if (attr_value)
        {
            axis2_array_list_t *namespaces_form_ele = NULL;
            axis2_array_list_t *last_list = NULL;
            axis2_char_t *name = NULL;
            axis2_qname_t *item_type_qn = NULL;
            axis2_char_t *ns = NULL;
            axis2_char_t *key = NULL;
            axis2_hash_t *ht_ns = NULL;
            namespaces_form_ele = axis2_tokenize(env, attr_value, ':');
            ht_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

            if (AXIS2_ARRAY_LIST_SIZE(namespaces_form_ele, env) > 1)
            {
                axis2_char_t *result = NULL;
                key = AXIS2_ARRAY_LIST_GET(namespaces_form_ele, env, 0);
                result = axis2_hash_get(ht_ns, key, AXIS2_HASH_KEY_STRING);
                if (NULL == result)
                {
                    /**TODO set error */
                    return NULL;
                }
                ns = (axis2_char_t*)result;
            }
            else
            {
                ns = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
            }

            last_list = axis2_last_token(env, attr_value, ':');

            name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);

            item_type_qn = axis2_qname_create(env, name, ns, NULL);

            XML_SCHEMA_SIMPLE_TYPE_LIST_SET_ITEM_TYPE_NAME(list, env, item_type_qn);
        }
        else if ((inline_list_ele =
                    axiom_util_get_first_child_element_with_uri_localname(list_ele, env,
                            list_node, "simpleType" , XML_SCHEMA_NS, &list_node)))
        {
            void *base_type = NULL;
            base_type = handle_simple_type(builder, env, list_node, sch_node);
            XML_SCHEMA_SIMPLE_TYPE_LIST_SET_ITEM_TYPE(list, env, base_type);
        }

        list_ann_ele = axiom_util_get_first_child_element_with_uri_localname(
                    list_ele, env, list_node, "annotation", XML_SCHEMA_NS, &list_node);

        if (list_ann_ele)
        {
            void *list_ann = NULL;
            list_ann = handle_annotation_with_element(builder, env, list_ann_node);

            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(list, env, list_ann);
        }
        XML_SCHEMA_SIMPLE_TYPE_SET_CONTENT(sim_type, env, list);
    }
    else if ((union_ele =
                axiom_util_get_first_child_element_with_uri_localname(union_ele, env, union_node,
                        "union", XML_SCHEMA_NS, &union_node)))
    {
        void *sch_union = NULL;

        axis2_char_t *attr_value = NULL;

        axiom_element_t *inline_uni_ele = NULL;
        axiom_node_t   *inline_uni_node = NULL;

        axiom_element_t *union_ann_ele = NULL;
        axiom_node_t *union_ann_node = NULL;

        sch_union = xml_schema_unique_create(env);

        attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(union_ele,
                env, "memberTypes");

        if (attr_value)
        {
            axis2_array_list_t *tokens = NULL;
            axis2_array_list_t *v = NULL;
            axis2_hash_t *ht_namespaces = NULL;
            int i = 0;
            XML_SCHEMA_SIMPLE_TYPE_UNION_SET_MEMBER_TYPES_SOURCE(sch_union,
                    env, attr_value);

            ht_namespaces = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(
                        builder_impl->schema, env);
            tokens = axis2_tokenize(env, attr_value, ' ');
            v = axis2_array_list_create(env, 10);

            for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(v, env); i++)
            {
                axis2_char_t *localname = NULL;
                axis2_char_t *prefix = NULL;
                axis2_char_t *uri = NULL;
                axis2_char_t *member = NULL;
                axis2_char_t *mem_dup = NULL;
                axis2_char_t *index = NULL;
                axis2_qname_t *qn = NULL;
                member = AXIS2_ARRAY_LIST_GET(v, env, i);
                mem_dup = AXIS2_STRDUP(member, env);

                index = strchr(mem_dup, ':');
                if (index)
                {
                    localname = index + 1;
                    mem_dup[index - mem_dup] = '\0';
                    prefix = mem_dup;
                }
                else
                {
                    localname = mem_dup;
                }
                uri = axis2_hash_get(ht_namespaces, prefix, AXIS2_HASH_KEY_STRING);
                qn = axis2_qname_create(env, localname, uri, prefix);
                /*
                AXIS2_FREE(env->allocator, mem_dup);
                AXIS2_FFRE(env->allocator, localname);
                */
                AXIS2_ARRAY_LIST_ADD(v, env, qn);
            }
            XML_SCHEMA_SIMPLE_TYPE_UNION_SET_MEMBER_TYPES_QNAMES(sch_union, env, v);
        }

        inline_uni_ele = axiom_util_get_first_child_element_with_uri_localname(
                    union_ele, env, union_node, "simpleType", XML_SCHEMA_NS, &inline_uni_node);

        while (inline_uni_ele)
        {
            xml_schema_obj_collection_t *base_types = NULL;
            void *union_sim_type = NULL;
            axis2_char_t *mem_types = NULL;
            axis2_char_t *sim_type_name = NULL;

            union_sim_type = handle_simple_type(builder, env, inline_uni_node, sch_node);

            base_types = XML_SCHEMA_SIMPLE_TYPE_UNION_GET_BASE_TYPES(sch_union, env);

            XML_SCHEMA_OBJ_COLLECTION_ADD(base_types, env, union_sim_type);

            sim_type_name = XML_SCHEMA_TYPE_GET_NAME(union_sim_type, env);

            mem_types = AXIS2_MALLOC(env->allocator, sizeof(strlen(sim_type_name) + 3));

            sprintf(mem_types, " %s", sim_type_name);

            XML_SCHEMA_SIMPLE_TYPE_UNION_SET_MEMBER_TYPES_SOURCE(union_sim_type,

                    env, mem_types);
            AXIS2_FREE(env->allocator, mem_types);

            inline_uni_ele = axiom_util_get_next_sibling_element_with_uri_localname(

                        union_ele, env, union_node, "simpleType", XML_SCHEMA_NS, &inline_uni_node);
        }

        union_ann_ele = axiom_util_get_first_child_element_with_uri_localname(union_ele,
                env, union_node, "annotation", XML_SCHEMA_NS, &union_ann_node);

        if (union_ann_ele)
        {
            void *union_annotation = NULL;
            union_annotation = handle_annotation_with_element(builder, env, union_ann_node);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sch_union, env, union_annotation);
        }

        XML_SCHEMA_SIMPLE_TYPE_SET_CONTENT(sim_type, env, sch_union);
    }
    return sim_type;
}

static xml_schema_complex_type_t*
handle_complex_type(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *complex_node,
    axiom_node_t *schema_node)
{
    void *cmp_type = NULL;

    axiom_element_t *cmp_ele = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t *node1   = NULL;
    axis2_char_t *attr_value = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    cmp_type = xml_schema_complex_type_create(env, builder_impl->schema);

    cmp_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(complex_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cmp_ele, env, "name");

    if (attr_value)
    {
        XML_SCHEMA_TYPE_SET_NAME(cmp_type, env, attr_value);
        attr_value = NULL;
    }

    ele1 = axiom_util_get_first_child_element_with_uri(complex_node , env,
            XML_SCHEMA_NS, &node1);
    while (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *sequence = NULL;

            sequence = handle_sequence(builder, env,
                    node1, schema_node);
            XML_SCHEMA_COMPLEX_TYPE_SET_PARTICLE(cmp_type, env, sequence);

        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice = NULL;

            choice = handle_choice(builder, env,
                    node1, schema_node);

            XML_SCHEMA_COMPLEX_TYPE_SET_PARTICLE(cmp_type, env, choice);
        }
        else if (AXIS2_STRCMP(localname, "all") == 0)
        {
            void *all = NULL;
            all = handle_all(builder, env, node1, schema_node);

            XML_SCHEMA_COMPLEX_TYPE_SET_PARTICLE(cmp_type, env, all);
        }
        else if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attribute = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attribute = handle_attribute(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_COMPLEX_TYPE_GET_ATTRIBUTES(cmp_type, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attribute);
        }
        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attr_grp = handle_attribute_group(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_COMPLEX_TYPE_GET_ATTRIBUTES(cmp_type, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr_grp);
        }
        else if (AXIS2_STRCMP(localname, "group") == 0)
        {
            void *grp = NULL;
            void *grp_particle = NULL;
            grp = handle_group(builder, env, node1, schema_node);
            grp_particle = XML_SCHEMA_GROUP_GET_PARTICLE(grp, env);
            if (NULL == grp_particle)
            {
                XML_SCHEMA_COMPLEX_TYPE_SET_PARTICLE(cmp_type, env, grp);
            }
            else
            {
                XML_SCHEMA_COMPLEX_TYPE_SET_PARTICLE(cmp_type, env, grp_particle);
            }
        }
        else if (AXIS2_STRCMP(localname, "simpleContent") == 0)
        {
            void *sim_cnt = NULL;
            sim_cnt = handle_simple_content(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_TYPE_SET_CONTENT_MODEL(cmp_type, env, sim_cnt);
        }
        else if (AXIS2_STRCMP(localname, "complexContent") == 0)
        {
            void *cmp_cnt = NULL;
            cmp_cnt = handle_complex_content(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_TYPE_SET_CONTENT_MODEL(cmp_type, env, cmp_cnt);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(cmp_type, env, annotation);
        }
        else if (AXIS2_STRCMP(localname, "anyAttribute") == 0)
        {
            void *any_attr = NULL;
            any_attr = handle_any_attribute(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_TYPE_SET_ANY_ATTRIBUTE(cmp_type, env, any_attr);
        }
        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }


    attr_value = NULL;

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cmp_ele, env, "block");
    if (attr_value)
    {
        void *drv_method = NULL;
        if (AXIS2_STRCASECMP(attr_value, "all") == 0 ||
                AXIS2_STRCASECMP(attr_value, "#all") == 0)
        {
            drv_method = xml_schema_derivation_method_create(env,
                    XML_SCHEMA_CONST_ALL);
            XML_SCHEMA_COMPLEX_TYPE_SET_BLOCK(cmp_type, env, drv_method);
        }
        else
        {
            drv_method = xml_schema_derivation_method_create(env,
                    attr_value);
            XML_SCHEMA_COMPLEX_TYPE_SET_BLOCK(cmp_type, env, drv_method);
        }
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cmp_ele, env, "final");
    if (attr_value)
    {
        void *drv_method = NULL;
        if (AXIS2_STRCASECMP(attr_value, "all") == 0 ||
                AXIS2_STRCASECMP(attr_value, "#all"))
        {
            drv_method = xml_schema_derivation_method_create(env,
                    XML_SCHEMA_CONST_ALL);
            XML_SCHEMA_COMPLEX_TYPE_SET_BLOCK(cmp_type, env, drv_method);
        }
        else
        {
            drv_method = xml_schema_derivation_method_create(env,
                    attr_value);
            XML_SCHEMA_COMPLEX_TYPE_SET_BLOCK(cmp_type, env, drv_method);
        }
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cmp_ele, env, "abstract");
    if (attr_value)
    {
        if (AXIS2_STRCASECMP(attr_value, "true") == 0)
        {
            XML_SCHEMA_COMPLEX_TYPE_SET_ABSTRACT(cmp_type, env, AXIS2_TRUE);
        }
        else
        {
            XML_SCHEMA_COMPLEX_TYPE_SET_ABSTRACT(cmp_type, env, AXIS2_FALSE);
        }
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cmp_ele, env, "mixed");
    if (attr_value)
    {
        if (AXIS2_STRCASECMP(attr_value, "true") == 0)
        {
            XML_SCHEMA_COMPLEX_TYPE_SET_MIXED(cmp_type, env, AXIS2_TRUE);
        }
        else
        {
            XML_SCHEMA_COMPLEX_TYPE_SET_MIXED(cmp_type, env, AXIS2_FALSE);
        }
    }
    return cmp_type;
}

static xml_schema_simple_content_t*
handle_simple_content(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *simple_node,
    axiom_node_t *schema_node)
{

    void *sim_cnt = NULL;
    axiom_element_t *sim_cnt_ele = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    sim_cnt = xml_schema_simple_content_create(env);

    sim_cnt_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(simple_node, env);

    ele1 = axiom_util_get_first_child_element_with_uri(simple_node , env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "restriction") == 0)
        {
            void *sim_res = NULL;

            sim_res = handle_simple_content_restriction(builder, env,
                    node1, schema_node);

            XML_SCHEMA_SIMPLE_CONTENT_SET_CONTENT(sim_cnt, env, sim_res);
        }
        else if (AXIS2_STRCMP(localname, "extension") == 0)
        {
            void *sim_ext = NULL;

            sim_ext = handle_simple_content_extension(builder, env,
                    node1, schema_node);

            XML_SCHEMA_SIMPLE_CONTENT_SET_CONTENT(sim_cnt, env, sim_ext);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sim_cnt, env, annotation);
        }

        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return sim_cnt;
}

static xml_schema_complex_content_t*
handle_complex_content(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *complex_node,
    axiom_node_t *schema_node)
{
    void *cmp_cnt = NULL;
    axiom_element_t *cmp_cnt_ele = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    cmp_cnt = xml_schema_complex_content_create(env);

    cmp_cnt_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(complex_node, env);

    ele1 = axiom_util_get_first_child_element_with_uri(complex_node , env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "restriction") == 0)
        {
            void *cmp_res = NULL;

            cmp_res = handle_complex_content_restriction(builder, env,
                    node1, schema_node);

            XML_SCHEMA_COMPLEX_CONTENT_SET_CONTENT(cmp_cnt, env, cmp_res);
        }
        else if (AXIS2_STRCMP(localname, "extension") == 0)
        {
            void *cmp_ext = NULL;

            cmp_ext = handle_complex_content_extension(builder, env,
                    node1, schema_node);

            XML_SCHEMA_COMPLEX_CONTENT_SET_CONTENT(cmp_cnt, env, cmp_ext);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(cmp_cnt, env, annotation);
        }

        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return cmp_cnt;
}

static xml_schema_simple_content_restriction_t*
handle_simple_content_restriction(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *res_node,
    axiom_node_t *schema_node)
{
    void *sim_cnt_res = NULL;

    axiom_element_t *res_ele = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    res_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(res_node, env);

    sim_cnt_res = xml_schema_simple_content_restriction_create(env);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(res_ele, env, "base");

    if (attr_value)
    {
        axis2_char_t *ns_from_ele = "";

        axis2_array_list_t *list = NULL;

        axis2_hash_t *namespaces = NULL;

        axis2_char_t *result = NULL;

        axis2_array_list_t* last_list = NULL;

        axis2_char_t *name = NULL;

        axis2_qname_t *qn = NULL;

        if (strchr(attr_value, ':'))
        {
            list = axis2_tokenize(env, attr_value, ':');
            ns_from_ele = AXIS2_ARRAY_LIST_GET(list, env, 0);
        }

        namespaces = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

        result = axis2_hash_get(namespaces, ns_from_ele, AXIS2_HASH_KEY_STRING);

        if (NULL == result)
        {
            /** TODO set error */
            return NULL;
        }

        last_list = axis2_last_token(env, attr_value, ':');
        if (last_list && AXIS2_ARRAY_LIST_SIZE(last_list, env) >= 2)
            name = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        qn = axis2_qname_create(env, name, result, NULL);

        XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_SET_BASE_TYPE_NAME(sim_cnt_res, env, qn);
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(res_ele, env, "id");
    if (attr_value)
    {
        XML_SCHEMA_ANNOTATED_SET_ID(sim_cnt_res, env, attr_value);
    }
    ele1 = axiom_util_get_first_child_element_with_uri(res_node, env, XML_SCHEMA_NS, &node1);

    if (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);
        if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attribute = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attribute = handle_attribute(builder, env, node1, schema_node);

            attributes = XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_GET_ATTRIBUTES(sim_cnt_res, env);

            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attribute);
        }
        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp_ref = NULL;
            xml_schema_obj_collection_t *attributes = NULL;

            attr_grp_ref = handle_attribute_group_ref(builder, env, node1, schema_node);

            attributes = XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_GET_ATTRIBUTES(sim_cnt_res, env);

            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr_grp_ref);
        }
        else if (AXIS2_STRCMP(localname, "simpleType") == 0)
        {
            void *simple_type = NULL;

            simple_type = handle_simple_type(builder, env, node1, schema_node);

            XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_SET_BASE_TYPE(sim_cnt_res, env, simple_type);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;

            annotation = handle_annotation_with_element(builder, env, node1);

            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sim_cnt_res, env, annotation);
        }
        else
        {
            void *facet = NULL;
            xml_schema_obj_collection_t *facets = NULL;
            axiom_element_t *child_ele = NULL;
            axiom_node_t *child_node   = NULL;
            facet = xml_schema_facet_construct(env, node1);

            child_ele = axiom_util_get_first_child_element_with_uri_localname(ele1, env,
                    node1, "annotation", XML_SCHEMA_NS, &child_node);
            while (child_ele)
            {
                void *annotation = NULL;

                annotation = handle_annotation_with_element(builder, env, child_node);

                XML_SCHEMA_ANNOTATED_SET_ANNOTATION(facet, env, annotation);

                child_ele = axiom_util_get_next_sibling_element_with_uri_localname(child_ele,
                        env, child_node, "annotation", XML_SCHEMA_NS, &child_node);
            }

            facets = XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION_GET_FACETS(sim_cnt_res, env);

            XML_SCHEMA_OBJ_COLLECTION_ADD(facets, env, facet);
        }
    }
    return sim_cnt_res;
}


static xml_schema_simple_content_extension_t*
handle_simple_content_extension(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *ext_node,
    axiom_node_t *schema_node)
{
    void *sim_cnt_ext = NULL;

    axiom_element_t *ext_ele = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    ext_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(ext_node, env);

    sim_cnt_ext = xml_schema_simple_content_extension_create(env);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ext_ele, env, "base");
    if (attr_value)
    {
        axis2_char_t *ns_from_ele = "";
        axis2_array_list_t *list = NULL;
        axis2_hash_t *namespaces = NULL;
        axis2_char_t *result = NULL;
        axis2_array_list_t* last_list = NULL;
        axis2_char_t *name = NULL;
        axis2_qname_t *qn = NULL;

        if (strchr(attr_value, ':'))
        {
            list = axis2_tokenize(env, attr_value, ':');
            ns_from_ele = AXIS2_ARRAY_LIST_GET(list, env, 0);
        }


        namespaces = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

        result = axis2_hash_get(namespaces, ns_from_ele, AXIS2_HASH_KEY_STRING);

        if (NULL == result)
        {
            /** TODO set error */
            return NULL;
        }

        last_list = axis2_last_token(env, attr_value, ':');
        if (last_list && AXIS2_ARRAY_LIST_SIZE(last_list, env) >= 2)
            name = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        qn = axis2_qname_create(env, name, result, NULL);

        XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_SET_BASE_TYPE_NAME(sim_cnt_ext, env, qn);

    }

    ele1 = axiom_util_get_first_child_element_with_uri(ext_node, env, XML_SCHEMA_NS, &node1);

    if (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attribute = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attribute = handle_attribute(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_ATTRIBUTES(sim_cnt_ext, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attribute);
        }

        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp_ref = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attr_grp_ref = handle_attribute_group_ref(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_GET_ATTRIBUTES(sim_cnt_ext, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr_grp_ref);
        }
        else if (AXIS2_STRCMP(localname, "anyAttribute") == 0)
        {
            void *any_attr = NULL;
            any_attr = handle_any_attribute(builder, env, node1, schema_node);
            XML_SCHEMA_SIMPLE_CONTENT_EXTENSION_SET_ANY_ATTRIBUTE(sim_cnt_ext, env, any_attr);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sim_cnt_ext, env, annotation);
        }
    }
    return sim_cnt_ext;
}

static xml_schema_complex_content_restriction_t*
handle_complex_content_restriction(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *res_node,
    axiom_node_t *schema_node)
{
    void *cmp_cnt_res = NULL;

    axiom_element_t *res_ele = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    res_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(res_node, env);

    cmp_cnt_res = xml_schema_complex_content_restriction_create(env);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(res_ele, env, "base");
    if (attr_value)
    {
        axis2_char_t *prefix = "";
        axis2_array_list_t *list = NULL;
        axis2_hash_t *namespaces = NULL;
        axis2_char_t *result = NULL;
        axis2_array_list_t* last_list = NULL;
        axis2_char_t *name = NULL;
        axis2_qname_t *qn = NULL;

        if (strchr(attr_value, ':'))
        {
            list = axis2_tokenize(env, attr_value, ':');
            prefix = AXIS2_ARRAY_LIST_GET(list, env, 0);
        }

        namespaces = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

        result = axis2_hash_get(namespaces, prefix, AXIS2_HASH_KEY_STRING);

        if (NULL == result)
        {
            /** TODO set error */
            return NULL;
        }

        last_list = axis2_last_token(env, attr_value, ':');
        if (last_list && AXIS2_ARRAY_LIST_SIZE(last_list, env) >= 2)
            name = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        qn = axis2_qname_create(env, name, result, NULL);

        XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_BASE_TYPE_NAME(cmp_cnt_res, env, qn);

    }

    ele1 = axiom_util_get_first_child_element_with_uri(res_node, env, XML_SCHEMA_NS, &node1);

    if (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);
        if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *sequence = NULL;
            sequence = handle_sequence(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_PARTICLE(cmp_cnt_res, env, sequence);
        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice = NULL;
            choice = handle_choice(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_PARTICLE(cmp_cnt_res, env, choice);
        }
        else if (AXIS2_STRCMP(localname, "all") == 0)
        {
            void *all = NULL;
            all = handle_all(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_PARTICLE(cmp_cnt_res, env, all);
        }
        if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attribute = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attribute = handle_attribute(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_ATTRIBUTES(cmp_cnt_res, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attribute);
        }

        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp_ref = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attr_grp_ref = handle_attribute_group_ref(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_GET_ATTRIBUTES(cmp_cnt_res, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr_grp_ref);
        }
        else if (AXIS2_STRCMP(localname, "anyAttribute") == 0)
        {
            void *any_attr = NULL;
            any_attr = handle_any_attribute(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION_SET_ANY_ATTRIBUTE(cmp_cnt_res, env, any_attr);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(cmp_cnt_res, env, annotation);
        }
    }
    return cmp_cnt_res;
}

static xml_schema_complex_content_extension_t*
handle_complex_content_extension(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ext_node,
    axiom_node_t *schema_node)
{
    void *cmp_cnt_ext = NULL;

    axiom_element_t *ext_ele = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    ext_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(ext_node, env);

    cmp_cnt_ext = xml_schema_complex_content_extension_create(env);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ext_ele, env, "base");
    if (attr_value)
    {
        axis2_char_t *ns_from_ele = "";
        axis2_array_list_t *list = NULL;
        axis2_hash_t *namespaces = NULL;
        axis2_char_t *result = NULL;
        axis2_array_list_t* last_list = NULL;
        axis2_char_t *name = NULL;
        axis2_qname_t *qn = NULL;

        if (strchr(attr_value, ':'))
        {
            list = axis2_tokenize(env, attr_value, ':');
            ns_from_ele = AXIS2_ARRAY_LIST_GET(list, env, 0);
        }


        namespaces = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

        result = axis2_hash_get(namespaces, ns_from_ele, AXIS2_HASH_KEY_STRING);

        if (NULL == result)
        {
            /** TODO set error */
            return NULL;
        }

        last_list = axis2_last_token(env, attr_value, ':');
        if (last_list && AXIS2_ARRAY_LIST_SIZE(last_list, env) >= 2)
            name = (axis2_char_t *)AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        qn = axis2_qname_create(env, name, result, NULL);

        XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_BASE_TYPE_NAME(cmp_cnt_ext, env, qn);

    }

    ele1 = axiom_util_get_first_child_element_with_uri(ext_node, env, XML_SCHEMA_NS, &node1);

    if (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *sequence = NULL;
            sequence = handle_sequence(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_PARTICLE(cmp_cnt_ext, env, sequence);
        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice = NULL;
            choice = handle_choice(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_PARTICLE(cmp_cnt_ext, env, choice);
        }
        else if (AXIS2_STRCMP(localname, "all") == 0)
        {
            void *all = NULL;
            all = handle_all(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_PARTICLE(cmp_cnt_ext, env, all);
        }
        else if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attribute = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attribute = handle_attribute(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_ATTRIBUTES(cmp_cnt_ext, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attribute);
        }

        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp_ref = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attr_grp_ref = handle_attribute_group_ref(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_GET_ATTRIBUTES(cmp_cnt_ext, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr_grp_ref);
        }
        else if (AXIS2_STRCMP(localname, "anyAttribute") == 0)
        {
            void *any_attr = NULL;
            any_attr = handle_any_attribute(builder, env, node1, schema_node);
            XML_SCHEMA_COMPLEX_CONTENT_EXTENSION_SET_ANY_ATTRIBUTE(cmp_cnt_ext, env, any_attr);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(cmp_cnt_ext, env, annotation);
        }
    }
    return cmp_cnt_ext;
}

static xml_schema_attribute_group_ref_t*
handle_attribute_group_ref(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *attr_grp_node,
    axiom_node_t *schema_node)
{
    void *attr_grp_ref = NULL;

    axiom_element_t *ann_ele = NULL;
    axiom_node_t *ann_node   = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *attr_grp_ele = NULL;

    attr_grp_ref = xml_schema_attribute_group_ref_create(env);

    attr_grp_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(attr_grp_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_grp_ele,
            env, "ref");

    if (attr_value)
    {
        axis2_array_list_t *parts = NULL;
        axis2_char_t *prefix = NULL;
        axis2_hash_t *ht_ns  = NULL;
        axis2_char_t *uri = NULL;
        axis2_char_t *ref = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_qname_t *ref_qname = NULL;
        parts = axis2_tokenize(env, attr_value, ':');

        if (parts && AXIS2_ARRAY_LIST_SIZE(parts, env) > 1)
        {
            prefix = AXIS2_ARRAY_LIST_GET(parts, env, 0);
            ht_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(
                        AXIS2_INTF_TO_IMPL(builder)->schema, env);

            uri = axis2_hash_get(ht_ns, prefix, AXIS2_HASH_KEY_STRING);
            if (NULL == uri)
            {
                /** TODO set error code */
                return NULL;
            }
        }

        last_list = axis2_last_token(env, attr_value, ':');
        ref = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        /** check this */
        ref_qname = axis2_qname_create(env, ref, uri, NULL);
        XML_SCHEMA_ATTRIBUTE_GROUP_REF_SET_REF_QNAME(attr_grp_ref, env, ref_qname);
    }

    attr_value = NULL;
    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_grp_ele, env, "id");

    if (attr_value)
        XML_SCHEMA_ANNOTATED_SET_ID(attr_grp_ref, env, attr_value);

    ann_ele = axiom_util_get_first_child_element_with_uri_localname(attr_grp_ele, env,
            attr_grp_node, "annotation", XML_SCHEMA_NS, &ann_node);

    if (ann_ele)
    {
        void *annotation = handle_annotation_with_element(builder, env, ann_node);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(attr_grp_ref, env, annotation);
    }
    return attr_grp_ref;
}

static xml_schema_group_base_t*
handle_sequence(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *seq_node,
    axiom_node_t *schema_node)
{
    void *sequence = NULL;
    axiom_element_t *ele1 = NULL;
    axiom_node_t    *node1 = NULL;
    xml_schema_obj_collection_t *items = NULL;

    sequence = xml_schema_sequence_create(env);

    ele1 = axiom_util_get_first_child_element_with_uri(seq_node, env,
            XML_SCHEMA_NS, &node1);

    items = XML_SCHEMA_GROUP_BASE_GET_ITEMS(sequence, env);
    while (ele1)
    {
        axis2_char_t *localname = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *seq = NULL;
            seq = handle_sequence(builder, env, node1, schema_node);
            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, seq);
        }
        else if (AXIS2_STRCMP(localname, "element") == 0)
        {
            void *element = NULL;
            element = handle_element(builder, env,
                    node1, schema_node, AXIS2_FALSE);
            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env,  element);
        }
        else if (AXIS2_STRCMP(localname, "group") == 0)
        {
            void *group = NULL;
            group = handle_group_ref(builder, env, node1, schema_node);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, group);
        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice_item = NULL;
            choice_item = handle_choice(builder, env, node1, schema_node);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, choice_item);
        }
        else if (AXIS2_STRCMP(localname, "any") == 0)
        {
            void *any = NULL;
            any = handle_any(builder, env, node1, schema_node);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, any);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation(builder, env, node1, schema_node);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sequence, env, annotation);
        }

        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return sequence;
}

static xml_schema_any_t*
handle_any(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *any_node,
    axiom_node_t *schema_node)
{
    axiom_element_t *any_ele = NULL;
    void *any                   = NULL;
    axis2_char_t *attr_value    = NULL;
    axis2_char_t *process_cnt   = NULL;
    void *pro_content           = NULL;

    axiom_element_t *annotation_ele = NULL;
    axiom_node_t    *annotation_node = NULL;
    long min_occurs = 0;
    long max_occurs = 0;


    any = xml_schema_any_create(env);
    any_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(any_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(any_ele, env,
            "namespace");

    if (attr_value)
        XML_SCHEMA_ANY_SET_NAMESPACE(any, env, attr_value);

    process_cnt = get_enum_string(builder, env, any_ele, "processContents");

    if (process_cnt)
    {
        pro_content = xml_schema_content_processing_create(env, process_cnt);
        XML_SCHEMA_ANY_SET_PROCESS_CONTENT(any, env, pro_content);
    }

    annotation_ele = axiom_util_get_first_child_element_with_uri_localname(any_ele, env,
            any_node, "annotation", XML_SCHEMA_NS, &annotation_node);

    if (annotation_ele)
    {
        void *annotation = NULL;
        annotation = handle_annotation_with_element(builder, env, annotation_node);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(any, env, annotation);
    }

    min_occurs = get_min_occurs(env, any_node);
    max_occurs = get_max_occurs(env, any_node);

    XML_SCHEMA_PARTICLE_SET_MAX_OCCURS(any, env, max_occurs);
    XML_SCHEMA_PARTICLE_SET_MIN_OCCURS(any, env, min_occurs);
    return any;
}

static xml_schema_group_base_t*
handle_choice(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *choice_node,
    axiom_node_t *schema_node)
{
    void *choice = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *choice_ele = NULL;
    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;
    xml_schema_obj_collection_t *items = NULL;
    AXIS2_PARAM_CHECK(env->error, choice_node, NULL);

    AXIS2_PARAM_CHECK(env->error, schema_node, NULL);

    choice = xml_schema_choice_create(env);

    choice_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(choice_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(choice_ele, env, "id");

    if (attr_value)
        XML_SCHEMA_ANNOTATED_SET_ID(choice, env, attr_value);

    ele1 = axiom_util_get_first_child_element_with_uri(choice_node, env,
            XML_SCHEMA_NS, &node1);

    items = XML_SCHEMA_GROUP_BASE_GET_ITEMS(choice, env);

    while (ele1)
    {
        axis2_char_t *localname = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);
        if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *seq = NULL;
            seq = handle_sequence(builder, env, node1, schema_node);
            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, seq);
        }
        else if (AXIS2_STRCMP(localname, "element") == 0)
        {
            void *element = NULL;
            element = handle_element(builder, env,
                    node1, schema_node, AXIS2_FALSE);
            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env,  element);
        }
        else if (AXIS2_STRCMP(localname, "group") == 0)
        {
            void *group = NULL;
            group = handle_group_ref(builder, env, node1, schema_node);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, group);
        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice_item = NULL;
            choice_item = handle_choice(builder, env, node1, schema_node);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, choice_item);
        }
        else if (AXIS2_STRCMP(localname, "any") == 0)
        {
            void *any = NULL;
            any = handle_any(builder, env, node1, schema_node);

            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, any);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation(builder, env, node1, schema_node);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(choice, env, annotation);
        }

        ele1 = axiom_util_get_first_child_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return choice;
}

static xml_schema_group_base_t*
handle_all(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *all_node,
    axiom_node_t *schema_node)
{
    void *all = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t *node1   = NULL;

    all = xml_schema_all_create(env);

    ele1 = axiom_util_get_first_child_element_with_uri(all_node, env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "element") == 0)
        {
            void *element = NULL;
            xml_schema_obj_collection_t *items = NULL;
            element = handle_element(builder, env, node1,
                    schema_node, AXIS2_FALSE);
            items = XML_SCHEMA_GROUP_BASE_GET_ITEMS(all, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, element);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(all, env, annotation);
        }
        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return all;
}

static xml_schema_group_t*
handle_group(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *group_node,
    axiom_node_t *schema_node)
{
    axiom_element_t *grp_ele = NULL;
    void *grp = NULL;
    axis2_char_t *grp_name = NULL;

    axiom_node_t *node1 = NULL;
    axiom_element_t *ele1 = NULL;


    grp = xml_schema_group_create(env);

    grp_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                group_node, env);

    grp_name = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(grp_ele, env,
            "name");

    XML_SCHEMA_GROUP_SET_NAME(grp, env, grp_name);

    grp_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(group_node, env);

    ele1 = axiom_util_get_first_child_element_with_uri(group_node, env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        axis2_char_t *localname = NULL;
        void *particle = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);
        if (AXIS2_STRCMP(localname, "all") == 0)
        {
            particle = handle_all(builder, env, node1, schema_node);
            XML_SCHEMA_GROUP_SET_PARTICLE(grp, env, particle);
        }
        else if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            particle = handle_sequence(builder, env, node1, schema_node);
            XML_SCHEMA_GROUP_SET_PARTICLE(grp, env, particle);
        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            particle = handle_choice(builder, env, node1, schema_node);
            XML_SCHEMA_GROUP_SET_PARTICLE(grp, env, particle);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *grp_annotation = NULL;
            grp_annotation =
                handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(grp, env, grp_annotation);
        }

        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return grp;
}

static xml_schema_attribute_group_t*
handle_attribute_group(
    xml_schema_builder_t* builder,
    const axis2_env_t *env,
    axiom_node_t *attr_grp_node,
    axiom_node_t *schema_node)
{
    void *attr_grp = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *attr_grp_ele = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t *node1   = NULL;

    attr_grp = xml_schema_any_attribute_create(env);
    attr_grp_ele = AXIOM_NODE_GET_DATA_ELEMENT(attr_grp_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_grp_ele, env, "name");
    if (attr_value)
    {
        XML_SCHEMA_ATTRIBUTE_GROUP_SET_NAME(attr_grp, env, attr_value);
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_grp_ele, env, "id");
    if (attr_value)
    {
        XML_SCHEMA_ANNOTATED_SET_ID(attr_grp, env, attr_value);
    }

    ele1 = axiom_util_get_first_child_element_with_uri(node1, env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "attribute") == 0)
        {
            void *attr = NULL;
            xml_schema_obj_collection_t *attributes = NULL;

            attr = handle_attribute(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_ATTRIBUTE_GROUP_GET_ATTRIBUTES(attr_grp, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr);
        }
        else if (AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp_ref = NULL;
            xml_schema_obj_collection_t *attributes = NULL;
            attr_grp_ref = handle_attribute_group_ref(builder, env, node1, schema_node);
            attributes = XML_SCHEMA_ATTRIBUTE_GROUP_GET_ATTRIBUTES(attr_grp, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(attributes, env, attr_grp_ref);
        }
        else if (AXIS2_STRCMP(localname, "anyAttribute") == 0)
        {
            void *any_attr = NULL;
            any_attr = handle_any_attribute(builder, env, node1, schema_node);
            XML_SCHEMA_ATTRIBUTE_GROUP_SET_ANY_ATTRIBUTE(attr_grp, env, any_attr);
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(attr_grp, env, annotation);
        }

        ele1 = axiom_util_get_first_child_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return attr_grp;
}

static xml_schema_any_attribute_t*
handle_any_attribute(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *any_attr_node,
    axiom_node_t *schema_node)
{
    void *any_attr = NULL;
    axiom_element_t *any_attr_ele = NULL;
    axis2_char_t *attr_value = NULL;

    axiom_element_t *ann_ele = NULL;
    axiom_node_t *ann_node   = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    any_attr = xml_schema_any_attribute_create(env);

    any_attr_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(any_attr_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(any_attr_ele, env, "namespace");

    if (attr_value)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_SET_NAMESPACE(any_attr, env, attr_value);
        attr_value = NULL;

    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(any_attr_ele, env, "processContents");

    if (attr_value)
    {
        axis2_char_t* content_processing  =  NULL;
        void *cnt_processnig = NULL;
        content_processing = get_enum_string(builder, env, any_attr_ele, "processContents");
        cnt_processnig = xml_schema_content_processing_create(env, content_processing);
        XML_SCHEMA_ANY_ATTRIBUTE_SET_PROCESS_CONTENT(any_attr, env, cnt_processnig);
        attr_value = NULL;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(any_attr_ele, env, "id");

    if (attr_value)
    {
        XML_SCHEMA_ANNOTATED_SET_ID(any_attr, env, attr_value);
        attr_value = NULL;
    }

    ann_ele = axiom_util_get_first_child_element_with_uri_localname(any_attr_ele, env,
            any_attr_node, "annotation", XML_SCHEMA_NS, &ann_node);

    if (ann_ele)
    {
        void *annotation = NULL;
        annotation = handle_annotation_with_element(builder, env, any_attr_node);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(any_attr, env, annotation);
    }
    return any_attr;
}

static xml_schema_group_ref_t*
handle_group_ref(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *grp_ref_node,
    axiom_node_t *schema_node)
{
    void *group = NULL;

    axiom_element_t *grp_ref_ele = NULL;

    axis2_char_t *attr_value = NULL;

    axiom_element_t *ann_ele = NULL;
    axiom_node_t    *ann_node = NULL;

    axiom_element_t *ele1 = NULL;
    axiom_node_t   *node1 = NULL;

    axis2_hash_t *ht_ns = NULL;

    group = xml_schema_group_ref_create(env);

    grp_ref_ele = AXIOM_NODE_GET_DATA_ELEMENT(grp_ref_node, env);

    ann_ele = axiom_util_get_first_child_element_with_uri_localname(
                grp_ref_ele, env, grp_ref_node, "annotation", XML_SCHEMA_NS, &ann_node);

    if (ann_ele)
    {
        void *annotation = NULL;
        annotation = handle_annotation_with_element(builder, env, ann_node);

        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(group, env, annotation);
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ann_ele, env, "ref");

    if (attr_value)
    {
        axis2_array_list_t* parts = NULL;
        axis2_char_t *prefix = "";
        axis2_char_t *result  = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *ref = NULL;
        axis2_qname_t *ref_qname = NULL;

        parts = axis2_tokenize(env, attr_value, ':');
        ht_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(
                    AXIS2_INTF_TO_IMPL(builder)->schema, env);


        if (AXIS2_ARRAY_LIST_SIZE(parts, env) > 1)
            prefix = (axis2_char_t*)AXIS2_ARRAY_LIST_GET(parts, env, 1);
        result = axis2_hash_get(ht_ns, prefix, AXIS2_HASH_KEY_STRING);

        if (NULL == result)
        {
            /**TODO set error */
            return NULL;
        }
        last_list = axis2_last_token(env, attr_value, ':');
        ref = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        ref_qname = axis2_qname_create(env, ref, XML_SCHEMA_NS, NULL);
        return group;
    }

    ele1 = axiom_util_get_first_child_element_with_uri(grp_ref_node, env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        axis2_char_t *localname = NULL;

        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *seq = NULL;
            seq = handle_sequence(builder, env, node1, schema_node);
            XML_SCHEMA_GROUP_REF_SET_PARTICLE(group, env, seq);
        }
        else if (AXIS2_STRCMP(localname, "all") == 0)
        {
            void *all = NULL;
            all = handle_all(builder, env, node1, schema_node);
            XML_SCHEMA_GROUP_REF_SET_PARTICLE(group, env, all);
        }
        else if (AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice = NULL;
            choice =  handle_choice(builder, env, node1, schema_node);
            XML_SCHEMA_GROUP_REF_SET_PARTICLE(group, env, choice);
        }
    }
    return group;
}

static xml_schema_attribute_t*
handle_attribute(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *attr_node,
    axiom_node_t *schema_node)
{
    void *attr = NULL;
    axiom_element_t *attr_ele = NULL;
    xml_schema_builder_impl_t *builder_impl = NULL;
    axis2_char_t *attr_value = NULL;
    axis2_char_t *target_namespace = NULL;

    axiom_element_t *sim_ele = NULL;
    axiom_node_t *sim_node   = NULL;

    axiom_element_t *ann_ele = NULL;
    axiom_node_t   *ann_node = NULL;


    attr_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(attr_node, env);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    target_namespace = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);

    attr = xml_schema_attribute_create(env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "name");

    if (attr_value)
    {
        axis2_qname_t *qn = NULL;
        XML_SCHEMA_ATTRIBUTE_SET_NAME(attr, env, attr_value);
        qn = axis2_qname_create(env, attr_value, target_namespace, NULL);
        XML_SCHEMA_ATTRIBUTE_SET_QNAME(attr, env, qn);
    }

    attr_value = NULL;
    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "type");

    if (attr_value)
    {
        axis2_array_list_t *args = NULL;
        axis2_char_t *namesp = NULL;
        axis2_char_t *prefix = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *name = NULL;
        axis2_qname_t *qn = NULL;

        args = axis2_tokenize(env, attr_value, ':');

        if (AXIS2_ARRAY_LIST_SIZE(args, env) > 1)
        {
            axis2_hash_t *ht_ns = NULL;
            axiom_namespace_t *ns = NULL;
            ht_ns = AXIOM_ELEMENT_GET_NAMESPACES(attr_ele, env);
            prefix = AXIS2_ARRAY_LIST_GET(args, env, 0);
            if (ht_ns)
            {
                ns = axis2_hash_get(ht_ns, prefix, AXIS2_HASH_KEY_STRING);
                if (ns)
                {
                    namesp = AXIOM_NAMESPACE_GET_URI(ns, env);
                }
            }
            else
            {
                namesp = XML_SCHEMA_GET_NAMESPACE(builder_impl->schema,
                        env, prefix);
            }
            if (namesp ==  NULL)
            {
                /** TODO set error */
                return NULL;
            }
        }
        else
        {
            namesp = XML_SCHEMA_GET_NAMESPACE(builder_impl->schema,
                    env, "");
        }

        last_list = axis2_last_token(env, attr_value, ':');
        name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        qn = axis2_qname_create(env, name, namesp, NULL);
        XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_TYPE_NAME(attr, env, qn);
    }

    attr_value = NULL;

    if ((attr_value =
                AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "default")))
    {
        XML_SCHEMA_ATTRIBUTE_SET_DEFAULT_VALUE(attr, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "fixed")))
    {
        XML_SCHEMA_ATTRIBUTE_SET_FIXED_VALUE(attr, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value =
                AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "form")))
    {
        axis2_char_t* form_value = NULL;
        void *form = NULL;
        form_value = get_enum_string(builder, env, attr_ele, "form");
        form = xml_schema_form_create(env, form_value);
        XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_FORM(attr, env, form);
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "id")))
    {
        XML_SCHEMA_ANNOTATED_SET_ID(attr, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "use")))
    {
        axis2_char_t* use_value = NULL;
        void *use = NULL;
        use_value = get_enum_string(builder, env, attr_ele, "use");
        use = xml_schema_use_create(env, use_value);
        XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_FORM(attr, env, use);
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "id")))
    {
        XML_SCHEMA_ANNOTATED_SET_ID(attr, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_ele, env, "ref")))
    {
        axis2_array_list_t *ns_list = NULL;
        axis2_char_t *namesp        = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *name          = NULL;
        axis2_qname_t *ref_name     = NULL;

        ns_list = axis2_tokenize(env, attr_value, ':');
        if (AXIS2_ARRAY_LIST_SIZE(ns_list, env) > 1)
        {
            axis2_char_t *result = NULL;
            axis2_hash_t *ns_hash = NULL;
            axis2_char_t *prefix  = NULL;
            axis2_char_t *xml_prefix = NULL;
            prefix = AXIS2_ARRAY_LIST_GET(ns_list, env, 1);
            xml_prefix = AXIS2_ARRAY_LIST_GET(ns_list, env, 1);

            ns_hash = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

            result = axis2_hash_get(ns_hash, prefix, AXIS2_HASH_KEY_STRING);
            if (NULL == result &&  xml_prefix && AXIS2_STRCMP(xml_prefix, XML_SCHEMANS_PREFIX))
                result = XML_SCHEMANS_URI;
            if (NULL == result)
            {
                /**TODO set error */
                return NULL;
            }
            namesp = result;
        }
        else
        {
            namesp = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
        }

        last_list = axis2_last_token(env, attr_value, ':');
        name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        ref_name = axis2_qname_create(env, name, namesp, NULL);

        XML_SCHEMA_ATTRIBUTE_SET_REF_NAME(attr, env, ref_name);
        XML_SCHEMA_ATTRIBUTE_SET_NAME(attr, env, name);
    }


    sim_ele = axiom_util_get_first_child_element_with_uri_localname(attr_ele,
            env, attr_node, "simpleType", XML_SCHEMA_NS, &sim_node);

    if (sim_ele)
    {
        void *sim_type = NULL;
        sim_type = handle_simple_type(builder, env, sim_node,
                schema_node);
        XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_TYPE(attr, env, sim_type);
    }

    ann_ele = axiom_util_get_first_child_element_with_uri_localname(attr_ele,
            env, attr_node, "annotation", XML_SCHEMA_NS, &ann_node);

    if (ann_ele)
    {
        void *annotation = NULL;
        annotation = handle_annotation_with_element(builder, env, ann_node);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(attr, env, annotation);
    }
    /**
     NamedNodeMap attrNodes = attrEl.getAttributes();
        Vector attrs = new Vector();
        for (int i = 0; i < attrNodes.getLength(); i++) {
            Attr att = (Attr) attrNodes.item(i);
            if (!att.getName().equals("name") &&
                    !att.getName().equals("type") &&
                    !att.getName().equals("default") &&
                    !att.getName().equals("fixed") &&
                    !att.getName().equals("form") &&
                    !att.getName().equals("id") &&
                    !att.getName().equals("use") &&
                    !att.getName().equals("ref")) {


                attrs.add(att);
                String value = att.getValue();

                if (value.indexOf(":") > -1) {
                    String prefix = value.substring(0, value.indexOf(":"));
                    String namespace = (String) schema.namespaces.get(prefix);
                    if (namespace != null) {
                        Attr nsAttr = attrEl.getOwnerDocument().createAttribute("xmlns:" + prefix);
                        nsAttr.setValue(namespace);
                        attrs.add(nsAttr);
                    }
                }
            }
        }

        if (attrs.size() > 0)
            attr.setUnhandledAttributes((Attr[]) attrs.toArray(new Attr[0]));
       
    */
    return attr;
}

static xml_schema_element_t*
handle_element(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axiom_node_t *schema_node,
    axis2_bool_t is_global)
{
    void *sch_ele = NULL;

    axis2_char_t *attr_value = NULL;

    axiom_element_t *om_ele = NULL;

    axis2_bool_t is_qualified = AXIS2_FALSE;

    void *ele_form_default = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    axis2_char_t *form_value = NULL;

    axis2_char_t *ns = NULL;

    axis2_char_t *element_name = NULL;

    axiom_element_t *ann_ele = NULL;
    axiom_node_t   *ann_node = NULL;

    axiom_element_t *sim_type_ele = NULL;
    axiom_node_t    *sim_type_node = NULL;

    axiom_element_t *com_type_ele = NULL;
    axiom_node_t    *com_type_node = NULL;

    axiom_element_t *key__ele = NULL;
    axiom_node_t    *key_node = NULL;

    axiom_element_t *keyref_ele = NULL;
    axiom_node_t    *keyref_node = NULL;

    axiom_element_t *unique_ele = NULL;
    axiom_node_t    *unique_node = NULL;

    int max_occurs = 0;
    int min_occurs = 0;



    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    sch_ele = xml_schema_element_create(env);

    om_ele = AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);

    element_name = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env,
            "name");

    if (element_name)
    {
        XML_SCHEMA_ELEMENT_SET_NAME(sch_ele, env, element_name);
    }

    ele_form_default = XML_SCHEMA_GET_ELEMENT_FORM_DEFAULT(
                builder_impl->schema, env);

    form_value = XML_SCHEMA_ENUM_GET_VALUE(ele_form_default, env);

    if (form_value &&
            AXIS2_STRCMP(form_value, XML_SCHEMA_FORM_QUALIFIED) == 0)
    {
        is_qualified = AXIS2_TRUE;
    }

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "form");

    if (attr_value)
    {
        void *form_def = NULL;
        form_def = xml_schema_form_create(env, attr_value);
        XML_SCHEMA_ELEMENT_SET_FORM(sch_ele, env, form_def);

        if (AXIS2_STRCMP(attr_value, "qualified") == 0)
            is_qualified = AXIS2_TRUE;
    }

    if (AXIS2_TRUE == is_qualified || AXIS2_TRUE == is_global)
        ns = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);

    if (element_name)
    {
        axis2_qname_t *qn = NULL;
        qn = axis2_qname_create(env, element_name, ns , NULL);
        XML_SCHEMA_ELEMENT_SET_QNAME(sch_ele, env, qn);
    }

    ann_ele = axiom_util_get_first_child_element_with_uri_localname(om_ele, env,
            ele_node, "annotation", XML_SCHEMA_NS, &ann_node);

    if (ann_ele)
    {
        void *annotation = NULL;

        annotation = handle_annotation_with_element(builder, env,  ann_node);

        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sch_ele, env, annotation);
    }

    attr_value = NULL;

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "type")))
    {
        axis2_array_list_t *args = NULL;
        axis2_char_t *namesp = NULL;
        axis2_char_t *prefix = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *type_name = NULL;
        axis2_qname_t *type_qname = NULL;
        void *type = NULL;

        args = axis2_tokenize(env, attr_value, ':');


        if (AXIS2_ARRAY_LIST_SIZE(args, env) > 1)
        {
            axis2_hash_t *ht_ns = NULL;
            axiom_namespace_t *ns = NULL;
            ht_ns = AXIOM_ELEMENT_GET_NAMESPACES(om_ele, env);
            prefix = AXIS2_ARRAY_LIST_GET(args, env, 0);
            if (ht_ns)
            {
                ns = axis2_hash_get(ht_ns, prefix, AXIS2_HASH_KEY_STRING);
                if (ns)
                    namesp = AXIOM_NAMESPACE_GET_URI(ns, env);
            }
            if (NULL == namesp)
            {       /** namespace not found */
                namesp = XML_SCHEMA_GET_NAMESPACE(builder_impl->schema,
                        env, prefix);
            }
            if (namesp ==  NULL)
            {
                /** TODO set error */
                return NULL;
            }
        }
        else
        {
            namesp = XML_SCHEMA_GET_NAMESPACE(builder_impl->schema,
                    env, "");

        }

        last_list = axis2_last_token(env, attr_value, ':');
        type_name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        type_qname = axis2_qname_create(env, type_name, namesp, NULL);
        XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE_QNAME(sch_ele, env, type_qname);

        type = XML_SCHEMA_COLLECTION_GET_TYPE_BY_QNAME(
                    builder_impl->collection, env, type_qname);

        if (type)
        {
            XML_SCHEMA_COLLECTION_ADD_UNRESOLVED_TYPE(
                builder_impl->collection, env, type_qname, sch_ele);

        }

        XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(sch_ele, env, type);
        if (args)
        {
            AXIS2_ARRAY_LIST_FREE(args, env);
            args = NULL;
        }
        if (last_list)
        {
            AXIS2_ARRAY_LIST_FREE(last_list, env);
            last_list = NULL;
        }
    }
    else if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "ref")))
    {
        axis2_array_list_t *args = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *ref_name   = NULL;
        axis2_char_t *args0      = NULL;
        axis2_char_t *namesp     = NULL;
        axis2_hash_t *ht_ns      = NULL;
        axis2_qname_t *qn        = NULL;

        args = axis2_tokenize(env, attr_value, ':');
        ht_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

        if (AXIS2_ARRAY_LIST_SIZE(args, env) > 0)
        {
            axis2_char_t *result = NULL;

            args0 = AXIS2_ARRAY_LIST_GET(args, env, 0);
            result = axis2_hash_get(ht_ns, args0, AXIS2_HASH_KEY_STRING);
            if (!result)
            {
                /**TODO set error */
                return NULL;
            }
            namesp = result;
        }
        else
        {
            namesp = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
        }

        last_list = axis2_last_token(env, attr_value, ':');
        ref_name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        qn = axis2_qname_create(env, ref_name, namesp, NULL);

        XML_SCHEMA_ELEMENT_SET_REF_NAME(sch_ele, env, qn);
        XML_SCHEMA_ELEMENT_SET_NAME(sch_ele, env, ref_name);
    }


    if ((sim_type_ele = axiom_util_get_first_child_element_with_uri_localname(
                om_ele, env, ele_node, "simpleType", XML_SCHEMA_NS, &sim_type_node)))
    {
        void *sim_type = NULL;
        axis2_qname_t *qn = NULL;

        sim_type = handle_simple_type(builder, env, sim_type_node, ele_node);

        XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(sch_ele, env, sim_type);

        qn = XML_SCHEMA_TYPE_GET_QNAME(sim_type, env);

        XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE_QNAME(sch_ele, env, qn);

    }
    else if ((com_type_ele = axiom_util_get_first_child_element_with_uri_localname(
                om_ele, env, ele_node, "complexType", XML_SCHEMA_NS, &com_type_node)))
    {
        void *cmp_type = NULL;
        cmp_type = handle_complex_type(builder, env, com_type_node, schema_node);

        XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(sch_ele, env, cmp_type);

    }
    else if ((key__ele = axiom_util_get_first_child_element_with_uri_localname(
                om_ele, env, ele_node, "key", XML_SCHEMA_NS, &key_node)))
    {
        void *key = NULL;
        xml_schema_obj_collection_t *constraints = NULL;
        key = handle_constraint(builder, env, key_node, schema_node, XML_SCHEMA_KEY);
        constraints = XML_SCHEMA_ELEMENT_GET_CONSTRAINTS(sch_ele, env);
        XML_SCHEMA_OBJ_COLLECTION_ADD(constraints, env, key);
    }
    else if ((keyref_ele = axiom_util_get_first_child_element_with_uri_localname(
                om_ele, env, ele_node, "keyref", XML_SCHEMA_NS, &keyref_node)))
    {
        void *keyref = NULL;
        xml_schema_obj_collection_t *constraints = NULL;
        axis2_char_t *attr_val = NULL;

        keyref = handle_constraint(builder, env, keyref_node, schema_node, XML_SCHEMA_KEYREF);
        constraints = XML_SCHEMA_ELEMENT_GET_CONSTRAINTS(sch_ele, env);
        attr_val = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "keyref");

        if (attr_val)
        {
            axis2_array_list_t *args = NULL;
            axis2_array_list_t *last_list = NULL;
            axis2_char_t *ref_name   = NULL;
            axis2_char_t *args0      = NULL;
            axis2_char_t *namesp     = NULL;
            axis2_hash_t *ht_ns      = NULL;
            axis2_qname_t *qn        = NULL;

            args = axis2_tokenize(env, attr_val, ':');
            ht_ns = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

            if (AXIS2_ARRAY_LIST_SIZE(args, env) > 0)
            {
                axis2_char_t *result = NULL;

                args0 = AXIS2_ARRAY_LIST_GET(args, env, 0);
                result = axis2_hash_get(ht_ns, args0, AXIS2_HASH_KEY_STRING);
                if (!result)
                {
                    /**TODO set error */
                    return NULL;
                }
                namesp = result;
            }
            else
            {
                namesp = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
            }

            last_list = axis2_last_token(env, attr_val, ':');
            ref_name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
            qn = axis2_qname_create(env, ref_name, namesp, NULL);

            XML_SCHEMA_IDENTITY_CONSTRAINT_SET_REFER(keyref, env, qn);
        }
        XML_SCHEMA_OBJ_COLLECTION_ADD(constraints, env, keyref);
    }
    else if ((unique_ele = axiom_util_get_first_child_element_with_uri_localname(
                om_ele, env, ele_node, "unique", XML_SCHEMA_NS, &unique_node)))
    {
        void *unique = NULL;
        xml_schema_obj_collection_t *constraints = NULL;
        unique  = handle_constraint(builder, env, unique_node, schema_node, XML_SCHEMA);
        constraints = XML_SCHEMA_ELEMENT_GET_CONSTRAINTS(sch_ele, env);

        XML_SCHEMA_OBJ_COLLECTION_ADD(constraints, env, unique);
    }
    attr_value = NULL;

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "abstract")))
    {
        if (AXIS2_STRCMP(attr_value, "true") == 0)
            XML_SCHEMA_ELEMENT_SET_ABSTRACT(sch_ele, env, AXIS2_TRUE);
        else if (AXIS2_STRCMP(attr_value, "false") == 0)
            XML_SCHEMA_ELEMENT_SET_ABSTRACT(sch_ele, env, AXIS2_FALSE);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "block")))
    {
        void *block = NULL;
        block = get_derivation(env, ele_node, "block");
        XML_SCHEMA_ELEMENT_SET_BLOCK(sch_ele, env, block);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "default")))
    {
        XML_SCHEMA_ELEMENT_SET_DEFAULT_VALUE(sch_ele, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "final")))
    {
        void *final_drv = NULL;
        final_drv = get_derivation(env, ele_node, "final");
        XML_SCHEMA_ELEMENT_SET_FINAL(sch_ele, env, final_drv);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "fixed")))
    {
        XML_SCHEMA_ELEMENT_SET_FIXED_VALUE(sch_ele, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "id")))
    {
        XML_SCHEMA_ANNOTATED_SET_ID(sch_ele, env, attr_value);
        attr_value = NULL;
    }

    if ((attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "nillable")))
    {
        if (AXIS2_STRCMP(attr_value, "true") == 0)
            XML_SCHEMA_ELEMENT_SET_NILLABLE(sch_ele, env, AXIS2_TRUE);
        else if (AXIS2_STRCMP(attr_value, "false") == 0)
            XML_SCHEMA_ELEMENT_SET_NILLABLE(sch_ele, env, AXIS2_FALSE);
        attr_value = NULL;
    }

    max_occurs = get_max_occurs(env, ele_node);
    min_occurs = get_min_occurs(env, ele_node);

    XML_SCHEMA_PARTICLE_SET_MAX_OCCURS(sch_ele, env, max_occurs);
    XML_SCHEMA_PARTICLE_SET_MIN_OCCURS(sch_ele, env, min_occurs);
    return sch_ele;
}
/*
static axis2_status_t
populate_element_namespaces(
        xml_schema_builder_t *builder,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axis2_hash_t* ele_to_ns_map)
{
    axiom_element_t *ele = NULL;
    axis2_hash_t *ns_hash = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ele_to_ns_map, AXIS2_FAILURE);

    ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
    ns_hash = AXIOM_ELEMENT_GET_NAMESPACES(ele, env);

    if( ns_hash)
    {
        axis2_hash_index_t *hi = NULL;
        for(hi = axis2_hash_first(ns_hash, env);
            hi; hi = axis2_hash_next(env, hi))
        {
            void *val = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            if( val)
            {
                axiom_namespace_t *ns = NULL;
                axis2_char_t *uri = NULL;
                axis2_char_t *prefix = NULL;
                ns = (axiom_namespace_t*)val;
                uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, env);

                axis2_hash_set(ele_to_ns_map, AXIS2_STRDUP(prefix, env),
                    AXIS2_HASH_KEY_STRING, AXIS2_STRDUP(uri, env));
            }
        }
    }
    return AXIS2_SUCCESS;
}
*/
static xml_schema_identity_constraint_t*
handle_constraint(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *cnst_node,
    axiom_node_t *schema_node,
    int type)
{
    void *constraint = NULL;
    axiom_element_t *cnst_ele = NULL;
    axis2_char_t *attr_value = NULL;
    xml_schema_builder_impl_t *builder_impl = NULL;


    axiom_element_t *ele1  = NULL;
    axiom_node_t    *node1 = NULL;
    axis2_char_t *localname   = NULL;

    if (type == XML_SCHEMA_KEY)
    {
        constraint = xml_schema_key_create(env);
    }
    else if (type == XML_SCHEMA_KEYREF)
    {
        constraint = xml_schema_keyref_create(env);
    }
    else if (type == XML_SCHEMA_UNIQUE)
    {
        constraint = xml_schema_unique_create(env);
    }
    else
    {
        return NULL;

    }
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    cnst_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(cnst_node, env);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnst_ele, env, "name");
    if (attr_value)
        XML_SCHEMA_IDENTITY_CONSTRAINT_SET_NAME(constraint, env, attr_value);

    attr_value = NULL;

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnst_ele, env, "refer");

    if (attr_value)
    {
        axis2_array_list_t *ns_from_ele = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *ns = NULL;
        axis2_char_t *name = NULL;
        axis2_qname_t *qname = NULL;

        ns_from_ele = axis2_tokenize(env, attr_value, ':');
        /** check this */
        if (ns_from_ele && AXIS2_ARRAY_LIST_SIZE(ns_from_ele, env) > 1)
        {
            axis2_char_t *ns1 = NULL;

            axis2_hash_t *namespaces_ht = NULL;
            ns1 = AXIS2_ARRAY_LIST_GET(ns_from_ele, env, 0);

            namespaces_ht =
                XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);

            ns = axis2_hash_get(namespaces_ht, ns1, AXIS2_HASH_KEY_STRING);
        }
        else
        {
            ns = XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
        }

        last_list = axis2_last_token(env, attr_value, ':');
        name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        XML_SCHEMA_IDENTITY_CONSTRAINT_SET_NAME(constraint, env, name);

        qname = axis2_qname_create(env, name, ns, NULL);

        XML_SCHEMA_IDENTITY_CONSTRAINT_SET_REFER(constraint, env, qname);
    }

    ele1 = axiom_util_get_first_child_element_with_uri(cnst_node, env,
            XML_SCHEMA_NS, &node1);

    while (ele1)
    {
        localname = AXIOM_ELEMENT_GET_LOCALNAME(ele1, env);

        if (AXIS2_STRCMP(localname, "selector") == 0)
        {
            void *sel_xpath = NULL;
            axiom_element_t *ann_ele = NULL;
            axiom_node_t *ann_node   = NULL;

            axis2_char_t *attr_value = NULL;
            sel_xpath = xml_schema_xpath_create(env);
            attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele1, env, "xpath");

            XML_SCHEMA_XPATH_SET_XPATH(sel_xpath, env, attr_value);

            ann_ele = axiom_util_get_first_child_element_with_uri_localname(ele1,
                    env, node1 , "annotation", XML_SCHEMA_NS, &ann_node);
            if (ann_ele)
            {
                void *annotation = handle_annotation_with_element(builder, env, ann_node);
                XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sel_xpath, env, annotation);


            }

            XML_SCHEMA_IDENTITY_CONSTRAINT_SET_SELECTOR(constraint, env, sel_xpath);
        }
        else if (AXIS2_STRCMP(localname, "field") == 0)
        {
            void *field_xpath = NULL;
            axiom_element_t *ann_ele = NULL;
            axiom_node_t    *ann_node = NULL;

            xml_schema_obj_collection_t *fields = NULL;
            axis2_char_t *attr_value = NULL;



            field_xpath = xml_schema_xpath_create(env);

            attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele1, env, "xpath");

            XML_SCHEMA_XPATH_SET_XPATH(field_xpath, env, attr_value);

            fields = XML_SCHEMA_IDENTITY_CONSTRAINT_GET_FIELDS(constraint, env);
            XML_SCHEMA_OBJ_COLLECTION_ADD(fields, env, field_xpath);

            ann_ele = axiom_util_get_first_child_element_with_uri_localname(
                        ele1, env, node1, "annotation", XML_SCHEMA_NS, &ann_node);

            if (ann_ele)
            {
                void *annotation = NULL;
                annotation = handle_annotation_with_element(builder, env, ann_node);
                XML_SCHEMA_ANNOTATED_SET_ANNOTATION(field_xpath, env, annotation);
            }
        }
        else if (AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *constraint_annotation = handle_annotation_with_element(builder, env, node1);
            XML_SCHEMA_ANNOTATED_SET_ANNOTATION(constraint, env, constraint_annotation);
        }
        ele1 = axiom_util_get_next_sibling_element_with_uri(node1, env,
                XML_SCHEMA_NS, &node1);
    }
    return constraint;
}

static xml_schema_import_t*
handle_import(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *import_node,
    axiom_node_t *schema_node)
{
    void *import = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;

    axiom_element_t *ann_ele = NULL;
    axiom_node_t*   ann_node = NULL;

    axiom_element_t *import_ele = NULL;

    axis2_char_t *ns    = NULL;
    axis2_char_t *sch_location = NULL;

    import_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(import_node, env);
    import = xml_schema_import_create(env);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    ann_ele = axiom_util_get_first_child_element_with_uri_localname(import_ele, env,
            import_node, "annotation", XML_SCHEMA_NS, &ann_node);

    if (ann_ele)
    {
        void *import_ann = NULL;
        import_ann = handle_annotation_with_element(builder, env, ann_node);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(import, env, import_ann);
    }

    ns = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_ele, env, "namespace");
    sch_location = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_ele, env, "schemaLocation");


    XML_SCHEMA_IMPORT_SET_NAMESPACE(import, env, ns);
    XML_SCHEMA_EXTERNAL_SET_SCHEMA_LOCATION(import, env, sch_location);

    if (sch_location && (AXIS2_STRCMP(sch_location, "") != 0))
    {
        axis2_char_t *source_uri = NULL;
        source_uri = XML_SCHEMA_OBJ_GET_SOURCE_URI(builder_impl->schema, env);
        if (source_uri)
        {
            void *schema = NULL;
            schema = resolve_xml_schema_with_uri(builder , env, ns, sch_location, source_uri);
            XML_SCHEMA_EXTERNAL_SET_SCHEMA(import, env, schema);
        }
        else
        {
            void *schema = NULL;
            schema = resolve_xml_schema(builder, env, ns, sch_location);
            XML_SCHEMA_EXTERNAL_SET_SCHEMA(import, env, schema);
        }
    }
    return import;
}

static xml_schema_external_t *
handle_include(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *include_node,
    axiom_node_t *schema_node)
{
    void* include = NULL;
    axiom_element_t *inc_ele = NULL;
    axiom_element_t *ann_ele = NULL;
    axiom_node_t *ann_node = NULL;
    axis2_char_t *sch_location = NULL;
    axis2_char_t *source_uri = NULL;

    xml_schema_builder_impl_t *builder_impl = NULL;
    void *sch = NULL;
    AXIS2_PARAM_CHECK(env->error, include_node, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    include = xml_schema_include_create(env);
    inc_ele = AXIOM_NODE_GET_DATA_ELEMENT(include_node, env);
    ann_ele = axiom_util_get_first_child_element_with_uri_localname(inc_ele, env,
            include_node, "annotation", XML_SCHEMA_NS, &ann_node);
    if (ann_ele)
    {
        void *include_annotation = NULL;
        include_annotation = handle_annotation_with_element(builder, env, ann_node);
        XML_SCHEMA_ANNOTATED_SET_ANNOTATION(include, env, include_annotation);
    }

    sch_location =
        AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(inc_ele, env, "schemaLocation");

    source_uri = XML_SCHEMA_OBJ_GET_SOURCE_URI(builder_impl->schema, env);

    if (source_uri)
    {
        sch_location = XML_SCHEMA_EXTERNAL_GET_SCHEMA_LOCATION(include, env);
        sch =  resolve_xml_schema_with_uri(builder, env, NULL, sch_location, source_uri);
        XML_SCHEMA_EXTERNAL_SET_SCHEMA(include, env , sch);
    }
    else
    {
        sch_location = XML_SCHEMA_EXTERNAL_GET_SCHEMA_LOCATION(include, env);
        sch = resolve_xml_schema(builder, env, NULL,  sch_location);
    }
    return include;
}

static xml_schema_annotation_t*
handle_annotation_with_element(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *ann_node)
{
    xml_schema_obj_collection_t *content = NULL;
    void *app_info_obj = NULL;
    void *docs_obj     = NULL;

    axiom_element_t *ann_ele = NULL;

    axiom_element_t* app_info_ele = NULL;
    axiom_node_t*    app_info_node = NULL;

    axiom_element_t* doc_ele  = NULL;
    axiom_node_t*    doc_node = NULL;

    void *annotation = NULL;

    annotation = xml_schema_annotation_create(env);

    content = XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env);

    ann_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ann_node, env);

    app_info_ele = axiom_util_get_first_child_element_with_uri_localname(
                ann_ele, env, ann_node, "appinfo" , XML_SCHEMA_NS, &app_info_node);
    while (app_info_ele)
    {
        app_info_obj = handle_app_info(builder, env, app_info_node);
        XML_SCHEMA_OBJ_COLLECTION_ADD(content, env, app_info_obj);

        app_info_obj = NULL;
        app_info_ele = axiom_util_get_next_sibling_element_with_uri_localname(app_info_ele, env,
                app_info_node, "appinfo", XML_SCHEMA_NS, &app_info_node);
    }


    doc_ele = axiom_util_get_first_child_element_with_uri_localname(ann_ele, env,
            ann_node, "documentation", XML_SCHEMA_NS, &doc_node);

    while (doc_ele)
    {
        docs_obj = handle_documentation(builder, env, doc_node);
        XML_SCHEMA_OBJ_COLLECTION_ADD(content, env, docs_obj);

        docs_obj = NULL;
        doc_ele = axiom_util_get_next_sibling_element_with_uri_localname(doc_ele, env,
                doc_node, "documentation", XML_SCHEMA_NS, &doc_node);
    }
    return annotation;
}

static xml_schema_app_info_t*
handle_app_info(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *content)
{
    void *app_info = NULL;
    axiom_element_t *cnt_ele = NULL;
    axiom_child_element_iterator_t *ele_iter = NULL;
    axis2_char_t *attr_value = NULL;

    AXIS2_PARAM_CHECK(env->error, content, NULL);

    app_info = xml_schema_app_info_create(env);
    cnt_ele  = AXIOM_NODE_GET_DATA_ELEMENT(content, env);

    ele_iter = axiom_util_get_child_elements(cnt_ele, env, content);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnt_ele, env, "source");
    if (!attr_value &&(!ele_iter || !AXIOM_CHILD_ELEMENT_ITERATOR_HAS_NEXT(ele_iter, env)))
        return NULL;

    XML_SCHEMA_APP_INFO_SET_SOURCE(app_info, env, attr_value);
    XML_SCHEMA_APP_INFO_SET_MARKUP(app_info, env, ele_iter);
    return app_info;
}

static xml_schema_documentation_t*
handle_documentation(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_node_t *content)
{
    void *documentation = NULL;
    axiom_child_element_iterator_t *ele_iter = NULL;
    axiom_element_t *cnt_ele = NULL;
    axis2_char_t *attr_value = NULL;
    axis2_char_t *xmllang = NULL;

    AXIS2_PARAM_CHECK(env->error, content, NULL);

    documentation = xml_schema_documentation_create(env);
    cnt_ele    = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(content, env);

    ele_iter   = axiom_util_get_child_elements(cnt_ele, env, content);

    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnt_ele, env, "source");
    /** TODO check this */
    xmllang    = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnt_ele, env, "xml:lang");

    if (!attr_value && !xmllang &&
            (!ele_iter || !AXIOM_CHILD_ELEMENT_ITERATOR_NEXT(ele_iter, env)))
        return NULL;

    XML_SCHEMA_DOCUMENTATION_SET_SOURCE(documentation, env, attr_value);
    XML_SCHEMA_DOCUMENTATION_SET_LANGUAGE(documentation, env, xmllang);
    XML_SCHEMA_DOCUMENTATION_SET_MARKUP(documentation, env, ele_iter);
    return documentation;
}

static long
get_min_occurs(
    const axis2_env_t *env,
    axiom_node_t *ele_node)
{
    axis2_char_t *attr_value = NULL;
    axiom_element_t *om_ele = NULL;
    if (!ele_node || AXIOM_NODE_GET_NODE_TYPE(ele_node, env) != AXIOM_ELEMENT)
        return AXIS2_FAILURE;
    om_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "minOccurs");
    if (attr_value)
    {
        if (AXIS2_STRCMP(attr_value, "unbounded") == 0)
        {
            return LONG_MIN;
        }
        else
            return atol(attr_value);
    }
    return -1;
}

static long
get_max_occurs(
    const axis2_env_t *env,
    axiom_node_t *ele_node)
{
    axiom_element_t *om_ele = NULL;
    axis2_char_t *attr_value = NULL;
    if (!ele_node || AXIOM_NODE_GET_NODE_TYPE(ele_node, env) != AXIOM_ELEMENT)
        return AXIS2_FAILURE;
    om_ele = (axiom_element_t*)
            AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "maxOccurs");
    if (attr_value)
    {
        if (AXIS2_STRCMP(attr_value, "unbounded") == 0)
        {
            return LONG_MAX;
        }
        else
        {
            return atol(attr_value);
        }
    }
    return 1;
}

static xml_schema_derivation_method_t*
get_derivation(
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axis2_char_t *attr_name)
{
    axis2_char_t *attr_value = NULL;
    axiom_element_t *om_ele = NULL;
    if (!ele_node || !attr_name)
        return NULL;
    if (AXIOM_NODE_GET_NODE_TYPE(ele_node, env) != AXIOM_ELEMENT)
        return NULL;
    om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, attr_name);
    if (attr_value && AXIS2_STRCMP(attr_value, "") != 0)
    {
        /** TODO trim attr_value */
        if (AXIS2_STRCMP(attr_value, "#all") == 0)
        {
            return xml_schema_derivation_method_create(env, XML_SCHEMA_CONST_ALL);
        }
        else
            return xml_schema_derivation_method_create(env, attr_value);
    }
    else
        return xml_schema_derivation_method_create(env, XML_SCHEMA_CONST_NONE);
}

static axis2_char_t *
get_enum_string(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axiom_element_t *om_ele,
    axis2_char_t *attr_name)
{
    axis2_char_t *attr_value = NULL;
    if (!om_ele && !attr_name)
        return NULL;
    attr_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, attr_name);
    if (attr_value)
    {
        axis2_char_t *atr_val = NULL;
        atr_val = AXIS2_STRDUP(attr_value, env);

        return (axis2_char_t*)AXIS2_STRTRIM(env, atr_val, NULL);
    }
    return XML_SCHEMA_CONST_NONE;
}

static axis2_bool_t
value_exist_in_hash(
    const axis2_env_t *env,
    axis2_hash_t *ht,
    axis2_char_t *value)
{
    void *val = NULL;
    axis2_hash_index_t *hi = NULL;
    if (!value || !ht)
        return AXIS2_FALSE;
    for (hi = axis2_hash_first(ht, env); hi;
            hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, NULL, NULL, &val);
        if (val)
        {
            axis2_char_t *htvalue = NULL;
            htvalue = (axis2_char_t *)val;
            if (AXIS2_STRCMP(value, htvalue) == 0)
                return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

static xml_schema_form_t*
get_from_default(
    const axis2_env_t *env,
    axiom_node_t *ele_node,
    axis2_char_t *attr_name)
{
    axiom_element_t *om_ele = NULL;
    if (ele_node &&
            AXIOM_NODE_GET_NODE_TYPE(ele_node , env) == AXIOM_ELEMENT)
    {
        axis2_char_t *value = NULL;
        om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
        value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, attr_name);
        if (value)
            return xml_schema_form_create(env, value);
        else
            return xml_schema_form_create(env, "unqualified");
    }
    return NULL;
}

static xml_schema_t *
resolve_xml_schema(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axis2_char_t *target_namespace,
    axis2_char_t *schema_location)
{
    axis2_char_t *base_uri = NULL;
    xml_schema_builder_impl_t *builder_impl = NULL;

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    base_uri = XML_SCHEMA_COLLECTION_GET_BASE_URI(
                builder_impl->collection, env);

    return resolve_xml_schema_with_uri(builder, env,
            target_namespace, schema_location,  base_uri);
}


static xml_schema_t *
resolve_xml_schema_with_uri(
    xml_schema_builder_t *builder,
    const axis2_env_t *env,
    axis2_char_t *target_namespace,
    axis2_char_t *schema_location,
    axis2_char_t *base_uri)
{
    axiom_xml_reader_t *xml_reader = NULL;
    axiom_document_t *om_doc = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    xml_schema_t *schema = NULL;
    xml_schema_builder_impl_t *sch_builder_impl = NULL;
    xml_schema_input_source_t *input_source = NULL;
    axis2_char_t *system_id = NULL;

    sch_builder_impl = AXIS2_INTF_TO_IMPL(builder);

    input_source = xml_schema_url_resolver_resolve_entity(env,
            target_namespace, schema_location, base_uri);

    if (!sch_builder_impl->collection)
        return NULL;

    if (input_source)
    {
        system_id = XML_SCHEMA_INPUT_SOURCE_GET_SYSTEM_ID(input_source, env);
        if (system_id)
        {
            schema = XML_SCHEMA_COLLECTION_GET_SCHEMA(
                        sch_builder_impl->collection, env, system_id);
        }
    }
    else
    {
        return NULL;
    }
    if (schema)
    {
        return schema;
    }

    xml_reader = axiom_xml_reader_create_for_file(env, system_id, NULL);
    if (!xml_reader)
        return NULL;
    om_builder = axiom_stax_builder_create(env, xml_reader);
    if (!om_builder)
        return NULL;

    om_doc = axiom_document_create(env, NULL, om_builder);

    AXIOM_DOCUMENT_BUILD_ALL(om_doc, env);

    return XML_SCHEMA_COLLECTION_READ_DOCUMENT_WITH_URI(
                sch_builder_impl->collection , env, om_doc, base_uri);
}
