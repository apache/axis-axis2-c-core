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

#include <xml_schema/axis2_xml_schema_includes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <axis2_string_util.h>


typedef struct axis2_xml_schema_builder_impl 
{
    axis2_xml_schema_builder_t builder;

    axis2_om_document_t *om_doc;
    
    axis2_om_node_t *root_node;
    
    void *schema;
    
    axis2_xml_schema_collection_t *collection;
    
    int gen_no;

}axis2_xml_schema_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(builder) \
        ((axis2_xml_schema_builder_impl_t*)builder)



#define LONG_MAX_VALUE 0x7fffffffffffffff

/************************* function prototypes ********************************/

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_build(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_document_t *om_doc,
        axis2_char_t *uri);
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_build_with_root_node(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *root,
        axis2_char_t *uri);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_builder_free(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env); 


static axis2_xml_schema_t*
handle_xml_schema_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t  *uri);
        
static axis2_xml_schema_annotation_t* 
handle_annotation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ann_node,
        axis2_om_node_t *sch_node);
        
static axis2_xml_schema_redefine_t*
handle_redefine(        
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *redefine_node,
        axis2_om_node_t *sch_node);
        
static axis2_status_t
set_namespace_attributes(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_xml_schema_t *schema,
        axis2_om_node_t *sch_node);
        
static axis2_status_t
put_namespace(  
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_char_t *prefix,
        axis2_char_t *ns);              

static axis2_xml_schema_simple_type_t*
handle_simple_type(        
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *simple_node,
        axis2_om_node_t *sch_node);

