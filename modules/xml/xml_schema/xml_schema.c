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
#include <xml_schema/axis2_xml_schema_collection.h>
#include <xml_schema/axis2_xml_schema_form.h>
#include <xml_schema/axis2_xml_schema_obj_table.h>
#include <xml_schema/axis2_xml_schema_derivation_method.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <xml_schema/axis2_validation_event_handler.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_xml_schema_impl axis2_xml_schema_impl_t;
/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_impl
{
    axis2_xml_schema_t schema;
    axis2_xml_schema_annotated_t *annotated;
    axis2_xml_schema_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_xml_schema_form_t *attr_form_default;
    axis2_xml_schema_form_t *element_form_default;
    axis2_xml_schema_obj_table_t *attr_groups;
    axis2_xml_schema_obj_table_t *attrs;
    axis2_xml_schema_obj_table_t *elements;
    axis2_xml_schema_obj_table_t *groups;
    axis2_xml_schema_obj_table_t *notations;
    axis2_xml_schema_obj_table_t *schema_types;
    axis2_xml_schema_derivation_method_t *block_default;
    axis2_xml_schema_derivation_method_t *final_default;
    axis2_xml_schema_obj_collection_t *includes;
    axis2_xml_schema_obj_collection_t *items;
    axis2_bool_t is_compiled;
    axis2_char_t *target_namespc;
    axis2_char_t *version;
    axis2_hash_t *namespaces_map;
    axis2_char_t *schema_ns_prefix;
    axis2_xml_schema_collection_t *parent;
};

