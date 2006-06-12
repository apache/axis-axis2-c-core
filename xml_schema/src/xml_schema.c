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

#define XML_SCHEMA_DEFAULT_TARGET_NS "DEFAULT"

typedef struct xml_schema_impl xml_schema_impl_t;

struct xml_schema_impl
{
    xml_schema_t schema;
    
    xml_schema_annotated_t *annotated;
    
    xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;
    
    xml_schema_form_t *attr_form_default;
    
    xml_schema_form_t *element_form_default;
    
    xml_schema_obj_table_t *attr_groups;
    
    xml_schema_obj_table_t *attrs;

    xml_schema_obj_table_t *elements;

    xml_schema_obj_table_t *groups;

    xml_schema_obj_table_t *notations;

    xml_schema_obj_table_t *schema_types;

    xml_schema_derivation_method_t *block_default;

    xml_schema_derivation_method_t *final_default;

    xml_schema_obj_collection_t *includes;

    xml_schema_obj_collection_t *items;

    axis2_bool_t is_compiled;

    axis2_char_t *target_namespace;

    axis2_char_t *version;

    axis2_hash_t *namespaces_map;

    axis2_char_t *schema_ns_prefix;

    xml_schema_collection_t *parent;
};

#define AXIS2_INTF_TO_IMPL(schema) \
        ((xml_schema_impl_t *) schema)