static axis2_xml_schema_complex_type_t*
handle_complex_type(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *complex_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_simple_content_t*
handle_simple_content(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *simple_node,
        axis2_om_node_t *schema_node);        

static axis2_xml_schema_complex_content_t*
handle_complex_content(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *complex_node,
        axis2_om_node_t *schema_node);

static axis2_xml_schema_simple_content_restriction_t*
handle_simple_content_restriction(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *res_node,
        axis2_om_node_t *schema_node);
        
        
static axis2_xml_schema_simple_content_extension_t*
handle_simple_content_extension(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *sim_ext_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_complex_content_restriction_t*
handle_complex_content_restriction(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *res_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_complex_content_extension_t*
handle_complex_content_extension(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *cmp_ext_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_attribute_group_ref_t*
handle_attribute_group_ref(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_grp_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_group_base_t*
handle_sequence(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *seq_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_any_t*
handle_any(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *any_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_group_base_t*
handle_choice(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *choice_node,
        axis2_om_node_t *schema_node);
                   
static axis2_xml_schema_group_base_t*
handle_all(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *all_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_group_t* 
handle_group(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *group_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_attribute_group_t*
handle_attribute_group(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_grp_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_any_attribute_t*
handle_any_attribute(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *any_attr_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_group_ref_t*
handle_group_ref(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *grp_ref_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_attribute_t*
handle_attribute(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_node,
        axis2_om_node_t *schema_node);        
        
static axis2_xml_schema_element_t*
handle_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_om_node_t *schema_node,
        axis2_bool_t is_global);                                            

static axis2_status_t
populate_element_namespaces(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_hash_t* ele_to_ns_map);

static axis2_xml_schema_identity_constraint_t*
handle_constraint(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_node,
        axis2_om_node_t *schema_node,
        int type);

static axis2_xml_schema_import_t*
handle_import(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *import_node,
        axis2_om_node_t *schema_node);
        
static axis2_xml_schema_external_t *
handle_include(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *include_node,
        axis2_om_node_t *schema_node);
        
       
static axis2_xml_schema_app_info_t* 
handle_app_info(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *content);
        
static axis2_xml_schema_documentation_t*
handle_documentation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *content);
        
static long
get_min_occurs(
        axis2_env_t **env,
        axis2_om_node_t *content);

static long
get_max_occurs(
        axis2_env_t **env,            
        axis2_om_node_t *content);

static axis2_xml_schema_derivation_method_t*
get_derivation(
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t *attr_name);
        
static axis2_char_t *
get_enum_string(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_element_t *om_ele,
        axis2_char_t *attr_name);
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_resolve_xml_schema(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri);
        
static axis2_bool_t 
value_exist_in_hash(
        axis2_env_t **env,
        axis2_hash_t *ht,
        axis2_char_t *value);
        
static axis2_xml_schema_form_t* 
get_from_default(
        axis2_env_t **env,
        axis2_om_node_t *ele_node, 
        axis2_char_t *attr_name);
        
static axis2_xml_schema_t *
resolve_xml_schema(
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location);
        
static axis2_xml_schema_t *
resolve_xml_schema_with_uri(
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri);        
        
static axis2_xml_schema_annotation_t* 
handle_annotation_with_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ann_node);                                 
        
/*************** end function prototypes **************************************/

AXIS2_DECLARE(axis2_xml_schema_builder_t *)
axis2_xml_schema_builder_create(
        axis2_env_t **env,
        axis2_xml_schema_collection_t *sch_collection)
{
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, sch_collection, NULL);
    builder_impl = (axis2_xml_schema_builder_impl_t *)AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_xml_schema_builder_impl_t));
    if(!builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }        
    
    builder_impl->builder.ops = NULL;
    builder_impl->collection = NULL;
    builder_impl->om_doc = NULL;
    builder_impl->root_node = NULL;
    builder_impl->schema = NULL;
    builder_impl->gen_no = 0;
    
    builder_impl->schema = axis2_xml_schema_create(env, NULL, sch_collection);
    if(!builder_impl->schema)
    {
        axis2_xml_schema_builder_free(&(builder_impl->builder), env);
        return NULL;
    }
    builder_impl->collection = sch_collection;
    
    builder_impl->builder.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_builder_t));
    if(!builder_impl->builder.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_xml_schema_builder_free(&(builder_impl->builder), env);
        return NULL;
    }            

    builder_impl->builder.ops->free =
        axis2_xml_schema_builder_free;
                
    builder_impl->builder.ops->build =
        axis2_xml_schema_builder_build;
        
    builder_impl->builder.ops->build_with_root_node =
        axis2_xml_schema_builder_build_with_root_node;
        
    return &(builder_impl->builder);
}        

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_build(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_document_t *om_doc,
        axis2_char_t *uri)
{
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, om_doc, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->om_doc = om_doc;
    builder_impl->root_node = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(om_doc, env);            
    if(!builder_impl->root_node)
        return NULL;        
    return handle_xml_schema_element(builder, env, builder_impl->root_node, uri);
}
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_build_with_root_node(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *root,
        axis2_char_t *uri)
{


}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_builder_free(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env)
{
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    

} 


static axis2_xml_schema_t*
handle_xml_schema_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t  *uri)
{
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    
    axis2_hash_t *ht_sch2schemas = NULL;
    axis2_array_list_t *schemas        = NULL;
    axis2_hash_t *namespaces     = NULL;
    axis2_char_t *target_ns = NULL;
    
    /** element form default */
    void *ele_form = NULL;   
    void *attr_form = NULL;
    
    void *drv = NULL;
    
    /** ele and node */
    axis2_om_element_t *child_ele = NULL;
    axis2_om_node_t    *child_ele_node = NULL;
    
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, ele_node, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    set_namespace_attributes( builder, env, builder_impl->schema, ele_node);
    
    
    if(NULL != uri)
    {
        ht_sch2schemas = AXIS2_XML_SCHEMA_COLLECTION_GET_SYSTEMID2_SCHEMAS(
            builder_impl->collection, env);
            
        if(NULL != ht_sch2schemas)
        {
            axis2_hash_set(ht_sch2schemas, uri, 
                AXIS2_HASH_KEY_STRING, builder_impl->schema);
        }                
    }
    
    schemas = 
        AXIS2_XML_SCHEMA_COLLECTION_GET_SCHEMAS(builder_impl->collection, env);
    if(NULL != schemas)
    {
        AXIS2_ARRAY_LIST_ADD(schemas, env, builder_impl->schema);
    }        
    
    
    namespaces = 
        AXIS2_XML_SCHEMA_COLLECTION_GET_NAMESPACES(builder_impl->collection, env);
    target_ns = AXIS2_XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);        
        
    if(NULL != namespaces && NULL != target_ns)
    {
        void *val = NULL;
        val = axis2_hash_get(namespaces, target_ns, AXIS2_HASH_KEY_STRING);
        if(!val)
        {
            /** value does not exist */
            axis2_hash_set(namespaces, target_ns, 
                AXIS2_HASH_KEY_STRING, builder_impl->schema);
        
        }
    }
    
    ele_form = get_from_default(env, ele_node, "elementFormDefault");
    AXIS2_XML_SCHEMA_SET_ELEMENT_FORM_DEFAULT(builder_impl->schema, env, ele_form);
    
    ele_form = NULL;
                    
    attr_form = get_from_default(env, ele_node, "attributeFormDefault");
    AXIS2_XML_SCHEMA_SET_ATTR_FORM_DEFAULT(builder_impl->schema, env, attr_form);
    attr_form = NULL;
    
    drv = get_derivation(env, ele_node, "blockDefault");
    AXIS2_XML_SCHEMA_SET_BLOCK_DEFAULT(builder_impl->schema, env, drv);
    
    drv = NULL;
    
    drv = get_derivation(env, ele_node, "finalDefault");
    AXIS2_XML_SCHEMA_SET_FINAL_DEFAULT(builder_impl->schema, env, drv);
    
    /***********
         * for ( each childElement)
         *		if( simpleTypeElement)
         *			handleSimpleType
         *		else if( complexType)
         *			handleComplexType
         *		else if( element)
         *			handleElement
         *		else if( include)
         *			handleInclude
         *		else if( import)
         *			handleImport
         *		else if (group)
         *			handleGroup
         *		else if (attributeGroup)
         *			handleattributeGroup
         *		else if( attribute)
         *			handleattribute
         *		else if (redefine)
         *			handleRedefine
         *		else if(notation)
         *			handleNotation
         */
        child_ele = axis2_om_util_get_first_child_element_with_uri(ele_node, env,
            AXIS2_XML_SCHEMA_NS, &child_ele_node);
        while( NULL != child_ele)
        {
            axis2_char_t *localname = NULL;
            axis2_xml_schema_obj_collection_t *items = NULL;
            axis2_xml_schema_obj_collection_t *includes = NULL;
            
            localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(child_ele, env);
            items = AXIS2_XML_SCHEMA_GET_ITEMS(builder_impl->schema, env);
            includes = AXIS2_XML_SCHEMA_GET_INCLUDES(builder_impl->schema, env);
            
            if(!localname)
                continue;
        
            if(AXIS2_STRCMP(localname,"simpleType") == 0)
            {
                void *type = NULL;
                axis2_qname_t *qn = NULL;
                
                
                type = handle_simple_type(builder, env, child_ele_node, ele_node);
                
                AXIS2_XML_SCHEMA_ADD_TYPE(builder_impl->schema, env, type);
                if(NULL != items)
                {
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
                }
                qn = AXIS2_XML_SCHEMA_TYPE_GET_QNAME(type, env);
                if(NULL != qn)
                {
                    AXIS2_XML_SCHEMA_COLLECTION_RESOLVE_TYPE(builder_impl->collection, 
                        env, qn, type);
                }
            }        
            else if(AXIS2_STRCMP(localname, "complexType") == 0)
            {
                void *type = NULL;
                axis2_qname_t *qn = NULL;
                type = handle_complex_type(builder, env, child_ele_node, ele_node);
                
                AXIS2_XML_SCHEMA_ADD_TYPE(builder_impl->schema, env, type);
                if(NULL != items)
                {
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
                }
                            
                qn = AXIS2_XML_SCHEMA_TYPE_GET_QNAME(type, env);
                if(NULL != qn)
                {
                    AXIS2_XML_SCHEMA_COLLECTION_RESOLVE_TYPE(
                        builder_impl->collection, env, qn, type);
                }
            }
            else if(AXIS2_STRCMP(localname, "element") == 0)
            {
                void *sch_ele = NULL;
                sch_ele = handle_element(builder, env, child_ele_node, ele_node, AXIS2_TRUE);
                if(NULL != sch_ele)
                {
                    axis2_qname_t *qualified_qname = NULL;
                    axis2_qname_t *ref_qname = NULL;
                    axis2_xml_schema_obj_table_t *elements = NULL;
                    
                    elements = AXIS2_XML_SCHEMA_GET_ELEMENTS(builder_impl->schema, env);
                    
                    qualified_qname = AXIS2_XML_SCHEMA_ELEMENT_GET_REF_NAME(sch_ele, env);
                    ref_qname = AXIS2_XML_SCHEMA_ELEMENT_GET_REF_NAME(sch_ele, env);
                    
                    if(NULL != qualified_qname && NULL != elements)
                    {
                            AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(elements, env, 
                                qualified_qname, sch_ele);
                    }
                    else if(NULL != ref_qname && NULL != elements)
                    {
                        AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(elements, env, ref_qname, sch_ele);
                    }
                    if(NULL != items)
                        AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, sch_ele);
                }
            }
            else if(AXIS2_STRCMP(localname, "include") == 0)
            {
                void *inc_ele = NULL;
                
                inc_ele = handle_include(builder, env, child_ele_node, ele_node);
                if(NULL != includes)
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(includes, env, inc_ele);
                if(NULL != items)
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, inc_ele);
            }
            else if(AXIS2_STRCMP(localname, "import") == 0)
            {
                void *import_ele = NULL;
                import_ele = handle_import(builder, env, child_ele_node, ele_node);
                if(NULL != includes)
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(includes, env, import_ele);
                if(NULL != items)
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, import_ele);
            }
            else if(AXIS2_STRCMP(localname, "group"))
            {
                void *group = NULL;
                axis2_xml_schema_obj_table_t *groups = NULL;
                axis2_char_t *name  = NULL;
                group = handle_group(builder, env, child_ele_node, ele_node);
                groups = AXIS2_XML_SCHEMA_GET_GROUPS(builder_impl->schema, env);
                if(NULL != group)
                {
                    name = AXIS2_XML_SCHEMA_GROUP_GET_NAME(group, env);
                    AXIS2_XML_SCHEMA_OBJ_TABLE_PUT(groups, env, name, group);
                }
                if(NULL != items)
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, group);
            }
            else if(AXIS2_STRCMP(localname,"attributeGroup") == 0)
            {
                void *attr_grp = NULL;
                axis2_char_t *name = NULL;
                axis2_xml_schema_obj_table_t *attr_grps = NULL;
                attr_grp = handle_attribute_group(builder, env, child_ele_node,
                        ele_node);
                attr_grps = AXIS2_XML_SCHEMA_GET_ATTR_GROUPS(builder_impl->schema, env);                        
                if(NULL != attr_grp)
                {                        
                    name = AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_GET_NAME(attr_grp, env);
                    AXIS2_XML_SCHEMA_OBJ_TABLE_PUT(attr_grps, 
                        env, name, attr_grp);
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, attr_grp);
                }
            }            
            else if(AXIS2_STRCMP(localname, "attribute") == 0)
            {
                void *attr = NULL;
                axis2_xml_schema_obj_table_t *attr_tbl = NULL;
                attr = handle_attribute(builder, env, child_ele_node, ele_node);
                attr_tbl = AXIS2_XML_SCHEMA_GET_ATTRS(builder_impl->schema, env);
                if(NULL != attr)
                {
                    axis2_qname_t *qn= NULL;
                    qn = AXIS2_XML_SCHEMA_ATTRIBUTE_GET_QNAME(attr, env);
                    AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(attr_tbl, env, qn, attr);
                    AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, attr);
                }
            }
            else if(AXIS2_STRCMP(localname, "redefine") == 0)
            {
                void *redefine = NULL;
                redefine = handle_redefine(builder, env, child_ele_node, ele_node);
                AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(includes, env, redefine);            
            }
            else if(AXIS2_STRCMP(localname,"notation") == 0)
            {
                /**TODO implement notation */
            }
            else if(AXIS2_STRCMP(localname, "annotation") == 0)
            {
                void *annotation = NULL;
                annotation = handle_annotation(builder, env, child_ele_node, ele_node);
                    AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(
                        builder_impl->schema, env, annotation);
                
            
            }
        child_ele = NULL;
        child_ele = axis2_om_util_get_next_sibling_element_with_uri(child_ele_node, env, 
            AXIS2_XML_SCHEMA_NS, &child_ele_node);
        }
    return builder_impl->schema;    
}
        