#define AXIS2_INTF_TO_IMPL(schema) ((axis2_xml_schema_impl_t *) schema)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_free(void *schema,
                        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_super_objs(void *schema,
                        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_type(void *schema,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_get_base_impl(void *schema,
                                        axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_get_namespace(void *schema,
                                 axis2_env_t **env,
                                 axis2_char_t *prefix);

axis2_xml_schema_form_t *AXIS2_CALL
axis2_xml_schema_get_attr_form_default(void *schema,
                                 axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_set_attr_form_default(void *schema,
                                 axis2_env_t **env,
                                 axis2_xml_schema_form_t *value);

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_attr_groups(void *schema,
                                 axis2_env_t **env);

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_attrs(void *schema,
                                 axis2_env_t **env);

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_get_block_default(void *schema,
                                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_block_default(void *schema,
                        axis2_env_t **env,
                        axis2_xml_schema_derivation_method_t *block_default); 

axis2_xml_schema_form_t *AXIS2_CALL
axis2_xml_schema_get_element_form_default(void *schema,
                                            axis2_env_t **env); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_element_form_default(void *schema,
                            axis2_env_t **env,
                            axis2_xml_schema_form_t *element_form_default); 

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_elements(void *schema,
                                axis2_env_t **env); 

axis2_xml_schema_element_t *AXIS2_CALL
axis2_xml_schema_get_element_by_qname(void *schema,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname); 

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_get_type_by_qname(void *schema,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname); 

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_get_final_default(void *schema,
                                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_final_default(void *schema,
                        axis2_env_t **env,
                        axis2_xml_schema_derivation_method_t *final_default); 

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_groups(void *schema,
                            axis2_env_t **env); 

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_get_includes(void *schema,
                                axis2_env_t **env); 

axis2_bool_t AXIS2_CALL
axis2_xml_schema_is_compiled(void *schema,
                             axis2_env_t **env); 

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_get_items(void *schema,
                           axis2_env_t **env); 

axis2_xml_schema_obj_table_t *AXIS2_CALL 
axis2_xml_schema_get_notations(void *schema,
                                axis2_env_t **env); 

axis2_xml_schema_obj_table_t *AXIS2_CALL 
axis2_xml_schema_get_schema_types(void *schema,
                                    axis2_env_t **env); 

axis2_char_t *AXIS2_CALL
axis2_xml_schema_get_target_namespace(void *schema,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_target_namespace(void *schema,
                                        axis2_env_t **env,
                                        axis2_char_t *target_namespc); 

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_get_version(void *schema,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_compile(void *schema,
                            axis2_env_t **env,
                            axis2_validation_event_handler_t *eh);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_write_with_out(void *schema,
                                axis2_env_t **env,
                                void *outstream); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_write_with_writer(void *schema,
                                void *writer); 

static axis2_status_t 
serialize_internal(void *schema,
                    axis2_env_t **env,
                    void *writer);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_get_prefix_to_namespace_map(void *schema,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_prefix_to_namespace_map(void *schema,
                                                axis2_env_t **env,
                                                axis2_hash_t *map);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_add_type(void *schema,
                            axis2_env_t **env,
                            axis2_xml_schema_type_t *type);

AXIS2_DECLARE(axis2_xml_schema_t *)
axis2_xml_schema_create(axis2_env_t **env,
                                axis2_char_t *namespc,
                                axis2_xml_schema_collection_t *parent)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    schema_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_impl_t));

    schema_impl->annotated = NULL;
    schema_impl->super = NULL;
    schema_impl->obj_type = AXIS2_XML_SCHEMA;
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
    
    schema_impl->schema.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_ops_t));

    schema_impl->schema.ops->free = axis2_xml_schema_free;
    schema_impl->schema.ops->super_objs = axis2_xml_schema_super_objs;
    schema_impl->schema.ops->type = axis2_xml_schema_type;
    schema_impl->schema.ops->get_base_impl = 
            axis2_xml_schema_get_base_impl;
    schema_impl->schema.ops->get_namespace = 
            axis2_xml_schema_get_namespace;
    schema_impl->schema.ops->get_attr_form_default = 
            axis2_xml_schema_get_attr_form_default;
    schema_impl->schema.ops->set_attr_form_default = 
            axis2_xml_schema_set_attr_form_default;
    schema_impl->schema.ops->get_attr_groups = 
            axis2_xml_schema_get_attr_groups;
    schema_impl->schema.ops->get_attrs = 
            axis2_xml_schema_get_attrs;
    schema_impl->schema.ops->get_block_default = 
            axis2_xml_schema_get_block_default;
    schema_impl->schema.ops->set_block_default = 
            axis2_xml_schema_set_block_default;
    schema_impl->schema.ops->get_element_form_default = 
            axis2_xml_schema_get_element_form_default;
    schema_impl->schema.ops->set_element_form_default = 
            axis2_xml_schema_set_element_form_default;
    schema_impl->schema.ops->get_elements = 
            axis2_xml_schema_get_elements;
    schema_impl->schema.ops->get_element_by_qname = 
            axis2_xml_schema_get_element_by_qname;
    schema_impl->schema.ops->get_type_by_qname = 
            axis2_xml_schema_get_type_by_qname;
    schema_impl->schema.ops->get_final_default = 
            axis2_xml_schema_get_final_default;
    schema_impl->schema.ops->set_final_default = 
            axis2_xml_schema_set_final_default;
    schema_impl->schema.ops->get_groups = 
            axis2_xml_schema_get_groups;
    schema_impl->schema.ops->get_includes = 
            axis2_xml_schema_get_includes;
    schema_impl->schema.ops->is_compiled = 
            axis2_xml_schema_is_compiled;
    schema_impl->schema.ops->get_items = 
            axis2_xml_schema_get_items;
    schema_impl->schema.ops->get_notations = 
            axis2_xml_schema_get_notations;
    schema_impl->schema.ops->get_schema_types = 
            axis2_xml_schema_get_schema_types;
    schema_impl->schema.ops->get_target_namespace = 
            axis2_xml_schema_get_target_namespace;
    schema_impl->schema.ops->set_target_namespace = 
            axis2_xml_schema_set_target_namespace;
    schema_impl->schema.ops->get_version = 
            axis2_xml_schema_get_version;
    schema_impl->schema.ops->compile = 
            axis2_xml_schema_compile;
    schema_impl->schema.ops->write_with_out = 
            axis2_xml_schema_write_with_out;
    schema_impl->schema.ops->write_with_writer = 
            axis2_xml_schema_write_with_writer;
    schema_impl->schema.ops->get_prefix_to_namespace_map = 
            axis2_xml_schema_get_prefix_to_namespace_map;
    schema_impl->schema.ops->set_prefix_to_namespace_map = 
            axis2_xml_schema_set_prefix_to_namespace_map;
    schema_impl->schema.ops->add_type = 
            axis2_xml_schema_add_type;
   
    schema_impl->methods = axis2_hash_make(env);
    if(!schema_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_free);
    axis2_hash_set(schema_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_super_objs);
    axis2_hash_set(schema_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_type);
    axis2_hash_set(schema_impl->methods, "get_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_namespace);
    axis2_hash_set(schema_impl->methods, "get_attr_form_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_attr_form_default);
    axis2_hash_set(schema_impl->methods, "set_attr_form_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_set_attr_form_default);
    axis2_hash_set(schema_impl->methods, "get_attr_groups", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_attr_groups);
    axis2_hash_set(schema_impl->methods, "get_attrs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_attrs);
    axis2_hash_set(schema_impl->methods, "get_block_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_block_default);
    axis2_hash_set(schema_impl->methods, "set_block_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_set_block_default);
    axis2_hash_set(schema_impl->methods, "get_element_form_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_element_form_default);
    axis2_hash_set(schema_impl->methods, "set_element_form_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_set_element_form_default);
    axis2_hash_set(schema_impl->methods, "get_elements", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_elements);
    axis2_hash_set(schema_impl->methods, "get_element_by_qname", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_element_by_qname);
    axis2_hash_set(schema_impl->methods, "get_type_by_qname", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_type_by_qname);
    axis2_hash_set(schema_impl->methods, "get_final_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_final_default);
    axis2_hash_set(schema_impl->methods, "set_final_default", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_set_final_default);
    axis2_hash_set(schema_impl->methods, "get_groups", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_groups);
    axis2_hash_set(schema_impl->methods, "get_includes", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_includes);
    axis2_hash_set(schema_impl->methods, "is_compiled", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_is_compiled);
    axis2_hash_set(schema_impl->methods, "get_items", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_items);
    axis2_hash_set(schema_impl->methods, "get_notations", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_notations);
    axis2_hash_set(schema_impl->methods, "get_schema_types", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_schema_types);
    axis2_hash_set(schema_impl->methods, "get_target_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_target_namespace);
    axis2_hash_set(schema_impl->methods, "set_target_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_set_target_namespace);
    axis2_hash_set(schema_impl->methods, "get_version", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_version);
    axis2_hash_set(schema_impl->methods, "compile", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_compile);
    axis2_hash_set(schema_impl->methods, "write_a_out", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_write_with_out);
    axis2_hash_set(schema_impl->methods, "write_a_writer", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_write_with_writer);
    axis2_hash_set(schema_impl->methods, "get_prefix_to_namespace_map", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_get_prefix_to_namespace_map);
    axis2_hash_set(schema_impl->methods, "set_prefix_to_namespace_map", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_set_prefix_to_namespace_map);
    axis2_hash_set(schema_impl->methods, "add_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_add_type);
    
    schema_impl->annotated = axis2_xml_schema_annotated_create(env);

    schema_impl->super = axis2_hash_make(env);
    if(!schema_impl->super)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->super, "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING, 
            &(schema_impl->schema));

    status = axis2_xml_schema_annotated_resolve_methods(
            &(schema_impl->schema.base), env, schema_impl->annotated, 
            schema_impl->methods);
    
    return &(schema_impl->schema);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_free(void *schema,
                                    axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    if(schema_impl->methods)
    {
        axis2_hash_free(schema_impl->methods, env);
        schema_impl->methods = NULL;
    }

    if(schema_impl->super)
    {
        axis2_hash_free(schema_impl->super, env);
        schema_impl->super = NULL;
    }

    if(schema_impl->annotated)
    {
        AXIS2_XML_SCHEMA_OBJ_FREE(schema_impl->annotated, env);
        schema_impl->annotated = NULL;
    }
    
    if((&(schema_impl->schema))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(schema_impl->schema))->ops);
        (&(schema_impl->schema))->ops = NULL;
    }

    if(schema_impl)
    {
        AXIS2_FREE((*env)->allocator, schema_impl);
        schema_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_type(void *schema,
                                axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    return schema_impl->obj_type;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_super_objs(void *schema,
                                axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    return schema_impl->super;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_get_base_impl(void *schema,
                                axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = AXIS2_INTF_TO_IMPL(schema);

    return schema_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_resolve_methods(
                                axis2_xml_schema_t *schema,
                                axis2_env_t **env,
                                axis2_xml_schema_t *schema_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_impl_t *schema_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, schema_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    schema_impl_l = (axis2_xml_schema_impl_t *) schema_impl;
    
    schema->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_ops_t));
    schema->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    schema->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    schema->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    schema->ops->get_namespace = axis2_hash_get(methods, "get_namespace",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_namespace)
            schema->ops->get_namespace = 
            schema_impl_l->schema.ops->get_namespace;
    
    schema->ops->get_attr_form_default = axis2_hash_get(methods, 
            "get_attr_form_default", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_attr_form_default)
            schema->ops->get_attr_form_default = 
            schema_impl_l->schema.ops->get_attr_form_default;
    
    schema->ops->set_attr_form_default = axis2_hash_get(methods, 
            "set_attr_form_default", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_attr_form_default)
            schema->ops->set_attr_form_default = 
            schema_impl_l->schema.ops->set_attr_form_default;
    
    schema->ops->get_attr_groups = axis2_hash_get(methods, 
            "get_attr_groups", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_attr_groups)
            schema->ops->get_attr_groups = 
            schema_impl_l->schema.ops->get_attr_groups;
    
    schema->ops->get_attrs = axis2_hash_get(methods, "get_attrs",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_attrs)
            schema->ops->get_attrs = 
            schema_impl_l->schema.ops->get_attrs;
    
    schema->ops->get_block_default = axis2_hash_get(methods, "get_block_default",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_block_default)
            schema->ops->get_block_default = 
            schema_impl_l->schema.ops->get_block_default;
    
    schema->ops->set_block_default = axis2_hash_get(methods, "set_block_default",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_block_default)
            schema->ops->set_block_default = 
            schema_impl_l->schema.ops->set_block_default;
    
    schema->ops->get_element_form_default = axis2_hash_get(methods, 
            "get_element_form_default", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_element_form_default)
            schema->ops->get_element_form_default = 
            schema_impl_l->schema.ops->get_element_form_default;
    
    schema->ops->set_element_form_default = axis2_hash_get(methods, 
            "set_element_form_default", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_element_form_default)
            schema->ops->set_element_form_default = 
            schema_impl_l->schema.ops->set_element_form_default;
    
    schema->ops->get_elements = axis2_hash_get(methods, "get_elements",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_elements)
            schema->ops->get_elements = schema_impl_l->schema.ops->get_elements;
    
    schema->ops->get_element_by_qname = axis2_hash_get(methods, 
            "get_element_by_qname", AXIS2_HASH_KEY_STRING);        
    if(!schema->ops->get_element_by_qname)
    schema->ops->get_element_by_qname = 
            schema_impl_l->schema.ops->get_element_by_qname;
    
    schema->ops->get_type_by_qname = axis2_hash_get(methods, 
            "get_type_by_qname", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_type_by_qname)
            schema->ops->get_type_by_qname = 
            schema_impl_l->schema.ops->get_type_by_qname;
    
    schema->ops->get_final_default = axis2_hash_get(methods, "get_final_default",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_final_default)
            schema->ops->get_final_default = 
            schema_impl_l->schema.ops->get_final_default;
    
    schema->ops->set_final_default = axis2_hash_get(methods, "set_final_default",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_final_default)
            schema->ops->set_final_default = 
            schema_impl_l->schema.ops->set_final_default;
    
    schema->ops->get_groups = axis2_hash_get(methods, "get_groups",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_groups)
            schema->ops->get_groups = 
            schema_impl_l->schema.ops->get_groups;
    
    schema->ops->get_includes = axis2_hash_get(methods, "get_includes",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_includes)
            schema->ops->get_includes = 
            schema_impl_l->schema.ops->get_includes;
    
    schema->ops->is_compiled = axis2_hash_get(methods, "is_compiled",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->is_compiled)
            schema->ops->is_compiled = 
            schema_impl_l->schema.ops->is_compiled;
    
    schema->ops->get_items = axis2_hash_get(methods, "get_items",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_items)
            schema->ops->get_items = 
            schema_impl_l->schema.ops->get_items;
    
    schema->ops->get_notations = axis2_hash_get(methods, "get_notations",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_notations)
            schema->ops->get_notations = 
            schema_impl_l->schema.ops->get_notations;
    
    schema->ops->get_schema_types = axis2_hash_get(methods, "get_schema_types",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_schema_types)
            schema->ops->get_schema_types = 
            schema_impl_l->schema.ops->get_schema_types;
    
    schema->ops->get_target_namespace = axis2_hash_get(methods, 
            "get_target_namespace", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_target_namespace)
            schema->ops->get_target_namespace = 
            schema_impl_l->schema.ops->get_target_namespace;
    
    schema->ops->set_target_namespace = axis2_hash_get(methods, 
            "set_target_namespace", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_target_namespace)
            schema->ops->set_target_namespace = 
            schema_impl_l->schema.ops->set_target_namespace;
    
    schema->ops->get_version = axis2_hash_get(methods, "get_version",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_version)
            schema->ops->get_version = 
            schema_impl_l->schema.ops->get_version;
    
    schema->ops->compile = axis2_hash_get(methods, "compile",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->compile)
            schema->ops->compile = 
            schema_impl_l->schema.ops->compile;
    
    schema->ops->write_with_out = axis2_hash_get(methods, "write_with_out",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->write_with_out)
            schema->ops->write_with_out = 
            schema_impl_l->schema.ops->write_with_out;
    
    schema->ops->write_with_writer = axis2_hash_get(methods, "write_with_writer",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->write_with_writer)
            schema->ops->write_with_writer = 
            schema_impl_l->schema.ops->write_with_writer;
    
    schema->ops->get_prefix_to_namespace_map = axis2_hash_get(methods, 
            "get_prefix_to_namespace_map", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_prefix_to_namespace_map)
            schema->ops->get_prefix_to_namespace_map = 
            schema_impl_l->schema.ops->get_prefix_to_namespace_map;
    
    schema->ops->set_prefix_to_namespace_map = axis2_hash_get(methods, 
            "set_prefix_to_namespace_map", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_prefix_to_namespace_map)
            schema->ops->set_prefix_to_namespace_map = 
            schema_impl_l->schema.ops->set_prefix_to_namespace_map;
    
    schema->ops->add_type = axis2_hash_get(methods, "add_type",
            AXIS2_HASH_KEY_STRING);
    if(!schema->ops->add_type)
            schema->ops->add_type = 
            schema_impl_l->schema.ops->add_type;
    
    return axis2_xml_schema_annotated_resolve_methods(&(schema->base), 
            env, schema_impl_l->annotated, methods);
}

axis2_char_t *AXIS2_CALL
axis2_xml_schema_get_namespace(void *schema,
                                 axis2_env_t **env,
                                 axis2_char_t *prefix)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, prefix, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
    
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
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

axis2_xml_schema_form_t *AXIS2_CALL
axis2_xml_schema_get_attr_form_default(void *schema,
                                 axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }            
    return schema_impl->element_form_default;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_set_attr_form_default(void *schema,
                                 axis2_env_t **env,
                                 axis2_xml_schema_form_t *value)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
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

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_attr_groups(void *schema,
                                 axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
           "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;           
    }            
    return schema_impl->attr_groups;
    
}

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_attrs(void *schema,
                                 axis2_env_t **env)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }            
    return schema_impl->attrs;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_get_block_default(void *schema,
                                    axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }                
    return schema_impl->block_default;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_block_default(void *schema,
                        axis2_env_t **env,
                        axis2_xml_schema_derivation_method_t *block_default) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return AXIS2_FAILURE;            
    }
    
    if(NULL != schema_impl->block_default)
    {
    
    }
    schema_impl->block_default = block_default;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_form_t *AXIS2_CALL
axis2_xml_schema_get_element_form_default(void *schema,
                                            axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }        
    return schema_impl->element_form_default;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_element_form_default(void *schema,
                            axis2_env_t **env,
                            axis2_xml_schema_form_t *element_form_default) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_PARAM_CHECK((*env)->error, element_form_default, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return AXIS2_FAILURE;            
    }
    schema_impl->element_form_default = element_form_default;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_elements(void *schema,
                                axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }        
    return schema_impl->elements;
}

axis2_xml_schema_element_t *AXIS2_CALL
axis2_xml_schema_get_element_by_qname(void *schema,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);

    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }
    if(NULL != schema_impl->elements)
    {
        return AXIS2_XML_SCHEMA_OBJ_TABLE_GET_ITEM(
            schema_impl->elements, env, qname);
    }    
    return NULL;
}

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_get_type_by_qname(void *schema,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error,qname, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(NULL != ht_super)
    {
        schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        if(!schema_impl)
            return NULL;            
    }
    if(NULL != schema_impl->schema_types)
    {
        return AXIS2_XML_SCHEMA_OBJ_TABLE_GET_ITEM(
            schema_impl->schema_types, env, qname);
    }
    return NULL;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_get_final_default(void *schema,
                                    axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL;        
    return schema_impl->final_default;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_final_default(void *schema,
                        axis2_env_t **env,
                        axis2_xml_schema_derivation_method_t *final_default) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return AXIS2_FAILURE;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return AXIS2_FAILURE; 
    if(NULL != schema_impl->final_default)
    {
    }
    schema_impl->final_default = final_default;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_obj_table_t *AXIS2_CALL
axis2_xml_schema_get_groups(void *schema,
                            axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->groups;
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_get_includes(void *schema,
                                axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 

    return schema_impl->includes;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_is_compiled(void *schema,
                             axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return AXIS2_FAILURE;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return AXIS2_FAILURE; 
    return schema_impl->is_compiled;
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_get_items(void *schema,
                           axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->items;        
}

axis2_xml_schema_obj_table_t *AXIS2_CALL 
axis2_xml_schema_get_notations(void *schema,
                                axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->notations;        
}

axis2_xml_schema_obj_table_t *AXIS2_CALL 
axis2_xml_schema_get_schema_types(void *schema,
                                    axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->schema_types;  
          
}

axis2_char_t *AXIS2_CALL
axis2_xml_schema_get_target_namespace(void *schema,
                                        axis2_env_t **env) 
{    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->target_namespc;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_target_namespace(void *schema,
                                        axis2_env_t **env,
                                        axis2_char_t *target_namespc) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return AXIS2_FAILURE;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return AXIS2_FAILURE; 
    if(NULL != schema_impl->target_namespc)
    {
        AXIS2_FREE((*env)->allocator, schema_impl->target_namespc);
        schema_impl->target_namespc = NULL;
    }
    
    schema_impl->target_namespc = AXIS2_STRDUP(target_namespc, env);

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_get_version(void *schema,
                                axis2_env_t **env) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    if(!ht_super)
        return NULL;
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    if(!schema_impl)
        return NULL; 
    return schema_impl->version;        
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_compile(void *schema,
                            axis2_env_t **env,
                            axis2_validation_event_handler_t *veh) 
{
    /** TODO */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_write_with_out(void *schema,
                                axis2_env_t **env,
                                void *outstream) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env,  AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_write_with_writer(void *schema,
                        void *writer) 
{
    return AXIS2_SUCCESS;
}

static axis2_status_t 
serialize_internal(void *schema,
                    axis2_env_t **env,
                    void *writer)
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env,  AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_get_prefix_to_namespace_map(void *schema,
                                                axis2_env_t **env) 
{

    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env,  NULL);
    super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
    
    return NULL;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_set_prefix_to_namespace_map(void *schema,
                                                axis2_env_t **env,
                                                axis2_hash_t *map) 
{
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, map, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);
    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        
    if(NULL != schema_impl->namespaces_map)
    {
        /** TODO */        
    }
    schema_impl->namespaces_map = map;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_add_type(void *schema,
                            axis2_env_t **env,
                            axis2_xml_schema_type_t *type) 
{

    axis2_qname_t *qname = NULL;
    axis2_xml_schema_impl_t *schema_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, type, AXIS2_FAILURE);
    
    super = AXIS2_XML_SCHEMA_SUPER_OBJS(schema, env);

    schema_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
        "AXIS2_XML_SCHEMA", AXIS2_HASH_KEY_STRING));
        
    qname = AXIS2_XML_SCHEMA_TYPE_GET_QNAME(type, env);
    if(NULL != qname && NULL != schema_impl->schema_types)
    {
        if(AXIS2_XML_SCHEMA_OBJ_TABLE_CONTAINS(schema_impl->schema_types,
             env, qname))
        {
            /* TODO set error */
            return AXIS2_FAILURE;
        }
    AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(schema_impl->schema_types, env, qname, 
        type);    
    }    
    return AXIS2_SUCCESS;
}
