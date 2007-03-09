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

#include <woden_types.h>
#include <woden_documentation_element.h>
#include <woden_component_exts.h>
#include <woden_ext_element.h>
#include <woden_xml_attr.h>
#include <woden_schema.h>

typedef struct woden_types_impl woden_types_impl_t;

/**
 * @brief Types Struct Impl
 *   Axis2 Types
 */
struct woden_types_impl
{
    woden_types_t types;
    woden_obj_types_t obj_type;
    woden_documentable_t *documentable;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_char_t *f_type_system;
    axis2_array_list_t *f_schemas;
    axis2_array_list_t *schema_defs;
};

#define INTF_TO_IMPL(types) ((woden_types_impl_t *) types)

axis2_status_t AXIS2_CALL
woden_types_free(
    void *types,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_types_super_objs(
    void *types,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_types_type(
    void *types,
    const axis2_env_t *env);

woden_documentable_t *AXIS2_CALL
woden_types_get_base_impl(
    void *types,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_types_set_type_system(
    void *types,
    const axis2_env_t *env,
    axis2_char_t *type_system);

axis2_char_t *AXIS2_CALL
woden_types_get_type_system(
    void *types,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_types_add_schema(
    void *types,
    const axis2_env_t *env,
    void *schema);

axis2_status_t AXIS2_CALL
woden_types_remove_schema(
    void *types,
    const axis2_env_t *env,
    xml_schema_t *schema);

axis2_array_list_t *AXIS2_CALL
woden_types_get_schemas(
    void *types,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_types_get_schemas_with_namespace(
    void *types,
    const axis2_env_t *env,
    axis2_uri_t *namespc);

axis2_array_list_t *AXIS2_CALL
woden_types_get_inlined_schemas(
    void *types,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_types_get_imported_schemas(
    void *types,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_types_get_element_declaration(
    void *types,
    const axis2_env_t *env,
    axis2_qname_t *qname);

void *AXIS2_CALL
woden_types_get_type_definition(
    void *types,
    const axis2_env_t *env,
    axis2_qname_t *qname);

axis2_array_list_t *AXIS2_CALL
woden_types_get_referenceable_schema_defs(
    void *types,
    const axis2_env_t *env);

static axis2_array_list_t *
woden_types_get_referenceable_schema_defs_with_namespace(
    void *types,
    const axis2_env_t *env,
    axis2_char_t *namespc);

axis2_bool_t AXIS2_CALL
woden_types_is_namespace_in_scope_with_namespace_uri(
    void *types,
    const axis2_env_t *env,
    axis2_uri_t *namespc_uri);

axis2_bool_t AXIS2_CALL
woden_types_is_namespace_in_scope_with_qname(
    void *types,
    const axis2_env_t *env,
    axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL
woden_types_is_namespace_in_scope_with_namespace(
    void *types,
    const axis2_env_t *env,
    axis2_char_t *namespc);

static woden_types_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_types_free_ops(
    void *types,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_types_element(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.types_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_types_element_ops_t));
    woden_types_element_resolve_methods(&(types_impl->types.base.
            types_element), env, types_impl->methods);
    return types;
}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_documentable_element(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.types_element.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(types_impl->types.base.
            types_element.documentable_element), env,
            types_impl->methods);
    return types;
}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_documentable(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.documentable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(types_impl->types.base.
            documentable), env, types_impl->documentable,
            types_impl->methods);
    return types;
}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_wsdl_obj(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;
    void *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.documentable.base.wsdl_obj.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_obj_ops_t));
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(types_impl->documentable,
            env);
    woden_wsdl_obj_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj), env, wsdl_obj, types_impl->methods);
    return types;
}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_wsdl_component(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_component_ops_t));
    woden_wsdl_component_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj.base.wsdl_component), env,
            types_impl->methods);
    return types;
}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_attr_extensible(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.documentable.base.wsdl_obj.base.
    wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.attr_extensible),
            env, NULL, types_impl->methods);
    return types;

}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_to_element_extensible(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!types)
    {
        types_impl = (woden_types_impl_t *) create(env);
    }
    else
        types_impl = (woden_types_impl_t *) types;

    woden_types_free_ops(types, env);

    types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
    base.element_extensible.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(types_impl->types.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.element_extensible),
            env, NULL, types_impl->methods);
    return types;

}