axis2_status_t AXIS2_CALL 
xml_schema_free(
        void *schema,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
xml_schema_super_objs(
        void *schema,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL 
xml_schema_get_type(
        void *schema,
        const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_get_base_impl(
        void *schema,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
xml_schema_get_namespace(
        void *schema,
        const axis2_env_t *env,
        axis2_char_t *prefix);

xml_schema_form_t *AXIS2_CALL
xml_schema_get_attr_form_default(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_set_attr_form_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_form_t *value);

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_attr_groups(
        void *schema,
        const axis2_env_t *env);

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_attrs(
        void *schema,
        const axis2_env_t *env);

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_get_block_default(
        void *schema,
        const axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
xml_schema_set_block_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_derivation_method_t *block_default); 

xml_schema_form_t *AXIS2_CALL
xml_schema_get_element_form_default(
        void *schema,
        const axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
xml_schema_set_element_form_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_form_t *element_form_default); 

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_elements(
        void *schema,
        const axis2_env_t *env); 

xml_schema_element_t *AXIS2_CALL
xml_schema_get_element_by_qname(
        void *schema,
        const axis2_env_t *env,
        axis2_qname_t *qname); 

xml_schema_type_t *AXIS2_CALL
xml_schema_get_type_by_qname(
        void *schema,
        const axis2_env_t *env,
        axis2_qname_t *qname); 

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_get_final_default(
        void *schema,
        const axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
xml_schema_set_final_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_derivation_method_t *final_default); 

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_groups(
        void *schema,
        const axis2_env_t *env); 

xml_schema_obj_collection_t *AXIS2_CALL
xml_schema_get_includes(
        void *schema,
        const axis2_env_t *env); 

axis2_bool_t AXIS2_CALL
xml_schema_is_compiled(
        void *schema,
        const axis2_env_t *env); 

xml_schema_obj_collection_t *AXIS2_CALL
xml_schema_get_items(
        void *schema,
        const axis2_env_t *env); 

xml_schema_obj_table_t *AXIS2_CALL 
xml_schema_get_notations(
        void *schema,
        const axis2_env_t *env); 

xml_schema_obj_table_t *AXIS2_CALL 
xml_schema_get_schema_types(
        void *schema,
        const axis2_env_t *env); 

axis2_char_t *AXIS2_CALL
xml_schema_get_target_namespace(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
xml_schema_set_target_namespace(
        void *schema,
        const axis2_env_t *env,
        axis2_char_t *target_namespace); 

axis2_char_t *AXIS2_CALL 
xml_schema_get_version(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
xml_schema_compile(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
xml_schema_write_with_out(
        void *schema,
        const axis2_env_t *env,
        void *outstream); 

axis2_status_t AXIS2_CALL 
xml_schema_write_with_writer(
        void *schema,
        void *writer); 
/*
static axis2_status_t 
serialize_internal(
        void *schema,
        const axis2_env_t *env,
        void *writer);
*/
axis2_hash_t *AXIS2_CALL 
xml_schema_get_prefix_to_namespace_map(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
xml_schema_set_prefix_to_namespace_map(
        void *schema,
        const axis2_env_t *env,
        axis2_hash_t *map);

axis2_status_t AXIS2_CALL 
xml_schema_add_type(
        void *schema,
        const axis2_env_t *env,
        xml_schema_type_t *type);

axis2_status_t AXIS2_CALL 
xml_schema_set_schema_ns_prefix(
        void *schema,
        const axis2_env_t *env,
        axis2_char_t *ns_prefix);
        

AXIS2_EXTERN xml_schema_t * AXIS2_CALL
xml_schema_create(const axis2_env_t *env,
                        axis2_char_t *namespc,
                        xml_schema_collection_t *parent)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    schema_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_impl_t));
    if(!schema_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                        
    
    schema_impl->annotated = NULL;
    schema_impl->ht_super = NULL;
    schema_impl->obj_type = XML_SCHEMA;
    schema_impl->methods = NULL;
    schema_impl->parent = parent;
    schema_impl->attr_form_default = NULL;
    schema_impl->element_form_default = NULL;
    schema_impl->block_default = NULL;
    schema_impl->final_default = NULL;
    schema_impl->items = NULL;
    schema_impl->includes = NULL;
    schema_impl->namespaces_map = NULL;
    schema_impl->elements = NULL;
    schema_impl->attr_groups = NULL;
    schema_impl->attrs = NULL;
    schema_impl->groups = NULL;
    schema_impl->notations = NULL;
    schema_impl->schema_types = NULL;
    schema_impl->target_namespace = NULL;
    schema_impl->schema_ns_prefix = NULL;
    
    if(NULL != namespc)
        schema_impl->target_namespace = AXIS2_STRDUP(namespc, env);
    if(!schema_impl->target_namespace)
    {
        schema_impl->target_namespace = 
            AXIS2_STRDUP(XML_SCHEMA_DEFAULT_TARGET_NS, env);
    }
    schema_impl->schema.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_ops_t));
    if(!schema_impl->schema.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        xml_schema_free(&(schema_impl->schema), env);
        return NULL;
    }

    schema_impl->schema.ops->free = xml_schema_free;
    schema_impl->schema.ops->super_objs = xml_schema_super_objs;
    schema_impl->schema.ops->get_type = xml_schema_get_type;
    schema_impl->schema.ops->get_base_impl = 
            xml_schema_get_base_impl;
    schema_impl->schema.ops->get_namespace = 
            xml_schema_get_namespace;
    schema_impl->schema.ops->get_attr_form_default = 
            xml_schema_get_attr_form_default;
    schema_impl->schema.ops->set_attr_form_default = 
            xml_schema_set_attr_form_default;
    schema_impl->schema.ops->get_attr_groups = 
            xml_schema_get_attr_groups;
    schema_impl->schema.ops->get_attrs = 
            xml_schema_get_attrs;
    schema_impl->schema.ops->get_block_default = 
            xml_schema_get_block_default;
    schema_impl->schema.ops->set_block_default = 
            xml_schema_set_block_default;
    schema_impl->schema.ops->get_element_form_default = 
            xml_schema_get_element_form_default;
    schema_impl->schema.ops->set_element_form_default = 
            xml_schema_set_element_form_default;
    schema_impl->schema.ops->get_elements = 
            xml_schema_get_elements;
    schema_impl->schema.ops->get_element_by_qname = 
            xml_schema_get_element_by_qname;
    schema_impl->schema.ops->get_type_by_qname = 
            xml_schema_get_type_by_qname;
    schema_impl->schema.ops->get_final_default = 
            xml_schema_get_final_default;
    schema_impl->schema.ops->set_final_default = 
            xml_schema_set_final_default;
    schema_impl->schema.ops->get_groups = 
            xml_schema_get_groups;
    schema_impl->schema.ops->get_includes = 
            xml_schema_get_includes;
    schema_impl->schema.ops->is_compiled = 
            xml_schema_is_compiled;
    schema_impl->schema.ops->get_items = 
            xml_schema_get_items;
    schema_impl->schema.ops->get_notations = 
            xml_schema_get_notations;
    schema_impl->schema.ops->get_schema_types = 
            xml_schema_get_schema_types;
    schema_impl->schema.ops->get_target_namespace = 
            xml_schema_get_target_namespace;
    schema_impl->schema.ops->set_target_namespace = 
            xml_schema_set_target_namespace;
    schema_impl->schema.ops->get_version = 
            xml_schema_get_version;
    schema_impl->schema.ops->compile = 
            xml_schema_compile;
    schema_impl->schema.ops->write_with_out = 
            xml_schema_write_with_out;
    schema_impl->schema.ops->write_with_writer = 
            xml_schema_write_with_writer;
    schema_impl->schema.ops->get_prefix_to_namespace_map = 
            xml_schema_get_prefix_to_namespace_map;
    schema_impl->schema.ops->set_prefix_to_namespace_map = 
            xml_schema_set_prefix_to_namespace_map;
    schema_impl->schema.ops->add_type = 
            xml_schema_add_type;
    schema_impl->schema.ops->set_schema_ns_prefix =
            xml_schema_set_schema_ns_prefix;
            
            
            
    /************ create objs *******************************/            
            
    schema_impl->parent = parent;
    
    schema_impl->attr_form_default = xml_schema_form_create(env,
            XML_SCHEMA_FORM_UNQUALIFIED);       
    
    schema_impl->element_form_default = xml_schema_form_create(env,
            XML_SCHEMA_FORM_UNQUALIFIED);
    
    schema_impl->block_default = xml_schema_derivation_method_create(env,
            XML_SCHEMA_CONST_NONE);
    
    schema_impl->final_default = xml_schema_derivation_method_create(env,
            XML_SCHEMA_CONST_NONE);
    
    
    schema_impl->items = xml_schema_obj_collection_create(env);
    
    schema_impl->includes = xml_schema_obj_collection_create(env);
    
    schema_impl->namespaces_map = axis2_hash_make(env);
    
    schema_impl->elements = xml_schema_obj_table_create(env);
    
    schema_impl->attr_groups = xml_schema_obj_table_create(env);
    
    schema_impl->attrs = xml_schema_obj_table_create(env);
    
    schema_impl->groups = xml_schema_obj_table_create(env);
    
    schema_impl->notations = xml_schema_obj_table_create(env);
    
    schema_impl->schema_types = xml_schema_obj_table_create(env);
    
    schema_impl->methods = axis2_hash_make(env);
    if(!schema_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            xml_schema_free);
    axis2_hash_set(schema_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            xml_schema_super_objs);
    axis2_hash_set(schema_impl->methods, "get_type", AXIS2_HASH_KEY_STRING, 
            xml_schema_get_type);
    axis2_hash_set(schema_impl->methods, "get_namespace", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_namespace);
    axis2_hash_set(schema_impl->methods, "get_attr_form_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_attr_form_default);
    axis2_hash_set(schema_impl->methods, "set_attr_form_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_set_attr_form_default);
    axis2_hash_set(schema_impl->methods, "get_attr_groups", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_attr_groups);
    axis2_hash_set(schema_impl->methods, "get_attrs", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_attrs);
    axis2_hash_set(schema_impl->methods, "get_block_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_block_default);
    axis2_hash_set(schema_impl->methods, "set_block_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_set_block_default);
    axis2_hash_set(schema_impl->methods, "get_element_form_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_element_form_default);
    axis2_hash_set(schema_impl->methods, "set_element_form_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_set_element_form_default);
    axis2_hash_set(schema_impl->methods, "get_elements", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_elements);
    axis2_hash_set(schema_impl->methods, "get_element_by_qname", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_element_by_qname);
    axis2_hash_set(schema_impl->methods, "get_type_by_qname", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_type_by_qname);
    axis2_hash_set(schema_impl->methods, "get_final_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_final_default);
    axis2_hash_set(schema_impl->methods, "set_final_default", 
            AXIS2_HASH_KEY_STRING, xml_schema_set_final_default);
    axis2_hash_set(schema_impl->methods, "get_groups", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_groups);
    axis2_hash_set(schema_impl->methods, "get_includes", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_includes);
    axis2_hash_set(schema_impl->methods, "is_compiled", 
            AXIS2_HASH_KEY_STRING, xml_schema_is_compiled);
    axis2_hash_set(schema_impl->methods, "get_items", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_items);
    axis2_hash_set(schema_impl->methods, "get_notations", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_notations);
    axis2_hash_set(schema_impl->methods, "get_schema_types", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_schema_types);
    axis2_hash_set(schema_impl->methods, "get_target_namespace", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_target_namespace);
    axis2_hash_set(schema_impl->methods, "set_target_namespace", 
            AXIS2_HASH_KEY_STRING, xml_schema_set_target_namespace);
    axis2_hash_set(schema_impl->methods, "get_version", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_version);
    axis2_hash_set(schema_impl->methods, "compile", 
            AXIS2_HASH_KEY_STRING, xml_schema_compile);
    axis2_hash_set(schema_impl->methods, "write_a_out", 
            AXIS2_HASH_KEY_STRING, xml_schema_write_with_out);
    axis2_hash_set(schema_impl->methods, "write_a_writer", 
            AXIS2_HASH_KEY_STRING, xml_schema_write_with_writer);
    axis2_hash_set(schema_impl->methods, "get_prefix_to_namespace_map", 
            AXIS2_HASH_KEY_STRING, xml_schema_get_prefix_to_namespace_map);
    axis2_hash_set(schema_impl->methods, "set_prefix_to_namespace_map", 
            AXIS2_HASH_KEY_STRING, xml_schema_set_prefix_to_namespace_map);
    axis2_hash_set(schema_impl->methods, "add_type", 
            AXIS2_HASH_KEY_STRING, xml_schema_add_type);
    
    schema_impl->annotated = xml_schema_annotated_create(env);

    schema_impl->ht_super = axis2_hash_make(env);
    if(!schema_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->ht_super, "XML_SCHEMA", 
        AXIS2_HASH_KEY_STRING, &(schema_impl->schema));
        
    axis2_hash_set(schema_impl->ht_super, "XML_SCHEMA_ANNOTATED", 
        AXIS2_HASH_KEY_STRING, schema_impl->annotated);
        
    axis2_hash_set(schema_impl->ht_super, "XML_SCHEMA_OBJ",
        AXIS2_HASH_KEY_STRING, 
        XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(schema_impl->annotated, env));
        
    status = xml_schema_annotated_resolve_methods(
            &(schema_impl->schema.base), env, schema_impl->annotated, 
            schema_impl->methods);
    
    return &(schema_impl->schema);
}

axis2_status_t AXIS2_CALL
xml_schema_free(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    if(schema_impl->methods)
    {
        axis2_hash_free(schema_impl->methods, env);
        schema_impl->methods = NULL;
    }

    if(schema_impl->ht_super)
    {
        axis2_hash_free(schema_impl->ht_super, env);
        schema_impl->ht_super = NULL;
    }

    if(schema_impl->annotated)
    {
        XML_SCHEMA_OBJ_FREE(schema_impl->annotated, env);
        schema_impl->annotated = NULL;
    }
    
    if((&(schema_impl->schema))->ops)
    {
        AXIS2_FREE(env->allocator, (&(schema_impl->schema))->ops);
        (&(schema_impl->schema))->ops = NULL;
    }

    if(schema_impl)
    {
        AXIS2_FREE(env->allocator, schema_impl);
        schema_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_types_t AXIS2_CALL
xml_schema_get_type(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    return schema_impl->obj_type;
}

axis2_hash_t *AXIS2_CALL
xml_schema_super_objs(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    return schema_impl->ht_super;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_get_base_impl(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    return schema_impl->annotated;
}

axis2_char_t *AXIS2_CALL
xml_schema_get_namespace(
        void *schema,
        const axis2_env_t *env,
        axis2_char_t *prefix)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, prefix, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
    
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }
    if(NULL != schema_impl->namespaces_map)
    {
        void *result = NULL;
        result = axis2_hash_get(schema_impl->namespaces_map, prefix,
            AXIS2_HASH_KEY_STRING);
        if(NULL != result)
            return (axis2_char_t *)result ;            
    }
    return NULL;
}

xml_schema_form_t *AXIS2_CALL
xml_schema_get_attr_form_default(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }            
    return schema_impl->element_form_default;
}

axis2_status_t AXIS2_CALL
xml_schema_set_attr_form_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_form_t *value)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return AXIS2_FAILURE;            
    }            
    if(NULL != schema_impl->attr_form_default)
    {
        /** TODO free*/
    }
    schema_impl->attr_form_default = value;
    return AXIS2_SUCCESS;
}

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_attr_groups(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
           "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;           
    }            
    return schema_impl->attr_groups;
    
}

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_attrs(
        void *schema,
        const axis2_env_t *env)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }            
    return schema_impl->attrs;
}

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_get_block_default(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }                
    return schema_impl->block_default;
}

axis2_status_t AXIS2_CALL 
xml_schema_set_block_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_derivation_method_t *block_default) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return AXIS2_FAILURE;            
    }
    
    if(NULL != schema_impl->block_default)
    {
    
    }
    schema_impl->block_default = block_default;
    return AXIS2_SUCCESS;
}

xml_schema_form_t *AXIS2_CALL
xml_schema_get_element_form_default(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }        
    return schema_impl->element_form_default;
}

