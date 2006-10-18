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

#include <woden_wsdl_obj.h>
#include <woden_import.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <woden_generic_obj.h>
#include <woden_documentable.h>

typedef struct woden_import_impl woden_import_impl_t;

/**
 * @brief Feature Struct Impl
 *   Axis2 Feature
 */
struct woden_import_impl
{
    woden_import_t import;
    woden_obj_types_t obj_type;
    woden_wsdl_ref_t *wsdl_ref;
    axis2_hash_t *super;
    axis2_hash_t *methods;

    axis2_uri_t *f_namespc;
};

#define INTF_TO_IMPL(import) ((woden_import_impl_t *) import)

axis2_status_t AXIS2_CALL
woden_import_free(
    void *import,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_import_super_objs(
    void *import,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_import_type(
    void *import,
    const axis2_env_t *env);

woden_wsdl_ref_t *AXIS2_CALL
woden_import_get_base_impl(
    void *import,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_import_set_namespace(
    void *import,
    const axis2_env_t *env,
    axis2_uri_t *ns_uri);

axis2_uri_t *AXIS2_CALL
woden_import_get_namespace(
    void *import,
    const axis2_env_t *env);

static woden_import_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_import_free_ops(
    void *import,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_import_element(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!import)
    {
        import_impl = (woden_import_impl_t *) create(env);
    }
    else
        import_impl = (woden_import_impl_t *) import;

    woden_import_free_ops(import, env);

    import_impl->import.base.import_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_import_element_ops_t));
    woden_import_element_resolve_methods(&(import_impl->import.base.
            import_element), env, import_impl->methods);
    return import;
}

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_wsdl_ref(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!import)
    {
        import_impl = (woden_import_impl_t *) create(env);
    }
    else
        import_impl = (woden_import_impl_t *) import;
    woden_import_free_ops(import, env);

    import_impl->import.base.wsdl_ref.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_ref_ops_t));
    woden_wsdl_ref_resolve_methods((woden_wsdl_ref_t *)&(import_impl->import.base.
            wsdl_ref), env, (woden_wsdl_ref_t *)import_impl->wsdl_ref, import_impl->methods);
    return import;
}

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_attr_extensible(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!import)
    {
        import_impl = (woden_import_impl_t *) create(env);
    }
    else
        import_impl = (woden_import_impl_t *) import;
    woden_import_free_ops(import, env);

    import_impl->import.base.wsdl_ref.wsdl_element.base.
    attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(import_impl->import.base.
            wsdl_ref.wsdl_element.base.attr_extensible),
            env, NULL, import_impl->methods);
    return import;

}

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_element_extensible(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!import)
    {
        import_impl = (woden_import_impl_t *) create(env);
    }
    else
        import_impl = (woden_import_impl_t *) import;
    woden_import_free_ops(import, env);
    import_impl->import.base.wsdl_ref.wsdl_element.
    base.element_extensible.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(import_impl->import.base.
            wsdl_ref.wsdl_element.base.element_extensible),
            env, NULL, import_impl->methods);
    return import;

}