/************************End of Woden C Internal Methods***********************/
static woden_types_t *
create(const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    types_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_types_impl_t));

    types_impl->obj_type = WODEN_TYPES;
    types_impl->super = NULL;
    types_impl->methods = NULL;
    types_impl->f_type_system = NULL;
    types_impl->f_schemas = NULL;
    types_impl->schema_defs = NULL;

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

    types_impl->types.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_types_ops_t));

    types_impl->f_schemas = axis2_array_list_create(env, 0);

    types_impl->types.ops->free = woden_types_free;
    types_impl->types.ops->super_objs = woden_types_super_objs;
    types_impl->types.ops->type = woden_types_type;
    types_impl->types.ops->get_base_impl = woden_types_get_base_impl;
    types_impl->types.ops->get_element_declaration =
        woden_types_get_element_declaration;
    types_impl->types.ops->get_type_definition =
        woden_types_get_type_definition;
    types_impl->types.ops->get_referenceable_schema_defs =
        woden_types_get_referenceable_schema_defs;
    types_impl->types.ops->is_namespace_in_scope_with_namespace_uri =
        woden_types_is_namespace_in_scope_with_namespace_uri;
    types_impl->types.ops->is_namespace_in_scope_with_qname =
        woden_types_is_namespace_in_scope_with_qname;
    types_impl->types.ops->is_namespace_in_scope_with_namespace =
        woden_types_is_namespace_in_scope_with_namespace;

    types_impl->methods = axis2_hash_make(env);
    if (!types_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(types_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_types_free);
    axis2_hash_set(types_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            woden_types_super_objs);
    axis2_hash_set(types_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_types_type);

    axis2_hash_set(types_impl->methods, "set_type_system",
            AXIS2_HASH_KEY_STRING,
            woden_types_set_type_system);
    axis2_hash_set(types_impl->methods, "get_type_system",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_type_system);
    axis2_hash_set(types_impl->methods, "add_schema",
            AXIS2_HASH_KEY_STRING,
            woden_types_add_schema);
    axis2_hash_set(types_impl->methods, "remove_schema",
            AXIS2_HASH_KEY_STRING,
            woden_types_remove_schema);
    axis2_hash_set(types_impl->methods, "get_schemas",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_schemas);
    axis2_hash_set(types_impl->methods, "get_schemas_with_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_schemas_with_namespace);
    axis2_hash_set(types_impl->methods, "get_inlined_schemas",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_inlined_schemas);
    axis2_hash_set(types_impl->methods, "get_imported_schemas",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_imported_schemas);
    axis2_hash_set(types_impl->methods, "get_element_declaration",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_element_declaration);
    axis2_hash_set(types_impl->methods, "get_type_definition",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_type_definition);
    axis2_hash_set(types_impl->methods, "get_referenceable_schema_defs",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_referenceable_schema_defs);
    axis2_hash_set(types_impl->methods, "get_referenceable_schema_defs_with_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_types_get_referenceable_schema_defs_with_namespace);
    axis2_hash_set(types_impl->methods, "is_namespace_in_scope_with_namespace_uri",
            AXIS2_HASH_KEY_STRING,
            woden_types_is_namespace_in_scope_with_namespace_uri);
    axis2_hash_set(types_impl->methods, "is_namespace_in_scope_with_qname",
            AXIS2_HASH_KEY_STRING,
            woden_types_is_namespace_in_scope_with_qname);
    axis2_hash_set(types_impl->methods, "is_namespace_in_scope_with_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_types_is_namespace_in_scope_with_namespace);


    return &(types_impl->types);
}

AXIS2_EXTERN woden_types_t * AXIS2_CALL
woden_types_create(const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    types_impl = (woden_types_impl_t *) create(env);

    types_impl->documentable = woden_documentable_create(env);

    types_impl->super = axis2_hash_make(env);
    if (!types_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(types_impl->super, "WODEN_TYPES", AXIS2_HASH_KEY_STRING,
            &(types_impl->types));
    axis2_hash_set(types_impl->super, "WODEN_DOCUMENTABLE", AXIS2_HASH_KEY_STRING,
            types_impl->documentable);

    return &(types_impl->types);
}

static axis2_status_t
woden_types_free_ops(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if (types_impl->types.base.types_element.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.
                types_element.ops);
        types_impl->types.base.types_element.ops = NULL;
    }

    if (types_impl->types.base.types_element.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.
                types_element.documentable_element.ops);
        types_impl->types.base.types_element.documentable_element.ops =
            NULL;
    }

    if (types_impl->types.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.
                documentable.ops);
        types_impl->types.base.documentable.ops =
            NULL;
    }

    if (types_impl->types.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.
                documentable.base.wsdl_obj.ops);
        types_impl->types.base.documentable.base.wsdl_obj.ops =
            NULL;
    }

    if (types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.
                documentable.base.wsdl_obj.base.wsdl_component.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_component.
        ops = NULL;
    }

    if (types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
        base.attr_extensible.ops = NULL;
    }

    if (types_impl->types.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, types_impl->types.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        types_impl->types.base.documentable.base.wsdl_obj.base.wsdl_element.
        base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_types_free(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    if (types_impl->f_schemas)
    {
        axis2_array_list_free(types_impl->f_schemas, env);
        types_impl->f_schemas = NULL;
    }

    if (types_impl->schema_defs)
    {
        axis2_array_list_free(types_impl->schema_defs, env);
        types_impl->schema_defs = NULL;
    }

    if (types_impl->f_type_system)
    {
        AXIS2_FREE(env->allocator, types_impl->f_type_system);
        types_impl->f_type_system = NULL;
    }

    if (types_impl->super)
    {
        axis2_hash_free(types_impl->super, env);
        types_impl->super = NULL;
    }

    if (types_impl->methods)
    {
        axis2_hash_free(types_impl->methods, env);
        types_impl->methods = NULL;
    }

    if (types_impl->documentable)
    {
        WODEN_DOCUMENTABLE_FREE(types_impl->documentable, env);
        types_impl->documentable = NULL;
    }

    woden_types_free_ops(types, env);

    if ((&(types_impl->types))->ops)
    {
        AXIS2_FREE(env->allocator, (&(types_impl->types))->ops);
        (&(types_impl->types))->ops = NULL;
    }

    if (types_impl)
    {
        AXIS2_FREE(env->allocator, types_impl);
        types_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_types_super_objs(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    types_impl = INTF_TO_IMPL(types);

    return types_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_types_type(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    types_impl = INTF_TO_IMPL(types);

    return types_impl->obj_type;
}

woden_documentable_t *AXIS2_CALL
woden_types_get_base_impl(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    types_impl = INTF_TO_IMPL(types);

    return types_impl->documentable;
}

axis2_status_t AXIS2_CALL
woden_types_resolve_methods(
    woden_types_t *types,
    const axis2_env_t *env,
    woden_types_t *types_impl,
    axis2_hash_t *methods)
{
    woden_types_impl_t *types_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    types_impl_l = INTF_TO_IMPL(types_impl);

    types->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    types->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    types->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    types->ops->get_element_declaration = axis2_hash_get(methods,
            "get_element_declaration", AXIS2_HASH_KEY_STRING);
    if (!types->ops->get_element_declaration && types_impl_l)
        types->ops->get_element_declaration =
            types_impl_l->types.ops->get_element_declaration;

    types->ops->get_type_definition = axis2_hash_get(methods,
            "get_type_definition", AXIS2_HASH_KEY_STRING);
    if (!types->ops->get_type_definition && types_impl_l)
        types->ops->get_type_definition =
            types_impl_l->types.ops->get_type_definition;

    types->ops->get_referenceable_schema_defs = axis2_hash_get(methods,
            "get_referenceable_schema_defs", AXIS2_HASH_KEY_STRING);
    if (!types->ops->get_referenceable_schema_defs && types_impl_l)
        types->ops->get_referenceable_schema_defs =
            types_impl_l->types.ops->get_referenceable_schema_defs;

    types->ops->is_namespace_in_scope_with_namespace_uri =
        axis2_hash_get(methods, "is_namespace_in_scope_with_namespace_uri",
                AXIS2_HASH_KEY_STRING);
    if (!types->ops->is_namespace_in_scope_with_namespace_uri && types_impl_l)
        types->ops->is_namespace_in_scope_with_namespace_uri =
            types_impl_l->types.ops->is_namespace_in_scope_with_namespace_uri;

    types->ops->is_namespace_in_scope_with_qname = axis2_hash_get(methods,
            "is_namespace_in_scope_with_qname", AXIS2_HASH_KEY_STRING);
    if (!types->ops->is_namespace_in_scope_with_qname && types_impl_l)
        types->ops->is_namespace_in_scope_with_qname =
            types_impl_l->types.ops->is_namespace_in_scope_with_qname;

    types->ops->is_namespace_in_scope_with_namespace = axis2_hash_get(methods,
            "is_namespace_in_scope_with_namespace", AXIS2_HASH_KEY_STRING);
    if (!types->ops->is_namespace_in_scope_with_namespace && types_impl_l)
        types->ops->is_namespace_in_scope_with_namespace =
            types_impl_l->types.ops->is_namespace_in_scope_with_namespace;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_types_set_type_system(
    void *types,
    const axis2_env_t *env,
    axis2_char_t *type_system)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_system, AXIS2_FAILURE);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if (types_impl->f_type_system)
    {
        AXIS2_FREE(env->allocator, types_impl->f_type_system);
    }
    types_impl->f_type_system = AXIS2_STRDUP(type_system, env);
    if (!types_impl->f_type_system)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
woden_types_get_type_system(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));
    return types_impl->f_type_system;
}

axis2_status_t AXIS2_CALL
woden_types_add_schema(
    void *types,
    const axis2_env_t *env,
    void *schema)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    return axis2_array_list_add(types_impl->f_schemas, env, schema);
}

axis2_status_t AXIS2_CALL
woden_types_remove_schema(
    void *types,
    const axis2_env_t *env,
    xml_schema_t *schema)
{
    woden_types_impl_t *types_impl = NULL;
    int index = 0;
    axis2_status_t code = AXIS2_FAILURE;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    index = axis2_array_list_index_of(types_impl->f_schemas, env, schema);
    axis2_array_list_remove(types_impl->f_schemas, env, index);
    code = AXIS2_ERROR_GET_STATUS_CODE(env->error);
    return code;
}

axis2_array_list_t *AXIS2_CALL
woden_types_get_schemas(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));
    return types_impl->f_schemas;
}