axis2_status_t AXIS2_CALL 
xml_schema_set_element_form_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_form_t *element_form_default) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_PARAM_CHECK(env->error, element_form_default, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return AXIS2_FAILURE;            
    }
    if(NULL != schema_impl->element_form_default)
    {
        XML_SCHEMA_FORM_FREE(schema_impl->element_form_default, env);
        schema_impl->element_form_default = NULL;
    }
    schema_impl->element_form_default = element_form_default;
    return AXIS2_SUCCESS;
}

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_elements(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }        
    return schema_impl->elements;
}

xml_schema_element_t *AXIS2_CALL
xml_schema_get_element_by_qname(
        void *schema,
        const axis2_env_t *env,
        axis2_qname_t *qname) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }
    if(NULL != schema_impl->elements)
    {
        return XML_SCHEMA_OBJ_TABLE_GET_ITEM(
            schema_impl->elements, env, qname);
    }    
    return NULL;
}

xml_schema_type_t *AXIS2_CALL
xml_schema_get_type_by_qname(
        void *schema,
        const axis2_env_t *env,
        axis2_qname_t *qname) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error,qname, NULL);
    
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }
    if(NULL != schema_impl->schema_types)
    {
        return XML_SCHEMA_OBJ_TABLE_GET_ITEM(
            schema_impl->schema_types, env, qname);
    }
    return NULL;
}

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_get_final_default(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    if(!schema_impl)
        return NULL;        
    
    return schema_impl->final_default;
}

