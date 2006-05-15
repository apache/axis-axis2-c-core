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

#include <woden/wsdl20/axis2_woden_types.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/schema/axis2_woden_schema.h>

typedef struct axis2_woden_types_impl axis2_woden_types_impl_t;

/** 
 * @brief Types Struct Impl
 *	Axis2 Types  
 */ 
struct axis2_woden_types_impl
{
    axis2_woden_types_t types;
    axis2_woden_obj_types_t obj_type;
    axis2_woden_documentable_t *documentable;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_char_t *f_type_system;
    axis2_array_list_t *f_schemas;
    axis2_array_list_t *schema_defs;
};

#define INTF_TO_IMPL(types) ((axis2_woden_types_impl_t *) types)

axis2_status_t AXIS2_CALL 
axis2_woden_types_free(
        void *types,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_types_super_objs(
        void *types,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_types_type(
        void *types,
        axis2_env_t **env);

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_types_get_base_impl(
        void *types,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_woden_types_set_type_system(
        void *types,
        axis2_env_t **env,
        axis2_char_t *type_system);

axis2_char_t *AXIS2_CALL 
axis2_woden_types_get_type_system(
        void *types,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_woden_types_add_schema(
        void *types,
        axis2_env_t **env,
        axis2_xml_schema_t *schema);

axis2_status_t AXIS2_CALL 
axis2_woden_types_remove_schema(
        void *types,
        axis2_env_t **env,
        axis2_xml_schema_t *schema);

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_schemas(
        void *types,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_types_get_schemas_with_namespace(
        void *types,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_inlined_schemas(
        void *types,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_imported_schemas(
        void *types,
        axis2_env_t **env);

void *AXIS2_CALL 
axis2_woden_types_get_element_declaration(
        void *types,
        axis2_env_t **env,
        axis2_qname_t *qname);

void *AXIS2_CALL 
axis2_woden_types_get_type_definition(
        void *types,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_referenceable_schema_defs(
        void *types,
        axis2_env_t **env);

static axis2_array_list_t * 
axis2_woden_types_get_referenceable_schema_defs_with_namespace(
        void *types,
        axis2_env_t **env,
        axis2_char_t *namespc);

axis2_bool_t AXIS2_CALL 
axis2_woden_types_is_namespace_in_scope_with_namespace_uri(
        void *types,
        axis2_env_t **env,
        axis2_url_t *namespc_uri);

axis2_bool_t AXIS2_CALL 
axis2_woden_types_is_namespace_in_scope_with_qname(
        void *types,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL 
axis2_woden_types_is_namespace_in_scope_with_namespace(
        void *types,
        axis2_env_t **env,
        axis2_char_t *namespc);

static axis2_woden_types_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_types_element(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;

    types_impl->types.base.types_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_types_element_ops_t));
    axis2_woden_types_element_resolve_methods(&(types_impl->types.base.
            types_element), env, types_impl->methods);
    return types;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_types_element_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.types_element.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                types_element.ops);
        types_impl->types.base.types_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_documentable_element(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;

    types_impl->types.base.types_element.documentable_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(types_impl->types.base.
            types_element.documentable_element), env, 
            types_impl->methods);
    return types;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_documentable_element_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.types_element.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                types_element.documentable_element.ops);
        types_impl->types.base.types_element.documentable_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_documentable(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;

    types_impl->types.base.documentable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_ops_t));
    axis2_woden_documentable_resolve_methods(&(types_impl->types.base.
            documentable), env, types_impl->documentable, 
            types_impl->methods);
    return types;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_documentable_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.documentable.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                documentable.ops);
        types_impl->types.base.documentable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_wsdl_obj(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    void *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;

    types_impl->types.base.documentable.base.wsdl_obj.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_wsdl_obj_ops_t));
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(types_impl->documentable,
            env);
    axis2_woden_wsdl_obj_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj), env, wsdl_obj, types_impl->methods);
    return types;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_wsdl_obj_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                documentable.base.wsdl_obj.ops);
        types_impl->types.base.documentable.base.wsdl_obj.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_wsdl_component(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;
    types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj.base.wsdl_component), env, 
            types_impl->methods);
    return types;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_wsdl_component_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                documentable.base.wsdl_obj.base.wsdl_component.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.
            ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_attr_extensible(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;

    types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops =
            AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.attr_extensible),
            env, NULL, types_impl->methods);
    return types;

}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_attr_extensible_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_to_element_extensible(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!types)
    {
        types_impl = (axis2_woden_types_impl_t *) create(env);
    }
    else
        types_impl = (axis2_woden_types_impl_t *) types;
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(types_impl->types.base.
                documentable.base.wsdl_obj.base.wsdl_element.base.element_extensible),
                env, NULL, types_impl->methods);
    return types;

}