static axis2_xml_schema_annotation_t* 
handle_annotation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ann_node,
        axis2_om_node_t *sch_node)
{
    void *annotation = NULL;
    axis2_om_element_t *child_ele = NULL;
    axis2_om_node_t *child_node   = NULL;
    axis2_om_element_t *ann_ele = NULL;
    axis2_xml_schema_obj_collection_t *collection = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, builder, NULL);
    AXIS2_PARAM_CHECK((*env)->error, ann_node, NULL);
    AXIS2_PARAM_CHECK((*env)->error, sch_node, NULL);

    annotation = axis2_xml_schema_annotation_create(env);
    collection = AXIS2_XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env);
    
    ann_ele = AXIS2_OM_NODE_GET_DATA_ELEMENT(ann_node, env);
    
    child_ele =    
        axis2_om_util_get_first_child_element(ann_ele, env, ann_node, &child_node);
        
    while(NULL != child_ele)
    {
        axis2_char_t *localname  = NULL;
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(child_ele, env);
        if(NULL != localname && AXIS2_STRCMP(localname, "documentation") == 0)
        {
            void *documentation = NULL;
            documentation = axis2_xml_schema_documentation_create(env);
            if(NULL != documentation)
            {
                axis2_om_child_element_iterator_t *ele_iter = NULL;
                ele_iter = axis2_om_util_get_child_elements(child_ele, env, child_node);
                if(NULL != ele_iter)
                {
                    AXIS2_XML_SCHEMA_DOCUMENTATION_SET_MARKUP(documentation, env, ele_iter);
                }
            }
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(collection, env, documentation);
        }    
        child_ele = axis2_om_util_get_first_child_element(child_ele , env, child_node, &child_node);
    }  
    return annotation;      
}
        
static axis2_xml_schema_redefine_t*
handle_redefine(        
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *redefine_node,
        axis2_om_node_t *sch_node)
{
    void *redefine = NULL;
    void *schema = NULL;
    axis2_om_element_t *redefine_ele = NULL;
    
    axis2_om_element_t *child_ele = NULL;
    axis2_om_node_t    *child_ele_node = NULL;
    axis2_char_t *schema_location = NULL;
    axis2_xml_schema_obj_table_t *schema_types = NULL;
    axis2_xml_schema_obj_collection_t *items = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, redefine_node, NULL);
    AXIS2_PARAM_CHECK((*env)->error, sch_node, NULL);
    
    redefine_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(redefine_node, env);
    if(!redefine_ele)
        return NULL;

    redefine = axis2_xml_schema_redefine_create(env);
    schema_location = 
    AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(redefine_ele, env, "schemaLocation");
    AXIS2_XML_SCHEMA_EXTERNAL_SET_SCHEMA_LOCATION(redefine, env, schema_location);
    
    schema = resolve_xml_schema(env, NULL, schema_location);
    
    schema_types = AXIS2_XML_SCHEMA_REDEFINE_GET_SCHEMA_TYPES(redefine, env);
    items =  AXIS2_XML_SCHEMA_REDEFINE_GET_ITEMS(redefine, env);
    
    child_ele = axis2_om_util_get_first_child_element_with_uri(
        redefine_node, env, AXIS2_XML_SCHEMA_NS, &child_ele_node);
    while(NULL != child_ele)
    {
        axis2_char_t *localname = NULL;
        
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(child_ele, env);
        if(AXIS2_STRCMP(localname, "simpleType") == 0)
        {
            void *type = NULL;
            axis2_qname_t *qn = NULL;
            type = handle_simple_type(builder, env, child_ele_node, sch_node);
            qn = AXIS2_XML_SCHEMA_TYPE_GET_QNAME(type, env);
            AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(schema_types, env, qn, type);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
        }            
        else if(AXIS2_STRCMP(localname,"complexType") == 0)
        {
            void *type = NULL;
            axis2_qname_t *qn = NULL;
            type = handle_complex_type(builder, env, child_ele_node, sch_node);
            qn = AXIS2_XML_SCHEMA_TYPE_GET_QNAME(type, env);
            
            AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(schema_types, env, qn, type);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, type);
        }
        else if(AXIS2_STRCMP(localname,"group") == 0)
        {
            void *grp = NULL;
            axis2_char_t *name = NULL;
            axis2_xml_schema_obj_table_t *groups = NULL;
            groups = AXIS2_XML_SCHEMA_REDEFINE_GET_GROUP(redefine, env);
            grp = handle_group(builder, env, child_ele_node, sch_node);
            name = AXIS2_XML_SCHEMA_GROUP_GET_NAME(grp, env);
            
            AXIS2_XML_SCHEMA_OBJ_TABLE_PUT(groups, env, name, grp);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, grp);
        }
        else if(AXIS2_STRCMP(localname, "attributeGroup") == 0)
        {
            void *attr_grp = NULL;
            axis2_xml_schema_obj_table_t *groups = NULL;
            axis2_char_t *name = NULL;
            
            attr_grp = handle_attribute_group(builder, env, child_ele_node, sch_node);
            groups = AXIS2_XML_SCHEMA_REDEFINE_GET_ATTRIBUTE_GROUP(redefine, env);
            name = AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_GET_NAME(attr_grp, env);
            AXIS2_XML_SCHEMA_OBJ_TABLE_PUT(groups, env, name, attr_grp);
        }
        else if(AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, child_ele_node);
            AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(redefine, env, annotation);
            
        }
    }       
    return redefine; 
}
        
static axis2_status_t
set_namespace_attributes(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_xml_schema_t *schema,
        axis2_om_node_t *sch_node)
{
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    axis2_om_node_t* parent_node = NULL;
    axis2_hash_t *ht_ns = NULL;
    axis2_hash_index_t *hi_ns = NULL;
    axis2_om_element_t *om_ele = NULL;
    
    axis2_hash_t *ht_sch_ns = NULL;
    axis2_qname_t *target_ns_qn = NULL;
    axis2_char_t *contain = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, schema, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, sch_node, AXIS2_FAILURE);
    if(AXIS2_OM_NODE_GET_NODE_TYPE(sch_node, env) != AXIS2_OM_ELEMENT)
        return AXIS2_FAILURE;
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    parent_node = AXIS2_OM_NODE_GET_PARENT(sch_node, env);
    if(NULL != parent_node && 
        AXIS2_OM_NODE_GET_NODE_TYPE(sch_node, env) == AXIS2_OM_ELEMENT)
    {
        set_namespace_attributes(builder, env, schema, parent_node);
    }
    
    om_ele = AXIS2_OM_NODE_GET_DATA_ELEMENT(sch_node, env);
    if(!om_ele)
        return AXIS2_FAILURE;            
    
    ht_ns = AXIS2_OM_ELEMENT_GET_NAMESPACES(om_ele, env);
    if(!ht_ns)
    {   /* no namespaces available */
        return AXIS2_SUCCESS;
    }
    
    ht_sch_ns = AXIS2_XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(schema, env);
    if(!ht_sch_ns)
        return AXIS2_FAILURE;
            
    for(hi_ns = axis2_hash_first(ht_ns, env); hi_ns;
        hi_ns = axis2_hash_next(env, hi_ns))
    {
        void *val = NULL;
        axis2_hash_this(hi_ns, NULL, NULL, &val);
        if(NULL != val)
        {
            axis2_char_t *uri = NULL;
            axis2_char_t *prefix = NULL;    
            uri = AXIS2_OM_NAMESPACE_GET_URI((axis2_om_namespace_t*)val, env);
            prefix = 
                AXIS2_OM_NAMESPACE_GET_PREFIX((axis2_om_namespace_t*)val, env);
            /**  if (map.item(i).getNodeName().startsWith("xmlns:")) {
                schema.namespaces.put(map.item(i).getLocalName(),
                        map.item(i).getNodeValue());
             */
            if(NULL != prefix)
            {                              
                axis2_hash_set(ht_sch_ns, prefix, AXIS2_HASH_KEY_STRING,
                    uri);
                if(NULL != uri && AXIS2_STRCMP(uri, AXIS2_XML_SCHEMA_NS) == 0)
                {
                    AXIS2_XML_SCHEMA_SET_SCHEMA_NS_PREFIX(schema, env, prefix);
                }
            }
            else
            {
                /** default namespace [xmlns=""] */
                axis2_hash_set(ht_sch_ns, "", AXIS2_HASH_KEY_STRING, uri);
            }
        }    
    }
    target_ns_qn = axis2_qname_create(env, "targetNamespace", NULL, NULL);
    contain = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE(om_ele, env, target_ns_qn);
    if(NULL != target_ns_qn)
        AXIS2_QNAME_FREE(target_ns_qn, env);

    if(NULL != contain && (value_exist_in_hash(env, ht_sch_ns, contain)) == AXIS2_FALSE)
    {
        put_namespace(builder, env, "", contain);
        if(AXIS2_STRCMP(contain,"") != 0)
        {
            AXIS2_XML_SCHEMA_SET_SCHEMA_NS_PREFIX(schema, env, contain);
        }               
    }                    
    return AXIS2_SUCCESS;
}    
        