axis2_status_t AXIS2_CALL 
xml_schema_set_final_default(
        void *schema,
        const axis2_env_t *env,
        xml_schema_derivation_method_t *final_default) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return AXIS2_FAILURE;
    
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    if(!schema_impl)
        return AXIS2_FAILURE; 
    
    if(NULL != schema_impl->final_default)
    {
        /** TODO free */
    }
    schema_impl->final_default = final_default;
    return AXIS2_SUCCESS;
}

xml_schema_obj_table_t *AXIS2_CALL
xml_schema_get_groups(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->groups;
}

xml_schema_obj_collection_t *AXIS2_CALL
xml_schema_get_includes(void *schema,
                                const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 

    return schema_impl->includes;
}

axis2_bool_t AXIS2_CALL
xml_schema_is_compiled(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return AXIS2_FAILURE;
    
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    if(!schema_impl)
        return AXIS2_FAILURE; 
    
    return schema_impl->is_compiled;
}

xml_schema_obj_collection_t *AXIS2_CALL
xml_schema_get_items(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    
    if(!ht_super)
        return NULL;
    
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    if(!schema_impl)
        return NULL; 
    
    return schema_impl->items;        
}

xml_schema_obj_table_t *AXIS2_CALL 
xml_schema_get_notations(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    
    if(!ht_super)
        return NULL;
    
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->notations;        
}

