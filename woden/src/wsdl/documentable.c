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

#include <woden_documentable.h>
#include <woden_documentation_element.h>
#include <woden_wsdl_obj.h>

typedef struct woden_documentable_impl woden_documentable_impl_t;

/**
 * @brief Documentable Struct Impl
 *   Axis2 Documentable
 */
struct woden_documentable_impl
{
    woden_documentable_t documentable;
    woden_wsdl_obj_t *wsdl_obj;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    axis2_array_list_t *f_doc_elems;
};

#define INTF_TO_IMPL(documentable) ((woden_documentable_impl_t *) documentable)

axis2_status_t AXIS2_CALL
woden_documentable_free(
    void *documentable,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_documentable_super_objs(
    void *documentable,
    const axis2_env_t *env);

woden_wsdl_obj_t *AXIS2_CALL
woden_documentable_get_base_impl(
    void *documentable,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_documentable_add_documentation_element(
    void *documentable,
    const axis2_env_t *env,
    woden_documentation_element_t *documentation);

axis2_array_list_t *AXIS2_CALL
woden_documentable_get_documentation_elements(
    void *documentable,
    const axis2_env_t *env);

static woden_documentable_t *
create(
    const axis2_env_t *env);

static axis2_status_t
woden_documentable_free_ops(
    void *documentable,
    const axis2_env_t *env);

AXIS2_EXTERN woden_documentable_t * AXIS2_CALL
woden_documentable_to_documentable_element(
    void *documentable,
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!documentable)
    {
        documentable_impl = (woden_documentable_impl_t *) create(env);
    }
    else
        documentable_impl = (woden_documentable_impl_t *) documentable;

    woden_documentable_free_ops(documentable, env);

    documentable_impl->documentable.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(documentable_impl->
            documentable.base.documentable_element), env,
            documentable_impl->methods);
    return documentable;
}


static woden_documentable_t *
create(
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentable_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_impl_t));

    documentable_impl->f_doc_elems = NULL;
    documentable_impl->super = NULL;

    documentable_impl->documentable.base.documentable_element.
    ops = NULL;
    documentable_impl->documentable.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_ops_t));

    documentable_impl->methods = axis2_hash_make(env);
    if (!documentable_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    documentable_impl->documentable.ops->free = woden_documentable_free;
    documentable_impl->documentable.ops->super_objs = woden_documentable_super_objs;
    documentable_impl->documentable.ops->get_base_impl = woden_documentable_get_base_impl;
    documentable_impl->documentable.ops->add_documentation_element =
        woden_documentable_add_documentation_element;
    documentable_impl->documentable.ops->get_documentation_elements =
        woden_documentable_get_documentation_elements;

    return &(documentable_impl->documentable);
}

AXIS2_EXTERN woden_documentable_t * AXIS2_CALL
woden_documentable_create(
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentable_impl = (woden_documentable_impl_t *) create(env);

    documentable_impl->wsdl_obj = woden_wsdl_obj_create(env);

    documentable_impl->super = axis2_hash_make(env);
    if (!documentable_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentable_impl->super, "WODEN_DOCUMENTABLE",
            AXIS2_HASH_KEY_STRING, &(documentable_impl->documentable));
    axis2_hash_set(documentable_impl->super, "WODEN_WSDL_OBJ",
            AXIS2_HASH_KEY_STRING, documentable_impl->wsdl_obj);


    return &(documentable_impl->documentable);
}

static axis2_status_t
woden_documentable_free_ops(
    void *documentable,
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentable_impl = INTF_TO_IMPL(documentable);

    if (documentable_impl->documentable.base.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, documentable_impl->documentable.base.
                documentable_element.ops);
        documentable_impl->documentable.base.documentable_element.ops =
            NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_documentable_free(
    void *documentable,
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentable_impl = INTF_TO_IMPL(documentable);

    if (documentable_impl->f_doc_elems)
    {
        AXIS2_ARRAY_LIST_FREE(documentable_impl->f_doc_elems, env);
        documentable_impl->f_doc_elems = NULL;
    }

    if (documentable_impl->wsdl_obj)
    {
        WODEN_WSDL_OBJ_FREE(documentable_impl->wsdl_obj, env);
        documentable_impl->wsdl_obj = NULL;
    }

    if (documentable_impl->super)
    {
        axis2_hash_free(documentable_impl->super, env);
        documentable_impl->super = NULL;
    }

    woden_documentable_free_ops(documentable, env);

    if (documentable_impl->documentable.ops)
    {
        AXIS2_FREE(env->allocator, documentable_impl->documentable.ops);
        documentable_impl->documentable.ops = NULL;
    }

    if (documentable_impl)
    {
        AXIS2_FREE(env->allocator, documentable_impl);
        documentable_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_documentable_super_objs(
    void *documentable,
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentable_impl = INTF_TO_IMPL(documentable);

    return documentable_impl->super;
}

woden_wsdl_obj_t *AXIS2_CALL
woden_documentable_get_base_impl(
    void *documentable,
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentable_impl = INTF_TO_IMPL(documentable);

    return documentable_impl->wsdl_obj;
}

axis2_status_t AXIS2_CALL
woden_documentable_resolve_methods(
    woden_documentable_t *documentable,
    const axis2_env_t *env,
    woden_documentable_t *documentable_impl,
    axis2_hash_t *methods)
{
    woden_documentable_impl_t *documentable_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    documentable_impl_l = INTF_TO_IMPL(documentable_impl);

    documentable->ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_ops_t));

    documentable->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    documentable->ops->to_documentable_free = axis2_hash_get(methods,
            "to_documentable_free", AXIS2_HASH_KEY_STRING);
    documentable->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);

    documentable->ops->add_documentation_element = axis2_hash_get(methods,
            "add_documentation_element", AXIS2_HASH_KEY_STRING);
    if (!documentable->ops->add_documentation_element && documentable_impl_l)
        documentable->ops->add_documentation_element =
            documentable_impl_l->documentable.ops->add_documentation_element;

    documentable->ops->get_documentation_elements = axis2_hash_get(methods,
            "get_documentation_elements", AXIS2_HASH_KEY_STRING);
    if (!documentable->ops->get_documentation_elements && documentable_impl_l)
        documentable->ops->get_documentation_elements =
            documentable_impl_l->documentable.ops->get_documentation_elements;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_documentable_add_documentation_element(
    void *documentable,
    const axis2_env_t *env,
    woden_documentation_element_t *documentation)
{
    woden_documentable_impl_t *documentable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, documentation, AXIS2_FAILURE);
    super = WODEN_DOCUMENTABLE_SUPER_OBJS(documentable, env);
    documentable_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DOCUMENTABLE", AXIS2_HASH_KEY_STRING));

    if (!documentable_impl->f_doc_elems)
    {
        documentable_impl->f_doc_elems = axis2_array_list_create(env, 0);
        if (!documentable_impl->f_doc_elems)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(documentable_impl->f_doc_elems, env,
            documentation);
}

axis2_array_list_t *AXIS2_CALL
woden_documentable_get_documentation_elements(
    void *documentable,
    const axis2_env_t *env)
{
    woden_documentable_impl_t *documentable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DOCUMENTABLE_SUPER_OBJS(documentable, env);
    documentable_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DOCUMENTABLE", AXIS2_HASH_KEY_STRING));


    return documentable_impl->f_doc_elems;
}