axis2_array_list_t *AXIS2_CALL
woden_types_get_schemas_with_namespace(
    void *types,
    const axis2_env_t *env,
    axis2_uri_t *namespc)
{
    woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_char_t *str_namespc = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if (namespc)
        str_namespc = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
    size = axis2_array_list_size(types_impl->f_schemas, env);
    for (i = 0; i < size; i++)
    {
        axis2_char_t *str_namespc_l = NULL;
        woden_schema_t *schema = (woden_schema_t *)
                axis2_array_list_get(types_impl->f_schemas, env, i);
        str_namespc_l = WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(schema, env);
        /* get schemas whose namespace is missing */
        if (!str_namespc && !str_namespc_l)
            axis2_array_list_add(types_impl->schema_defs, env, schema);
        else if (str_namespc && str_namespc_l && 0 == AXIS2_STRCMP(str_namespc,
                str_namespc_l))
        {
            axis2_array_list_add(types_impl->schema_defs , env, schema);
        }
    }
    return types_impl->schema_defs;
}

axis2_array_list_t *AXIS2_CALL
woden_types_get_inlined_schemas(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = axis2_array_list_size(types_impl->f_schemas, env);
    for (i = 0; i < size; i++)
    {
        woden_schema_t *s = (woden_schema_t *)
                axis2_array_list_get(types_impl->f_schemas, env, i);
        if (WODEN_INLINED_SCHEMA == WODEN_SCHEMA_TYPE(s, env))
        {
            axis2_array_list_add(types_impl->schema_defs, env, s);
        }
    }
    return types_impl->schema_defs;
}

