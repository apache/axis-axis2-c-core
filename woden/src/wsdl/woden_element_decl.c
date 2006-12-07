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

#include <woden_element_decl.h>
#include <axis2_uri.h>
#include <axis2_hash.h>

typedef struct woden_element_decl_impl woden_element_decl_impl_t;

/**
 * @brief Element Declaration Struct Impl
 *   Axis2 Element Declaration
 */
struct woden_element_decl_impl
{
    woden_element_decl_t decl;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;

    axis2_qname_t *f_qname;
    axis2_uri_t *f_system;
    axis2_char_t *f_content_model;
    axis2_generic_obj_t *f_content;
};

#define INTF_TO_IMPL(decl) \
    ((woden_element_decl_impl_t *) decl)

axis2_status_t AXIS2_CALL
woden_element_decl_free(
    void *decl,
    const axis2_env_t *envv);

axis2_hash_t *AXIS2_CALL
woden_element_decl_super_objs(
    void *decl,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_element_decl_type(
    void *decl,
    const axis2_env_t *envv);

/* ************************************************************
 *  Element Declaration interface methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
woden_element_decl_get_qname(
    void *decl,
    const axis2_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_element_decl_get_system(
    void *decl,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_element_decl_get_content_model(
    void *decl,
    const axis2_env_t *env);

axis2_generic_obj_t *AXIS2_CALL
woden_element_decl_get_content(
    void *decl,
    const axis2_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_element_decl_set_qname(
    void *decl,
    const axis2_env_t *env,
    axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_element_decl_set_system(
    void *decl,
    const axis2_env_t *env,
    axis2_uri_t *type_system_uri);

axis2_status_t AXIS2_CALL
woden_element_decl_set_content_model(
    void *decl,
    const axis2_env_t *env,
    axis2_char_t *content_model);

axis2_status_t AXIS2_CALL
woden_element_decl_set_content(
    void *decl,
    const axis2_env_t *env,
    axis2_generic_obj_t *element_content);


AXIS2_EXTERN woden_element_decl_t * AXIS2_CALL
woden_element_decl_create(
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    decl_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_element_decl_impl_t));

    decl_impl->obj_type = WODEN_ELEMENT_DECL;
    decl_impl->super = NULL;
    decl_impl->f_qname = NULL;
    decl_impl->f_system = NULL;
    decl_impl->f_content_model = NULL;
    decl_impl->f_content = NULL;

    decl_impl->decl.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_decl_ops_t));

    decl_impl->decl.ops->free =
        woden_element_decl_free;
    decl_impl->decl.ops->super_objs =
        woden_element_decl_super_objs;
    decl_impl->decl.ops->type =
        woden_element_decl_type;

    decl_impl->decl.ops->get_qname =
        woden_element_decl_get_qname;
    decl_impl->decl.ops->get_system =
        woden_element_decl_get_system;
    decl_impl->decl.ops->get_content_model =
        woden_element_decl_get_content_model;
    decl_impl->decl.ops->get_content =
        woden_element_decl_get_content;
    decl_impl->decl.ops->set_qname =
        woden_element_decl_set_qname;
    decl_impl->decl.ops->set_system =
        woden_element_decl_set_system;
    decl_impl->decl.ops->set_content_model =
        woden_element_decl_set_content_model;
    decl_impl->decl.ops->set_content =
        woden_element_decl_set_content;

    decl_impl->super = axis2_hash_make(env);
    if (!decl_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(decl_impl->super, "WODEN_ELEMENT_DECL",
            AXIS2_HASH_KEY_STRING, &(decl_impl->decl));

    return &(decl_impl->decl);
}

axis2_status_t AXIS2_CALL
woden_element_decl_free(void *decl,
        const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    decl_impl = INTF_TO_IMPL(decl);

    if (decl_impl->super)
    {
        axis2_hash_free(decl_impl->super, env);
        decl_impl->super = NULL;
    }

    if ((&(decl_impl->decl))->ops)
    {
        AXIS2_FREE(env->allocator, (&(decl_impl->decl))->ops);
        (&(decl_impl->decl))->ops = NULL;
    }

    if (decl_impl)
    {
        AXIS2_FREE(env->allocator, decl_impl);
        decl_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_element_decl_super_objs(
    void *decl,
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    decl_impl = INTF_TO_IMPL(decl);

    return decl_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_element_decl_type(
    void *decl,
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    decl_impl = INTF_TO_IMPL(decl);

    return decl_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_element_decl_resolve_methods(
    woden_element_decl_t *decl,
    const axis2_env_t *env,
    woden_element_decl_t *decl_impl,
    axis2_hash_t *methods)
{
    woden_element_decl_impl_t *decl_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    decl_impl_l = INTF_TO_IMPL(decl_impl);

    decl->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    decl->ops->to_element_decl_free = axis2_hash_get(methods,
            "to_element_decl_free", AXIS2_HASH_KEY_STRING);
    decl->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    decl->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    decl->ops->get_qname = axis2_hash_get(methods,
            "get_qname", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->get_qname && decl_impl_l)
        decl->ops->get_qname =
            decl_impl_l->decl.ops->get_qname;

    decl->ops->get_system = axis2_hash_get(methods,
            "get_system", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->get_system && decl_impl_l)
        decl->ops->get_system =
            decl_impl_l->decl.ops->get_system;

    decl->ops->get_content_model = axis2_hash_get(methods,
            "get_content_model", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->get_content_model && decl_impl_l)
        decl->ops->get_content_model =
            decl_impl_l->decl.ops->get_content_model;

    decl->ops->get_content = axis2_hash_get(methods,
            "get_content", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->get_content && decl_impl_l)
        decl->ops->get_content =
            decl_impl_l->decl.ops->get_content;

    decl->ops->set_qname = axis2_hash_get(methods,
            "set_qname", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->set_qname && decl_impl_l)
        decl->ops->set_qname =
            decl_impl_l->decl.ops->set_qname;

    decl->ops->set_system = axis2_hash_get(methods,
            "set_system", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->set_system && decl_impl_l)
        decl->ops->set_system =
            decl_impl_l->decl.ops->set_system;

    decl->ops->set_content_model = axis2_hash_get(methods,
            "set_content_model", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->set_content_model && decl_impl_l)
        decl->ops->set_content_model =
            decl_impl_l->decl.ops->set_content_model;

    decl->ops->set_content = axis2_hash_get(methods,
            "set_content", AXIS2_HASH_KEY_STRING);
    if (!decl->ops->set_content && decl_impl_l)
        decl->ops->set_content =
            decl_impl_l->decl.ops->set_content;

    return AXIS2_SUCCESS;
}
/* ************************************************************
 *  Element Declaration interface methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
woden_element_decl_get_qname(
    void *decl,
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    return decl_impl->f_qname;
}

axis2_uri_t *AXIS2_CALL
woden_element_decl_get_system(
    void *decl,
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    return decl_impl->f_system;
}

axis2_char_t *AXIS2_CALL
woden_element_decl_get_content_model(
    void *decl,
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    return decl_impl->f_content_model;
}

axis2_generic_obj_t *AXIS2_CALL
woden_element_decl_get_content(
    void *decl,
    const axis2_env_t *env)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    return decl_impl->f_content;
}




/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_element_decl_set_qname(
    void *decl,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    if (decl_impl->f_qname)
    {
        AXIS2_QNAME_FREE(decl_impl->f_qname, env);
        decl_impl->f_qname = NULL;
    }
    decl_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_element_decl_set_system(
    void *decl,
    const axis2_env_t *env,
    axis2_uri_t *type_system_uri)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_system_uri, AXIS2_FAILURE);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    if (decl_impl->f_system)
    {
        AXIS2_URI_FREE(decl_impl->f_system, env);
    }
    decl_impl->f_system = type_system_uri;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_element_decl_set_content_model(
    void *decl,
    const axis2_env_t *env,
    axis2_char_t *content_model)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content_model, AXIS2_FAILURE);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    if (decl_impl->f_content_model)
    {
        AXIS2_FREE(env->allocator, decl_impl->f_content_model);
        decl_impl->f_content_model = NULL;
    }
    decl_impl->f_content_model = AXIS2_STRDUP(content_model, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_element_decl_set_content(
    void *decl,
    const axis2_env_t *env,
    axis2_generic_obj_t *element_content)
{
    woden_element_decl_impl_t *decl_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_content, AXIS2_FAILURE);
    super = WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env);
    decl_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_ELEMENT_DECL", AXIS2_HASH_KEY_STRING));

    if (decl_impl->f_content)
    {
        /* TODO */
    }
    decl_impl->f_content = element_content;
    return AXIS2_SUCCESS;
}