static axis2_status_t
put_namespace(  
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_char_t *prefix,
        axis2_char_t *ns)
{
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    axis2_hash_t *ht_sch_ns = NULL;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ns , AXIS2_FAILURE);
    ht_sch_ns = AXIS2_XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);
    if(NULL != axis2_hash_get(ht_sch_ns, prefix, AXIS2_HASH_KEY_STRING))
    {
        /** key already exist */
        axis2_char_t *gen_prefix = AXIS2_MALLOC((*env)->allocator, sizeof(10));
        sprintf(gen_prefix, "gen%d", builder_impl->gen_no++);
        axis2_hash_set(ht_sch_ns, gen_prefix, AXIS2_HASH_KEY_STRING, ns);            
    }
    axis2_hash_set(ht_sch_ns, prefix, AXIS2_HASH_KEY_STRING, ns);
    return AXIS2_SUCCESS;    
}              

static axis2_xml_schema_simple_type_t*
handle_simple_type(        
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *simple_node,
        axis2_om_node_t *sch_node)
{
}

static axis2_xml_schema_complex_type_t*
handle_complex_type(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *complex_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_simple_content_t*
handle_simple_content(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *simple_node,
        axis2_om_node_t *schema_node){}        

static axis2_xml_schema_complex_content_t*
handle_complex_content(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *complex_node,
        axis2_om_node_t *schema_node){}

static axis2_xml_schema_simple_content_restriction_t*
handle_simple_content_restriction(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *res_node,
        axis2_om_node_t *schema_node){}
        
        
static axis2_xml_schema_simple_content_extension_t*
handle_simple_content_extension(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *sim_ext_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_complex_content_restriction_t*
handle_complex_content_restriction(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *res_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_complex_content_extension_t*
handle_complex_content_extension(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *cmp_ext_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_attribute_group_ref_t*
handle_attribute_group_ref(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_grp_node,
        axis2_om_node_t *schema_node)
{
    void *attr_grp_ref = NULL;
    
    axis2_om_element_t *ann_ele = NULL;
    axis2_om_node_t *ann_node   = NULL;
    axis2_char_t *attr_value = NULL;
    
    axis2_om_element_t *attr_grp_ele = NULL;

    attr_grp_ref = axis2_xml_schema_attribute_group_ref_create(env);
    
    attr_grp_ele = (axis2_om_element_t*)
        AXIS2_OM_NODE_GET_DATA_ELEMENT(attr_grp_node, env);
    
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_grp_ele,
        env, "ref");
        
    if(NULL != attr_value)
    {
        axis2_array_list_t *parts = NULL;
        axis2_char_t *prefix = NULL;
        axis2_hash_t *ht_ns  = NULL;
        axis2_char_t *uri = NULL;
        axis2_char_t *ref = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_qname_t *ref_qname = NULL;
        parts = axis2_tokenize(env, attr_value, ':');
        
        if(NULL != parts && AXIS2_ARRAY_LIST_SIZE(parts, env) > 1)
        {
            prefix = AXIS2_ARRAY_LIST_GET(parts, env, 0);
            ht_ns = AXIS2_XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(
                AXIS2_INTF_TO_IMPL(builder)->schema, env);
            
            uri = axis2_hash_get(ht_ns, prefix, AXIS2_HASH_KEY_STRING);
            if(NULL == uri)
            {
                /** TODO set error code */
                return NULL;
            }
        }
            
        last_list = axis2_last_token(env, attr_value, ':');
        ref = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        /** check this */
        ref_qname = axis2_qname_create(env, ref, uri, NULL);
        AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_SET_REF_QNAME(attr_grp_ref, env, ref_qname);                 }                    

    attr_value = NULL;
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(attr_grp_ele, env, "id");
    
    if(NULL != attr_value)
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ID(attr_grp_ref, env, attr_value);
        
    ann_ele = axis2_om_util_get_first_child_element_with_uri_localname(attr_grp_ele, env,
        attr_grp_node, "annotation", AXIS2_XML_SCHEMA_NS, &ann_node);
    
    if(NULL != ann_ele)
    {
        void *annotation = handle_annotation_with_element(builder, env, ann_node);
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(attr_grp_ref, env, annotation);
    }              
    return attr_grp_ref;
}
        
static axis2_xml_schema_group_base_t*
handle_sequence(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *seq_node,
        axis2_om_node_t *schema_node)
{
    void *sequence = NULL;
    axis2_om_element_t *seq_ele = NULL;
    
    axis2_om_element_t *ele1 = NULL;
    axis2_om_node_t    *node1 = NULL;
    axis2_xml_schema_obj_collection_t *items = NULL;
    
    sequence = axis2_xml_schema_sequence_create(env);
    
    ele1 = axis2_om_util_get_first_child_element_with_uri(seq_node, env, 
        AXIS2_XML_SCHEMA_NS, &node1);
        
    items = AXIS2_XML_SCHEMA_GROUP_BASE_GET_ITEMS(sequence, env);     
    while(NULL != ele1)
    {
        axis2_char_t *localname = NULL;
        
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1, env);
        
        if(AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *seq = NULL;
            seq = handle_sequence(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, seq);
        }
        else if(AXIS2_STRCMP(localname, "element") == 0)
        {
            void *element = NULL;
            element = handle_element(builder, env, 
                node1, schema_node, AXIS2_FALSE);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env,  element);
        }
        else if(AXIS2_STRCMP(localname, "group") == 0)
        {
            void *group = NULL;
            group = handle_group_ref(builder, env, node1, schema_node);
            
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, group);
        }
        else if(AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice_item = NULL;
            choice_item = handle_choice(builder, env, node1, schema_node);
            
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, choice_item);                    
        }
        else if(AXIS2_STRCMP(localname, "any") == 0)
        {
            void *any = NULL;
            any = handle_any(builder, env, node1, schema_node);
            
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, any);
        }
        else if(AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sequence, env, annotation);
        }          

        ele1 = axis2_om_util_get_first_child_element_with_uri(node1, env, 
            AXIS2_XML_SCHEMA_NS, &node1);
    }        
    return sequence;
}
        