axis2_array_list_t *AXIS2_CALL
woden_types_get_imported_schemas(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = axis2_array_list_size(types_impl->f_schemas, env);
    for (i = 0; i < size; i++)
    {
        woden_schema_t *s = (woden_schema_t *)
                axis2_array_list_get(types_impl->f_schemas, env, i);
        if (WODEN_IMPORTED_SCHEMA == WODEN_SCHEMA_TYPE(s, env))
        {
            axis2_array_list_add(types_impl->schema_defs, env, s);
        }
    }
    return types_impl->schema_defs;
}

void *AXIS2_CALL
woden_types_get_element_declaration(
    void *types,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    woden_types_impl_t *types_impl = NULL;
    xml_schema_element_t *xml_schema_el = NULL;
    axis2_array_list_t *schemas = NULL;
    axis2_hash_t *super = NULL;
    axis2_char_t *uri = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));
    /* Can't resolve the element if the QName is NULL.*/
    if (NULL == qname)
        return NULL;

    uri = AXIS2_QNAME_GET_URI(qname, env);
    schemas = woden_types_get_referenceable_schema_defs_with_namespace(
                types, env, uri);
    if (schemas)
    {
        int i = 0, size = 0;

        /* search the schemas with this qname's namespace */
        size = axis2_array_list_size(schemas, env);
        for (i = 0; i < size; i++)
        {
            xml_schema_t *xml_schema = (xml_schema_t *)
                    axis2_array_list_get(schemas, env, i);
            xml_schema_el = XML_SCHEMA_GET_ELEMENT_BY_QNAME(xml_schema,
                    env, qname);
            if (xml_schema_el)
            {
                break;
            }
        }
    }
    return xml_schema_el;
}

