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

#include <woden_wsdl_ref.h>
#include <woden_component_exts.h>
#include <woden_desc_element.h>

typedef struct woden_wsdl_ref_impl woden_wsdl_ref_impl_t;

/**
 * @brief Wsdl Ref Struct Impl
 *   Axis2 Wsdl Ref
 */
struct woden_wsdl_ref_impl
{
    woden_wsdl_ref_t wsdl_ref;
    woden_wsdl_element_t *wsdl_element;
    axutil_hash_t *super;
    woden_obj_types_t obj_type;
    axutil_array_list_t *f_documentation_elements;
    axutil_uri_t *f_location;
    woden_desc_element_t *f_desc_element;
};

#define INTF_TO_IMPL(wsdl_ref) ((woden_wsdl_ref_impl_t *) wsdl_ref)

axis2_status_t AXIS2_CALL
woden_wsdl_ref_free(
    void *wsdl_ref,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_wsdl_ref_super_objs(
    void *wsdl_ref,
    const axutil_env_t *env);

woden_wsdl_element_t *AXIS2_CALL
woden_wsdl_ref_get_base_impl(
    void *wsdl_ref,
    const axutil_env_t *env);

axutil_uri_t *AXIS2_CALL
woden_wsdl_ref_get_location(
    void *wsdl_ref,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_wsdl_ref_set_location(
    void *wsdl_ref,
    const axutil_env_t *env,
    axutil_uri_t *loc_uri);

void *AXIS2_CALL
woden_wsdl_ref_get_desc_element(
    void *wsdl_ref,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_wsdl_ref_set_desc_element(
    void *wsdl_ref,
    const axutil_env_t *env,
    void *desc);

axis2_status_t AXIS2_CALL
woden_wsdl_ref_add_documentation_element(
    void *wsdl_ref,
    const axutil_env_t *env,
    void *doc_el);

axutil_array_list_t *AXIS2_CALL
woden_wsdl_ref_get_documentation_elements(
    void *wsdl_ref,
    const axutil_env_t *env);

static woden_wsdl_ref_t *
create(
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_ref_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl_ref_impl_t));

    wsdl_ref_impl->wsdl_element = NULL;
    wsdl_ref_impl->f_documentation_elements = NULL;
    wsdl_ref_impl->f_location = NULL;
    wsdl_ref_impl->f_desc_element = NULL;

    wsdl_ref_impl->wsdl_ref.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl_ref_ops_t));

    wsdl_ref_impl->wsdl_ref.ops->free = woden_wsdl_ref_free;
    wsdl_ref_impl->wsdl_ref.ops->super_objs = woden_wsdl_ref_super_objs;
    wsdl_ref_impl->wsdl_ref.ops->get_base_impl = woden_wsdl_ref_get_base_impl;
    wsdl_ref_impl->wsdl_ref.ops->get_location =
        woden_wsdl_ref_get_location;
    wsdl_ref_impl->wsdl_ref.ops->set_location =
        woden_wsdl_ref_set_location;
    wsdl_ref_impl->wsdl_ref.ops->get_desc_element =
        woden_wsdl_ref_get_desc_element;
    wsdl_ref_impl->wsdl_ref.ops->set_desc_element =
        woden_wsdl_ref_set_desc_element;
    wsdl_ref_impl->wsdl_ref.ops->add_documentation_element =
        woden_wsdl_ref_add_documentation_element;
    wsdl_ref_impl->wsdl_ref.ops->get_documentation_elements =
        woden_wsdl_ref_get_documentation_elements;

    return &(wsdl_ref_impl->wsdl_ref);
}

