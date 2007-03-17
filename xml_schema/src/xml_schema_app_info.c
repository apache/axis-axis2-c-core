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

#include <xml_schema_app_info.h>
#include <xml_schema_use.h>
#include <axis2_utils.h>
#include <xml_schema_constants.h>

typedef struct xml_schema_app_info_impl
            xml_schema_app_info_impl_t;

/**
 * @brief xml_schema_app_info impl struct
 *   Axis2 Other Extension
 */
struct xml_schema_app_info_impl
{
    xml_schema_app_info_t app_info;

    xml_schema_obj_t *schema_obj;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    axis2_char_t *source;

    void *markup; /* TODO Replace (void *) with node list */
};

#define AXIS2_INTF_TO_IMPL(app_info) \
        ((xml_schema_app_info_impl_t *) app_info)

axis2_status_t AXIS2_CALL
xml_schema_app_info_free(
    void *app_info,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_app_info_super_objs(
    void *app_info,
    const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_app_info_get_type(
    void *app_info,
    const axis2_env_t *env);

xml_schema_obj_t *AXIS2_CALL
xml_schema_app_info_get_base_impl(void *app_info,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
xml_schema_app_info_get_source(void *app_info,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_app_info_set_source(void *app_info,
        const axis2_env_t *env,
        axis2_char_t *source);

/* TODO replace (void *) mark up with node list */
void *AXIS2_CALL
xml_schema_app_info_get_markup(void *app_info,
        const axis2_env_t *env);

/* TODO replace (void *) mark up with node list */
axis2_status_t AXIS2_CALL
xml_schema_app_info_set_markup(void *app_info,
        const axis2_env_t *env,
        void *markup);
/****************** end macros ***********************************************/

AXIS2_EXTERN xml_schema_app_info_t * AXIS2_CALL
xml_schema_app_info_create(const axis2_env_t *env)
{
    xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    app_info_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_app_info_impl_t));
    if (!app_info_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    app_info_impl->schema_obj = NULL;
    app_info_impl->app_info.base.ops = NULL;
    app_info_impl->app_info.ops = NULL;
    app_info_impl->obj_type = XML_SCHEMA_APP_INFO;
    app_info_impl->ht_super = NULL;
    app_info_impl->source = NULL;
    app_info_impl->markup = NULL;

    app_info_impl->app_info.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_app_info_ops_t));

    if (!app_info_impl->app_info.ops)
    {
        xml_schema_app_info_free(&(app_info_impl->app_info), env);
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    app_info_impl->app_info.ops->free =
        xml_schema_app_info_free;

    app_info_impl->app_info.ops->super_objs =
        xml_schema_app_info_super_objs;

    app_info_impl->app_info.ops->get_type =
        xml_schema_app_info_get_type;

    app_info_impl->app_info.ops->get_base_impl =
        xml_schema_app_info_get_base_impl;

    app_info_impl->app_info.ops->get_source =
        xml_schema_app_info_get_source;

    app_info_impl->app_info.ops->set_source =
        xml_schema_app_info_set_source;

    app_info_impl->app_info.ops->get_markup =
        xml_schema_app_info_get_markup;

    app_info_impl->app_info.ops->set_markup =
        xml_schema_app_info_set_markup;

    app_info_impl->schema_obj = xml_schema_obj_create(env);
    if (!app_info_impl->schema_obj)
    {
        xml_schema_app_info_free(&(app_info_impl->app_info), env);
        return NULL;
    }

    app_info_impl->ht_super = axis2_hash_make(env);

    if (!app_info_impl->ht_super)
    {
        xml_schema_app_info_free(&(app_info_impl->app_info), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(app_info_impl->ht_super,
            axis2_strdup("XML_SCHEMA_APP_INFO", env),
            AXIS2_HASH_KEY_STRING, &(app_info_impl->app_info));

    axis2_hash_set(app_info_impl->ht_super,
            axis2_strdup("XML_SCHEMA_OBJ", env),
            AXIS2_HASH_KEY_STRING, app_info_impl->schema_obj);

    status = xml_schema_obj_resolve_methods(&(app_info_impl->app_info.base),
            env, app_info_impl->schema_obj,
            xml_schema_app_info_super_objs,
            xml_schema_app_info_get_type,
            xml_schema_app_info_free);

    return &(app_info_impl->app_info);
}

axis2_status_t AXIS2_CALL
xml_schema_app_info_free(void *app_info,
        const axis2_env_t *env)
{
    xml_schema_app_info_impl_t *app_info_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    app_info_impl = AXIS2_INTF_TO_IMPL(app_info);

    if (app_info_impl->source)
    {
        AXIS2_FREE(env->allocator, app_info_impl->source);
        app_info_impl->source = NULL;
    }

    /* TODO Free markup */

    if (app_info_impl->ht_super)
    {
        axis2_hash_free(app_info_impl->ht_super, env);
        app_info_impl->ht_super = NULL;
    }

    if (app_info_impl->schema_obj)
    {
        XML_SCHEMA_OBJ_FREE(app_info_impl->schema_obj, env);
        app_info_impl->schema_obj = NULL;
    }

    if (app_info_impl->app_info.ops)
    {
        AXIS2_FREE(env->allocator, app_info_impl->app_info.ops);
        app_info_impl->app_info.ops = NULL;
    }
    if (app_info_impl->app_info.base.ops)
    {
        AXIS2_FREE(env->allocator, app_info_impl->app_info.base.ops);
        app_info_impl->app_info.base.ops = NULL;
    }
    if (app_info_impl)
    {
        AXIS2_FREE(env->allocator, app_info_impl);
        app_info_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_app_info_super_objs(
    void *app_info,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(app_info)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_app_info_get_type(
    void *app_info,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(app_info)->obj_type;
}

xml_schema_obj_t *AXIS2_CALL
xml_schema_app_info_get_base_impl(
    void *app_info,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(app_info)->schema_obj;
}

axis2_char_t *AXIS2_CALL
xml_schema_app_info_get_source(void *app_info,
        const axis2_env_t *env)
{

    xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    if (ht_super)
    {
        app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if (app_info_impl)
            return  app_info_impl->source;
    }

    return NULL;
}

axis2_status_t AXIS2_CALL
xml_schema_app_info_set_source(void *app_info,
        const axis2_env_t *env,
        axis2_char_t *source)
{
    xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, source, AXIS2_FAILURE);

    ht_super = XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    if (ht_super)
    {
        app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if (app_info_impl)
            return  AXIS2_FAILURE;
    }

    if (app_info_impl->source)
    {
        AXIS2_FREE(env->allocator, app_info_impl->source);
        app_info_impl->source = NULL;
    }

    app_info_impl->source = axis2_strdup(source, env);
    if (!app_info_impl->source)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* TODO replace (void *) mark up with node list */
void *AXIS2_CALL
xml_schema_app_info_get_markup(void *app_info,
        const axis2_env_t *env)
{
    xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    if (ht_super)
    {
        app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if (app_info_impl)
            return  NULL;
    }
    return app_info_impl->markup;
}

/* TODO replace (void *) mark up with node list */
axis2_status_t AXIS2_CALL
xml_schema_app_info_set_markup(void *app_info,
        const axis2_env_t *env,
        void *markup)
{
    xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, markup, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    if (ht_super)
    {
        app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if (app_info_impl)
            return  AXIS2_FAILURE;
    }
    app_info_impl->markup = markup;
    return AXIS2_SUCCESS;
}