void *AXIS2_CALL
woden_types_get_type_definition(
    void *types,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    woden_types_impl_t *types_impl = NULL;
    xml_schema_type_t *xml_schema_type = NULL;
    axis2_array_list_t *schema_refs = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if (qname)
    {
        axis2_char_t *uri = AXIS2_QNAME_GET_URI(qname, env);
        schema_refs =
            woden_types_get_referenceable_schema_defs_with_namespace(
                types, env, uri);
        if (schema_refs)
        {
            int i = 0, size = 0;

            /* search the schemas with this qname's namespace */
            size = axis2_array_list_size(schema_refs, env);
            for (i = 0; i < size; i++)
            {
                xml_schema_t *xml_schema = (xml_schema_t *)
                        axis2_array_list_get(schema_refs, env, i);
                xml_schema_type = XML_SCHEMA_GET_TYPE_BY_QNAME(xml_schema,
                        env, qname);
                if (xml_schema_type)
                {
                    break;
                }
            }
        }
    }
    return xml_schema_type;
}

axis2_array_list_t *AXIS2_CALL
woden_types_get_referenceable_schema_defs(
    void *types,
    const axis2_env_t *env)
{
    woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    if (types_impl->schema_defs)
    {
        int i = 0, size = 0;
        size = axis2_array_list_size(types_impl->schema_defs, env);
        for (i = 0; i < size; i++)
        {
            woden_schema_t *schema = axis2_array_list_get(
                        types_impl->schema_defs, env, i);
            WODEN_SCHEMA_FREE(schema, env);
        }
        axis2_array_list_free(types_impl->schema_defs, env);
    }
    types_impl->schema_defs = axis2_array_list_create(env, 0);
    if (!types_impl->schema_defs)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    size = axis2_array_list_size(types_impl->f_schemas, env);
    for (i = 0; i < size; i++)
    {
        void *s = axis2_array_list_get(types_impl->f_schemas, env, i);
        xml_schema_t *schema_def = WODEN_SCHEMA_GET_SCHEMA_DEF(s,
                env);
        if (AXIS2_TRUE == WODEN_SCHEMA_IS_REFERENCEABLE(s, env) &&
                schema_def)
        {
            axis2_array_list_add(types_impl->schema_defs, env, schema_def);
        }
    }
    return types_impl->schema_defs;
}