static axis2_xml_schema_any_t*
handle_any(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *any_node,
        axis2_om_node_t *schema_node)
{
    axis2_om_element_t *any_ele = NULL;
    void *any                   = NULL;
    axis2_char_t *attr_value    = NULL;
    axis2_char_t *process_cnt   = NULL;
    void *pro_content           = NULL;
   
    axis2_om_element_t *annotation_ele = NULL;
    axis2_om_node_t    *annotation_node = NULL;
    long min_occurs = 0;
    long max_occurs = 0;

     
    any = axis2_xml_schema_any_create(env);
    any_ele = (axis2_om_element_t*)
        AXIS2_OM_NODE_GET_DATA_ELEMENT(any_node, env);
    
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(any_ele, env,
        "namespace");
    
    if(NULL != attr_value)
        AXIS2_XML_SCHEMA_ANY_SET_NAMESPACE(any, env, attr_value);
        
    process_cnt = get_enum_string(builder, env, any_ele, "processContents");
    
    if(NULL != process_cnt)
    {
        pro_content = axis2_xml_schema_content_processing_create(env, process_cnt);
        AXIS2_XML_SCHEMA_ANY_SET_PROCESS_CONTENT(any, env, pro_content);
    }                
    
    annotation_ele = axis2_om_util_get_first_child_element_with_uri_localname(any_ele, env,
        any_node, "annotation", AXIS2_XML_SCHEMA_NS, &annotation_node);
        
    if(NULL != annotation_ele)
    {
        void *annotation = NULL;
        annotation = handle_annotation_with_element(builder, env, annotation_node);
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(any, env, annotation);
    }    
    
    min_occurs = get_min_occurs(env, any_node);
    max_occurs = get_max_occurs(env, any_node);
    
    AXIS2_XML_SCHEMA_PARTICLE_SET_MAX_OCCURS(any, env, max_occurs);
    AXIS2_XML_SCHEMA_PARTICLE_SET_MIN_OCCURS(any, env, min_occurs);
    return any;
}
        
static axis2_xml_schema_group_base_t*
handle_choice(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *choice_node,
        axis2_om_node_t *schema_node)
{
    void *choice = NULL;
    axis2_char_t *attr_value = NULL;
    
    axis2_om_element_t *choice_ele = NULL;
    axis2_om_element_t *ele1 = NULL;
    axis2_om_node_t   *node1 = NULL;
    axis2_xml_schema_obj_collection_t *items = NULL;
    AXIS2_PARAM_CHECK((*env)->error, choice_node, NULL);
    
    AXIS2_PARAM_CHECK((*env)->error, schema_node, NULL);
    
    choice = axis2_xml_schema_choice_create(env);
    
    choice_ele = (axis2_om_element_t*)
        AXIS2_OM_NODE_GET_DATA_ELEMENT(choice_node, env);
    
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(choice_ele, env, "id");
    
    if(NULL != attr_value)
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ID(choice, env, attr_value);
        
    ele1 = axis2_om_util_get_first_child_element_with_uri(choice_node, env,
        AXIS2_XML_SCHEMA_NS, &node1);
        
    items = AXIS2_XML_SCHEMA_GROUP_BASE_GET_ITEMS(choice, env);
            
    while(NULL != ele1)
    {
        axis2_char_t *localname = NULL;
        
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1, env);
        if(AXIS2_STRCMP(localname, "sequence") == 0)
        {
            void *seq = NULL;
            seq = handle_sequence(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, seq);
        }
        else if(AXIS2_STRCMP(localname, "element") == 0)
        {
            void *element = NULL;
            element = handle_element(builder, env, 
                node1, schema_node, AXIS2_FALSE);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env,  element);
        }
        else if(AXIS2_STRCMP(localname, "group") == 0)
        {
            void *group = NULL;
            group = handle_group_ref(builder, env, node1, schema_node);
            
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, group);
        }
        else if(AXIS2_STRCMP(localname, "choice") == 0)
        {
            void *choice_item = NULL;
            choice_item = handle_choice(builder, env, node1, schema_node);
            
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, choice_item);                    }
        else if(AXIS2_STRCMP(localname, "any") == 0)
        {
            void *any = NULL;
            any = handle_any(builder, env, node1, schema_node);
            
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, any);
        }
        else if(AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(choice, env, annotation);
        }          

        ele1 = axis2_om_util_get_first_child_element_with_uri(node1, env, 
            AXIS2_XML_SCHEMA_NS, &node1);
    }        
    return choice;
}
                   
static axis2_xml_schema_group_base_t*
handle_all(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *all_node,
        axis2_om_node_t *schema_node)
{
    void *all = NULL;
    
    axis2_om_element_t *ele1 = NULL;
    axis2_om_node_t *node1   = NULL;
    
    axis2_om_node_t *all_ele = NULL;
    
    all = axis2_xml_schema_all_create(env);
    
    ele1 = axis2_om_util_get_first_child_element_with_uri(all_node, env,
        AXIS2_XML_SCHEMA_NS, &node1);
        
    while(NULL != ele1)
    {
        axis2_char_t *localname = NULL;
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1, env);
        
        if(AXIS2_STRCMP(localname, "element") == 0)
        {
            void *element = NULL;
            axis2_xml_schema_obj_collection_t *items = NULL;
            element = handle_element(builder, env, node1, 
                schema_node, AXIS2_FALSE);
            items = AXIS2_XML_SCHEMA_GROUP_BASE_GET_ITEMS(all, env);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(items, env, element);
        }
        else if(AXIS2_STRCMP(localname,"annotation") == 0)
        {
            void *annotation = NULL;
            annotation = handle_annotation_with_element(builder, env, node1);
            AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(all, env, annotation);
        }    
        ele1 = axis2_om_util_get_next_sibling_element_with_uri(node1, env, 
            AXIS2_XML_SCHEMA_NS, &node1);
    }        
    return all;
}
        
static axis2_xml_schema_group_t* 
handle_group(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *group_node,
        axis2_om_node_t *schema_node)
{
    axis2_om_element_t *grp_ele = NULL;
    void *grp = NULL;
    axis2_char_t *grp_name = NULL;
    
    axis2_om_node_t *node1 = NULL;
    axis2_om_element_t *ele1 = NULL;
    
    
    grp = axis2_xml_schema_group_create(env);
    
    grp_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
        group_node, env);
    
    grp_name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(grp_ele, env, 
        "name");
        
    AXIS2_XML_SCHEMA_GROUP_SET_NAME(grp, env, grp_name);
    
    grp_ele = (axis2_om_element_t*)
        AXIS2_OM_NODE_GET_DATA_ELEMENT(group_node, env);                
    
    ele1 = axis2_om_util_get_first_child_element_with_uri(group_node, env, 
        AXIS2_XML_SCHEMA_NS, &node1);
        
    while(NULL != ele1)
    {
        axis2_char_t *localname = NULL;
        void *particle = NULL;
        
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1, env);
        if(AXIS2_STRCMP(localname, "all") == 0)
        {
            particle = handle_all(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_GROUP_SET_PARTICLE(grp, env, particle);
        }
        else if(AXIS2_STRCMP(localname, "sequence") == 0)
        {
            particle = handle_sequence(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_GROUP_SET_PARTICLE(grp, env, particle);
        }
        else if(AXIS2_STRCMP(localname, "choice") == 0)
        {
            particle = handle_choice(builder, env, node1, schema_node);
            AXIS2_XML_SCHEMA_GROUP_SET_PARTICLE(grp, env, particle);
        }
        else if(AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *grp_annotation = NULL;
            grp_annotation = 
                handle_annotation_with_element(builder, env, node1);
            AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(grp, env, grp_annotation);
        }
        
        ele1 = axis2_om_util_get_next_sibling_element_with_uri(node1, env,
            AXIS2_XML_SCHEMA_NS, &node1);
    }        
    return grp;
}
        
