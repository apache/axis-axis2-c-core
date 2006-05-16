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

#include <xml_schema/axis2_xml_schema.h>
#include <xml_schema/axis2_xml_schema_builder.h>
#include <xml_schema/axis2_xml_schema_collection.h>
#include <xml_schema/axis2_xml_schema_form.h>
#include <xml_schema/axis2_xml_schema_obj_table.h>
#include <xml_schema/axis2_xml_schema_derivation_method.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <xml_schema/axis2_validation_event_handler.h>
#include <xml_schema/axis2_xml_schema_element.h>
#include <xml_schema/axis2_xml_schema_annotation.h>
#include <xml_schema/axis2_xml_schema_redefine.h>
#include <xml_schema/axis2_xml_schema_simple_type.h>
#include <xml_schema/axis2_xml_schema_complex_type.h>
#include <xml_schema/axis2_xml_schema_simple_content.h>
#include <xml_schema/axis2_xml_schema_complex_content.h>
#include <xml_schema/axis2_xml_schema_simple_content_restriction.h>
#include <xml_schema/axis2_xml_schema_simple_content_extension.h>
#include <xml_schema/axis2_xml_schema_complex_content_restriction.h>
#include <xml_schema/axis2_xml_schema_complex_content_extension.h>
#include <xml_schema/axis2_xml_schema_attribute_group_ref.h>
#include <xml_schema/axis2_xml_schema_group_base.h>
#include <xml_schema/axis2_xml_schema_any.h>
#include <xml_schema/axis2_xml_schema_group.h>
#include <xml_schema/axis2_xml_schema_attribute_group.h>
#include <xml_schema/axis2_xml_schema_group_ref.h>
#include <xml_schema/axis2_xml_schema_identity_constraint.h>
#include <xml_schema/axis2_xml_schema_attribute.h>
#include <xml_schema/axis2_xml_schema_import.h>
#include <xml_schema/axis2_xml_schema_app_info.h>
#include <xml_schema/axis2_xml_schema_documentation.h>



typedef struct axis2_xml_schema_builder_impl 
{
    axis2_xml_schema_builder_t builder;

    axis2_om_document_t *om_doc;
    
    axis2_om_node_t *root_node;
    
    axis2_xml_schema_t *schema;
    
    axis2_xml_schema_collection_t *collection;

}axis2_xml_schema_builder_impl_t;

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
set_namespace_attribute(
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
        axis2_char_t *type);

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
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t *attr_name);
        
static axis2_char_t *
get_enum_string(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t *attr_name);
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_resolve_xml_schema(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri);
        
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
        axis2_char_t *uri){}
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_build_with_root_node(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *root,
        axis2_char_t *uri){}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_builder_free(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env){} 


static axis2_xml_schema_t*
handle_xml_schema_element(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t  *uri){}
        
static axis2_xml_schema_annotation_t* 
handle_annotation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ann_node,
        axis2_om_node_t *sch_node){}
        
static axis2_xml_schema_redefine_t*
handle_redefine(        
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *redefine_node,
        axis2_om_node_t *sch_node){}
        
static axis2_status_t
set_namespace_attribute(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_xml_schema_t *schema,
        axis2_om_node_t *sch_node){}
        
static axis2_status_t
put_namespace(  
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_char_t *prefix,
        axis2_char_t *ns){}              

static axis2_xml_schema_simple_type_t*
handle_simple_type(        
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *simple_node,
        axis2_om_node_t *sch_node){}

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
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_group_base_t*
handle_sequence(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *seq_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_any_t*
handle_any(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *any_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_group_base_t*
handle_choice(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *choice_node,
        axis2_om_node_t *schema_node){}
                   
static axis2_xml_schema_group_base_t*
handle_all(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *all_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_group_t* 
handle_group(
        axis2_xml_schema_builder_t* builder,
        axis2_env_t **env,
        axis2_om_node_t *group_node,
        axis2_om_node_t *schema_node){}
        
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
        axis2_bool_t is_global){}                                            

static axis2_status_t
populate_element_namespaces(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_hash_t* ele_to_ns_map){}

static axis2_xml_schema_identity_constraint_t*
handle_constraint(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *attr_node,
        axis2_om_node_t *schema_node,
        axis2_char_t *type){}

static axis2_xml_schema_import_t*
handle_import(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *import_node,
        axis2_om_node_t *schema_node){}
        
static axis2_xml_schema_external_t *
handle_include(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *include_node,
        axis2_om_node_t *schema_node){}
        
       
static axis2_xml_schema_app_info_t* 
handle_app_info(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *content){}
        
static axis2_xml_schema_documentation_t*
handle_documentation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *content){}
        
static long
get_min_occurs(
        axis2_env_t **env,
        axis2_om_node_t *content){}

static long
get_max_occurs(
        axis2_env_t **env,            
        axis2_om_node_t *content){}

static axis2_xml_schema_derivation_method_t*
get_derivation(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t *attr_name){}
        
static axis2_char_t *
get_enum_string(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_om_node_t *ele_node,
        axis2_char_t *attr_name){}
        
axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_builder_resolve_xml_schema(
        axis2_xml_schema_builder_t *builder,
        axis2_env_t **env,
        axis2_char_t *target_namespace,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri){}