static axis2_array_list_t *
woden_types_get_referenceable_schema_defs_with_namespace(
    void *types,
    const axis2_env_t *env,
    axis2_char_t *namespc)
{
    woden_types_impl_t *types_impl = NULL;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, namespc, NULL);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = axis2_array_list_size(types_impl->f_schemas, env);
    for (i = 0; i < size; i++)
    {
        woden_schema_t *schema =
            (woden_schema_t *) axis2_array_list_get(types_impl->f_schemas,
                    env, i);
        axis2_char_t *namespc_l =
            WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(schema, env);
        xml_schema_t *schema_def =
            WODEN_SCHEMA_GET_SCHEMA_DEF(schema, env);
        if (AXIS2_TRUE == WODEN_SCHEMA_IS_REFERENCEABLE(schema, env) &&
                0 == AXIS2_STRCMP(namespc, namespc_l) &&  schema_def)
        {
            axis2_array_list_add(types_impl->schema_defs, env, schema_def);
        }
    }
    return types_impl->schema_defs;
}

axis2_bool_t AXIS2_CALL
woden_types_is_namespace_in_scope_with_namespace_uri(
    void *types,
    const axis2_env_t *env,
    axis2_uri_t *namespc_uri)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_char_t *str_uri = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_PARAM_CHECK(env->error, namespc_uri, AXIS2_FAILURE);

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    str_uri = AXIS2_URI_TO_STRING(namespc_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
    return woden_types_is_namespace_in_scope_with_namespace(types, env, str_uri);
}

axis2_bool_t AXIS2_CALL
woden_types_is_namespace_in_scope_with_qname(
    void *types,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_char_t *str_uri = NULL;
    axis2_uri_t *uri = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    str_uri = AXIS2_QNAME_GET_URI(qname, env);
    uri = axis2_uri_parse_string(env, str_uri);
    return woden_types_is_namespace_in_scope_with_namespace_uri(types, env, uri);
}

axis2_bool_t AXIS2_CALL
woden_types_is_namespace_in_scope_with_namespace(
    void *types,
    const axis2_env_t *env,
    axis2_char_t *namespc)
{
    woden_types_impl_t *types_impl = NULL;
    axis2_bool_t result = AXIS2_FALSE;
    int i = 0, size = 0;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = WODEN_TYPES_SUPER_OBJS(types, env);
    types_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPES", AXIS2_HASH_KEY_STRING));

    size = axis2_array_list_size(types_impl->f_schemas, env);
    for (i = 0; i < size; i++)
    {
        axis2_char_t *namespc_l = NULL;

        woden_schema_t *s = (woden_schema_t *) axis2_array_list_get(
                    types_impl->f_schemas, env, i);
        if (!s)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_WSDL_SCHEMA_IS_NULL,
                    AXIS2_FAILURE);
            return AXIS2_FALSE;
        }
        namespc_l = WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(s, env);
        if (AXIS2_TRUE == WODEN_SCHEMA_IS_REFERENCEABLE(s, env) &&
                0 == AXIS2_STRCMP(namespc, namespc_l))
        {
            result = AXIS2_TRUE;
            break;
        }
    }
    return result;
}