static axis2_xml_schema_attribute_group_t*
handle_attribute_group(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_grp_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_any_attribute_t*
handle_any_attribute(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *any_attr_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_group_ref_t*
handle_group_ref(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *grp_ref_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_attribute_t*
handle_attribute(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_node,
        axis2_om_node_t *schema_node){}        
        
static axis2_xml_schema_element_t*
handle_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_om_node_t *schema_node,
        axis2_bool_t is_global)
{
    void *sch_ele= NULL;
    
    axis2_char_t *attr_value = NULL;
    
    axis2_om_element_t *om_ele = NULL;
    
    axis2_bool_t is_qualified = AXIS2_FALSE;
    
    void *ele_form_default = NULL;
    
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    
    axis2_char_t *form_value = NULL;
    
    axis2_char_t *ns = NULL;
    
    axis2_char_t *element_name = NULL;
    
    axis2_om_element_t *ann_ele = NULL;
    axis2_om_node_t   *ann_node = NULL;
    
    
    
    AXIS2_PARAM_CHECK((*env)->error, ele_node, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    sch_ele = axis2_xml_schema_element_create(env);
    
    om_ele = AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
    
    element_name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env,
            "name");
      
    if(NULL != element_name)
    {
        AXIS2_XML_SCHEMA_ELEMENT_SET_NAME(sch_ele, env, element_name);
    }
    
    ele_form_default = AXIS2_XML_SCHEMA_GET_ELEMENT_FORM_DEFAULT(
        builder_impl->schema, env);
        
    form_value = AXIS2_XML_SCHEMA_ENUM_GET_VALUE(ele_form_default, env);
    
    if(NULL != form_value && 
        AXIS2_STRCMP(form_value, AXIS2_XML_SCHEMA_FORM_QUALIFIED) == 0)
    {
        is_qualified = AXIS2_TRUE;
    }        
    
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "form");
     
    if(NULL != attr_value)
    {
        void *form_def = NULL;
        form_def = axis2_xml_schema_form_create(env, attr_value);
        AXIS2_XML_SCHEMA_ELEMENT_SET_FORM(sch_ele, env, form_def);
        
        if(AXIS2_STRCMP(attr_value, "form") == 0)
            is_qualified = AXIS2_TRUE;
    }    
    
    if(AXIS2_TRUE == is_qualified || AXIS2_TRUE == is_global)
        ns = AXIS2_XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
        
    if(NULL != element_name)
    {
        axis2_qname_t *qn = NULL;
        qn = axis2_qname_create(env, element_name, ns , NULL);
        AXIS2_XML_SCHEMA_ELEMENT_SET_QNAME(sch_ele, env, qn);
    }            
    
    ann_ele = axis2_om_util_get_first_child_element_with_uri_localname(om_ele, env,
            ele_node, "annotation", AXIS2_XML_SCHEMA_NS, &ann_node);

    if(NULL != ann_ele)
    {
        void *annotation = NULL;
        
        annotation = handle_annotation_with_element(builder, env,  ann_node);
        
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sch_ele, env, annotation);    
    }            
    
    attr_value = NULL;
    
    if(NULL != (attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "type")) )
    {
        axis2_array_list_t *args = NULL;
        axis2_char_t *namesp = NULL;
        axis2_char_t *prefix = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *type_name = NULL;
        axis2_qname_t *type_qname = NULL;
        void *type = NULL;
        
        args = axis2_tokenize(env, attr_value, ':');
        
        
        if(AXIS2_ARRAY_LIST_SIZE(args, env) > 1)
        {
            axis2_hash_t *ht_ns = NULL;
            axis2_om_namespace_t *ns = NULL;
            axis2_char_t *uri        = NULL;
            
            ht_ns = AXIS2_OM_ELEMENT_GET_NAMESPACES(om_ele, env);
            prefix = AXIS2_ARRAY_LIST_GET(args, env, 0);
                        
            ns = axis2_hash_get(ht_ns, prefix, AXIS2_HASH_KEY_STRING);
            if(NULL != ns)
                namesp = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
            else
                namesp = AXIS2_XML_SCHEMA_GET_NAMESPACE(builder_impl->schema,
                    env, prefix);
                    
            if(namesp ==  NULL)
            {
                /** TODO set error */
                return NULL;                                                                  }
        }
        else
        {
            namesp = AXIS2_XML_SCHEMA_GET_NAMESPACE(builder_impl->schema, 
                        env, "");
        
        }
        
        last_list = axis2_last_token(env, attr_value, ':');
        type_name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);
        type_qname = axis2_qname_create(env, type_name, namesp, NULL);
        AXIS2_XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE_QNAME(sch_ele, env, type_qname);
        
        type = AXIS2_XML_SCHEMA_COLLECTION_GET_TYPE_BY_QNAME(
            builder_impl->collection, env, type_qname);
            
        if(NULL != type)
        {
            AXIS2_XML_SCHEMA_COLLECTION_ADD_UNRESOLVED_TYPE(
            builder_impl->collection, env, type_qname, sch_ele);
           
        }            
        
        AXIS2_XML_SCHEMA_ELEMENT_SET_SCHEMA_TYPE(sch_ele, env, type);                                     }
    else if(NULL != (attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "ref")))
    {
        axis2_array_list_t *args = NULL;
        axis2_char_t *namesp     = NULL;
        axis2_hash_t *ht_ns      = NULL;
        
        args = axis2_tokenize(env, attr_value, ':');
        ht_ns = AXIS2_XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);
        
        if(AXIS2_ARRAY_LIST_SIZE(args, env) > 0)
        {
            axis2_char_t *result = NULL;
        
        }
    }     
     
}                                            

static axis2_status_t
populate_element_namespaces(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_hash_t* ele_to_ns_map)
{
    axis2_om_element_t *ele = NULL;
    axis2_hash_t *ns_hash = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, ele_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ele_to_ns_map, AXIS2_FAILURE);
    
    ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
    ns_hash = AXIS2_OM_ELEMENT_GET_NAMESPACES(ele, env);
    
    if(NULL != ns_hash)
    {
        axis2_hash_index_t *hi = NULL;
        for(hi = axis2_hash_first(ns_hash, env);
            hi; hi = axis2_hash_next(env, hi))
        {
            void *val = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            if(NULL != val)
            {
                axis2_om_namespace_t *ns = NULL;
                axis2_char_t *uri = NULL;
                axis2_char_t *prefix = NULL;
                ns = (axis2_om_namespace_t*)val;
                uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
                prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env);
                
                axis2_hash_set(ele_to_ns_map, AXIS2_STRDUP(prefix, env),
                    AXIS2_HASH_KEY_STRING, AXIS2_STRDUP(uri, env));                         }
        }
    }
    return AXIS2_SUCCESS;
}