xml_schema_obj_table_t *AXIS2_CALL 
xml_schema_get_schema_types(
        void *schema,
        const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->schema_types;  
          
}

axis2_char_t *AXIS2_CALL
xml_schema_get_target_namespace(
        void *schema,
        const axis2_env_t *env)
        
{    
    
    xml_schema_impl_t *schema_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    schema_impl = AXIS2_INTF_TO_IMPL(schema);
    
    if(!schema_impl)
        return NULL;
    return schema_impl->target_namespace;
}

axis2_status_t AXIS2_CALL 
xml_schema_set_target_namespace(void *schema,
                                        const axis2_env_t *env,
                                        axis2_char_t *target_namespace) 
{
    xml_schema_impl_t *schema_impl = NULL;
   
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return AXIS2_FAILURE;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return AXIS2_FAILURE; 
    if(NULL != schema_impl->target_namespace)
    {
        AXIS2_FREE(env->allocator, schema_impl->target_namespace);
        schema_impl->target_namespace = NULL;
    }
    
    schema_impl->target_namespace = AXIS2_STRDUP(target_namespace, env);

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
xml_schema_get_version(void *schema,
                                const axis2_env_t *env) 
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->version;        
}

axis2_status_t AXIS2_CALL 
xml_schema_compile(
        void *schema,
        const axis2_env_t *env)
{
    /** TODO */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
xml_schema_write_with_out(
        void *schema,
        const axis2_env_t *env,
        void *outstream) 
{
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env,  AXIS2_FAILURE);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);
    
    /** TODO */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