/************************End of Woden C Internal Methods***********************/
static woden_import_t *
create(const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_import_impl_t));

    import_impl->obj_type = WODEN_IMPORT;
    import_impl->super = NULL;
    import_impl->methods = NULL;
    import_impl->f_namespc = NULL;

    import_impl->import.base.import_element.ops = NULL;
    import_impl->import.base.wsdl_ref.ops = NULL;
    import_impl->import.base.wsdl_ref.wsdl_element.base.attr_extensible.ops = NULL;
    import_impl->import.base.wsdl_ref.wsdl_element.base.element_extensible.ops = NULL;

    import_impl->import.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_import_ops_t));

    import_impl->import.ops->free = woden_import_free;
    import_impl->import.ops->super_objs = woden_import_super_objs;
    import_impl->import.ops->type = woden_import_type;
    import_impl->import.ops->get_base_impl = woden_import_get_base_impl;

    import_impl->import.ops->get_namespace = woden_import_get_namespace;
    import_impl->import.ops->set_namespace = woden_import_set_namespace;

    import_impl->methods = axis2_hash_make(env);
    if (!import_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(import_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_import_free);
    axis2_hash_set(import_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            woden_import_super_objs);
    axis2_hash_set(import_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_import_type);

    axis2_hash_set(import_impl->methods, "set_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_import_set_namespace);
    axis2_hash_set(import_impl->methods, "get_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_import_get_namespace);

    return &(import_impl->import);
}

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_create(const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = (woden_import_impl_t *) create(env);

    import_impl->wsdl_ref = woden_wsdl_ref_create(env);

    import_impl->super = axis2_hash_make(env);
    if (!import_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(import_impl->super, "WODEN_IMPORT",
            AXIS2_HASH_KEY_STRING, &(import_impl->import));
    axis2_hash_set(import_impl->super, "WODEN_WSDL_REF",
            AXIS2_HASH_KEY_STRING, import_impl->wsdl_ref);

    return &(import_impl->import);
}

static axis2_status_t
woden_import_free_ops(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    if (import_impl->import.base.import_element.ops)
    {
        AXIS2_FREE(env->allocator, import_impl->import.base.
                import_element.ops);
        import_impl->import.base.import_element.ops = NULL;
    }

    if (import_impl->import.base.wsdl_ref.ops)
    {
        AXIS2_FREE(env->allocator, import_impl->import.base.wsdl_ref.ops);
        import_impl->import.base.wsdl_ref.ops = NULL;
    }

    if (import_impl->import.base.wsdl_ref.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, import_impl->import.base.wsdl_ref.
                wsdl_element.base.attr_extensible.ops);
        import_impl->import.base.wsdl_ref.wsdl_element.
        base.attr_extensible.ops = NULL;
    }

    if (import_impl->import.base.wsdl_ref.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, import_impl->import.base.wsdl_ref.
                wsdl_element.base.element_extensible.ops);
        import_impl->import.base.wsdl_ref.wsdl_element.
        base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_import_free(void *import,
        const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    if (import_impl->f_namespc)
    {
        AXIS2_URI_FREE(import_impl->f_namespc, env);
        import_impl->f_namespc = NULL;
    }

    if (import_impl->super)
    {
        axis2_hash_free(import_impl->super, env);
        import_impl->super = NULL;
    }

    if (import_impl->methods)
    {
        axis2_hash_free(import_impl->methods, env);
        import_impl->methods = NULL;
    }

    if (import_impl->wsdl_ref)
    {
        WODEN_WSDL_REF_FREE(import_impl->wsdl_ref, env);
        import_impl->wsdl_ref = NULL;
    }

    woden_import_free_ops(import, env);

    if ((&(import_impl->import))->ops)
    {
        AXIS2_FREE(env->allocator, (&(import_impl->import))->ops);
        (&(import_impl->import))->ops = NULL;
    }

    if (import_impl)
    {
        AXIS2_FREE(env->allocator, import_impl);
        import_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_import_super_objs(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = INTF_TO_IMPL(import);

    return import_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_import_type(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    return import_impl->obj_type;
}

woden_wsdl_ref_t *AXIS2_CALL
woden_import_get_base_impl(void *import,
        const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = INTF_TO_IMPL(import);

    return import_impl->wsdl_ref;
}

axis2_status_t AXIS2_CALL
woden_import_resolve_methods(
    woden_import_t *import,
    const axis2_env_t *env,
    woden_import_t *import_impl,
    axis2_hash_t *methods)
{
    woden_import_impl_t *import_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    import_impl_l = INTF_TO_IMPL(import_impl);

    import->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    import->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    import->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    import->ops->get_namespace = axis2_hash_get(methods,
            "get_namespace", AXIS2_HASH_KEY_STRING);
    if (!import->ops->get_namespace && import_impl_l)
        import->ops->get_namespace =
            import_impl_l->import.ops->get_namespace;

    import->ops->set_namespace = axis2_hash_get(methods,
            "set_namespace", AXIS2_HASH_KEY_STRING);
    if (!import->ops->set_namespace && import_impl_l)
        import->ops->set_namespace =
            import_impl_l->import.ops->set_namespace;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_import_set_namespace(
    void *import,
    const axis2_env_t *env,
    axis2_uri_t *ns_uri)
{
    woden_import_impl_t *import_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns_uri, AXIS2_FAILURE);
    super = WODEN_IMPORT_SUPER_OBJS(import, env);
    import_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_IMPORT", AXIS2_HASH_KEY_STRING));

    if (import_impl->f_namespc)
    {
        AXIS2_URI_FREE(import_impl->f_namespc, env);
        import_impl->f_namespc = NULL;
    }
    import_impl->f_namespc = AXIS2_URI_CLONE(ns_uri, env);

    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_import_get_namespace(
    void *import,
    const axis2_env_t *env)
{
    woden_import_impl_t *import_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_IMPORT_SUPER_OBJS(import, env);
    import_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_IMPORT", AXIS2_HASH_KEY_STRING));

    return import_impl->f_namespc;
}