static axis2_xml_schema_identity_constraint_t*
handle_constraint(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *cnst_node,
        axis2_om_node_t *schema_node,
        int type)
{
    void *constraint = NULL;
    axis2_om_element_t *cnst_ele = NULL;
    axis2_char_t *attr_value = NULL;
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    
    
    axis2_om_element_t *ele1  = NULL;
    axis2_om_node_t    *node1 = NULL;
    axis2_char_t *localname   = NULL;
    
    if(type == AXIS2_XML_SCHEMA_KEY)
    {
        constraint = axis2_xml_schema_key_create(env);
    }
    else if(type == AXIS2_XML_SCHEMA_KEYREF)
    {
        constraint = axis2_xml_schema_keyref_create(env);
    }
    else if(type == AXIS2_XML_SCHEMA_UNIQUE)
    {
        constraint = axis2_xml_schema_unique_create(env);
    }
    else 
    {    return NULL;                
                
    }   
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    cnst_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(cnst_node, env);

    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnst_ele, env, "name");
    if(NULL != attr_value)
        AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_NAME(constraint, env, attr_value);
    
    attr_value = NULL;
    
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnst_ele, env, "refer");
    
    if(NULL != attr_value)
    {
        axis2_array_list_t *ns_from_ele = NULL;
        axis2_array_list_t *last_list = NULL;
        axis2_char_t *ns = NULL;
        axis2_char_t *name = NULL;
        axis2_qname_t *qname = NULL;
        
        ns_from_ele = axis2_tokenize(env, attr_value, ':');
        /** check this */
        if(NULL != ns_from_ele && AXIS2_ARRAY_LIST_SIZE(ns_from_ele, env) > 1)
        {
            axis2_char_t *ns1 = NULL;
            
            axis2_hash_t *namespaces_ht = NULL;
            ns1 = AXIS2_ARRAY_LIST_GET(ns_from_ele, env, 0);
            
            namespaces_ht = 
            AXIS2_XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(builder_impl->schema, env);
            
            ns = axis2_hash_get(namespaces_ht, ns1, AXIS2_HASH_KEY_STRING);
        }
        else
        {
            ns = AXIS2_XML_SCHEMA_GET_TARGET_NAMESPACE(builder_impl->schema, env);
        }
            
        last_list = axis2_last_token(env, attr_value, ':');
        name = AXIS2_ARRAY_LIST_GET(last_list, env, 1);

        AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_NAME(constraint, env, name);
            
        qname = axis2_qname_create(env, name, ns, NULL); 
        
        AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_REFER(constraint, env, qname);   
    }
    
    ele1 = axis2_om_util_get_first_child_element_with_uri(cnst_node, env, 
        AXIS2_XML_SCHEMA_NS, &node1);
        
    while(NULL != ele1)
    {
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1, env);
        
        if(AXIS2_STRCMP(localname, "selector") == 0)
        {
            void *sel_xpath = NULL;
            axis2_om_element_t *ann_ele = NULL;
            axis2_om_node_t *ann_node   = NULL;
            
            axis2_char_t *attr_value = NULL;
            sel_xpath = axis2_xml_schema_xpath_create(env);
            attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele1, env, "xpath");
            
            AXIS2_XML_SCHEMA_XPATH_SET_XPATH(sel_xpath, env, attr_value);
                                
            ann_ele = axis2_om_util_get_first_child_element_with_uri_localname(ele1,
                    env, node1 , "annotation", AXIS2_XML_SCHEMA_NS, &ann_node);
            if(NULL != ann_ele)
            {
                void *annotation = handle_annotation_with_element(builder, env, ann_node);
                AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(sel_xpath, env, annotation);
                
            
            }                     
            
            AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_SELECTOR(constraint, env, sel_xpath);
        }
        else if(AXIS2_STRCMP(localname, "field") == 0)
        {
            void *field_xpath = NULL;
            void *annotation = NULL;
            axis2_om_element_t *ann_ele = NULL;
            axis2_om_node_t    *ann_node = NULL;

            axis2_xml_schema_obj_collection_t *fields = NULL;
            axis2_char_t *attr_value = NULL;
            
            
            
            field_xpath = axis2_xml_schema_xpath_create(env);
                            
            attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele1, env, "xpath");
            
            AXIS2_XML_SCHEMA_XPATH_SET_XPATH(field_xpath, env, attr_value);
            
            fields = AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_FIELDS(constraint, env);
            AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(fields, env, field_xpath);
            
            ann_ele = axis2_om_util_get_first_child_element_with_uri_localname(
                ele1, env, node1, "annotation", AXIS2_XML_SCHEMA_NS, &ann_node);
            
            if(NULL != ann_ele)
            {
                void *annotation = NULL;
                annotation = handle_annotation_with_element(builder, env, ann_node);
                AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(field_xpath, env, annotation);
            }                 
        }
        else if(AXIS2_STRCMP(localname, "annotation") == 0)
        {
            void *constraint_annotation = handle_annotation_with_element(builder, env, node1);
            AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(constraint, env, constraint_annotation);
        }
        
        
        ele1 = axis2_om_util_get_next_sibling_element_with_uri(node1, env,
                AXIS2_XML_SCHEMA_NS, &node1); 
    }
    return constraint;            
}

static axis2_xml_schema_import_t*
handle_import(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *import_node,
        axis2_om_node_t *schema_node)
{
    void *import = NULL;
    
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    
    axis2_om_element_t *ann_ele = NULL;
    axis2_om_node_t*   ann_node = NULL;
    
    axis2_om_element_t *import_ele = NULL;
    
    axis2_char_t *ns    = NULL;
    axis2_char_t *sch_location = NULL;
    
    import_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(import_node, env);
    import = axis2_xml_schema_import_create(env);
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    ann_ele = axis2_om_util_get_first_child_element_with_uri_localname(import_ele, env,
        import_node, "annotation", AXIS2_XML_SCHEMA_NS, &ann_node);
        
    if(NULL != ann_ele)
    {
        void *import_ann = NULL;
        import_ann = handle_annotation_with_element(builder, env, ann_node);
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(import, env, import_ann);
    }            

    ns = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_ele, env, "schemaLocation");
    sch_location = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_ele, env, "namespace");
    
    
    AXIS2_XML_SCHEMA_IMPORT_SET_NAMESPACE(import, env, ns);
    AXIS2_XML_SCHEMA_EXTERNAL_SET_SCHEMA_LOCATION(import, env, sch_location);
    
    if(NULL != sch_location && (AXIS2_STRCMP(sch_location, "") != 0))
    {
        axis2_char_t *source_uri = NULL;
        source_uri = AXIS2_XML_SCHEMA_OBJ_GET_SOURCE_URI(builder_impl->schema, env);
        
        if(NULL != source_uri)
        {
            void *schema = NULL;
            schema = resolve_xml_schema_with_uri(env, ns, sch_location, source_uri);
            AXIS2_XML_SCHEMA_EXTERNAL_SET_SCHEMA(import, env, schema);        
        }
        else
        {
             void *schema = NULL;
            schema = resolve_xml_schema(env, ns, sch_location);
            AXIS2_XML_SCHEMA_EXTERNAL_SET_SCHEMA(import, env, schema);
        }
    }
    return import;
}
        
static axis2_xml_schema_external_t *
handle_include(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *include_node,
        axis2_om_node_t *schema_node)
{
    void* include = NULL;
    axis2_om_element_t *inc_ele= NULL;
    axis2_om_element_t *ann_ele = NULL;
    axis2_om_node_t *ann_node = NULL;
    axis2_char_t *sch_location = NULL;
    axis2_char_t *source_uri = NULL;
    axis2_xml_schema_builder_impl_t *builder_impl = NULL;
    void *sch = NULL;
    AXIS2_PARAM_CHECK((*env)->error, include_node, NULL);
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    include = axis2_xml_schema_include_create(env);
    inc_ele = AXIS2_OM_NODE_GET_DATA_ELEMENT(include_node, env);
    ann_ele = axis2_om_util_get_first_child_element_with_uri_localname(inc_ele, env, 
        include_node, "annotation", AXIS2_XML_SCHEMA_NS, &ann_node);
    if(NULL != ann_ele)
    {
        void *include_annotation = NULL;
        include_annotation = handle_annotation_with_element(builder, env, ann_node);
        AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(include, env, include_annotation);
    }        
                
    sch_location = 
        AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(inc_ele, env, "schemaLocation");
    
    source_uri = AXIS2_XML_SCHEMA_OBJ_GET_SOURCE_URI(builder_impl->schema, env);
    if(NULL != source_uri)
    {
        sch_location = AXIS2_XML_SCHEMA_EXTERNAL_GET_SCHEMA_LOCATION(include, env);
        sch =  resolve_xml_schema_with_uri(env, NULL, sch_location, source_uri);
        AXIS2_XML_SCHEMA_EXTERNAL_SET_SCHEMA(include, env ,sch);
    }
    else
    {
        sch_location = AXIS2_XML_SCHEMA_EXTERNAL_GET_SCHEMA_LOCATION(include, env);
        sch = resolve_xml_schema(env, NULL,  sch_location);      
    }
    return include;
}