xml_schema_write_with_writer(
        void *schema,
        void *writer) 
{
    /** TODO */
    return AXIS2_SUCCESS;
}

/*
static axis2_status_t 
serialize_internal(void *schema,
                    const axis2_env_t *env,
                    void *writer)
{
    xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env,  AXIS2_FAILURE);
    ht_super = XML_SCHEMA_SUPER_OBJS(schema, env);
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    return AXIS2_SUCCESS;
}
*/
axis2_hash_t *AXIS2_CALL 
xml_schema_get_prefix_to_namespace_map(
        void *schema,
        const axis2_env_t *env) 
{

    xml_schema_impl_t *schema_impl = NULL;
    AXIS2_ENV_CHECK(env,  NULL);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);
    return schema_impl->namespaces_map;
}

axis2_status_t AXIS2_CALL 
xml_schema_set_prefix_to_namespace_map(
        void *schema,
        const axis2_env_t *env,
        axis2_hash_t *map) 
{
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, map, AXIS2_FAILURE);
    
    schema_impl = AXIS2_INTF_TO_IMPL(schema);        
    
    if(NULL != schema_impl->namespaces_map)
    {
        /** TODO */        
    }
    schema_impl->namespaces_map = map;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
xml_schema_add_type(
        void *schema,
        const axis2_env_t *env,
        xml_schema_type_t *type) 
{

    axis2_qname_t *qname = NULL;
    xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type, AXIS2_FAILURE);
    
    schema_impl = AXIS2_INTF_TO_IMPL(schema);
        
    qname = XML_SCHEMA_TYPE_GET_QNAME(type, env);
    if(NULL != qname && NULL != schema_impl->schema_types)
    {
        if(XML_SCHEMA_OBJ_TABLE_CONTAINS(schema_impl->schema_types,
             env, qname))
        {
            /* TODO set error */
            return AXIS2_FAILURE;
        }
    XML_SCHEMA_OBJ_TABLE_ADD(schema_impl->schema_types, env, qname, 
        type);    
    }    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
xml_schema_set_schema_ns_prefix(void *schema,
                            const axis2_env_t *env,
                            axis2_char_t *ns_prefix)
{
    xml_schema_impl_t *sch_impl = NULL;
    
    AXIS2_PARAM_CHECK(env->error, ns_prefix, AXIS2_FAILURE);
    
    sch_impl = AXIS2_INTF_TO_IMPL(schema);
    
    if(NULL != sch_impl->schema_ns_prefix)
    {
        AXIS2_FREE(env->allocator, sch_impl->schema_ns_prefix);
        sch_impl->schema_ns_prefix = NULL;
    }
    sch_impl->schema_ns_prefix = AXIS2_STRDUP(ns_prefix, env);
    return AXIS2_FAILURE;
}