axis2_status_t AXIS2_CALL
axis2_woden_types_to_element_extensible_free(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_types_t *
create(axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_types_impl_t));

    types_impl->obj_type= AXIS2_WODEN_TYPES;
    types_impl->super = NULL;
    types_impl->methods = NULL;
    types_impl->f_type_system = NULL;
    types_impl->f_schemas = NULL;
    
    types_impl->types.base.types_element.ops = NULL;
    types_impl->types.base.types_element.documentable_element.ops = 
            NULL;
    types_impl->types.base.documentable.ops = NULL;
    types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.ops = 
            NULL;
    types_impl->types.base.documentable.base.wsdl_obj.ops = NULL;
    types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
            base.attr_extensible.ops = NULL;
    types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
            base.element_extensible.ops = NULL;
    
    types_impl->types.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_types_ops_t));

    types_impl->f_schemas = axis2_array_list_create(env, 0);

    types_impl->types.ops->free = axis2_woden_types_free;
    types_impl->types.ops->super_objs = axis2_woden_types_super_objs;
    types_impl->types.ops->type = axis2_woden_types_type;
    types_impl->types.ops->get_base_impl = axis2_woden_types_get_base_impl;
    types_impl->types.ops->get_element_declaration = 
        axis2_woden_types_get_element_declaration;
    types_impl->types.ops->get_type_definition = 
        axis2_woden_types_get_type_definition;
    types_impl->types.ops->get_referenceable_schema_defs = 
        axis2_woden_types_get_referenceable_schema_defs;
    types_impl->types.ops->is_namespace_in_scope_with_namespace_uri = 
        axis2_woden_types_is_namespace_in_scope_with_namespace_uri;
    types_impl->types.ops->is_namespace_in_scope_with_qname = 
        axis2_woden_types_is_namespace_in_scope_with_qname;
    types_impl->types.ops->is_namespace_in_scope_with_namespace = 
        axis2_woden_types_is_namespace_in_scope_with_namespace;
 
    types_impl->methods = axis2_hash_make(env);
    if(!types_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(types_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_free);
    axis2_hash_set(types_impl->methods, "to_types_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_types_element_free);
    axis2_hash_set(types_impl->methods, "to_documentable_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_documentable_element_free);
    axis2_hash_set(types_impl->methods, "to_documentable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_documentable_free);
    axis2_hash_set(types_impl->methods, "to_wsdl_obj_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_wsdl_obj_free);
    axis2_hash_set(types_impl->methods, "to_wsdl_component_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_wsdl_component_free);
    axis2_hash_set(types_impl->methods, "to_attr_extensible_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_attr_extensible_free);
    axis2_hash_set(types_impl->methods, "to_element_extensible_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_to_element_extensible_free);
    axis2_hash_set(types_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_super_objs);
    axis2_hash_set(types_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_types_type);

    axis2_hash_set(types_impl->methods, "set_type_system", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_set_type_system);
    axis2_hash_set(types_impl->methods, "get_type_system", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_type_system);
    axis2_hash_set(types_impl->methods, "add_schema", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_add_schema);
    axis2_hash_set(types_impl->methods, "remove_schema", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_remove_schema);
    axis2_hash_set(types_impl->methods, "get_schemas", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_schemas);
    axis2_hash_set(types_impl->methods, "get_schemas_with_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_schemas_with_namespace);
    axis2_hash_set(types_impl->methods, "get_inlined_schemas", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_inlined_schemas);
    axis2_hash_set(types_impl->methods, "get_imported_schemas", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_imported_schemas);
    axis2_hash_set(types_impl->methods, "get_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_element_declaration);
    axis2_hash_set(types_impl->methods, "get_type_definition", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_type_definition);
    axis2_hash_set(types_impl->methods, "get_referenceable_schema_defs", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_referenceable_schema_defs);
    axis2_hash_set(types_impl->methods, "get_referenceable_schema_defs_with_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_get_referenceable_schema_defs_with_namespace);
    axis2_hash_set(types_impl->methods, "is_namespace_in_scope_with_namespace_uri", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_is_namespace_in_scope_with_namespace_uri);
    axis2_hash_set(types_impl->methods, "is_namespace_in_scope_with_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_is_namespace_in_scope_with_qname);
    axis2_hash_set(types_impl->methods, "is_namespace_in_scope_with_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_types_is_namespace_in_scope_with_namespace);
    

    return &(types_impl->types);
}

AXIS2_DECLARE(axis2_woden_types_t *)
axis2_woden_types_create(axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = (axis2_woden_types_impl_t *) create(env);

    types_impl->documentable = axis2_woden_documentable_create(env);

    types_impl->super = axis2_hash_make(env);
    if(!types_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(types_impl->super, "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING, 
            &(types_impl->types));
    axis2_hash_set(types_impl->super, "AXIS2_WODEN_DOCUMENTABLE", AXIS2_HASH_KEY_STRING, 
            types_impl->documentable);
    
    return &(types_impl->types);
}

axis2_status_t AXIS2_CALL
axis2_woden_types_free(void *types,
                        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if(types_impl->f_schemas)
    {
        AXIS2_ARRAY_LIST_FREE(types_impl->f_schemas, env);
        types_impl->f_schemas = NULL;
    }
    
    if(types_impl->schema_defs)
    {
        AXIS2_ARRAY_LIST_FREE(types_impl->schema_defs, env);
        types_impl->schema_defs = NULL;
    }

    if(types_impl->f_type_system)
    {
        AXIS2_FREE((*env)->allocator, types_impl->f_type_system);
        types_impl->f_type_system = NULL;
    }
    
    if(types_impl->super)
    {
        axis2_hash_free(types_impl->super, env);
        types_impl->super = NULL;
    }
    
    if(types_impl->methods)
    {
        axis2_hash_free(types_impl->methods, env);
        types_impl->methods = NULL;
    }

    if(types_impl->documentable)
    {
        AXIS2_WODEN_DOCUMENTABLE_FREE(types_impl->documentable, env);
        types_impl->documentable = NULL;
    }

    if(types_impl->types.base.types_element.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                types_element.ops);
        types_impl->types.base.types_element.ops = NULL;
    }

    if(types_impl->types.base.types_element.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                types_element.documentable_element.ops);
        types_impl->types.base.types_element.documentable_element.ops = 
            NULL;
    }
    
    if(types_impl->types.base.documentable.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                documentable.ops);
        types_impl->types.base.documentable.ops = 
            NULL;
    }
    
    if(types_impl->types.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                documentable.base.wsdl_obj.ops);
        types_impl->types.base.documentable.base.wsdl_obj.ops = 
            NULL;
    }
    
    if(types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.
                documentable.base.wsdl_obj.base.wsdl_component.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.
            ops = NULL;
    }

    if(types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }

    if(types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, types_impl->types.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    if((&(types_impl->types))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(types_impl->types))->ops);
        (&(types_impl->types))->ops = NULL;
    }
    
    if(types_impl)
    {
        AXIS2_FREE((*env)->allocator, types_impl);
        types_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_types_super_objs(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    return types_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_types_type(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    return types_impl->obj_type;
}

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_types_get_base_impl(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    types_impl = INTF_TO_IMPL(types);

    return types_impl->documentable;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_resolve_methods(
        axis2_woden_types_t *types,
        axis2_env_t **env,
        axis2_woden_types_t *types_impl,
        axis2_hash_t *methods)
{
    axis2_woden_types_impl_t *types_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    types_impl_l = INTF_TO_IMPL(types_impl);
    
    types->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    types->ops->to_types_free = axis2_hash_get(methods, "to_types_free", 
            AXIS2_HASH_KEY_STRING);
    types->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    types->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    types->ops->get_element_declaration = axis2_hash_get(methods, 
            "get_element_declaration", AXIS2_HASH_KEY_STRING);
    if(!types->ops->get_element_declaration && types_impl_l)
            types->ops->get_element_declaration = 
            types_impl_l->types.ops->get_element_declaration;
    
    types->ops->get_type_definition = axis2_hash_get(methods, 
            "get_type_definition", AXIS2_HASH_KEY_STRING);
    if(!types->ops->get_type_definition && types_impl_l)
            types->ops->get_type_definition = 
            types_impl_l->types.ops->get_type_definition;
    
    types->ops->get_referenceable_schema_defs = axis2_hash_get(methods, 
            "get_referenceable_schema_defs", AXIS2_HASH_KEY_STRING);
    if(!types->ops->get_referenceable_schema_defs && types_impl_l)
            types->ops->get_referenceable_schema_defs = 
            types_impl_l->types.ops->get_referenceable_schema_defs;
    
    types->ops->is_namespace_in_scope_with_namespace_uri = 
        axis2_hash_get(methods, "is_namespace_in_scope_with_namespace_uri", 
        AXIS2_HASH_KEY_STRING);
    if(!types->ops->is_namespace_in_scope_with_namespace_uri && types_impl_l)
            types->ops->is_namespace_in_scope_with_namespace_uri = 
            types_impl_l->types.ops->is_namespace_in_scope_with_namespace_uri;
    
    types->ops->is_namespace_in_scope_with_qname = axis2_hash_get(methods, 
            "is_namespace_in_scope_with_qname", AXIS2_HASH_KEY_STRING);
    if(!types->ops->is_namespace_in_scope_with_qname && types_impl_l)
            types->ops->is_namespace_in_scope_with_qname = 
            types_impl_l->types.ops->is_namespace_in_scope_with_qname;
    
    types->ops->is_namespace_in_scope_with_namespace = axis2_hash_get(methods, 
            "is_namespace_in_scope_with_namespace", AXIS2_HASH_KEY_STRING);
    if(!types->ops->is_namespace_in_scope_with_namespace && types_impl_l)
            types->ops->is_namespace_in_scope_with_namespace = 
            types_impl_l->types.ops->is_namespace_in_scope_with_namespace;
   
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_types_set_type_system(
        void *types,
        axis2_env_t **env,
        axis2_char_t *type_system)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, type_system, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if(types_impl->f_type_system)
    {
        AXIS2_FREE((*env)->allocator, types_impl->f_type_system);
    }
    types_impl->f_type_system = AXIS2_STRDUP(type_system, env);
    if(!types_impl->f_type_system)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
axis2_woden_types_get_type_system(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));
    return types_impl->f_type_system;
}

axis2_status_t AXIS2_CALL 
axis2_woden_types_add_schema(
        void *types,
        axis2_env_t **env,
        axis2_xml_schema_t *schema)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, schema, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));
        
    return AXIS2_ARRAY_LIST_ADD(types_impl->f_schemas, env, schema);
}

axis2_status_t AXIS2_CALL 
axis2_woden_types_remove_schema(
        void *types,
        axis2_env_t **env,
        axis2_xml_schema_t *schema)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    int index = 0;
    axis2_status_t code = AXIS2_FAILURE;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, schema, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    index = AXIS2_ARRAY_LIST_INDEX_OF(types_impl->f_schemas, env, schema);
    AXIS2_ARRAY_LIST_REMOVE(types_impl->f_schemas, env, index);
    code = AXIS2_ERROR_GET_STATUS_CODE((*env)->error);
    return code;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_schemas(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));
    return types_impl->f_schemas;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_types_get_schemas_with_namespace(
        void *types,
        axis2_env_t **env,
        axis2_url_t *namespc)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_char_t *str_namespc = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if(namespc)
        str_namespc = AXIS2_URL_TO_EXTERNAL_FORM(namespc, env);
    size = AXIS2_ARRAY_LIST_SIZE(types_impl->f_schemas, env);
    for(i = 0; i < size; i++)
    {
        axis2_char_t *str_namespc_l = NULL;        
        axis2_woden_schema_t *schema = (axis2_woden_schema_t *) 
            AXIS2_ARRAY_LIST_GET(types_impl->f_schemas, env, i);
        str_namespc_l = AXIS2_WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(schema, env);
        /* get schemas whose namespace is missing */
        if(!str_namespc && !str_namespc_l)
            AXIS2_ARRAY_LIST_ADD(types_impl->schema_defs, env, schema);
        else if(str_namespc && str_namespc_l && 0 == AXIS2_STRCMP(str_namespc, 
                    str_namespc_l)) 
        {
                AXIS2_ARRAY_LIST_ADD(types_impl->schema_defs ,env, schema);
        }
    }
    return types_impl->schema_defs;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_inlined_schemas(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = AXIS2_ARRAY_LIST_SIZE(types_impl->f_schemas, env);
    for(i = 0; i < size; i++)
    {
        axis2_woden_schema_t *s = (axis2_woden_schema_t *) 
            AXIS2_ARRAY_LIST_GET(types_impl->f_schemas, env, i);
        if(AXIS2_WODEN_INLINED_SCHEMA == AXIS2_WODEN_SCHEMA_TYPE(s, env))
        {
            AXIS2_ARRAY_LIST_ADD(types_impl->schema_defs, env, s);
        }
    }
    return types_impl->schema_defs;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_types_get_imported_schemas(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = AXIS2_ARRAY_LIST_SIZE(types_impl->f_schemas, env);
    for(i = 0; i < size; i++)
    {
        axis2_woden_schema_t *s = (axis2_woden_schema_t *) 
            AXIS2_ARRAY_LIST_GET(types_impl->f_schemas, env, i);
        if(AXIS2_WODEN_IMPORTED_SCHEMA == AXIS2_WODEN_SCHEMA_TYPE(s, env))
        {
            AXIS2_ARRAY_LIST_ADD(types_impl->schema_defs, env, s);
        }
    }
    return types_impl->schema_defs;
}

void *AXIS2_CALL 
axis2_woden_types_get_element_declaration(
        void *types,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_xml_schema_element_t *xml_schema_el = NULL;
    axis2_array_list_t *schemas = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));
    /* Can't resolve the element if the QName is NULL.*/
    if(NULL == qname)
      return NULL;
   
    axis2_char_t *uri = AXIS2_QNAME_GET_URI(qname, env);
    schemas = axis2_woden_types_get_referenceable_schema_defs_with_namespace(
            types, env, uri);
    if(schemas)
    {
        int i = 0, size = 0;

        /* search the schemas with this qname's namespace */
        size = AXIS2_ARRAY_LIST_SIZE(schemas, env);
        for(i = 0; i < size; i++)
        {
            axis2_xml_schema_t *xml_schema = (axis2_xml_schema_t *)
                AXIS2_ARRAY_LIST_GET(schemas, env, i);
            xml_schema_el = AXIS2_XML_SCHEMA_GET_ELEMENT_BY_QNAME(xml_schema, 
                    env, qname);
            if(xml_schema_el) 
            {
                break;
            }
        }
    }
    return xml_schema_el;
}

void *AXIS2_CALL 
axis2_woden_types_get_type_definition(
        void *types,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_xml_schema_type_t *xml_schema_type = NULL;
    axis2_array_list_t *schema_refs = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if(qname)
    {
        axis2_char_t *uri = AXIS2_QNAME_GET_URI(qname, env);
        schema_refs = 
            axis2_woden_types_get_referenceable_schema_defs_with_namespace(
            types, env, uri);
        if(schema_refs)
        {
            int i = 0, size = 0;

            /* search the schemas with this qname's namespace */
            size = AXIS2_ARRAY_LIST_SIZE(schema_refs, env);
            for(i = 0; i < size; i++)
            {
                axis2_xml_schema_t *xml_schema = (axis2_xml_schema_t *)
                    AXIS2_ARRAY_LIST_GET(schema_refs, env, i);
                xml_schema_type = AXIS2_XML_SCHEMA_GET_TYPE_BY_QNAME(xml_schema, 
                        env, qname);
                if(xml_schema_type) 
                {
                    break;
                }
            }
        }
    }
    return xml_schema_type;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_types_get_referenceable_schema_defs(
        void *types,
        axis2_env_t **env)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if(types_impl->schema_defs)
    {
        int i = 0, size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(types_impl->schema_defs, env);
        for(i = 0; i < size; i++)
        {
            axis2_woden_schema_t *schema = AXIS2_ARRAY_LIST_GET(
                    types_impl->schema_defs, env, i);
            AXIS2_WODEN_SCHEMA_FREE(schema, env);
        }
        axis2_array_list_free(types_impl->schema_defs, env);
    }
    types_impl->schema_defs = axis2_array_list_create(env, 0);
    if(!types_impl->schema_defs)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    size = AXIS2_ARRAY_LIST_SIZE(types_impl->f_schemas, env);
    for(i = 0; i < size; i++)
    {
        axis2_woden_schema_t *s = (axis2_woden_schema_t *) 
            AXIS2_ARRAY_LIST_GET(types_impl->f_schemas, env, i);
        axis2_xml_schema_t *schema_def = AXIS2_WODEN_SCHEMA_GET_SCHEMA_DEF(s, 
                env);
        if(AXIS2_TRUE == AXIS2_WODEN_SCHEMA_IS_REFERENCEABLE(s, env) &&
                NULL != schema_def)
        {
            AXIS2_ARRAY_LIST_ADD(types_impl->schema_defs, env, schema_def);
        }
    }
    return types_impl->schema_defs;
}

static axis2_array_list_t *AXIS2_CALL 
axis2_woden_types_get_referenceable_schema_defs_with_namespace(
        void *types,
        axis2_env_t **env,
        axis2_char_t *namespc)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, NULL);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));
   
    size = AXIS2_ARRAY_LIST_SIZE(types_impl->f_schemas, env);
    for(i = 0; i < size; i++)
    {
        axis2_woden_schema_t *schema = 
            (axis2_woden_schema_t *) AXIS2_ARRAY_LIST_GET(types_impl->f_schemas, 
             env, i);
        axis2_char_t *namespc_l = 
            AXIS2_WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(schema, env);
        axis2_xml_schema_t *schema_def = 
            AXIS2_WODEN_SCHEMA_GET_SCHEMA_DEF(schema, env);
        if(AXIS2_TRUE == AXIS2_WODEN_SCHEMA_IS_REFERENCEABLE(schema, env) &&
                0 == AXIS2_STRCMP(namespc, namespc_l) && NULL != schema_def)
        {
            AXIS2_ARRAY_LIST_ADD(types_impl->schema_defs, env, schema_def);
        }
    }
    return types_impl->schema_defs;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_types_is_namespace_in_scope_with_namespace_uri(
        void *types,
        axis2_env_t **env,
        axis2_url_t *namespc_uri)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, namespc_uri, AXIS2_FAILURE);
    axis2_char_t *str_uri = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    str_uri = AXIS2_URL_TO_EXTERNAL_FORM(namespc_uri, env);
    return is_namespace_in_scope_with_namespace(types, env, str_uri);
}

axis2_bool_t AXIS2_CALL 
axis2_woden_types_is_namespace_in_scope_with_qname(
        void *types,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_char_t *str_uri = NULL;
    axis2_url_t *uri = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    str_uri = AXIS2_QNAME_GET_URI(qname, env);
    uri = axis2_url_parse_string(env, str_uri);
    return is_namespace_in_scope_with_namespace_uri(types, env, uri);
}

axis2_bool_t AXIS2_CALL 
axis2_woden_types_is_namespace_in_scope_with_namespace(
        void *types,
        axis2_env_t **env,
        axis2_char_t *namespc)
{
    axis2_woden_types_impl_t *types_impl = NULL;
    axis2_bool_t result = AXIS2_FALSE;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, AXIS2_FAILURE);
    super = AXIS2_WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = AXIS2_ARRAY_LIST_SIZE(types_impl->f_schemas, env);
    for(i = 0; i < size; i++)
    {
        axis2_char_t *namespc_l = NULL;
        
        axis2_woden_schema_t *s = (axis2_woden_schema_t *) AXIS2_ARRAY_LIST_GET(
                types_impl->f_schemas, env, i);
        if(!s)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WSDL_SCHEMA_IS_NULL, 
                    AXIS2_FAILURE);
            return AXIS2_FALSE;
        }
        namespc_l = AXIS2_WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(s, env);
        if(AXIS2_TRUE ==AXIS2_WODEN_SCHEMA_IS_REFERENCEABLE(s, env) &&
                0 == AXIS2_STRCMP(namespc, namespc_l)) 
        {
            result = AXIS2_TRUE;
            break;
        }
    }
    return result;
}

