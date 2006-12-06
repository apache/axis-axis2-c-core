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

#include <woden_type_def.h>
#include <woden_ext_element.h>
#include <woden_xml_attr.h>
#include <woden_generic_obj.h>

typedef struct woden_type_def_impl woden_type_def_impl_t;

/**
 * @brief Type Definition Struct Impl
 *   Axis2 Type Definition
 */
struct woden_type_def_impl
{
    woden_type_def_t type_def;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_qname;
    axis2_uri_t *f_system;
    axis2_char_t *f_content_model;
    axis2_generic_obj_t *f_content;
};

#define INTF_TO_IMPL(type_def) ((woden_type_def_impl_t *) type_def)

axis2_status_t AXIS2_CALL
woden_type_def_free(
    void *type_def,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_type_def_super_objs(
    void *type_def,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_type_def_type(
    void *type_def,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_type_def_set_qname(
    void *type_def,
    const axis2_env_t *env,
    axis2_qname_t *qname);

axis2_qname_t *AXIS2_CALL
woden_type_def_get_qname(
    void *type_def,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_type_def_set_system(
    void *type_def,
    const axis2_env_t *env,
    axis2_uri_t *type_system_uri);

axis2_uri_t *AXIS2_CALL
woden_type_def_get_system(
    void *type_def,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_type_def_set_content_model(
    void *type_def,
    const axis2_env_t *env,
    axis2_char_t *content_model);

axis2_char_t *AXIS2_CALL
woden_type_def_get_content_model(
    void *type_def,
    const axis2_env_t *env);


axis2_status_t AXIS2_CALL
woden_type_def_set_content(
    void *type_def,
    const axis2_env_t *env,
    void *type_def_content);

void *AXIS2_CALL
woden_type_def_get_content(
    void *type_def,
    const axis2_env_t *env);

static woden_type_def_t *
create(const axis2_env_t *env);

static woden_type_def_t *
create(const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    type_def_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_type_def_impl_t));

    type_def_impl->super = NULL;
    type_def_impl->methods = NULL;
    type_def_impl->obj_type = WODEN_TYPE_DEF;
    type_def_impl->f_qname = NULL;
    type_def_impl->f_system = NULL;
    type_def_impl->f_content_model = NULL;
    type_def_impl->f_content = NULL;

    type_def_impl->type_def.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_type_def_ops_t));

    type_def_impl->type_def.ops->free = woden_type_def_free;
    type_def_impl->type_def.ops->super_objs =
        woden_type_def_super_objs;
    type_def_impl->type_def.ops->type = woden_type_def_type;

    type_def_impl->type_def.ops->set_qname = woden_type_def_set_qname;
    type_def_impl->type_def.ops->set_system = woden_type_def_set_system;
    type_def_impl->type_def.ops->set_content_model =
        woden_type_def_set_content_model;
    type_def_impl->type_def.ops->set_content =
        woden_type_def_set_content;

    type_def_impl->methods = axis2_hash_make(env);
    if (!type_def_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(type_def_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_type_def_free);
    axis2_hash_set(type_def_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            woden_type_def_super_objs);
    axis2_hash_set(type_def_impl->methods, "type", AXIS2_HASH_KEY_STRING,
            woden_type_def_type);

    axis2_hash_set(type_def_impl->methods, "set_qname",
            AXIS2_HASH_KEY_STRING, woden_type_def_set_qname);
    axis2_hash_set(type_def_impl->methods, "get_qname",
            AXIS2_HASH_KEY_STRING, woden_type_def_get_qname);
    axis2_hash_set(type_def_impl->methods, "set_system",
            AXIS2_HASH_KEY_STRING, woden_type_def_set_system);
    axis2_hash_set(type_def_impl->methods, "get_system",
            AXIS2_HASH_KEY_STRING, woden_type_def_get_system);
    axis2_hash_set(type_def_impl->methods, "set_content_model",
            AXIS2_HASH_KEY_STRING, woden_type_def_set_content_model);
    axis2_hash_set(type_def_impl->methods, "get_content_model",
            AXIS2_HASH_KEY_STRING, woden_type_def_get_content_model);
    axis2_hash_set(type_def_impl->methods, "set_content",
            AXIS2_HASH_KEY_STRING, woden_type_def_set_content);
    axis2_hash_set(type_def_impl->methods, "get_content",
            AXIS2_HASH_KEY_STRING, woden_type_def_get_content);

    return &(type_def_impl->type_def);
}

AXIS2_EXTERN woden_type_def_t * AXIS2_CALL
woden_type_def_create(
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    type_def_impl = (woden_type_def_impl_t *) create(env);

    type_def_impl->super = axis2_hash_make(env);
    if (!type_def_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(type_def_impl->super, "WODEN_TYPE_DEF",
            AXIS2_HASH_KEY_STRING, &(type_def_impl->type_def));

    return &(type_def_impl->type_def);
}

axis2_status_t AXIS2_CALL
woden_type_def_free(
    void *type_def,
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    type_def_impl = INTF_TO_IMPL(type_def);

    if (type_def_impl->f_content)
    {
        /*AXIS2_FREE(type_def_impl->f_content, env);*/
        type_def_impl->f_content = NULL;
    }

    if (type_def_impl->super)
    {
        axis2_hash_free(type_def_impl->super, env);
        type_def_impl->super = NULL;
    }

    if (type_def_impl->methods)
    {
        axis2_hash_free(type_def_impl->methods, env);
        type_def_impl->methods = NULL;
    }

    if ((&(type_def_impl->type_def))->ops)
    {
        AXIS2_FREE(env->allocator, type_def_impl->type_def.ops);
        (&(type_def_impl->type_def))->ops = NULL;
    }

    if (type_def_impl)
    {
        AXIS2_FREE(env->allocator, type_def_impl);
        type_def_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_type_def_super_objs(
    void *type_def,
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    type_def_impl = INTF_TO_IMPL(type_def);

    return type_def_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_type_def_type(void *type_def,
        const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    type_def_impl = INTF_TO_IMPL(type_def);

    return type_def_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_type_def_resolve_methods(
    woden_type_def_t *type_def,
    const axis2_env_t *env,
    woden_type_def_t *type_def_impl,
    axis2_hash_t *methods)
{
    woden_type_def_impl_t *type_def_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    type_def_impl_l = INTF_TO_IMPL(type_def_impl);

    type_def->ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_type_def_ops_t));

    type_def->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    type_def->ops->super_objs = axis2_hash_get(methods,
            "super_objs", AXIS2_HASH_KEY_STRING);
    type_def->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    type_def->ops->set_qname = axis2_hash_get(methods,
            "set_qname", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->set_qname && type_def_impl_l)
        type_def->ops->set_qname =
            type_def_impl_l->type_def.ops->set_qname;

    type_def->ops->get_qname = axis2_hash_get(methods,
            "get_qname", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->get_qname && type_def_impl_l)
        type_def->ops->get_qname =
            type_def_impl_l->type_def.ops->get_qname;

    type_def->ops->set_system = axis2_hash_get(methods,
            "set_system", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->set_system && type_def_impl_l)
        type_def->ops->set_system =
            type_def_impl_l->type_def.ops->set_system;

    type_def->ops->get_system = axis2_hash_get(methods,
            "get_system", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->get_system && type_def_impl_l)
        type_def->ops->get_system =
            type_def_impl_l->type_def.ops->get_system;

    type_def->ops->set_content_model = axis2_hash_get(methods,
            "set_content_model", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->set_content_model && type_def_impl_l)
        type_def->ops->set_content_model =
            type_def_impl_l->type_def.ops->set_content_model;

    type_def->ops->get_content_model = axis2_hash_get(methods,
            "get_content_model", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->get_content_model && type_def_impl_l)
        type_def->ops->get_content_model =
            type_def_impl_l->type_def.ops->get_content_model;

    type_def->ops->set_content = axis2_hash_get(methods,
            "set_content", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->set_content && type_def_impl_l)
        type_def->ops->set_content =
            type_def_impl_l->type_def.ops->set_content;

    type_def->ops->get_content = axis2_hash_get(methods,
            "get_content", AXIS2_HASH_KEY_STRING);
    if (!type_def->ops->get_content && type_def_impl_l)
        type_def->ops->get_content =
            type_def_impl_l->type_def.ops->get_content;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_type_def_set_qname(
    void *type_def,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    if (type_def_impl->f_qname)
    {
        AXIS2_QNAME_FREE(type_def_impl->f_qname, env);
        type_def_impl->f_qname = NULL;
    }
    type_def_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_type_def_get_qname(
    void *type_def,
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    return type_def_impl->f_qname;
}

axis2_status_t AXIS2_CALL
woden_type_def_set_system(
    void *type_def,
    const axis2_env_t *env,
    axis2_uri_t *type_system_uri)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_system_uri, AXIS2_FAILURE);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    if (type_def_impl->f_system)
    {
        /* Free f_content */
    }
    type_def_impl->f_system = type_system_uri;
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_type_def_get_system(
    void *type_def,
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    return type_def_impl->f_system;
}

axis2_status_t AXIS2_CALL
woden_type_def_set_content_model(
    void *type_def,
    const axis2_env_t *env,
    axis2_char_t *content_model)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content_model, AXIS2_FAILURE);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    if (type_def_impl->f_content_model)
    {
        /* Free f_content */
    }
    type_def_impl->f_content_model = content_model;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
woden_type_def_get_content_model(
    void *type_def,
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    return type_def_impl->f_content_model;
}

axis2_status_t AXIS2_CALL
woden_type_def_set_content(
    void *type_def,
    const axis2_env_t *env,
    void *type_def_content)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_def, AXIS2_FAILURE);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    if (type_def_impl->f_content)
    {
        /* Free f_content */
    }
    type_def_impl->f_content = type_def_content;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_type_def_get_content(
    void *type_def,
    const axis2_env_t *env)
{
    woden_type_def_impl_t *type_def_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_TYPE_DEF_SUPER_OBJS(type_def, env);
    type_def_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_TYPE_DEF", AXIS2_HASH_KEY_STRING));

    return type_def_impl->f_content;
}