static axis2_xml_schema_annotation_t* 
handle_annotation_with_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ann_node) 
{
    axis2_xml_schema_obj_collection_t *content = NULL;
    void *app_info_obj = NULL;
    void *docs_obj     = NULL;
    
    axis2_om_element_t *ann_ele = NULL;
    
    axis2_om_element_t* app_info_ele = NULL;
    axis2_om_node_t*    app_info_node = NULL;
    
    axis2_om_element_t* doc_ele  = NULL;
    axis2_om_node_t*    doc_node = NULL;
    
    void *annotation = NULL;
    
    annotation = axis2_xml_schema_annotation_create(env);
    
    content = AXIS2_XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env);
    
    ann_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ann_node, env);
        
    app_info_ele = axis2_om_util_get_first_child_element_with_uri_localname(
            ann_ele, env, ann_node,"appinfo" ,AXIS2_XML_SCHEMA_NS, &app_info_node);
    while(NULL != app_info_ele)
    {
        app_info_obj = handle_app_info(builder, env, app_info_node);
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(content, env, app_info_obj);
        
        app_info_obj = NULL;
        app_info_ele = axis2_om_util_get_next_sibling_element_with_uri_localname(app_info_ele, env,
                app_info_node, "appinfo", AXIS2_XML_SCHEMA_NS, &app_info_node);     
    }
    
    
    doc_ele = axis2_om_util_get_first_child_element_with_uri_localname(ann_ele, env,
            ann_node, "documentation", AXIS2_XML_SCHEMA_NS, &doc_node);
    
    while(NULL != doc_ele)
    {
        docs_obj = handle_documentation(builder, env, doc_node);
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(content, env, docs_obj);               
        
        docs_obj = NULL;
        doc_ele = axis2_om_util_get_next_sibling_element_with_uri_localname(doc_ele, env,
            doc_node, "documentation", AXIS2_XML_SCHEMA_NS, &doc_node);    
    }
    return annotation;
}                
       
static axis2_xml_schema_app_info_t* 
handle_app_info(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *content)
{
    void *app_info = NULL;
    axis2_om_element_t *cnt_ele = NULL;
    axis2_om_child_element_iterator_t *ele_iter = NULL;
    axis2_char_t *attr_value = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, content, NULL);

    app_info = axis2_xml_schema_app_info_create(env);
    cnt_ele  = AXIS2_OM_NODE_GET_DATA_ELEMENT(content, env);
    
    ele_iter = axis2_om_util_get_child_elements(cnt_ele, env, content);
    
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnt_ele, env, "source");
    if(!ele_iter && !AXIS2_OM_CHILD_ELEMENT_ITERATOR_HAS_NEXT(ele_iter, env))
        return NULL;

    AXIS2_XML_SCHEMA_APP_INFO_SET_SOURCE(app_info, env, attr_value);
    AXIS2_XML_SCHEMA_APP_INFO_SET_MARKUP(app_info, env, ele_iter);
    return app_info;
}
        
static axis2_xml_schema_documentation_t*
handle_documentation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *content)
{
    void *documentation = NULL;
    axis2_om_child_element_iterator_t *ele_iter = NULL;
    axis2_om_element_t *cnt_ele = NULL;
    axis2_char_t *attr_value = NULL;
    axis2_char_t *xmllang = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, content, NULL);
    
    documentation = axis2_xml_schema_documentation_create(env);
    cnt_ele    = (axis2_om_element_t*)
        AXIS2_OM_NODE_GET_DATA_ELEMENT(content, env);
    
    ele_iter   = axis2_om_util_get_child_elements(cnt_ele, env, content);
            
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnt_ele, env, "source");
    /** TODO check this */
    xmllang    = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(cnt_ele, env, "xml:lang");
    
    if(!attr_value && !xmllang && 
    (!ele_iter || !AXIS2_OM_CHILD_ELEMENT_ITERATOR_NEXT(ele_iter, env)))
        return NULL; 
    
    AXIS2_XML_SCHEMA_DOCUMENTATION_SET_SOURCE(documentation, env, attr_value);
    AXIS2_XML_SCHEMA_DOCUMENTATION_SET_LANGUAGE(documentation, env, xmllang);
    AXIS2_XML_SCHEMA_DOCUMENTATION_SET_MARKUP(documentation, env, ele_iter);
    return documentation;
}
        
static long
get_min_occurs(
        axis2_env_t **env,
        axis2_om_node_t *ele_node)
{
    axis2_char_t *attr_value = NULL;
    axis2_om_element_t *om_ele = NULL;
    if(!ele_node || AXIS2_OM_NODE_GET_NODE_TYPE(ele_node, env) != AXIS2_OM_ELEMENT)
        return AXIS2_FAILURE;
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "maxOccurs");
    if(NULL != attr_value)
    {
        if(AXIS2_STRCMP(attr_value,"unbounded") == 0)
        {
            return LONG_MAX_VALUE;
        }
        else
            return atol(attr_value);
    }
        return -1;
}

static long
get_max_occurs(
        axis2_env_t **env,            
        axis2_om_node_t *ele_node)
{
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *attr_value = NULL;
    if(!ele_node || AXIS2_OM_NODE_GET_NODE_TYPE(ele_node, env) != AXIS2_OM_ELEMENT)
        return AXIS2_FAILURE;
    om_ele = (axis2_om_element_t*)
        AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, "maxOccurs");
    if(NULL != attr_value)
    {
        if(AXIS2_STRCMP(attr_value, "unbounded") == 0)
        {
            return LONG_MAX_VALUE;
        }
        else
        {
            return atol(attr_value);
        }
    }
    return 1;
}

static axis2_xml_schema_derivation_method_t*
get_derivation(
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t *attr_name)
{
    axis2_char_t *attr_value = NULL;
    axis2_om_element_t *om_ele = NULL;
    if(!ele_node || !attr_name)
        return NULL;
    if(AXIS2_OM_NODE_GET_NODE_TYPE(ele_node, env) != AXIS2_OM_ELEMENT)
        return NULL;
    om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, attr_name);
    if(NULL != attr_value && AXIS2_STRCMP(attr_value,"") != 0)
    {
        /** TODO trim attr_value */
        if(AXIS2_STRCMP(attr_value,"#all") == 0)
        {
            return axis2_xml_schema_derivation_method_create(env, AXIS2_XML_SCHEMA_CONST_ALL);
        }
        else
            return axis2_xml_schema_derivation_method_create(env, attr_value);
    }
    else
        return axis2_xml_schema_derivation_method_create(env, AXIS2_XML_SCHEMA_CONST_NONE);
}
        
static axis2_char_t *
get_enum_string(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_element_t *om_ele,
        axis2_char_t *attr_name)
{
    axis2_char_t *attr_value = NULL;
    if(!om_ele && !attr_name)
        return NULL;
    attr_value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, attr_name);
    if(NULL != attr_value)
    {
        /** TODO trim attr_value */
        return attr_value;
    }        
    return AXIS2_XML_SCHEMA_CONST_NONE;
}
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_resolve_xml_schema(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri){}


static axis2_bool_t 
value_exist_in_hash(
        axis2_env_t **env,
        axis2_hash_t *ht,
        axis2_char_t *value)
{
    void *val = NULL;
    axis2_hash_index_t *hi = NULL;
    if(!value || !ht)
        return AXIS2_FALSE;
    for(hi = axis2_hash_first(ht, env); hi;
        hi = axis2_hash_next( env, hi))
    {
        axis2_hash_this(hi, NULL, NULL, &val);
        if(NULL != val )
        {
            if(AXIS2_STRCMP(value, val) == 0)
                return AXIS2_TRUE;
        }
    
    }        
    return AXIS2_FALSE;    
}    

static axis2_xml_schema_form_t* 
get_from_default(
        axis2_env_t **env,
        axis2_om_node_t *ele_node, 
        axis2_char_t *attr_name)
{
    axis2_om_element_t *om_ele = NULL;
    if(NULL != ele_node &&
        AXIS2_OM_NODE_GET_NODE_TYPE(ele_node , env) == AXIS2_OM_ELEMENT)
    {
        axis2_char_t *value = NULL;
        om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ele_node, env);
        value = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(om_ele, env, attr_name); 
        if(NULL != value)
            return axis2_xml_schema_form_create(env, value);
        else
            return axis2_xml_schema_form_create(env, "unqualified");            
    }
    return NULL;
}

static axis2_xml_schema_t *
resolve_xml_schema(
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location)
{

}     


static axis2_xml_schema_t *
resolve_xml_schema_with_uri(
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri)
{

}            