AXIS2_EXTERN woden_wsdl_ref_t * AXIS2_CALL
woden_wsdl_ref_create(
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_ref_impl = (woden_wsdl_ref_impl_t *) create(env);

    wsdl_ref_impl->wsdl_element = woden_wsdl_element_create(env);

    wsdl_ref_impl->super = axutil_hash_make(env);
    if (!wsdl_ref_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(wsdl_ref_impl->super, "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING,
            &(wsdl_ref_impl->wsdl_ref));
    axutil_hash_set(wsdl_ref_impl->super, "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING,
            wsdl_ref_impl->wsdl_element);
    return &(wsdl_ref_impl->wsdl_ref);
}

axis2_status_t AXIS2_CALL
woden_wsdl_ref_free(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_ref_impl = INTF_TO_IMPL(wsdl_ref);

    if (wsdl_ref_impl->wsdl_element)
    {
        WODEN_WSDL_ELEMENT_FREE(wsdl_ref_impl->wsdl_element, env);
        wsdl_ref_impl->wsdl_element = NULL;
    }

    if (wsdl_ref_impl->super)
    {
        axutil_hash_free(wsdl_ref_impl->super, env);
        wsdl_ref_impl->super = NULL;
    }

    if ((&(wsdl_ref_impl->wsdl_ref))->ops)
    {
        AXIS2_FREE(env->allocator, (&(wsdl_ref_impl->wsdl_ref))->ops);
        (&(wsdl_ref_impl->wsdl_ref))->ops = NULL;
    }

    if (wsdl_ref_impl)
    {
        AXIS2_FREE(env->allocator, wsdl_ref_impl);
        wsdl_ref_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_wsdl_ref_super_objs(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_ref_impl = INTF_TO_IMPL(wsdl_ref);

    return wsdl_ref_impl->super;
}

axutil_hash_t *AXIS2_CALL
woden_wsdl_ref_super(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_ref_impl = INTF_TO_IMPL(wsdl_ref);

    return wsdl_ref_impl->super;
}

woden_wsdl_element_t *AXIS2_CALL
woden_wsdl_ref_get_base_impl(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_ref_impl = INTF_TO_IMPL(wsdl_ref);

    return wsdl_ref_impl->wsdl_element;
}

axis2_status_t AXIS2_CALL
woden_wsdl_ref_resolve_methods(
    woden_wsdl_ref_t *wsdl_ref,
    const axutil_env_t *env,
    woden_wsdl_ref_t *wsdl_ref_impl,
    axutil_hash_t *methods)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    wsdl_ref_impl_l = INTF_TO_IMPL(wsdl_ref_impl);

    wsdl_ref->ops->free = axutil_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    wsdl_ref->ops->super_objs = axutil_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    wsdl_ref->ops->type = axutil_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    wsdl_ref->ops->get_location = axutil_hash_get(methods,
            "get_location", AXIS2_HASH_KEY_STRING);
    if (!wsdl_ref->ops->get_location && wsdl_ref_impl_l)
        wsdl_ref->ops->get_location =
            wsdl_ref_impl_l->wsdl_ref.ops->get_location;

    wsdl_ref->ops->set_location = axutil_hash_get(methods,
            "set_location", AXIS2_HASH_KEY_STRING);
    if (!wsdl_ref->ops->set_location && wsdl_ref_impl_l)
        wsdl_ref->ops->set_location =
            wsdl_ref_impl_l->wsdl_ref.ops->set_location;

    wsdl_ref->ops->get_desc_element = axutil_hash_get(methods,
            "get_desc_element", AXIS2_HASH_KEY_STRING);
    if (!wsdl_ref->ops->get_desc_element && wsdl_ref_impl_l)
        wsdl_ref->ops->get_desc_element =
            wsdl_ref_impl_l->wsdl_ref.ops->get_desc_element;

    wsdl_ref->ops->set_desc_element = axutil_hash_get(methods,
            "set_desc_element", AXIS2_HASH_KEY_STRING);
    if (!wsdl_ref->ops->set_desc_element && wsdl_ref_impl_l)
        wsdl_ref->ops->set_desc_element =
            wsdl_ref_impl_l->wsdl_ref.ops->set_desc_element;

    wsdl_ref->ops->add_documentation_element = axutil_hash_get(methods,
            "add_documentation_element", AXIS2_HASH_KEY_STRING);
    if (!wsdl_ref->ops->add_documentation_element && wsdl_ref_impl_l)
        wsdl_ref->ops->add_documentation_element =
            wsdl_ref_impl_l->wsdl_ref.ops->add_documentation_element;

    wsdl_ref->ops->get_documentation_elements = axutil_hash_get(methods,
            "get_documentation_elements", AXIS2_HASH_KEY_STRING);
    if (!wsdl_ref->ops->get_documentation_elements && wsdl_ref_impl_l)
        wsdl_ref->ops->get_documentation_elements =
            wsdl_ref_impl_l->wsdl_ref.ops->get_documentation_elements;



    return AXIS2_SUCCESS;
}

axutil_uri_t *AXIS2_CALL
woden_wsdl_ref_get_location(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env);
    wsdl_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING));

    return wsdl_ref_impl->f_location;
}

axis2_status_t AXIS2_CALL
woden_wsdl_ref_set_location(
    void *wsdl_ref,
    const axutil_env_t *env,
    axutil_uri_t *loc_uri)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env);
    wsdl_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING));

    if (wsdl_ref_impl->f_location)
    {
        /* TODO */
    }
    wsdl_ref_impl->f_location = loc_uri;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_wsdl_ref_get_desc_element(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env);
    wsdl_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING));

    return wsdl_ref_impl->f_desc_element;
}

axis2_status_t AXIS2_CALL
woden_wsdl_ref_set_desc_element(
    void *wsdl_ref,
    const axutil_env_t *env,
    void *desc)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env);
    wsdl_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING));

    if (wsdl_ref_impl->f_desc_element)
    {
        /* TODO */
    }
    wsdl_ref_impl->f_desc_element = desc;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_wsdl_ref_add_documentation_element(
    void *wsdl_ref,
    const axutil_env_t *env,
    void *doc_el)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env);
    wsdl_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING));

    if (!wsdl_ref_impl->f_documentation_elements)
    {
        wsdl_ref_impl->f_documentation_elements = axutil_array_list_create(env, 0);
        if (!wsdl_ref_impl->f_documentation_elements)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return axutil_array_list_add(wsdl_ref_impl->f_documentation_elements, env,
            doc_el);
}

axutil_array_list_t *AXIS2_CALL
woden_wsdl_ref_get_documentation_elements(
    void *wsdl_ref,
    const axutil_env_t *env)
{
    woden_wsdl_ref_impl_t *wsdl_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env);
    wsdl_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL_REF", AXIS2_HASH_KEY_STRING));

    return wsdl_ref_impl->f_documentation_elements;
}

